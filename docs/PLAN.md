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
   (`functionRelocDiffs=data_value`) decides what matches; once the DOL links,
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

Steps:
1. Add the high-confidence files to `splits.txt` in batches (`.text` plus each
   file's data ranges, found with `tools/data_splits.py`-style checks), as
   placeholder units `game/tu_<addr>.cpp`, and verify with dtk's split.
2. Resolve the medium/low regions as their files are worked on (e.g. the THP
   block at 0x80427D2C with oot-gc's THP sources).
3. Rename files as evidence arrives (actor table, resource names, debug
   strings, minigame `mg_*` archives), never by guess.

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
- Stop rule: two measurements without progress, or about 10 minutes on one
  function: record the differing instructions and move on. Leftovers go to a
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

1. Finish the libraries: MSL/Runtime, MetroTRK (pikmin2/tp v2.6), THP, the
   rest of JSystem including J3D (pikmin2, same GC/2.6), donor-first.
2. Load the file map (`config/G4SE01/tu_map.tsv`) into `splits.txt`: 530
   game units with their data ranges, actor files named from
   `docs/names/actors.tsv`, the rest `tu_<addr>`; verify with the SHA-1 link.
3. Game compiler setup (GC/2.6 `-O4,p`, C++ exceptions), proven on the 26
   game functions from the recomp era, rewritten cleanly. Then the shared
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
