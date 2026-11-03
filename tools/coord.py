#!/usr/bin/env python3
"""Coordinator workflow: everything around the decompiling itself.

  python3 tools/coord.py queue [--lib PREFIX] [-n N] [--small]
      Unclaimed units that aren't fully matched, most unmatched code first
      (--small: least first). Game units (--lib src/game) are queued before
      their source exists; the worker creates src/game/<name>.cpp.
  python3 tools/coord.py start <unit>... [--owner NAME] [--notes TEXT]
      Claim the units, create their worktrees and branches, print the worker
      prompt (one combined prompt for several tiny units).
  python3 tools/coord.py review <unit>...
      Mark the units "in review" and print the adversarial reviewer prompt
      (one batched prompt for several small units; separate verdicts).
  python3 tools/coord.py verdict <unit> ACCEPT|CHANGES|REJECT
      Record the reviewer's verdict.
  python3 tools/coord.py merge <unit> "<message>"
      Merge an ACCEPTed unit (tools/merge_unit.py), push, remove the worktree.
  python3 tools/coord.py status
      Every claimed unit with its state.
  python3 tools/coord.py promote [--apply]
      Unclaimed NonMatching units now 100 % in code and data with no
      fakematch construct (often completed by names found elsewhere); with
      --apply, flag them Matching, rebuild, gate, commit and push.

Prompts come from docs/prompts/{unit,review}.md. Agents themselves are
launched by the coordinator (a person or a Claude session) with those prompts.
"""

import argparse
import fcntl
import json
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
CLAIMS = REPO / "progress" / "claims.json"
DONORS = REPO.parent / "donors"
REVIEWS = REPO.parent / "fsa-wt" / "_reviews"
LIB_ROOTS = ("libs/sdk/", "libs/sdk_gx/", "libs/jsystem/", "src/")


def run(*cmd, cwd=REPO, check=True):
    out = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    if check and out.returncode != 0:
        sys.exit(f"{' '.join(map(str, cmd))} failed:\n{out.stdout}{out.stderr}")
    return out.stdout


def slug_of(unit):
    return re.sub(r"\.[^.]*$", "", unit).replace("/", "__")


def with_claims(update=None):
    with open(CLAIMS, "r+") as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        claims = json.load(f)
        if update:
            update(claims)
            f.seek(0)
            f.truncate()
            json.dump(claims, f, indent=2, sort_keys=True)
            f.write("\n")
        return claims


def honest_units():
    """Units of the honest report (every unit diffed; see tools/honest_report.py), rebuilt first."""
    run("timeout", "1800", "ninja", "build/G4SE01/report_honest.json")
    return json.loads((REPO / "build/G4SE01/report_honest.json").read_text())["units"]


def unit_rows():
    """(unit, source, unmatched code bytes, status text) for every unit in the report."""
    sources = {}
    for u in json.loads((REPO / "objdiff.json").read_text())["units"]:
        src = (u.get("metadata") or {}).get("source_path") or ""
        name = u["name"].split("/", 1)[-1]
        if not src and name.startswith("game/") and "game" in ((u.get("metadata") or {}).get("progress_categories") or []):
            # a game unit nobody has started: configure.py lists it, its source comes with its agent
            src = f"src/{name}.cpp"
        for root in LIB_ROOTS:
            if src.startswith(root):
                sources[u["name"]] = (src[len(root):], src)
    rows = []
    for u in honest_units():
        if u["name"] not in sources:
            continue
        m = u["measures"]
        tc, mc = int(m.get("total_code", 0)), int(m.get("matched_code", 0))
        td, md = int(m.get("total_data", 0)), int(m.get("matched_data", 0))
        if mc == tc and md == td:
            continue
        unit, src = sources[u["name"]]
        status = (f"{m.get('matched_functions') or 0}/{m.get('total_functions') or 0} functions match, "
                  f"code {mc}/{tc} bytes, data {md}/{td} bytes.")
        rows.append((unit, src, tc - mc + td - md, status))
    return sorted(rows, key=lambda r: -r[2])


def cmd_queue(args):
    claims = with_claims()
    shown = 0
    rows = unit_rows()
    if args.small:
        rows = rows[::-1]
    for unit, src, missing, status in rows:
        if unit in claims or (args.lib and not src.startswith(args.lib)):
            continue
        print(f"{missing:7}  {unit:48} {status}")
        shown += 1
        if shown >= args.n:
            break


BATCH_WORK = """You own {n} small units of the Four Swords Adventures byte-matching
decompilation. Do them one after another, each completely (including its
commit and checkpoint) before the next; each has its own worktree and branch,
and each gets its own final report section. The unit instructions follow.

"""
BATCH_REVIEW = """You review {n} small unit branches of the Four Swords Adventures
byte-matching decompilation. Review each one independently, as if it were the
only one: its own measurements, its own review file, its own verdict line
(`<unit>: ACCEPT|CHANGES|REJECT`). One unit's verdict must not influence
another's. The per-unit instructions follow.

"""


def start_one(unit, owner, notes):
    rows = {r[0]: r for r in unit_rows()}
    if unit not in rows:
        sys.exit(f"{unit}: not in the queue (fully matched or unknown)")
    _, src, _, status = rows[unit]
    wt = run("sh", "tools/unit_worktree.sh", unit, owner).strip().splitlines()[-1]
    wt = str(Path(wt).resolve())
    with_claims(lambda c: c[unit].update(state="working"))
    template = (REPO / "docs/prompts/unit.md").read_text()
    return template.format(unit=unit, source=src, worktree=wt, branch=f"unit/{slug_of(unit)}",
                           status=status, notes=notes or "")


def cmd_start(args):
    prompts = [start_one(u, args.owner, args.notes) for u in args.units]
    if len(prompts) == 1:
        print(prompts[0])
    else:
        print(BATCH_WORK.format(n=len(prompts)) + "\n\n---\n\n".join(prompts))


def review_one(unit):
    claims = with_claims()
    if unit not in claims:
        sys.exit(f"{unit} is not claimed")
    wt = str(Path(claims[unit]["worktree"]).resolve())
    base = run("git", "-C", wt, "merge-base", "matching", "HEAD").strip()
    log = run("git", "-C", wt, "log", "--format=%h %s", f"{base}..HEAD")
    if not log.strip():
        sys.exit(f"{unit}: no commits on the branch")
    baseline = run("python3", "tools/progress.py").splitlines()[0]
    REVIEWS.mkdir(parents=True, exist_ok=True)
    with_claims(lambda c: c[unit].update(state="review"))
    template = (REPO / "docs/prompts/review.md").read_text()
    return template.format(unit=unit, branch=f"unit/{slug_of(unit)}", worktree=wt, repo=REPO,
                           slug=slug_of(unit), baseline=baseline, donors=DONORS, log=log.strip(),
                           reviews=REVIEWS)


def cmd_review(args):
    prompts = [review_one(u) for u in args.units]
    if len(prompts) == 1:
        print(prompts[0])
    else:
        print(BATCH_REVIEW.format(n=len(prompts)) + "\n\n---\n\n".join(prompts))


def cmd_verdict(args):
    verdict = args.verdict.upper()
    if verdict not in ("ACCEPT", "CHANGES", "REJECT"):
        sys.exit("verdict must be ACCEPT, CHANGES or REJECT")
    with_claims(lambda c: c[args.unit].update(state=verdict.lower()))
    print(f"{args.unit}: {verdict.lower()}")


def cmd_merge(args):
    claims = with_claims()
    state = claims.get(args.unit, {}).get("state")
    if state != "accept":
        sys.exit(f"{args.unit}: state is {state!r}; only ACCEPTed units merge")
    wt = claims[args.unit]["worktree"]
    subprocess.run(["python3", "tools/merge_unit.py", args.unit, args.message], cwd=REPO, check=True)
    run("git", "push", "-q")
    run("git", "worktree", "remove", "--force", wt, check=False)
    run("git", "branch", "-D", f"unit/{slug_of(args.unit)}", check=False)
    print(f"merged, pushed, worktree removed: {args.unit}")


def cmd_status(_):
    for unit, c in sorted(with_claims().items()):
        print(f"{c.get('state', 'working'):9} {unit:48} {c['owner']:16} {c['started']}")


def cmd_promote(args):
    claims = with_claims()
    report = {u["name"]: u for u in honest_units()}
    fake = re.compile(r"FakeMatch|force_order|fakematch|NO_INLINE\(\)|CW_FORCE_(STRINGS|BSS)|fake_function", re.I)
    ready = []
    # Units held back with a FIXME (link breaks, donor fakematches) stay NonMatching until fixed.
    held = set(re.findall(r'Object\(NonMatching, "([^"]+)".*# FIXME', (REPO / "configure.py").read_text()))
    for u in json.loads((REPO / "objdiff.json").read_text())["units"]:
        meta = u.get("metadata") or {}
        src = meta.get("source_path") or ""
        if meta.get("complete") or not src.startswith(LIB_ROOTS):
            continue
        rel = next(src[len(r):] for r in LIB_ROOTS if src.startswith(r))
        if rel in held:
            continue
        m = report.get(u["name"], {}).get("measures", {})
        if (rel in claims or not int(m.get("total_code", 0)) or m.get("matched_code") != m.get("total_code")
                or int(m.get("matched_data", 0)) != int(m.get("total_data", 0))):
            continue
        if fake.search((REPO / src).read_text(errors="replace")):
            print(f"skip (fakematch): {rel}")
            continue
        ready.append(rel)
    for rel in ready:
        print(f"ready: {rel}")
    if not args.apply or not ready:
        return
    original = (REPO / "configure.py").read_text()
    for _ in range(4):
        cfg = original
        for rel in ready:
            cfg = cfg.replace(f'Object(NonMatching, "{rel}"', f'Object(Matching, "{rel}"')
        (REPO / "configure.py").write_text(cfg)
        run("python3", "configure.py")
        gate = subprocess.run(["python3", "tools/gate.py", "--all-units"], cwd=REPO, capture_output=True, text=True)
        if gate.returncode == 0:
            break
        # Units that are 100 % but break the link (usually a short split): leave them out and retry.
        broken = {rel for rel in ready if re.search(r"FAIL \S*" + re.escape(rel) + r": marked Matching but doesn't link", gate.stdout)}
        if not broken and len(ready) > 1:
            # The link check blamed units that were already Matching (a candidate shifted the
            # layout): try the candidates one at a time and keep those that pass on their own.
            alone = []
            for rel in ready:
                (REPO / "configure.py").write_text(original.replace(f'Object(NonMatching, "{rel}"', f'Object(Matching, "{rel}"'))
                run("python3", "configure.py")
                if subprocess.run(["python3", "tools/gate.py", "--all-units"], cwd=REPO, capture_output=True).returncode == 0:
                    alone.append(rel)
                else:
                    print(f"skip (breaks the link): {rel}")
            broken = set(ready) - set(alone)
        if not broken:
            break
        for rel in sorted(broken):
            print(f"skip (breaks the link): {rel}")
        ready = [rel for rel in ready if rel not in broken]
        if not ready:
            break
    if gate.returncode != 0 or not ready:
        (REPO / "configure.py").write_text(original)
        run("python3", "configure.py")
        sys.exit("nothing promoted:\n" + gate.stdout)
    run("git", "add", "configure.py")
    run("python3", "tools/site_data.py", "--subject", "Flag units Matching that are now 100 % in code and data", "--stage")  # site data, README visuals, README
    run("git", "commit", "-q", "-m",
        "Flag units Matching that are now 100 % in code and data\n\n" + "\n".join(ready)
        + "\n\nCompleted by names found in other units; no fakematch constructs; gate ok."
        + "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
    run("git", "push", "-q")
    print(f"promoted {len(ready)} units")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    q = sub.add_parser("queue")
    q.add_argument("--lib", default="")
    q.add_argument("-n", type=int, default=20)
    q.add_argument("--small", action="store_true", help="least unmatched first (small files)")
    s = sub.add_parser("start")
    s.add_argument("units", nargs="+")
    s.add_argument("--owner", default="agent")
    s.add_argument("--notes", default="")
    r = sub.add_parser("review")
    r.add_argument("units", nargs="+")
    v = sub.add_parser("verdict")
    v.add_argument("unit")
    v.add_argument("verdict")
    m = sub.add_parser("merge")
    m.add_argument("unit")
    m.add_argument("message")
    sub.add_parser("status")
    pr = sub.add_parser("promote")
    pr.add_argument("--apply", action="store_true")
    args = ap.parse_args()
    {"queue": cmd_queue, "start": cmd_start, "review": cmd_review, "verdict": cmd_verdict,
     "merge": cmd_merge, "status": cmd_status, "promote": cmd_promote}[args.cmd](args)


if __name__ == "__main__":
    main()
