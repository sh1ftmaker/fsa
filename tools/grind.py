#!/usr/bin/env python3
"""Grind small game functions with headless Haiku workers, judged by script.

Usage:
  python3 tools/grind.py setup [--workers N]   # grind branch, integration + worker worktrees
  python3 tools/grind.py worker N [--max-size B] [--per-unit K]
  python3 tools/grind.py status                 # results summary
  touch ../fsa-wt/grind-logs/STOP               # workers finish their current unit and exit

Each worker claims one NonMatching game unit (owner grind-wN), resets its worktree to
the `grind` branch, and for each unmatched function up to --max-size bytes (smallest
first) runs `claude -p --model claude-haiku-5-5` with a short prompt. The script, not a
model, decides: the diff may only touch the unit's .cpp, its own header and pure
renames in symbols.txt; added lines may not contain banned constructs; and after a
rebuild the unit must have more 100 % functions with none worse. Accepted attempts are
committed on branch grind-wN; at the end of the unit the branch is merged into `grind` in the
integration worktree, which must then build byte-identical, pass gen_game --check and
the gate, or the merge is undone. `matching` is never touched.

Logs: ../fsa-wt/grind-logs/results.tsv (one row per attempt), haiku/<unit>__<fn>.json.
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
import time
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
# The main checkout (holds orig/, build/compilers and progress/claims.json), wherever this copy runs.
MAIN = Path(subprocess.run(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"], cwd=REPO,
                          capture_output=True, text=True).stdout.strip()).parent
WT = MAIN.parent / "fsa-wt"
INTEG = WT / "grind"
LOGS = WT / "grind-logs"
MODEL = "claude-haiku-5-5"
TRAILER = "Co-Authored-By: Claude Haiku 5.5 <noreply@anthropic.com>"
BANNED = re.compile(r"\basm\b|__asm|#\s*pragma|fakematch|force_order|\bgoto\b|NO_INLINE|dummy", re.I)
# A placeholder free function whose first parameter is a class pointer is a member function.
C_TAKES_CLASS = re.compile(r'extern\s+"C"\s+[\w\s\*]+\bfn_\w+\s*\(\s*(const\s+)?[A-Z]\w*\s*\*')
FN_LINE = re.compile(r"^[ *] (\S+)\s+(\d+) bytes\s+(missing|[\d.]+ %)")
SEC_LINE = re.compile(r"^(\.\S+|extab\S*)\s+([\d.]+) %")


def sh(cmd, cwd, check=False, timeout=None, **kw):
    r = subprocess.run(cmd, cwd=cwd, shell=isinstance(cmd, str), capture_output=True,
                       text=True, timeout=timeout, **kw)
    if check and r.returncode:
        raise RuntimeError(f"{cmd}: {r.stdout[-2000:]}{r.stderr[-2000:]}")
    return r


def log(row):
    LOGS.mkdir(parents=True, exist_ok=True)
    with open(LOGS / "results.tsv", "a") as f:
        f.write("\t".join([datetime.datetime.now().isoformat(timespec="seconds")] + [str(x) for x in row]) + "\n")


def notify(title, body=""):
    subprocess.run(["tm-notify", title, body, "--tag", "fsa-grind"], capture_output=True)


def make_worktree(path, branch, base):
    if path.exists():
        return
    sh(["git", "worktree", "add", "-q", str(path), "-B", branch, base], MAIN, check=True)
    os.symlink(MAIN / "orig", path / "orig")
    (path / "build").mkdir(exist_ok=True)
    os.symlink(MAIN / "build/compilers", path / "build/compilers")
    sh(["cp", "-a", str(MAIN / "build/tools"), str(path / "build/tools")], MAIN, check=True)
    claims = path / "progress/claims.json"
    claims.unlink(missing_ok=True)
    os.symlink(MAIN / "progress/claims.json", claims)
    sh(["git", "update-index", "--assume-unchanged", "progress/claims.json"], path)
    sh(["python3", "configure.py"], path, check=True)


def setup(args):
    if sh(["git", "rev-parse", "-q", "--verify", "grind"], MAIN).returncode:
        sh(["git", "branch", "grind", "matching"], MAIN, check=True)
    make_worktree(INTEG, "grind", "grind")
    print("building integration worktree (first build is slow)...")
    sh("timeout 3600 ninja", INTEG)
    for n in range(1, args.workers + 1):
        make_worktree(WT / f"grind-w{n}", f"grind-w{n}", "grind")
    LOGS.mkdir(parents=True, exist_ok=True)
    print("ok")


# ---------------------------------------------------------------- scoring

def report_fns(unit):
    """Target functions of a unit from the integration report (for units with no source yet)."""
    name = "/" + unit[:-4]
    for u in json.loads((INTEG / "build/G4SE01/report_honest.json").read_text())["units"]:
        if u["name"].endswith(name):
            return {f["name"]: (int(f.get("size", 0)), float(f.get("fuzzy_match_percent", 0)))
                    for f in u.get("functions", [])}
    return {}


def fdiff(wt, unit):
    """Build the unit's object (try.py re-splits if needed) and return (functions, sections)."""
    if not (wt / "src" / unit).exists():
        return report_fns(unit), {}
    if sh(["git", "ls-files", "--error-unmatch", f"src/{unit}"], wt).returncode:
        sh(["python3", "configure.py"], wt)  # new source: objdiff.json needs the unit
    sh(["timeout", "1200", "python3", "tools/try.py", unit], wt)
    out = sh(["python3", "tools/fdiff.py", unit], wt).stdout
    fns, secs = {}, {}
    for line in out.splitlines():
        m = FN_LINE.match(line)
        if m:
            p = m.group(3)
            fns[m.group(1)] = (int(m.group(2)), 0.0 if p == "missing" else float(p.split()[0]))
            continue
        m = SEC_LINE.match(line.strip())
        if m:
            secs[m.group(1)] = float(m.group(2))
    return fns, secs


def symbol_renames(wt):
    """Pure renames in symbols.txt (old -> new); None if the diff is anything else."""
    d = sh(["git", "diff", "-U0", "config/G4SE01/symbols.txt"], wt).stdout
    rem = [l[1:] for l in d.splitlines() if l.startswith("-") and not l.startswith("---")]
    add = [l[1:] for l in d.splitlines() if l.startswith("+") and not l.startswith("+++")]
    if len(rem) != len(add):
        return None
    ren = {}
    for a, b in zip(rem, add):
        na, ra = a.split(" = ", 1)
        nb, rb = b.split(" = ", 1)
        if ra != rb:
            return None
        ren[na] = nb
    return ren


def check_diff(wt, unit):
    allowed = {f"src/{unit}", "config/G4SE01/symbols.txt", "config/G4SE01/gen_game.tsv"}
    st = sh(["git", "status", "--porcelain", "-uall"], wt).stdout.splitlines()
    paths = {l[3:] for l in st if l[3:] not in ("progress/claims.json", "objdiff.json")}
    ren = symbol_renames(wt)
    if ren is None:
        return "symbols.txt change is not a pure rename", None
    names = set(ren) | set(ren.values())
    d = sh(["git", "diff", "-U0", "--", "config/G4SE01/gen_game.tsv"], wt).stdout.splitlines()
    if any(l.startswith("+") and not l.startswith("+++") for l in d) or any(
            l.startswith("-") and not l.startswith("---") and l[1:].split("\t")[0] != Path(unit).stem for l in d):
        return "gen_game.tsv may only lose this unit's rows", None
    # Game headers are shared: allowed, and the merge step rejects any loss elsewhere.
    # Other source files may change only at the uses of a renamed symbol.
    for p in sorted(paths - allowed):
        if p.startswith("include/game/") and p.endswith(".h"):
            continue
        if not (p.startswith("src/") and p in {l[3:] for l in st if l.startswith(" M")}):
            return f"touched {p}", None
        d = sh(["git", "diff", "-U0", "--", p], wt).stdout.splitlines()
        body = [l[1:] for l in d if l[:1] in "+-" and not l.startswith(("+++", "---"))]
        if not names or any(not any(re.search(rf"\b{re.escape(n)}\b", l) for n in names) for l in body):
            return f"touched {p} beyond renamed-symbol uses", None
    added = [l for l in sh(["git", "diff", "-U0"], wt).stdout.splitlines() if l.startswith("+")]
    for f in sh(["git", "ls-files", "--others", "--exclude-standard"], wt).stdout.splitlines():
        added += (wt / f).read_text(errors="replace").splitlines()  # new files: every line is added
    for l in added:
        if C_TAKES_CLASS.search(l):
            return f"extern C taking a class pointer (write a member): {l.strip()[:80]}", None
        if BANNED.search(l):
            return f"banned: {l.strip()[:80]}", None
    # A leftover use of a renamed symbol can't be told from text (members keep the short C++
    # name); it breaks the link or another unit's relocations, which the merge checks reject.
    return None, ren


def revert(wt):
    sh(["git", "checkout", "--", "."], wt)
    sh(["git", "clean", "-fdq", "src", "include"], wt)


# ---------------------------------------------------------------- haiku

PROMPT = """You are decompiling ONE function of The Legend of Zelda: Four Swords Adventures (GameCube)
into C++ that compiles to the exact original bytes (Metrowerks GC/2.6, -O4,p, C++ with exceptions).
Your working directory is the repository; stay in it.

Target: `{fn}` ({size} bytes, now {pct}) in unit `{unit}`, source file `src/{unit}`
(create it if missing, following other src/game/*.cpp files). Original assembly:
`build/G4SE01/asm/game/{base}.s` (use `grep -n '{fn}' build/G4SE01/asm/game/{base}.s` and read
from there). Read docs/RULES_CARD.md once, especially "Known game-code shapes".

Loop: edit the source, run `python3 tools/try.py {unit} {fn}`, read its classification and the
differing rows, change one thing, repeat. Stop when it prints 100 %, when it prints STOP,
or after 8 attempts. Matched functions in the same file and include/game/ headers show the style.

What this function touches (from the original assembly; read these definitions, they decide
inlining, const-ness, types and literal order):
{context}

Rules (a script checks them and throws away your work if broken):
- Edit only `src/{unit}`, headers under include/game/ (add a member or split a field only with
  evidence from the asm; other files use them too), and, for a placeholder name only, rename
  lines in config/G4SE01/symbols.txt (change just the name before ` = `; nothing else). If you
  rename, update every use of the old name in src/ and include/ (`grep -rw <name> src include`);
  in other src files change nothing but those uses.
- A placeholder function that takes a class pointer as its first argument is a member function
  of that class: declare it in the class and write it as a member, not as extern "C".
- No asm, no #pragma, no goto, no dummy/fake functions, no tricks: write natural C++ with real
  types, as the original programmers would. Other functions in the file must not get worse.
- Do not run git, ninja or any build other than tools/try.py.
End with one line: the final match % of {fn}.
"""


def asm_block(wt, unit, fn):
    asm = (wt / "build/G4SE01/asm/game" / (Path(unit).stem + ".s"))
    if not asm.exists():
        return []
    out, on = [], False
    for line in asm.read_text(errors="replace").splitlines():
        if line.startswith(f".fn {fn},"):
            on = True
        if on:
            out.append(line)
            if line.startswith(f".endfn {fn}"):
                break
    return out


def class_of(name):
    """Class name from a CodeWarrior-mangled member (fn_X__15Player_804A6F38Fv -> Player_804A6F38)."""
    m = re.match(r".+?__(\d+)([A-Za-z_]\w*)", name)
    return m.group(2)[:int(m.group(1))] if m else None


def context(wt, unit, fn):
    """What the function touches and where those things live in our source (cheap, scripted)."""
    lines = asm_block(wt, unit, fn)
    calls = sorted({m.group(1) for l in lines for m in [re.search(r"\bbl\s+([^\s,]+)", l)] if m})
    data = sorted({m.group(1) for l in lines for m in re.finditer(r"([\w$<>.]+)@(?:ha|l|h|sda21|sda2)\b", l)
                   if not m.group(1).startswith(".L")})
    out = []
    cls = class_of(fn)
    if cls:
        hdr = sh(["git", "grep", "-l", "-E", rf"(class|struct) {re.escape(cls)}\b", "--", "include"], wt).stdout.split()
        out.append(f"- `{fn}` is a member of `{cls}`; class declared in: {', '.join(hdr[:2]) or 'not found yet'}")
    for sym in calls + data:
        short = sym.split("__")[0] if not sym.startswith("__") else sym
        where = sh(["git", "grep", "-l", "-w", short, "--", "src", "include"], wt).stdout.split()
        where = [w for w in where if w != f"src/{unit}"][:3]
        st = sh(["grep", "-m1", "-F", f"{sym} = ", "config/G4SE01/symbols.txt"], wt).stdout.strip()
        kind = "called" if sym in calls else "data"
        out.append(f"- {kind} `{sym}`: {('in ' + ', '.join(where)) if where else 'not in our source yet'}"
                   + (f"; symbols.txt: `{st.split(' = ', 1)[1][:90]}`" if st else ""))
    return "\n".join(out[:25])


DATA_PROMPT = """The function `{fn}` in unit `{unit}` (source `src/{unit}`) is at {pct:.2f} %. Its instructions
are probably right; what usually remains is data or symbol naming: float/double literals in the
.sdata2/.rodata order the original uses, a load of a symbol whose name differs ("reloc-name": target
vs ours), a field's type, a const qualifier on a called method, a static that belongs in this file's
data, or a missing extern. try.py's analysis:

```
{tryout}
```
Fix that side, run `python3 tools/try.py {unit} {fn}` after each change, at most {attempts} attempts.
Same rules as before: edit only src/{unit}, include/game/ headers, and pure renames in
config/G4SE01/symbols.txt; no asm, #pragma, goto, dummies or tricks; other functions must not get
worse; don't run git or ninja. End with one line: the final match % of {fn}.
"""


def run_haiku(wt, unit, fn, size, pct, model=MODEL, meta=None):
    # Bigger functions get more attempts and time (try.py's own budget also scales with size).
    attempts = 8 if size <= 128 else 12 if size <= 512 else 16 if size <= 2048 else 24
    prompt = PROMPT.format(fn=fn, size=size, pct=f"{pct:.2f} %" if pct else "missing",
                           unit=unit, base=Path(unit).stem, attempts=attempts,
                           context=context(wt, unit, fn) or "- (nothing found)")
    timeout = 1200 if size <= 512 else 2400 if size <= 2048 else 4800
    if os.environ.get("GRIND_TIMEOUT"):  # A/B cap
        timeout = int(os.environ["GRIND_TIMEOUT"])
    cost = _claude(wt, prompt, f"{Path(unit).stem}__{fn[:80]}", timeout, model, meta)
    # Data-fix pass: a near-miss usually differs in data or naming, not instructions.
    near = fdiff(wt, unit)[0].get(fn, (0, 0))[1] if (wt / "src" / unit).exists() else 0
    if 90 <= near < 100:
        tryout = sh(["python3", "tools/try.py", unit, fn], wt).stdout[-2500:]
        dp = DATA_PROMPT.format(fn=fn, unit=unit, pct=near, tryout=tryout, attempts=6)
        m2 = {}
        cost += _claude(wt, dp, f"{Path(unit).stem}__{fn[:70]}__data", min(timeout, 900), model, m2)
        if meta is not None:
            meta["turns"] = (meta.get("turns") or 0) + (m2.get("turns") or 0)
            meta["datafix"] = 1
    return cost


def _claude(wt, prompt, tag, timeout, model=MODEL, meta=None):
    tools = ",".join(["Read", "Edit", "Write", "Grep", "Glob",
                      "Bash(python3 tools/try.py:*)", "Bash(python3 tools/fdiff.py:*)",
                      "Bash(grep:*)", "Bash(sed -n:*)", "Bash(head:*)", "Bash(tail:*)"])
    try:
        r = sh(["claude", "-p", prompt, "--model", model, "--output-format", "json",
                "--permission-mode", "acceptEdits", "--allowedTools", tools,
                "--disallowedTools", "Agent,WebFetch,WebSearch"], wt, timeout=timeout)
        out = r.stdout
    except subprocess.TimeoutExpired:
        out = '{"result": "timeout"}'
    d = LOGS / ("haiku" if model == MODEL else model)
    d.mkdir(parents=True, exist_ok=True)
    (d / f"{tag}.json").write_text(out)
    try:
        j = json.loads(out)
    except ValueError:
        j = {}
    if meta is not None:
        meta.update(turns=j.get("num_turns", ""), api_ms=j.get("duration_api_ms", ""))
    return j.get("total_cost_usd", 0) or 0


# ---------------------------------------------------------------- worker

def candidates(max_size):
    for _ in range(30):  # the integration build may be rewriting the report
        try:
            report = json.loads((INTEG / "build/G4SE01/report_honest.json").read_text())["units"]
            break
        except (ValueError, OSError):
            time.sleep(20)
    else:
        return []
    cfg = (INTEG / "configure.py").read_text()
    bad = rejected()
    out = []
    for u in report:
        if "/game/" not in u["name"]:
            continue
        unit = "game/" + u["name"].split("/game/")[1] + ".cpp"
        if not (INTEG / "src" / unit).exists():  # no source yet: try.py/fdiff cannot score it
            continue
        if f'Object(NonMatching, "{unit}"' not in cfg:
            continue
        # Only unwritten functions: partial ones are mostly near-misses an Opus worker gave up on.
        fns = [f for f in u.get("functions", []) if f.get("fuzzy_match_percent", 0) == 0
               and int(f.get("size", 0)) <= max_size and (unit, f["name"]) not in bad]
        if fns:
            out.append((unit, len(fns)))
    out.sort(key=lambda x: -x[1])
    return [u for u, _ in out]


def rejected():
    """(unit, function) pairs some attempt already failed; escalated workers don't retry them."""
    p = LOGS / "results.tsv"
    rows = [l.split("\t") for l in p.read_text().splitlines()] if p.exists() else []
    return {(r[2], r[3]) for r in rows if len(r) > 6 and r[6] == "REJECT"}


def done_units():
    p = LOGS / "units_done.txt"
    return set(p.read_text().split()) if p.exists() else set()


def claim(unit, owner, wt):
    lock = open(LOGS / "claim.lock", "w")
    fcntl.flock(lock, fcntl.LOCK_EX)
    claims = json.loads((MAIN / "progress/claims.json").read_text() or "{}")
    if unit in claims or unit in done_units():
        return False
    sh(["python3", "tools/claim.py", "take", unit, owner, owner, str(wt)], MAIN, check=True)
    with open(LOGS / "units_done.txt", "a") as f:  # never picked twice, whatever happens
        f.write(unit + "\n")
    return True


def progress_count():
    m = re.search(r"total: (\d+) /", sh(["python3", "tools/progress.py"], INTEG).stdout)
    return int(m.group(1)) if m else None


def merge_into_grind(wt, branch, unit):
    lock = open(LOGS / "merge.lock", "w")
    fcntl.flock(lock, fcntl.LOCK_EX)
    # Finish or undo a merge before honouring a stop; never leave grind half-checked.
    signal.signal(signal.SIGTERM, signal.SIG_IGN)
    try:
        return _merge(wt, branch, unit)
    finally:
        signal.signal(signal.SIGTERM, lambda *_: sys.exit(1))


def _merge(wt, branch, unit):
    pre = sh(["git", "rev-parse", "HEAD"], INTEG, check=True).stdout.strip()
    before = progress_count()
    if sh(["git", "merge", "-q", "--no-edit", branch], INTEG).returncode:
        sh(["git", "merge", "--abort"], INTEG)
        return "merge conflict"
    ok = sh("python3 configure.py >/dev/null && timeout 3600 ninja >/dev/null 2>&1 && sha1sum -c config/G4SE01/build.sha1", INTEG).returncode == 0
    why = "" if ok else "SHA-1"
    after = progress_count() if ok else None
    if ok and (after is None or before is None or after < before):
        ok, why = False, f"total functions {before} -> {after}"
    if ok and "check: ok" not in sh(["python3", "tools/gen_game.py", "--check"], INTEG).stdout:
        ok, why = False, "gen_game --check"
    if ok:
        g = sh(["python3", "tools/gate.py", "--all-units", "--base", pre], INTEG).stdout
        ok, why = ("gate: ok" in g), (g.strip().splitlines() or ["gate"])[-1]
    if not ok:
        sh(["git", "reset", "-q", "--hard", pre], INTEG)
        notify("FSA grind: merge rejected", f"{unit}: {why}")
        return f"rejected: {why}"
    return "merged"


def release_generated(wt, unit):
    """A hand-edited generated unit leaves the generator (the project's convention): drop its
    gen_game.tsv rows so gen_game --check stops requiring the generator's exact output."""
    if not sh(["git", "status", "--porcelain", "--", f"src/{unit}"], wt).stdout.strip():
        return
    tsv = wt / "config/G4SE01/gen_game.tsv"
    lines = tsv.read_text().splitlines(keepends=True)
    keep = [l for l in lines if l.split("\t")[0] != Path(unit).stem]
    if len(keep) != len(lines):
        tsv.write_text("".join(keep))


def judge(wt, unit, fns, secs, focus=None):
    """Script verdict on the worktree's changes: (why-rejected or None, fns, secs, gained, renames)."""
    release_generated(wt, unit)
    why, ren = check_diff(wt, unit)
    # Generated functions must stay byte-equal to their templates.
    if not why and "check: ok" not in sh(["python3", "tools/gen_game.py", "--check"], wt).stdout:
        why = "breaks gen_game --check (edited a generated function or header)"
    if why:
        return why, fns, secs, 0, {}
    ren = ren or {}
    new_fns, new_secs = fdiff(wt, unit)
    renamed = {ren.get(k, k): v for k, v in fns.items()}
    worse = [k for k, (_, p) in renamed.items() if new_fns.get(k, (0, 0))[1] < p - 0.001]
    # extab/extabindex shift whenever a function is added; they can't score until the file is whole.
    worse += [k for k, p in secs.items() if new_secs.get(k, 0) < p - 0.001 and not k.startswith("extab")]
    gained = sum(p == 100 for _, p in new_fns.values()) - sum(p == 100 for _, p in fns.values())
    if worse:
        why = f"worse: {worse[:3]}"
    elif gained <= 0:
        best = new_fns.get(ren.get(focus, focus), (0, 0))[1] if focus else 0
        why = f"no new 100 % (best {best:.2f})"
    return why, new_fns, new_secs, gained, ren


def file_candidates(max_bytes):
    """Whole NonMatching game units (with or without source) whose unmatched code is small."""
    for _ in range(30):
        try:
            report = json.loads((INTEG / "build/G4SE01/report_honest.json").read_text())["units"]
            break
        except (ValueError, OSError):
            time.sleep(20)
    else:
        return []
    cfg = (INTEG / "configure.py").read_text()
    bad = rejected()
    out = []
    for u in report:
        if "/game/" not in u["name"]:
            continue
        unit = "game/" + u["name"].split("/game/")[1] + ".cpp"
        if f'Object(NonMatching, "{unit}"' not in cfg:
            continue
        left = sum(int(f.get("size", 0)) for f in u.get("functions", []) if f.get("fuzzy_match_percent", 0) == 0)
        if 0 < left <= max_bytes and (unit, "FILE") not in bad:
            out.append((left, unit))
    return [u for _, u in sorted(out)]


FILE_PROMPT = """You are decompiling a small file of The Legend of Zelda: Four Swords Adventures (GameCube)
into C++ that compiles to the exact original bytes (Metrowerks GC/2.6, -O4,p, C++ with exceptions).
Your working directory is the repository; stay in it.

Unit `{unit}`, source `src/{unit}` ({state}). Original assembly of the whole file:
`build/G4SE01/asm/game/{base}.s`. Functions not written yet (name, bytes, current %); leave the
partly matched ones alone:
{todo}
Read docs/RULES_CARD.md once, especially "Known game-code shapes"; look at matched src/game/*.cpp
files and include/game/ headers for the style and the classes this file uses.

Work smallest function first. Loop: edit, run `python3 tools/try.py {unit}` (overview) or
`python3 tools/try.py {unit} <function>` (one function's differing rows), change one thing, repeat.
When a function hits STOP or 8 attempts without progress, move on. Get as many functions to
100 % as you can; data sections (.data/.rodata/.sdata2) count too. About {attempts} attempts in total.

Rules (a script checks them and throws away ALL your work if one is broken):
- Edit only `src/{unit}`, headers under include/game/ (add members or split fields only with
  evidence from the asm; other files use them too), and, for a placeholder name only, rename
  lines in config/G4SE01/symbols.txt (change just the name before ` = `; nothing else). If you
  rename, update every use of the old name in src/ and include/ (`grep -rw <name> src include`);
  in other src files change nothing but those uses.
- A placeholder function that takes a class pointer as its first argument is a member function
  of that class: declare it in the class and write it as a member, not as extern "C".
- No asm, no #pragma, no goto, no dummy/fake functions, no tricks: write natural C++ with real
  types, as the original programmers would. Functions already at 100 % must stay at 100 %.
- Do not run git, ninja or any build other than tools/try.py.
End with one line: how many functions are at 100 %.
"""


def run_haiku_file(wt, unit, fns):
    todo = "\n".join(f"  {f}  {sz}  {p:.2f} %" for f, (sz, p) in sorted(fns.items(), key=lambda x: x[1][0]) if p == 0)
    state = "exists" if (wt / "src" / unit).exists() else "does not exist yet: create it, following other src/game/*.cpp files"
    left = sum(sz for sz, p in fns.values() if p == 0)
    attempts = 30 if left <= 3072 else 50
    prompt = FILE_PROMPT.format(unit=unit, base=Path(unit).stem, todo=todo, state=state, attempts=attempts)
    return _claude(wt, prompt, f"{Path(unit).stem}__FILE", 2700 if left <= 3072 else 5400)


def worker(args):
    # sbg stop sends SIGTERM: unwind so the claim is released and the worktree reverted.
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(1))
    n, owner = args.n, f"grind-w{args.n}"
    wt, branch = WT / f"grind-w{n}", f"grind-w{n}"
    # A worker killed mid-git leaves its worktree's index.lock behind; nothing else uses this worktree.
    gitdir = sh(["git", "rev-parse", "--git-dir"], wt).stdout.strip()
    (Path(wt) / gitdir / "index.lock" if not os.path.isabs(gitdir) else Path(gitdir) / "index.lock").unlink(missing_ok=True)
    # gen_game --check needs the template units' objects; one full build provides them.
    sh(["git", "reset", "-q", "--hard", "grind"], wt, check=True)
    sh("timeout 3600 ninja build/G4SE01/report_honest.json", wt)
    while not (LOGS / "STOP").exists():
        # Workers the watchdog restarts aren't tied to the session: guard the weekly quota here too.
        u = usage()
        if u is not None and u >= args.soft_usage:
            (LOGS / "STOP").touch()
            notify("FSA grind stopping", f"{owner}: weekly usage {u:.0%}")
            return
        pool = file_candidates(args.file_max) if args.mode == "file" else candidates(args.max_size)
        unit = next((u for u in pool if claim(u, owner, wt)), None)
        if unit is None:
            notify("FSA grind", f"{owner}: no units left")
            return
        sh(["git", "reset", "-q", "--hard", "grind"], wt, check=True)
        revert(wt)
        try:
            fns, secs = fdiff(wt, unit)
            accepted = 0
            if args.mode == "file":
                t0 = time.time()
                left = sum(sz for sz, p in fns.values() if p == 0)
                cost = run_haiku_file(wt, unit, fns)
                why, new_fns, new_secs, gained, ren = judge(wt, unit, fns, secs)
                if why:
                    revert(wt)
                    log([owner, unit, "FILE", left, "-", "REJECT", why, f"{cost:.3f}", int(time.time() - t0)])
                else:
                    sh(["git", "add", "-A", "src", "include", "config/G4SE01/symbols.txt", "config/G4SE01/gen_game.tsv"], wt)
                    sh(["git", "commit", "-qm", f"grind: {unit} +{gained} functions (whole file)\n\n{TRAILER}"], wt)
                    accepted = gained
                    log([owner, unit, "FILE", left, "-", "ACCEPT", f"+{gained}", f"{cost:.3f}", int(time.time() - t0)])
            bad = rejected()
            todo = [] if args.mode == "file" else sorted((sz, f, p) for f, (sz, p) in fns.items()
                                                         if p == 0 and sz <= args.max_size and (unit, f) not in bad)
            for size, fn, pct in todo[:args.per_unit]:
                if (LOGS / "STOP").exists():
                    break
                t0 = time.time()
                cost = run_haiku(wt, unit, fn, size, pct)
                why, new_fns, new_secs, gained, ren = judge(wt, unit, fns, secs, fn)
                if why:
                    revert(wt)
                    log([owner, unit, fn, size, f"{pct:.2f}", "REJECT", why, f"{cost:.3f}", int(time.time() - t0)])
                    continue
                sh(["git", "add", "-A", "src", "include", "config/G4SE01/symbols.txt", "config/G4SE01/gen_game.tsv"], wt)
                sh(["git", "commit", "-qm", f"grind: {unit} {ren.get(fn, fn)} matches\n\n{TRAILER}"], wt)
                fns, secs, accepted = new_fns, new_secs, accepted + 1
                log([owner, unit, fn, size, f"{pct:.2f}", "ACCEPT", f"+{gained}", f"{cost:.3f}", int(time.time() - t0)])
            result = merge_into_grind(wt, branch, unit) if accepted else "nothing accepted"
            log([owner, unit, "-", "-", "-", "UNIT", result, accepted, ""])
        except Exception as e:  # keep the worker alive; the unit stays in units_done
            log([owner, unit, "-", "-", "-", "ERROR", str(e)[:200].replace("\n", " "), "", ""])
            revert(wt)
        finally:
            revert(wt) if sys.exc_info()[0] is SystemExit else None
            sh(["python3", "tools/claim.py", "release", unit], MAIN)


def usage():
    """7-day plan utilization (0..1) from a minimal headless call's rate_limit_event, or None."""
    try:
        out = subprocess.run(["claude", "-p", "Reply: ok", "--model", MODEL, "--output-format", "stream-json",
                              "--verbose"], cwd="/tmp", capture_output=True, text=True, timeout=180).stdout
    except subprocess.TimeoutExpired:
        return None
    for line in out.splitlines():
        try:
            ev = json.loads(line)
        except ValueError:
            continue
        info = ev.get("rate_limit_info") or {}
        win = (info.get("unifiedWindows") or {}).get("seven_day") or {}
        if "utilization" in win:
            return float(win["utilization"])
    return None


LADDER = {"function": [2048, 4096, 8192, 10**9], "file": [8192, 16384, 32768, 10**9]}


def escalate(running):
    """Restart a worker that ran out of work with the next size limit (limits.json holds each one's)."""
    path = LOGS / "limits.json"
    limits = json.loads(path.read_text()) if path.exists() else {}
    for n, (mode, limit) in sorted(limits.items()):
        if f"grind-w{n} " in running or f"grind-w{n}." in running:
            continue
        nxt = [x for x in LADDER[mode] if x > limit]
        if not nxt:
            continue
        limits[n] = [mode, nxt[0]]
        path.write_text(json.dumps(limits, indent=1))
        # Units done at a smaller limit may now have eligible functions; rejected ones stay skipped.
        claims = json.loads((MAIN / "progress/claims.json").read_text() or "{}")
        (LOGS / "units_done.txt").write_text("".join(u + "\n" for u in claims))
        flag = ["--max-size", str(nxt[0]), "--per-unit", "10"] if mode == "function" else ["--mode", "file", "--file-max", str(nxt[0])]
        subprocess.run(["sbg", "start", f"grind-w{n}", "--", "python3", "tools/grind.py", "worker", n] + flag,
                       cwd=INTEG, capture_output=True)
        notify("FSA grind", f"w{n} out of work: {mode} limit raised to {nxt[0]}")


def watch(args):
    """Stop the workers gracefully at --soft utilization, immediately at --hard."""
    while True:
        u = usage()
        with open(LOGS / "usage.tsv", "a") as f:
            f.write(f"{datetime.datetime.now().isoformat(timespec='seconds')}\t{u}\n")
        if u is not None and u >= args.hard:
            subprocess.run("for s in $(sbg ls | grep -o 'grind-w[0-9]*' | sort -u); do sbg stop $s; done",
                           shell=True, capture_output=True)
            notify("FSA grind stopped", f"weekly usage {u:.0%}: workers stopped")
            return
        if u is not None and u >= args.soft and not (LOGS / "STOP").exists():
            (LOGS / "STOP").touch()
            notify("FSA grind stopping", f"weekly usage {u:.0%}: workers finish their current unit")
        running = subprocess.run(["sbg", "ls"], capture_output=True, text=True).stdout
        if (LOGS / "STOP").exists() and "grind-w" not in running:
            notify("FSA grind stopped", f"all workers finished (usage {u})")
            return
        if not (LOGS / "STOP").exists():
            escalate(running)
        time.sleep(args.every)


# ---------------------------------------------------------------- A/B: models on the same functions

BUCKETS = [("tiny", 0, 32), ("small", 33, 128), ("medium", 129, 512), ("large", 513, 2048),
           ("xlarge", 2049, 8192), ("huge", 8193, 10**9)]
AB = LOGS / "ab"


def ab_sample(per_bucket, seed):
    """Unwritten functions per size bucket (one per unit), the same set for every model."""
    import random
    report = json.loads((INTEG / "build/G4SE01/report_honest.json").read_text())["units"]
    cfg = (INTEG / "configure.py").read_text()
    bad = rejected()
    pool = {b[0]: [] for b in BUCKETS}
    for u in report:
        if "/game/" not in u["name"]:
            continue
        unit = "game/" + u["name"].split("/game/")[1] + ".cpp"
        if not (INTEG / "src" / unit).exists() or f'Object(NonMatching, "{unit}"' not in cfg:
            continue
        for f in u.get("functions", []):
            size = int(f.get("size", 0))
            if f.get("fuzzy_match_percent", 0) == 0 and (unit, f["name"]) not in bad:
                b = next(b[0] for b in BUCKETS if b[1] <= size <= b[2])
                pool[b].append((unit, f["name"], size))
    rng = random.Random(seed)
    sample = []
    for b, items in pool.items():
        rng.shuffle(items)
        seen = set()
        for unit, fn, size in items:
            if unit in seen or any(unit == x["unit"] for x in sample):
                continue
            seen.add(unit)
            sample.append({"bucket": b, "unit": unit, "fn": fn, "size": size})
            if len(seen) == per_bucket:
                break
    return sample


def ab(args):
    """Every sampled function, once per model, in fresh copies of grind; nothing is merged.
    Successful diffs are kept as patches under grind-logs/ab[-tag]/<model>/."""
    import threading, queue
    global AB
    AB = LOGS / (f"ab-{args.tag}" if args.tag else "ab")
    AB.mkdir(parents=True, exist_ok=True)
    sp = AB / "sample.json"
    if not sp.exists() and (LOGS / "ab/sample.json").exists():
        sp.write_text((LOGS / "ab/sample.json").read_text())  # same functions as the first test
    if not sp.exists():
        sp.write_text(json.dumps(ab_sample(args.per_bucket, args.seed), indent=1))
    sample = json.loads(sp.read_text())
    models = args.models.split(",")
    tsv = AB / "results.tsv"
    done = {tuple(l.split("\t")[1:4]) for l in tsv.read_text().splitlines()} if tsv.exists() else set()
    jobs = queue.Queue()
    for s_ in sample:  # interleave models so a stop leaves comparable pairs
        for m in models:
            if (m, s_["unit"], s_["fn"]) not in done:
                jobs.put((m, s_))
    stop = threading.Event()
    lock = threading.Lock()

    def run(n):
        wt = WT / f"grind-w{n}"
        sh(["git", "reset", "-q", "--hard", "grind"], wt, check=True)
        sh("timeout 3600 ninja build/G4SE01/report_honest.json", wt)
        while not stop.is_set():
            try:
                model, s_ = jobs.get_nowait()
            except queue.Empty:
                return
            u = usage()
            if u is not None and u >= args.stop_usage:
                stop.set()
                notify("FSA A/B stopped", f"weekly usage {u:.0%}")
                return
            unit, fn, size = s_["unit"], s_["fn"], s_["size"]
            sh(["git", "reset", "-q", "--hard", "grind"], wt, check=True)
            revert(wt)
            t0, meta = time.time(), {}
            try:
                fns, secs = fdiff(wt, unit)
                pct = fns.get(fn, (size, 0.0))[1]
                cost = run_haiku(wt, unit, fn, size, pct, model, meta)
                why, new_fns, _, gained, ren = judge(wt, unit, fns, secs, fn)
                best = new_fns.get(ren.get(fn, fn) if ren else fn, (0, 0))[1]
                ok = why is None and best == 100
                if ok:
                    pd = AB / model
                    pd.mkdir(exist_ok=True)
                    sh(["git", "add", "-A", "src", "include", "config/G4SE01"], wt)
                    (pd / f"{Path(unit).stem}__{fn[:80]}.patch").write_text(
                        sh(["git", "diff", "--cached"], wt).stdout)
            except Exception as e:
                cost, ok, best, why = 0, False, 0, f"error {str(e)[:120]}"
            revert(wt)
            sh(["git", "reset", "-q", "--hard", "grind"], wt)
            with lock, open(tsv, "a") as f:
                f.write("\t".join(str(x) for x in [
                    datetime.datetime.now().isoformat(timespec="seconds"), model, unit, fn, s_["bucket"], size,
                    int(ok), f"{best:.2f}", f"{cost:.4f}", meta.get("turns", ""), int(time.time() - t0),
                    (why or "")[:100]]) + "\n")

    threads = [threading.Thread(target=run, args=(n,)) for n in args.workers.split(",")]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    notify("FSA A/B finished", "python3 tools/grind.py ab-report")
    ab_report(args)


def ab_report(args):
    global AB
    AB = LOGS / (f"ab-{args.tag}" if getattr(args, "tag", "") else "ab")
    rows = [l.split("\t") for l in (AB / "results.tsv").read_text().splitlines()]
    print(f"{'bucket':8} {'model':18} {'runs':>4} {'solved':>6} {'cost':>8} {'$/solve':>8} {'turns':>6} {'min':>5}")
    for b, *_ in BUCKETS:
        for m in sorted({r[1] for r in rows}):
            rs = [r for r in rows if r[4] == b and r[1] == m]
            if not rs:
                continue
            solved = sum(int(r[6]) for r in rs)
            cost = sum(float(r[8]) for r in rs)
            turns = [int(r[9]) for r in rs if r[9].isdigit()]
            print(f"{b:8} {m:18} {len(rs):>4} {solved:>6} {cost:>8.2f} "
                  f"{(cost / solved if solved else float('inf')):>8.2f} "
                  f"{(sum(turns) / len(turns) if turns else 0):>6.1f} {sum(int(r[10]) for r in rs) / len(rs) / 60:>5.1f}")


def status(_):
    rows = [l.split("\t") for l in (LOGS / "results.tsv").read_text().splitlines()] if (LOGS / "results.tsv").exists() else []
    acc = [r for r in rows if r[6] == "ACCEPT"]
    rej = [r for r in rows if r[6] == "REJECT"]
    units = [r for r in rows if r[6] == "UNIT"]
    cost = sum(float(r[8] or 0) for r in acc + rej)
    print(f"attempts {len(acc) + len(rej)}  accepted {len(acc)}  rejected {len(rej)}  "
          f"units {len(units)} (merged {sum(r[7] == 'merged' for r in units)})  cost ${cost:.2f}")
    from collections import Counter
    print(Counter(r[7].split(":")[0].split(" (")[0] for r in rej).most_common(6))


def main():
    ap = argparse.ArgumentParser()
    sp = ap.add_subparsers(dest="cmd", required=True)
    s = sp.add_parser("setup"); s.add_argument("--workers", type=int, default=4); s.set_defaults(f=setup)
    w = sp.add_parser("worker"); w.add_argument("n", type=int)
    w.add_argument("--max-size", type=int, default=256); w.add_argument("--per-unit", type=int, default=6)
    w.add_argument("--mode", choices=["function", "file"], default="function")
    w.add_argument("--soft-usage", type=float, default=0.94)
    w.add_argument("--file-max", type=int, default=1536, help="file mode: max unwritten bytes per unit")
    w.set_defaults(f=worker)
    sp.add_parser("status").set_defaults(f=status)
    a = sp.add_parser("ab"); a.add_argument("--models", default="claude-haiku-5-5,claude-opus-5-5")
    a.add_argument("--per-bucket", type=int, default=4); a.add_argument("--seed", type=int, default=1)
    a.add_argument("--workers", default="1,2,3,4", help="worktree numbers to use")
    a.add_argument("--stop-usage", type=float, default=0.97); a.add_argument("--tag", default="")
    a.set_defaults(f=ab)
    r = sp.add_parser("ab-report"); r.add_argument("--tag", default=""); r.set_defaults(f=ab_report)
    wa = sp.add_parser("watch"); wa.add_argument("--soft", type=float, default=0.94)
    wa.add_argument("--hard", type=float, default=0.95); wa.add_argument("--every", type=int, default=600)
    wa.set_defaults(f=watch)
    a = ap.parse_args()
    a.f(a)


if __name__ == "__main__":
    main()
