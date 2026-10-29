#!/usr/bin/env python3
"""Write donor symbol names into symbols.txt for verified function matches.

For every donor function that donor_match.py matched to exactly one FSA
function start, this names the FSA function after the donor function, then
walks both functions' relocations in lockstep to name the code/data symbols
they reference. Conflicting proposals (one address, two names, or one name,
two addresses) are dropped. Existing symbols.txt entries that collide by name
or address with an accepted proposal are removed.

Usage:
  python3 tools/donor_symbols.py MATCH_JSON DONOR_OBJ_ROOT [--write]
"""

import argparse
import bisect
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

sys.path.insert(0, str(Path(__file__).parent))
from donor_match import FSA_OBJ_DIR  # noqa: E402

SYMBOLS = Path("config/G4SE01/symbols.txt")
LINE_RE = re.compile(r"^(\S+) = (\.\w+):0x([0-9A-Fa-f]+);(.*)$")
DTK_AUTO_RE = re.compile(r"^(?:fn|lbl|jumptable)_[0-9A-Fa-f]{8}$")
AUTO_RE = re.compile(r"^(?:fn|lbl|jumptable|\w+?)_([0-9A-Fa-f]{8})$")


def obj_info(path):
    """Return (functions{name: (sec, off, size)}, relocs{sec: {off: (sym, type, addend)}}, symbols{name: info})."""
    with open(path, "rb") as f:
        elf = ELFFile(f)
        sections = list(elf.iter_sections())
        symtab = elf.get_section_by_name(".symtab")
        syms = list(symtab.iter_symbols())
        funcs, syminfo = {}, {}
        for s in syms:
            shndx = s["st_shndx"]
            sec = sections[shndx].name if isinstance(shndx, int) else None
            syminfo[s.name] = {
                "type": s["st_info"]["type"],
                "bind": s["st_info"]["bind"],
                "sec": sec,
                "value": s["st_value"],
                "size": s["st_size"],
            }
            if s["st_info"]["type"] == "STT_FUNC" and s["st_size"]:
                funcs[s.name] = (sec, s["st_value"], s["st_size"])
        relocs = defaultdict(dict)
        for sec in sections:
            if isinstance(sec, RelocationSection):
                target = sections[sec["sh_info"]].name
                for r in sec.iter_relocations():
                    s = syms[r["r_info_sym"]]
                    name = s.name
                    if s["st_info"]["type"] == "STT_SECTION":
                        name = None  # anonymous section-relative reference
                    relocs[target][r["r_offset"] & ~3] = (name, r["r_info_type"], r["r_addend"])
        return funcs, relocs, syminfo


def load_symbols():
    entries = []
    for line in SYMBOLS.read_text().splitlines():
        m = LINE_RE.match(line)
        if m:
            entries.append({"name": m.group(1), "sec": m.group(2), "addr": int(m.group(3), 16),
                            "rest": m.group(4), "line": line})
        else:
            entries.append({"line": line})
    return entries


def section_ranges():
    """FSA section address ranges, from dtk's auto unit names and splits.txt."""
    ranges = defaultdict(lambda: [1 << 32, 0])
    config = json.loads(Path("build/G4SE01/config.json").read_text())
    for u in config["units"]:
        m = re.match(r"auto_\d+_([0-9A-F]{8})_(\w+)$", u["name"])
        if m:
            sec = "." + m.group(2) if m.group(2) not in ("extab", "extabindex") else m.group(2)
            a = int(m.group(1), 16)
            ranges[sec][0] = min(ranges[sec][0], a)
            ranges[sec][1] = max(ranges[sec][1], a + 1)
    for line in Path("config/G4SE01/splits.txt").read_text().splitlines():
        m = re.match(r"\s+(\S+)\s+start:0x(\w+) end:0x(\w+)", line)
        if m:
            r = ranges[m.group(1)]
            r[0] = min(r[0], int(m.group(2), 16))
            r[1] = max(r[1], int(m.group(3), 16))
    # Sections are laid out in order; each runs until the next one starts.
    order = sorted(ranges, key=lambda k: ranges[k][0])
    for cur, nxt in zip(order, order[1:]):
        ranges[cur][1] = max(ranges[cur][1], ranges[nxt][0])
    ranges[order[-1]][1] = max(ranges[order[-1]][1], 0x81800000)
    return ranges


def section_of(addr, ranges):
    hits = [s for s, (lo, hi) in ranges.items() if lo <= addr < hi]
    return hits[0] if len(hits) == 1 else None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("match_json", type=Path)
    ap.add_argument("donor_root", type=Path)
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()

    entries = load_symbols()
    ranges = section_ranges()
    proposals = defaultdict(set)  # addr -> {(name, sec, kind, size)}
    by_name = defaultdict(set)
    cache = {}

    def info(path):
        if path not in cache:
            cache[path] = obj_info(path)
        return cache[path]

    text_ranges = {}
    cur = None
    for line in Path("config/G4SE01/splits.txt").read_text().splitlines():
        if line.endswith(":") and not line.startswith(("\t", " ", "Sections")):
            cur = line[:-1].rsplit(".", 1)[0]
        m = re.match(r"\s+\.text\s+start:0x(\w+) end:0x(\w+)", line)
        if m and cur:
            text_ranges[cur] = (int(m.group(1), 16), int(m.group(2), 16))

    for row in json.loads(args.match_json.read_text()):
        if len(row["hits"]) != 1 or row["hits"][0]["addr"] is None:
            continue
        hit = row["hits"][0]
        fsa_addr = hit["addr"]
        # Only trust a function match inside the donor file's own split range.
        rng = text_ranges.get(row["donor_obj"].rsplit(".", 1)[0])
        if rng is None or not rng[0] <= fsa_addr < rng[1]:
            continue
        d_funcs, d_rel, d_syms = info(args.donor_root / row["donor_obj"])
        t_funcs, t_rel, _ = info(FSA_OBJ_DIR / (hit["obj"] + ".o"))
        dsec, doff, dsize = d_funcs[row["func"]]
        tsec, toff, _ = t_funcs[hit["name"]]
        scope = "local" if d_syms[row["func"]]["bind"] == "STB_LOCAL" else "global"
        proposals[fsa_addr].add((row["func"], ".text", "function", dsize, scope))
        for o in range(0, dsize, 4):
            d = d_rel[dsec].get(doff + o)
            t = t_rel[tsec].get(toff + o)
            if not d or not t or d[0] is None or t[0] is None:
                continue
            m = AUTO_RE.match(t[0])
            if not m:
                continue
            target = int(m.group(1), 16) + t[2] - d[2]
            ds = d_syms.get(d[0], {})
            if ds.get("sec") is None and ds.get("type") == "STT_NOTYPE":
                continue  # undefined in donor object: name only, no size/kind
            kind = "function" if ds.get("type") == "STT_FUNC" else "object"
            sec = ".text" if kind == "function" else section_of(target, ranges)
            if not sec or d[0].startswith("@"):
                continue
            scope = "local" if ds.get("bind") == "STB_LOCAL" else "global"
            proposals[target].add((d[0], sec, kind, ds.get("size", 0), scope))

    for addr, props in proposals.items():
        for p in props:
            by_name[p[0]].add(addr)
    accepted = {}
    for addr, props in proposals.items():
        names = {p[0] for p in props}
        if len(names) != 1:
            continue
        name = next(iter(names))
        if len(by_name[name]) != 1:
            continue
        accepted[addr] = max(props, key=lambda p: p[3])

    # A sized symbol must not overlap other symbols: drop dtk's auto labels
    # inside it, and drop the proposal if a real named symbol is in the way.
    starts = sorted(e["addr"] for e in entries if "addr" in e)
    by_addr = defaultdict(list)
    for e in entries:
        if "addr" in e:
            by_addr[e["addr"]].append(e)
    inner = set()
    for addr, (name, sec, kind, size, scope) in list(accepted.items()):
        lo, hi = bisect.bisect_right(starts, addr), bisect.bisect_left(starts, addr + size)
        inside = [e for a in starts[lo:hi] for e in by_addr[a] if a not in accepted]
        if any(not DTK_AUTO_RE.match(e["name"]) for e in inside):
            del accepted[addr]
            continue
        inner.update(id(e) for e in inside)

    names = {p[0] for p in accepted.values()}
    kept, removed = [], 0
    for e in entries:
        if "addr" in e and (e["addr"] in accepted or e["name"] in names or id(e) in inner):
            removed += 1
            continue
        kept.append(e["line"])
    for addr, (name, sec, kind, size, scope) in sorted(accepted.items()):
        size_s = f" size:0x{size:X}" if size else ""
        kept.append(f"{name} = {sec}:0x{addr:08X}; // type:{kind}{size_s} scope:{scope}")

    n_fn = sum(1 for p in accepted.values() if p[2] == "function")
    print(f"accepted {len(accepted)} symbols ({n_fn} functions, {len(accepted) - n_fn} data); "
          f"removed {removed} colliding entries; dropped {len(proposals) - len(accepted)} conflicting proposals")
    if args.write:
        # dtk wants symbols sorted by section order then address; keep it simple: sort by address.
        def key(line):
            m = LINE_RE.match(line)
            return int(m.group(3), 16) if m else -1
        SYMBOLS.write_text("\n".join(sorted(kept, key=key)) + "\n")


if __name__ == "__main__":
    sys.exit(main())
