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

**Weak duplicates the link discarded** are the one thing the honest report
leaves out of a compiled object. Every TU that instantiates a weak function
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
`build/` that git tracks. Nothing else is stripped: code, `.bss` (split weak
`memPool_` copies yourself, see docs/COORDINATOR.md) and kept weak copies
are diffed as compiled.

<!-- status:start -->
Status (2026-11-03, honest report, written by `tools/site_data.py`): 4,501 of 11,561 functions, 808,092 of 4,442,088 bytes of code (18.19 %). Game code 80 / 6,789 functions (0.42 %); JSystem 3,368 / 3,702 functions (89.5 %); Dolphin SDK 606 / 614 functions (99.6 %); GX 166 / 172 functions (95.8 %); MSL, Runtime, TRK 281 / 284 functions (90.0 %). 251 of 886 split units are Matching; `ninja` ends in SHA-1 OK.
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
- Never commit game data: `orig/`, `build/`, disc images, extracted assets.
- Archived history: branch `archive/pre-matching-main` has the old m2c /
  port-agent pipeline and TWW game files; `archive/recomp-game-decomp` has the
  recomp-era game-code work.
