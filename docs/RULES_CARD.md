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

**Donor-lead preflight.** Before writing a unit or a big function, run
`python3 tools/donor_leads.py game/<unit>.cpp` (under a second). It fingerprints
the unit's asm (string literals, multi-char tags, named callees, distinctive
floats), greps the allowed donors' game source (never the banned ones below) (not their SDK/JSystem
copies) and ranks files by weighted hits. Read the top file when the verdict
is "STRONG LEAD" (a "WEAK LEAD" is a hint, "NO LEAD" means skip) to borrow
structure and idioms: state machine shape, member layout, helper split.
Donor game code is a structural reference only: different game and
compiler, so never paste bodies, and cite the donor file in a comment when
you borrow a structure. Hits in `four-swords-online` are FSA asset-name
evidence, not decomp code. Units whose callees are all anonymous `fn_`
symbols yield little; name more callees first and rerun.

**Big functions: structure first.** A written function helps its file
before it reaches 100 %: it fixes the unit's literal-pool order (constants
are pooled in first-use order), proves field offsets and types, gives
callees their real names, and lets the unit's data be written. So judge a
sub-100 % function by its `try.py` class, not its percentage:
- *structurally done*: `regalloc-only`, `scheduling`, `scheduling+regalloc`,
  with or without `reloc-name`, at any percentage. Every field, call,
  constant and branch is right. Merge it with its FIXME.
- *structurally close*: `missing/extra` of a few instructions, about 85 %
  or more, with a FIXME naming exactly what is missing (an inlined helper,
  a type). Merge it.
- anything else (`different`, or many instructions missing): keep working
  or leave it unwritten; it misleads the headers more than it helps.
In a big unit, write the constructor first (it proves the layout), then the
unit's first functions (they fix the literal order), then the rest, each at
its place in the file's descending address order (next section). Honest progress still counts only 100 % functions;
`python3 tools/progress.py` also shows the structural count. The
no-tricks rules below apply unchanged: never raise a percentage with a
trick, and never write a body twice.

## Source order and inlining (game code, `-inline auto,deferred`)
Game code is compiled with deferred inlining, as the original was (docs/INLINING.md).
- **Order**: a game .cpp lists its functions in DESCENDING address order: the
  highest address at the top, the lowest at the bottom (MWCC emits them in
  reverse source order). Includes, types, file-scope data and `inline`
  helpers stay on top. `python3 tools/order_check.py <unit>` checks the
  compiled order. (The THP unit tu_80427D2C keeps its own `-inline off,deferred`.)
- **Plain functions**: MWCC sees the whole file before it generates code, so a
  caller inlines a small plain function of its file whichever side of it the
  callee sits. Write each function once, at its own address, with its real
  body; never a file `inline` copy plus an out-of-line wrapper for a function
  the original defines in the unit (existing ones are follow-up cleanup).
  Header `inline` stays for real header inlines (their out-of-line copy is
  weak and sits right after its first out-of-line caller).
- **Callee never inlined** (pragma rule 1): when the original calls a same-unit
  function out of line (`bl`) from a caller that our build inlines it into, put
  `#pragma auto_inline off` immediately before the callee's definition and
  `#pragma auto_inline reset` immediately after it (one region may cover
  adjacent callees), with a one-line comment naming the original caller(s),
  e.g. `// Not auto-inlined: fn_801BE6C4 calls fn_801BE86C out of line in the
  original (docs/INLINING.md, pragma rule 1).` `python3 tools/obj_calls.py
  build/G4SE01/src/game/<unit>.o <fn>` lists our calls; the original's are the
  `bl` lines of `build/G4SE01/asm/game/<unit>.s`.
- **Whole file** (rule 2): `#pragma auto_inline off` as the first line after the
  includes only when the original inlines no same-unit plain function anywhere
  (built with inlining off, every written function's same-unit calls equal the
  original's `bl`s); the comment says so and names the callers.
- **Big callers** (rule 5): a large function (≳ 1 KB) that calls small callees
  out of line which other callers inline has hit the inline budget; write the
  natural calls, no pragma.
- **Never** (rule 3): caller-side `dont_inline`, `inline_max_*`, `inline_depth`,
  `defer_codegen`, or a per-unit `-inline` flag. A reviewer checks every
  pragma's comment against the asm; these regions are not FAKEMATCH.
- **Data**: file-scope constants come before every function literal in
  `.sdata2`, and data defined anywhere in the file is addressed off the
  section base (`...data.0`). An object the original addresses on its own
  (lis + lfs/lfsu per object), or a constant after the unit's literals,
  belongs to another unit: fix the split (tu_80457C08 / tu_80457DFC volume
  tables, act_vaati_boss's 90.0f / 1.7f).
- A unit whose dtor the compiler writes implicitly: deferred writes an implicit
  destructor first; if the original has it elsewhere, declare and define it
  (`~Foo() {}`) at its place (act_tutorial_old_man).

## No tricks (gate.py checks some; review checks all)
A function counts only when natural source produces the bytes. Banned, and
a REJECT/CHANGES in review:
- Escape hatches: inline `asm`, `register ... asm` bindings, `.incbin`,
  hand-typed byte arrays, any new `#pragma` or `goto` the donor file lacks
  (the only exception: evidenced `auto_inline` regions, see above).
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
- Weak-function order (`-sym on`, i.e. JSystem/SDK): weak functions are
  grouped per defining file, and the groups follow the order in which each
  file's first function was emitted. `__sinit` and implicit dtors join the
  section of whatever was emitted just before them, usually the file holding
  the last template static's definition. Fix the order by where definitions
  live, never with dummy functions. Game code is `-sym off`: one section, in
  emission order.
- A 4-byte zero gap before an 8-aligned `.rodata` object marks the start of
  the next unit's data (tu_8021C848 / tu_8021CDE4).

## Known game-code shapes (reviewed; natural source)
- Library JGeometry methods (e.g. the paired-single `TVec3f::normalize`) are ordinary calls in game code, not an exception: `cflags_game` searches `libs/sdk/include` before `include`, so game units already get the libs `JGeometry.h` -> `JGeometry/Vec.h` (the game-side `include/JSystem/JGeometry.h` is used only by JUTFader.o). An out-of-line copy of such an inline is a small free function calling the member (e.g. `fn_80233C08`); never write asm in game .cpp.
- Literals before their first visible use: (1) file-scope `static const` aggregates (e.g. GXColor) are emitted before the
  literal pool in declaration order (tu_802F5040, tu_8016A2B4); (2) a helper the original defined early and inlined into its
  callers creates the pool's first literals even though the linker dead-stripped its out-of-line copy: reconstruct it as a
  plain function with exactly the body the callers inline, with a comment saying so (tu_8016A2B4 getMapTexView: +16 near-misses
  flipped). Never invent a helper only to emit constants: its body must be proven by the inlining callers.
- Pane-tag tables: J2D pane tags are u64 ASCII names; write them as multi-char constants (`'Pabb10'`), never hex. A real indexed table
  of them that trips gate.py's check_byte_arrays gets an `array` line in config/G4SE01/blobs.txt once a reviewer confirms it
  (real data the code indexes, original symbol sizes, address order) and writes fsa-wt/_reviews/<unit>.review.md; the owner
  approved this as a standing rule (2026-11-05), widened the same day to ANY real indexed data table a reviewer verifies
  (original symbol size, bytes equal to the original, indexed by the code, no more source-like form such as named constants
  or enums available). Workers still stop and report; the coordinator adds the line. Never split or reshape data to get
  under the tripwire.
- Constant-bound `for` loops count with a plain `int`: an `s32` (signed long) counter makes CW emit a `cmpwi/bge` entry test the original lacks (tu_802F5040 fn_802FF798 87.7 → 99.4 %, tu_80301CD0 fn_80308D0C 93.8 → 100 %).
- Unfused `fmuls, fmuls, fadds` squared sums: `square(a) + square(b)` (game/GameMath.h).
- A two-value test compiled as `subi; clrlwi; cmplwi 1` is `(x == 2 || x == 3)` with literal constants; its negation is `!(x == 2 || x == 3)` (act_eyeball_switch init). `x != 2 && x != 3` and `x < 2 || x > 3` don't fold.
- A position difference that the original routes through an extra stack TVec3f (xy psq_st, z reloaded, then a copy) is `a - b` with JGeometry's `TVec3::operator-` (pikmin2 form), e.g. `from.set(mPos - vel48)` (tu_80206B30 vfunc_0x58). Don't add a redundant named copy instead.
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
- `drawShadow_80158B5C` (ShadowCircle_804A0160, the shadow sprite) calls: use `drawShadow()`
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
- Radians to an s16 angle (`fmuls f0, f1, f0`, the atan2 result first):
  `atan2_(y, x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180())`
  (JMath.h). The constant must come out of an inline call; a literal
  10430.378f gives the operands swapped.
- A by-value position temporary sharing a slot it shouldn't: the original
  copied into a NAMED local (e.g. `TVec3f playerPos;` declared at the top
  of the block, `playerPos = *getPlayerPos_8023E724(player);`), and helpers take it by
  `const TVec3f&` (act_pull_switch).
- Countdown stores through a base register (`addi rX, r31, 8` then
  `stw r0, 0x234(rX)`): the countdowns are an array (`s32 mTimers[3]`)
  stepped in a loop; MWCC unrolls it with that base (act_popo).
- `fabs; frsp; fcmpo` against a float: the SDK's `FABS(x)` macro
  (msl_include/math.h); plain `fabs` compares in double, `__fabsf` has no
  frsp.
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
- Game-mode test on gGameRoot_8053AAF8 0x7F / 0x4C (cmpwi 7, 8, 9, 0x16, 0xA, 0xB
  [, 0xC] in that order, never subi/cmplwi): `isGameModeSet()` (game/GameMode.h,
  `const s32` mode names; literals or enumerators fold into range checks).
  GameRoot2_8049F108::fn_8013B538 / fn_8013B618 (tu_8013AAD4).
- A load off `this` (e.g. `lwz rX, 4(r3)`, field_0x4) scheduled above a
  prologue save (`stw r0`/`stw r31`), above `mr r29, r3`, or a few slots
  earlier than ours: the method is `const`. MWCC hoists loads through a const
  `this` past stores; through a plain `this`, a local, a cast or an inlined
  const accessor it never does. Callees on `this` must be const too (only if
  they just read, e.g. startSound_801F0E34, fn_80219E04); rename the symbols to `...CFv`.
  tu_8020CCA8, tu_801E5FC8, tu_8021AF28 fn_8021B668.
- `getGameView_802D6A9C()->field_0x131df[layer]`: `viewLayerFlag(layer)` (game/ViewLayer.h;
  the view in a local, else the index add swaps). act_magic_jar, tu_802242F4.
- Player bytes 0xC80 / 0xCEE / 0x1215 / 0xCA0 / 0xCE9 tested in a row:
  `playerFlags_802413F0(player)` (game/PlayerFlags.h). Matches only the
  out-of-line copy, Player_804A6F38::fn_802413F0; inlined sites still differ at
  0xCE9 (cmplwi / bne there, neg / or / srwi here; fn_80246F28 94.8 %). Under
  deferred inlining fn_802413F0 can be the plain function in its unit
  (tu_8023FBF0); follow-up.
- UNPROVEN (no 100 % example): sin x/y/z, cos x/y/z (bl sin; frsp) into a
  stack Mtx times a scale: `TVec3f rot(x, y, z); setRotScaleMtx(mtx, rot, scale)`
  (game/RotMatrix.h; scalar 0.0f angles become lfd doubles). tu_801A908C
  fn_801A90D4 and act_hardhat_beetle's draw: 99.0 %, regalloc only.
- frsqrte + three double Newton steps + NaN/fpclassify tail: `std::sqrtf(x)`
  (std/math.h). Not a counted match: act_sword_soldier fn_80154378 is 99.74 %,
  instructions identical, only its .sdata2 relocations differ.
- frsqrte, frsp, one Newton step (`x <= 0` returns x): JGeometry's
  `TUtilf::sqrt` / `inv_sqrt` via `TVec2f::length()`, `TVec3f::normalize()`
  (isZero loads `__float_epsilon`: include libs/sdk's "JSystem/JGeometry.h", not
  a literal epsilon); bare `frsqrte; fmuls` is `JMAFastSqrt`. cls_800F802C::fn_800F802C
  (tu_800F7FCC, 100 %: TVec2f::length / TUtilf::sqrt). normalize / isZero /
  inv_sqrt and JMAFastSqrt only in act_quicksand fn_80210E90 (86.6 %, not a
  counted match).
- A TVec2 stored member by member around calls (stfs x before the second
  getPlayerPos_8023E724 call), then reused for another value: one named `TVec2f v;`
  assigned `v.x = ...; v.y = ...;` (cls_800F802C::fn_800F802C).
- Emitter deleted and cleared (field read again after bl getEffectSystem_8013C824):
  `deleteEmitter(&mEmitter)` (game/EmitterDelete.h: the manager in a local).
  act_eyeball_switch EyeballSwitch_804AE678::vfunc_0x24.
- Plain calls, no wrapper: effect `spawnEmitter_8013CC50(field_0x4, &mPos, id, ...)` and
  sound `startSound_801F0E34(id, 0)` (act_coconut.cpp:29-30); actor lookup / spawn
  `getActorManager_801F856C()->findActorBySerial_801F666C(serial)` (act_invisible_npc_wall_tomi.cpp:22),
  `->spawnActor_801F9484(id, layer, &pos, arg, -1, -1)` (act_leaf_effect_npc_generator.cpp:58);
  random `gGameRoot_8053AAF8->mRandom.get_ufloat_1()` (tu_801C6A4C.cpp:75),
  `(s32)(k * get_ufloat_1())` (act_sand_crab.cpp:49); resources
  `gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', name)` (vtable slot 0x18,
  tu_803536C4.cpp:45).
- J2D pane lookup through vtable slot 0x3C with a two-word tag (r5/r6):
  `mScreen->search(0x7072737374727431ull /* "prsstrt1" */)` (tu_803DA6B4.cpp:45).
- A compare materialised as a byte and tested (`subfic rX, rX, N; cntlzw; extrwi. rX, rX, 8, 19; bne`):
  an inline returning `u8` with `return x == N ? true : false;`, called in the condition
  (`static inline u8 isGameMode(s32 mode) { return gGameRoot_8053AAF8->field_0x4c == mode ? true : false; }`).
  A plain `x == N`, a bool return or `return x == N;` fold into `cmpwi; bne`. tu_80285E58 fn_80286688
  88.7 -> 100 % and the copy inlined in fn_802870C8; tu_8026FC20 isGameMode7 (fn_8027057C). Same shape:
  tu_802F5040 fn_802FB69C, tu_80301CD0, tu_8013AAD4 fn_8013BBE0 (mode 0xC).
- Early-out tests ending `bne +8; b end` (the last test jumps over a `b` to the epilogue) instead of
  `beq end`: the tests are ONE condition, `if (a || b || !c) return;`, not separate `if`s
  (tu_80285E58 fn_80286688; separate `if`s or `if (!c) return;` give `beq`).
- A member loaded into a saved register BEFORE the calls of the same expression (`lwz r29, 0x3b4(r31)`
  ahead of the bl's for the other arguments; `lwz r29, 0x4(r31); bl fn_X; cmpw r3, r29`): the original
  read it through an inlined getter (MWCC orders an inlined call with the other calls; a plain member
  read comes after them). Player_804A6F38::getIndex() / getLayer() (Player.h), used for every read in
  tu_80285E58 (8 sites flipped, nothing else changed).
- Two calls compared, `cmpw r3, r29` with the FIRST call's result in r29: write the call made second
  on the left, `if (fn_80240888() != fn_80248778())` (MWCC calls the right operand first); a local for
  the first result gives `cmpw r29, r3` (tu_80285E58).
- A `||` chain on one member that the original folds only partly (`cmpwi 8; beq; subi 9; cmplwi 1; ble;
  cmpwi 0x39`): compare the member itself each time (`field_0x1240 == 8 || field_0x1240 == 9 || ...`); a
  local read once lets MWCC fold 8-10 (tu_80285E58 fn_802870C8).

- A u8 global or field tested with `lbz; cmpwi 0` (plain u8 gives cmplwi): `(s32)flag != 0` (reviewed
  form, tu_8025EB18 fn_80263078's cucco 0x2A1 `== 1`; tu_80285E58 lbl_8053ABA8 / lbl_8053ABF0).
- A vector copied on through the first copy's address (`addi r4, rB, A; addi r3, rB, B`, store to r4,
  reload from r4, store to r3): a chained assignment `b = a = v;` (tu_80285E58 0xD1C / 0xCBC); two
  statements `a = v; b = a;` swap r3 / r4.

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
