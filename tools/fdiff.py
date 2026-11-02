#!/usr/bin/env python3
"""Show how one unit or one function differs from the original.

Uses objdiff-cli with the same relocation setting as the progress report, so
"100 %" here means the function counts as matched.

Usage:
  python3 tools/fdiff.py <unit>              # every function in the unit, with match %
  python3 tools/fdiff.py <unit> <function>   # differing instructions, target | ours

<unit> is a source path such as dolphin/os/OS.c (or the objdiff unit name).
Build the unit first (ninja builds it as part of build/G4SE01/report.json).
"""

import json
import subprocess
import sys

OBJDIFF = "build/tools/objdiff-cli"
CONFIG = "functionRelocDiffs=data_value"


def unit_name(arg):
    with open("objdiff.json") as f:
        units = json.load(f)["units"]
    for u in units:
        source = (u.get("metadata") or {}).get("source_path") or ""
        if u["name"] == arg or source.endswith("/" + arg) or source == arg:
            return u["name"]
    sys.exit(f"no unit matches {arg!r}")


def run_diff(unit, symbol=None):
    cmd = [OBJDIFF, "diff", "-p", ".", "-u", unit, "-o", "-", "-c", CONFIG]
    if symbol:
        cmd.append(symbol)
    out = subprocess.run(cmd, capture_output=True, text=True)
    if out.returncode != 0:
        sys.exit(out.stderr.strip() or "objdiff-cli failed")
    return json.loads(out.stdout)


def functions(side):
    return {s["name"]: s for s in side["symbols"] if s.get("kind") == "SYMBOL_FUNCTION"}


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    unit = unit_name(sys.argv[1])
    if len(sys.argv) == 2:
        diff = run_diff(unit)
        for section in diff["left"]["sections"]:
            if section.get("kind") not in ("SECTION_CODE", "SECTION_DATA", "SECTION_BSS"):
                continue
            print(f"{section['name']:10} {section.get('match_percent', 0):7.2f} %")
        target = functions(diff["left"])
        for name, sym in sorted(target.items(), key=lambda kv: int(kv[1].get("address", 0))):
            pct = sym.get("match_percent")
            mark = "  " if pct == 100 else "* "
            shown = "missing" if pct is None else f"{pct:.2f} %"
            print(f"{mark}{name:48} {int(sym['size']):6} bytes  {shown}")
        return

    symbol = sys.argv[2]
    diff = run_diff(unit, symbol)
    left = functions(diff["left"]).get(symbol)
    right = functions(diff["right"]).get(symbol)
    if left is None:
        sys.exit(f"{symbol} not in the target object")
    print(f"{symbol}: {left.get('match_percent', 0):.2f} %")
    if right is None:
        print("(not in our object yet)")
        return
    rows = zip(left.get("instructions", []), right.get("instructions", []))
    for i, (a, b) in enumerate(rows):
        if not a.get("diff_kind") and not b.get("diff_kind"):
            continue
        fa = (a.get("instruction") or {}).get("formatted", "")
        fb = (b.get("instruction") or {}).get("formatted", "")
        kind = (a.get("diff_kind") or b.get("diff_kind")).replace("DIFF_", "").lower()
        print(f"{i * 4:5X}  {fa:40} | {fb:40} {kind}")


if __name__ == "__main__":
    main()
