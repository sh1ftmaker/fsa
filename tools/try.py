#!/usr/bin/env python3
"""One attempt: compile one unit, diff it, say what kind of difference is left.

Usage:
  python3 tools/try.py <unit> [<function>]   # e.g. game/act_rat.cpp, JSystem/JKernel/JKRUnitHeap.cpp
  python3 tools/try.py <unit> <function> --reset   # forget the attempt log of that function

Compiles only the unit's object (ninja <its .o>; first re-splits with dtk if
splits.txt or symbols.txt is newer than build/G4SE01/config.json; no link,
no report), diffs it with objdiff-cli
under the report's relocation setting (functionRelocDiffs=all, as
tools/fdiff.py does) and prints at most 20 lines:

  * without <function>: the unit's sections below 100 % and every function
    below 100 %, each with a classification;
  * with <function>: its match %, the classification and the first
    differing rows (target | ours).

Classifications (non-matching functions):
  regalloc-only   same instructions apart from register numbers
  scheduling      same instructions (multiset), different order (+regalloc: registers differ too)
  reloc-name      same instructions, only relocation targets differ (a symbol name or a split)
  reloc+regalloc  only registers and relocation targets differ
  reloc-data      identical instructions and targets; a referenced data value differs
  missing/extra   N instructions more or fewer than the target
  different       same length, different instructions
  missing         the function isn't in our object (not written yet, or named differently)

Attempt log: build/try/<unit slug>.json. An attempt is a run in which the
function's compiled code differs from the previous run's (re-running without a
change counts nothing). After 4 attempts without beating the best %, it prints
`STOP: 4 attempts without progress — leave a FIXME`. The % here is objdiff's
for the compiled object; the honest report (ninja build/G4SE01/report_honest.json,
tools/gate.py) stays the measure of record.
"""

import hashlib
import json
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

OBJDIFF = "build/tools/objdiff-cli"
CONFIG = "functionRelocDiffs=all"  # same as the honest report and objdiff.json
LOGDIR = Path("build/try")
STOP_AFTER = 4
MAX_LINES = 20
REG = re.compile(r"^(r|f|cr|qr)\d+$")


def die(msg):
    print(msg)
    sys.exit(1)


# ---------------------------------------------------------------- unit lookup

def load_units():
    try:
        return json.loads(Path("objdiff.json").read_text())["units"]
    except FileNotFoundError:
        return None


def strip_ext(p):
    return re.sub(r"\.[^./]*$", "", p)


def find_unit(units, arg):
    want = strip_ext(arg.replace("__", "/"))
    for u in units:
        src = (u.get("metadata") or {}).get("source_path") or ""
        name = u["name"].split("/", 1)[-1]
        if u["name"] == arg or src == arg or src.endswith("/" + arg) or name == want:
            return u
    return None


def slug_of(unit_name):
    return unit_name.split("/", 1)[-1].replace("/", "__")


# ---------------------------------------------------------------- build + diff

def ninja(*targets):
    out = subprocess.run(["timeout", "900", "ninja", *targets], capture_output=True, text=True)
    return out.returncode, out.stdout + out.stderr


def build(arg):
    # dtk re-splits only when config.json goes (CLAUDE.md: rm it after editing splits/symbols)
    cfg = Path("build/G4SE01/config.json")
    if cfg.exists() and any(Path(f"config/G4SE01/{n}").stat().st_mtime > cfg.stat().st_mtime
                            for n in ("splits.txt", "symbols.txt")):
        cfg.unlink()
    units = load_units()
    unit = find_unit(units, arg) if units else None
    if unit is None or "base_path" not in unit:
        # a new source file (or a fresh worktree): split and configure first
        rc, out = ninja("build/G4SE01/config.json")
        if rc:
            die("split failed:\n" + "\n".join(out.splitlines()[-10:]))
        subprocess.run(["python3", "configure.py"], capture_output=True)
        unit = find_unit(load_units() or [], arg)
    if unit is None:
        die(f"no unit matches {arg!r}")
    if "base_path" not in unit:
        die(f"{unit['name']}: no source file yet (create src/{unit['name'].split('/', 1)[-1]}.cpp, then rerun)")
    rc, out = ninja("build/G4SE01/config.json", unit["base_path"])
    if rc:
        lines = [l for l in out.splitlines() if l.strip() and not l.startswith(("[", "ninja:", "FAILED:", "build/tools/", "User break"))]
        die("compile failed:\n" + "\n".join(lines[-14:]))
    # the split may have rewritten objdiff.json
    return find_unit(load_units(), arg) or unit


def run_diff(unit):
    cmd = [OBJDIFF, "diff", "-p", ".", "-u", unit["name"], "-o", "-", "-c", CONFIG]
    out = subprocess.run(cmd, capture_output=True, text=True)
    if out.returncode != 0:
        die(out.stderr.strip()[-800:] or "objdiff-cli failed")
    return json.loads(out.stdout)


# ---------------------------------------------------------------- normalising instructions

def insn_list(side, sym):
    """[(mnemonic, args, reloc target name or None, formatted)] of one function."""
    syms = side["symbols"]
    start = int(sym.get("address", 0))
    res = []
    for row in sym.get("instructions", []):
        ins = row.get("instruction")
        if not ins:
            continue
        mn, args = "", []
        for p in ins.get("parts", []):
            if "opcode" in p:
                mn = p["opcode"].get("mnemonic", "")
            elif "arg" in p:
                a = p["arg"]
                if "opaque" in a:
                    args.append(a["opaque"])
                elif "branch_dest" in a:
                    args.append(f"@{int(a['branch_dest']) - start:x}")
                elif "reloc" in a:
                    args.append("RELOC")
                else:
                    args.append(str(next(iter(a.values()))))
        rel = ins.get("relocation")
        target = None
        if rel is not None:
            t = rel.get("target_symbol")
            target = syms[t]["name"] if t is not None and t < len(syms) else "?"
            if rel.get("addend"):
                target += f"+{rel['addend']}"
        res.append((mn, tuple(args), target, ins.get("formatted", "")))
    return res


def noreg(i):
    return (i[0], tuple("R" if REG.match(a) else a for a in i[1]), i[2])


def noreloc(i):
    return (i[0], i[1], None)


def nobranch(i):
    return (i[0], tuple("@" if a.startswith("@") else a for a in i[1]), i[2])


def full(i):
    return i[:3]


def classify(t, o):
    """(class, detail) for target and our instruction lists.

    The instruction shape (with relocation targets erased) gives one of
    regalloc-only, scheduling(+regalloc), missing/extra, different; differing
    relocation targets add reloc-name. Nothing else differing: reloc-data."""
    tags, details = [], []
    T, O = [noreloc(i) for i in t], [noreloc(i) for i in o]
    if T == O:
        pass
    elif len(T) == len(O) and list(map(noreg, T)) == list(map(noreg, O)):
        pairs = Counter()
        for a, b in zip(T, O):
            for x, y in zip(a[1], b[1]):
                if x != y and REG.match(x):
                    pairs[f"{x}->{y}"] += 1
        tags.append("regalloc-only")
        details.append("target->ours " + ", ".join(p for p, _ in pairs.most_common(4)))
    elif Counter(map(nobranch, T)) == Counter(map(nobranch, O)):
        tags.append("scheduling")
        details.append("same instructions, different order")
    elif Counter(nobranch(noreg(i)) for i in T) == Counter(nobranch(noreg(i)) for i in O):
        tags.append("scheduling+regalloc")
        details.append("same instructions modulo registers, different order")
    elif len(T) != len(O):
        n = len(O) - len(T)
        tags.append("missing/extra")
        details.append(f"ours has {abs(n)} {'more' if n > 0 else 'fewer'} instrs ({len(O)} vs {len(T)})")
    else:
        ops = sum(1 for a, b in zip(T, O) if a[0] != b[0])
        tags.append("different")
        details.append(f"same length, {ops} opcodes differ")
    rt = Counter(i[2] for i in t if i[2])
    ro = Counter(i[2] for i in o if i[2])
    gone, new = sorted(rt - ro), sorted(ro - rt)
    if gone or new:
        names = [f"{a} vs {b}" for a, b in zip(gone, new)]
        names += [f"{a} vs -" for a in gone[len(new):]] + [f"- vs {b}" for b in new[len(gone):]]
        tags.append("reloc-name")
        details.append("target vs ours: " + "; ".join(names[:3]) + (f" (+{len(names) - 3})" if len(names) > 3 else ""))
        if any(b.startswith("@") for b in new) and any(a.startswith("lbl_") for a in gone):
            details.append("@NNN vs lbl_: literals not yet split into this unit or not in the original order")
    if not tags:
        return "reloc-data", "same instructions and targets; a referenced data value differs"
    return " + ".join(tags), "; ".join(details)


def window(lsym, rsym, rows=7, structural=False):
    """The aligned objdiff rows around the first difference (with structural=True,
    the first inserted, deleted or different instruction rather than a register)."""
    lrows, rrows = lsym.get("instructions", []), rsym.get("instructions", [])
    pairs = list(zip(lrows, rrows))
    kinds = ("DIFF_INSERT", "DIFF_DELETE", "DIFF_REPLACE", "DIFF_OP_MISMATCH") if structural else None
    first = next((k for k, (a, b) in enumerate(pairs)
                  if (a.get("diff_kind") or b.get("diff_kind")) and
                  (kinds is None or (a.get("diff_kind") or b.get("diff_kind")) in kinds)), None)
    if first is None and structural:
        return window(lsym, rsym, rows)
    if first is None:
        return []
    start = int(lsym.get("address", 0))
    out = []
    for k in range(max(0, first - 2), min(len(pairs), first - 2 + rows)):
        a, b = pairs[k]
        fa = (a.get("instruction") or {}).get("formatted", "").strip()
        fb = (b.get("instruction") or {}).get("formatted", "").strip()
        kind = (a.get("diff_kind") or b.get("diff_kind") or "").replace("DIFF_", "").lower()
        mark = ">" if kind else " "
        addr = (a.get("instruction") or {}).get("address")
        off = f"{int(addr) - start:4X}" if addr is not None else "    "
        out.append(f"{mark}{off}  {fa[:34]:34} | {fb[:34]:34} {kind}")
    return out


# ---------------------------------------------------------------- attempt log

def code_hash(o):
    return hashlib.sha1(repr([full(i) for i in o]).encode()).hexdigest()[:16]


def record(log, name, pct, o, cls, src=None):
    """Update the log; returns (entry, note) where note is '' or a STOP line.

    An attempt is a run where the function's code changed, or (src given: the
    function was named on the command line) where the unit's source changed."""
    h = code_hash(o) if o is not None else "missing"
    e = log.get(name)
    if e is None:
        e = log[name] = {"best": pct, "start": pct, "attempts": 0, "stale": 0, "hash": h, "src": src}
    elif h != e["hash"] or (src is not None and e.get("src") not in (None, src)):
        e["attempts"] += 1
        e["hash"] = h
        if pct > e["best"]:
            e["best"], e["stale"] = pct, 0
        else:
            e["stale"] += 1
    e["last"], e["class"] = pct, cls
    if src is not None:
        e["src"] = src
    if pct < 100 and e["stale"] >= STOP_AFTER:
        return e, f"STOP: {STOP_AFTER} attempts without progress — leave a FIXME"
    return e, ""


def fmt_pct(p):
    return "missing" if p is None else f"{p:.2f}%"


# ---------------------------------------------------------------- main

def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if not args or "-h" in sys.argv or "--help" in sys.argv:
        print(__doc__)
        return
    unit = build(args[0])
    diff = run_diff(unit)
    left, right = diff["left"], diff["right"]
    funcs = [s for s in left["symbols"] if s.get("kind") == "SYMBOL_FUNCTION"]
    funcs.sort(key=lambda s: int(s.get("address", 0)))

    LOGDIR.mkdir(parents=True, exist_ok=True)
    logpath = LOGDIR / f"{slug_of(unit['name'])}.json"
    try:
        log = json.loads(logpath.read_text())
    except (FileNotFoundError, ValueError):
        log = {}

    def ours(s):
        t = s.get("target_symbol")
        return right["symbols"][t] if t is not None else None

    def analyse(s):
        r = ours(s)
        pct = s.get("match_percent") if r is not None else None
        if r is None:
            return 0.0, None, ("missing", "not in our object (unwritten, or named differently)"), r
        if pct == 100:
            return 100.0, insn_list(right, r), ("match", ""), r
        o = insn_list(right, r)
        return float(pct or 0), o, classify(insn_list(left, s), o), r

    lines = []
    if len(args) == 1:
        matched = sum(1 for s in funcs if s.get("match_percent") == 100 and ours(s) is not None)
        lines.append(f"{unit['name'].split('/', 1)[-1]}: {matched}/{len(funcs)} functions match")
        secs = [f"{s['name']} {s.get('match_percent', 0):.2f}%" for s in left["sections"]
                if s.get("kind") in ("SECTION_CODE", "SECTION_DATA", "SECTION_BSS")
                and s.get("match_percent", 0) < 100]
        if secs:
            lines.append("sections below 100%: " + ", ".join(secs))
            if all(x.startswith("extab") for x in secs):
                lines.append("  (extab only: weak-duplicate entries the honest report strips on link evidence?"
                             " check report_honest.json)")
        extra = [x["name"] for x in right["symbols"]
                 if x.get("kind") == "SYMBOL_FUNCTION" and x.get("target_symbol") is None
                 and int(x.get("size", 0)) and not (x.get("flags") or {}).get("weak")]
        if extra:
            lines.append(f"ours only ({len(extra)}): " + ", ".join(extra[:4])[:150])
        rest, missing = [], []
        for s in funcs:
            pct, o, (cls, detail), _ = analyse(s)
            _, note = record(log, s["name"], pct, o, cls)
            if cls == "match":
                continue
            if cls == "missing":
                missing.append(s["name"])
                continue
            stop = "  STOP" if note else ""
            name = s["name"] if len(s["name"]) <= 50 else s["name"][:49] + "~"
            rest.append(f"  {name:50} {fmt_pct(None if o is None else pct):>8}  {cls}{stop}")
        if missing:
            rest.append(f"  not in our object ({len(missing)}): " + ", ".join(missing[:5])[:110]
                        + (", ..." if len(missing) > 5 else ""))
        room = MAX_LINES - len(lines) - 1
        lines += rest[:room]
        if len(rest) > room:
            lines.append(f"  ... {len(rest) - room} more (name one: try.py <unit> <function>)")
        elif any("STOP" in r for r in rest):
            lines.append(f"STOP marks: {STOP_AFTER} attempts without progress — leave a FIXME")
    else:
        want = args[1]
        hits = [s for s in funcs if s["name"] == want] or \
               [s for s in funcs if s.get("demangled_name", "").split("(")[0] == want] or \
               [s for s in funcs if want in s["name"] or want in s.get("demangled_name", "")]
        if len(hits) != 1:
            names = [s["name"] for s in hits] or [s["name"] for s in funcs]
            die(f"{want!r} matches {len(hits)} target functions: " + ", ".join(names[:12]))
        s = hits[0]
        pct, o, (cls, detail), r = analyse(s)
        source = Path((unit.get("metadata") or {}).get("source_path") or "-")
        src = hashlib.sha1(source.read_bytes()).hexdigest()[:16] if source.is_file() else None
        e, note = record(log, s["name"], pct, o, cls, src)
        lines.append(f"{s['name']}: {fmt_pct(None if o is None else pct)} ({int(s['size'])} bytes)")
        if cls == "missing":
            extra = [x["name"] for x in right["symbols"]
                     if x.get("kind") == "SYMBOL_FUNCTION" and x.get("target_symbol") is None
                     and not (x.get("flags") or {}).get("weak")]
            lines.append(f"missing: {detail}")
            if extra:
                lines.append("ours, not in the target: " + ", ".join(extra[:6]))
        elif cls != "match":
            lines.append(f"{cls}: {detail}")
            lines += window(s, r, rows=min(11, MAX_LINES - 5),
                            structural=cls.split(" ")[0] in ("missing/extra", "different"))
        lines.append(f"attempts {e['attempts']}, best {e['best']:.2f}%, start {e['start']:.2f}%"
                     + (f", {e['stale']} since last gain" if e["stale"] else ""))
        if note:
            lines.append(note)
    logpath.write_text(json.dumps(log, indent=1, sort_keys=True) + "\n")
    print("\n".join(lines[:MAX_LINES]))


def reset():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    units = load_units() or []
    unit = find_unit(units, args[0]) if args else None
    if unit is None:
        die("usage: try.py <unit> [<function>] --reset")
    logpath = LOGDIR / f"{slug_of(unit['name'])}.json"
    log = json.loads(logpath.read_text()) if logpath.exists() else {}
    for k in [k for k in log if len(args) < 2 or args[1] in k]:
        del log[k]
    logpath.parent.mkdir(parents=True, exist_ok=True)
    logpath.write_text(json.dumps(log, indent=1, sort_keys=True) + "\n")
    print(f"attempt log reset: {logpath}" + (f" ({args[1]})" if len(args) > 1 else ""))


if __name__ == "__main__":
    reset() if "--reset" in sys.argv else main()
