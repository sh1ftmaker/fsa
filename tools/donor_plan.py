#!/usr/bin/env python3
"""Turn donor_match.py results into per-file .text split ranges.

For each donor object, takes the FSA addresses of its uniquely matched
functions, builds the [first, last_end) range, and reports how many of the FSA
functions inside that range the donor accounts for. Overlaps with existing
splits.txt stanzas are flagged (same-name stanzas are expected and replaced).

Usage:
  python3 tools/donor_plan.py MATCH_JSON [--min-coverage 1.0] [--splits out.txt]
"""

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from donor_match import FSA_OBJ_DIR, current_objects, fsa_address, load_functions, split_text_bases  # noqa: E402

SPLITS = Path("config/G4SE01/splits.txt")


def parse_splits():
    stanzas = {}
    cur = None
    for line in SPLITS.read_text().splitlines():
        m = re.match(r"^(\S.*):$", line)
        if m and not line.startswith("Sections"):
            cur = m.group(1)
            stanzas[cur] = {}
            continue
        m = re.match(r"^\s+(\S+)\s+start:0x([0-9A-Fa-f]+) end:0x([0-9A-Fa-f]+)", line)
        if m and cur:
            stanzas[cur][m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
    return stanzas


def fsa_function_table():
    """All FSA .text functions as {addr: (size, unit)}."""
    bases = split_text_bases()
    table = {}
    for obj in current_objects():
        unit = str(obj.relative_to(FSA_OBJ_DIR))[:-2]
        for name, sec, off, size, _, _ in load_functions(obj):
            addr = fsa_address(unit, name, off, bases) if sec == ".text" else None
            if addr is not None:
                table[addr] = (size, unit)
    return table


def largest_cluster(addrs, table, max_gap_fns=4):
    """Split matched (addr, size) pairs where more than max_gap_fns unmatched
    FSA functions sit between neighbours; return the cluster with most bytes."""
    addrs = sorted(addrs)
    starts = sorted(table)
    import bisect
    clusters, cur = [], [addrs[0]]
    for prev, nxt in zip(addrs, addrs[1:]):
        lo = bisect.bisect_left(starts, prev[0] + prev[1])
        hi = bisect.bisect_left(starts, nxt[0])
        if hi - lo > max_gap_fns:
            clusters.append(cur)
            cur = []
        cur.append(nxt)
    clusters.append(cur)
    return max(clusters, key=lambda c: sum(s for _, s in c))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("match_json", type=Path)
    ap.add_argument("--min-coverage", type=float, default=1.0)
    ap.add_argument("--splits", type=Path, help="write proposed .text stanzas here")
    args = ap.parse_args()

    stanzas = parse_splits()
    table = fsa_function_table()
    rows = json.loads(args.match_json.read_text())

    by_obj = {}
    for r in rows:
        by_obj.setdefault(r["donor_obj"], []).append(r)

    plan = []
    for obj, fns in sorted(by_obj.items()):
        addrs = []
        for f in fns:
            if len(f["hits"]) == 1 and f["hits"][0]["addr"] is not None:
                addrs.append((f["hits"][0]["addr"], f["size"]))
        # Ambiguous hits count toward coverage if one candidate is in range.
        if not addrs:
            continue
        addrs = largest_cluster(addrs, table)
        start = min(a for a, _ in addrs)
        end = max(a + s for a, s in addrs)
        claimed = {a for a, _ in addrs}
        for f in fns:
            if len(f["hits"]) > 1:
                for h in f["hits"]:
                    if h["addr"] is not None and start <= h["addr"] < end:
                        claimed.add(h["addr"])
        in_range = [a for a in table if start <= a < end]
        covered = sum(1 for a in in_range if a in claimed)
        cov = covered / len(in_range) if in_range else 0
        src = obj[:-2]  # donor objects are named <source>.o, e.g. foo.cpp.o
        if not src.endswith((".c", ".cpp", ".cp", ".s")):
            src += ".c"
        overlaps = []
        for name, secs in stanzas.items():
            t = secs.get(".text")
            if t and t[0] < end and start < t[1] and name != src:
                overlaps.append(name)
        plan.append((src, start, end, covered, len(in_range), cov, overlaps))

    for src, start, end, covered, total, cov, overlaps in plan:
        flag = "OK " if cov >= args.min_coverage and not overlaps else "-- "
        ov = f"  OVERLAPS {overlaps}" if overlaps else ""
        print(f"{flag}{src:44} 0x{start:08X}-0x{end:08X}  {covered:3}/{total:3} fns{ov}")

    if args.splits:
        out = []
        for src, start, end, covered, total, cov, overlaps in plan:
            if cov >= args.min_coverage and not overlaps:
                out.append(f"{src}:\n\t.text       start:0x{start:08X} end:0x{end:08X}\n")
        args.splits.write_text("\n".join(out))
        print(f"\nWrote {len(out)} stanzas to {args.splits}")


if __name__ == "__main__":
    sys.exit(main())
