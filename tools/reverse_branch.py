#!/usr/bin/env python3
"""Carry a branch made before the deferred-inlining switch onto the new `matching`.

The switch (docs/INLINING.md) reversed every game .cpp (descending address order)
and then hand-fixed a few (auto_inline regions, an explicit dtor, split moves). A
plain rebase of an older branch conflicts on every game source it touched. This
script "reverses, then rebases": for each game .cpp the branch changed it runs a
three-way merge in which all three sides are in deferred (reversed) order:

    base   = reverse_funcs(the file at the fork point)
    ours   = the file on the new matching (reversed + hand fixes)
    theirs = reverse_funcs(the file on the branch)

so the merge only sees the branch's own edits against the switch's hand fixes.
Every other changed file is taken over with `git diff fork..branch | git apply -3`.
The result is committed as one commit on a NEW branch (default <branch>-deferred)
created from the target; the old branch is never touched.

Usage (in a clean worktree):
  python3 tools/reverse_branch.py <branch> [--onto matching] [--name <new branch>]

Afterwards: resolve any conflict markers it reports, build, and check
`python3 tools/order_check.py <unit>` and `tools/try.py` as usual. A branch that
edits a generator-pinned unit (config/G4SE01/gen_game.tsv) must re-pin it.
Already-deferred sources (DEFERRED_ALREADY) are merged without reversal.
"""
import argparse
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from reverse_funcs import reverse_source  # noqa: E402

# Sources that were in deferred order before the switch (not reversed by it).
DEFERRED_ALREADY = {"src/game/tu_80427D2C.cpp", "src/game/tu_80222CC4.cpp", "src/game/tu_80221B44.cpp"}


def git(*args, check=True, input=None):
    r = subprocess.run(["git", *args], capture_output=True, text=True, input=input)
    if check and r.returncode:
        sys.exit(f"git {' '.join(args)}: {r.stderr.strip()}")
    return r


def show(rev, path):
    r = git("show", f"{rev}:{path}", check=False)
    return r.stdout if r.returncode == 0 else None


def rev_text(text, path):
    if text is None or path in DEFERRED_ALREADY:
        return text
    out, _, _ = reverse_source(text)
    return text if out is None else out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("branch")
    ap.add_argument("--onto", default="matching")
    ap.add_argument("--name")
    a = ap.parse_args()
    if git("status", "--porcelain", "--untracked-files=no").stdout.strip():
        sys.exit("worktree not clean")
    fork = git("merge-base", a.branch, a.onto).stdout.strip()
    changed = git("diff", "--name-only", fork, a.branch).stdout.split()
    game = [p for p in changed if p.startswith("src/game/") and p.endswith(".cpp")]
    other = [p for p in changed if p not in game]
    new = a.name or f"{a.branch}-deferred"
    git("checkout", "-b", new, a.onto)
    conflicts = []
    if other:
        patch = git("diff", "--binary", fork, a.branch, "--", *other).stdout
        r = git("apply", "-3", "--index", input=patch, check=False)
        if r.returncode:
            conflicts.append("non-game files (git apply -3): " + r.stderr.strip()[-400:])
    tmp = Path(git("rev-parse", "--git-dir").stdout.strip()) / "reverse_branch"
    tmp.mkdir(exist_ok=True)
    for p in game:
        theirs = show(a.branch, p)
        if theirs is None:  # deleted on the branch
            git("rm", "-q", "--ignore-unmatch", p)
            continue
        base, ours = rev_text(show(fork, p), p), show(a.onto, p)
        theirs = rev_text(theirs, p)
        if ours is None or base is None:  # new on the branch (or gone on the target)
            Path(p).write_text(theirs)
        else:
            files = []
            for tag, text in (("ours", ours), ("base", base), ("theirs", theirs)):
                f = tmp / tag
                f.write_text(text)
                files.append(str(f))
            r = subprocess.run(["git", "merge-file", "-p", "-L", a.onto, "-L", "fork (reversed)",
                                "-L", f"{a.branch} (reversed)", *files], capture_output=True, text=True)
            Path(p).write_text(r.stdout)
            if r.returncode:
                conflicts.append(p)
        git("add", p)
    if conflicts:
        print("conflicts to resolve, then commit:\n  " + "\n  ".join(conflicts))
        return 1
    git("commit", "-q", "-m", f"{a.branch}: carried onto {a.onto} in deferred order (tools/reverse_branch.py)")
    print(f"{new}: {len(game)} game sources merged reversed, {len(other)} other files applied; build and check it")
    return 0


if __name__ == "__main__":
    sys.exit(main())
