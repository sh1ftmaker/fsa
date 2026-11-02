# Coordinator runbook

How to run the per-unit agent workflow, and every rake hit so far. Workers
follow `docs/AGENT_BRIEF.md`, reviewers `docs/REVIEW_BRIEF.md`; this file is
for whoever launches them and merges their work.

## The loop (`tools/coord.py`)

```sh
python3 tools/coord.py queue --lib libs/sdk      # what's left, most unmatched first
python3 tools/coord.py start <unit> --notes "…"  # claim + worktree + worker prompt
#   launch a worker agent with that prompt; wait for its report
python3 tools/coord.py review <unit>             # reviewer prompt
#   launch a fresh reviewer agent with it; never the worker itself
python3 tools/coord.py verdict <unit> ACCEPT     # or CHANGES / REJECT
python3 tools/coord.py merge <unit> "<message>"  # gate, no-regression check, commit, push, cleanup
python3 tools/coord.py status
python3 tools/coord.py promote --apply        # after merges: flag units that names elsewhere completed
```

- CHANGES: send the findings back to the same worker (it keeps its
  context), then review again (`coord.py review`, a fresh reviewer is fine).
- REJECT: release the claim (`tools/claim.py release <unit>`), delete the
  branch, and restart the unit with a note about what went wrong.
- Shared edits a reviewer approved: apply them as a commit on the unit's
  branch, rebuild there, then merge (that keeps `merge_unit.py`'s checks).
- Concurrency: about 6 workers plus their reviewers. Each full build in a
  fresh worktree is the heavy part; reviewers mostly read.
- Notes for `start`: tell the worker what's already known about the unit
  (which names other units found, which donor has the right SDK version).

## Merge commit messages

`<unit> matches (N/N)` or `<unit>: <what> (n/N functions)`, then a short body:
the review verdict, what made it match, any decision (dead-stripped code,
stays NonMatching because …). `merge_unit.py` adds the trailer.

## Rakes (each cost time once)

**Tooling and worktrees**
- Shared `build/tools` across worktrees: fresh checkouts make ninja think the
  tools are stale, so every worktree re-downloaded them into the same files
  ("Text file busy", ETXTBSY). Worktrees get private copies, and
  `download_tool.py` writes a temp file and renames it.
- Tracked generated/runtime files cause merge noise or data loss:
  `objdiff.json` (written by configure.py) and `progress/claims.json` (live
  claims; a merge restoring it wiped every claim) are untracked now.
- Old copies of tools inside branches: worktrees carry the gate from when
  they were cut. Reviewers and merges must run the main checkout's
  `tools/gate.py`.
- The gate must compare against the branch point (`merge-base`), not the
  base branch's tip, or it blames a branch for renames made on `matching`.
- `symbols.txt` holds several symbols at one address (labels with
  functions): anything parsing it must key by (section, address,
  occurrence), not address alone. Both the gate and `merge_unit.py` do.
- dtk rewrites (re-sorts) `symbols.txt` on every split. A merge must keep
  dtk's form: `merge_unit.py` inserts new symbols in place and stages dtk's
  rewrite with the merge, so the tree stays clean afterwards.
- `configure.py` conflicts are usually two neighbouring unit flags;
  `merge_unit.py` applies a branch's flag flips onto the current file and
  refuses any other configure.py change.
- Stale reports: after any experiment in the main checkout (switching
  configure.py, flipping flags), rebuild the report before quoting numbers
  or using it as a baseline. Reviewers twice found the main report stale.
- MWCC under wibo occasionally segfaults (exit 139) on unrelated units when
  many worktrees build at once: rerun the build (at `-j8`) before suspecting
  a source change.
- The same placeholder can be named on two branches (e.g. `__div2i`): if the
  lines differ (scope tags), `merge_unit.py` stops; keep the fuller line.
- Reviewers testing a merge must create their own scratch worktree, never
  reuse one they find (one review copied a build over another's).
- Real symbols' sizes that dtk guessed wrong are changed only through
  `config/G4SE01/approved_symbol_changes.txt` (coordinator, citing a review).
- Two ninja runs in one worktree at once corrupt `.ninja_log` ("manifest
  still dirty" forever); move `.ninja_log`/`.ninja_deps` aside to recover.
  One build at a time per worktree.
- Hung processes: wrap every build/run in `timeout`; start long-lived
  processes with `sbg`. One forgotten test run spun for two hours.

**Measuring honestly**
- Only the ninja report with `functionRelocDiffs=data_value` counts.
  objdiff's lenient default and byte-pattern hits overstated progress
  several times over.
- `Matching` must mean 100 % code and data. 77 units were once marked
  Matching while partly matched; `gate.py` now fails any such unit.
- A unit marked complete can make tiny functions count as matched (10
  8-byte stubs in J3DMatBlock did). Counts from honest flags are lower;
  that's correct.

**Matching and naming**
- Most misses in donor-imported units are data and symbols, not code:
  placeholder names (`lbl_804B5BA0` vs `__CARDBlock`), unsplit `.data` /
  `.sbss` (switch jump tables, file statics), dtk's placeholder blobs
  spanning several real objects. Fix those first.
- One correct name can complete several units (`__CARDBlock`,
  `__shl2i`/`__shr2i`). Tell later workers about names found in this wave.
- Names need evidence from the bytes, not declaration order borrowed from
  another SDK version: `GXNtsc480Prog` was really `GXNtsc480Int` (the
  object's TV/XFB mode fields and its only caller showed it). The review
  caught it.
- Dead-stripped functions stay in the source, like the donors. If they
  leave extra data in our object (a jump table), the unit stays NonMatching
  until the linked DOL's SHA-1 can confirm it (GXPerf.c).
- `OSErrorLine` and similar SDK macros depend on `MATCHING`; this project
  doesn't define it globally (it would switch on JAudio fake-match code).
  Use the form tp/dolsdk2004 use in that function.
- Original SDK `asm` functions (nofralloc) are source, not tricks; workers
  keep them and never add new ones.

## Fakematch debt imported from donors

Donor code came with fakematches: mkdd's `JASFakeMatch*.h` headers (template
instantiations to force weak-function order, included by 37 JAudio files),
mkdd's `unused_force_order_in_data_section*` dummies in `mtx.c`, and TP/TWW
comments admitting a fakematch (`JAISoundHandles.cpp`,
`J3DMaterialFactory.cpp`). None of these units may be `Matching`
(`tools/gate.py` enforces it). Five that were are now NonMatching:
JAISoundChild, JAISoundHandles, JAUSoundTable, JASSeqCtrl,
J3DMaterialFactory. Their functions still count in the function total,
because objdiff can't tell how a function matched; replacing each construct
with natural source is a dedicated task per unit (find the real reason for
the weak-function order: usually where inline/template definitions live).

Fix found for one class: inlining fakematches in SDK code (pragmas, dummy
statements) often stand in for release-build asserts, which expand to
nothing but still count toward the inline size limit. Pad.c matched by
restoring dolsdk2004's `ASSERTMSG` lines and dropping its pragma.
