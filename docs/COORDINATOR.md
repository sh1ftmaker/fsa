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

## Harvesting names from the link (`tools/harvest_names.py`)

After merges, once a full `ninja` ends in SHA-1 OK:

```sh
python3 tools/harvest_names.py -v        # list what it would change
python3 tools/harvest_names.py --write   # apply, then rm build/G4SE01/config.json && ninja
```

It runs `dtk dol apply` (the linked ELF's names back into symbols.txt) on a
scratch copy of the config and keeps only placeholders (`fn_`, `lbl_`,
`*_80xxxxxx`) that a Matching unit's compiled object names at exactly that
address (split range start + offset in the object) with the same size. A
placeholder dtk guessed too large is split into the object's symbols when
they tile it exactly (`lbl_8045E030` -> `atanhi`, `atanlo`, `aT`). Real
symbols are never touched; everything else `dol apply` would do (scope
changes, `@etb_`/`...data.0` names, names from dtk's own split objects) is
dropped. The first run named 15 placeholders (gxData, ErrorTable,
two_over_pi, the JASOscillator curve tables, ...). Commit the result like
any symbols.txt change (the gate accepts placeholder renames).

## Game units

- All 528 game units are in `splits.txt` and configure.py's
  `GameLib("game", ...)` block (`NonMatching`, category `game`) before
  anyone starts them; `config/G4SE01/game_units.tsv` lists each unit's
  range, confidence, sections and name evidence (actor IDs included).
- Names: `game/tu_<text start>.cpp` (257 units; the address is tu_map's
  row start, kept when a thunk moved to the previous unit), or an evidenced name
  (270 `game/act_<actor>.cpp` from the actor table, `game/RailBase.cpp`).
  Renames need evidence and touch the `splits.txt` stanza, the configure.py
  line and `game_units.tsv` together: a coordinator edit, not a worker's.
- Queue: `python3 tools/coord.py queue --lib src/game --small` (smallest
  first; game units show up before their source exists). `coord.py start
  game/tu_8014FD5C.cpp` claims one as usual; the worker creates
  `src/game/<name>.cpp` (headers under `include/game/`), and configure.py
  picks the file up on the next `python3 configure.py`.
- Build: `cflags_game` (docs/PLAN.md, "Game code"); the THP unit
  `game/tu_80427D2C.cpp` adds `-inline off,deferred` and includes sms's
  THP sources. The worker renames `fn_` placeholders to the mangled names
  its source emits (PLAN.md, "Symbols").
- Tell game workers in `--notes`: the unit's `game_units.tsv` row, the
  actor IDs it constructs, and which `references ... inside another unit`
  note of `to_splits.py` concerns it (extern data owned elsewhere). Those
  notes cover only references the tool anchors to one unit; constants
  several units read (shared `.sdata2`) aren't listed, so a worker who
  finds its relocations pointing into a neighbour's data reports it as a
  SHARED-EDIT.
- 38 medium units may hold several files: a boundary found inside one is a
  SHARED-EDIT for the coordinator (split stanza, new configure.py line).
- Weak `memPool_` copies in `.bss` (tell game workers in `--notes`): a unit
  whose `__sinit` constructs the 9 JAudio pools has, after its 9 0xC
  destructor-chain objects (`@NNN`, the names its object emits), a 0x94
  placeholder (e.g. `lbl_804CD1A4`) that is really 9 weak 0x10 `memPool_`
  copies: the linker resolves the names to the first copy (tu_80021840's)
  but keeps each object's space. The worker splits that placeholder in
  symbols.txt into 9 unnamed `lbl_<addr>` symbols of size 0x10, one per
  copy at +0x00..+0x80, in the object's own order (the `_MultiThreaded`
  pools first), and leaves the 4 alignment bytes after them unsplit. That
  is the real object layout and claims no names: naming them `memPool___...`
  would make a second global definition and redirect relocations. Per unit,
  never as a global symbols.txt pass (the copies differ per unit: tu_80457C08
  has 8). With the split and the proven extab removal (CLAUDE.md, "Measuring
  progress") such a unit reaches 100 % data; without the split it stays at
  ~1 %.

## Shared game headers (`include/game/`)

Every game unit includes these instead of declaring its own copies. Each has
one owner (wave 1: the `hdr/actor` agent); game-file agents never edit them
and list what they need under SHARED-EDITS (offset, width, type, the
function that shows it, and the evidence for any name). The owner applies
reviewed edits on its branch; the coordinator merges them before the units
that need them.

| Header | Holds |
|---|---|
| `game/ActorBase.h` | `ActorBase_804A5770` (named after its vtable), the base class of every spawned actor: 0x230 bytes, 19 virtual slots (dtor, `init` 0x0C, `update` 0x14, `savePrevPos` 0x1C, `draw` 0x20, the rest `vfunc_0xNN`), field layout from its ctor/dtor, the spawner's setters and the manager's loops; the inline setters the spawner uses; `ActorSub120_801F5AE0`, `ActorSub1DC_801EAD68` |
| `game/ActorId.h` | `enum ActorId`: the spawner's 354 case IDs (`ACTOR_RAIL = 'RAIL'`; DMCS, FGAM, ZSLE, ZSLG are empty cases that return -1), English names from docs/names/actors.tsv in comments |
| `game/ActorManager.h` | `ActorManager_804A5800` (0x3060 bytes: actor slots, sound objects, actor list, EnemyParam.csv table), the EnemyParam column keys, and the spawner and manager functions as `extern "C"` declarations (`fn_801F9484`, `fn_801F8544`, `fn_801F620C`, `fn_801F856C`, ...) |
| `game/GameGlobals.h` | The two most-used singletons `lbl_8053AAF8` (`GameRoot_8049EF20`) and `lbl_8053AB10` (`GameRoot2_8049F108`), the fields known so far, their getters `fn_802D6A9C` / `fn_8013C824` (`GameView_802D6A9C`, `EffectSystem_8013C824`), `cls_8030C058` (with its `fn_8030C5F0` rename in symbols.txt) and the per-player queries of tu_8022AD64 |
| `game/GameAudioBase.h` | The base sound header: the JAI includes and `cls_804B21EC` (pool at 0x804B21EC, probably `JAUAudible<4>`, unproven; its memPool_, guard and dtor are renamed in symbols.txt). Gives the 8-pool `__sinit` of tu_80457C08 and tu_8045B6F0 |
| `game/GameAudio.h` | The full sound header for the 526 game units with the 9-pool `__sinit`: GameAudioBase.h plus JAUAudience.h and a FIXME stand-in (`FIXME_instantiateDopplerAudiblePool`, never call or copy it) that instantiates the `JAUDopplerAudible<4>` pool. Replace the stand-in with the game audience class of tu_80457DFC once known. **Include GameAudio.h / GameAudioBase.h before any other JAudio header**: including e.g. JAIStreamMgr.h first reorders the pools |
| `game/RailBase.h` | `cls_800FE3EC` (0xC8 bytes, its method `fn_800FE3EC` in RailBase.cpp), and notes on the RAIL actor class (vtable 0x8049C770) |

Conventions for game code (docs/PLAN.md, "Symbols"):
- Placeholder names: `cls_<addr>` for a class with no known role; a
  provisional descriptive name keeps the address attached (`ActorBase_804A5770`):
  the vtable, else the constructor or first method, said in a comment.
  Evidenced names take no suffix.
- `extern "C"`: only for declarations, in `include/game/` headers, of
  functions another unfinished unit defines, under their dtk names. Never on
  a definition, never in a .cpp. The defining unit renames symbols.txt and
  updates the declaration in the same commit; reviewers check each
  prototype against the asm.
- Vectors: `JGeometry::TVec3f` (mkdd's `JSystem/JGeometry.h`; its
  `psq_l`/`psq_st` copy is the game's) for game data, dolphin's `Vec` only
  where a mangled name or a PSMTX/PSVEC call proves it; boxes are
  `JGeometry::TBox2f`; matrices dolphin's `Mtx`.
- Derived actor classes: `class Foo : public ActorBase_804A5770`,
  constructor `Foo() {}` plus their own members; the spawner's unit holds
  inline copies of many of their constructors.

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
commit). The same run (`tools/readme_assets.py`) writes the README's
visuals into `site/badges/`, the shields.io endpoint badges into
`site/data/badge-*.json`, and the numbers between the README's
`<!-- progress|regions|now:start/end -->` markers. `merge_unit.py` and
`coord.py promote` run it with `--stage`, which stages all of that in their
commit. **After any manual merge or commit to `matching`, run
`python3 tools/site_data.py --subject "<commit subject>" --stage` after the
full `ninja`, in the same commit.**
As a safety net, the Pages workflow runs `tools/readme_refresh.py` on every
push: it redraws the README visuals, badges and README numbers from the
committed `site/data/summary.json` and `docs/roadmap.yml` (no build needed)
and commits them back if they changed. It can't refresh the measurements
themselves; those only change through a local build and `site_data.py`.
It writes only names, addresses, sizes and match flags, never DOL bytes.
Preview: `sbg start fsasite -- python3 -m http.server 8765 -d site`.

The site's quest path comes from `docs/roadmap.yml` (phases, milestones,
what each covers and how its progress is measured); `site_data.py` adds the
live numbers. **When a phase or milestone changes (an import lands, a wave
starts), update its `status` in `docs/roadmap.yml` in the same merge
commit**, and add a `landed:` commit for milestones that finished. Unknown
library names or statuses there stop `site_data.py`, so a typo can't show 0 %.

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

- Provenance: twice an agent submitted qsort.c as "clean-room" when it was
  a lightly reworded copy of PrimeDecomp's unlicensed file (same local and
  macro names, a formatter leftover). Never take a clean-room claim on
  trust: the reviewer compares against the unlicensed donor's same-named
  file, and `tools/gate.py` (check_provenance) flags changed sources whose
  distinctive identifiers mostly come from one. Reviewed false positives go
  in `config/G4SE01/provenance_reviewed.txt`. The first copy reached the
  public history; it was removed with git filter-repo and a force-push of
  `matching` (2026-11-02). Only licensed donors may be read for copying
  (gate.licensed_donors()).

- Common BSS (`.bss` 0x8052E184-0x80539BAC, the JSystem statics): JSystem
  is built with `-common on`, so these sit after all other `.bss`, in link
  order, and each unit owns its range as `.bss ... align:4 common`. The
  common ranges must form one unbroken tail: an unsplit gap stays ordinary
  `.bss` and moves in front of all commons. The GC 2.6 linker inflates the
  first common of each object by the size of all its other commons, even
  ones that resolve elsewhere or are dead-stripped, so symbols.txt holds
  the real (uninflated) size of that first symbol, and the commons that take
  no space here are `stripped` symbols (sEndMesgQueue__7JKRTask, the
  JASTrack/JASChannel memPool_ commons, JAUAudience's unk_common_8052E78C).
  dtk's guess of the inflated size (j3dSys 0x1A4 instead of 0x11C) shifted
  .sdata by 0x88 and broke the SHA-1 in the first J3D import.
  An object's inflation disappears entirely when its first common is unused
  or resolves elsewhere (a weak copy in `.bss`); that is why JAUAudience
  needs the stripped `unk_common_8052E78C` first (tested with standalone
  GC/2.6 links in the infra/bss-names review).

**Measuring honestly**
- Only `build/G4SE01/report_honest.json` (every unit diffed,
  `functionRelocDiffs=all`) counts; progress.py, gate.py and
  coord.py read it. `data_value`, used until 2026-11-02, ignores relocation
  target names: 24 functions counted as matched while calling or loading a
  differently named symbol, among them a swapped pair of TRK statics
  (`IsTRKConnected` / `g_CurrentSequence`) in a Matching, SHA-1-linked unit.
  objdiff's lenient default and byte-pattern hits overstated progress
  several times over.
- Weak-duplicate extab entries (2026-11): ~523 game units construct 9
  JAudio pools in `__sinit`, so each compiled object carries 9 weak
  `JASMemPool<T>` destructors with extab/extabindex entries the original
  link dropped (and a 0x94 run of weak `memPool_` copies in `.bss`).
  objdiff can't see the dedup (objdiff#14, PR #349 open; FFCC and bfbb
  settle for ~99.7 %). `honest_report.py` diffs a copy without an entry
  only on map evidence: WEAK FUNC; `UNREFERENCED DUPLICATE` found in this
  object; both labels `UNUSED` for this object; a link that includes this
  exact object (ninja's, or `link_check.py --unit`'s trial link, which
  records the object's SHA-1) with a DOL matching build.sha1; unique object
  file name among the link inputs. It's honest because it reproduces what
  the original linker provably did with this object, from the linker's own
  log, and nothing else (no code, no data bytes, no kept copy). `gate.py`
  re-verifies every manifest entry (`build/G4SE01/weakdedup_manifest.json`)
  against the map lines, the object's symbol table and the DOL hash, and
  that each stripped copy differs from the compiled object only by those
  entries. A map from a link without the object strips nothing (tested on
  game/tu_80214244 with ninja's map, whose `tu_80214244.o` is dtk's split).
  Before flipping such a unit, the worker runs
  `python3 tools/link_check.py --unit game/<unit>.cpp`, then
  `ninja build/G4SE01/report_honest.json`; after the flip ninja's own map
  is the evidence and the trial is no longer needed.
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
- objdiff's `report generate` used to count a `complete` (Matching) unit as
  100 % without diffing it, so `report.json` built after flipping a flag
  couldn't disprove the flag, and the gate read exactly that report. 14
  units were Matching with 0 % of their code and 12 more broke the link; all
  26 are NonMatching now. Hence `report_honest.json`, written with no unit
  complete. objdiff 3.8.3 does diff a complete unit whose compiled object
  exists (checked 2026-11-02: JUTException at 97.7 % stays 97.7 % when
  marked complete; only `complete_code` changes); it still assumes 100 % for
  a complete unit with no object, and honors `symbol_mappings` from the
  objdiff GUI. The honest report clears both, and the gate fails a Matching
  unit with no source, no split or no compiled object (configure.py would
  link the original object in its place), and any symbol mapping.
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

JASHeapCtrl import (import/jaudio): JASHeapCtrl.cpp does not define the
pools. Its `__sinit` (0x80028CA4) only constructs `JASKernel::audioAramHeap`,
and `JASGenericMemPool`'s own functions (0x80028974-0x80028C0C) sit in it
with no pool statics. The pools are instantiated per TU, guarded by their
`__init__memPool_` bytes, in every TU whose `__sinit` FSA shows them in:
the JAI* units with the three JASTrack/JASChannel pools (plus their own
single-threaded `JASPoolAllocObject<JAI*>` pools), JASAiCtrl,
JASAudioReseter, JASChannel, JASBasicBank, JASBNKParser, JASDrumSet,
JASInstRand, JASInstSense, JASBank and JASVoiceBank with the JASChannel pool
alone. MWCC 2.6 emits a pool for every `JASPoolAllocObject_MultiThreaded<X>`
whose class definition the TU sees, used or not, as long as the generic
`template <typename T> ... ::memPool_;` definition is visible: JASInstSense.cpp
plus `#include "JASChannel.h"` and the generic definition gives the 0x58-byte
`__sinit` at 0x80030B78 byte for byte (relocations aside); without the
JASChannel class it emits nothing. So a generic definition in JASHeapCtrl.h
can fit FSA, but only once every TU's include set is right. As the headers
stand, two Matching units would gain the 0xD4-byte `__sinit`: JASTrackPort.cpp
(it must not see JASTrack.h; tp gives it its own JASTrackPort.h, which the
review confirmed) and JAUStreamFileTable.cpp (through JAIStreamDataMgr.h ->
JAISound.h -> JASTrack.h, so the include fix reaches JAISound.h). The fix is
per-file includes, not per-file specializations; apply the definition only
together with those include fixes, gated by the link.

Other donor quirks to clean up when touching those files (readability
debt, not fakematches by themselves): CARDBlock.c's `goto error`, an unused
`u32 temp[2]` padding local, `card = card =`, and raw `workArea + 0x6000`
casts.

Fix found for one class: inlining fakematches in SDK code (pragmas, dummy
statements) often stand in for release-build asserts, which expand to
nothing but still count toward the inline size limit. Pad.c matched by
restoring dolsdk2004's `ASSERTMSG` lines and dropping its pragma.
