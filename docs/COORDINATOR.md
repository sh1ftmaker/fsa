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
- Batching: units with under ~1 KiB unmatched get batched reviews (3-5 per
  reviewer, separate verdicts and review files). Tiny units (under ~300
  bytes unmatched) go 3-5 to one worker (`coord.py start a b c`), each still
  on its own branch and merged on its own.
- Compiler flags: a worker may find its unit's donor-copied `extra_cflags`
  wrong (JKernel/JUtility units carry mkdd's `-O4,s`; the original's
  separate `stw`/`lwz` register saves show `-O4,p`). The reviewer judges it
  against the asm; `merge_unit.py` applies a branch's change to its own
  unit's line and refuses any other configure.py change.
- Concurrency: about 6 workers plus their reviewers. Each full build in a
  fresh worktree is the heavy part; reviewers mostly read.
- Notes for `start`: tell the worker what's already known about the unit
  (which names other units found, which donor has the right SDK version).

## Merge commit messages

`<unit> matches (N/N)` or `<unit>: <what> (n/N functions)`, then a short body:
the review verdict, what made it match, any decision (dead-stripped code,
stays NonMatching because …). `merge_unit.py` adds the trailer.

## Progress website (`site/`)

GitHub Pages publishes `site/` on every push to `matching`
(`.github/workflows/pages.yml`). CI can't build the game, so the data is
made locally: `tools/site_data.py` turns the honest report, `objdiff.json`,
`splits.txt`, `lib_map.tsv` and `tu_map.tsv` into `site/data/` (a summary,
one function file per library, and `history.json` with one entry per
commit). `merge_unit.py` and `coord.py promote` run it and stage
`site/data/` in their commit. **After any manual merge or commit to
`matching`, run `python3 tools/site_data.py --subject "<commit subject>"`
after the full `ninja` and stage `site/data/` in the same commit.**
It writes only names, addresses, sizes and match flags, never DOL bytes.
Preview: `sbg start fsasite -- python3 -m http.server 8765 -d site`.

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
- Only `build/G4SE01/report_honest.json` (every unit diffed,
  `functionRelocDiffs=data_value`) counts; progress.py, gate.py and
  coord.py read it.
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
  leave extra data in our object (a jump table), objdiff shows data < 100 %;
  `python3 tools/link_check.py` (with the unit flipped to Matching locally)
  shows whether the linked DOL is still identical: GXPerf.c and mtx.c are
  (mtx.c still has a force-order fakematch), GXPixel.c's .sdata2 is 0x38
  bytes too long.
- objdiff's `report generate` counts a `complete` (Matching) unit as 100 %
  without diffing it, so `report.json` built after flipping a flag can't
  disprove the flag, and the gate read exactly that report. 14 units were
  Matching with 0 % of their code and 12 more broke the link; all 26 are
  NonMatching now. Hence `report_honest.json`, written with no unit
  complete.
- The link is part of every merge: `gate.py` runs `ninja build/G4SE01/ok`
  (link + SHA-1 against build.sha1) and, if it fails, `tools/link_check.py`
  to name the Matching units at fault; `merge_unit.py` runs the full `ninja`
  first and refuses the merge if the SHA-1 check fails. A Matching unit can
  be 100 % in objdiff and still break the link: functions its source defines
  outside its splits (duplicates), statics the DOL references by their
  globalized names, a short `.bss`. `gate.py --no-link` is for quick local
  iteration only. The gate runs ninja itself: never alongside another ninja
  in the same worktree.
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
J3DMaterialFactory. mkdd's `CW_FORCE_STRINGS` / `CW_FORCE_BSS` macros
(`libs/sdk/include/macros.h`) are fakematches too: a fake function that
drags a dead-stripped function's strings or statics into the object. The
natural fix is the dead-stripped function itself (tp keeps them). Seven
JKernel/JAudio/JUtility units use them (JKRDvdArchive, JKRThread,
JKRExpHeap, JKRAramArchive, JAIStreamMgr, JAUSectionHeap, JUTConsole). Their functions still count in the function total,
because objdiff can't tell how a function matched; replacing each construct
with natural source is a dedicated task per unit (find the real reason for
the weak-function order: usually where inline/template definitions live).

JASFakeMatch findings (JASSeqParser worker): 23 built files include the
headers. A generic `template <typename T> ... JASPoolAllocObject_MultiThreaded<T>::memPool_;`
definition reproduces plain `JASFakeMatch.h` (JAISound, JAISoundHandles,
JASSeqCtrl, JASTrack), but it can't live in JASHeapCtrl.h: every unit that
sees it emits the pool static initializers, and Matching JASTrackPort and
JAUStreamFileTable then gain a 0xD4-byte `__sinit` the original lacks (link
SHA-1 fails). So the original defined it only in some translation units;
which header or .cpp is still open. `JASFakeMatch2`–`12` also define
single-threaded `JASPoolAllocObject<JAI*>` pools; a generic definition
reproduces JAISeq, JAISoundChild, JAIStream and JAUInitializer, not the rest.

Other donor quirks to clean up when touching those files (readability
debt, not fakematches by themselves): CARDBlock.c's `goto error`, an unused
`u32 temp[2]` padding local, `card = card =`, and raw `workArea + 0x6000`
casts.

Fix found for one class: inlining fakematches in SDK code (pragmas, dummy
statements) often stand in for release-build asserts, which expand to
nothing but still count toward the inline size limit. Pad.c matched by
restoring dolsdk2004's `ASSERTMSG` lines and dropping its pragma.
