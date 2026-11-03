#!/usr/bin/env python3
"""One attempt: compile one unit, diff it, say what kind of difference is left.

Usage:
  python3 tools/try.py <unit> [<function>] [--budget N]
      e.g. game/act_rat.cpp, JSystem/JKernel/JKRUnitHeap.cpp

Compiles only the unit's object (ninja <its .o>; first re-splits with dtk if
splits.txt or symbols.txt is newer than build/G4SE01/config.json; no link,
no report), diffs it with objdiff-cli under the report's relocation setting
(functionRelocDiffs=all, as tools/fdiff.py does) and prints at most 20 lines:

  * without <function>: the unit's sections below 100 % and every function
    below 100 %, each with a classification;
  * with <function>: its match %, the classification and the first
    differing rows (target | ours).

Classifications (non-matching functions; shape and reloc-name combine):
  regalloc-only        same instructions apart from register numbers
  scheduling           same instructions (multiset), different order
  scheduling+regalloc  the same, and registers differ too
  operands             same opcodes in the same order, an immediate differs
  missing/extra        N instructions more or fewer than the target
  different            same length, different opcodes
  reloc-name           objdiff marks a relocation target as different
                       (target vs ours, paired by row)
  same-text            identical as printed, but objdiff marks rows (a referenced data
                       value, or offsets from a base register it pairs differently)
  missing              the function isn't in our object (unwritten, or named differently)

Attempt log: build/try/<unit slug>.json. An attempt is a run in which the
function's own compiled code changed since the previous run (compiler @NNN
label numbers are ignored, so edits elsewhere in the file, comments and
no-op rewrites count nothing). Try budget by the target's size (GAME_PLAN
item 8): 3 tries up to 32 instructions, 6 up to 128, and for larger
functions the budget the coordinator sets with --budget N (default 10).
When a function has used its budget without beating its best %, try.py
prints STOP: leave a FIXME saying what differs and carry on with the rest
of the file. Only the coordinator resets the log (delete the file).

The % here is objdiff's for the compiled object; the honest report
(ninja build/G4SE01/report_honest.json, tools/gate.py) stays the measure of
record.
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
LARGE_BUDGET = 10  # >128 instructions, unless --budget N
LOCAL_LABEL = re.compile(r"@\d+")
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
        mn, args, rel_arg = "", [], None
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
                    rel_arg = len(args)
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
        res.append((mn, tuple(args), target, rel_arg))
    return res


def reloc_pairs(lsym, rsym, left, right):
    """(target name, our name) for each aligned row where objdiff marks the
    relocation argument as different."""
    pairs = []
    for a, b in zip(lsym.get("instructions", []), rsym.get("instructions", [])):
        if not (a.get("instruction") and b.get("instruction")) or not (a.get("diff_kind") or b.get("diff_kind")):
            continue
        ta, = insn_list(left, {"address": lsym.get("address", 0), "instructions": [a]})
        tb, = insn_list(right, {"address": rsym.get("address", 0), "instructions": [b]})
        marked = any(k is not None and k < len(d.get("arg_diff", [])) and d["arg_diff"][k]
                     for k, d in ((ta[3], a), (tb[3], b)))
        if marked and (ta[2], tb[2]) not in pairs:
            pairs.append((ta[2] or "-", tb[2] or "-"))
    return pairs


def noreg(i):
    return (i[0], tuple("R" if REG.match(a) else a for a in i[1]), i[2])


def noreloc(i):
    return (i[0], i[1], None)


def nobranch(i):
    return (i[0], tuple("@" if a.startswith("@") else a for a in i[1]), i[2])



def classify(t, o, rpairs):
    """(class, detail) from target and our instruction lists and the
    relocation pairs objdiff marked (reloc_pairs)."""
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
    elif [i[0] for i in T] == [i[0] for i in O]:
        tags.append("operands")
        details.append("same opcodes, an immediate or offset differs")
    else:
        ops = sum(1 for a, b in zip(T, O) if a[0] != b[0])
        tags.append("different")
        details.append(f"same length, {ops} opcodes differ")
    if rpairs:
        names = [f"{a} vs {b}" for a, b in rpairs]
        tags.append("reloc-name")
        details.append("target vs ours: " + "; ".join(names[:3]) + (f" (+{len(names) - 3})" if len(names) > 3 else ""))
        if any(b.startswith("@") for a, b in rpairs if a.startswith("lbl_")):
            details.append("@NNN vs lbl_: literals not yet split into this unit or not in the original order")
    if not tags:
        return "same-text", "identical as printed; objdiff marks rows (a data value, or offsets from a base it pairs differently)"
    return " + ".join(tags), "; ".join(details)


def budget_for(n, large):
    """GAME_PLAN item 8: tries per function by the target's instruction count."""
    if n <= 32:
        return 3, "<=32 instrs"
    if n <= 128:
        return 6, "33-128 instrs"
    return large, ">128 instrs"


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
    """Our function's code, with compiler label numbers (@NNN) erased: they are
    renumbered across the file whenever a literal is added anywhere earlier."""
    norm = [(i[0], i[1], LOCAL_LABEL.sub("@", i[2]) if i[2] else None) for i in o]
    return hashlib.sha1(repr(norm).encode()).hexdigest()[:16]


def record(log, name, pct, o, cls, budget, tier):
    """Update the log; returns (entry, note) where note is '' or a STOP line.
    An attempt is a run where the function's own (normalised) code changed."""
    h = code_hash(o) if o is not None else "missing"
    e = log.get(name)
    if e is None:
        e = log[name] = {"best": pct, "start": pct, "attempts": 0, "stale": 0, "hash": h}
    elif h != e["hash"]:
        e["attempts"] += 1
        e["hash"] = h
        if pct > e["best"]:
            e["best"], e["stale"] = pct, 0
        else:
            e["stale"] += 1
    e["last"], e["class"], e["budget"] = pct, cls, budget
    if pct < 100 and e["stale"] >= budget:
        return e, (f"STOP: {budget} tries without progress ({tier}) — leave a FIXME saying what"
                   " differs and carry on with the rest of the file")
    return e, ""


def fmt_pct(p):
    return "missing" if p is None else f"{p:.2f}%"


# ---------------------------------------------------------------- main

def main():
    argv, large = sys.argv[1:], LARGE_BUDGET
    if "--budget" in argv:
        k = argv.index("--budget")
        try:
            large = int(argv[k + 1])
        except (IndexError, ValueError):
            die("--budget needs a number")
        del argv[k:k + 2]
    args = [a for a in argv if not a.startswith("-")]
    if not args or "-h" in argv or "--help" in argv:
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
        t = insn_list(left, s)
        budget, tier = budget_for(len(t), large)
        if r is None:
            cls = ("missing", "not in our object (unwritten, or named differently)")
            return 0.0, None, cls, r, budget, tier
        o = insn_list(right, r)
        if pct == 100:
            return 100.0, o, ("match", ""), r, budget, tier
        return float(pct or 0), o, classify(t, o, reloc_pairs(s, r, left, right)), r, budget, tier

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
            pct, o, (cls, detail), _, budget, tier = analyse(s)
            _, note = record(log, s["name"], pct, o, cls, budget, tier)
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
            lines.append("STOP: try budget used without progress — leave a FIXME saying what differs"
                         " and carry on with the rest of the file")
    else:
        want = args[1]
        hits = [s for s in funcs if s["name"] == want] or \
               [s for s in funcs if s.get("demangled_name", "").split("(")[0] == want] or \
               [s for s in funcs if want in s["name"] or want in s.get("demangled_name", "")]
        if len(hits) != 1:
            names = [s["name"] for s in hits] or [s["name"] for s in funcs]
            die(f"{want!r} matches {len(hits)} target functions: " + ", ".join(names[:12]))
        s = hits[0]
        pct, o, (cls, detail), r, budget, tier = analyse(s)
        e, note = record(log, s["name"], pct, o, cls, budget, tier)
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
        lines.append(f"attempts {e['attempts']}, best {e['best']:.2f}%, start {e['start']:.2f}%, budget {budget} ({tier})"
                     + (f", {e['stale']} since last gain" if e["stale"] else ""))
        if note:
            lines.append(note)
    logpath.write_text(json.dumps(log, indent=1, sort_keys=True) + "\n")
    print("\n".join(lines[:MAX_LINES]))



if __name__ == "__main__":
    main()
