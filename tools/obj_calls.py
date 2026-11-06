#!/usr/bin/env python3
"""List an object's .text functions (offset, size) and the functions each one calls (R_PPC_REL24).

Usage: python3 tools/obj_calls.py <object.o> [name-substring]
  e.g. python3 tools/obj_calls.py build/G4SE01/src/game/tu_8022AD64.o fn_80232A0C

Shows what MWCC inlined (a callee missing from the list) or called out of line,
and where out-of-line copies of inline functions were placed. Compare with the
target's `bl` lines in build/G4SE01/asm/game/<unit>.s. See docs/INLINING.md.
"""
import collections
import struct
import sys


def main():
    d = open(sys.argv[1], 'rb').read()
    shoff, = struct.unpack('>I', d[0x20:0x24])
    shnum, shstrndx = struct.unpack('>HH', d[0x30:0x34])
    sh = [struct.unpack('>IIIIIIIIII', d[shoff + i * 40:shoff + i * 40 + 40]) for i in range(shnum)]

    def name(off, base):
        return d[base + off:d.index(b'\0', base + off)].decode('latin1')

    secn = [name(s[0], sh[shstrndx][4]) for s in sh]
    symtab = [s for s in sh if s[1] == 2][0]
    strtab = sh[symtab[6]][4]
    syms = []
    for o in range(symtab[4], symtab[4] + symtab[5], 16):
        nm, val, sz, info, _, shn = struct.unpack('>IIIBBH', d[o:o + 16])
        syms.append((name(nm, strtab), val, sz, info & 15, shn))
    flt = sys.argv[2] if len(sys.argv) > 2 else ''
    for t in [i for i, n in enumerate(secn) if n == '.text']:
        funcs = sorted([s for s in syms if s[4] == t and s[3] == 2], key=lambda s: s[1])
        calls = collections.defaultdict(list)
        for r in sh:
            if r[1] != 4 or r[7] != t:
                continue
            for o in range(r[4], r[4] + r[5], 12):
                off, info, _ = struct.unpack('>IIi', d[o:o + 12])
                if info & 0xff == 10:  # R_PPC_REL24
                    f = [fn for fn in funcs if fn[1] <= off < fn[1] + fn[2]]
                    calls[f[0][0] if f else '?'].append(syms[info >> 8][0])
        for fn in funcs:
            if flt and flt not in fn[0]:
                continue
            c = collections.Counter(calls[fn[0]])
            print(f"{fn[1]:5x} {fn[2]:5x} {fn[0]}: " + ', '.join(f"{k} x{v}" if v > 1 else k for k, v in c.items()))


if __name__ == '__main__':
    main()
