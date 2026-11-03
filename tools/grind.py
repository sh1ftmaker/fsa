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

def fdiff(wt, unit):
    """Build the unit's object (try.py re-splits if needed) and return (functions, sections)."""
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
    base = Path(unit).stem
    allowed = {f"src/{unit}", f"include/game/act/{base}.h", "config/G4SE01/symbols.txt"}
    st = sh(["git", "status", "--porcelain"], wt).stdout.splitlines()
    paths = {l[3:] for l in st if l[3:] not in ("progress/claims.json", "objdiff.json")}
    extra = paths - allowed
    if extra:
        return f"touched {sorted(extra)}", None
    added = [l for l in sh(["git", "diff", "-U0"], wt).stdout.splitlines() if l.startswith("+")]
    added += sh(["git", "ls-files", "--others", "--exclude-standard"], wt).stdout.splitlines()
    for l in added:
        if BANNED.search(l):
            return f"banned: {l.strip()[:80]}", None
    ren = symbol_renames(wt)
    if ren is None:
        return "symbols.txt change is not a pure rename", None
    for old in ren:
        if sh(["git", "grep", "-q", "-w", old, "--", "src", "include", "libs"], wt).returncode == 0:
            return f"renamed {old} is still used in source", None
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

Rules (a script checks them and throws away your work if broken):
- Edit only `src/{unit}`, `include/game/act/{base}.h`, and, for a placeholder name only,
  rename lines in config/G4SE01/symbols.txt (change just the name before ` = `; nothing else).
- No asm, no #pragma, no goto, no dummy/fake functions, no tricks: write natural C++ with real
  types, as the original programmers would. Other functions in the file must not get worse.
- Do not run git, ninja or any build other than tools/try.py.
End with one line: the final match % of {fn}.
"""


def run_haiku(wt, unit, fn, size, pct):
    prompt = PROMPT.format(fn=fn, size=size, pct=f"{pct:.2f} %" if pct else "missing",
                           unit=unit, base=Path(unit).stem)
    tools = ",".join(["Read", "Edit", "Write", "Grep", "Glob",
                      "Bash(python3 tools/try.py:*)", "Bash(python3 tools/fdiff.py:*)",
                      "Bash(grep:*)", "Bash(sed -n:*)", "Bash(head:*)", "Bash(tail:*)"])
    try:
        r = sh(["claude", "-p", prompt, "--model", MODEL, "--output-format", "json",
                "--permission-mode", "acceptEdits", "--allowedTools", tools,
                "--disallowedTools", "Agent,WebFetch,WebSearch"], wt, timeout=1200)
        out = r.stdout
    except subprocess.TimeoutExpired:
        out = '{"result": "timeout"}'
    (LOGS / "haiku").mkdir(parents=True, exist_ok=True)
    (LOGS / "haiku" / f"{Path(unit).stem}__{fn[:80]}.json").write_text(out)
    try:
        return json.loads(out).get("total_cost_usd", 0)
    except ValueError:
        return 0


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
    out = []
    for u in report:
        if "/game/" not in u["name"]:
            continue
        unit = "game/" + u["name"].split("/game/")[1] + ".cpp"
        if not (INTEG / "src" / unit).exists():  # no source yet: try.py/fdiff cannot score it
            continue
        if f'Object(NonMatching, "{unit}"' not in cfg:
            continue
        fns = [f for f in u.get("functions", []) if f.get("fuzzy_match_percent", 0) < 100
               and int(f.get("size", 0)) <= max_size]
        if fns:
            out.append((unit, len(fns)))
    out.sort(key=lambda x: -x[1])
    return [u for u, _ in out]


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


def merge_into_grind(wt, branch, unit):
    lock = open(LOGS / "merge.lock", "w")
    fcntl.flock(lock, fcntl.LOCK_EX)
    pre = sh(["git", "rev-parse", "HEAD"], INTEG, check=True).stdout.strip()
    if sh(["git", "merge", "-q", "--no-edit", branch], INTEG).returncode:
        sh(["git", "merge", "--abort"], INTEG)
        return "merge conflict"
    ok = sh("timeout 3600 ninja >/dev/null 2>&1 && sha1sum -c config/G4SE01/build.sha1", INTEG).returncode == 0
    why = "" if ok else "SHA-1"
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


def worker(args):
    # sbg stop sends SIGTERM: unwind so the claim is released and the worktree reverted.
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(1))
    n, owner = args.n, f"grind-w{args.n}"
    wt, branch = WT / f"grind-w{n}", f"grind-w{n}"
    while not (LOGS / "STOP").exists():
        unit = next((u for u in candidates(args.max_size) if claim(u, owner, wt)), None)
        if unit is None:
            notify("FSA grind", f"{owner}: no units left")
            return
        sh(["git", "reset", "-q", "--hard", "grind"], wt, check=True)
        revert(wt)
        try:
            fns, secs = fdiff(wt, unit)
            todo = sorted((s, f, p) for f, (s, p) in fns.items() if p < 100 and s <= args.max_size)
            accepted = 0
            for size, fn, pct in todo[:args.per_unit]:
                if (LOGS / "STOP").exists():
                    break
                t0 = time.time()
                cost = run_haiku(wt, unit, fn, size, pct)
                why, ren = check_diff(wt, unit)
                new_fns, new_secs = (fns, secs) if why else fdiff(wt, unit)
                if not why:
                    ren = ren or {}
                    renamed = {ren.get(k, k): v for k, v in fns.items()}
                    worse = [k for k, (s, p) in renamed.items() if new_fns.get(k, (0, 0))[1] < p - 0.001]
                    worse += [k for k, p in secs.items() if new_secs.get(k, 0) < p - 0.001]
                    gained = sum(p == 100 for _, p in new_fns.values()) - sum(p == 100 for _, p in fns.values())
                    if worse:
                        why = f"worse: {worse[:3]}"
                    elif gained <= 0:
                        why = f"no new 100 % (best {new_fns.get(ren.get(fn, fn), (0, 0))[1]:.2f})"
                if why:
                    revert(wt)
                    log([owner, unit, fn, size, f"{pct:.2f}", "REJECT", why, f"{cost:.3f}", int(time.time() - t0)])
                    continue
                sh(["git", "add", "-A", "src", "include", "config/G4SE01/symbols.txt"], wt)
                sh(["git", "commit", "-qm", f"grind: {unit} {ren.get(fn, fn) if ren else fn} matches\n\n{TRAILER}"], wt)
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
    w.set_defaults(f=worker)
    sp.add_parser("status").set_defaults(f=status)
    a = ap.parse_args()
    a.f(a)


if __name__ == "__main__":
    main()
