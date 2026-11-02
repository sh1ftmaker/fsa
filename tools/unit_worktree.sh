#!/bin/sh
# Create a worktree and branch for one unit, ready to build.
# Usage: tools/unit_worktree.sh <unit source path, e.g. dolphin/dvd/dvd.c> <owner>
# Prints the worktree path.
set -eu
unit="$1"
owner="$2"
repo=$(git rev-parse --show-toplevel)
slug=$(printf '%s' "$unit" | sed 's#/#__#g; s#\.[^.]*$##')
wt="$repo/../fsa-wt/$slug"
branch="unit/$slug"

python3 "$repo/tools/claim.py" take "$unit" "$owner" "$branch" "$wt"
git -C "$repo" worktree add -q "$wt" -b "$branch" matching
# Game data and downloaded tools are shared, never committed.
ln -s "$repo/orig" "$wt/orig"
mkdir -p "$wt/build"
ln -s "$repo/build/compilers" "$wt/build/compilers"
ln -s "$repo/build/tools" "$wt/build/tools"
# Claims are kept in the main checkout only.
rm -f "$wt/progress/claims.json"
ln -s "$repo/progress/claims.json" "$wt/progress/claims.json"
(cd "$wt" && python3 configure.py >/dev/null)
echo "$wt"
