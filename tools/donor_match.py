#!/usr/bin/env python3
"""Match functions in donor objects against FSA's DOL functions.

Unlike compile_search.py, candidates are only the function starts dtk found
(build/G4SE01/obj/*.o), never arbitrary offsets inside .text, so small
functions can't "match" inside larger ones. A function matches when sizes are
equal, relocations sit at the same offsets, and every word is identical except
the operand bits of relocated instructions.

Usage:
  python3 tools/donor_match.py DONOR_OBJ_ROOT [--json out.json]

Requires pyelftools. Results still need objdiff verification after splitting.
"""

import argparse
import hashlib
import json
import sys
from collections import defaultdict
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

FSA_OBJ_DIR = Path("build/G4SE01/obj")


def load_functions(path):
    """Yield (name, section, offset, size, bytes, reloc_offsets) for each function."""
    with open(path, "rb") as f:
        elf = ELFFile(f)
        sections = list(elf.iter_sections())
        relocs = defaultdict(set)
        for sec in sections:
            if isinstance(sec, RelocationSection):
                target = sections[sec["sh_info"]].name
                for r in sec.iter_relocations():
                    relocs[target].add(r["r_offset"] & ~3)
        symtab = elf.get_section_by_name(".symtab")
        if symtab is None:
            return
        for sym in symtab.iter_symbols():
            if sym["st_info"]["type"] != "STT_FUNC" or sym["st_size"] == 0:
                continue
            shndx = sym["st_shndx"]
            if not isinstance(shndx, int):
                continue
            sec = sections[shndx]
            if sec["sh_type"] != "SHT_PROGBITS":
                continue
            data = sec.data()
            off, size = sym["st_value"], sym["st_size"]
            rel = frozenset(o - off for o in relocs[sec.name] if off <= o < off + size)
            yield sym.name, sec.name, off, size, data[off:off + size], rel


def masked_key(code, rel):
    out = bytearray(code)
    for o in rel:
        # Keep the 6-bit primary opcode, drop the relocated operand bits.
        out[o] &= 0xFC
        out[o + 1:o + 4] = b"\0\0\0"
    return hashlib.sha1(bytes(out) + repr(sorted(rel)).encode()).hexdigest()


def current_objects():
    """Target objects of the current dtk split (the obj dir keeps stale ones)."""
    config = json.loads(Path("build/G4SE01/config.json").read_text())
    return sorted(Path(u["object"]) for u in config["units"] if Path(u["object"]).exists())


def split_text_bases():
    """Map unit path (no extension) -> .text start address from splits.txt."""
    bases = {}
    cur = None
    for line in Path("config/G4SE01/splits.txt").read_text().splitlines():
        if line.endswith(":") and not line.startswith(("\t", " ", "Sections")):
            cur = line[:-1].rsplit(".", 1)[0]
        elif cur and line.strip().startswith(".text"):
            bases[cur] = int(line.split("start:")[1].split()[0], 16)
    return bases


def fsa_address(unit, fn_name, off, bases):
    # dtk names unknown functions fn_<addr>; split units use their .text base.
    if fn_name.startswith("fn_"):
        return int(fn_name[3:], 16)
    if unit in bases:
        return bases[unit] + off
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("donor_root", type=Path)
    ap.add_argument("--json", type=Path)
    args = ap.parse_args()

    bases = split_text_bases()
    fsa = defaultdict(list)
    for obj in current_objects():
        unit = str(obj.relative_to(FSA_OBJ_DIR))[:-2]
        for name, sec, off, size, code, rel in load_functions(obj):
            if sec != ".text":
                continue
            fsa[(size, masked_key(code, rel))].append(
                {"obj": unit, "name": name, "addr": fsa_address(unit, name, off, bases)}
            )

    results = []
    for obj in sorted(args.donor_root.rglob("*.o")):
        for name, sec, off, size, code, rel in load_functions(obj):
            if sec != ".text":
                continue
            hits = fsa.get((size, masked_key(code, rel)), [])
            results.append({
                "donor_obj": str(obj.relative_to(args.donor_root)),
                "func": name,
                "size": size,
                "hits": hits,
            })

    by_obj = defaultdict(list)
    for r in results:
        by_obj[r["donor_obj"]].append(r)
    total = uniq = 0
    for obj, rows in by_obj.items():
        n_uniq = sum(1 for r in rows if len(r["hits"]) == 1)
        n_amb = sum(1 for r in rows if len(r["hits"]) > 1)
        total += len(rows)
        uniq += n_uniq
        print(f"{obj:50} {n_uniq:3}/{len(rows):3} unique  {n_amb:3} ambiguous")
    print(f"\nTotal: {uniq}/{total} donor functions matched a unique FSA function start")
    if args.json:
        args.json.write_text(json.dumps(results, indent=1))


if __name__ == "__main__":
    sys.exit(main())
