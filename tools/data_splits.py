#!/usr/bin/env python3
"""Find and check the data-section ranges of split units.

For every unit whose .text is already split, this reads the unit's compiled
object (objdiff.json base_path) and locates each of its data sections in
main.dol:

  * by name: each named symbol in the section votes for a base address
    (FSA address in symbols.txt minus the symbol's offset in the object);
  * by content: an initialized section whose symbols are all anonymous
    (string and float literals) is searched for in its DOL section, with
    relocated words treated as wildcards; only a unique hit counts.

A candidate is accepted only if the object's initialized bytes equal the
DOL's at that address (relocated words excepted), and if it doesn't overlap
another unit's split. Accepted ranges are printed, and with --write they
replace that unit's data lines in splits.txt.

Usage:
  python3 tools/data_splits.py [--write] [--only PREFIX ...]
"""

import argparse
import bisect
import json
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

DOL = Path("orig/sys/main.dol")
SPLITS = Path("config/G4SE01/splits.txt")
SYMBOLS = Path("config/G4SE01/symbols.txt")
DATA_SECTIONS = [".ctors", ".dtors", ".rodata", ".data", ".bss", ".sdata", ".sbss", ".sdata2", ".sbss2"]
BSS_SECTIONS = {".bss", ".sbss", ".sbss2"}


class Elf:
    """Minimal big-endian ELF32 reader: sections, symbols, relocations."""

    def __init__(self, path):
        data = path.read_bytes()
        shoff, = struct.unpack_from(">I", data, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)
        headers = [struct.unpack_from(">IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]
        strtab = headers[shstrndx]

        def name_at(table, offset):
            start = table[4] + offset
            return data[start:data.index(b"\0", start)].decode()

        self.sections = []
        for h in headers:
            self.sections.append({
                "name": name_at(strtab, h[0]), "type": h[1], "offset": h[4], "size": h[5],
                "link": h[6], "info": h[7], "align": h[8], "entsize": h[9],
                "data": data[h[4]:h[4] + h[5]] if h[1] != 8 else b"",
            })
        self.symbols = []
        self.relocated = defaultdict(set)  # section index -> offsets of relocated words
        for index, s in enumerate(self.sections):
            if s["type"] == 2:  # SHT_SYMTAB
                names = headers[s["link"]]
                for i in range(s["size"] // 16):
                    st_name, value, size, info, _, shndx = struct.unpack_from(">IIIBBH", data, s["offset"] + i * 16)
                    self.symbols.append((name_at(names, st_name), value, size, info >> 4, info & 0xF, shndx))
            elif s["type"] == 4:  # SHT_RELA
                for i in range(s["size"] // 12):
                    offset, = struct.unpack_from(">I", data, s["offset"] + i * 12)
                    self.relocated[s["info"]].add(offset & ~3)


def read_dol():
    data = DOL.read_bytes()
    offsets = struct.unpack_from(">18I", data, 0)
    addrs = struct.unpack_from(">18I", data, 0x48)
    sizes = struct.unpack_from(">18I", data, 0x90)
    bss_addr, bss_size = struct.unpack_from(">II", data, 0xD8)
    segments = [(a, s, o) for o, a, s in zip(offsets, addrs, sizes) if s]
    return data, segments, (bss_addr, bss_size)


def read_symbols():
    by_name = defaultdict(list)
    by_addr = {}
    pattern = re.compile(r"^(\S+) = (\.?\w+):0x([0-9A-Fa-f]+);")
    for line in SYMBOLS.read_text().splitlines():
        m = pattern.match(line)
        if m:
            name, section, addr = m.group(1), m.group(2), int(m.group(3), 16)
            by_name[name].append((section, addr))
            by_addr[(section, addr)] = name
    return by_name, by_addr


def read_splits():
    """Return (header lines, [(unit, [(section, start, end, extra)])])."""
    units = []
    header = []
    current = None
    in_header = True
    for line in SPLITS.read_text().splitlines():
        if in_header:
            # The "Sections:" block lists section types, not a unit.
            if line and not line[0].isspace() and line.endswith(":") and line != "Sections:":
                in_header = False
            else:
                header.append(line)
                continue
        if line and not line[0].isspace() and line.endswith(":"):
            current = (line[:-1], [])
            units.append(current)
        elif line.strip().startswith(".") or line.strip().startswith("extab"):
            parts = line.split()
            fields = dict(p.split(":", 1) for p in parts[1:] if ":" in p)  # bare flags such as `common`
            extra = " ".join(p for p in parts[1:] if not p.startswith(("start:", "end:")))
            current[1].append((parts[0], int(fields["start"], 16), int(fields["end"], 16), extra))
        elif current is None:
            header.append(line)
    return header, units


def write_splits(header, units):
    out = list(header)
    for unit, sections in units:
        out.append(f"{unit}:")
        for section, start, end, extra in sections:
            tail = f" {extra}" if extra else ""
            out.append(f"\t{section:<11} start:0x{start:08X} end:0x{end:08X}{tail}")
        out.append("")
    SPLITS.write_text("\n".join(out).rstrip("\n") + "\n")


def dol_bytes(dol, segments, addr, size):
    for seg_addr, seg_size, seg_off in segments:
        if seg_addr <= addr and addr + size <= seg_addr + seg_size:
            return dol[seg_off + addr - seg_addr:seg_off + addr - seg_addr + size]
    return None


def matches(obj_bytes, target, relocated):
    if target is None or len(target) != len(obj_bytes):
        return False
    for i in range(0, len(obj_bytes), 4):
        if i in relocated:
            continue
        if obj_bytes[i:i + 4] != target[i:i + 4]:
            return False
    return True


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--only", nargs="*", default=[], help="unit name prefixes")
    args = ap.parse_args()

    dol, segments, _ = read_dol()
    by_name, by_addr = read_symbols()
    header, units = read_splits()
    project = json.loads(Path("objdiff.json").read_text())
    base_paths = {}
    for u in project["units"]:
        source = (u.get("metadata") or {}).get("source_path") or ""
        if u.get("base_path"):
            for root in ("libs/sdk/", "libs/sdk_gx/", "libs/jsystem/", "src/"):
                if source.startswith(root):
                    base_paths[source[len(root):]] = Path(u["base_path"])

    claims = []  # (start, end, unit, section) for every existing data split
    for unit, sections in units:
        for section, start, end, _ in sections:
            if section != ".text":
                claims.append((start, end, unit, section))
    text_start = {unit: start for unit, sections in units for section, start, _, _ in sections if section == ".text"}

    proposals = {}
    sorted_addrs = defaultdict(list)
    for sec, a in by_addr:
        sorted_addrs[sec].append(a)
    for addrs in sorted_addrs.values():
        addrs.sort()
    report = Counter()
    for unit, sections in units:
        if args.only and not unit.startswith(tuple(args.only)):
            continue
        obj_path = base_paths.get(unit)
        if not obj_path or not obj_path.exists():
            report["no object"] += 1
            continue
        elf = Elf(obj_path)
        found = []
        for index, s in enumerate(elf.sections):
            if s["name"] not in DATA_SECTIONS or s["size"] == 0:
                continue
            name = s["name"]
            votes = Counter()
            for sym, value, _, _, typ, shndx in elf.symbols:
                if shndx != index or not sym or sym.startswith("@") or sym.startswith("..."):
                    continue
                for section, addr in by_name.get(sym, []):
                    if section == name:
                        votes[addr - value] += 1
            relocated = elf.relocated.get(index, set())
            by_content = False
            candidates = [base for base, _ in votes.most_common()]
            if not candidates and name not in BSS_SECTIONS:
                # Search the bytes, relocated words as wildcards; link order and
                # overlaps below must leave exactly one hit.
                body = s["data"]
                pattern = b"".join(
                    b"(?s:....)" if i in relocated else re.escape(body[i:i + 4])
                    for i in range(0, len(body), 4)
                )
                for seg_addr, seg_size, seg_off in segments:
                    for m in re.finditer(pattern, dol[seg_off:seg_off + seg_size]):
                        if m.start() % 4 == 0:
                            candidates.append(seg_addr + m.start())
                if len(candidates) > 256:
                    report[f"{name}: ambiguous"] += 1
                    continue
                if not candidates:
                    report[f"{name}: not found"] += 1
                    continue
                by_content = True
            if not candidates:
                report[f"{name}: no named symbols"] += 1
                continue
            named = [(sym, value) for sym, value, _, _, _, shndx in elf.symbols
                     if shndx == index and sym and not sym.startswith(("@", "..."))]
            mine = text_start.get(unit)

            def check(base):
                """Return (end, None) if base is acceptable, else (None, reason)."""
                # Every named symbol must land on the same name, or on an unnamed address.
                for sym, value in named:
                    there = by_addr.get((name, base + value))
                    if there is not None and there != sym and not there.startswith("lbl_"):
                        return None, "names disagree"
                if name not in BSS_SECTIONS and not matches(s["data"], dol_bytes(dol, segments, base, s["size"]), relocated):
                    return None, "bytes differ"
                # The linker pads each section; the split must reach the next symbol.
                end = base + s["size"]
                addrs = sorted_addrs[name]
                i = bisect.bisect_left(addrs, end)
                following = [a for a in addrs[i:i + 8] if a < end + 32 and a % 4 == 0]
                end = following[0] if following else (end + 3) & ~3
                if any(c[2] != unit and c[0] < end and base < c[1] for c in claims):
                    return None, "overlaps"
                # Link order: the unit's data must follow the data of every unit whose
                # code comes first, and precede the data of every unit whose code comes later.
                if mine is not None and any(
                        c[3] == name and c[2] in text_start and c[2] != unit
                        and (text_start[c[2]] < mine) != (c[0] < base) for c in claims):
                    return None, "out of link order"
                return end, None

            results = [(base, *check(base)) for base in candidates]
            valid = [(base, end) for base, end, _ in results if end is not None]
            if by_content and len(valid) > 1:
                report[f"{name}: ambiguous"] += 1
                continue
            if not valid:
                report[f"{name}: {results[0][2]}"] += 1
                continue
            chosen, end = valid[0]
            found.append((name, chosen, end))
            claims.append((chosen, end, unit, name))
        if found:
            proposals[unit] = found

    changed = 0
    for i, (unit, sections) in enumerate(units):
        if unit not in proposals:
            continue
        new = {name: (start, end) for name, start, end in proposals[unit]}
        old = {sec: (start, end) for sec, start, end, _ in sections}
        if all(old.get(k) == v for k, v in new.items()):
            continue
        changed += 1
        print(f"{unit}:")
        for name, (start, end) in new.items():
            before = old.get(name)
            mark = "" if before == (start, end) else (f"  (was 0x{before[0]:08X}-0x{before[1]:08X})" if before else "  (new)")
            print(f"\t{name:<11} start:0x{start:08X} end:0x{end:08X}{mark}")
        kept = [x for x in sections if x[0] == ".text" or x[0] not in new]
        merged = kept + [(name, start, end, "") for name, (start, end) in new.items()]
        order = [".init", "extab", "extabindex", ".text"] + DATA_SECTIONS
        merged.sort(key=lambda x: order.index(x[0]) if x[0] in order else 99)
        units[i] = (unit, merged)

    print(f"\n{changed} units changed; skipped: {dict(report)}", file=sys.stderr)
    if args.write and changed:
        write_splits(header, units)
        trim_auto_symbols([(name, start, end) for found in proposals.values() for name, start, end in found])


def trim_auto_symbols(ranges):
    """Shrink dtk's guessed lbl_ symbols that would straddle a new split boundary."""
    pattern = re.compile(r"^(lbl_\w+) = (\.?\w+):0x([0-9A-Fa-f]+); // (.*?)size:0x([0-9A-Fa-f]+)(.*)$")
    boundaries = defaultdict(set)
    for name, start, end in ranges:
        boundaries[name].update((start, end))
    lines = SYMBOLS.read_text().splitlines()
    trimmed = 0
    for i, line in enumerate(lines):
        m = pattern.match(line)
        if not m:
            continue
        section, addr, size = m.group(2), int(m.group(3), 16), int(m.group(5), 16)
        cuts = [b for b in boundaries.get(section, ()) if addr < b < addr + size]
        if cuts:
            new_size = min(cuts) - addr
            lines[i] = f"{m.group(1)} = {section}:0x{addr:08X}; // {m.group(4)}size:0x{new_size:X}{m.group(6)}"
            trimmed += 1
    SYMBOLS.write_text("\n".join(lines) + "\n")
    print(f"trimmed {trimmed} auto symbols", file=sys.stderr)


if __name__ == "__main__":
    main()
