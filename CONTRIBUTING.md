# Contributing

This guide covers how to work on the Four Swords Adventures decompilation and
what a submission needs before it can be merged. Contributions from people and
from AI agents are held to the same rules.

## Contents

- [Before you start](#before-you-start)
- [Setup and building](#setup-and-building)
- [Picking and claiming work](#picking-and-claiming-work)
- [Code style](#code-style)
- [Compiler notes](#compiler-notes)
- [Verifying a match](#verifying-a-match)
- [Submitting](#submitting)
- [AI assistance](#ai-assistance)
- [Questions](#questions)

## Before you start

The goal is C and C++ source that the original Metrowerks compilers build
into the exact bytes of `main.dol`, and that reads like code the original
programmers could have written. A function that matches only through a
construct nobody would write is not a match here. It stays `NonMatching`
until someone finds the natural source.

Useful reading, in this order:

1. [`CLAUDE.md`](CLAUDE.md): build, measurement, layout and the matching rules.
2. [`docs/PLAN.md`](docs/PLAN.md): how the work is organised, and the
   submission gate.
3. [`docs/AGENT_BRIEF.md`](docs/AGENT_BRIEF.md): the brief every per-file
   worker follows, including what a branch may change.
4. [`docs/REVIEW_BRIEF.md`](docs/REVIEW_BRIEF.md): what the reviewer checks.

## Setup and building

You need your own legally obtained copy of the game. Nothing from the disc may
be committed.

1. Install Python 3.8+ and
   [ninja](https://github.com/ninja-build/ninja/releases). On Linux, wibo is
   downloaded automatically to run the Windows compilers; on Windows they run
   natively.
2. Fork and clone the repository. The working branch is `matching`.
3. Extract the disc's system data so that `orig/sys/main.dol` exists, for
   example with `dtk disc extract` or Dolphin's *Export Partition Root*.
4. Build:

   ```sh
   python3 configure.py
   ninja
   ```

   This downloads the compilers, dtk, objdiff-cli and wibo into `build/`,
   compiles every unit, writes the honest report
   (`build/G4SE01/report_honest.json`), links `main.dol` and checks its SHA-1
   against `config/G4SE01/build.sha1`. A fresh clone must pass that check.

5. Optionally, open the repository root in
   [objdiff](https://github.com/encounter/objdiff/releases). It reads
   `objdiff.json` and rebuilds as you edit.

## Picking and claiming work

Work is done one source file (unit) at a time, with one owner per file.

**Finding a file.** Look for `NonMatching` units in objdiff, on the
[progress site](https://sh1ftmaker.github.io/fsa/) (the Files table can filter
by status), or with:

```sh
python3 tools/coord.py queue            # unclaimed units, most unmatched code first
python3 tools/coord.py queue --lib libs/jsystem/JSystem/JAudio
```

The [roadmap](README.md#roadmap) shows which areas are being worked on now.
The game's own code is not split into files yet; that is phase 5, so start
with a library unit.

**Claiming.** Claims are kept in `progress/claims.json` by
[`tools/claim.py`](tools/claim.py), which is not tracked in git. To claim a
file from outside, open an issue titled `claim: <unit path>` (for example
`claim: JSystem/JAudio/JASHeapCtrl.cpp`). A maintainer records it, and the
file is yours until you submit or release it. Check open issues and pull
requests first. If a claim has been idle for a long time, ask on the issue
before taking it over.

With a local checkout of the coordinator's setup, the same step is:

```sh
tools/unit_worktree.sh <unit path> <your name>   # claim + worktree + branch unit/<slug>
python3 tools/coord.py start <unit path>         # the same, plus the worker prompt
```

**Scope of a branch.** A unit branch may change:

- the unit's source file, and headers used only by it;
- the unit's stanza in `config/G4SE01/splits.txt`;
- `config/G4SE01/symbols.txt`, only to give placeholder symbols (`fn_`,
  `lbl_`, `*_8xxxxxxx`) real names with evidence, or to resize or split
  placeholders into the real symbols they cover;
- the unit's `Matching`/`NonMatching` flag in `configure.py`.

Shared headers, other units, tools and report settings are out of scope.
List what you need changed in the pull request instead.

## Code style

The standard is the same as zeldaret/tp and KATAM: real names, real types,
and code shaped the way the original was.

**Names**

- Use the SDK's and JSystem's own names, from donor projects, SDK symbols
  and strings in the binary. Keep `fn_8xxxxxxx` and `lbl_8xxxxxxx` for
  anything nobody has identified yet.
- Every new name needs evidence. Cite it in the pull request (donor file and
  line, string, symbol). A wrong name is worse than a placeholder.
- Don't name a function you haven't worked through. A name should come from
  understanding what the code does.

**Types and structure**

- Declare structs and classes once, in headers that mirror the original
  layout. `include/` paths follow the original library layout where it is
  known.
- Access fields through members. Raw pointer arithmetic for field access
  (`*(T*)((u8*)p + 0x10)`) is rejected.
- Use named constants and enums for hardware registers and flags, where the
  donor or SDK has them.

**Control flow and leftovers**

- Write loops rather than `goto`, unless the donor file has the `goto`.
- No m2c leftovers: `M2C_ERROR`, `saved_reg_*`, `var_r*`/`var_f*`, `sp`+hex
  locals, `MULT_HI`, `?` types.
- Comments explain why, sparingly. No progress logs in source files.

**Data**

- Every file owns its data. Split each unit's `.data`, `.rodata`, `.sdata`,
  `.sdata2`, `.bss`, `.sbss`, `.sbss2`, `.ctors` and `extab` ranges in
  `splits.txt`, not just `.text`. A function that references a file-static
  variable can't match until that variable is in its unit.
- When a range moves into a new source file, split `extabindex` with it:
  every `extabindex` entry references its function and must live in the same
  object, or `dtk dol split` aborts with `Bad extabindex relocation`.

**Matching-only constructs**

- If a construct exists only to match (an odd temporary, an ordering a person
  wouldn't choose), it needs a short comment saying so, and the reviewer
  will ask whether a natural form exists.
- `#ifdef NONMATCHING` bodies are fine for functions that are complete and
  correct but not byte-identical yet. Say what still differs.

## Compiler notes

- FSA's JSystem and game code use **GC/2.6** at `-O4,p`. The original saves
  2-4 registers with separate `stw` instructions, where `-O4,s` emits `stmw`.
  The game code is C++ with exception tables.
- SDK, GX, MSL, Runtime and MetroTRK units use the compiler and flags of the
  donor build they came from (see the library helpers in `configure.py`).
  Change flags only as the donor or SDK build used them.
- FSA's Dolphin SDK is the 2003 build: OS and GX from Jul 23 2003, PAD from
  Aug 6 2003, DVD from Sep 16 2003, and the rest from Apr 17 2003. SDK
  versions differ in real code, so start from the donor closest in date.
- After editing `splits.txt` or `symbols.txt`, run
  `rm build/G4SE01/config.json` before rebuilding so dtk re-splits.

## Verifying a match

Only the honest report counts. It diffs every unit with relocations checked
(`functionRelocDiffs=all`), so a function matches only if its calls and data
references point at symbols with the same names (and, for data, the same
values). A placeholder against a real name (`fn_80083E70` against
`JSUTreeIterator<JKRHeap>::operator->`) is a miss until the name is fixed.
`build/G4SE01/report.json` exists for decomp.dev-style tooling; it
counts a `Matching` unit with no compiled object as 100 % unseen and honors
objdiff GUI symbol mappings, so quote the honest one.

```sh
ninja build/G4SE01/report_honest.json   # rebuild changed units, rewrite the report
python3 tools/fdiff.py <unit>           # every function of the unit with its match
python3 tools/fdiff.py <unit> <func>    # the differing instructions, target | ours
python3 tools/progress.py               # totals; must not go down
python3 tools/gate.py                   # the submission gate
```

A unit may be flagged `Matching` only when every function, every data section
and every relocation is at 100 %. `Matching` units are linked into
`main.dol`, and the SHA-1 is the final check. objdiff cannot see code
missing outside a unit's splits, `.bss` sizes, or symbols the rest of the DOL
expects. If the SHA-1 fails, `python3 tools/link_check.py` names the unit.

`tools/gate.py` must print `gate: ok`. It checks:

- no inline asm, register binding, `#pragma` or `goto` the donor file doesn't
  have; no m2c leftovers; no casts-and-offsets field access; no embedded
  bytes;
- no changes to the measuring tools, `objdiff.json`, report settings, tool
  versions or `orig/`;
- `symbols.txt` keeps every real symbol's address, size, type and name;
- no `Matching` unit contains a fakematch;
- every `Matching` unit is 100 % in the honest report, and `main.dol` links
  with the original SHA-1.

## Submitting

1. Work on a branch named after the unit, `unit/<path with / as __>` (for
   example `unit/JSystem__JAudio__JASHeapCtrl`), based on `matching`.
2. Run `python3 tools/gate.py` and `python3 tools/progress.py` before you
   push. The gate must pass, and no other unit may regress.
3. Open a pull request against `matching`. In the description, give:
   - functions matched before and after, and whether the unit is now
     `Matching`;
   - evidence for every new name;
   - functions that still differ, with what differs;
   - anything outside your scope that needs changing.
4. A reviewer who has not seen your reasoning reads every changed function
   against the target assembly and the donor file. It checks the data splits
   and the names, and returns ACCEPT, CHANGES or REJECT
   ([`docs/REVIEW_BRIEF.md`](docs/REVIEW_BRIEF.md)). CHANGES come back to you
   with file and line. Only ACCEPT merges.
5. The maintainer merges with `tools/merge_unit.py`. It squash-merges, merges
   `symbols.txt` per address, relinks, runs the gate again, and regenerates
   the progress site and the README numbers in the same commit. You don't
   need to merge `matching` into your branch, and you shouldn't edit
   `site/data/`, `site/badges/` or the generated README blocks.

Single functions without a full setup are welcome as a
[decomp.me](https://decomp.me/preset/228) scratch (preset *The Legend of
Zelda: Four Swords Adventures (DOL)*). Open an issue with the link, and a
maintainer will bring it into its unit.

## AI assistance

The project is AI-first: most units are matched by AI agents working from
`docs/AGENT_BRIEF.md`, and each is reviewed by a separate agent working from
`docs/REVIEW_BRIEF.md`. AI-written submissions are welcome under the same
conditions as any other. Agents working from a fork should follow
[`AGENTS.md`](AGENTS.md), a tool-agnostic version of the brief.

- **No tricks.** The gate and the review apply unchanged. A match that comes
  from steering the register allocator or the scheduler with constructs
  nobody would write is a fakematch, whoever wrote it.
- **Evidence for names.** An AI may propose a name only with a citation
  (donor code, SDK symbol, string in the binary). Plausible-sounding guesses
  are rejected.
- **Honest reports.** Quote numbers from the honest report and `fdiff.py`,
  never from byte-pattern searches or a lenient diff.
- **Readable pull requests.** Keep descriptions short and factual. Don't paste
  raw model output or automated reviews into issues or pull requests.

## Questions

Ask in [GitHub issues](https://github.com/sh1ftmaker/fsa/issues). For general
GameCube decompilation questions, the documentation of
[decomp-toolkit](https://github.com/encounter/decomp-toolkit) and
[objdiff](https://github.com/encounter/objdiff), and the donor projects listed
in the [README](README.md#repository-layout), are good references.
