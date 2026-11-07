# CLAUDE.md: Four Swords Adventures decompilation

A byte-matching decompilation of *The Legend of Zelda: Four Swords Adventures*
(GameCube, USA, `G4SE01`), in the style of zeldaret/tp and KATAM: C/C++ that the
original Metrowerks compilers turn back into the exact bytes of `main.dol`.
Readable, correctly typed source is part of the goal, not an afterthought.

The earlier static-recompilation, browser and web-streaming work lives outside
this repo in `../recomp/` (fsa-recomp and its worktrees). The live remote-play
host still runs from `../fsa-host/`. Neither is part of this project; don't
build, change or depend on them from here.

## Build (Linux)

```sh
python3 configure.py
ninja build/G4SE01/report_honest.json   # compile every unit and write the (honest) objdiff report
```

`orig/sys/main.dol` is the extracted original (never commit it). Tools (dtk,
objdiff-cli, wibo, compilers) are downloaded into `build/` on the first run.

A plain `ninja` also links every `Matching` unit's compiled object (all other
code comes from dtk's split of the original) into `main.dol` and ends in
`dtk shasum -c config/G4SE01/build.sha1`. That SHA-1 is the final authority:
objdiff can't see code a unit is missing outside its splits, `.bss` sizes, or
symbols the rest of the DOL needs under their `symbols.txt` names. When it
fails, find the unit:

```sh
python3 tools/link_check.py          # which Matching units link byte-identical
```

It relinks by hand under `build/link_check/`, swapping units that break the
link, shift the layout or differ in bytes for dtk's split, until the SHA-1
matches, and lists them with the reason.

## Measuring progress

Only the honest objdiff report counts: `build/G4SE01/report_honest.json`,
written by ninja (`tools/honest_report.py`) with every unit diffed and
`functionRelocDiffs=all`: a relocation matches only if its target has the
same name (or address) and, for data, the same value, so a function that
calls or loads a differently named symbol doesn't count. (`data_value`, used
here before, ignores names entirely; objdiff's default `name_address` skips
the value.) objdiff.json carries the same setting in its `options`, so the
objdiff GUI, `objdiff-cli diff` and `tools/fdiff.py` agree with the report.
The honest report also marks no unit complete and drops objdiff.json
`symbol_mappings`. `report.json` stays for decomp.dev-style tooling: objdiff
3.8.3 diffs a `Matching` unit there too when its compiled object exists, but
counts it as 100 % unseen when it has none, and it honors symbol mappings,
so quote the honest one. `progress.py`, `gate.py` and `coord.py` read it;
the gate fails a `Matching` unit without source or compiled object, and any
symbol mapping.

```sh
ninja build/G4SE01/report_honest.json
python3 tools/progress.py            # totals and per-library breakdown
python3 tools/gate.py --all-units    # Matching units: 100 % honest, and main.dol links byte-identical
```

Never report byte-pattern hits or objdiff's lenient default config as
progress. Both were used here before and overstated it several times over.

**Weak duplicates and dead-stripped data the link discarded** are the only
things the honest report leaves out of a compiled object (four kinds:
weak extab entries, extab entries of dead-stripped weak functions, dead
data, weak data). Every TU that instantiates a weak function
(e.g. the 9 `JASMemPool<T>` destructors of the JAudio pools ~523 game units
construct in `__sinit`) gets its own copy with an extab/extabindex entry; the
linker keeps one copy and drops the others' entries, so the original
object's split lacks them and objdiff could never reach 100 % data although
the link is byte-identical (objdiff#14 / PR #349 are open upstream).
`tools/honest_report.py` diffs a copy without such an entry only when the
entry's function is a WEAK FUNC, and the map (`-map -mapunused`) of a link
that includes this exact compiled object and whose DOL matches `build.sha1`
lists the function as `UNREFERENCED DUPLICATE ... found in <this object>`
and both its extab and extabindex labels as `UNUSED` for this object, whose
file name must be unique among the link inputs. That link is ninja's own
(`build/G4SE01/fsa_game_code.elf.MAP`, Matching units) or, for a NonMatching
unit before its flip, a trial link: `python3 tools/link_check.py --unit
<unit>` links the current Matching set with this unit's compiled object in
place of dtk's split and records the map (and the DOL's SHA-1) in
`build/G4SE01/weakdedup_evidence/` only if the SHA-1 matches; the report
uses that record only for its own unit, against the current main link
inputs, never if git tracks it, and only after a re-link (~0.7 s per
record) gives the same map and SHA-1. That re-link is cached in
`build/G4SE01/weakdedup_verified.json`, a local speed-up trusted like any
build output and forgeable by anyone who can write `build/`: only
`gate.py`, which re-links every trial without reading the cache, proves a
trial removal. Each removal and its map lines go to
`build/G4SE01/weakdedup_manifest.json` (with notes on what wasn't stripped
and why). `gate.py` re-runs every trial link the manifest relies on
(`link_check.py --verify-unit`, ~1 s) and requires the same map and a
SHA-1 OK DOL, re-reads the map lines and the object's symbols, rebuilds the
expected stripped object from the compiled one (symbols, relocations,
extab/extabindex bytes) and fails anything else, and fails any file under
`build/` that git tracks.

A variant of the first: a weak function nobody references (e.g. the inline
`__dt__19SpriteAnim_801D267CFv` every unit with a SpriteAnim member emits)
has no kept copy, so the map lists it `UNUSED`, not as an `UNREFERENCED
DUPLICATE`, and its extab/extabindex entries are dead-stripped with it. The
report leaves such an entry out (manifest kind `unused_weak_extab`) only
when the function is WEAK FUNC with a name unique in the object; the same
kind of verified link's map lists it `UNUSED`, with its size, for this
object in its own section's layout and doesn't place it there; and both its
extab and extabindex labels are `UNUSED` for this object, as above. The
manifest carries those three map lines and the gate re-reads them from the
map and rebuilds the stripped object as for the first kind. The function's
code stays (objdiff doesn't count functions the target lacks), a non-weak
function's entries are never removed (the TU's own dead code still counts),
and data such a function uses goes only by the dead-data or weak-data rules.

Dead-stripped data is the second case: an object keeps the constants, jump
tables and strings of functions the original linker dead-stripped (e.g.
`C_MTXPerspective`'s two `.sdata2` floats, `GXReadGPMetric`'s jump table),
while dtk's split has only what survived. The report leaves such a data
object out only when the map of the same kind of verified link lists it
`UNUSED`, with its size, for this object in its section's layout; every
allocated location that references it (code or data; debug sections don't
count) lies in a function or object the map also lists `UNUSED` for this
object; a size-0 anchor (`...data.0`) through which the section is reached
is itself `UNUSED`, with only `UNUSED` referencers; only zero padding up to
the next symbol goes with it; the map places every kept symbol of the
section at the offset the stripped copy gives it; and the next object in
the map's layout of that section starts 0-7 bytes after the stripped end. Data a live function references is never removed (e.g.
`__vt__12JKRDvdFinder` stays: listed UNUSED, but referenced by a destructor
the map does not list UNUSED for that object).
The manifest lists these as `dead_data` with the map lines of the object,
of each referencer and of each anchor, and the gate recomputes all of it from the object
and the map.

Weak data is the third case: every TU that uses an inline class emits its
weak vtable (e.g. J3DMatBlock.o's `__vt__10J3DPEBlock`, `__vt__11J3DIndBlock`,
`__vt__14J3DTexGenBlock`, `__vt__13J3DColorBlock`), the linker keeps one
copy, and dtk's split of the other objects references it as an external
symbol. The report leaves such a data object out only when it is WEAK
OBJECT in our object with a unique name; the same kind of verified link's
map has, in its link tree, `<name> (object,...) found in <kept>`, then
`>>> UNREFERENCED DUPLICATE <name>` and `>>> (object,weak) found in <this
object>`, every tree node listing it names the same kept object, and that
is another, uniquely named input of the link (not newer than ninja's map);
the map places the kept copy, with this size, in the kept object's layout
of the same section; the kept object's symbol has the same section name,
size, bytes and relocations (offset, type, target name, addend; a local or
section target fails); every allocated relocation that reaches into it or
its padding targets the symbol itself by name (the link resolves those to
the kept copy, so they stay); nothing reaches the section through its
section symbol or a size-0 label at or past the first removed byte (dead
code included); only zero padding goes with it; and the map places the
section's kept symbols and next object as for dead data, with dead and weak
removals of a section checked together. (For dead and weak data alike,
"zero padding" means zero bytes with no relocation in them.) In the stripped copy the symbol
becomes undefined, as in dtk's split. A kept copy may be in a dtk split
(J2DMatBlock.o, an `auto_` data object); it passes only when its relocation
target names equal ours, so a split that still says `fn_...` keeps ours
(the `__vt__10JSUIosBase` copies, kept in `JSUOutputStream.o`). The manifest lists these as `weak_data` with
the three tree lines, the kept copy's placement and the layout lines; the
gate recomputes all of it from both objects and the map. Nothing else is
stripped: dead functions' code (objdiff doesn't count functions the target
lacks), `.bss` (split weak `memPool_` copies yourself, see
docs/COORDINATOR.md) and kept weak copies are diffed as compiled.

<!-- status:start -->
Status (2026-11-07, honest report, written by `tools/site_data.py`): 9,286 of 11,557 functions, 1,963,944 of 4,442,032 bytes of code (44.21 %); weighted 58.68 % (2,606,761 bytes, honest plus partial credit for 863 structurally matched functions). Game code 4,573 / 6,789 functions (31.2 %; Player 59.4 %, Enemies & bosses 27.6 %, NPCs 25.2 %, Objects & gimmicks 30.6 %, Stage & world 14.1 %, Menus & screens 20.9 %, Game core 50.6 %, Media & link 39.3 %); JSystem 3,650 / 3,702 functions (97.9 %); Dolphin SDK 610 / 610 functions (100 %); GX 172 / 172 functions (100 %); MSL, Runtime, TRK 281 / 284 functions (90.0 %). 470 of 903 split units are Matching; `ninja` ends in SHA-1 OK.
<!-- status:end -->

## Layout

| Path | What |
|---|---|
| `config/G4SE01/` | dtk config: `config.yml`, `splits.txt`, `symbols.txt` (real FSA names; dtk rewrites it on every split) |
| `libs/sdk/` | Dolphin SDK sources from doldecomp/mkdd (CC0), built with `cflags_sdk` / `SdkLib()` |
| `libs/sdk_gx/` | GX from zeldaret/oot-gc (CC0), built with `cflags_sdk_gx` / `SdkGxLib()` |
| `libs/jsystem/` | JSystem from doldecomp/mkdd (CC0), compiled with GC/2.6, `MkddJSystemLib()` |
| `src/JSystem/` | JSystem units mkdd doesn't cover (J3D, J2D, ...), derived from zeldaret/tww, `JSystemLib()` |
| `src/PowerPC_EABI_Support/` | MSL / Runtime sources, not yet wired into `configure.py` |
| `include/` | Headers for `src/` |
| `docs/PLAN.md` | How the project proceeds: file map, per-file agents, submission gate |
| `docs/COORDINATOR.md` | Running the agent workflow (`tools/coord.py`) and the rakes hit so far |
| `tools/` | dtk-template's `project.py` plus donor tools (`donor_match.py`, `donor_plan.py`, `donor_symbols.py`) |
| `../donors/` | Read-only checkouts of other decomps; `../donors/DONORS.md` lists them |

Compilers: SDK files use the compiler and flags their donor build used. FSA's
JSystem and the game's own code are **GC/2.6**; the game code is `-O4,p`
C++ with exception tables (`cflags_game`, proven in docs/PLAN.md "Game
code"; its 528 units are `game/*.cpp` in `splits.txt` and configure.py).
FSA's JSystem is `-O4,p`: the original saves 2-4 registers with separate
`stw`, where `-O4,s` emits `stmw`. mkdd's `-O4,s` for JKernel and JUtility
and J2DGraph was wrong for FSA and is gone; JAudio/Task still carries it
until someone checks it the same way.

## Rules for matching work

1. **A unit is `Matching` only when all of it matches**: code, data, and
   relocations. Anything less is `NonMatching` (compiled and reported, never
   linked). `Equivalent` is for code that is functionally identical but can't
   be made to match, linked only with `--non-matching`.
2. **Every file owns its data.** Split each unit's `.data`, `.rodata`,
   `.sdata`, `.sdata2`, `.bss`, `.sbss`, `.sbss2`, `.ctors` and `extab` ranges
   in `splits.txt`, not just `.text`. A function that references a file-static
   variable can't match until that variable is in its unit (objdiff then
   shows it as `Name_8053A3D0` against `Name`).
3. **Matches start at real function starts.** Donor matches are only valid at
   dtk function boundaries (`donor_match.py` enforces this). A short getter
   that matches bytes inside a larger function is not a match.
4. **Extend a donor file's `.text` over functions that don't match yet**, or
   dtk globalizes its statics with `_<addr>` suffixes.
5. **No tricks.** No weakened objdiff config, no hand-edited target objects,
   no `asm` blocks standing in for functions that were written in C, no
   matching-only hacks where a natural construct exists.

6. **Binary blobs are extracted, never written or committed.** Only two
   kinds qualify: non-PowerPC code (DSP or other co-processor microcode) and
   byte-exact copies of an asset file. Each one is listed in
   `config/G4SE01/blobs.txt` (coordinator-only, citing a review) and in
   `config/G4SE01/config.yml`'s `extract:` list (dtk-template's mechanism:
   `symbol`, `header: assets/<path>.inc`, `header_type: raw`). `dtk dol
   split` writes it from the user's own `orig/sys/main.dol` to
   `build/G4SE01/include/assets/`, and the listed unit includes it inside the
   array's initializer:

   ```c
   static u8 lbl_804985D0[0x380] = {
   #include "assets/dolphin/gba/GBAKey_ucode.inc"
   };
   ```

   Never for anything else: PowerPC code, strings, constants, jump tables,
   vtables, structs and tables the code uses are written as C. `.incbin`
   stays banned. Hand-typed bytes are also banned, and **review is the real
   enforcement**: `check_byte_arrays` in `gate.py` is only a tripwire. It
   fails a changed file that gains large integer-literal initializers, bare
   literal lists or escaped string bytes, unless blobs.txt has an `array`
   line for the file. A determined writer can get around it, and a reviewer
   who sees that is looking at a REJECT.

   `check_blobs` checks each blob against symbols.txt and splits.txt (a data
   section, exact address and size, at most 0x2000 bytes unless `gate.py`'s
   `BLOB_MAX_RAISED` records that symbol's size on the owner's approval, inside its unit),
   and against dtk's split of the original (no relocations, no vtable or RTTI
   names). Before checking include paths it normalizes them the way MWCC
   resolves them: continuations spliced, `\` read as `/`, any case. It fails
   on:
   - an `assets/` include that isn't allowlisted, comes from another file or
     sits under `#if`;
   - any non-blob include inside an initializer;
   - an include path naming `build/`, and a computed `#include`;
   - a configure.py include root under `build/` other than
     `build/{version}/include`;
   - a tracked symlink, a tracked file under any directory named `assets`
     (any case), or a tracked file named like an extracted header.

## Code quality (KATAM / TP style)

- Real names and real types. Use the SDK's and JSystem's own names (from
  donors, symbols and debug strings); `fn_8xxxxxxx` / `lbl_` only for things
  nobody has identified yet.
- Structs and classes declared once, in headers that mirror the original
  layout (`include/` paths follow the original library layout where known).
- Write code the way the original programmers did: loops, not gotos; named
  constants and enums for hardware registers and flags; no m2c leftovers
  (`M2C_ERROR`, `saved_reg_*`, raw pointer arithmetic for field access).
- Matching-only constructs (odd temporaries, register pinning, ordering
  tricks a human wouldn't write) get a short comment saying they are there
  to match. If a portable build is added later, guard them like KATAM's
  `PORTABLE`.
- `#ifdef NONMATCHING` bodies are allowed for functions that are complete
  and correct but not yet byte-identical; say what still differs.
- Comments explain why, sparingly. No progress logs in source files.

## Donor workflow

FSA shares its SDK builds with Mario Kart: Double Dash!! (doldecomp/mkdd) and
its JSystem with MKDD (GC/2.6); TWW and TP cover most other JSystem libs.

1. Compile the donor's matched sources with the donor's flags into a scratch dir.
2. `python3 tools/donor_match.py <donor_obj_root> --json m.json`
3. `python3 tools/donor_plan.py m.json --splits s.txt`; merge into `splits.txt`
4. `python3 tools/donor_symbols.py m.json <donor_obj_root> --write`, then
   `rm build/G4SE01/config.json && ninja build/G4SE01/report_honest.json`; repeat 2–4 once.

Then do the data splits (rule 2) by hand or with the donor's own
`splits.txt` for that file, and verify in objdiff.

## Repo hygiene

- Don't push without being asked.
- Never commit game data: `orig/`, `build/`, disc images, extracted assets
  (blobs come from `orig/` at build time, rule 6; `gate.py` checks it, and
  bans tracked `assets/` directories anywhere in the repo).
- Archived history: branch `archive/pre-matching-main` has the old m2c /
  port-agent pipeline and TWW game files; `archive/recomp-game-decomp` has the
  recomp-era game-code work.
