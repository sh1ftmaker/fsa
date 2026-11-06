#!/usr/bin/env python3
"""Report game units whose compiled function order differs from the original address order.

Game code is built with -sym off, so a unit's .text must emit its strong functions in
the original address order. It is also built with -inline auto,deferred, which emits
functions in REVERSE source order: a game .cpp lists its functions in DESCENDING address
order (highest at the top; docs/INLINING.md). This checks the compiled object, so it is
the same test either way; for a unit out of order it prints the expected .text order and
the source order that gives it. Usage: python3 tools/order_check.py [unit...]
(unit = game/X.cpp or X; default: every src/game/*.cpp). Exit 1 if any unit is out of order.
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
NM = ROOT / "build/binutils/powerpc-eabi-nm"


def split_ranges():
    out, cur = {}, None
    for l in (ROOT / "config/G4SE01/splits.txt").read_text().splitlines():
        if l and not l[0].isspace():
            cur = l.rstrip(":")
        elif cur:
            m = re.match(r"\s+\.text\s+start:0x([0-9A-Fa-f]+)\s+end:0x([0-9A-Fa-f]+)", l)
            if m:
                out[cur] = (int(m.group(1), 16), int(m.group(2), 16))
    return out


def symbols():
    syms = []
    for l in (ROOT / "config/G4SE01/symbols.txt").read_text().splitlines():
        m = re.match(r"(\S+) = \.text:0x([0-9A-Fa-f]+);", l)
        if m:
            syms.append((m.group(1), int(m.group(2), 16)))
    return syms


def check(unit, ranges, syms):
    """Return (compiled_seq, expected_seq) when out of order, else None."""
    lo, hi = ranges.get(unit, (0, 0))
    addr = {n: a for n, a in syms if lo <= a < hi}
    obj = ROOT / "build/G4SE01/src" / unit.replace(".cpp", ".o")
    if not obj.exists():
        return None
    nm = subprocess.run([str(NM), "-n", "--defined-only", str(obj)], capture_output=True, text=True).stdout.splitlines()
    seq = [p[2] for p in (l.split(None, 2) for l in nm) if len(p) == 3 and p[1] in "tT" and p[2] in addr]
    exp = sorted(seq, key=lambda n: addr[n])
    return None if seq == exp else (seq, exp)


def main():
    ranges, syms = split_ranges(), symbols()
    args = sys.argv[1:]
    if args:
        units = [a if a.endswith(".cpp") else a + ".cpp" for a in args]
        units = [u if "/" in u else "game/" + u for u in units]
    else:
        units = sorted(str(p.relative_to(ROOT / "src")) for p in (ROOT / "src/game").glob("*.cpp"))
    bad = 0
    for u in units:
        r = check(u, ranges, syms)
        if r:
            bad += 1
            seq, exp = r
            first = next(i for i, (a, b) in enumerate(zip(seq, exp)) if a != b)
            print(f"{u}: out of order at #{first} (compiled {seq[first]}, expected {exp[first]})")
            print("  expected .text order: " + " ".join(exp))
            print("  source order (descending, top of file first): " + " ".join(reversed(exp)))
    print(f"order_check: {bad} unit(s) out of order")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
