#!/usr/bin/env python3
"""Merge one reviewed unit branch into the current branch (matching).

Usage: python3 tools/merge_unit.py <unit source path> "<commit message>"

Steps: squash-merge unit/<slug>; merge symbols.txt per address (three-way
against the branch point, so independent renames on both sides combine);
drop generated files; reconfigure, re-split and rebuild the report; run the
gate; refuse if the progress total went down; commit; release the claim.
Only run it after the unit's adversarial review says ACCEPT.
"""

import re
import subprocess
import sys
from pathlib import Path

SYMBOLS = "config/G4SE01/symbols.txt"
OBJECT = re.compile(r'Object\((Matching|NonMatching|Equivalent)(, "[^"]+")')
LINE = re.compile(r"^(\S+) = (\S+):(0x[0-9A-Fa-f]+);")
TRAILER = "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"


def run(*cmd, check=True):
    out = subprocess.run(cmd, capture_output=True, text=True)
    if check and out.returncode != 0:
        sys.exit(f"{' '.join(cmd)} failed:\n{out.stdout}{out.stderr}")
    return out.stdout


def keyed(text):
    """Key each line by (section, address, n): several symbols can share an address."""
    lines, order, seen = {}, [], {}
    for line in text.splitlines():
        m = LINE.match(line)
        place = (m.group(2), int(m.group(3), 16)) if m else ("#", line)
        n = seen.get(place, 0)
        seen[place] = n + 1
        key = (*place, n)
        order.append(key)
        lines[key] = line
    return lines, order


def merge_symbols(base, ours, theirs):
    """Per address: take the side that changed; if both changed differently, fail."""
    b, _ = keyed(base)
    o, order = keyed(ours)
    t, t_order = keyed(theirs)
    out = []
    seen = set()
    for key in order + [k for k in t_order if k not in o]:
        if key in seen:
            continue
        seen.add(key)
        bo, oo, to = b.get(key), o.get(key), t.get(key)
        if to == bo:
            line = oo
        elif oo == bo or oo == to:
            line = to
        else:
            sys.exit(f"symbols.txt: both sides changed {key}: {oo!r} vs {to!r}")
        if line is not None:
            out.append(line)
    return "\n".join(out) + "\n"


def merge_configure(base, ours, theirs):
    """Apply the branch's Matching/NonMatching flips to ours; anything else must be identical."""
    flips = {}
    base_flags = {m.group(2): m.group(1) for m in OBJECT.finditer(base)}
    for m in OBJECT.finditer(theirs):
        if base_flags.get(m.group(2)) != m.group(1):
            flips[m.group(2)] = m.group(1)
    if OBJECT.sub(r"Object(X\2", base) != OBJECT.sub(r"Object(X\2", theirs):
        sys.exit("configure.py: the branch changed more than unit flags; merge by hand")
    return OBJECT.sub(lambda m: f"Object({flips.get(m.group(2), m.group(1))}{m.group(2)}", ours)


def progress_total():
    first = run("python3", "tools/progress.py").splitlines()[0]
    return int(first.split()[1])


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    unit, message = sys.argv[1], sys.argv[2]
    slug = re.sub(r"\.[^.]*$", "", unit).replace("/", "__")
    branch = f"unit/{slug}"
    if run("git", "status", "--porcelain", "--untracked-files=no").strip():
        sys.exit("working tree not clean; commit or restore first")
    before = progress_total()
    base = run("git", "merge-base", "HEAD", branch).strip()
    sym_base = run("git", "show", f"{base}:{SYMBOLS}")
    sym_ours = Path(SYMBOLS).read_text()
    sym_theirs = run("git", "show", f"{branch}:{SYMBOLS}")
    cfg = [run("git", "show", f"{rev}:configure.py") for rev in (base, "HEAD", branch)]

    subprocess.run(["git", "merge", "--squash", branch], capture_output=True, text=True)
    Path(SYMBOLS).write_text(merge_symbols(sym_base, sym_ours, sym_theirs))
    run("git", "add", SYMBOLS)
    Path("configure.py").write_text(merge_configure(*cfg))
    run("git", "add", "configure.py")
    for generated in ("objdiff.json", "progress/claims.json"):
        run("git", "checkout", "HEAD", "--", generated, check=False)
    conflicts = run("git", "diff", "--name-only", "--diff-filter=U").split()
    if conflicts:
        sys.exit(f"unresolved conflicts: {conflicts} (resolve, then rerun the checks by hand)")

    run("python3", "configure.py")
    Path("build/G4SE01/config.json").unlink(missing_ok=True)
    run("timeout", "1800", "ninja", "build/G4SE01/report.json")
    gate = subprocess.run(["python3", "tools/gate.py", "--base", "HEAD"], capture_output=True, text=True)
    print(gate.stdout.strip())
    if gate.returncode != 0:
        sys.exit("gate failed; merge left staged for inspection")
    after = progress_total()
    print(f"progress: {before} -> {after} functions")
    if after < before:
        sys.exit("progress went down; merge left staged for inspection")
    run("git", "-c", "user.name=sh1ftmaker", "-c", "user.email=shiftmaker@gmail.com",
        "commit", "-q", "-m", message + TRAILER)
    run("python3", "tools/claim.py", "release", unit)
    print(run("git", "log", "--oneline", "-1").strip())


if __name__ == "__main__":
    main()
