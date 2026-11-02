# Brief for per-unit agents

You own one translation unit (source file) of the Four Swords Adventures
decompilation. Your job: make every function, its data and its relocations
byte-identical to the original, with source a Nintendo programmer could have
written. Read `CLAUDE.md` and `docs/PLAN.md` first.

## Your setup

- Your worktree and branch (`unit/<name>`) were created for you with
  `tools/unit_worktree.sh`. Work only there. Commit to your branch as you go
  (`sh1ftmaker <shiftmaker@gmail.com>`, trailer
  `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`). Never commit to
  `matching`, never push, never rebase or merge.
- Build: `ninja build/G4SE01/report_honest.json` (compiles changed units and writes
  the report). Prefix long runs with `timeout`.
- Check: `python3 tools/fdiff.py <unit>` lists every function with its match;
  `python3 tools/fdiff.py <unit> <function>` shows the differing
  instructions (target | ours). 100 % there means matched in the report.
- Your checkpoint: `progress/<unit path, / replaced by __>.md`.

## The loop, per function

1. Pick the next function: smallest and simplest first; leaf functions
   before their callers.
2. Read the target asm (`build/G4SE01/asm/...` and `fdiff.py`): stack frame,
   saved registers, calls, constants.
3. Write natural C/C++ with real types. Look for the same function in other
   decomps first (read-only, in `../donors/`: doldecomp/mkdd, zeldaret/oot-gc,
   projectPiki/pikmin2, zeldaret/tp, zeldaret/tww, doldecomp/dolsdk2001 and
   dolsdk2004, ...). FSA's SDK is the 2003 build: OS/GX Jul 23 2003,
   PAD Aug 6 2003, DVD Sep 16 2003, the rest Apr 17 2003. Different SDK
   versions differ in real code; pick the one closest in date and adapt.
4. Build, run `fdiff.py`, change one thing at a time. Keep what helps.
5. Stop rule: two attempts in a row without progress, or about 10 minutes on
   one function: write the differing instructions into your checkpoint and
   move on. Come back at the end if time remains.
6. After each function, rewrite your checkpoint (matched / resistant with
   diff hunks / SHARED-EDITS / next targets).

**Use the donor projects.** For any shared library (SDK, MSL, Runtime, TRK,
JSystem), read every donor's version of the file before writing anything:
mkdd, oot-gc, pikmin2, tp, tww, sms, melee, Petari, ttyd/lm's dolsdk2004,
dolsdk2001, the Mario Party and Animal Crossing decomps (`../donors/`).
A donor function is a good starting point even if it doesn't match FSA's
build: start from the closest one and adapt it. Functions missing from one
donor's file are often present in another's (tp and dolsdk2004 have more of
the SDK than mkdd).

Lessons from the first wave:
- Most misses in donor-imported files are not code at all: a reference to a
symbol that is named differently or sits in another unit, a string that is
in the wrong section, a static that isn't split into this unit. Fix those in
data and symbols, not by bending code.

## What you may change

- Your unit's source file and any header used only by it.
- Your unit's stanza in `config/G4SE01/splits.txt` (its section ranges).
- `config/G4SE01/symbols.txt`: give placeholder symbols (`fn_`, `lbl_`,
  `*_8xxxxxxx`) their real names when you have evidence. Placeholders may also be resized or split into
  the real symbols they cover. Never change a real symbol's address, size or
  type, and never rename an already-real name (the coordinator handles
  reviewed exceptions in `approved_symbol_changes.txt`).
- `configure.py`: only your unit's `Matching`/`NonMatching` flag, and only
  set `Matching` when `fdiff.py` shows every function and every data section
  at 100 %.
- Anything else (shared headers, other units, tools): don't. List what you
  need under SHARED-EDITS in your checkpoint and final report.

After editing `splits.txt` or `symbols.txt`: `rm build/G4SE01/config.json`
then rebuild, so dtk re-splits.

## No tricks

A function counts only when natural source produces the bytes. These are
rejected by `tools/gate.py` and must never appear: inline asm, register
binding, a `#pragma` or `goto` that the original/donor file doesn't have,
m2c leftovers (`M2C_`, `saved_reg`, `var_r0`, `sp10`), field access through
casts and offsets, embedded bytes, edits to tools or report settings. No
fakematches: if a function only matches through a construct no programmer
would write, leave it unmatched with a `// FIXME:` comment that says what
differs. Wrong names are worse than placeholders: name things only from
evidence (donor code, SDK symbols, strings).

## Before you finish

1. `python3 tools/gate.py` must print `gate: ok`. It builds the honest
   report and links main.dol itself (SHA-1 against build.sha1, never edit
   it); `tools/link_check.py` says which Matching unit breaks the link.
2. `python3 tools/progress.py` total must not be lower than when you started
   (no other unit may regress).
3. Commit. Final report (≤ 300 words): functions matched before → after,
   whether the unit is now `Matching`, resistant functions with their diff,
   SHARED-EDITS, and anything the next agent should know.
4. Release nothing yourself; the coordinator merges and releases claims.
