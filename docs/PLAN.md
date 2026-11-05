# Project plan: how FSA gets decompiled

Status: agreed direction (2026-11-02). Based on three research passes: how TP, TWW,
Melee, Petari, pikmin2 and others are run today; how to find FSA's source
files without a map; and what published AI-agent decomp efforts learned.

## Principles

1. **AI-first, no slop.** Most of this decomp will be written by AI agents.
   That is welcome. Submissions that reach a percentage through tricks are
   not: a function counts only when natural source compiles to the original
   bytes.
2. **Mechanical authority.** The objdiff report with relocations checked
   (`functionRelocDiffs=all`: target names and data values) decides what matches; once the DOL links,
   the SHA-1 of `main.dol` decides what is correct. Agents never touch the
   things that measure them.
3. **One file, one owner.** Work is divided by translation unit (the original
   source file). One agent (or person) owns a file until it is done, on its
   own branch and worktree, and grinds every function in it.
4. **Names need evidence.** A wrong name is worse than a placeholder. Every
   new name cites its evidence (debug string, assert, donor decomp, actor
   table, menu text).

## Phase 0: infrastructure (coordinator, before any wave)

- Bump tools to current: objdiff ≥ 3.8.3 (one-shot per-function JSON diff via
  `objdiff-cli diff -o -`, wrong-order detection, exact percentages), dtk
  1.8.4, wibo 1.0.3, binutils (for the permuter).
- Done: the full link under wibo 1.0.3 (1.0.0 failed past ~1,000 objects;
  sections need align:32), and the default `ninja` ends in
  `dtk shasum -c build.sha1`, as TWW does.
- `tools/gate.py` (see "Submission gate").
- CI (GitHub Actions, pinned container like TP/TWW): `configure.py`,
  `ninja` (all units, report_honest.json, the link and the SHA-1 check),
  the gate, a check that every 100 % unit is `Matching` (Melee's
  `check_complete.py`), and `ninja changes` against the base branch.
- `.clang-format` from TP (C++03), applied through pre-commit.
- Progress categories: `game`, `jsystem`, `sdk`, `msl`/`runtime`, `trk`.

## Phase 1: the file map

FSA has no symbol map, no RTTI names and almost no `__FILE__` strings, so
source files are inferred (`tools/tu_map/`, output `config/G4SE01/tu_map.tsv`):

- **Static initializers in `.ctors`**: one per file, the file's last function.
  483 of 527 game files contain the same JAudio pool-allocator initializer, so
  almost every file boundary is marked.
- **Exception tables** (`extab`/`extabindex`) follow file order: a boundary may
  never invert them.
- **`.sdata2` float pools** restart per file: a repeated constant means a new
  file; alignment padding before a float marks a file start.
- **Data order** in every data section follows code order.

Result: the game code (0x800F39EC–0x8045BD00) splits into **530 files**,
94 % of its bytes at high confidence. The method reproduces 118 of 120 known
library boundaries.

Done (branch infra/gamemap): all of it is in `splits.txt` as **528 game
units** (`tools/tu_map/to_splits.py`, list in `config/G4SE01/game_units.tsv`),
and dtk splits them cleanly (no errors, no alignment warnings; the link SHA-1
stays OK):

- Every unit has `.text`, and every section its evidence covers: extab and
  extabindex 528 (from the DOL's extabindex entries), `.ctors` 528 (its
  `__sinit` slot), `.bss` 528, `.sdata2` 459, `.data` 463, `.rodata` 355,
  `.sdata` 63, `.sbss` 61. 100 % of the game's code (3,573,784 bytes, 6,789
  functions) is in game units.
- Data ownership, computed over all data sections at once: a symbol only one
  unit's code references belongs to that unit; then, transitively and across
  sections, a symbol only referenced from data one unit owns (a `.data`
  table's strings in `.rodata`, its floats in `.sdata2`). The actor
  spawner's file holds inline copies of other files' constructors, so its
  references don't count when another unit references the same symbol
  (otherwise 230 small actor files lost their vtable, often their only
  data). A vtable several units reference goes to the referencing unit
  that defines most of its virtual functions (ties: the one where it fits
  the data order; 13 vtables, e.g. tu_8045A640's 0x804B2098). A `.bss`/
  `.sbss` object exactly one unit's `__sinit` references belongs to that
  unit even when other units use it (4 objects; 3 had gone to the
  previous unit: tu_8045B6F0's JAISoundID table 0x8052DEE0,
  act_searchlight 0x804C0EA0, act_shooting_gallery 0x804C6330).
  Units are tiled from the owned data; gaps nobody owns go to the
  earlier unit. Check: only one unit's data still points into another unit's
  `.rodata`/`.sdata2` (act_ganon's table into tu_801902C0, 77 pointers; the
  table itself is never referenced by name), and the tool prints it.
- Thunks: CodeWarrior emits a vtable's this-adjusting thunks (`subi r3,
  r3, N; b fn`) at the end of the file that emits the vtable, after its
  `__sinit`, so tu_map's cut put them at the start of the next unit. Each
  goes to the unit whose `.data` holds its vtable: 0x80457C08 to
  tu_80455CA4, 0x8045B010-0x8045B030 (four) to tu_8045A640, 0x8045B6F0 to
  tu_8045B010. These are the only thunks in the game code. The units keep
  their names (tu_80457C08 now starts at 0x80457C10).
- The tool's "references ... inside another unit" notes cover only
  references it anchors to a single unit (10 of them). Code also reads
  `.sdata2`/`.rodata` constants several units share (about 40 unit pairs,
  e.g. tu_801CB270's constants from 8 units); those aren't listed.
- Corrections the evidence forced on `tu_map.tsv`:
  - 0x80021840–0x80021F24 (linked before JSystem) is two units, split at
    0x80021D60: two `__sinit`s in `.ctors` (0x80021848, 0x80021DA8), and
    dtk's own extab grouping cuts there. Only 0x80021840–0x80021D60 is game
    code (extab for its `__sinit` and JASMemPool dtors, so exceptions on;
    its `.bss` runs to 0x804B2240, the memPools every game `__sinit` builds
    first). 0x80021D60–0x80021F24 is JSystem's
    `JAudio/Interface/JAIAudience.cpp` (mkdd_JAudio), Matching: tp's
    include set (mkdd's file plus `JAISeq.h`, without its `DECL_WEAK`
    block) with `cflags_jsystem_mkdd -O4,p` gives the dtor and the 0x17C
    `__sinit` (6 memPools, `.bss` 0x48) byte for byte; mkdd's file as it
    was gave a 0x144 `__sinit` with 5 pools. No extab in the DOL, `.data`
    0x804916A0 `__vt__11JAIAudience`.
  - 0x80457DFC and 0x8045A338 are one unit: dtk, "Conflicting splits within
    reversed extab group".
  - The THP block 0x80427D2C–0x8042E964 is one unit (tu_map's 0x804280C0 and
    0x8042E780 cuts were placeholders): `.sbss`/`.data` of 0x80427D2C.. sit
    inside the player's, 0x8053AEBD is used on both sides, data order is
    reversed like the rest of this `-inline deferred` file, and it ends at
    its `__sinit` 0x8042E788. It includes sms's THPDec.c/THPAudio.c: all 28
    of their functions match with the game flags plus `-inline off,deferred`.
  - `.text` ends at 0x8045BCE4 (tu_map.tsv's last row says 0x8045BD00).
- Names (271): `game/RailBase.cpp` from its own `"RailBase.cpp"` string;
  270 actor files `game/act_<name>.cpp` from `docs/names/actors.tsv` (high
  and medium names only), placed through the actor spawner `spawnActor_801F9484`,
  whose compare tree is decoded along its branches (an ID counts only from
  `lis`/`addi r0`/`cmpw r29, r0`/`beq`): the case's constructor lies in the
  unit, or, when the constructor is an inline copy in the spawner's file,
  the vtable it stores lies in the unit's `.data` and the unit's own code
  references it (true for all 232 vtable placements). Where actors.tsv
  suggests one stem for several IDs, each gets its ID
  (`act_great_fairy_spfr`, `act_great_fairy_bfry`). 33 more units construct
  several or low-confidence actors; their IDs are in `game_units.tsv` and
  they stay `game/tu_<start>.cpp`, like the remaining 224.

Next, as each file is worked on:
1. Splitting further: 38 medium units (`unverified_single_TU`, one
   `__sinit` but no `.sdata2` proof) may hold several files; a worker who
   finds a file boundary inside its unit proposes the split as a SHARED-EDIT.
2. Rename files as evidence arrives (actor table, resource names, debug
   strings, minigame `mg_*` archives), never by guess. Rename the
   `splits.txt` stanza, the configure.py line and `game_units.tsv` together.
   Don't rerun `to_splits.py --apply` once units have been renamed or
   hand-edited: it would restore its own names and cuts.

## Phase 2: names

Four investigations run in parallel (results in `docs/names/` when done):
debug output (OSReport) logged in Dolphin; assert and crash-debugger strings
(TCRF documents a debug mode and crash debugger for FSA); dormant debug
menus (TCRF's stage select, debug actors STGN and DWAP); C++ metadata and
tables (vtables, the actor table pairing 4-character IDs with Japanese
names, resource tables). Each produces an evidence-backed `names.tsv`.

## Phase 3: per-file agents

### Claiming

- `progress/claims.json`, edited only under `flock`: unit → owner, branch,
  start time.
- The queue comes from the report: donor-backed units first (SDK, JSystem,
  MSL: cheapest), then small game files, then large ones.

### Each agent

- Owns exactly one unit, on branch `unit/<name>` in its own worktree with its
  own scratch dir. Never commits to `matching`, never pushes, never rebases.
- Inputs: the unit's dtk asm, `decompctx.py` context, an m2c draft
  (`ppc-mwcc-c++`) as a draft only, donor code for the same function if any
  exists (TP, TWW, pikmin2, mkdd, Petari, Melee: reference only), an
  offset→field table for big classes, and the names manifest.
- Per function: read the asm (frame size, saved registers first), write
  natural C/C++ with real types, compile with the exact ninja command, check
  that one function with objdiff, change one thing at a time. Small and leaf
  functions first.
- Stop rule (since G0, docs/GAME_PLAN.md item 8): `tools/try.py` counts
  attempts per function and prints STOP when its try budget (3 tries up to
  32 instructions, 6 up to 128, a set budget above) is used without
  progress: record the differing instructions in a FIXME and carry on with
  the rest of the file. Leftovers go to a
  later crack pass (stronger model; the permuter for C units).
- After every function it rewrites `progress/<unit>.md`: matched, resistant
  (with diff hunks), shared edits needed, next targets.
- Shared files (`splits.txt`, `symbols.txt`, `configure.py`, shared headers)
  are not edited by workers; they list the edits they need under
  `SHARED-EDITS` in their report.

### Concurrency

6–8 workers at a time on this machine (each `ninja -j4`), one coordinator,
one serial merge queue, at most one permuter (niced). Everything long-lived
runs under `sbg`.

## Submission gate

A unit is accepted only if all of these hold. Everything above the line is
checked by `tools/gate.py` and CI:

- Every function, its data and its relocations are 100 % in the honest
  report (`report_honest.json`, every unit diffed);
  the unit is `Matching`; all of its sections are split (no `_<addr>`
  statics).
- `ninja changes`: no other unit regressed. `ninja` links main.dol and
  its SHA-1 matches build.sha1 (`tools/link_check.py` names a culprit).
- No `asm` in C/C++ source (functions, blocks or register-asm bindings).
- No `#pragma` that the corresponding donor file doesn't use.
- No `goto` unless the donor has it.
- No m2c leftovers: `M2C_`, `saved_reg`, `unksp`, `var_r`/`var_f`, `sp` +
  hex locals, `MULT_HI`, `?` types.
- No field access through casts and offsets (`*(T*)((u8*)p + 0x10)`).
- No `.incbin` or byte arrays standing in for code.
- No changes to `objdiff.json`, report settings, `tools/progress.py`,
  `tools/project.py`, `tools/gate.py` (hash-checked), `orig/` or any object.
- `symbols.txt`: no address, size or type changes; renames only from
  `fn_`/`lbl_` to an evidenced name.
- `Matching` only for units at 100 %; compiler flags only as the donor/SDK
  build used them.

---

Reviewed adversarially by a separate agent that has not seen the author's
reasoning (`docs/REVIEW_BRIEF.md`; verdict ACCEPT / CHANGES / REJECT), then
merged by the coordinator. CHANGES go back to a unit agent; only ACCEPT merges.
The reviewer checks:

- Names and types: evidence for each new name; placeholders otherwise.
  Structs carry `/* 0xNN */` offsets and a size comment.
- Code reads like the original programmers wrote it: loops not gotos,
  enums and named constants, existing headers reused.
- Any construct that only exists to match has a one-line comment and a
  precedent (donor code or a cited earlier match). A function that can only
  match through a trick stays `NonMatching` with a `// FIXME:` saying what
  differs.

Merging: the coordinator rebases the branch onto `matching`, applies its
shared edits, re-runs configure and the full `ninja` (link + SHA-1), runs the gate and
`ninja changes`, and squash-merges one unit per commit.

## Order of work

1. Phase 0 infrastructure; finish the SDK and JSystem data and donor
   imports (`tools/data_splits.py`), since they are cheapest.
2. Phase 1 file map into `splits.txt`; Phase 2 names in parallel.
3. Waves of per-file agents: SDK/MSL/JSystem units still NonMatching, then
   game files by size, minigames (clean, self-contained) early.
4. Crack passes and a naming/review pass between waves.

## Decisions

- **No fakematches.** A function that only matches through a trick (TP's
  `// fakematch`, Petari's `// FAKEMATCH`, register pins, ordering hacks a
  person wouldn't write) does not count. It stays `NonMatching` with a
  `// FIXME:` saying what differs, until a natural match is found.
- **Claims stay local.** `progress/claims.json` (under `flock`) is the only
  claim list; no per-unit GitHub issues.

## Roadmap after SDK, GX and JSystem (approved 2026-11-02)

The current state of this roadmap (which phase and milestone is done,
under way or next, and what each covers) is kept in `docs/roadmap.yml`,
which drives the quest path on the progress website. Change statuses there.

1. Finish the libraries: MSL/Runtime, MetroTRK (pikmin2/tp v2.6), THP, the
   rest of JSystem including J3D (pikmin2, same GC/2.6), donor-first.
2. Load the file map (`config/G4SE01/tu_map.tsv`) into `splits.txt`: done,
   528 game units with their data ranges (Phase 1 above).
3. Game compiler setup: done, see "Game code" below. Then the shared
   headers, each with one owner, before the broad waves: the actor base class
   (vtable slots known), the enemy/NPC/demo bases, the scene manager, the
   global game-state struct, heap and message systems. Workers propose header
   changes as reviewed SHARED-EDITS.
4. Waves of per-file agents: small self-contained files and minigames, then
   actor families sharing a base class, then the big systems (player, map/UI,
   messages).
5. Naming as its own track (evidence only, a name-only review).
6. Between waves: fakematch debt (the release-build-assert method that fixed
   Pad.c), crack passes on resistant functions, readability cleanup.
7. Done: every unit Matching and `ninja` links main.dol with the original
   SHA-1; a KATAM-style PORTABLE build becomes possible after that.

## Game code

### Compiler (`cflags_game`, `GameLib` in configure.py)

GC/2.6, C++: `-Cpp_exceptions on -O4,p -inline auto -use_lmw_stmw on
-fp_contract on -str reuse,readonly -RTTI off`, default `-sdata 8 -sdata2 8`,
no Shift-JIS literals in the game (plain `mwcc`). Proof: natural C++ for five
game functions matches byte for byte (relocation targets aside) with exactly
these flags, and each of the alternatives breaks one:

| Function | What | Shows |
|---|---|---|
| getActorManager_801F856C | `return gGame->mStage->mActorMgr;` | baseline |
| fn_80154248 | `getActorMgr()->spawn(a, b, c, d);` | `-O4,p`: 4 saved registers as separate `stw` (`-O4,s` emits `stmw r28`) |
| fn_80154310 | `getActorMgr()->create(id, a, b, c, d, e);` | `-use_lmw_stmw on`: `stmw r26` (off: six `stw`) |
| fn_80154528 | midpoint, `0.5f * (a - b) + b` | `-fp_contract on`: `fmadds` (off: `fmuls` + `fadds`) |
| fn_80105DD4 | an empty virtual destructor | `-Cpp_exceptions on`: extab entries, as the DOL has for all three non-leaf functions |

String literals sit unpooled and 4-aligned in `.rodata` (RailBase.cpp's two
strings at +0x0 and +0x10, exactly what `-str reuse,readonly` emits;
`pool` packs them). `.sdata`/`.sbss` hold 8-byte objects and `.data`/`.bss`
nothing smaller than 12. Most units are forward-ordered (`-inline auto`);
the THP unit is `-inline off,deferred` (its `extra_cflags`). `-O3,p` gives
the same code for these five functions; `-O4,p` follows the JSystem build.
`-char signed` and the include order (`libs/sdk/include` first, then
`include`, `src`) follow the JSystem build, whose headers the game uses.
Not settled: whether game files use `-common on` (the common `.bss` at
0x8052E184–0x80539BAC is being sorted out on infra/bss-names).

### Units

- Every game unit is listed in configure.py's `GameLib("game", ...)` block as
  `Object(NonMatching, "game/<name>.cpp")` from the start, source or not
  (progress category `game`). Without `src/game/<name>.cpp` it is reported
  as its target only (0 %), exactly like an unsplit region, so the `game`
  category measures the whole game code. The worker creates the source;
  configure.py only changes when the unit's flag flips (merge_unit.py
  refuses any other configure.py change).
- Names: `game/tu_<text start>.cpp` until there is evidence, then the
  evidenced name (see Phase 1).
- Symbols: game functions are `fn_8xxxxxxx` in `symbols.txt`, but C++
  mangles, so a function only lines up in objdiff once `symbols.txt` has the
  name the source emits. The worker renames the placeholder to that mangled
  name (a placeholder rename, which the gate allows): `spawn__8ActorMgrFiiii`
  when the class and method names have evidence, otherwise a placeholder
  that keeps the address, e.g. a free function `fn_80154248(int, int, int,
  int)` → `fn_80154248__Fiiii`; methods of an unnamed class get a
  placeholder class named after its vtable (`cls_8049CC34`). Say which in
  the unit's report.
- Placeholder names (the user's decision, 2026-11-02): a class with no
  known role is `cls_<addr>`. A provisional descriptive name keeps the
  address attached until a real name is proven: `ActorBase_804A5770`,
  `ActorManager_804A5800`, `GameRoot2_8049F108`. The address is the class's
  vtable; for a class without one, its constructor or first method (or,
  failing that, the first function seen reaching it); a comment says which.
  The same goes for provisional function and global names. Member names
  (`init`, `update`, `draw`, `field_0x4`) need no suffix of their own: the
  class's address suffix covers them, and they change with it. Names ending in
  `_80xxxxxx` count as placeholders for the gate, so they stay renameable.
  Evidenced names (`EnemyParam` from enemyparam.csv, JSystem and SDK types,
  the game's own strings) take no suffix.
- `extern "C"` (approved 2026-11-02): functions defined in *another,
  unfinished* unit are declared once, only in an `include/game/` header, as
  `extern "C"` under their dtk name (`spawnActor_801F9484`), with a comment naming
  the owning unit. The call's bytes and relocation are what the target has
  (the 100 % ~ActorBase_804A5770 proves it). Never in a .cpp, never
  duplicated, and never on a definition: a unit defines its functions only
  as real C++ (a member of its class, or `fn_<addr>` with C++ linkage), and
  its symbols.txt rename and the header's declaration change land in the
  same commit. Methods of a placeholder class that callers already need
  (`Camera_804AA9E4::fn_8030C5F0`) are declared as members only when the
  symbols.txt rename ships with the header. Reviewers check every
  `extern "C"` prototype's argument and return types against the asm, since
  C linkage hides signature errors from the linker and objdiff.
