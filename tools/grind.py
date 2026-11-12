#!/usr/bin/env python3
"""Grind v2: lean, sandboxed, script-judged decomp sessions for every model tier.

  python3 tools/grind.py setup [--workers N]      # worker worktrees ../fsa-wt/g2-wN (branch grind2-wN)
  python3 tools/grind.py queue                     # what is left, by lane and tier
  python3 tools/grind.py packet <unit> <function>  # the task text a worker would get
  python3 tools/grind.py worker N --tier haiku|sonnet|opus [--lanes write,structure,match]
                                  [--max-size B] [--min-size B] [--per-unit K] [--sessions M]
  python3 tools/grind.py check [--full]            # inside a session: the verdict the judge would give now
  python3 tools/grind.py status                    # sessions, accept rate, tokens per tier
  touch ../fsa-wt/g2-logs/STOP                     # workers finish their session and exit

What changed from the first grinder (branch `grind`), each from its logs:
  * 33k tokens of fixed context per turn -> about 7k: sessions run with their own short
    system prompt, four tools, no CLAUDE.md, memory, skills or MCP servers.
  * 77 % of sessions hit permission denials (2,902 wasted turns) -> none: full Bash, inside
    Claude Code's sandbox (writes only in the worker's own worktree, no network).
  * 22 turns of exploring per small function -> the task text already carries the original
    assembly, our current source, try.py's diff, the behaviour to-do list from
    tools/fingerprint.py and where every callee is declared.
  * 56 % of the spend was thrown away by "a new 100 % or nothing" -> any improvement is
    kept: closer behaviour, or the same behaviour and a higher %, with nothing else worse.
  * rule breaks were found only after the session -> `grind.py check` gives the worker the
    judge's verdict while it can still fix things.
  * edits were confined to the unit and its header -> edit anything under src/, include/,
    libs/, symbols.txt and splits.txt; a change outside the unit is judged on the whole
    project (full build, SHA-1, no function anywhere worse, gate).

Lanes (tools/fingerprint.py): write = not in our object; structure = behaviour differs from
the original; match = behaviour equal, only codegen differs. Tiers start by size (<= 512 B
haiku, <= 2 KB sonnet, above that opus) and a function moves up one tier after a session
that kept nothing, or after three kept sessions that still left it unfinished.

Integration branch `grind2` (worktree ../fsa-wt/g2); `matching` is never touched. Logs:
../fsa-wt/g2-logs/ledger.tsv (one row per session), sessions/<tag>.json, rejects/<tag>.patch.
"""

import argparse
import datetime
import fcntl
import json
import os
import re
import signal
import subprocess
import sys
import threading
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))
import fingerprint as FP  # noqa: E402
import grind_broker as BR  # noqa: E402
import grind_style as ST  # noqa: E402

sh = ST.sh
MAIN = Path(sh(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"], REPO).stdout.strip()).parent
WT = MAIN.parent / "fsa-wt"
INTEG = WT / "g2"
LOGS = WT / "g2-logs"
BRANCH = "grind2"
TIERS = {"haiku": ("claude-haiku-5-5", "Claude Haiku 5.5"),
         "sonnet": ("claude-sonnet-5-5", "Claude Sonnet 5.5"),
         "opus": ("claude-opus-5-5", "Claude Opus 5.5")}
ORDER = ["haiku", "sonnet", "opus"]
LANES = ["write", "structure", "match"]
MISSING = 10 ** 6
FLOOR = 40.0  # a newly written function below this is guesswork
EPS = 0.005
ALLOWED_DIRS = ("src/", "include/", "libs/")
ALLOWED_FILES = {"config/G4SE01/symbols.txt", "config/G4SE01/splits.txt", "config/G4SE01/gen_game.tsv"}
IGNORED = {"objdiff.json", "progress/claims.json"}
SANDBOX = {"sandbox": {"enabled": True, "failIfUnavailable": True, "allowUnsandboxedCommands": False,
                       "autoAllowBashIfSandboxed": True, "network": {"allowedDomains": []},
                       "filesystem": {"denyRead": ["~/.ssh", "~/.aws", "~/.gnupg", "~/.claude", "~/.config"]}}}

SYSTEM = """You are a decompilation worker on The Legend of Zelda: Four Swords Adventures (GameCube). The repository in your working directory is your private copy: read, run and edit anything in it. A script keeps your changes only if its checks pass, so you cannot damage anything.

Goal: C++ under src/ that Metrowerks GC/2.6 (-O4,p, deferred inlining) compiles to the original code. Behaviour first (the same calls, constants, compares and fields as the original), then the same bytes.

How to work
1. The task below already contains the original assembly, our current source, the diff and what differs. Don't re-read those; start editing.
2. After an edit run `python3 tools/try.py <unit> <function>`: it compiles (about 2 s) and prints the match %, the kind of difference and the first differing rows.
3. One idea per try. Put independent tool calls in one turn. Keep going while tries improve; stop after 3 tries in a row without progress, or when try.py prints STOP.
4. Before you finish run `python3 tools/grind.py check`. It prints the verdict the judging script will give and what to fix; fix that and run it again. Finish when it says ACCEPT, or when you cannot improve further (leave the best state you reached; revert experiments that made things worse).

What is kept: any improvement of the task's function (behaviour closer to the original, or equal behaviour and a higher %), provided nothing else got worse. Partial progress counts. What is thrown away: a change that lowers another function, breaks the build or breaks a rule.

Rules (checked by script)
- Natural C++ as the original programmers wrote it: real types, real members. No asm, no goto, no volatile, no #pragma except an evidenced `#pragma auto_inline off/reset` region (docs/RULES_CARD.md), no casts, temporaries or reordered statements whose only purpose is to steer codegen, no dummy functions, no byte-offset access (reinterpret_cast, (u8*)p + N): declare the field in its class.
- Literals stay literals (1.0f, "text", 'NULL'); never an lbl_ name or a one-element array for a float or string of this unit.
- A function that takes an object first is a member of that object's class; a function the assembly lists in a __vt__ table is virtual on that class. Never extern "C" with a class or void* self parameter, never an extern "C" definition in a .cpp.
- Game .cpp files list their functions in DESCENDING address order (highest address first). Keep it.
- Keep fn_/lbl_/cls_ placeholder names unless you have evidence for a real one. Declare another unit's function once, in its owner's shared header under include/game/, never in a .cpp.
- Shared headers and other files may be changed when the assembly proves the change (a field's type or offset, const, a missing member). Then the whole project is rebuilt and nothing anywhere may get worse.
- Don't write FIXME percentages (the script stamps them); a one-line comment on what still differs is welcome. No progress logs in source.
- Don't run git or ninja. Only `python3 tools/try.py ...` and `python3 tools/grind.py check` can compile: the compiler does not run directly in your sandbox, and tools/ and configure.py must stay unedited.

Useful
- `python3 tools/fingerprint.py <unit> <function>`: behaviour differences (calls, constants, compares, field offsets), original vs ours.
- `python3 tools/try.py <unit>`: every function of the unit below 100 %.
- build/G4SE01/asm/game/<unit>.s: the unit's whole original assembly (other functions, data, vtables, jump tables).
- docs/RULES_CARD.md, "Known game-code shapes": recurring MWCC shapes and their natural source. grep it for a keyword; don't read it whole.
- Functions already at 100 % in the same file and include/game/ headers show the style and the classes.

End with one line: DONE <final %> or STUCK <why>."""

LANE_TEXT = {
    "write": "It is not in our object yet. Write it in src/{unit}, at its place in the file's descending address "
             "order ({place}). Get the behaviour right first; the % follows.",
    "structure": "Our code does not do what the original does. The list below names each behaviour difference "
                 "(original vs ours): fix those first, then look at the %.",
    "match": "Its behaviour already equals the original (same calls, constants, compares and fields); only codegen "
             "differs ({cls}). Don't change behaviour. Honest levers that move MWCC codegen: the order of local "
             "declarations, a value in its own local vs used directly, operand order of commutative expressions, "
             "signedness and width of locals and counters, if/else vs early return, loop form, a header inline vs "
             "the spelled-out expression, const on locals and parameters. Cheap ones first.",
}
RELOC_TEXT = ("Its instructions are identical to the original; only a relocation names a different symbol (the pairs "
              "are in try.py's output). `lbl_... vs @NNN` means this unit's literals are pooled in a different order "
              "than the original's, or the literal is not split into this unit (config/G4SE01/splits.txt); a named "
              "global means the wrong symbol.")


# ---------------------------------------------------------------- small helpers

def now():
    return datetime.datetime.now().isoformat(timespec="seconds")


def notify(title, body=""):
    subprocess.run(["tm-notify", title, body, "--tag", "fsa-grind"], capture_output=True)


def head(wt):
    return sh(["git", "rev-parse", "HEAD"], wt, check=True).stdout.strip()


def ledger_rows():
    try:
        return [l.rstrip("\n").split("\t") for l in open(LOGS / "ledger.tsv")]
    except OSError:
        return []


COLS = ["ts", "worker", "tier", "lane", "unit", "fn", "size", "pct0", "score0", "pct1", "score1", "verdict", "why",
        "turns", "in", "cache_write", "cache_read", "out", "cost", "secs"]


def log(row):
    LOGS.mkdir(parents=True, exist_ok=True)
    with open(LOGS / "ledger.tsv", "a") as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        f.write("\t".join(str(x).replace("\t", " ").replace("\n", " ") for x in [now()] + row) + "\n")


# ---------------------------------------------------------------- evaluation

def evaluate(wt, unit):
    """tools/fingerprint.py's view of a unit, with every function keyed by its address in the
    target object (stable across renames). None when the unit has no compiled object."""
    ev = FP.unit_eval(unit, wt)
    if ev is None:
        return None
    return ev


def by_addr(ev):
    return {e["addr"]: dict(e, name=n) for n, e in ev["functions"].items()}


def totals(ev):
    f = ev["functions"].values()
    return {"n100": sum(e["pct"] >= 100 for e in f), "missing": sum(e["cls"] == "missing" for e in f),
            "score": sum(e["score"] for e in f if e["score"] < MISSING),
            "credit": sum(e["size"] * e["pct"] / 100 for e in f)}


def lane_of(e):
    if e["cls"] == "missing":
        return "write"
    return "structure" if e["grade"] == "drift" else "match"


def start_tier(size):
    return 0 if size <= 512 else 1 if size <= 2048 else 2


def tier_index(size, hist):
    """Which tier a function is at now (len(ORDER) = parked), from its ledger rows (tier, verdict)."""
    t = start_tier(size)
    for tier, verdict in hist:
        i = ORDER.index(tier)
        if i >= t and verdict != "ACCEPT":
            t = i + 1
    while t < len(ORDER) and sum(1 for tier, v in hist if tier == ORDER[t] and v == "ACCEPT") >= 3:
        t += 1
    return t


def history():
    h = {}
    for r in ledger_rows():
        d = dict(zip(COLS, r))
        if d.get("verdict") in ("ACCEPT", "REJECT") and d.get("tier") in TIERS:
            h.setdefault((d["unit"], d["fn"]), []).append((d["tier"], d["verdict"]))
    return h


# ---------------------------------------------------------------- queue

def game_units(wt):
    units = json.loads((wt / "objdiff.json").read_text())["units"]
    out = []
    for u in units:
        src = (u.get("metadata") or {}).get("source_path") or ""
        if "/game/" in u["name"] and u.get("base_path") and (wt / u["base_path"]).exists() and src.startswith("src/"):
            out.append((u["name"], src[4:]))
    return out


def build_queue(force=False):
    """{unit source: {function: {pct,size,cls,grade,score,addr}}} for every game function below
    100 %, from the integration worktree; cached per integration commit."""
    LOGS.mkdir(parents=True, exist_ok=True)
    cache, h = LOGS / "queue.json", head(INTEG)
    if not force and cache.exists():
        d = json.loads(cache.read_text())
        if d.get("head") == h:
            return d["units"]
    units = game_units(INTEG)

    def one(u):
        try:
            ev = evaluate(INTEG, u[0])
        except Exception:
            return u[1], {}
        return u[1], {n: {k: e[k] for k in ("pct", "size", "cls", "grade", "score", "addr")}
                      for n, e in (ev or {"functions": {}})["functions"].items() if e["pct"] < 100}
    with ThreadPoolExecutor(12) as ex:
        res = {src: fns for src, fns in ex.map(one, units) if fns}
    cache.write_text(json.dumps({"head": h, "units": res}))
    return res


def eligible(fns, unit, tier, lanes, lo, hi, hist):
    out = []
    for n, e in fns.items():
        lane = lane_of(e)
        if lane in lanes and lo <= e["size"] <= hi and tier_index(e["size"], hist.get((unit, n), [])) == ORDER.index(tier):
            out.append((LANES.index(lane), e["size"], n))
    return [n for _, _, n in sorted(out)]


def cmd_queue(_):
    q, hist = build_queue(), history()
    tab = {}
    for unit, fns in q.items():
        for n, e in fns.items():
            t = tier_index(e["size"], hist.get((unit, n), []))
            k = (lane_of(e), ORDER[t] if t < len(ORDER) else "parked")
            c = tab.setdefault(k, [0, 0])
            c[0] += 1
            c[1] += e["size"]
    print(f"{'lane':10} {'tier':7} {'functions':>9} {'bytes':>9}")
    for lane in LANES:
        for tier in ORDER + ["parked"]:
            if (lane, tier) in tab:
                print(f"{lane:10} {tier:7} {tab[(lane, tier)][0]:9} {tab[(lane, tier)][1]:9}")


# ---------------------------------------------------------------- source lookup and FIXME stamps

def find_def(lines, demangled):
    """(first line, last line) of a function definition in a source file, 0-based, or None.
    `demangled` is objdiff's name, e.g. `cls_804A4C58::fn_801D95F4()` or `fn_80139994(int, void*)`."""
    parts = demangled.split("(")[0].strip().split("::")
    name, cls = parts[-1], (parts[-2].split("<")[0] if len(parts) > 1 else None)
    if not re.match(r"^[~\w]+$", name):
        return None
    rx = re.compile((rf"\b{re.escape(cls)}\s*::\s*" if cls else r"(?<![\w:>.~])") + re.escape(name) + r"\s*\(")
    depth, hits = 0, []
    for i, raw in enumerate(lines):
        code = ST.code_part(raw)
        if depth == 0 and rx.search(code):
            rest = code[rx.search(code).end():] + " " + " ".join(ST.code_part(l) for l in lines[i + 1:i + 12])
            m = re.search(r"[{;]", rest)
            if m and m.group() == "{":
                hits.append(i)
        depth += code.count("{") - code.count("}")
        depth = max(depth, 0)
    if len(hits) != 1:
        return None
    start, depth, seen = hits[0], 0, False
    for j in range(start, len(lines)):
        code = ST.code_part(lines[j])
        depth += code.count("{") - code.count("}")
        seen = seen or "{" in code
        if seen and depth <= 0:
            return start, j
    return None


CLS = (r"(?:missing/extra|missing|regalloc-only|scheduling\+regalloc|scheduling/regalloc|scheduling|different|operands"
       r"|same-text|reloc-name-unverified|reloc-name|reloc-data|matches)")
FIXME = re.compile(r"^(\s*//\s*FIXME(?:\([^)]*\))?:\s*)[\d.]+\s*%(.*)$")
OLD_STATS = re.compile(rf"^,?\s*(?:{CLS}(?:\s*\+\s*{CLS})*)?(?:;\s*behaviour \w+)?")


def stamp_fixmes(wt, unit, ev):
    """Write the measured `// FIXME: <pct> %, <class>; behaviour <grade>` above every function
    below 100 % and drop the FIXME of every function at 100 %. Returns True if the file changed."""
    path = wt / "src" / unit
    lines = path.read_text(errors="replace").split("\n")
    edits = []
    for name, e in ev["functions"].items():
        if e["cls"] == "missing":
            continue
        d = find_def(lines, e["demangled"])
        if d is None:
            continue
        top = d[0]
        while top > 0 and lines[top - 1].lstrip().startswith("//"):
            top -= 1
        edits.append((d[0], top, e))
    for start, top, e in sorted(edits, key=lambda x: -x[0]):
        block = range(top, start)
        at = next((i for i in block if re.match(r"\s*//\s*FIXME", lines[i])), None)
        if e["pct"] >= 100:
            if at is not None:
                del lines[at:start]
            continue
        stats = f"{e['pct']:.2f} %, {e['cls']}; behaviour {e['grade']}"
        if at is None:
            lines.insert(start, f"// FIXME: {stats}.")
            continue
        m = FIXME.match(lines[at])
        if m:  # keep the author's note, replace the numbers and the class
            note = OLD_STATS.sub("", m.group(2), count=1).lstrip(" .:;,")
            lines[at] = f"{m.group(1)}{stats}." + (f" {note}" if note else "")
        else:
            lines.insert(at, f"// FIXME: {stats}.")
    new = "\n".join(lines)
    if new == path.read_text(errors="replace"):
        return False
    path.write_text(new)
    return True


# ---------------------------------------------------------------- task text

def asm_block(wt, unit, fn):
    asm = wt / "build/G4SE01/asm/game" / (Path(unit).stem + ".s")
    out, on = [], False
    if not asm.exists():
        return out
    for line in asm.read_text(errors="replace").splitlines():
        if line.startswith(f".fn {fn},"):
            on = True
            continue
        if on:
            if line.startswith(f".endfn {fn}"):
                break
            out.append(re.sub(r"^/\* \w+ \w+  [0-9A-F ]+\*/\s*", "  ", line))
    return out


def declarations(wt, unit, fn, asm):
    calls = sorted({m.group(2) or m.group(1) for l in asm for m in [re.search(r"\bbl\s+(\"([^\"]+)\"|\S+)", l)] if m})
    out = []
    cls = ST.class_of(fn)
    if cls:
        hdr = sh(["git", "grep", "-l", "-E", rf"(class|struct) {re.escape(cls)}\b", "--", "include"], wt).stdout.split()
        out.append(f"class {cls}: {', '.join(hdr[:2]) or 'no header yet'}")
    for sym in calls[:24]:
        short = sym.split("__")[0] if not sym.startswith("__") else sym
        hit = [h.split(":", 2) for h in sh(["git", "grep", "-n", "-w", "-F", short, "--", "include", "libs"], wt).stdout.splitlines()]
        hit = [h for h in hit if len(h) == 3 and f"{short}(" in h[2].replace(" (", "(") and not h[2].lstrip().startswith(("//", "*", "/*"))]
        if hit:
            out.append(f"{short}: {hit[0][0]}:{hit[0][1]}: {hit[0][2].strip()[:110]}")
        else:
            out.append(f"{sym[:90]}: not declared in include/ yet")
    return out


def build_packet(wt, unit, fn, ev):
    e = ev["functions"][fn]
    lane = lane_of(e)
    fns = sorted(ev["functions"].items(), key=lambda kv: kv[1]["addr"])
    idx = next(i for i, (n, _) in enumerate(fns) if n == fn)
    hi = fns[idx + 1][0].split("__")[0] if idx + 1 < len(fns) else None
    lo = fns[idx - 1][0].split("__")[0] if idx > 0 else None
    place = "; ".join(x for x in [f"after `{hi}` in the file" if hi else "", f"before `{lo}`" if lo else ""] if x) or "the only function"
    text = LANE_TEXT[lane].format(unit=unit, place=place, cls=e["cls"])
    if lane == "match" and e["cls"] == "reloc-name":
        text = RELOC_TEXT
    out = [f"TASK ({lane}): `{fn}` = `{e['demangled']}`, {e['size']} bytes, unit `{unit}` (src/{unit}).",
           f"Now: {e['pct']:.2f} %, {e['cls']}; behaviour {e['grade']}.", text, ""]
    if lane != "write":
        if e["todo"]:
            out += ["== Behaviour differences (tools/fingerprint.py) =="] + e["todo"][:30]
            if len(e["todo"]) > 30:
                out.append(f"... and {len(e['todo']) - 30} more (run tools/fingerprint.py)")
            out.append("")
        t = sh(["python3", "tools/try.py", unit, fn], wt).stdout.strip().splitlines()
        out += ["== tools/try.py now (target | ours) =="] + t[-18:] + [""]
    src = wt / "src" / unit
    if src.exists():
        lines = src.read_text(errors="replace").split("\n")
        d = find_def(lines, e["demangled"])
        if d:
            top = d[0]
            while top > 0 and lines[top - 1].lstrip().startswith("//"):
                top -= 1
            state = " (not compiled: inside #ifdef NONMATCHING or failing)" if lane == "write" else ""
            out += [f"== Our source{state}: src/{unit} lines {top + 1}-{d[1] + 1} =="] + lines[top:d[1] + 1] + [""]
    asm = asm_block(wt, unit, fn)
    out += [f"== Original assembly ({len(asm)} lines; build/G4SE01/asm/game/{Path(unit).stem}.s) =="] + asm + [""]
    out += ["== Where its callees and class are declared =="] + declarations(wt, unit, fn, asm)
    return "\n".join(out) + "\n"


def cmd_packet(args):
    sh(["python3", "tools/try.py", args.unit], REPO)
    ev = evaluate(REPO, args.unit)
    fn = args.function if args.function in ev["functions"] else next(
        (n for n in ev["functions"] if n.split("__")[0] == args.function), None)
    if fn is None:
        sys.exit("no such function")
    sys.stdout.write(build_packet(REPO, args.unit, fn, ev))


# ---------------------------------------------------------------- judge

def changed_paths(wt):
    st = sh(["git", "status", "--porcelain", "-uall"], wt).stdout.splitlines()
    return [(l[:2], l[3:].split(" -> ")[-1]) for l in st if l[3:] not in IGNORED and not l[3:].startswith(".")]


def is_allowed(path, wt):
    if path in ALLOWED_FILES:
        return True
    return path.startswith(ALLOWED_DIRS) and path.endswith((".cpp", ".c", ".h", ".hpp", ".inc"))


def discard_outside(wt, dry=False):
    """Undo everything outside the editable paths (scratch files, tool or doc edits). Returns
    the discarded paths."""
    dropped, cfg = [], (wt / "configure.py").read_text()
    for st, p in changed_paths(wt):
        new = st == "??"
        ok = is_allowed(p, wt)
        if ok and new:  # a new file: a header, or a source file the build already lists; anything else is scratch
            ok = p.endswith((".h", ".hpp")) or (p.startswith("src/") and f'"{p[4:]}"' in cfg)
        if ok:
            continue
        dropped.append(p)
        if not dry:
            if new:
                (wt / p).unlink(missing_ok=True)
            else:
                sh(["git", "checkout", "-q", "--", p], wt)
    return dropped


def release_generated(wt, unit):
    """A hand-edited generated unit leaves the generator: drop its gen_game.tsv rows."""
    if not sh(["git", "status", "--porcelain", "--", f"src/{unit}"], wt).stdout.strip():
        return
    tsv = wt / "config/G4SE01/gen_game.tsv"
    lines = tsv.read_text().splitlines(keepends=True)
    keep = [l for l in lines if l.split("\t")[0] != Path(unit).stem]
    if len(keep) != len(lines):
        tsv.write_text("".join(keep))


def report_map(wt):
    try:
        rep = json.loads((wt / "build/G4SE01/report_honest.json").read_text())
    except (OSError, ValueError):
        return None
    return {(u["name"], f["name"]): float(f.get("fuzzy_match_percent") or 0)
            for u in rep["units"] for f in u.get("functions", [])}


def rule_problem(wt, unit):
    added = [l for l in sh(["git", "diff", "-U0", "--", "src", "include", "libs"], wt).stdout.splitlines()
             if l.startswith("+") and not l.startswith("+++")]
    for f in sh(["git", "ls-files", "--others", "--exclude-standard", "src", "include", "libs"], wt).stdout.splitlines():
        added += ["+" + l for l in (wt / f).read_text(errors="replace").splitlines()]
    for l in added:
        code = ST.code_part(l)
        if ST.C_TAKES_CLASS.search(l):
            return f'extern "C" taking a class pointer (write a member): {l[1:].strip()[:80]}'
        if ST.BANNED.search(code) or ST.PRAGMA.search(code):
            return f"banned construct: {l[1:].strip()[:80]}"
    return ST.style_check(wt, unit)


def judge(wt, unit, base, focus_addr, full=None, dry=False):
    """(why rejected or None, new evaluation or None, notes). `base` is the evaluation before
    the session. full=None decides by what changed; dry=True changes nothing (the worker's check)."""
    notes = []
    dropped = discard_outside(wt, dry)
    if dropped:
        notes.append("not kept (outside src/, include/, libs/, symbols.txt, splits.txt): " + ", ".join(dropped[:4]))
    paths = [p for _, p in changed_paths(wt) if p not in dropped]
    if not paths:
        return "no change", None, notes
    if not dry:
        release_generated(wt, unit)
    why = rule_problem(wt, unit)
    if why:
        return why, None, notes
    obj = wt / "build/G4SE01/src" / unit.replace(".cpp", ".o")
    if not dry:
        obj.unlink(missing_ok=True)  # a compiled object the session left behind proves nothing
    r = sh(["timeout", "900", "python3", "tools/try.py", unit], wt)
    if "compile failed" in r.stdout or "split failed" in r.stdout or not obj.exists():
        tail = " | ".join(l.strip() for l in r.stdout.strip().splitlines()[-6:])
        return f"does not build: {tail[:400]}", None, notes
    ev = evaluate(wt, unit)
    old, new = by_addr(base), by_addr(ev)
    lost = [e["name"] for a, e in new.items() if old.get(a, {}).get("pct", 0) >= 100 > e["pct"]]
    if lost:
        return f"lost matched function(s): {', '.join(lost[:3])}", ev, notes
    worse = [e["name"] for a, e in new.items() if a in old and e["score"] > old[a]["score"]]
    if worse:
        return f"behaviour moved away from the original in: {', '.join(worse[:3])} (tools/fingerprint.py)", ev, notes
    secs = [k for k, p in base["sections"].items() if not k.startswith("extab") and ev["sections"].get(k, 0) < p - EPS]
    low = [e["name"] for a, e in new.items() if e["cls"] != "missing" and e["pct"] < FLOOR
           and old.get(a, {}).get("cls") == "missing"]
    if low:
        return f"newly written {low[0]} is below {FLOOR:.0f} % (guesswork: get its behaviour right or leave it out)", ev, notes
    t0, t1 = totals(base), totals(ev)
    f0, f1 = old.get(focus_addr), new.get(focus_addr)
    focus_better = bool(f0 and f1 and (f1["score"] < f0["score"] or (f1["score"] == f0["score"] and f1["pct"] > f0["pct"] + EPS)))
    better = (focus_better or t1["n100"] > t0["n100"] or t1["missing"] < t0["missing"]
              or (t1["score"] < t0["score"] and t1["credit"] >= t0["credit"] - 2)
              or (t1["score"] == t0["score"] and t1["credit"] > t0["credit"] + 2))
    if secs and not t1["n100"] > t0["n100"]:
        return f"data section(s) got worse: {', '.join(secs)}", ev, notes
    if t1["credit"] < t0["credit"] - 2 and not focus_better:
        return f"the unit as a whole got worse ({t0['credit']:.0f} -> {t1['credit']:.0f} matched bytes)", ev, notes
    if not better:
        return "no improvement", ev, notes
    changed = {e["name"] for a, e in new.items() if a not in old or abs(e["pct"] - old[a]["pct"]) > EPS}
    written = [e["name"] for a, e in new.items() if e["cls"] != "missing" and old.get(a, {}).get("cls") == "missing"]
    why = ST.order_problem(wt, unit, changed) or ST.vtable_owner_problem(wt, {}, written)
    if why:
        return why, ev, notes
    own = {f"src/{unit}", f"include/game/act/{Path(unit).stem}.h", "config/G4SE01/gen_game.tsv"}
    wide = [p for p in paths if p not in own]
    if wide and full is None:
        full = not dry
        if dry:
            notes.append(f"you changed shared files ({', '.join(wide[:3])}): after you finish the whole project is "
                         "rebuilt and nothing anywhere may get worse (`python3 tools/grind.py check --full`, ~1 min, runs it now)")
    if full:
        why = wide_problem(wt, unit)
        if why:
            return why, ev, notes
    elif "check: ok" not in sh(["python3", "tools/gen_game.py", "--check"], wt).stdout:
        return "breaks gen_game --check (a generated function or header was edited)", ev, notes
    return None, ev, notes


def wide_problem(wt, unit):
    """The whole project after a change outside the unit: builds, links byte-identical, nothing
    anywhere lower than at the start of the unit, generator and gate clean."""
    base = json.loads((wt / "build/grind/report_base.json").read_text())
    base = {tuple(k.split("\t")): v for k, v in base.items()}
    r = sh("python3 configure.py >/dev/null && timeout 3000 ninja", wt)
    if r.returncode:
        bad = [l for l in r.stdout.splitlines() if l.startswith(("FAILED", "###", "#   "))][:4]
        return "the project no longer builds or links byte-identical: " + " | ".join(bad)[:400]
    rep = report_map(wt)
    name = next((u for u, src in game_units(wt) if src == unit), None)
    worse = sorted(f"{u.split('/')[-1]}:{f.split('__')[0]}" for (u, f), p in base.items()
                   if u != name and rep.get((u, f), 0) < p - EPS)
    if worse:
        return f"{len(worse)} function(s) in other units got worse: {', '.join(worse[:4])}"
    if "check: ok" not in sh(["python3", "tools/gen_game.py", "--check"], wt).stdout:
        return "breaks gen_game --check (a generated function or header was edited)"
    g = sh(["python3", "tools/gate.py", "--all-units", "--base", BRANCH], wt).stdout
    if "gate: ok" not in g:
        return "gate: " + (g.strip().splitlines() or ["failed"])[-1][:300]
    return None


def describe(base, ev, focus_addr):
    f0, f1 = by_addr(base).get(focus_addr), by_addr(ev).get(focus_addr)
    if not f0 or not f1:
        return ""
    s = lambda e: "unwritten" if e["cls"] == "missing" else f"{e['pct']:.2f} % {e['grade']}"
    return f"{f1['name'].split('__')[0]}: {s(f0)} -> {s(f1)}"


def cmd_check(args):
    BR.maybe_delegate("check", ["--full"] if args.full else [])
    wt = Path.cwd()
    task = json.loads((wt / "build/grind/task.json").read_text())
    why, ev, notes = judge(wt, task["unit"], task["base"], task["addr"], full=True if args.full else None, dry=True)
    line = describe(task["base"], ev, task["addr"]) if ev else ""
    print(f"check: {'REJECT: ' + why if why else 'ACCEPT'}" + (f"  ({line})" if line else ""))
    for n in notes:
        print("note: " + n)
    if ev and not why:
        f1 = by_addr(ev).get(task["addr"])
        if f1 and f1["pct"] < 100:
            print(f"still open: {f1['cls']}; behaviour {f1['grade']}" + (f" ({len(f1['todo'])} differences)" if f1["todo"] else ""))


# ---------------------------------------------------------------- sessions

def limits(size):
    """(max turns, wall seconds) for a function of this size."""
    if size <= 128:
        return 14, 600
    if size <= 512:
        return 22, 1200
    if size <= 2048:
        return 34, 2400
    return 48, 3600


def run_session(wt, tier, packet, size, tag, effort=None):
    turns, timeout = limits(size)
    settings = LOGS / "sandbox.json"
    settings.write_text(json.dumps(SANDBOX))
    cmd = ["claude", "-p", "--model", TIERS[tier][0], "--output-format", "stream-json", "--verbose",
           "--no-session-persistence", "--safe-mode", "--strict-mcp-config", "--disable-slash-commands",
           "--tools", "Bash,Read,Edit,Write", "--system-prompt", SYSTEM, "--settings", str(settings),
           "--permission-mode", "acceptEdits", "--allowedTools", "Bash,Read,Edit,Write",
           "--disallowedTools", "Bash(git *)", "--max-turns", str(turns)]
    if effort:
        cmd += ["--effort", effort]
    env = {k: v for k, v in os.environ.items() if k != "ANTHROPIC_API_KEY"}
    t0 = time.time()
    stop = threading.Event()
    broker = threading.Thread(target=BR.serve, args=(wt, stop), daemon=True)
    broker.start()
    try:
        out = subprocess.run(cmd, cwd=wt, input=packet, capture_output=True, text=True, timeout=timeout, env=env).stdout
        timed_out = False
    except subprocess.TimeoutExpired as e:
        out = e.stdout.decode(errors="replace") if isinstance(e.stdout, bytes) else (e.stdout or "")
        timed_out = True
    finally:
        stop.set()
        broker.join(timeout=1600)
    result, usage, seen, nturn, denials = {}, {}, set(), 0, 0
    for line in out.splitlines():
        try:
            ev = json.loads(line)
        except ValueError:
            continue
        if ev.get("type") == "result":
            result = ev
        elif ev.get("type") == "assistant":
            msg = ev.get("message") or {}
            if msg.get("id") in seen:
                continue
            seen.add(msg.get("id"))
            nturn += 1
            for k, v in (msg.get("usage") or {}).items():
                if isinstance(v, int):
                    usage[k] = usage.get(k, 0) + v
    u = result.get("usage") or usage
    denials = len(result.get("permission_denials") or [])
    d = LOGS / "sessions"
    d.mkdir(parents=True, exist_ok=True)
    (d / f"{tag}.json").write_text(json.dumps({"result": result or None, "timed_out": timed_out, "packet_chars": len(packet)}))
    (d / f"{tag}.stream.jsonl").write_text(out)
    return {"turns": result.get("num_turns", nturn), "in": u.get("input_tokens", 0),
            "cache_write": u.get("cache_creation_input_tokens", 0), "cache_read": u.get("cache_read_input_tokens", 0),
            "out": u.get("output_tokens", 0), "cost": result.get("total_cost_usd") or 0.0,
            "secs": int(time.time() - t0), "timed_out": timed_out, "denials": denials,
            "last": (result.get("result") or "").strip().splitlines()[-1:] or [""]}


def usage():
    """Seven-day plan utilization (0..1) from a minimal headless call, or None."""
    try:
        out = subprocess.run(["claude", "-p", "Reply: ok", "--model", TIERS["haiku"][0], "--output-format", "stream-json",
                              "--verbose", "--no-session-persistence", "--safe-mode", "--strict-mcp-config",
                              "--disable-slash-commands", "--tools", "", "--system-prompt", "Reply ok."],
                             cwd="/tmp", capture_output=True, text=True, timeout=180, stdin=subprocess.DEVNULL,
                             env={k: v for k, v in os.environ.items() if k != "ANTHROPIC_API_KEY"}).stdout
    except subprocess.TimeoutExpired:
        return None
    for line in out.splitlines():
        try:
            ev = json.loads(line)
        except ValueError:
            continue
        win = ((ev.get("rate_limit_info") or {}).get("unifiedWindows") or {}).get("seven_day") or {}
        if "utilization" in win:
            return float(win["utilization"])
    return None


# ---------------------------------------------------------------- worktrees, claims, integration

def make_worktree(path, branch, base):
    if path.exists():
        return
    sh(["git", "worktree", "add", "-q", str(path), "-B", branch, base], MAIN, check=True)
    os.symlink(MAIN / "orig", path / "orig")
    (path / "build").mkdir(exist_ok=True)
    for d in ("compilers", "binutils"):
        os.symlink(MAIN / "build" / d, path / "build" / d)
    sh(["cp", "-a", str(MAIN / "build/tools"), str(path / "build/tools")], MAIN, check=True)
    claims = path / "progress/claims.json"
    claims.unlink(missing_ok=True)
    os.symlink(MAIN / "progress/claims.json", claims)
    sh("python3 configure.py >/dev/null && timeout 3000 ninja >/dev/null 2>&1", path)


def cmd_setup(args):
    LOGS.mkdir(parents=True, exist_ok=True)
    for n in range(1, args.workers + 1):
        make_worktree(WT / f"g2-w{n}", f"{BRANCH}-w{n}", BRANCH)
        print(f"g2-w{n} ready")


def claim(unit, owner, wt):
    with open(LOGS / "claim.lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        claims = json.loads((MAIN / "progress/claims.json").read_text() or "{}")
        if unit in claims:
            return False
        sh(["python3", "tools/claim.py", "take", unit, owner, owner, str(wt)], MAIN, check=True)
        return True


def revert(wt):
    sh(["git", "checkout", "-q", "--", "."], wt)
    sh(["git", "clean", "-fdq", "--", "src", "include", "libs", "config"], wt)


def reset_to_integration(wt):
    sh(["git", "reset", "-q", "--hard", BRANCH], wt, check=True)
    sh(["git", "clean", "-fdq", "--", "src", "include", "libs", "config"], wt)
    r = sh("python3 configure.py >/dev/null && timeout 3000 ninja >/dev/null 2>&1", wt)
    if r.returncode:
        raise RuntimeError("integration state does not build in " + wt.name)
    rep = report_map(wt)
    (wt / "build/grind").mkdir(parents=True, exist_ok=True)
    (wt / "build/grind/report_base.json").write_text(json.dumps({"\t".join(k): v for k, v in rep.items()}))


def integrate(wt, branch, unit):
    """Bring the worker's kept commits into `grind2` and re-verify the whole project there."""
    with open(LOGS / "merge.lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        signal.signal(signal.SIGTERM, signal.SIG_IGN)  # never leave grind2 half-checked
        try:
            pre = head(INTEG)
            before = report_map(INTEG) or {}
            if sh(["git", "merge", "-q", "--no-edit", branch], INTEG).returncode:
                sh(["git", "merge", "--abort"], INTEG)
                return "merge conflict"
            r = sh("python3 configure.py >/dev/null && timeout 3000 ninja >/dev/null 2>&1", INTEG)
            why = "" if r.returncode == 0 else "build or SHA-1"
            if not why:
                after = report_map(INTEG) or {}
                n0, n1 = sum(p >= 100 for p in before.values()), sum(p >= 100 for p in after.values())
                if n1 < n0:
                    why = f"matched functions {n0} -> {n1}"
            if not why and "check: ok" not in sh(["python3", "tools/gen_game.py", "--check"], INTEG).stdout:
                why = "gen_game --check"
            if not why:
                g = sh(["python3", "tools/gate.py", "--all-units", "--base", pre], INTEG).stdout
                if "gate: ok" not in g:
                    why = "gate: " + (g.strip().splitlines() or ["failed"])[-1][:200]
            if why:
                sh(["git", "reset", "-q", "--hard", pre], INTEG)
                sh("python3 configure.py >/dev/null && timeout 3000 ninja >/dev/null 2>&1", INTEG)
                notify("FSA grind: merge rejected", f"{unit}: {why}")
                return f"rejected: {why}"
            return "merged"
        finally:
            signal.signal(signal.SIGTERM, lambda *_: sys.exit(1))


# ---------------------------------------------------------------- worker

def cmd_worker(args):
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(1))
    owner, tier = f"g2-w{args.n}", args.tier
    wt, branch = WT / owner, f"{BRANCH}-w{args.n}"
    lanes = args.lanes.split(",")
    if not wt.exists():
        sys.exit(f"{wt} missing: run `python3 tools/grind.py setup --workers {args.n}`")
    done_sessions = 0
    skip = set()
    while not (LOGS / "STOP").exists() and (not args.sessions or done_sessions < args.sessions):
        if args.usage_stop and done_sessions and done_sessions % 10 == 0:
            u = usage()
            if u is not None and u >= args.usage_stop:
                (LOGS / "STOP").touch()
                notify("FSA grind stopping", f"{owner}: weekly usage {u:.0%}")
                return
        q, hist = build_queue(), history()
        pool = []
        for unit, fns in q.items():
            if unit in skip or (args.unit and unit != args.unit):
                continue
            el = eligible(fns, unit, tier, lanes, args.min_size, args.max_size, hist)
            if el:
                pool.append((-len(el), unit))
        unit = next((u for _, u in sorted(pool) if claim(u, owner, wt)), None)
        if unit is None:
            notify("FSA grind", f"{owner} ({tier}): nothing left in its lanes")
            return
        skip.add(unit)
        kept = 0
        try:
            reset_to_integration(wt)
            sh(["timeout", "900", "python3", "tools/try.py", unit], wt)
            base = evaluate(wt, unit)
            todo = eligible({n: e for n, e in base["functions"].items() if e["pct"] < 100}, unit, tier, lanes,
                            args.min_size, args.max_size, hist)
            for fn in todo[:args.per_unit]:
                if (LOGS / "STOP").exists() or (args.sessions and done_sessions >= args.sessions):
                    break
                e = base["functions"].get(fn)
                if e is None or e["pct"] >= 100:
                    continue
                lane, addr = lane_of(e), e["addr"]
                packet = build_packet(wt, unit, fn, base)
                (wt / "build/grind/task.json").write_text(json.dumps({"unit": unit, "fn": fn, "addr": addr, "base": base}))
                (wt / "build/try" / (Path(unit).stem + ".json")).unlink(missing_ok=True)
                slug = "__".join(unit[:-4].split("/"))
                (wt / "build/try" / f"{slug}.json").unlink(missing_ok=True)
                tag = f"{datetime.datetime.now():%m%d-%H%M%S}_{tier}_{Path(unit).stem}__{re.sub(r'[^A-Za-z0-9_]', '_', fn)[:60]}"
                s = run_session(wt, tier, packet, e["size"], tag, args.effort)
                done_sessions += 1
                why, ev, notes = judge(wt, unit, base, addr)
                f1 = by_addr(ev).get(addr) if ev else None
                row = [owner, tier, lane, unit, fn, e["size"], f"{e['pct']:.2f}", e["score"] if e["score"] < MISSING else "-",
                       f"{f1['pct']:.2f}" if f1 else "-", (f1["score"] if f1 and f1["score"] < MISSING else "-")]
                stats = [s["turns"], s["in"], s["cache_write"], s["cache_read"], s["out"], f"{s['cost']:.4f}", s["secs"]]
                if why:
                    diff = sh(["git", "diff", "--", "src", "include", "libs", "config"], wt).stdout
                    if diff.strip() and why != "no change":
                        (LOGS / "rejects").mkdir(exist_ok=True)
                        (LOGS / "rejects" / f"{tag}.patch").write_text(f"# {why}\n{diff}")
                    revert(wt)
                    log(row + ["REJECT", why] + stats)
                    continue
                if stamp_fixmes(wt, unit, ev):
                    sh(["timeout", "900", "python3", "tools/try.py", unit], wt)
                    ev2 = evaluate(wt, unit)
                    if ev2 is None or totals(ev2) != totals(ev):  # a stamp must not change code
                        raise RuntimeError(f"FIXME stamp changed the build of {unit}")
                    ev = ev2
                sh(["git", "add", "-A", "--", "src", "include", "libs", "config/G4SE01/symbols.txt",
                    "config/G4SE01/splits.txt", "config/G4SE01/gen_game.tsv"], wt)
                msg = f"{unit[:-4]}: {describe(base, ev, addr)} ({lane} lane, grind)\n\nCo-Authored-By: {TIERS[tier][1]} <noreply@anthropic.com>"
                sh(["git", "commit", "-qm", msg], wt, check=True)
                kept += 1
                log(row + ["ACCEPT", describe(base, ev, addr)] + stats)
                base = ev
            result = integrate(wt, branch, unit) if kept else "nothing kept"
            log([owner, tier, "-", unit, "-", "-", "-", "-", "-", "-", "UNIT", result] + [""] * 7)
        except Exception as ex:  # keep the worker alive
            log([owner, tier, "-", unit, "-", "-", "-", "-", "-", "-", "ERROR", str(ex)[:300]] + [""] * 7)
            revert(wt)
        finally:
            sh(["python3", "tools/claim.py", "release", unit], MAIN)


# ---------------------------------------------------------------- status

def cmd_status(_):
    rows = [dict(zip(COLS, r)) for r in ledger_rows() if len(r) >= len(COLS)]
    sess = [r for r in rows if r["verdict"] in ("ACCEPT", "REJECT")]
    num = lambda r, k: float(r[k] or 0) if r[k] not in ("-", "") else 0.0
    print(f"{'tier':7} {'lane':10} {'sessions':>8} {'kept':>5} {'to 100%':>7} {'turns':>6} {'ctx/turn':>8} {'out':>7} {'cost':>8} {'$/kept':>7}")
    keys = sorted({(r["tier"], r["lane"]) for r in sess}, key=lambda k: (ORDER.index(k[0]), LANES.index(k[1])))
    for k in keys + [None]:
        rs = [r for r in sess if k is None or (r["tier"], r["lane"]) == k]
        if not rs:
            continue
        kept = [r for r in rs if r["verdict"] == "ACCEPT"]
        full = sum(1 for r in kept if num(r, "pct1") >= 100)
        turns = sum(num(r, "turns") for r in rs)
        ctx = sum(num(r, "cache_read") + num(r, "cache_write") + num(r, "in") for r in rs) / max(1, turns)
        cost = sum(num(r, "cost") for r in rs)
        name = k or ("all", "")
        print(f"{name[0]:7} {name[1]:10} {len(rs):8} {len(kept):5} {full:7} {turns / len(rs):6.1f} {ctx / 1000:7.1f}k "
              f"{sum(num(r, 'out') for r in rs) / len(rs) / 1000:6.1f}k {cost:8.2f} {cost / max(1, len(kept)):7.3f}")
    why = {}
    for r in sess:
        if r["verdict"] == "REJECT":
            w = re.sub(r"[\d.]+", "N", r["why"])[:60]
            why[w] = why.get(w, 0) + 1
    for w, n in sorted(why.items(), key=lambda x: -x[1])[:8]:
        print(f"  rejected {n:3}x: {w}")
    units = [r for r in rows if r["verdict"] == "UNIT"]
    if units:
        res = {}
        for r in units:
            res[r["why"]] = res.get(r["why"], 0) + 1
        print("units:", ", ".join(f"{k} {v}" for k, v in res.items()))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("setup"); p.add_argument("--workers", type=int, default=4); p.set_defaults(fn=cmd_setup)
    p = sub.add_parser("queue"); p.set_defaults(fn=cmd_queue)
    p = sub.add_parser("packet"); p.add_argument("unit"); p.add_argument("function"); p.set_defaults(fn=cmd_packet)
    p = sub.add_parser("check"); p.add_argument("--full", action="store_true"); p.set_defaults(fn=cmd_check)
    p = sub.add_parser("status"); p.set_defaults(fn=cmd_status)
    p = sub.add_parser("worker")
    p.add_argument("n", type=int)
    p.add_argument("--tier", choices=ORDER, required=True)
    p.add_argument("--lanes", default=",".join(LANES))
    p.add_argument("--min-size", type=int, default=0)
    p.add_argument("--max-size", type=int, default=10 ** 9)
    p.add_argument("--per-unit", type=int, default=8)
    p.add_argument("--sessions", type=int, default=0, help="stop after this many sessions (0 = until the queue is empty)")
    p.add_argument("--unit", help="only this unit (game/X.cpp)")
    p.add_argument("--effort", choices=["low", "medium", "high"])
    p.add_argument("--usage-stop", type=float, default=0.85, help="stop all workers at this weekly utilization (0 = off)")
    p.set_defaults(fn=cmd_worker)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
