# Rules card (unit workers and reviewers)

All you need. Workers don't read PLAN.md, COORDINATOR.md or the briefs;
reviewers read this card and REVIEW_BRIEF.md. CLAUDE.md and the global
CLAUDE.md (`sbg`, never kill the shared browser on CDP :9222) are already
loaded. Changes in your worktree you didn't make may be the user's: leave them
and mention them in your report.

## Setup
- Your worktree `../fsa-wt/<slug>` on branch `unit/<slug>` was made for you
  (`tools/unit_worktree.sh`). Work only there. Commit to your branch as you
  go with the repo's identity (don't pass `--author` or change git config),
  every message ending with the `Co-Authored-By: Claude <Model> <noreply@anthropic.com>` line for the model you are (e.g. `Claude Opus 5.5`, `Claude Sonnet 5.5`).
  Never commit to `matching`, push, rebase or merge.
- Never commit game data: `orig/`, `build/`, disc images, DOL/RAM dumps,
  extracted assets (blobs come only through config.yml `extract:`, a
  coordinator decision).
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
`regalloc-only`, `scheduling`, `operands`, `missing/extra`, `different`,
`reloc-name` (objdiff marks a relocation; `@NNN vs lbl_`: literals not split
or not in the original order, a data fix, not code), `missing` (unwritten
or misnamed). Only the honest report counts as progress.

## Working through the file
You own the whole unit; take its functions small and leaf ones first. Read the asm (frame, saved registers,
calls, constants), check the donors, write natural C/C++ with real types,
`try.py`, change one thing at a time. Most misses are data and symbols (a
misnamed or unsplit static, a string in the wrong section), not code.

**Stop rule.** Each function has a try budget: 3 tries up to 32
instructions, 6 up to 128, the coordinator's `--budget` above that (an
attempt = a change to that function's compiled code). When `try.py` prints
STOP for a function, or after about 10 minutes on it without progress,
leave natural code with a `// FIXME: <pct>. <exactly what differs>` (e.g.
"Only the clrlwi for index & 7 is scheduled two instructions earlier than
in the original."), put the hunk in your checkpoint, and carry on with the
rest of the file. `#ifdef NONMATCHING` bodies are fine for complete,
correct functions.

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
  `config.yml`, `blobs.txt`, `approved_symbol_changes.txt`,
  `provenance_reviewed.txt`, `orig/`, `build/`.
- Compiler flags: no per-unit flags to make a function match. The one
  allowed change is correcting a donor-copied `extra_cflags` when the asm
  proves the original's (e.g. `stw` saves show `-O4,p`, not `-O4,s`); the
  reviewer checks it against the asm.
- m2c leftovers: `M2C_`, `saved_reg_`, `var_r0`, `sp10`, `unksp`.
A construct that exists only to match needs a one-line comment and a
precedent (donor code or a cited earlier match); otherwise it's a FIXME.

## Donors (`../donors/`, read-only)
Read every donor's version of a library file first and adapt the closest
(FSA's SDK: OS/GX Jul 23 2003, PAD Aug 6 2003, DVD Sep 16 2003, rest Apr 17
2003). Copy only from licensed donors (a LICENSE file: tww, tp, pikmin2,
pikmin, mkdd, sms, oot-gc, bfbb, ac-decomp, lm-decomp, marioparty4-6;
ttyd is MIT: keep its notice; Petari is Wii, reference only). **Banned as a source**:
dolsdk2004, dolsdk2001, melee, kar (no license) and MetroidPrimePort (all
of it). Never submit a "clean-room" rewrite of an unlicensed file;
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
- Data with no name evidence (tables, file statics) keeps its `lbl_<addr>`
  name, even as a function-local static. Classes/structs are named after
  their vtable, else constructor, else destructor (e.g. the one passed to
  `__register_global_object`), else first function — never a data address.
- A function of another, unwritten unit: grep `include/game/` first; else
  declare it ONCE, `extern "C"`, in the shared header of its owning area
  (GameGlobals.h if none fits) in its own block with an owner-unit comment.
  Never in a .cpp or your unit's act header. If it clearly takes an
  existing class in r3 and sits in that class's unit, make it a member.
  Types come from the callee's body and other callers, not from what makes
  your function match. Squared sums the target leaves unfused (fmuls,
  fmuls, fadds) use `square()` from game/GameMath.h.
- Generator-owned files (units listed in config/G4SE01/gen_game.tsv): to
  change one — yours or another unit's act header — delete ALL of that
  unit's rows from gen_game.tsv in the same commit (whole rows only), keep
  its generated lines unchanged, and list it under SHARED-EDITS.
- `symbols.txt`: rename or split placeholders only; never change a real
  symbol's address, size or type, never rename a real name.

## Known game-code shapes (reviewed; natural source)
- Unfused `fmuls, fmuls, fadds` squared sums: `square(a) + square(b)` (game/GameMath.h).
- An extra unreachable `b` after a switch dispatch: an empty `case N: break;`
  for a state value nothing sets (one-line comment).
- A bool the original materialises (li 1 / li 0, then `cmplwi 1`):
  `bool result = false; if (cond) result = true;` or `x == true`.
- mPos loaded before a TBox2f copy: `JGeometry::TVec2<f32> p(mPos.x, mPos.y);
  TBox2f box(mBox); box.addPos(p);`. Loaded after the copy instead: add
  the corners by hand in the target's order (i.x, i.y, f.x, f.y).
- Point-in-box test with x/y float registers swapped: a file-local inline
  taking the point by reference or pointer,
  `static inline bool contains(const TBox2f& b, const TVec3f& p) { return
  p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y; }` — not
  scalar (x, y) arguments, not `result = true` inside it.
- A u8 converted to float with the signed form (xoris 0x8000): an explicit
  `(s32)` on the u8 operand; plain `u8 * f32` gives the unsigned form.
- Several tables addressed off one base register (lis/addi once, then
  base+offset with lwzx): separate `static const` arrays laid out in
  address order, not a struct of arrays.
- A function-local singleton class: name it after the destructor its getter
  passes to `__register_global_object`; a class with no vtable, ctor or dtor:
  after its lowest-address function.
- `fn_80158B5C` (cls_804A0160, the shadow sprite) calls: use `drawShadow()`
  from game/DrawShadow.h (it loads the receiver last, as the original does).
- New inline FUNCTIONS go in a small header included only where used, never
  in a widely included one (GameGlobals.h, ActorBase.h): every inline adds
  compiler @NNN labels to each includer and renumbers their split symbols.
- A value kept in one register across a call: a local read once
  (`s32 layer = field_0x4;`) — the original read it once too.
- Message calls taking the position BY VALUE (`fn_8038AD24`, `fn_8038BD08`,
  `fn_8038C678`; 338 sites) where the original loads other arguments before
  the psq copy of mPos: a class-local inline helper that takes the position
  as `const JGeometry::TVec3f&` and passes it on by value. Arguments loaded
  BEFORE the copy are the helper's parameters; members loaded AFTER it are
  read inside the helper (e.g. `bool talk(u32 player, u32 msg, s32 layer,
  const TVec3f& pos, u32 flags) { return fn_8038AD24(player, msg,
  field_0x234, layer, pos, flags); }`). Read the split off the asm; don't
  search parameter orders (scratchpad research_byval.md). Layer before
  serial (fn_8022FE80's order) is a reviewed exception: act_dummy_conversation,
  act_merchant.
- Two stack structs in each other's slots: MWCC gives address-taken
  aggregate locals slots in REVERSE declaration order (the later one gets
  the lower sp offset; scope doesn't matter). Declare the one the original
  has higher first, e.g. `JGeometry::TBox2f box;` at the top of the block,
  then `box.set(...)` where it is filled (tu_801EE24C, act_log_monitor_npc
  do this; scratchpad research_stackslots.md).
- `clrlwi` then `neg/or/srwi` on a call result returned as bool: the callee
  returns a byte (`u8`), converted to bool by the caller; declare it `u8`,
  don't write `!= false`.
- `fnmsubs` with the operands as `x - size*0.5`: write `x - size / 2`
  (MWCC turns the divide into the multiply-subtract in the original order).
- A range check compiled as `cmpwi hi; bgt; cmpwi lo; bge body`: an early
  return `if (x > hi || x < lo) return;` (a switch gives `cmpwi hi+1; bge`).
- Positions built on the stack as 8-byte objects (y stored with x, frame
  slots of 8): the callee takes `const JGeometry::TVec2f*`
  (fn_80238548, fn_80226BC4 tile probes); build `TVec2f p(x, y)`, and TVec3f
  callers pass `(const JGeometry::TVec2f*)&mPos` with a one-line comment.
  Check the callee's body reads only +0/+4 first.
- `cmpw` operands swapped against a call result (`cmpw r3, rN` in the
  original): name the result first, `s32 owner = fn_X(...); if (owner ==
  player)`. That is a local read once, not a trick.

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
You don't edit or commit; you judge. Run the main checkout's tools inside
the worktree (`python3 <repo>/tools/try.py <unit>`, `python3
<repo>/tools/gate.py`), never the branch's copies. Check: numbers real
(try.py, gate, progress not below `matching`); `git diff
<merge-base>..HEAD --stat` touches only what's allowed above; every changed
function against its asm for the tricks above; every `extern "C"`
prototype's argument and return types against the asm (C linkage hides
signature errors); structs carry `/* 0xNN */` offsets and a size comment;
split ranges neither swallow neighbours nor leave gaps; every new name has
evidence; readability (real types, loops not gotos, FIXMEs say what
differs). Verdict: **ACCEPT** (no blocking findings), **CHANGES** (each
finding with file:line and the fix; a trick or unsupported name is always
CHANGES), **REJECT** (numbers reached by cheating, or other units broken).
