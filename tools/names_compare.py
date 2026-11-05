#!/usr/bin/env python3
"""Compare two honest objdiff reports function by function, keyed by ADDRESS
(names differ after tools/rename.py). Prints every function whose match
percentage dropped, and totals. Exit 1 on any drop.

Usage: python3 tools/names_compare.py BEFORE.json AFTER.json
"""
import json
import sys


def funcs(path):
    out = {}
    for u in json.load(open(path)).get("units", []):
        for f in u.get("functions", []) or []:
            meta = f.get("metadata") or {}
            addr = meta.get("virtual_address")
            if addr is None:
                continue
            out[int(addr)] = (f.get("fuzzy_match_percent", 0.0), f.get("name"), u["name"])
    return out


def main(a, b):
    x, y = funcs(a), funcs(b)
    drops = [(k, x[k], y.get(k)) for k in sorted(x) if k in y and y[k][0] + 1e-9 < x[k][0]]
    missing = [k for k in x if k not in y]
    full = lambda d: sum(1 for v in d.values() if v[0] >= 100.0)
    for k, o, n in drops:
        print(f"DROP 0x{k:08X} {o[1]} {o[0]:.2f} -> {n[1]} {n[0]:.2f} ({o[2]})")
    for k in missing[:20]:
        print(f"MISSING 0x{k:08X} {x[k][1]} ({x[k][2]})")
    print(f"functions: {len(x)} -> {len(y)}; 100 %: {full(x)} -> {full(y)}; drops {len(drops)}; missing {len(missing)}")
    return 1 if drops or missing else 0


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:3]))
