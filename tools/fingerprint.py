#!/usr/bin/env python3
"""Behaviour fingerprint: does our function do what the original does?

Usage:
  python3 tools/fingerprint.py <unit>              # every function below 100 %: grade and score
  python3 tools/fingerprint.py <unit> <function>   # what differs, as a to-do list

objdiff's match % mixes two things: behaviour (which calls, constants, compares
and fields a function uses) and codegen (registers, order, stack slots). This
tool measures the first alone. For the target function and ours it compares
multisets of:

  hard  calls (by name), constant-pool values (the float/double bytes behind
        every lfs/lfd, read from the objects or from orig/sys/main.dol),
        compare immediates, named globals
  soft  non-stack memory accesses (kind + displacement), integer immediates,
        branch-condition classes, arithmetic/logic operations (by mnemonic)

score = 3 * hard differences + soft differences
grade = clean   score 0: same behaviour, only codegen differs
        near    no hard difference, soft <= max(2, 3 %)
        drift   anything else (the to-do list says what)

It reads the unit's compiled object, so build it first (tools/try.py <unit>).
tools/grind.py imports unit_eval() as its judge's measure.
"""

import importlib.util
import json
import re
import struct
import subprocess
import sys
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent


def _load(name):
    spec = importlib.util.spec_from_file_location(f"fsa_{name}", HERE / f"{name}.py")
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


T = _load("try")
S = _load("structural")

LS = {"lwz": "w", "stw": "w", "lwzu": "w", "stwu": "w", "lbz": "b", "stb": "b", "lbzu": "b", "stbu": "b",
      "lhz": "h", "lha": "h", "sth": "h", "lhzu": "h", "lhau": "h", "sthu": "h",
      "lfs": "fs", "stfs": "fs", "lfsu": "fs", "stfsu": "fs", "lfd": "fd", "stfd": "fd", "lfdu": "fd", "stfdu": "fd"}
CMP = {"cmpwi", "cmplwi"}
IMM = {"li", "lis", "addi", "addis", "ori", "oris", "andi.", "andis.", "xori", "xoris", "mulli", "subfic",
       "addic", "addic."}
BR = {"beq": "eq/ne", "bne": "eq/ne", "blt": "lt/ge", "bge": "lt/ge", "bgt": "gt/le", "ble": "gt/le",
      "bdnz": "ctr", "bdz": "ctr"}
PLACEHOLDER = re.compile(r"^(@\d+|lbl_[0-9A-Fa-f]{8}|\.\.\.|\.L)")
SAVEREST = re.compile(r"^_(save|rest)(gpr|fpr)_\d+")  # register save helpers: which one is regalloc, not behaviour
# Not behaviour: moves, frame and link-register handling, plain jumps (everything else that is
# not a call, load/store, compare or immediate counts as an operation).
NOT_OPS = {"mr", "fmr", "nop", "mflr", "mtlr", "blr", "b", "bctr", "mtctr", "stmw", "lmw", "crclr", "crset", "mfcr",
           "psq_st", "psq_l"}
KIND = {"w": "word", "b": "byte", "h": "halfword", "fs": "float", "fd": "double"}


class Dol:
    """Initialized bytes of the original DOL by address (for literals split into another unit)."""

    def __init__(self, root):
        self.secs = []
        try:
            d = (Path(root) / "orig/sys/main.dol").read_bytes()
        except OSError:
            return
        offs = struct.unpack_from(">18I", d, 0x00)
        addrs = struct.unpack_from(">18I", d, 0x48)
        sizes = struct.unpack_from(">18I", d, 0x90)
        self.secs = [(a, a + n, d[o:o + n]) for o, a, n in zip(offs, addrs, sizes) if n]

    def read(self, addr, n):
        for lo, hi, data in self.secs:
            if lo <= addr and addr + n <= hi:
                return data[addr - lo: addr - lo + n]
        return None


_SYM_ADDR = {}


def sym_addr(root, name):
    """Address of a symbol: from an lbl_ name, else config/G4SE01/symbols.txt."""
    m = re.match(r"^lbl_([0-9A-Fa-f]{8})$", name)
    if m:
        return int(m.group(1), 16)
    tab = _SYM_ADDR.get(str(root))
    if tab is None:
        tab = {}
        try:
            for line in (Path(root) / "config/G4SE01/symbols.txt").read_text().splitlines():
                m = re.match(r"^(\S+) = \.\w+:0x([0-9A-Fa-f]+);", line)
                if m:
                    tab[m.group(1)] = int(m.group(2), 16)
        except OSError:
            pass
        _SYM_ADDR[str(root)] = tab
    return tab.get(name)


def fingerprint(insns, syms, dol, root):
    """Seven Counters for one function's instruction list (tools/try.py insn_list)."""
    calls, lit, cmps, glob, mem, imm, br, ops = (Counter() for _ in range(8))
    for mn, args, rel, _ in insns:
        base = mn.rstrip("+-")
        if mn == "bl" and rel:
            if not SAVEREST.match(rel):
                calls[rel] += 1
        elif mn in ("bctrl", "blrl"):
            calls["<indirect call>"] += 1
        elif mn in LS:
            if len(args) == 3 and args[2] != "r1" and args[1] != "RELOC":
                mem[(("store " if mn.startswith("st") else "load ") + KIND[LS[mn]], args[1])] += 1
        elif mn in CMP and len(args) >= 2:
            cmps[(mn, args[-1])] += 1
        elif mn in IMM:
            if "RELOC" not in args and "r1" not in args[1:] and not (mn == "li" and args[-1] in ("0", "1")):
                imm[args[-1]] += 1
        elif base in BR:
            br[BR[base]] += 1
        elif base not in NOT_OPS and not base.startswith("b"):
            ops[mn.rstrip(".")] += 1
        if rel and mn != "bl":
            name = rel.split("+")[0]
            if mn in ("lfs", "lfd"):
                n = 4 if mn == "lfs" else 8
                val = next((d[:n] for d, sec in syms.get(name, []) if d is not None and len(d) >= n and "bss" not in sec), None)
                if val is None and dol is not None:
                    a = sym_addr(root, name)
                    val = dol.read(a, n) if a is not None else None
                if val is not None:
                    lit[val.hex()] += 1
                    continue
            if not PLACEHOLDER.match(name):
                glob[rel] += 1
    return {"calls": calls, "lit": lit, "cmp": cmps, "glob": glob, "mem": mem, "imm": imm, "br": br, "ops": ops}


HARD, SOFT = ("calls", "lit", "cmp", "glob"), ("mem", "imm", "br", "ops")


def _num(h):
    b = bytes.fromhex(h)
    v = struct.unpack(">f" if len(b) == 4 else ">d", b)[0]
    return f"{v:g}{'f' if len(b) == 4 else ''}"


def _hexv(s):
    try:
        v = int(s)
        return f"{v} (0x{v & 0xFFFF:X})" if abs(v) > 9 else str(v)
    except ValueError:
        return s


LABEL = {
    "calls": lambda k: f"call {k}",
    "lit": lambda k: f"constant {_num(k)}",
    "cmp": lambda k: f"compare ({k[0]}) with {_hexv(k[1])}",
    "glob": lambda k: f"global {k}",
    "mem": lambda k: f"{k[0]} at offset {_hexv(k[1])}",
    "imm": lambda k: f"immediate {_hexv(k)}",
    "br": lambda k: f"{k} branch",
    "ops": lambda k: f"operation {k}",
}


def compare(ft, fo):
    """(score, grade, to-do lines) of a target and our fingerprint."""
    hard = soft = total = 0
    todo = []
    for key in HARD + SOFT:
        a, b = ft[key], fo[key]
        d = sum(((a - b) + (b - a)).values())
        total += sum(a.values())
        if key in HARD:
            hard += d
        else:
            soft += d
        for k in sorted(set(a) | set(b), key=str):
            if a[k] != b[k]:
                todo.append(f"{LABEL[key](k)}: original {a[k]}x, ours {b[k]}x")
    score = 3 * hard + soft
    if score == 0:
        grade = "clean"
    elif hard == 0 and soft <= max(2, 0.03 * total):
        grade = "near"
    else:
        grade = "drift"
    return score, grade, todo


def unit_eval(arg, root="."):
    """Evaluate one unit's compiled object against the target.

    Returns {"unit", "source", "sections": {name: %}, "functions": {name: {...}}} where each
    function has pct, size, cls (tools/try.py class; "match" at 100 %, "missing" when our
    object lacks it), demangled, score, grade, todo. None if the unit has no compiled object."""
    root = Path(root)
    units = json.loads((root / "objdiff.json").read_text())["units"]
    unit = T.find_unit(units, arg)
    if unit is None or not unit.get("base_path") or not (root / unit["base_path"]).exists():
        return None
    out = subprocess.run([str(root / T.OBJDIFF), "diff", "-p", ".", "-u", unit["name"], "-o", "-", "-c", T.CONFIG],
                         capture_output=True, text=True, cwd=root)
    if out.returncode:
        raise RuntimeError(out.stderr.strip()[-400:] or "objdiff-cli failed")
    d = json.loads(out.stdout)
    left, right = d["left"], d["right"]
    tsyms = S.read_symbols(root / unit["target_path"])
    osyms = S.read_symbols(root / unit["base_path"])
    dol = Dol(root)
    secs = {s["name"]: float(s.get("match_percent") or 0) for s in left.get("sections", [])
            if s.get("match_percent") is not None and not s["name"].startswith(".rela")}
    fns = {}
    for s in left["symbols"]:
        if s.get("kind") != "SYMBOL_FUNCTION":
            continue
        size, pct = int(s.get("size", 0)), float(s.get("match_percent") or 0)
        e = {"pct": pct, "size": size, "addr": int(s.get("address", 0)), "demangled": s.get("demangled_name") or s["name"]}
        ti = s.get("target_symbol")
        if ti is None:
            e.update(cls="missing", score=10 ** 6, grade="missing", todo=["not in our object (unwritten or compiled out)"], pct=0.0)
        elif pct >= 100:
            e.update(cls="match", score=0, grade="clean", todo=[])
        else:
            r = right["symbols"][ti]
            t, o = T.insn_list(left, s), T.insn_list(right, r)
            cls, _ = T.classify(t, o, T.reloc_pairs(s, r, left, right))
            score, grade, todo = compare(fingerprint(t, tsyms, dol, root), fingerprint(o, osyms, dol, root))
            e.update(cls=cls, score=score, grade=grade, todo=todo, insns=(len(t), len(o)))
        fns[s["name"]] = e
    src = (unit.get("metadata") or {}).get("source_path") or ""
    return {"unit": unit["name"], "source": src, "sections": secs, "functions": fns}


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    ev = unit_eval(sys.argv[1])
    if ev is None:
        sys.exit(f"{sys.argv[1]}: no compiled object (run tools/try.py first)")
    fns = ev["functions"]
    if len(sys.argv) > 2:
        want = sys.argv[2]
        name = want if want in fns else next((n for n in fns if n.split("__")[0] == want), None)
        if name is None:
            sys.exit(f"no function {want!r} in {ev['unit']}")
        e = fns[name]
        print(f"{name}: {e['pct']:.2f} %, {e['cls']}; behaviour {e['grade']} (score {e['score'] if e['score'] < 10**6 else '-'})")
        for line in e["todo"][:40]:
            print("  " + line)
        if len(e["todo"]) > 40:
            print(f"  ... and {len(e['todo']) - 40} more")
        return
    for name, e in sorted(fns.items(), key=lambda kv: (-kv[1]["score"], kv[0])):
        if e["pct"] < 100:
            sc = "-" if e["score"] >= 10 ** 6 else e["score"]
            print(f"{e['grade']:8} score {sc:>5}  {e['pct']:6.2f} %  {e['size']:5} B  {e['cls']:32} {name}")


if __name__ == "__main__":
    main()
