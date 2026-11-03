# Rules card (unit workers and reviewers)

All you need. Don't read PLAN.md, COORDINATOR.md or the briefs; CLAUDE.md
is already loaded.

## Setup
- Your worktree `../fsa-wt/<slug>` on branch `unit/<slug>` was made for you
  (`tools/unit_worktree.sh`). Work only there. Commit to your branch as you
  go, as the repo identity (sh1ftmaker <shiftmaker@gmail.com>), every message
  ending `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`. Never
  commit to `matching`, push, rebase or merge.
- Don't stage `progress/claims.json` (symlink) or `objdiff.json`. dtk
  re-sorts `symbols.txt` on every split: commit only your own lines.
- One build per worktree at a time; wrap builds in `timeout`; anything
  long-lived goes under `sbg`.

## Commands
```sh
timeout 1800 ninja build/G4SE01/report_honest.json  # once at the start
python3 tools/progress.py | head -1                  # baseline: write it in your checkpoint
python3 tools/try.py <unit> [<function>]             # every attempt: compile + diff + classify, ≤20 lines
python3 tools/fdiff.py <unit> <function>             # the full diff, only if try.py's window isn't enough
python3 tools/gate.py                                # before your last commit: must print `gate: ok`
```
`try.py` re-splits when `splits.txt`/`symbols.txt` changed. Its classes:
`regalloc-only`, `scheduling`, `reloc-name` (`@NNN vs lbl_`: literals not
split or not in the original order; fix data, not code), `missing/extra`,
`different`, `missing` (unwritten or misnamed). Only the honest report
counts as progress.

## Per function
Small and leaf functions first. Read the asm (frame, saved registers,
calls, constants), check the donors, write natural C/C++ with real types,
`try.py`, change one thing at a time. Most misses are data and symbols (a
misnamed or unsplit static, a string in the wrong section), not code.

**Stop rule.** When `try.py` prints `STOP: 4 attempts without progress`,
stop. Leave natural code with a `// FIXME: <pct>. <exactly what differs>`
above it (e.g. "Only the clrlwi for index & 7 is scheduled two instructions
earlier than in the original."), put the diff hunk in your checkpoint, move
on. `#ifdef NONMATCHING` bodies are fine for complete, correct functions.

## No tricks (gate.py checks some; review checks all)
A function counts only when natural source produces the bytes. Banned, and
a REJECT/CHANGES in review:
- Escape hatches: inline `asm`, `register ... asm` bindings, `.incbin`,
  hand-typed byte arrays, any new `#pragma` or `goto` the donor file lacks.
  (Original SDK `nofralloc` asm functions in donor files are source; keep
  them, never add new ones.)
- Codegen-forcing: `volatile` or casts to steer codegen; types picked to
  force a sign/zero extension; pointer-cast word copies
  `*(u32*)this = *(u32*)&other` (no exception); field access through casts
  and offsets `*(T*)((u8*)p + 0x10)`; temporaries, dead stores, duplicated
  or reordered statements that exist only to move registers or scheduling.
- Fabrication: fake functions or dummies that drag data in or force order
  (`CW_FORCE_STRINGS`/`CW_FORCE_BSS`, `force_order` dummies, `JASFakeMatch`
  headers); the fix is the real dead-stripped function. Labelled fakematches
  don't count either.
- Measurement: no edits to tools, objdiff/report settings, `build.sha1`,
  `config.yml`, `blobs.txt`, `approved_symbol_changes.txt`, `orig/`, `build/`.
  No per-unit compiler flags: only what the donor/SDK build used, per
  directory like the original build.
- m2c leftovers: `M2C_`, `saved_reg_`, `var_r0`, `sp10`, `unksp`.
A construct that exists only to match needs a one-line comment and a
precedent (donor code or a cited earlier match); otherwise it's a FIXME.

## Donors (`../donors/`, read-only)
Read every donor's version of a library file first and adapt the closest
(FSA's SDK: OS/GX Jul 23 2003, PAD Aug 6 2003, DVD Sep 16 2003, rest Apr 17
2003). Copy only from licensed donors (a LICENSE file: tww, tp, pikmin2,
pikmin, mkdd, sms, oot-gc, bfbb, ac-decomp, lm-decomp, marioparty4-6;
ttyd is MIT: keep its notice; Petari is Wii, reference only). **Banned as a source**: dolsdk2004,
dolsdk2001, melee, kar (no license) and MetroidPrimePort's src/include/
extern/config. Never submit a "clean-room" rewrite of an unlicensed file;
`gate.py` (check_provenance) flags it. Game code has no donor engine.

## Names
Evidence or a placeholder; a wrong name is worse than none. Evidence: a
donor symbol, SDK header, string, `docs/names/actors.tsv`, EnemyParam.csv.
- Unknown: `fn_8xxxxxxx`, `lbl_8xxxxxxx`; unknown class `cls_<vtable addr>`
  (`cls_8049CC34`); free function renamed to what the source emits
  (`fn_80154248` → `fn_80154248__Fiiii`).
- Provisional descriptive names keep the address: `ActorBase_804A5770`,
  `StartMovie_80428630` (vtable, else ctor/first method; a comment says
  which). Members (`init`, `field_0x4`) need no suffix.
- `symbols.txt`: rename or split placeholders only; never change a real
  symbol's address, size or type, never rename a real name.

## What you may change
Your unit's source and headers only it uses; its `splits.txt` stanza (all
its sections: .text, .data, .rodata, .sdata(2), .bss, .sbss, .ctors, extab);
placeholder renames in `symbols.txt`; its own `Matching` flag in
configure.py, only when every function and data section is 100 %. Anything
shared (`include/game/` headers, other units, tools) is a **SHARED-EDIT**:
list offset, width, type, the function that shows it and the name evidence.
Game code: `src/game/<unit>.cpp`; include `game/GameAudio.h` before any other
JAudio header; `extern "C"` only on declarations in `include/game/` headers,
for functions another unfinished unit defines; actors are
`class Foo : public ActorBase_804A5770` with `Foo() {}`; vectors
`JGeometry::TVec3f`.

## Finish and report
`gate: ok`; `progress.py` total not below your baseline; commit. Checkpoint
`progress/<slug>.md` (matched / resistant with hunks / SHARED-EDITS / next).
Report ≤300 words: functions before → after, Matching or not, each
resistant function with its `try.py` class and FIXME, SHARED-EDITS, new
names with evidence, notes for the next agent.

## Reviewers
You don't edit or commit; you judge. Use the main checkout's `tools/gate.py`.
Check: numbers real (`try.py`/`fdiff.py`, gate, progress not below
`matching`); `git diff <merge-base>..HEAD --stat` touches only what's allowed
above; every changed function against its asm for the tricks above; split
ranges neither swallow neighbours nor leave gaps; every new name has
evidence; readability (real types, loops not gotos, FIXMEs say what
differs). Verdict: **ACCEPT** (no blocking findings), **CHANGES** (each
finding with file:line and the fix; a trick or unsupported name is always
CHANGES), **REJECT** (numbers reached by cheating, or other units broken).
