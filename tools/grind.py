#!/usr/bin/env python3
"""Grind v2: lean, sandboxed, script-judged decomp sessions for every model tier.

  python3 tools/grind.py setup [--workers N]      # worker worktrees ../fsa-wt/g2-wN (branch grind2-wN)
  python3 tools/grind.py queue                     # what is left, by lane and tier
  python3 tools/grind.py packet <unit> <function>  # the task text a worker would get
  python3 tools/grind.py worker N --tier haiku|sonnet|opus [--mode function|file]
                                  [--lanes write,structure,match] [--max-size B] [--min-size B]
                                  [--per-unit K] [--sessions M] [--unit game/X.cpp]
  python3 tools/grind.py check [--full]            # inside a session: the verdict the judge would give now
  python3 tools/grind.py status                    # sessions, accept rate, tokens per tier
  python3 tools/grind.py watch --stop 0.10         # stop everything at this weekly utilization
  python3 tools/grind.py publish [--every N]       # reviewed grind2 work -> matching: verify, site data, push
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
the original; match = behaviour equal, only codegen differs. Workers take write and structure
by default (structure first); the match lane is opt-in. Tiers start by size (<= 512 B haiku,
<= 2 KB sonnet, above that opus). A function moves up one tier after a session that kept
nothing or whose change the reviewer threw out, and after 3 (6 above 2 KB, 10 above 8 KB) kept
sessions that still left it open: big functions are done in passes, each from a fresh context.

Sizes: the task text grows with the difference, not the function. A written function gets
objdiff's rows around each difference and its behaviour to-do list; the full assembly is inlined
only for small or unwritten functions (else its line range in the .s file).

Whole files (--mode file): one session gets a unit's open small functions (state, to-do, where
each sits; assembly inlined for the tiny ones) and is judged on the unit as a whole; while a
session is kept the next one starts fresh on what is left (--relay). For units with several
small open functions this shares the class and header reading between them.

Adversarial review: before a unit's kept changes are integrated, a separate reviewer session
(sonnet for haiku and sonnet authors, opus for opus; --review-tier overrides) reads each diff
against the original assembly and gives a verdict per change. It looks for what scripts can't see: statements that only move a metric, codegen
steering, fakes, wrong facts, guesswork. A change without an explicit ACCEPT is dropped, its
function moves up a tier and the reviewer's reason is given to the next attempt.

Integration branch `grind2` (worktree ../fsa-wt/g2). Workers never touch `matching`; `publish`
fast-forwards it to grind2 after the full checks and pushes (owner's instruction 2026-11-12). Logs:
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

What is kept: any improvement of the task's function (behaviour closer to the original, or equal behaviour and a higher %), provided nothing else got worse. Partial progress counts: the project wants faithful structure first, so a function at 60-80 % whose code does what the original does is a good result. What is thrown away: a change that lowers another function, breaks the build or breaks a rule.

After the scripts, an adversarial reviewer (a stronger model) reads every kept change against the original assembly and throws out trickery: a statement that exists only to move a number (a dead load, store, call or compare, duplicated code), a temporary, cast or reordering that only steers the compiler, logic the assembly does not have, a guessed type or an invented name. A higher % reached that way is worth nothing; an honest lower one is kept. What the reviewer has thrown out so far, so don't write it: a cast that changes nothing ((s32) on a u16 getter, `3u`) and `== true` / `== false`; a chained assignment or spelled-out stores where the class has set() or zero(); an assignment or comma expression inside a condition; a callee's body pasted in place of the call; identical if/else branches or a condition that cannot matter; a local added only to pin a register or to stop constant folding; a loop turned around or rewritten (for/while, counting down) without the assembly showing that order; a union or second struct that reads the same bytes another way; code that leans on an existing FAKEMATCH helper.

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
             "order ({place}). Get the behaviour right first; the % follows. If try.py then says `missing` and "
             "`ours, not in the target: <mangled name>`, the original still carries the placeholder name: rename it "
             "in config/G4SE01/symbols.txt to the mangled name your definition emits (change only the name before "
             "` = `) and update every other use (`grep -rnw <old name> src include`).",
    "structure": "Our code does not do what the original does. The list below names each behaviour difference "
                 "(original vs ours): fix those first, then look at the %.",
    "match": "Its behaviour already equals the original (same calls, constants, compares and fields); only codegen "
             "differs ({cls}). Don't change behaviour. Things that move MWCC codegen and still read as code a programmer "
             "wrote: the order of local declarations, naming a value in a local vs using the expression directly, "
             "operand order of commutative expressions, signedness and width of locals and counters, if/else vs "
             "early return, loop form, a header inline vs the spelled-out expression, const on locals and "
             "parameters. Cheap ones first. A temporary or reordering that only exists to move a register is a "
             "trick: don't leave one in.",
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
    # At 95 % and above what is left is mostly codegen (run 2026-11-12: sonnet and opus gained ~250-300
    # matched bytes per dollar there, against ~6,800 for writing unwritten functions): the match lane.
    return "structure" if e["grade"] == "drift" and e["pct"] < 95 else "match"


def start_tier(size):
    return 0 if size <= 512 else 1 if size <= 2048 else 2


def max_kept(size):
    """Kept sessions a tier may spend on one function before it moves up: big functions are done in passes."""
    return 3 if size <= 2048 else 6 if size <= 8192 else 10


def tier_index(size, hist):
    """Which tier a function is at now (len(ORDER) = parked), from its ledger rows (tier, verdict):
    up one tier after a session that kept nothing or whose change the reviewer threw out, and
    after max_kept() kept sessions that still left it open."""
    t = start_tier(size)
    for tier, verdict in hist:
        i = ORDER.index(tier)
        if i >= t and verdict != "ACCEPT":
            t = i + 1
    while t < len(ORDER) and sum(1 for tier, v in hist if tier == ORDER[t] and v == "ACCEPT") >= max_kept(size):
        t += 1
    return t


def history():
    h = {}
    for r in ledger_rows():
        d = dict(zip(COLS, r))
        if d.get("verdict") in ("ACCEPT", "REJECT", "TRICK") and d.get("tier") in TIERS:
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
    cache = LOGS / "queue.json"
    with open(LOGS / "merge.lock", "a") as lock:  # not while an integration is rebuilding the tree
        fcntl.flock(lock, fcntl.LOCK_SH)
        return _build_queue(cache, force)


def _build_queue(cache, force):
    h = head(INTEG)
    if not force and cache.exists():
        try:
            d = json.loads(cache.read_text())
            if d.get("head") == h and d.get("v") == 2:
                return d["units"]
        except ValueError:
            pass
    units = game_units(INTEG)

    def one(u):
        try:
            ev = evaluate(INTEG, u[0])
        except Exception:
            return u[1], {}
        return u[1], {n: {k: e[k] for k in ("pct", "size", "cls", "grade", "score", "addr", "demangled")}
                      for n, e in (ev or {"functions": {}})["functions"].items() if e["pct"] < 100}
    with ThreadPoolExecutor(12) as ex:
        res = {src: fns for src, fns in ex.map(one, units) if fns}
    tmp = cache.with_suffix(f".{os.getpid()}.tmp")
    tmp.write_text(json.dumps({"head": h, "v": 2, "units": res}))
    tmp.rename(cache)
    return res


LIBRARY = re.compile(r"^(JAS|JKR|JGeometry|JUtility|JMath|JGadget|J2D|J3D|JPA|JAI|JSU|JUT|JMessage|std)\w*(::|<)")


def eligible(fns, unit, tier, lanes, lo, hi, hist):
    out = []
    for n, e in fns.items():
        lane = lane_of(e)
        if lane == "write" and LIBRARY.match(e.get("demangled", "")):
            continue
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
FIXME = re.compile(r"^(\s*//\s*FIXME(?:\([^)]*\))?:\s*)\d+(?:\.\d+)?\s*%?(.*)$")
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
        if m:  # the numbers and the class go on the FIXME line, the author's note on the line below it
            note = OLD_STATS.sub("", m.group(2), count=1).lstrip(" .:;,")
            pad = re.match(r"\s*", lines[at]).group()
            for i in range(at + 1, start):  # an author's own "FIXME: 96.9. ..." below a stamp: keep its text only
                d = FIXME.match(lines[i])
                if d:
                    lines[i] = pad + "// " + OLD_STATS.sub("", d.group(2), count=1).lstrip(" .:;,")
            lines[at:at + 1] = [f"{m.group(1)}{stats}."] + ([pad + "// " + note] if note else [])
        else:
            lines.insert(at, f"// FIXME: {stats}.")
    new = "\n".join(lines)
    if new == path.read_text(errors="replace"):
        return False
    path.write_text(new)
    return True


# ---------------------------------------------------------------- task text

WRITE_HINT = ("If try.py says a function you wrote is `missing` and prints `ours, not in the target: <mangled name>`, the "
              "original still carries the placeholder name: rename it in config/G4SE01/symbols.txt to the mangled name "
              "your definition emits (change only the name before ` = `) and update every other use "
              "(`grep -rnw <old name> src include`).")


def asm_block(wt, unit, fn):
    """(the function's original assembly without the address/bytes prefix, its first line number in the .s file)."""
    asm = wt / "build/G4SE01/asm/game" / (Path(unit).stem + ".s")
    out, on, first = [], False, 0
    if not asm.exists():
        return out, first
    for i, line in enumerate(asm.read_text(errors="replace").splitlines(), 1):
        if line.startswith(f".fn {fn},") or line.startswith(f'.fn "{fn}",'):
            on, first = True, i + 1
            continue
        if on:
            if line.startswith(".endfn"):
                break
            out.append(re.sub(r"^/\* \w+ \w+  [0-9A-F ]+\*/\s*", "  ", line))
    return out, first


def declarations(wt, fn, asm):
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


def diff_hunks(wt, unit, fn, limit):
    """(lines, differing rows, all rows): objdiff's aligned rows `original | ours` around every
    difference, unchanged stretches skipped, so the text grows with the difference, not the function."""
    units = json.loads((wt / "objdiff.json").read_text())["units"]
    u = FP.T.find_unit(units, unit)
    out = subprocess.run([str(wt / FP.T.OBJDIFF), "diff", "-p", ".", "-u", u["name"], "-o", "-", "-c", FP.T.CONFIG],
                         capture_output=True, text=True, cwd=wt)
    if out.returncode:
        return [], 0, 0
    d = json.loads(out.stdout)
    s = next((x for x in d["left"]["symbols"] if x["name"] == fn), None)
    if s is None or s.get("target_symbol") is None:
        return [], 0, 0
    r = d["right"]["symbols"][s["target_symbol"]]
    rows = list(zip(s.get("instructions", []), r.get("instructions", [])))
    start = int(s.get("address", 0))
    diffs = [k for k, (a, b) in enumerate(rows) if a.get("diff_kind") or b.get("diff_kind")]
    keep = set()
    for k in diffs:
        keep.update(range(max(0, k - 2), min(len(rows), k + 3)))
    lines, last = [], None
    for k in sorted(keep):
        if len(lines) >= limit:
            lines.append(f"   ... {sum(1 for i in diffs if i >= k)} more differing rows (python3 tools/fdiff.py {unit} '{fn}' prints all)")
            break
        if last is not None and k != last + 1:
            lines.append("   ...")
        a, b = rows[k]
        fa = (a.get("instruction") or {}).get("formatted", "").strip()
        fb = (b.get("instruction") or {}).get("formatted", "").strip()
        kind = (a.get("diff_kind") or b.get("diff_kind") or "").replace("DIFF_", "").lower()
        addr = (a.get("instruction") or {}).get("address")
        off = f"{int(addr) - start:5X}" if addr is not None else "     "
        lines.append(f"{'>' if kind else ' '}{off}  {fa[:36]:36} | {fb[:36]:36} {kind}".rstrip())
        last = k
    return lines, len(diffs), len(rows)


def source_of(wt, unit, e):
    """(lines, first, last line number) of our definition with the comment block above it, or None."""
    src = wt / "src" / unit
    if not src.exists():
        return None
    lines = src.read_text(errors="replace").split("\n")
    d = find_def(lines, e["demangled"])
    if d is None:
        return None
    top = d[0]
    while top > 0 and lines[top - 1].lstrip().startswith("//"):
        top -= 1
    return lines[top:d[1] + 1], top + 1, d[1] + 1


def build_packet(wt, unit, fn, ev, note=None):
    e = ev["functions"][fn]
    lane, size, stem = lane_of(e), e["size"], Path(unit).stem
    fns = sorted(ev["functions"].items(), key=lambda kv: kv[1]["addr"])
    idx = next(i for i, (n, _) in enumerate(fns) if n == fn)
    hi = fns[idx + 1][0].split("__")[0] if idx + 1 < len(fns) else None
    lo = fns[idx - 1][0].split("__")[0] if idx > 0 else None
    place = "; ".join(x for x in [f"after `{hi}` in the file" if hi else "", f"before `{lo}`" if lo else ""] if x) or "the only function"
    text = LANE_TEXT[lane].format(unit=unit, place=place, cls=e["cls"])
    if lane == "match" and e["cls"] == "reloc-name":
        text = RELOC_TEXT
    elif lane == "match" and e["todo"]:
        text = ("Its behaviour is all but equal to the original; the few differences listed below may be real (fix "
                "those) or artifacts of codegen (an extsh. where the original has cmpwi 0, a strength-reduced "
                "counter). " + text.split("Don't change behaviour. ", 1)[-1])
    if lane == "write":
        text += " " + WRITE_HINT
    out = [f"TASK ({lane}): `{fn}` = `{e['demangled']}`, {size} bytes, unit `{unit}` (src/{unit}).",
           f"Now: {e['pct']:.2f} %, {e['cls']}; behaviour {e['grade']}.", text,
           f"Commands: `python3 tools/try.py {unit} '{fn}'`   `python3 tools/grind.py check`", ""]
    if note:
        out += [f"A reviewer rejected an earlier change to this function: {note} Don't repeat that.", ""]
    asm, first = asm_block(wt, unit, fn)
    if lane != "write":
        if e["todo"]:
            out += ["== Behaviour differences (tools/fingerprint.py) =="] + e["todo"][:30]
            if len(e["todo"]) > 30:
                out.append(f"... and {len(e['todo']) - 30} more (python3 tools/fingerprint.py {unit} '{fn}')")
            out.append("")
        t = sh(["python3", "tools/try.py", unit, fn], wt).stdout.strip().splitlines()
        hunks, nd, nr = diff_hunks(wt, unit, fn, 60 if size <= 512 else 140 if size <= 2048 else 240)
        out += [f"== Diff, original | ours: {nd} of {nr} rows differ (equal stretches skipped; offsets are hex from the function start) =="]
        out += [l for l in t[1:2] if l.strip()] + hunks + [""]
    src = source_of(wt, unit, e)
    if src:
        state = " (not compiled: inside #ifdef NONMATCHING, or it fails to compile)" if lane == "write" else ""
        if len(src[0]) <= 260:
            out += [f"== Our source{state}: src/{unit} lines {src[1]}-{src[2]} =="] + src[0] + [""]
        else:
            out += [f"Our source{state}: src/{unit} lines {src[1]}-{src[2]} ({len(src[0])} lines; read the parts the diff points at).", ""]
    where = f"build/G4SE01/asm/game/{stem}.s"
    if lane == "write" or len(asm) <= 160:
        cap = 900
        out += [f"== Original assembly ({len(asm)} lines; {where} from line {first}) =="] + asm[:cap]
        if len(asm) > cap:
            out.append(f"... {len(asm) - cap} more lines: sed -n '{first + cap},{first + len(asm)}p' {where}")
        out.append("")
    else:
        out += [f"Original assembly: {where} lines {first}-{first + len(asm)} ({len(asm)} lines; the diff above shows where ours differs).", ""]
    out += ["== Where its callees and class are declared =="] + declarations(wt, fn, asm)
    return "\n".join(out) + "\n"


def build_file_packet(wt, unit, ev, names, notes):
    fns, t, stem = ev["functions"], totals(ev), Path(unit).stem
    secs = ", ".join(f"{k} {p:.0f} %" for k, p in ev["sections"].items() if p < 100 and not k.startswith(("extab", ".text")))
    out = [f"TASK (file): unit `{unit}` (src/{unit}). {len(fns)} functions: {t['n100']} at 100 %, {t['missing']} not "
           f"written, the rest below 100 %." + (f" Data sections below 100 %: {secs}." if secs else ""),
           "Goal: faithful structural progress on the functions listed below, in this order: make each one do what the "
           "original does (behaviour `clean` in tools/fingerprint.py), then go to the next. Don't polish percentages: a "
           "function at 60-80 % with the right behaviour is a good result. The unit is judged as a whole: every "
           "improvement is kept as long as no function got worse, so run the check after each function you finish.",
           f"Commands: `python3 tools/try.py {unit}` (all functions)   `python3 tools/try.py {unit} '<function>'`   "
           f"`python3 tools/fingerprint.py {unit} '<function>'`   `python3 tools/grind.py check`", WRITE_HINT, "",
           "== Functions to work on =="]
    budget, all_asm = 320, []
    for n in names:
        e = fns[n]
        asm, first = asm_block(wt, unit, n)
        all_asm += asm
        src = source_of(wt, unit, e)
        state = "not written" if e["cls"] == "missing" else f"{e['pct']:.1f} %, {e['cls']}, behaviour {e['grade']}"
        where = f"src lines {src[1]}-{src[2]}" if src else "not in the source yet"
        out.append(f"- `{n}` = `{e['demangled']}`, {e['size']} B: {state}; {where}; asm lines {first}-{first + len(asm)}")
        if notes.get(f"{unit}\t{n}"):
            out.append(f"    a reviewer rejected an earlier change here: {notes[unit + chr(9) + n]}")
        if e["cls"] != "missing":
            out += ["    " + x for x in e["todo"][:5]]
        if len(asm) <= 40 and budget >= len(asm):
            out += ["    |" + l for l in asm]
            budget -= len(asm)
    out += ["", f"The unit's original assembly: build/G4SE01/asm/game/{stem}.s (each function between `.fn <name>` and `.endfn`).",
            "", "== Where callees and classes are declared =="]
    seen = []
    for n in names:
        for line in declarations(wt, n, all_asm if n == names[0] else []):
            if line not in seen:
                seen.append(line)
    return "\n".join(out + seen[:40]) + "\n"


def cmd_packet(args):
    sh(["python3", "tools/try.py", args.unit], REPO)
    ev = evaluate(REPO, args.unit)
    if args.function == "FILE":
        names = [n for _, n in sorted((e["size"], n) for n, e in ev["functions"].items() if e["pct"] < 100)][:12]
        sys.stdout.write(build_file_packet(REPO, args.unit, ev, names, {}))
        return
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
    if worse:  # weigh the unit as a whole: an unwritten function counts as two per instruction
        dist = lambda e: 2 * (e["size"] // 4) if e["cls"] == "missing" else e["score"]
        d0, d1 = sum(dist(e) for e in old.values()), sum(dist(e) for e in new.values())
        if not d1 < d0:
            return f"behaviour moved away from the original in: {', '.join(worse[:3])} (tools/fingerprint.py)", ev, notes
        notes.append(f"side effect: behaviour moved away in {', '.join(worse[:3])}; kept because the unit as a whole is "
                     f"closer to the original ({d0} -> {d1}). If your change caused it, fix it.")
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
        lines = r.stdout.splitlines()
        bad = [l for l in lines if l.startswith("FAILED")][:3] + [l.strip() for l in lines if "Error" in l or "undefined" in l][:3]
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
    if focus_addr is None:
        t0, t1 = totals(base), totals(ev)
        return (f"at 100 %: {t0['n100']} -> {t1['n100']}, unwritten: {t0['missing']} -> {t1['missing']}, "
                f"behaviour score: {t0['score']} -> {t1['score']}, matched bytes: {t0['credit']:.0f} -> {t1['credit']:.0f}")
    f0, f1 = by_addr(base).get(focus_addr), by_addr(ev).get(focus_addr)
    if not f0 or not f1:
        return ""
    s = lambda e: "unwritten" if e["cls"] == "missing" else f"{e['pct']:.2f} % {e['grade']}"
    return f"{f1['demangled'].split('(')[0]}: {s(f0)} -> {s(f1)}"


def cmd_check(args):
    BR.maybe_delegate("check", ["--full"] if args.full else [])
    wt = Path.cwd()
    task = json.loads((wt / "build/grind/task.json").read_text())
    why, ev, notes = judge(wt, task["unit"], task["base"], task["addr"], full=True if args.full else None, dry=True)
    line = describe(task["base"], ev, task["addr"]) if ev else ""
    if ev and task["addr"] is None:
        for a, e in sorted(by_addr(ev).items()):
            o = by_addr(task["base"]).get(a)
            if o and (abs(e["pct"] - o["pct"]) > EPS or e["score"] != o["score"]):
                print(f"  {e['name'].split('__')[0]}: {o['pct']:.1f} % {o['grade']} -> {e['pct']:.1f} % {e['grade']}")
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
    if size <= 8192:
        return 50, 3600
    return 64, 5400


def run_session(wt, tier, system, packet, turns, timeout, tag, tools="Bash,Read,Edit,Write", effort=None):
    settings = LOGS / "sandbox.json"
    settings.write_text(json.dumps(SANDBOX))
    cmd = ["claude", "-p", "--model", TIERS[tier][0], "--output-format", "stream-json", "--verbose",
           "--no-session-persistence", "--safe-mode", "--strict-mcp-config", "--disable-slash-commands",
           "--tools", tools, "--system-prompt", system, "--settings", str(settings),
           "--permission-mode", "acceptEdits", "--allowedTools", tools,
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
    result, usage_, seen, nturn = {}, {}, set(), 0
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
                    usage_[k] = usage_.get(k, 0) + v
    u = result.get("usage") or usage_
    d = LOGS / "sessions"
    d.mkdir(parents=True, exist_ok=True)
    (d / f"{tag}.packet.txt").write_text(packet)
    (d / f"{tag}.stream.jsonl").write_text(out)
    return {"turns": result.get("num_turns", nturn), "in": u.get("input_tokens", 0),
            "cache_write": u.get("cache_creation_input_tokens", 0), "cache_read": u.get("cache_read_input_tokens", 0),
            "out": u.get("output_tokens", 0), "cost": result.get("total_cost_usd") or 0.0,
            "secs": int(time.time() - t0), "timed_out": timed_out,
            "denials": len(result.get("permission_denials") or []), "text": result.get("result") or ""}


def stats_of(s):
    return [s["turns"], s["in"], s["cache_write"], s["cache_read"], s["out"], f"{s['cost']:.4f}", s["secs"]]


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
    with open(LOGS / "merge.lock", "a") as lock:
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


# ---------------------------------------------------------------- adversarial review

# The reviewer is never weaker than the author and never below sonnet. (Pilot: an opus review of two sonnet
# changes cost more than writing them; sonnet and haiku reviewers both caught a planted trick.)
REVIEW_TIER = {"haiku": "sonnet", "sonnet": "sonnet", "opus": "opus"}
REVIEW_SYSTEM = """You are the adversarial reviewer of a decompilation project: The Legend of Zelda: Four Swords Adventures (GameCube), C++ that Metrowerks GC/2.6 must compile back to the original code. The repository in your working directory is a private read-only copy with the changes applied.

The authors are other models. Scripts reward them for two numbers: objdiff's match % and a behaviour fingerprint that counts a function's calls, constants, compares, field accesses, immediates and operations against the original. The scripts already proved that every change builds, that no function got worse and that some regex style rules hold. They cannot see intent. Assume each author may have gamed the numbers. You never saw the author's reasoning: judge only the diff against the original assembly.

REJECT a change that contains any of these:
1. Metric gaming: a statement whose only effect is to add a call, constant, compare, load, store or operation so the counts line up (dead stores, results never used, duplicated statements, conditions that cannot matter), or logic the original assembly does not have.
2. Codegen steering: a temporary, cast, volatile, reordered or split statement, or an odd type, whose only purpose is register allocation, scheduling or a sign/zero extension. The test is whether a programmer would plausibly have written it. A local that names a value, or statements in the order the original evidently had, are fine.
3. Fakes: extern "C" stand-ins for member functions, a struct redefined or overlaid, byte-offset pointer access, a float or string literal behind an lbl_ name, a hand-typed table standing in for real initializers, a body copied from another function, a dangling pointer or other undefined behaviour, #ifdef tricks, any #pragma other than an evidenced auto_inline region, asm, goto.
4. Wrong facts: a field type or offset, constant, callee, branch condition, argument or class membership that contradicts the assembly; a real-looking name replacing a placeholder without evidence; a change to a shared header that the assembly does not prove.
5. Guesswork: new code that does not follow the assembly, or a comment that misdescribes what differs.

ACCEPT a change that is faithful and natural even when its function is still far from 100 %. Honest partial structure is the goal: a function at 60-80 % whose code follows the original is welcome. Do not reject for incompleteness, a low %, placeholder names (fn_, lbl_, cls_, field_0x), FIXME percentage lines (a script writes those) or style nits. Project conventions that are not tricks: a static or table without name evidence carries its lbl_<address> name, also as a function-local static; a placeholder function is renamed in config/G4SE01/symbols.txt to the mangled name the source emits; header inline accessors (getGameView_802D6A9C() and the like) where the original inlined them.

The task text has each change's diff and the original assembly or where to find it. Look further only to settle a suspicion; useful read-only commands: grep, sed -n, `python3 tools/fingerprint.py <unit> '<function>'`, `python3 tools/try.py <unit> '<function>'`. You cannot edit.

Finish with exactly one line per change and nothing after them:
VERDICT <id> ACCEPT
VERDICT <id> REJECT: <the trick or wrong fact, naming the line>"""


def review_packet(wt, unit, changes):
    stem = Path(unit).stem
    out = [f"Review {len(changes)} change(s) to unit `{unit}` (src/{unit}). Original assembly of the unit: "
           f"build/G4SE01/asm/game/{stem}.s (each function between `.fn <name>` and `.endfn`).", ""]
    for c in changes:
        out.append(f"=== Change {c['id']} ({c['lane']} lane, written by {c['tier']}): {c['desc']} ===")
        diff = sh(["git", "show", "--format=", c["sha"], "--", "src", "include", "libs", "config"], wt).stdout.splitlines()
        diff = [l for l in diff if not l.startswith(("index ", "diff --git"))]
        out += diff[:240]
        if len(diff) > 240:
            out.append(f"... {len(diff) - 240} more diff lines: git is not available to you; read the file instead")
        for fn in c["fns"][:6]:
            asm, first = asm_block(wt, unit, fn)
            if not asm:
                continue
            if len(asm) <= 180:
                out += [f"--- original assembly of `{fn}` ({len(asm)} lines) ---"] + asm
            else:
                out.append(f"--- original assembly of `{fn}`: build/G4SE01/asm/game/{stem}.s lines {first}-{first + len(asm)} ---")
        out.append("")
    return "\n".join(out) + "\n"


def review_notes():
    try:
        return json.loads((LOGS / "review_notes.json").read_text())
    except (OSError, ValueError):
        return {}


def run_review(wt, owner, unit, base_head, first, changes, args):
    """Ask a reviewer (REVIEW_TIER of the strongest author) for a verdict per kept change, rebuild
    the worker branch from the accepted ones and return those. A change without an explicit
    ACCEPT is out."""
    tier = REVIEW_TIER[max((c["tier"] for c in changes), key=ORDER.index)]
    if args.review_tier:
        tier = args.review_tier
    tag = f"{datetime.datetime.now():%m%d-%H%M%S}_review-{tier}_{Path(unit).stem}"
    s = run_session(wt, tier, REVIEW_SYSTEM, review_packet(wt, unit, changes), 6 + 4 * len(changes), 1800, tag,
                    tools="Bash,Read")
    revert(wt)  # a reviewer cannot edit; make sure nothing is left behind
    verdicts = {m.group(1): (m.group(2), m.group(3).strip()) for m in
                re.finditer(r"^VERDICT[ \t]+(c\d+)[ \t]+(ACCEPT|REJECT)\b:?[ \t]*([^\n]*)$", s["text"], re.M)}
    good = [c for c in changes if verdicts.get(c["id"], ("", ""))[0] == "ACCEPT"]
    bad = [c for c in changes if c not in good]
    notes = review_notes()
    for c in bad:
        why = verdicts.get(c["id"], ("", "no verdict from the reviewer"))[1] or "rejected in review"
        for fn in c["fns"] or [c["fn"]]:
            notes[f"{unit}\t{fn}"] = why[:400]
        log([owner, c["tier"], c["lane"], unit, c["fn"], c["size"], "-", "-", "-", "-", "TRICK", why] + [""] * 7)
    if bad:
        (LOGS / "review_notes.json").write_text(json.dumps(notes, indent=1))
        sh(["git", "reset", "-q", "--hard", base_head], wt, check=True)
        kept = []
        for c in good:
            if sh(["git", "cherry-pick", "--allow-empty", c["sha"]], wt).returncode:
                sh(["git", "cherry-pick", "--abort"], wt)
                log([owner, c["tier"], c["lane"], unit, c["fn"], c["size"], "-", "-", "-", "-", "TRICK",
                     "built on a change the reviewer rejected"] + [""] * 7)
                continue
            kept.append(dict(c, sha=head(wt)))
        good = kept
        if good:  # the surviving changes must still stand on their own
            sh(["timeout", "900", "python3", "tools/try.py", unit], wt)
            ev = evaluate(wt, unit)
            old, new = by_addr(first), by_addr(ev) if ev else {}
            if not ev or any(old[a]["pct"] >= 100 > new.get(a, {"pct": 0})["pct"] or new.get(a, {"score": MISSING * 2})["score"] > old[a]["score"]
                             for a in old):
                sh(["git", "reset", "-q", "--hard", base_head], wt, check=True)
                good = []
    log([owner, tier, "review", unit, "-", "-", "-", "-", "-", "-", "REVIEW",
         f"{len(good)} accepted, {len(changes) - len(good)} rejected"] + stats_of(s))
    return good


# ---------------------------------------------------------------- worker

def attempt(wt, owner, tier, unit, base, fn, names, args, notes, seq):
    """One session on a function (`fn`) or on the unit (`fn` None, `names` its listed functions).
    Returns (the evaluation to go on from, a change record or None)."""
    sc = lambda x: x["score"] if x["score"] < MISSING else "-"
    if fn:
        e = base["functions"][fn]
        lane, addr, size = lane_of(e), e["addr"], e["size"]
        packet = build_packet(wt, unit, fn, base, notes.get(f"{unit}\t{fn}"))
        turns, timeout = limits(size)
    else:
        lane, addr, size = "file", None, sum(base["functions"][n]["size"] for n in names)
        packet = build_file_packet(wt, unit, base, names, notes)
        turns, timeout = min(64, 16 + 5 * len(names)), 3600
    (wt / "build/grind").mkdir(parents=True, exist_ok=True)
    (wt / "build/grind/task.json").write_text(json.dumps({"unit": unit, "fn": fn, "addr": addr, "base": base}))
    (wt / "build/try" / ("__".join(unit[:-4].split("/")) + ".json")).unlink(missing_ok=True)  # try.py's attempt log
    tag = (f"{datetime.datetime.now():%m%d-%H%M%S}_{tier}_{Path(unit).stem}__"
           + re.sub(r"[^A-Za-z0-9_]", "_", fn or "FILE")[:60])
    s = run_session(wt, tier, SYSTEM, packet, turns, timeout, tag, effort=args.effort)
    why, ev, _ = judge(wt, unit, base, addr)
    t0, t1 = totals(base), (totals(ev) if ev else None)
    if fn:
        f1 = by_addr(ev).get(addr) if ev else None
        row = [owner, tier, lane, unit, fn, size, f"{e['pct']:.2f}", sc(e),
               f"{f1['pct']:.2f}" if f1 else "-", sc(f1) if f1 else "-"]
    else:
        f1 = None
        row = [owner, tier, lane, unit, "FILE", size, t0["n100"], t0["score"],
               t1["n100"] if t1 else "-", t1["score"] if t1 else "-"]
    if why:
        diff = sh(["git", "diff", "--", "src", "include", "libs", "config"], wt).stdout
        if diff.strip() and why != "no change":
            (LOGS / "rejects").mkdir(exist_ok=True)
            (LOGS / "rejects" / f"{tag}.patch").write_text(f"# {why}\n{diff}")
        revert(wt)
        log(row + ["REJECT", why] + stats_of(s))
        return base, None
    desc = describe(base, ev, addr)
    sh(["git", "add", "-A", "--", "src", "include", "libs", "config/G4SE01/symbols.txt",
        "config/G4SE01/splits.txt", "config/G4SE01/gen_game.tsv"], wt)
    sh(["git", "commit", "-qm", f"{unit[:-4]}: {desc} ({lane} lane, grind)\n\n"
        f"Co-Authored-By: {TIERS[tier][1]} <noreply@anthropic.com>"], wt, check=True)
    log(row + ["ACCEPT", desc] + stats_of(s))
    old, new = by_addr(base), by_addr(ev)
    fns = [x["name"] for a, x in new.items()
           if a not in old or abs(x["pct"] - old[a]["pct"]) > EPS or x["score"] != old[a]["score"]]
    if f1 and f1["name"] not in fns:
        fns.insert(0, f1["name"])
    return ev, {"id": f"c{seq}", "sha": head(wt), "tier": tier, "lane": lane, "fn": fn or "FILE", "fns": fns,
                "size": size, "desc": desc}


def stamp_commit(wt, unit):
    """Refresh the unit's FIXME lines from the measurement, as the script's own commit (kept out
    of the authors' commits so the reviewer reads only what a model wrote)."""
    sh(["timeout", "900", "python3", "tools/try.py", unit], wt)
    ev = evaluate(wt, unit)
    if ev is None or not stamp_fixmes(wt, unit, ev):
        return
    sh(["timeout", "900", "python3", "tools/try.py", unit], wt)
    ev2 = evaluate(wt, unit)
    if ev2 is None or totals(ev2) != totals(ev):  # a stamp must not change code
        sh(["git", "checkout", "-q", "--", f"src/{unit}"], wt)
        return
    sh(["git", "commit", "-qam", f"{unit[:-4]}: FIXME lines from the measurement (tools/grind.py)"], wt, check=True)


def cmd_worker(args):
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(1))
    owner, tier = f"g2-w{args.n}", args.tier
    wt, branch = WT / owner, f"{BRANCH}-w{args.n}"
    lanes = args.lanes.split(",")
    if not wt.exists():
        sys.exit(f"{wt} missing: run `python3 tools/grind.py setup --workers {args.n}`")
    done, skip = 0, set()
    (LOGS / "pids").mkdir(parents=True, exist_ok=True)
    (LOGS / "pids" / f"{owner}.pid").write_text(str(os.getpid()))
    more = lambda: not (LOGS / "STOP").exists() and (not args.sessions or done < args.sessions)
    while more():
        if args.usage_stop and done and done % 10 == 0:
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
            if args.mode == "file":
                el = eligible(fns, unit, tier, lanes, 0, args.file_fn_max, hist)
                fh = hist.get((unit, "FILE"), [])
                if (len(el) < args.file_min or any(t == tier and v != "ACCEPT" for t, v in fh)
                        or sum(t == tier for t, _ in fh) >= 3):
                    continue
            else:
                el = eligible(fns, unit, tier, lanes, args.min_size, args.max_size, hist)
            if el:  # units with unwritten code first (most bytes), then the ones with most open functions
                wb = sum(fns[n]["size"] for n in el if lane_of(fns[n]) == "write")
                pool.append((-wb, -len(el), unit))
        unit = next((u for *_, u in sorted(pool) if claim(u, owner, wt)), None)
        if unit is None:
            notify("FSA grind", f"{owner} ({tier}, {args.mode}): nothing left in its lanes")
            return
        skip.add(unit)
        changes = []
        try:
            reset_to_integration(wt)
            base_head = head(wt)
            sh(["timeout", "900", "python3", "tools/try.py", unit], wt)
            base = first = evaluate(wt, unit)
            notes = review_notes()
            still = lambda ev, lo, hi: eligible({n: e for n, e in ev["functions"].items() if e["pct"] < 100},
                                                unit, tier, lanes, lo, hi, hist)
            if args.mode == "file":
                for _ in range(args.relay):
                    names = still(base, 0, args.file_fn_max)[:12]
                    if not names or not more():
                        break
                    base, ch = attempt(wt, owner, tier, unit, base, None, names, args, notes, len(changes) + 1)
                    done += 1
                    if ch is None:
                        break
                    changes.append(ch)
            else:
                for fn in still(base, args.min_size, args.max_size)[:args.per_unit]:
                    if not more():
                        break
                    if fn not in base["functions"] or base["functions"][fn]["pct"] >= 100:
                        continue
                    base, ch = attempt(wt, owner, tier, unit, base, fn, [], args, notes, len(changes) + 1)
                    done += 1
                    if ch:
                        changes.append(ch)
            if changes and not args.no_review:
                changes = run_review(wt, owner, unit, base_head, first, changes, args)
            if changes:
                stamp_commit(wt, unit)
            result = integrate(wt, branch, unit) if changes else "nothing kept"
            if result == "merge conflict":
                if sh(["git", "rebase", BRANCH], wt).returncode == 0:
                    result = integrate(wt, branch, unit) + " (rebased)"
                else:
                    sh(["git", "rebase", "--abort"], wt)
            log([owner, tier, "-", unit, "-", "-", "-", "-", "-", "-", "UNIT", result] + [""] * 7)
        except Exception as ex:  # keep the worker alive
            log([owner, tier, "-", unit, "-", "-", "-", "-", "-", "-", "ERROR", str(ex)[:300]] + [""] * 7)
            revert(wt)
        finally:
            sh(["python3", "tools/claim.py", "release", unit], MAIN)


def cmd_rejudge(args):
    """Judge a saved rejects/<tag>.patch again under the current rules (after a rule change); when it
    passes, review and integrate it like a session's change. Its cost is already in the ledger."""
    owner, tier, unit = f"g2-w{args.n}", args.tier, args.unit
    wt, branch = WT / owner, f"{BRANCH}-w{args.n}"
    if not claim(unit, owner, wt):
        sys.exit(f"{unit} is claimed")
    try:
        reset_to_integration(wt)
        base_head = head(wt)
        sh(["timeout", "900", "python3", "tools/try.py", unit], wt)
        base = evaluate(wt, unit)
        fn = args.function
        e = base["functions"][fn]
        patch = "".join(Path(args.patch).read_text().splitlines(keepends=True)[1:])
        r = subprocess.run(["git", "apply", "--3way", "--whitespace=nowarn", "-"], cwd=wt, input=patch, capture_output=True, text=True)
        if r.returncode:
            revert(wt)
            sys.exit("patch no longer applies: " + r.stderr.strip()[-300:])
        sh(["git", "reset", "-q"], wt)
        why, ev, notes = judge(wt, unit, base, e["addr"])
        print("\n".join("note: " + n for n in notes))
        if why:
            revert(wt)
            sys.exit("still rejected: " + why)
        desc = describe(base, ev, e["addr"])
        sh(["git", "add", "-A", "--", "src", "include", "libs", "config/G4SE01/symbols.txt",
            "config/G4SE01/splits.txt", "config/G4SE01/gen_game.tsv"], wt)
        sh(["git", "commit", "-qm", f"{unit[:-4]}: {desc} ({lane_of(e)} lane, grind)\n\n"
            f"Co-Authored-By: {TIERS[tier][1]} <noreply@anthropic.com>"], wt, check=True)
        old, new = by_addr(base), by_addr(ev)
        fns = [x["name"] for a, x in new.items() if a not in old or abs(x["pct"] - old[a]["pct"]) > EPS or x["score"] != old[a]["score"]]
        changes = [{"id": "c1", "sha": head(wt), "tier": tier, "lane": lane_of(e), "fn": fn, "fns": fns, "size": e["size"], "desc": desc}]
        changes = run_review(wt, owner, unit, base_head, base, changes, args)
        if changes:
            stamp_commit(wt, unit)
        result = integrate(wt, branch, unit) if changes else "rejected in review"
        print(desc, "->", result)
        if result.startswith("merged"):
            with open(LOGS / "ledger.tsv", "r+") as f:  # the old verdict no longer holds the function back
                fcntl.flock(f, fcntl.LOCK_EX)
                rows = f.read().splitlines(keepends=True)
                for i, row in enumerate(rows):
                    c = row.rstrip("\n").split("\t")
                    if len(c) > 11 and c[4] == unit and c[5] == fn and c[2] == tier and c[11] == "REJECT":
                        c[11] = "REJUDGED"
                        rows[i] = "\t".join(c) + "\n"
                f.seek(0); f.truncate(); f.write("".join(rows))
            log([owner, tier, lane_of(e), unit, fn, e["size"], f"{e['pct']:.2f}", "-", "-", "-", "ACCEPT", "rejudged: " + desc] + [0, 0, 0, 0, 0, "0", 0])
    finally:
        sh(["python3", "tools/claim.py", "release", unit], MAIN)


# ---------------------------------------------------------------- publishing to matching

def progress_numbers(wt):
    """(matched functions, matched code %, weighted %, average match %) from the honest report and structural.json."""
    try:
        m = json.loads((wt / "build/G4SE01/report_honest.json").read_text())["measures"]
        w = json.loads((wt / "build/G4SE01/structural.json").read_text())["weighted"]["percent"]
    except (OSError, ValueError, KeyError):
        return None
    return (int(m["matched_functions"]), round(float(m["matched_code_percent"]), 3), round(float(w), 3),
            round(float(m["fuzzy_match_percent"]), 2))


def highlights(since, k=3):
    """The biggest kept gains in the ledger after timestamp `since` (reviewed-out ones excluded)."""
    rows = [dict(zip(COLS, r)) for r in ledger_rows() if len(r) >= len(COLS) and r[0] > since]
    out_ = {(r["unit"], r["fn"]) for r in rows if r["verdict"] == "TRICK"}
    num = lambda v: float(v) if re.match(r"^-?\d+(\.\d+)?$", v or "") else 0.0
    best = []
    for r in rows:
        if r["verdict"] == "ACCEPT" and r["fn"] != "FILE" and (r["unit"], r["fn"]) not in out_:
            gain = num(r["size"]) * (num(r["pct1"]) - num(r["pct0"])) / 100
            best.append((gain, f"{r['why'].replace('rejudged: ', '')} ({int(num(r['size']))} B, {r['tier']})"))
    kept = sum(1 for r in rows if r["verdict"] == "ACCEPT")
    return kept, len(out_), [t for _, t in sorted(best, reverse=True)[:k]]


def publish_once():
    """Reviewed grind2 work -> matching: fast-forward, full build with SHA-1, no 100 % function lost,
    no new unit out of address order, generator and gate clean, site data, commit, push; grind2 then
    follows matching. Everything on grind2 passed the adversarial review before it was integrated."""
    with open(LOGS / "merge.lock", "a") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        if sh(["git", "rev-parse", "--abbrev-ref", "HEAD"], MAIN).stdout.strip() != "matching":
            return "main checkout is not on matching"
        old, new = head(MAIN), head(INTEG)
        if sh(["git", "merge-base", "--is-ancestor", new, old], MAIN).returncode == 0:
            return "nothing new"
        if sh(["git", "status", "--porcelain", "--untracked-files=no"], MAIN).stdout.strip():
            return "main checkout has local changes"
        before = report_map(MAIN) or {}
        n0 = progress_numbers(MAIN)
        order0 = set(re.findall(r"game/\w+\.cpp", sh(["python3", "tools/order_check.py"], MAIN).stdout))
        if sh(["git", "merge", "-q", "--ff-only", BRANCH], MAIN).returncode:
            return "grind2 is not a fast-forward of matching"

        def undo(why):
            sh(["git", "reset", "-q", "--hard", old], MAIN)
            sh("python3 configure.py >/dev/null && timeout 3000 ninja >/dev/null 2>&1", MAIN)
            notify("FSA grind: publish failed", why)
            return "FAILED: " + why
        if sh("python3 configure.py >/dev/null && timeout 3000 ninja >/dev/null 2>&1", MAIN).returncode:
            return undo("build or SHA-1")
        after = report_map(MAIN) or {}
        lost = [f for (u, f), p in before.items() if p >= 100 and after.get((u, f), 0) < 100]
        if lost:
            return undo(f"{len(lost)} matched function(s) lost: {', '.join(lost[:3])}")
        order1 = set(re.findall(r"game/\w+\.cpp", sh(["python3", "tools/order_check.py"], MAIN).stdout))
        if order1 - order0:
            return undo("out of address order: " + ", ".join(sorted(order1 - order0)[:3]))
        if "check: ok" not in sh(["python3", "tools/gen_game.py", "--check"], MAIN).stdout:
            return undo("gen_game --check")
        g = sh(["python3", "tools/gate.py", "--all-units", "--base", old], MAIN).stdout
        if "gate: ok" not in g:
            return undo("gate: " + (g.strip().splitlines() or ["failed"])[-1][:200])
        last = (LOGS / "publish.tsv").read_text().splitlines()[-1].split("\t")[0] if (LOGS / "publish.tsv").exists() else ""
        kept, tricks, top = highlights(last)
        n1 = progress_numbers(MAIN)
        subject = f"site: regenerate data (grind v2: {kept} reviewed changes" + (f", {n1[0] - n0[0]:+d} functions at 100 %" if n0 and n1 else "") + ")"
        r = sh(["python3", "tools/site_data.py", "--subject", subject, "--stage"], MAIN)
        if r.returncode:
            return undo("site_data.py: " + (r.stdout + r.stderr).strip()[-200:])
        sh(["git", "commit", "-qm", subject], MAIN, check=True)
        pushed = sh(["git", "push", "-q", "origin", "matching"], MAIN, timeout=300).returncode == 0
        sh(["git", "merge", "-q", "--ff-only", "matching"], INTEG)
        with open(LOGS / "publish.tsv", "a") as f:
            f.write("\t".join(str(x) for x in [now(), old[:9], head(MAIN)[:9], *(n1 or ()), kept, tricks, int(pushed)]) + "\n")
        if n1:
            d = f" ({n1[1] - n0[1]:+.2f})" if n0 else ""
            dw = f" ({n1[2] - n0[2]:+.2f})" if n0 else ""
            notify(f"FSA: {n1[1]:.2f} % matched{d}, {n1[2]:.2f} % weighted{dw}",
                   f"{n1[0]} functions; {kept} reviewed changes merged, {tricks} sent back by review. " + "; ".join(top))
        return f"published {head(MAIN)[:9]}: {n1}; kept {kept}, reviewed out {tricks}; pushed {pushed}; " + "; ".join(top)


def cmd_publish(args):
    while True:
        try:
            res = publish_once()
        except Exception as e:
            res = f"error: {e}"
        print(now(), res, flush=True)
        if not args.every:
            return
        alive = any(Path(f"/proc/{pf.read_text().strip()}").exists() for pf in (LOGS / "pids").glob("*.pid")) if (LOGS / "pids").exists() else False
        if res == "nothing new" and not alive:
            return
        time.sleep(args.every)


# ---------------------------------------------------------------- usage watchdog

def cmd_watch(args):
    """Poll the weekly utilization. One point below --stop: STOP file (workers finish their session
    and exit). At --stop: SIGTERM to every worker (a running session is cut off; nothing of it is kept)."""
    LOGS.mkdir(parents=True, exist_ok=True)
    soft = args.stop - 0.01
    while True:
        u = usage()
        with open(LOGS / "usage.tsv", "a") as f:
            f.write(f"{now()}\t{u}\n")
        alive = []
        for pf in (LOGS / "pids").glob("*.pid") if (LOGS / "pids").exists() else []:
            try:
                os.kill(int(pf.read_text()), 0)
                alive.append(pf)
            except (OSError, ValueError):
                pf.unlink(missing_ok=True)
        if u is not None and u >= args.stop:
            (LOGS / "STOP").touch()
            for pf in alive:
                try:
                    os.kill(int(pf.read_text()), signal.SIGTERM)
                except (OSError, ValueError):
                    pass
            notify("FSA grind: hard stop", f"weekly usage {u:.0%} (limit {args.stop:.0%})")
            return
        if u is not None and u >= soft and not (LOGS / "STOP").exists():
            (LOGS / "STOP").touch()
            notify("FSA grind: stopping", f"weekly usage {u:.0%}; workers finish their session")
        if (LOGS / "STOP").exists() and not alive:
            notify("FSA grind stopped", f"weekly usage {u if u is None else format(u, '.0%')}")
            return
        time.sleep(args.every)


# ---------------------------------------------------------------- status

def cmd_status(_):
    rows = [dict(zip(COLS, r)) for r in ledger_rows() if len(r) >= len(COLS)]
    num = lambda r, k: float(r[k]) if re.match(r"^-?\d+(\.\d+)?$", r[k] or "") else 0.0
    sess = [r for r in rows if r["verdict"] in ("ACCEPT", "REJECT")]
    tricked = {(r["unit"], r["fn"], r["tier"]) for r in rows if r["verdict"] == "TRICK"}
    print(f"{'tier':7} {'lane':10} {'sessions':>8} {'kept':>5} {'to 100%':>7} {'turns':>6} {'ctx/turn':>8} {'out':>7} {'cost':>8} {'$/kept':>7}")
    order = LANES + ["file"]
    keys = sorted({(r["tier"], r["lane"]) for r in sess}, key=lambda k: (ORDER.index(k[0]), order.index(k[1])))
    for k in keys + [None]:
        rs = [r for r in sess if k is None or (r["tier"], r["lane"]) == k]
        if not rs:
            continue
        kept = [r for r in rs if r["verdict"] == "ACCEPT"]
        full = sum((num(r, "pct1") - num(r, "pct0")) if r["fn"] == "FILE" else (num(r, "pct1") >= 100) for r in kept)
        turns = sum(num(r, "turns") for r in rs)
        ctx = sum(num(r, "cache_read") + num(r, "cache_write") + num(r, "in") for r in rs) / max(1, turns)
        cost = sum(num(r, "cost") for r in rs)
        name = k or ("all", "")
        print(f"{name[0]:7} {name[1]:10} {len(rs):8} {len(kept):5} {int(full):7} {turns / len(rs):6.1f} {ctx / 1000:7.1f}k "
              f"{sum(num(r, 'out') for r in rs) / len(rs) / 1000:6.1f}k {cost:8.2f} {cost / max(1, len(kept)):7.3f}")
    rev = [r for r in rows if r["verdict"] == "REVIEW"]
    if rev:
        acc = sum(int(re.match(r"(\d+) accepted", r["why"]).group(1)) for r in rev)
        rej = sum(int(re.search(r"(\d+) rejected", r["why"]).group(1)) for r in rev)
        print(f"review: {len(rev)} sessions, {acc} changes accepted, {rej} rejected ({len(tricked)} functions sent back), "
              f"cost {sum(num(r, 'cost') for r in rev):.2f}")
        for r in rows:
            if r["verdict"] == "TRICK":
                print(f"  reviewer rejected {r['unit']} {r['fn'].split('__')[0]}: {r['why'][:150]}")
    why = {}
    for r in sess:
        if r["verdict"] == "REJECT":
            w = re.sub(r"[\d.]+", "N", r["why"])[:60]
            why[w] = why.get(w, 0) + 1
    for w, n in sorted(why.items(), key=lambda x: -x[1])[:8]:
        print(f"  judge rejected {n:3}x: {w}")
    units = {}
    for r in rows:
        if r["verdict"] in ("UNIT", "ERROR"):
            k = r["why"] if r["verdict"] == "UNIT" else "ERROR: " + r["why"][:60]
            units[k] = units.get(k, 0) + 1
    if units:
        print("units:", ", ".join(f"{k} {v}" for k, v in units.items()))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("setup"); p.add_argument("--workers", type=int, default=4); p.set_defaults(fn=cmd_setup)
    p = sub.add_parser("queue"); p.set_defaults(fn=cmd_queue)
    p = sub.add_parser("packet"); p.add_argument("unit"); p.add_argument("function"); p.set_defaults(fn=cmd_packet)
    p = sub.add_parser("check"); p.add_argument("--full", action="store_true"); p.set_defaults(fn=cmd_check)
    p = sub.add_parser("status"); p.set_defaults(fn=cmd_status)
    p = sub.add_parser("rejudge")
    p.add_argument("n", type=int); p.add_argument("unit"); p.add_argument("function"); p.add_argument("patch")
    p.add_argument("--tier", choices=ORDER, required=True, help="the tier that wrote the patch")
    p.add_argument("--review-tier", choices=ORDER)
    p.set_defaults(fn=cmd_rejudge)
    p = sub.add_parser("publish")
    p.add_argument("--every", type=int, default=0, help="repeat every N seconds until the workers are gone and nothing is new")
    p.set_defaults(fn=cmd_publish)
    p = sub.add_parser("watch")
    p.add_argument("--stop", type=float, required=True, help="weekly utilization (0..1) at which everything stops")
    p.add_argument("--every", type=int, default=150, help="seconds between checks")
    p.set_defaults(fn=cmd_watch)
    p = sub.add_parser("worker")
    p.add_argument("n", type=int)
    p.add_argument("--tier", choices=ORDER, required=True)
    p.add_argument("--mode", choices=["function", "file"], default="function")
    p.add_argument("--lanes", default="write,structure", help="write,structure (default: structure first) and/or match")
    p.add_argument("--min-size", type=int, default=0)
    p.add_argument("--max-size", type=int, default=10 ** 9)
    p.add_argument("--per-unit", type=int, default=8, help="function mode: sessions per claimed unit")
    p.add_argument("--file-min", type=int, default=3, help="file mode: a unit needs this many open functions")
    p.add_argument("--file-fn-max", type=int, default=512, help="file mode: functions up to this size are listed")
    p.add_argument("--relay", type=int, default=3, help="file mode: fresh sessions per unit while each one is kept")
    p.add_argument("--sessions", type=int, default=0, help="stop after this many sessions (0 = until the queue is empty)")
    p.add_argument("--unit", help="only this unit (game/X.cpp)")
    p.add_argument("--effort", choices=["low", "medium", "high"])
    p.add_argument("--no-review", action="store_true", help="skip the adversarial review (never for work that will be merged)")
    p.add_argument("--review-tier", choices=ORDER, help="reviewer model (default: one tier above the author)")
    p.add_argument("--usage-stop", type=float, default=0.85, help="stop all workers at this weekly utilization (0 = off)")
    p.set_defaults(fn=cmd_worker)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
