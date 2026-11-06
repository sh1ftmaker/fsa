# MWCC inlining in FSA's game code: evidence, experiments, model

Research branch `research/inlining`, 2026-11-05 (Opus). Nothing in `configure.py`
or `src/` was changed; every experiment below was built in a scratch worktree and
reverted. Numbers are game functions at 100 % in `report_honest.json` (base at
`matching` 644f5f46: **3980 / 6789**).

## Status: adopted (2026-11-05)

The owner adopted the model after an independent Opus verification
(verdict YES on the switch and on the pragma rule). Branch
`tools/deferred-switch` made game code `-inline auto,deferred`, reversed every
game source, and applied the pragma rule below. Measured on the whole build
against `matching` b24c88f6 (honest report, by address): game functions at
100 % 4003 → 4018, none lost; SHA-1 OK. The rules for workers are in
docs/RULES_CARD.md ("Source order and inlining").

**Pragma rule (owner-approved via the verifier).**
1. `#pragma auto_inline off` immediately before a callee's definition and
   `#pragma auto_inline reset` immediately after it (one region may cover
   adjacent callees), with a one-line comment naming the original caller(s)
   whose asm has `bl <callee>` while our deferred build inlines it.
2. A whole-file `#pragma auto_inline off` (first line after the includes) only
   if the unit's original asm shows no same-unit plain callee inlined anywhere
   (built with inlining off, every written function's same-unit calls equal the
   original's `bl`s); the comment states that check.
3. Never caller-side `dont_inline`, `inline_max_*`, `inline_depth` or
   `defer_codegen`, never per-unit `-inline` flags (the THP unit tu_80427D2C's
   `-inline off,deferred` stays).
4. These are not FAKEMATCH; a reviewer checks each comment against the asm.

**Corrections to the research below** (verifier, then the switch itself):
- tu_80427D2C's "−4" was an artifact: reverse_funcs re-reversed a file already
  in deferred order. Excluded from the reversal, it loses nothing. Likewise
  tu_80222CC4 / tu_80221B44 (their `defer_codegen` crack-pass pragmas are gone;
  they stay at their percentages under the project flag).
- No `@NNN` pass is needed for extab: the 105 "extab drops" came from a broken
  link (act_tutorial_old_man: deferred writes an implicit destructor first; fixed
  with an explicit `virtual ~TutorialOldMan_804AE508() {}` at the top of the
  .cpp). The generator's sinit units with a destructor (52) did renumber their 9
  `.bss` chain objects (`@408..` → `@409..`); gen_game.py --check pins those
  names, so symbols.txt was updated for them.
- The tu_801EE24C gains are not register or scheduling fixes: they come from
  `.sdata2` literal relocations (pool order), i.e. data order.
- "dont_inline is caller-side only" is incomplete: under deferred
  `#pragma dont_inline on/reset` around a callee's definition also works per
  callee, like `auto_inline off/reset`. The rule uses `auto_inline`.
- tu_801D3CBC `.sdata2` 86 → 34 %: 0x8053F0B8-0x8053F0C8 (90.0f, 1.7f and two
  unknown words) are shared constants only other units read. Under deferred,
  file-scope constants always come before the file's function literals, and
  the original has them after tu_801D3CBC's, so they start act_vaati_boss's
  `.sdata2` (split moved; tu_801D3CBC back to 90.9 %).
- tu_80457DFC's volume tables lbl_804B1E60..lbl_804B1ECC: under deferred,
  data defined anywhere in a file is addressed off `...data.0`; the original
  addresses each table on its own (an extern), and the tables lead `.data`
  ahead of the unit's jump tables. They are now defined in the preceding unit,
  tu_80457C08 (whose functions were all dead-stripped), and fn_80458744 and
  fn_80458FF0 reach 100 %. tu_80457DFC needs a region around fn_80458744, not
  the whole-file `off` (fn_80458FF0 inlines fn_80458408).
- tu_802684A4: the verifier's single region over fn_8026EB1C..fn_8026F54C also
  covers fn_8026EE60, which fn_8026F000-F54C inline in the original (they drop
  90 → 14.5 %). The switch uses two regions (fn_8026F54C..F000, and
  fn_8026EC88..EB1C) and leaves fn_8026EE60 inlinable. fn_8026C350 calls
  fn_8026EE60 out of line in the original although its siblings inline it: a
  caller-budget effect (rule 5; a per-file `inline_max_total_size(6000)`
  reproduces the whole unit, banned by rule 3, and the same budget game-wide
  loses tu_803DAC3C fn_803DAC58). Ours stays under the budget and inlines it:
  fn_8026C350 98.53 → 95.33 %, the one function the switch lowered.
- More over-inlined callees than the doc found: fn_801BE86C (act_beamos),
  fn_801158B8 (act_vaati_demo, now also at its own address: the unit is in
  address order), fn_8013C394 (tu_8013AAD4), fn_8037AA7C (tu_80379F84),
  fn_80139958 (tu_80134C04, fn_80135020's small drop). Each has a region.
- tu_8045A640's namespace block was reversed by hand (reverse_funcs doesn't
  reorder inside namespace / extern "C" blocks).

## Summary

The original game code was compiled with **deferred inlining**. Whether a call is
inlined depends on the **callee**, not on where it sits:

1. **Deferred codegen.** The compiler sees the whole file before it generates code,
   so a call can be inlined whichever way it points, earlier or later in `.text`.
   It writes the functions out in reverse source order, so our sources must list
   them in **descending** address order. `#pragma defer_codegen on` or
   `-inline …,deferred` does this.
2. **Which non-`inline` functions can be auto-inlined is set per callee**, by the
   `#pragma auto_inline` state where the callee is **defined**. Measured: under
   deferred, the `auto_inline` state is recorded for each function at its
   definition. Caller-side pragmas (`dont_inline`, `inline_max_total_size`,
   `inline_depth`) are read once, at the end of the file, so they cannot be scoped
   to one function. Most callees are auto-inlinable. A minority (about 11 units so
   far) are not: they are always called out of line, even when they are tiny
   (4 instructions) and even from earlier code.
3. **The default `inline_max_total_size` budget** stops inlining in very large
   callers. fn_80232A0C (0x1194 bytes) is such a caller: it calls the six small
   queries and the later forwarders out of line. The header inlines it cannot
   afford (TVec3 normalize, operator-, TBox2 set) get weak copies placed straight
   after it (fn_80233BA0/BC4/C08/C9C), exactly as MWCC does.

This explains all four symptoms (a)-(d) from one mechanism. Applied to the whole
game (sources reversed, `-inline auto,deferred`, `#pragma auto_inline off` at the
top of 10 units), it scores **3985** functions against 3980 today. It gains 10
functions (vtable/jump-table `.data` order, the tu_801EE24C register near-misses),
and nine units' `.data` and act_deadrock's `.rodata` go to 100 %. The only losses
are the 4 THP functions in tu_80427D2C (see open items). The same build plus one
`auto_inline off` region in tu_802684A4 recovers fn_8026E68C. On tu_8022AD64 it
turns the three recursive leader queries from 99.95 % to 100 % (they now call
fn_8023F264 at the depth limit, as the original does). It also makes the 17
"inline later-defined" functions there writable as plain functions.

Adopting it is an **owner decision**: a project-wide flag change, a mechanical
reversal of every game source, and `#pragma auto_inline off` regions. See
"Decisions needed".

## Evidence table: functions blocked by inlining (2026-11-05)

Symptom types: **L** = inlines a callee that sits LATER in `.text` (impossible
under `-inline auto`). **E** = calls a small EARLIER callee out of line, where our
`-inline auto` inlines it. **R** = recursion or depth-limit call target.
**D** = deferred-only `.data` / weak placement.

| Unit | Function(s) | Type | Callee(s) / detail | Source |
|---|---|---|---|---|
| tu_8022AD64 | fn_8022B334, fn_8022CFE8, fn_8022D534, fn_8022E438, fn_8022EA0C, fn_802316C8, fn_80231A50, fn_80231DE4, fn_80237A80, fn_80238548, fn_8023885C, fn_80239108, fn_8023CEAC, fn_8023D21C, fn_8023D644, fn_8023DAF4 (16, unwritten) | L | fn_8022F514, fn_8023561C, getPlayerPos_8023E724, getPlayerLayer_8023DE58, fn_8023ED8C, fn_8023EA28, fn_80238A68 (35-87 instr) | progress/game__tu_8022AD64.md r11 |
| tu_8022AD64 | fn_80232A0C (unwritten, 52.8 % draft) | E | fn_8022D75C, fn_8022F514, fn_8022F3DC, fn_8022CAF8, fn_8022CCAC, fn_8023188C out of line; header inlines normalize / operator- / TBox2::set out of line too | r12 |
| tu_8022AD64 | fn_8023434C, fn_8023BEA8, fn_8023C36C (99.95), fn_80235B68 (96.5) | R | depth-limit call goes to fn_8023F264 / fn_8018A614 | r11 |
| tu_803099FC | fn_80309A9C, fn_80309B74, fn_80309C4C, fn_80309D90 | L | getPlayerInputSlot_8030AAF4 + D48/D24/D6C; fn_80309F20, fn_80309FCC | progress, src FIXME |
| tu_803944C8 | do_reset, fn_803A2F74 | L | setters fn_803A3070/30A4/30DC/30FC | progress (coordinator) |
| tu_8028FBA0 | fn_80290070 | L | fn_80290D58, fn_80290E90 | progress, PLAYER.md |
| tu_802D39B8 | fn_802D49A8, fn_802D4ADC | L | fn_802D7AC4 | progress |
| tu_80457DFC | fn_80457E54, fn_8045815C, fn_80458880 | L | fn_80458508, fn_80458F04 | progress |
| tu_80457DFC | fn_80457E04 | E/L | fn_80458744 out of line (deferred+auto over-inlines it) | progress |
| tu_8026FC20 | fn_8027057C | L | fn_80270DDC (isDown workaround already used for fn_80270A8C) | progress |
| tu_8023FBF0 | fn_80248FF0 | L | fn_80249070 | progress |
| tu_8023FBF0 | fn_8024FEFC | E | fn_8024FEBC, fn_8024FE78 out of line | progress |
| tu_801F6058 | fn_801F8108 | L | fn_801F7F58 | progress |
| tu_8043A0E8 | fn_804408F0 | E | fn_8043E7CC (16 instr) out of line | progress |
| tu_803E1668 | fn_803E3320 (unwritten) | E | out of line in fn_803E9034 / fn_803ED604 | progress |
| tu_801291A0 | update (80.6 %) | E | fn_8012952C (27 instr) out of line x3 | GAME_PLAN |
| tu_8016A2B4, tu_802F5040, tu_80256C74, tu_8043A0E8 | fn_8016DA6C, fn_802F5570, … | L | listed in CRACK_PASS "open questions" | CRACK_PASS |
| tu_8021A570, tu_802F5040, tu_8016A2B4, tu_8023FBF0 (follows_80240670, fn_802486A0, fn_80248778), tu_8026FC20 (isDown), tu_801EE24C, tu_80457DFC (moveMusicTempo …), set2DDrawInfo, screenMask | (matched) | L, worked around | a file/header inline plus an out-of-line wrapper | RULES_CARD, PLAYER.md |
| tu_80222CC4, tu_80221B44, act_shadow_link_bow/jump, tu_800F7FCC + 160 jump-table units | vfunc_0x90 etc. | D | vtable before jump table | CRACK_PASS, GAME_PLAN |
| tu_801D2C60 | weak dtor first in .text | D | | progress |
| act_vaati_demo | (matched) | E | "defined after draw(), above it MWCC inlines it" | src comment |

Tally from the original asm (all 510 game units; `bl` to a function of the same
unit, `samecalls` below): out-of-line calls to a same-unit callee of ≤ 32
instructions are rare in the EARLIER direction (61 caller/callee pairs). Most
small earlier functions are inlined, so auto-inlining is the norm. In the LATER
direction there are 589 such pairs. Many of those are weak inline copies, and some
are real "never auto-inline" callees (tu_801CDC8C, tu_801286C0).

## Experiments (GC/2.6, game flags)

Tools: `tools/obj_calls.py` (per-function call list of an object, new), and
`tools/reverse_funcs.py` (rewrites a .cpp with its non-inline functions in reverse
order; added from the 2026-11-03 deferred research). Scratch C++ probes are
described inline; each is 5-20 lines.

### 1. Caller budget `inline_max_total_size` (the fn_80232A0C symptom)

GC/2.6 accepts `inline_max_size` and `inline_max_total_size` (`warn_illpragma`
only complains about unknown ones, e.g. `inline_max_auto_size`). The compiler
strings also list `auto_inline`, `inline_bottom_up`, `defer_codegen`,
`dont_inline`, `always_inline`, `inline_depth`.

- Scratch: one caller, k plain statements plus one inline call. The call stops
  being inlined at a budget of about 40 + 12·k. So the caller's own size counts,
  not only the inlined bodies.
- Scratch with mixed callees: inlining is decided per call site. A callee that
  does not fit is called out of line and later, smaller ones are still inlined.
  Explicit `inline` and auto candidates are treated alike.
- Under non-deferred `-inline auto`, `inline_max_total_size` before the caller
  **does** work, contrary to round 12's note ("10..8000 changed nothing"; that
  measured the %, not the calls). On round 12's full fn_80232A0C draft
  (scratchpad `tu_with_a0c.cpp`), the out-of-line calls compared with the
  original's `bl` list were:

  | budget | difference from the original's call list |
  |---|---|
  | ≤ 1800 | everything out of line (isPlayerActive, layerBox, scale, …) |
  | 2500 - 4600 | only `TBox::addPos(f32,f32)` x8 out of line (the original inlines it); the six queries match |
  | ≥ 4610 (default) | the six queries inlined (today's build) |

- A whole-game `-pragma "inline_max_total_size(N)"` (non-deferred) loses
  matched functions: N=1000 -46, 2000 -20, 3000 -16, 4000 -4, 4500 -3, 6000 -1
  (fn_803DAC58 inlines `TColor`'s ctor 96 times and needs more). So a lower
  global budget is **not** the answer under `-inline auto`.
- Under **deferred** (sources reversed), the **default** budget already gives
  fn_80232A0C's pattern. The caller's expanded size now includes the later
  forwarders, so it overflows on its own. The six queries and getPlayerPos /
  getPlayerLayer / fn_8023EA28 … are called out of line. The only differences
  left are `addPos` x8 (out of line in ours, inlined in the original) and
  fn_80233BC4 (inlined here because the draft wrote it as a plain function; in the
  original it is the weak operator- copy). No pragma is needed.

### 2. Where MWCC places out-of-line copies of inline functions

Scratch with an inline H forced out of line (budget, `dont_inline`, or address
taken):

- `-inline auto`: the weak copy goes **right after the first function that calls
  it out of line**, in reverse order of first reference. That is fn_80232A0C
  followed by fn_80233BA0/BC4/C08/C9C in the original.
- `-inline auto,deferred` / `deferred`: also next to the first `.text` referencer.
- An `inline` defined after a plain declaration and a call (`int H(int); … inline
  int H(int) {…}`) under `-inline auto`: the earlier call stays out of line and
  the copy sits at H's source position.

Consequence for (c): fn_8023F264 sits at its own source position between strong
functions (fn_8023F1D4, fn_8023F2BC). It is not right after its first
out-of-line referencer (fn_8022EA0C). So it is **not** a header inline's weak copy.
It is a plain function, and earlier functions inline it. Under `-inline auto` that
is impossible. Under deferred it is the default behaviour.

### 3. Deferred mode: which pragmas are per-function

Scratch (A calls H, K defined later; B after them calls both):

| source | `-inline auto` | `-inline auto,deferred` / `deferred` |
|---|---|---|
| `#pragma dont_inline on` around the caller only | caller calls out of line | **ignored** (state at end of file) |
| `#pragma inline_max_total_size(20)` around the caller only | caller over budget | **ignored** |
| `#pragma auto_inline on` around H's definition, `off` around K's | A: H, K called (both later); B: inlines H | **A and B inline H, call K**; H and K both emitted at their own source positions |

So under deferred only the callee-side `auto_inline` state is per function, and it
works in both directions. (Under deferred, `auto` in `-inline` only sets the
initial `auto_inline` state.)

### 4. tu_801CDC8C probe (the GAME_PLAN "probe 1")

Reversed tu_801CDC8C, `-inline auto,deferred`:

- No pragma: fn_801CF6B8 inlines fn_801CFCF0 and fn_801CF720 (wrong).
- Global `inline_max_size(5..40)`: fn_801CF6B8's calls match. But the same flag on
  the whole game destroys header inlining (N=10 → 2884, 40 → 3267, 100 → 3814
  functions): the draws inline large header inlines. **Rejected**: a callee-size
  threshold cannot be the separator. The original also keeps 4- and 5-instruction
  callees out of line (fn_80128C58, fn_8018330C) while it inlines 39-instruction
  forwarders.
- `#pragma auto_inline off` at the top of the file: calls match, the unit loses
  nothing.

### 5. Whole-game builds (all game .cpp reversed with reverse_funcs.py)

| build | at 100 % | gained | lost |
|---|---|---|---|
| base (`-inline auto`, address order) | 3980 | | |
| `-inline deferred` (no auto) | 3932 | 11 | 59 (callers that need auto-inlined callees: tu_8022AD64 forwarders, tu_8023FBF0, tu_8026FC20, tu_8037DF68, act_stop_npc dtor, …) |
| `-inline auto,deferred` | 3970 | 9 | 19 (over-inlining in 11 units, list below) |
| `auto,deferred` + `#pragma auto_inline off` at file top of tu_801CDC8C, act_shadow_link_bow, tu_801EDBA4, tu_8021AF28, tu_8043A0E8, tu_80457DFC, tu_801286C0, tu_80182914, tu_801CB270, tu_80427D2C | **3985** | 10 | 5 (fn_8026E68C, 4 THP functions in tu_80427D2C) |
| same + `auto_inline off` region around tu_802684A4's fn_8026EB1C only | fn_8026E68C back to 100 % (unit 13 → 14 / 25) | | |

The over-inlined callees under plain `auto,deferred` (orig = out of line):
fn_80222668 (50 instr), fn_801EDC0C (64), fn_8021AFB8 (125), fn_8026EB1C (91),
fn_8043E7CC (16), fn_80458744 (79), fn_80128C58 (4), fn_8018330C (5),
fn_801CF6B8 / fn_801CF720 / fn_801CFCF0 / fn_801CF798 (26-60), fn_801CD930/950.
There is no size pattern. Ten of the fourteen are member functions, but so are
callees the original auto-inlines elsewhere.

Gains under deferred: act_deadrock draw / update; act_shadow_link_bow and
act_shadow_link_jump vfunc_0x90; tu_801EE24C fn_801EE24C, fn_801F26CC, fn_801F2718,
fn_801F2A10, fn_801F3128 (99.5-99.9 % register / scheduling near-misses that become
exact); tu_802EF76C init.

Sections, deferred model vs base (game units):

- `.data`: 13 units better, 9 of them now 100 % (act_force_gem_rain,
  act_link_ending, act_princess_zelda_ending, tu_80182914, tu_801EE24C,
  tu_802EF76C, tu_803DAC3C, tu_80414F6C, tu_804432DC). These are the vtable /
  jump-table order cases.
- `.rodata`: act_deadrock 46 → 100 %.
- `.sdata2`: unchanged except tu_801D3CBC (86 → 34 %) and tu_80427D2C (91 → 77 %).
  This refutes symptom (d) for reversed sources. tu_8026FC20's "deferred pools the
  literals in reverse" came from compiling deferred **without** reversing: with the
  source reversed, the pool comes out in forward first-use order. tu_8026FC20
  scores 97/120 both ways.
- extab / extabindex: 105 units drop below 100 %. The bytes are identical (checked
  act_magic_jar); only the compiler's `@NNN` label numbers change, which
  symbols.txt pins. This is renaming work, not a layout problem.

### 6. Real-function checks on tu_8022AD64 and tu_8026FC20 (deferred, reversed)

- tu_8022AD64: replace the header inline `isPlayerActive` with the plain function
  `fn_8023F264` (its body at its own address, every call site calling
  `fn_8023F264`). Result: 349 → **352**/373. fn_8023434C, fn_8023BEA8 and
  fn_8023C36C reach 100 % (their depth-limit call is now `bl fn_8023F264`), and
  nothing is lost. Patch: scratchpad `inl/tu_8022AD64_deferred.diff`.
- tu_8026FC20: delete the `isDown()` workaround inline, give fn_80270A8C the body
  and call fn_80270A8C everywhere. Still 97/120. The workaround is not needed
  under the model.

## Hypotheses tested

| Hypothesis | Verdict |
|---|---|
| H1. Callees inlined "before their definition" are header / file inlines (current workaround) | Covers (a) mechanically, but **fails (c)**: a header inline's out-of-line copy is placed after its first out-of-line referencer, never at its own address between strong functions, and a wrapper's depth-limit call would target the inline's symbol, not fn_8023F264. Cannot give the `.data` order. |
| H2. Unit is a concatenation of two source files | Not needed, and contradicted for tu_8022AD64: fn_8023434C (early) inlines fn_8023F264 (late) at two levels and calls it at the third. That needs the body in the same TU. Under H1-style headers the copy would sit elsewhere. |
| H3. Per-function `#pragma dont_inline` around callers (b) | Works under `-inline auto`, but is **impossible under deferred** (the state is read at end of file), and the deferred evidence (c), (D) is strong. Under deferred, (b) comes from the callee's `auto_inline` state or the caller's budget. |
| H4. A different global size threshold (`inline_max_size`) | Rejected: kills header inlines (§4); 4-instruction callees stay out of line in the original. |
| H5. A different global `inline_max_total_size` | Rejected under `-inline auto` (§1, -3..-46 functions). Under deferred the **default** budget already reproduces fn_80232A0C. |
| H6. `-inline deferred` (no auto) + explicit `inline` keyword on every inlined callee | Fails (c) and the forwarders' positions: under deferred, explicit-`inline` copies go after their first referencer, and the tu_8022AD64 tail (fn_8023DEDC … fn_8023EFEC) is in clean address order at its own positions. |
| **H7. Deferred codegen + per-callee `auto_inline` (default on) + default budget** | **Explains (a)-(d)**, the `.data` / weak-dtor order, the tu_801EE24C near-misses, and 3985 > 3980 on the first mechanical try. |

Second candidate: H7 with **per-file** `-inline auto,deferred` versus
`-inline deferred` instead of `#pragma auto_inline` (a makefile per module). It is
indistinguishable except where one unit needs both states. tu_802684A4 does
(fn_8026E68C needs some callees auto-inlined but not fn_8026EB1C), and so
probably does tu_8023FBF0 (fn_8024FEFC's callees). So region pragmas, or the
original wrote those callees in a different way, are still needed in a few places.

## Recommended rules (if the owner adopts H7)

1. **Flag**: `cflags_game` `-inline auto` → `-inline auto,deferred`. This is
   project-wide, not per unit. The rules card ban on per-unit flags stays.
2. **Order**: every game .cpp lists its non-inline functions in **descending**
   address order (`python3 tools/reverse_funcs.py src/game/X.cpp`, once per file,
   in one commit). gate/order_check must accept descending order for game units.
   New code is written in that order: the highest address at the top.
3. **Plain functions, no wrappers**: write each function once, at its own address,
   with its real body. Callers inline it in either direction. Drop the
   "file inline + out-of-line wrapper" and "static inline copy" workarounds as
   units are touched: isDown / fn_80270A8C, follows_80240670, playerFlags_802413F0,
   tu_8021A570, tu_802F5040, tu_803944C8 setters, `isPlayerActive` → fn_8023F264.
   Header `inline` stays for real header inlines (JGeometry, accessors used across
   units whose copy is placed after a caller).
4. **Never-inlined callees**: when the original calls a small same-unit function
   out of line from a caller that would otherwise inline it (in either
   direction), put `#pragma auto_inline off` before that callee's definition and
   `#pragma auto_inline reset` after it, with a one-line comment naming the
   caller(s) as evidence. Whole-file `off` when every function of the unit is
   called that way (tu_801CDC8C, tu_801286C0, …). Caller-side `dont_inline` /
   `inline_max_*` pragmas do nothing under deferred; don't use them.
5. **Big callers**: if a big function (≳ 1 KB) calls small callees out of line and
   header inlines get weak copies right after it, that is the default
   `inline_max_total_size` budget. Write the natural calls and add no pragma.
   Match the caller's own source form; extra helper layers change its size.
6. **Header inline copies**: an out-of-line function placed right after the first
   function that calls it (fn_80233BA0 … after fn_80232A0C) is a weak copy of a
   header inline. Don't write it as a plain function.

## Decisions needed (owner)

- Adopt H7: the flag change plus the one-time reversal of all game sources. It
  touches every game .cpp, so do it at a quiet point with no units claimed and
  rebase open branches. Also the order checker and the RULES_CARD / AGENT_BRIEF
  text ("MWCC only inlines what is defined earlier" is wrong for game code).
- Allow `#pragma auto_inline off` / `reset` regions as natural source (Metrowerks
  pragmas, like the `cats off` already in the flags), with the evidence comment
  of rule 4. Alternative: per-module flags, which the rules currently ban.
- CRACK_PASS: the `defer_codegen` + reversal "trick" for tu_80222CC4 /
  tu_80221B44 stops being a trick under H7 (it becomes the project mode). Retire
  those entries once adopted.
- Symbols: deferred renumbers compiler `@NNN` labels (extab, jump tables, sinit
  chains). symbols.txt / blobs pins need a regeneration pass after the switch.

## Open items

- tu_80427D2C (THP, C code in extern "C"): 4 functions and `.sdata2` drop under
  deferred even with `auto_inline off`. Its progress notes already say the
  original order looks like `-inline off,deferred` with the wrappers placed
  after THPAudio. It probably needs its own reversal of the `extern "C"` blocks,
  which `reverse_funcs.py` does not reorder. Check before the switch.
- tu_801D3CBC `.sdata2` 86 → 34 % under deferred: not examined.
- fn_80232A0C's `addPos` x8: under any budget, ours keeps them out of line while
  the original inlines them. The original likely used a smaller addPos form (FSA's
  own JGeometry may differ from libs/sdk's three-level `addPos(f32,f32)`).
- Whether the original wrote `auto_inline` regions or per-module flags is
  unknowable from the binary. Rule 4 picks the form that reproduces it.

## Reproducing

- Calls of one function in our object: `python3 tools/obj_calls.py
  build/G4SE01/src/game/<unit>.o <name>`; the target's are the `bl` lines of
  `build/G4SE01/asm/game/<unit>.s`.
- Whole-game deferred trial (do not commit): in a scratch worktree,
  `python3 tools/reverse_funcs.py src/game/*.cpp`, change `"-inline auto"` in
  `cflags_game` to `"-inline auto,deferred"`, add `#pragma auto_inline off` as line 1
  of the ten units listed in §5, `python3 configure.py && ninja
  build/G4SE01/report_honest.json`, compare the game functions at 100 %. Restore
  with `git checkout configure.py src/game`.
