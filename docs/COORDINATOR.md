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
| `game/GameAudioBase.h` | The base sound header: the JAI includes, JAUAudience.h and a FIXME stand-in (`FIXME_instantiateAudiblePool`, never call or copy it) that instantiates the `JAUAudible<4>` pool at 0x804B21EC (formerly `cls_804B21EC`; JAUAudioMgr.cpp's `__sinit` proves the name: its seven pools match FSA's by name and address). Gives the 8-pool `__sinit` of tu_80457C08 and tu_8045B6F0 |
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

## Binary blobs (`config.yml` `extract:`, `blobs.txt`)

A unit whose data holds a blob with no C source form gets it the way
tp/melee/oot-gc do. GBAKey's 0x380 bytes of GBA key-exchange DSP microcode is
the first. The unit needs two entries:
- a line in `config/G4SE01/blobs.txt`: symbol, section, address, size, unit,
  header, review, justification;
- an `extract:` entry in `config/G4SE01/config.yml`: `symbol`, `header:
  assets/<lib path>/<name>.inc`, `header_type: raw`.

`dtk dol split` writes the raw bytes from `orig/sys/main.dol` into
`build/G4SE01/include/assets/`. The unit includes that header inside the
array initializer. `build/G4SE01/include` is an include root of every cflags
set: dtk-template's `cflags_base` has it second (also for the PCH), and
`cflags_sdk` has it last. A fresh clone builds with `configure.py` + `ninja`,
because the split runs before build.ninja is regenerated and before anything
compiles.

Allowed: non-PowerPC code (DSP or other co-processor microcode), or a
byte-exact copy of an asset file. Each needs its own review and a blobs.txt
line. Anything with a C source form is not allowed: PowerPC code, strings,
constants, jump tables, vtables, structs, tables the code uses. Those are
written as C, even when a C initializer could spell out the same bytes.
blobs.txt and config.yml are protected and coordinator-only, so a worker
asks for an entry in its report.

The gate has two checks. `check_blobs`:
- reads config.yml with `yaml.safe_load` and fails on any other shape or key;
- requires the extract list and blobs.txt to match 1:1;
- checks each blob in symbols.txt (`.data`/`.rodata`/`.sdata`/`.sdata2`
  object, exact address and size, at most 0x2000) and splits.txt (inside the
  unit's range);
- rejects `__vt__`/`__RTTI__`/compiler symbols, and any blob whose range holds
  relocations in dtk's split of the original;
- normalizes include paths as MWCC under wibo resolves them (continuations
  spliced, `\` as `/`, `./` dropped, casefolded);
- lets only the listed unit include the header, exactly once and
  unconditionally;
- fails other `assets/` includes, any non-blob include inside an
  initializer, include paths naming `build/`, computed includes and
  configure.py include roots under `build/` other than
  `build/{version}/include`;
- fails tracked symlinks, tracked files under any directory named `assets`
  (any case, anywhere), and tracked files named like a blob header.

`check_byte_arrays` is a tripwire; review is the real enforcement. On any
changed source-like file (headers and `.inc` too) it fails:
- an edited initializer of more than 64 integer literals or 128 bytes' worth;
- a gain of 256 literals or 512 bytes' worth across the file's initializers;
- a bare literal list over 64 that grew;
- a gain of more than 128 escaped bytes in string literals.

Every number counts, including casts and `+0`; hex words count by their
digits. An `array` line in blobs.txt exempts a file.

Donor files already on `matching` hold large literal arrays, and only an
edit inside one of them, or growth, trips the check. There are about 17 such
files, for example:
- DSP microcode: JAudio `dsptask.c`, `CARDUnlock.c`, `OSAudioSystem.c`;
- lookup tables: `JASCalc.cpp`, `JASDSPInterface.cpp`, `JPAResource.cpp`,
  `JUTDirectPrint.cpp`, `JUTResFont.cpp`, `J2DWindowEx.cpp`, `vi.c`,
  `thp.h`, `rom_tables.h`, MSL `ctype.c`, `e_rem_pio2.c`, `k_rem_pio2.c`.

To list them, run `check_byte_arrays` over every tracked file.

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
  against the map lines, the object's symbol table and the DOL hash,
  re-runs each trial link (`link_check.py --verify-unit`: same map SHA-1,
  DOL SHA-1 OK, same object and inputs), and rebuilds each stripped copy
  independently (symbols, every relocation section, extab/extabindex bytes).
  A map from a link without the object strips nothing (tested on
  game/tu_80214244 with ninja's map, whose `tu_80214244.o` is dtk's split).
  The infra/weakdedup review broke the first version three ways: a hand-made
  evidence file (map of a failed trial link plus ninja's DOL) was accepted;
  evidence could be committed under build/; and the stripped-copy check
  looked only at sizes. Trial evidence is now accepted only as the unit's
  own `<slug>.json`/`.MAP` for the current inputs and re-linked by the gate,
  `build/` is protected, and the copy is compared field by field.
  Before flipping such a unit, the worker runs
  `python3 tools/link_check.py --unit game/<unit>.cpp`, then
  `ninja build/G4SE01/report_honest.json`; after the flip ninja's own map
  is the evidence and the trial is no longer needed. The report re-links
  each trial record before using it, but caches that in
  `build/G4SE01/weakdedup_verified.json`, a local speed-up anyone who can
  write `build/` could forge: only `gate.py` (which re-links without the
  cache) proves a trial removal, so quote trial-based numbers after a gate
  run. Every flip changes the main link inputs, so all recorded trial
  evidence goes stale (the unit's data % drops back, with a note in the
  manifest) until its `--unit` is re-run. Each trial keeps its map, about
  5.4 MB (~2.8 GB if all ~523 pool units had one); delete
  `build/G4SE01/weakdedup_evidence/` entries no longer needed.
- Dead-stripped data (2026-11, infra/stripdata): units whose every
  function matched stayed short in data because the compiled object keeps
  data only dead-stripped functions used: dvdfs.c (8 OSPanic strings of
  DVDSeekAsyncPrio, DVDOpenDir and others), GXPerf.c (GXReadGPMetric's
  jump table), GXPixel.c (5 GXInitFogAdjTable constants), mtx.c and
  mtx44.c (2 constants each), JASChannel.cpp (effectOsc's 0x1C jump
  table; with it gone the other tables line up and its code is 100 % too).
  The `-mapunused` map lists each of them `UNUSED ... <label> <object>` in
  its section layout, next to its functions' `UNUSED` lines in `.text`.
  `honest_report.py` leaves such data out under the same link rules as the
  weak extab entries, only when every allocated reference to it comes from
  a symbol the map lists `UNUSED` for this object, a size-0 anchor it is
  reached through (MWCC's `...data.0`, used with offsets computed in code:
  dvdfs's strings via the dead DVDPrepareStream*) is itself `UNUSED` with
  only `UNUSED` referencers, the map's layout of the kept symbols equals
  the stripped copy's, and the next object in the map's section layout
  starts within 0-7 bytes of the stripped end; the gate redoes every step.
  All six are Matching and linked. mtx.c needed two more things:
  `lbl_8053B300` resized from 8 to 4 in symbols.txt (dtk had swallowed the
  padding before mtx44's data), and mkdd's force-order dummies replaced by
  the SDK's own dead-stripped `C_MTXIdentity` (TP's body; the trial map
  lists it UNUSED, 0x3C), whose 1.0f/0.0f set the constant order.
  Matching units with dead data that objdiff already counted (Padclamp,
  float, JUTAssert) are stripped too; their numbers don't change.
- Weak data (2026-11, infra/weakdata): inline classes' weak vtables are
  emitted by every TU that uses them and kept once by the linker, so a
  unit that emits a copy another object keeps has more `.data` than dtk's
  split. `honest_report.py` removes such a copy (`weak_data` in the
  manifest) only on the verified map's link-tree lines
  (`UNREFERENCED DUPLICATE`, `(object,weak) found in` this object, kept in
  one other link input), when the kept copy has the same bytes and
  relocations by target name, every reference into ours names the symbol,
  only zero padding without relocations goes with it, and the map's layout
  of the section is the stripped one; the gate redoes it from both objects.
  A kept copy in a dtk split passes when its relocation names equal ours
  (J2DMatBlock.o, `auto_07_80498B20_data.o`); one whose targets are still
  `fn_...` doesn't (`__vt__10JSUIosBase`, kept in `auto_07_80498FB8_data.o`).
  On matching (705995b) 12 copies are stripped from 9 Matching units
  already at 100 %, whose numbers don't change: JKRCompArchive and
  JKRDvdFile (`__vt__7JKRFile`), JUTResFont and J2DTextBox
  (`__vt__7JUTFont`), J2DAnmLoader and J2DAnimation (`__vt__10JUTNameTab`;
  J2DAnimation also 3 J2DAnm vtables kept in J2DAnmLoader.o), J2DMaterial,
  J3DModelLoader, J3DMaterialAttach. A trial of the NonMatching
  J3DAnmLoader also strips 3 vtables (kept in the J3DAnimation.o split).
  Not handled: a section left empty (JGadget/binary.o and J3DJoint.o,
  whose .data is only weak vtables: the map has no section line to bound
  it), and JKRFileFinder (the live-referenced `__vt__12JKRDvdFinder` keeps
  the layout from matching the map).
  J3DMatBlock: its four block vtables (`__vt__10J3DPEBlock`,
  `__vt__11J3DIndBlock`, `__vt__14J3DTexGenBlock`, `__vt__13J3DColorBlock`,
  0x1BC, kept in J3DMaterial.o) are proven duplicates, but matching's
  J3DMatBlock trial link fails (code 95.99 %), so nothing is stripped. The
  only SHA-1-OK object we have used tp's word-copy `operator=`, which
  6660f8c rejected as a fakematch; it is a demonstration, not progress. With
  it, .data went from 93.44 % to 99.93 %; the rest was dtk's split putting
  4 bytes of alignment before J3DModelData.o into J3DMatBlock (.data end
  0x8049BA98, `__vt__21J3DColorBlockLightOff` size 0x8C). Ending the split
  at 0x8049BA94 with the vtable at 0x88 gave 100 % (SHA-1 still OK), but
  that symbol-size change needs an `approved_symbol_changes.txt` entry and
  its own review once J3DMatBlock really matches.
- Dead-stripped units (tu_802BA334, tu_800F39EC, tu_80457C08): the unit's
  own functions were all dead-stripped in the original, so only its
  `__sinit` (9 pools, 8 for tu_80457C08), its `.ctors` entry and its `.bss`
  remain. Verified unlinkable from source (wave 6, 2026-11), details below.
  - Why the trial link differs: MWLD drops the compiled object entirely. In
    `link_check.py --unit` the map names the object only in `UNREFERENCED
    DUPLICATE` lines for its weak pools and dtors, never in a section
    layout, and the memory map shrinks by exactly the unit's splits
    (tu_802BA334: extab -0x8, extabindex -0xC, .text -0x224, .ctors -0x4,
    .bss -0x100; tu_800F39EC identical; tu_80457C08 .text -0x1EC, .bss
    -0xE0), shifting everything after it. The compiled object defines only
    locals (`__sinit_*`, `@NNN`) and weak duplicates, so no symbol of it is
    reachable and MWLD ignores the object, `.ctors` included.
  - Why dtk's split survives: not the ldscript's FORCEACTIVE (it does not
    name `lbl_804D27D4` or the split's other symbols), but dtk's `.comment`
    section, which marks every symbol of a split object force-active
    (active flags 0x08 in each symbol's 8-byte entry; MWCC writes 0). The
    map lists those symbols as level-1 roots.
  - Reachability is a pure graph walk: a scratch object with an extra
    unreferenced global function, or global initialized data, is still
    dropped whole; a reference from code that is itself dead-stripped
    (a FORCEACTIVE'd test object whose second function calls ours: that
    function is `UNUSED`) does not pull the object in either. All GC
    linkers 1.0 to 2.6 give the same base SHA-1 and the same trial SHA-1;
    2.7 already breaks the base link. No MWCC version 2.0 to 2.7 marks
    `__sinit` active in `.comment`.
  - FORCEFILES can't model it: `FORCEFILES { tu_802BA334.o }` fails with
    multiply-defined `memPool_` (previously defined in tu_80021840.o,
    JASTrack.o, JASChannel.o). It still fails after weakening the
    competing definitions in scratch copies of the split objects (objcopy
    `--weaken-symbol` on all memPool_/__init__memPool_/JASMemPool dtors in
    tu_80021840.o and auto_10_8053A280_sbss.o, plus clearing their
    `.comment` force-active flag): weak against weak is an error under
    FORCEFILES. FORCEFILES on the already-Matching, SHA-1-linked
    tu_80214244.o fails the same way, so it is FORCEFILES semantics in MWLD
    1.3.2, not dtk's globals; marking the pools `scope:weak` in symbols.txt
    would not help. FORCEACTIVE needs a global; the object's only globals
    are weak duplicates whose name resolves to tu_80021840's copy.
  - So the original object was kept by something the DOL no longer shows
    (most likely its own dead-stripped code reached some way our link
    can't reproduce, or a different link setup). No honest config.yml,
    ldscript or dtk option reproduces it.
  Decision: keep these units NonMatching. They count their code (100 % of
  the `__sinit`) and `.bss` in the honest report; extab/extabindex stay
  short by the weak-duplicate entries, which can't be proven removed
  without a link that includes the object. Don't mark them Matching or
  Equivalent, and don't add FORCEFILES/FORCEACTIVE for them. If more units
  of this type turn up, list them here (check: trial link fails and the
  trial map has no section-layout line for the object).
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
  leave extra data in our object (a jump table), the honest report leaves
  it out on map evidence (see "Dead-stripped data" above); run
  `python3 tools/link_check.py --unit <unit>` before the flip.
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
- `-sym on` is per JAudio directory (2026-11-03, PROVISIONAL: drop if any
  Interface/Utility unit is later found to need -sym on). configure.py
  builds JAudio/System and Task (`mkdd_JAudio`) with it, Interface
  (`mkdd_JAudio_Interface`) and Utility (`mkdd_JAudio_Utility`) without.
  Evidence: the honest report is identical per unit and per function both
  ways, and the Matching set links SHA-1 OK both ways; `link_check.py
  --unit` gives SHA-1 OK for JAISeq, JAISeMgr and JAUSectionHeap only with
  it off (on, `-sym` moves `__dt__18JASCriticalSectionFv` /
  `__ct__16JAISeCategoryMgrFv` / weak copies to the file end, where FSA
  does not have them). System units need it on. Never per-unit flags: a
  directory split mirrors how the original build would have set it.

## Evaluated and rejected: J3D word-copy `operator=` (2026-11-02)

Don't retry this. tp's release build copies `J3DTevOrderInfo`,
`J3DTexCoord`, `J3DIndTexOrder`, `J3DIndTexCoordScale` with
`*(u32*)this = *(u32*)&other` and `J3DGXColorS10::operator=(const
GXColorS10&)` with two `((u32*)this)[i]` copies, labelling most of them
"Fakematch". In FSA this cast is the only form found that matches J3DMatBlock's 15
setters (`lwz r5,0(r5); addi r0,r4,off; stwx r5,r3,r0`), its four TevBlock
`initialize` (j3dDefaultTevColor's second word through a base register) and
J3DMaterialFactory's `modifyPatchedCurrentMtx`. The Infos can't hold a u32
(FSA: `J3DIndBlockFull::mIndTexOrder` at 0x5, `J3DTexCoord` stride 6,
`J3DTevBlock1::mTevOrder` at 0xA). The user briefly allowed the cast on the
strength of tp's ShieldD map (`__as__` 0xC for the four Infos, 0x14 for
`__as__13J3DGXColorS10FRC11_GXColorS10`), but `__memcpy` compiles to the
same sizes at -O0 (0xC byte-identical to the cast, 0x14), and tp's matched
debug build uses `__memcpy` in those base `operator=` (J3DStruct.h:143-145,
224-227, 240-243). The debug map rules out only a field-wise copy (0x34);
the only evidence for the cast is release register order, which is what tp
calls a fakematch. The user reverted to the ban: these types use
`__memcpy`, and the affected functions stay below 100 % with FIXMEs.
Variants already tried (unions, bitfields, u32 members, field-wise,
aligned attributes, pointer temporaries, derived operator=) are listed in
progress/JSystem__J3DGraphBase__J3DMatBlock.md.

## Fakematch debt imported from donors

Donor code came with fakematches: mkdd's `JASFakeMatch*.h` headers (template
instantiations to force weak-function order, included by 37 JAudio files),
mkdd's `unused_force_order_in_data_section*` dummies in `mtx.c` (gone: the
order came from the dead-stripped `C_MTXIdentity`, now restored), and TP/TWW
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
