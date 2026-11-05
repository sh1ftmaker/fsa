#!/usr/bin/env python3
"""Second progress metric: "structurally matched" (docs/RULES_CARD.md, "Big functions: structure first").

A function below 100 % counts as structurally done when tools/try.py's classifier
(imported here, not copied) calls it regalloc-only, scheduling or
scheduling+regalloc, with or without reloc-name: every instruction the same apart
from register numbers and order. A function whose instructions are identical and
only a relocation name differs (class reloc-name) counts too. "Structurally
close" is missing/extra with |instruction difference| <= 4 and >= 85 %.

Writes build/G4SE01/structural.json:
  honest      {functions, code}   100 % matched, from report_honest.json
  structural  {functions, code}   functions at 100 % plus structurally done
  done        {functions, code}   the structurally done ones alone
  close       {functions, code}   structurally close ones alone
  classes     {class: [functions, code]}  every non-100 % function
  units       {unit: {function: [match %, bytes, class]}}  non-100 % functions

It diffs every unit with source once (objdiff-cli, the same project directory
and relocation setting as report_honest.json: build/G4SE01/honest, where the
weak-duplicate stripping already applies), in parallel, and caches each unit's
result by the mtimes of its objects (build/G4SE01/structural_cache.json).

A reloc-name difference counts only when every differing relocation pair is
ours @NNN against the target's lbl_XXXXXXXX (naming of a literal/data object);
any other target difference could be a wrong call or global, so that function
is class "... reloc-name-unverified" and left out of the structural total.

Usage: python3 tools/structural.py [-j N] [--no-cache]
Run after `ninja build/G4SE01/report_honest.json` (ninja runs it as
build/G4SE01/structural.json).
"""

import argparse
import importlib.util
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

OUT = Path("build/G4SE01")
HONEST = OUT / "honest"
REPORT = OUT / "report_honest.json"
CACHE = OUT / "structural_cache.json"
DONE = {"regalloc-only", "scheduling", "scheduling+regalloc", "reloc-name"}
CLOSE_INSNS, CLOSE_PCT = 4, 85.0
VERSION = 2  # bump when the classification changes


LOCAL_LIT = re.compile(r"^@\d+$")
ORIG_LBL = re.compile(r"^lbl_[0-9A-Fa-f]+$")


def naming_only(pairs):
    """True when every differing relocation is a compiler-local literal/data
    object (ours @NNN) against the target's unnamed data label (lbl_XXXXXXXX),
    no addend on either side: just naming. A call, a named global, or a
    differing addend is a possible wrong target and stays unverified."""
    return bool(pairs) and all(LOCAL_LIT.match(b) and ORIG_LBL.match(a) for a, b in pairs)


def load_try():
    spec = importlib.util.spec_from_file_location("fsa_try", Path(__file__).resolve().parent / "try.py")
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def is_done(cls):
    parts = cls.split(" + ")
    return all(p in DONE for p in parts) and parts[0] in DONE


def mtime(p):
    try:
        return os.stat(p).st_mtime_ns
    except OSError:
        return 0


def unit_stamp(u):
    return [mtime(u.get("base_path", "")), mtime(u.get("target_path", "")), VERSION]


def diff_unit(T, u):
    """{function: [pct, bytes, class, detail-count]} for the unit's non-100 % functions."""
    cmd = [T.OBJDIFF, "diff", "-p", str(HONEST), "-u", u["name"], "-o", "-", "-c", T.CONFIG]
    out = subprocess.run(cmd, capture_output=True, text=True)
    if out.returncode != 0:
        raise RuntimeError(f"{u['name']}: {out.stderr.strip()[-300:]}")
    d = json.loads(out.stdout)
    left, right = d["left"], d["right"]
    res = {}
    for s in left["symbols"]:
        if s.get("kind") != "SYMBOL_FUNCTION" or s.get("match_percent") == 100:
            continue
        r = s.get("target_symbol")
        r = right["symbols"][r] if r is not None else None
        size = int(s.get("size", 0))
        if r is None:
            res[s["name"]] = [0.0, size, "missing", 0]
            continue
        t, o = T.insn_list(left, s), T.insn_list(right, r)
        rp = T.reloc_pairs(s, r, left, right)
        cls, _ = T.classify(t, o, rp)
        if "reloc-name" in cls and not naming_only(rp):
            cls = cls.replace("reloc-name", "reloc-name-unverified")
        res[s["name"]] = [float(s.get("match_percent") or 0), size, cls, len(o) - len(t)]
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-j", type=int, default=min(16, os.cpu_count() or 4))
    ap.add_argument("--no-cache", action="store_true")
    args = ap.parse_args()
    T = load_try()
    cfg = json.loads((HONEST / "objdiff.json").read_text())
    units = [u for u in cfg["units"] if u.get("base_path") and Path(u["base_path"]).exists()]
    try:
        cache = {} if args.no_cache else json.loads(CACHE.read_text())
    except (OSError, ValueError):
        cache = {}

    todo, results = [], {}
    for u in units:
        c = cache.get(u["name"])
        if c and c["stamp"] == unit_stamp(u):
            results[u["name"]] = c["fns"]
        else:
            todo.append(u)
    with ThreadPoolExecutor(args.j) as ex:
        for u, fns in zip(todo, ex.map(lambda u: diff_unit(T, u), todo)):
            results[u["name"]] = fns
            cache[u["name"]] = {"stamp": unit_stamp(u), "fns": fns}
    live = {u["name"] for u in units}
    CACHE.write_text(json.dumps({k: v for k, v in cache.items() if k in live}, separators=(",", ":")))

    m = json.loads(REPORT.read_text())["measures"]
    honest = {"functions": m.get("matched_functions", 0), "code": int(m.get("matched_code", 0))}
    done, close, classes, out_units = [0, 0], [0, 0], {}, {}
    for name, fns in sorted(results.items()):
        if fns:
            out_units[name] = {f: v[:3] for f, v in fns.items()}
        for pct, size, cls, ninsn in fns.values():
            c = classes.setdefault(cls, [0, 0])
            c[0] += 1
            c[1] += size
            if is_done(cls):
                done[0] += 1
                done[1] += size
            elif cls == "missing/extra" and abs(ninsn) <= CLOSE_INSNS and pct >= CLOSE_PCT:
                close[0] += 1
                close[1] += size
    doc = {
        "total_functions": m["total_functions"], "total_code": int(m["total_code"]),
        "honest": honest,
        "structural": {"functions": honest["functions"] + done[0], "code": honest["code"] + done[1]},
        "done": {"functions": done[0], "code": done[1]},
        "close": {"functions": close[0], "code": close[1]},
        "classes": classes, "units": out_units,
    }
    (OUT / "structural.json").write_text(json.dumps(doc, separators=(",", ":")))
    print(f"structural: {doc['structural']['functions']} / {doc['total_functions']} functions "
          f"(honest {honest['functions']} + {done[0]} structurally done), "
          f"+{close[0]} close; {len(todo)}/{len(units)} units diffed")


if __name__ == "__main__":
    main()
