#!/usr/bin/env python3
"""Coordinator workflow: everything around the decompiling itself.

  python3 tools/coord.py queue [--lib PREFIX] [-n N]
      Unclaimed units that aren't fully matched, most unmatched code first.
  python3 tools/coord.py start <unit> [--owner NAME] [--notes TEXT]
      Claim the unit, create its worktree and branch, print the worker prompt.
  python3 tools/coord.py review <unit>
      Mark the unit "in review" and print the adversarial reviewer prompt.
  python3 tools/coord.py verdict <unit> ACCEPT|CHANGES|REJECT
      Record the reviewer's verdict.
  python3 tools/coord.py merge <unit> "<message>"
      Merge an ACCEPTed unit (tools/merge_unit.py), push, remove the worktree.
  python3 tools/coord.py status
      Every claimed unit with its state.

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


def unit_rows():
    """(unit, source, unmatched code bytes, status text) for every unit in the report."""
    sources = {}
    for u in json.loads((REPO / "objdiff.json").read_text())["units"]:
        src = (u.get("metadata") or {}).get("source_path") or ""
        for root in LIB_ROOTS:
            if src.startswith(root):
                sources[u["name"]] = (src[len(root):], src)
    rows = []
    for u in json.loads((REPO / "build/G4SE01/report_honest.json").read_text())["units"]:
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
    for unit, src, missing, status in unit_rows():
        if unit in claims or (args.lib and not src.startswith(args.lib)):
            continue
        print(f"{missing:7}  {unit:48} {status}")
        shown += 1
        if shown >= args.n:
            break


def cmd_start(args):
    rows = {r[0]: r for r in unit_rows()}
    if args.unit not in rows:
        sys.exit(f"{args.unit}: not in the queue (fully matched or unknown)")
    _, src, _, status = rows[args.unit]
    wt = run("sh", "tools/unit_worktree.sh", args.unit, args.owner).strip().splitlines()[-1]
    wt = str(Path(wt).resolve())
    with_claims(lambda c: c[args.unit].update(state="working"))
    template = (REPO / "docs/prompts/unit.md").read_text()
    print(template.format(unit=args.unit, source=src, worktree=wt, branch=f"unit/{slug_of(args.unit)}",
                          status=status, notes=args.notes or ""))


def cmd_review(args):
    claims = with_claims()
    if args.unit not in claims:
        sys.exit(f"{args.unit} is not claimed")
    wt = claims[args.unit]["worktree"]
    wt = str(Path(wt).resolve())
    base = run("git", "-C", wt, "merge-base", "matching", "HEAD").strip()
    log = run("git", "-C", wt, "log", "--format=%h %s", f"{base}..HEAD")
    if not log.strip():
        sys.exit(f"{args.unit}: no commits on the branch")
    baseline = run("python3", "tools/progress.py").splitlines()[0]
    REVIEWS.mkdir(parents=True, exist_ok=True)
    with_claims(lambda c: c[args.unit].update(state="review"))
    template = (REPO / "docs/prompts/review.md").read_text()
    print(template.format(unit=args.unit, branch=f"unit/{slug_of(args.unit)}", worktree=wt, repo=REPO,
                          slug=slug_of(args.unit), baseline=baseline, donors=DONORS, log=log.strip(),
                          reviews=REVIEWS))


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


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    q = sub.add_parser("queue")
    q.add_argument("--lib", default="")
    q.add_argument("-n", type=int, default=20)
    s = sub.add_parser("start")
    s.add_argument("unit")
    s.add_argument("--owner", default="agent")
    s.add_argument("--notes", default="")
    r = sub.add_parser("review")
    r.add_argument("unit")
    v = sub.add_parser("verdict")
    v.add_argument("unit")
    v.add_argument("verdict")
    m = sub.add_parser("merge")
    m.add_argument("unit")
    m.add_argument("message")
    sub.add_parser("status")
    args = ap.parse_args()
    {"queue": cmd_queue, "start": cmd_start, "review": cmd_review, "verdict": cmd_verdict,
     "merge": cmd_merge, "status": cmd_status}[args.cmd](args)


if __name__ == "__main__":
    main()
