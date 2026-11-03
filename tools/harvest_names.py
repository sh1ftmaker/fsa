#!/usr/bin/env python3
"""Harvest placeholder names from the linked ELF with `dtk dol apply`.

`dtk dol apply` writes every name of a linked ELF back into symbols.txt. On
its own that is mostly noise here (scope changes, `@etb_` and `...data.0`
renames, guesses from dtk's own split objects). This wrapper runs it on a
scratch copy of the config and keeps one kind of change only:

  a placeholder (`fn_`, `lbl_`, `*_80xxxxxx`) renamed to a real name that a
  Matching unit's compiled object defines at exactly that address, with the
  same size as the placeholder;

  or a placeholder that dtk guessed too large, split into the real symbols
  that a Matching object defines there, when they tile it exactly (from its
  address to its end, with only alignment padding between them).

"At that address" means: the address lies in the unit's split range for the
symbol's section, and range start + the symbol's offset in the object's
section equals the placeholder's address. Matching units link byte-identical,
so that is evidence from the bytes. Real symbols are never touched, and no
other symbols are added or removed.

Usage (after a full `ninja` that ended in SHA-1 OK):
  python3 tools/harvest_names.py            # list what would change
  python3 tools/harvest_names.py --write    # apply it to symbols.txt
Then `rm build/G4SE01/config.json` and rebuild.
"""

import argparse
import json
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from data_splits import Elf  # noqa: E402

CONFIG = Path("config/G4SE01/config.yml")
SYMBOLS = Path("config/G4SE01/symbols.txt")
SPLITS = Path("config/G4SE01/splits.txt")
ELF = Path("build/G4SE01/fsa_game_code.elf")
DTK = Path("build/tools/dtk")

PLACEHOLDER = re.compile(r"^(fn_|lbl_)|_80[0-9A-Fa-f]{6}$")
NOT_REAL = re.compile(r"^(fn_|lbl_|auto_|func_|jumptable_|@|\.\.\.)|_80[0-9A-Fa-f]{6}$|^__unknown")
LINE = re.compile(r"^(\S+) = (\S+):0x([0-9A-Fa-f]+); // (.*)$")
SHN_UNDEF, SHN_COMMON = 0, 0xFFF2


def parse_symbols(text):
    """[(name, section, address, size, line)] in file order."""
    out = []
    for line in text.splitlines():
        m = LINE.match(line)
        if m:
            size = re.search(r"size:0x([0-9A-Fa-f]+)", m.group(4))
            out.append((m.group(1), m.group(2), int(m.group(3), 16), int(size.group(1), 16) if size else None, line))
    return out


def parse_splits():
    """unit stem -> {section: start of its first (non-common) range}."""
    units, cur = {}, None
    for line in SPLITS.read_text().splitlines():
        if line and not line[0].isspace() and line.endswith(":") and line != "Sections:":
            cur = units.setdefault(line[:-1].rsplit(".", 1)[0], {})
        elif cur is not None and line.startswith("\t"):
            m = re.match(r"\t(\S+)\s+start:0x(\w+) end:0x(\w+)(.*)", line)
            if m and "common" not in m.group(4):
                cur.setdefault(m.group(1), (int(m.group(2), 16), int(m.group(3), 16)))
    return units


def matching_definitions():
    """(name, section, address) -> (size, unit) for every symbol a Matching unit's object defines."""
    splits = parse_splits()
    out = {}
    for unit in json.loads(Path("objdiff.json").read_text())["units"]:
        meta = unit.get("metadata") or {}
        base = unit.get("base_path")
        if not meta.get("complete") or not base or not Path(base).exists():
            continue
        stem = unit["name"].split("/", 1)[1]
        ranges = splits.get(stem, {})
        elf = Elf(Path(base))
        for name, value, size, _bind, _type, shndx in elf.symbols:
            if not name or shndx in (SHN_UNDEF, SHN_COMMON) or shndx >= len(elf.sections):
                continue
            section = elf.sections[shndx]["name"]
            if section not in ranges:
                continue
            start, end = ranges[section]
            addr = start + value
            if start <= addr < end:
                out[(name, section, addr)] = (size, stem)
    return out


def run_apply():
    """symbols.txt as `dtk dol apply` would rewrite it, from a scratch copy."""
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        shutil.copy(SYMBOLS, tmp / "symbols.txt")
        shutil.copy(SPLITS, tmp / "splits.txt")
        config = CONFIG.read_text()
        config = re.sub(r"(?m)^splits: .*$", f"splits: {tmp / 'splits.txt'}", config)
        config = re.sub(r"(?m)^symbols: .*$", f"symbols: {tmp / 'symbols.txt'}", config)
        config = re.sub(r"(?m)^object_base: (.*)$", lambda m: f"object_base: {Path(m.group(1)).resolve()}", config)
        (tmp / "config.yml").write_text(config)
        out = subprocess.run([str(DTK), "dol", "apply", str(tmp / "config.yml"), str(ELF)],
                             capture_output=True, text=True)
        if out.returncode != 0:
            sys.exit("dtk dol apply failed:\n" + out.stdout[-2000:] + out.stderr[-2000:])
        return (tmp / "symbols.txt").read_text()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--write", action="store_true", help="apply the accepted renames to symbols.txt")
    ap.add_argument("-v", "--verbose", action="store_true", help="also list rejected placeholder renames")
    args = ap.parse_args()

    old_text = SYMBOLS.read_text()
    old = parse_symbols(old_text)
    new = parse_symbols(run_apply())
    defs = matching_definitions()
    known = {s[0] for s in old}

    new_at = {}
    for name, section, addr, size, line in new:
        new_at.setdefault((section, addr), []).append((name, size, line))

    by_unit = {}
    for (n, sec, a), (sz, unit) in defs.items():
        by_unit.setdefault((sec, unit), []).append((a, n, sz))

    def tile(section, addr, size, unit):
        """The object's symbols that exactly cover [addr, addr + size), or None."""
        parts = sorted(x for x in by_unit.get((section, unit), [])
                       if addr <= x[0] < addr + size and not NOT_REAL.search(x[1]))
        pos = addr
        for a, n, sz in parts:
            if a < pos or a - pos >= 8 or (a - pos and a % 4):
                return None
            pos = a + sz
        return parts if parts and parts[0][0] == addr and 0 <= addr + size - pos < 8 else None

    accepted, rejected = {}, []
    for name, section, addr, size, line in old:
        if not PLACEHOLDER.search(name):
            continue
        for new_name, new_size, new_line in new_at.get((section, addr), []):
            if NOT_REAL.search(new_name) or new_name in known:
                continue
            why = None
            d = defs.get((new_name, section, addr))
            if d is None:
                why = "no Matching object defines it there"
            elif d[0] != size:
                parts = tile(section, addr, size or 0, d[1]) if size and d[0] < size else None
                if parts and all(n not in known for _, n, _ in parts):
                    lines = []
                    for a, n, sz in parts:
                        own = [l for nn, _, l in new_at.get((section, a), []) if nn == n]
                        lines.append(own[0] if own else
                                     f"{n} = {section}:0x{a:08X}; // type:object size:0x{sz:X}")
                    accepted[line] = ("\n".join(lines), d[1])
                    continue
                why = f"size 0x{d[0]:X} in {d[1]}, placeholder 0x{size or 0:X}, no exact tiling"
            if why:
                rejected.append(f"{name} -> {new_name} ({section}:0x{addr:08X}): {why}")
                continue
            if line in accepted:
                rejected.append(f"{name} -> {new_name}: second name at one address")
                continue
            # dtk's line, with the placeholder's own size and type kept.
            replacement = line.replace(name + " = ", new_name + " = ", 1)
            scope = re.search(r"scope:\w+", new_line)
            if scope and "scope:" not in replacement:
                replacement += " " + scope.group(0)
            accepted[line] = (replacement, d[1])

    for line, (replacement, unit) in accepted.items():
        names = ", ".join(r.split(" = ")[0] for r in replacement.split("\n"))
        print(f"{line.split(' = ')[0]:<24} -> {names:<40} {unit}")
    print(f"{len(accepted)} placeholder(s) named, {len(rejected)} candidate(s) rejected", file=sys.stderr)
    if args.verbose:
        for r in rejected:
            print("  rejected:", r, file=sys.stderr)

    if args.write and accepted:
        lines = [accepted[l][0] if l in accepted else l for l in old_text.split("\n")]
        SYMBOLS.write_text("\n".join(lines))
        print(f"wrote {SYMBOLS}; now rm build/G4SE01/config.json and rebuild", file=sys.stderr)


if __name__ == "__main__":
    main()
