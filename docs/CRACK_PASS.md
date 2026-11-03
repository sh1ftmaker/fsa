# Crack pass

Tricks to revisit at the end; each verified byte-identical with the unit Matching

## game/tu_80222CC4.cpp: `#pragma defer_codegen on` + reversed functions (jump-table .data order)

- Trick: `#pragma defer_codegen on` after the includes, and the six function
  definitions listed in reverse address order (ctor, dtor, init, vfunc_0x90,
  vfunc_0x88, vfunc_0x6C). Marked `FAKEMATCH(crack-pass):` in the source. No
  flag change in configure.py.
- Why needed: the original .data is `__vt__12cls_804A6C38` at 0, vfunc_0x90's
  jump table at 0xA4. The game's `-inline auto` writes a jump table when its
  function is compiled and every class vtable at the end of the TU, so ours was
  jump table, weak `__vt__12J3DFrameCtrl`, vtable. Deferred codegen writes a
  vtable with its first referencing function, which gives the original order;
  it also emits functions in reverse source order, hence the reversed listing.
  Code, .sdata2, extab and relocations are unchanged; only .data moves.
- Honest fix: a project-wide compile mode with deferred-style data order and
  `auto`-style inlining (only callees earlier in .text). No GC compiler
  1.0-3.0a5.2 has one (see GAME_PLAN.md "Measured: compiler versions and inline
  modes"), so the honest fix needs either a version we don't have, or a
  different explanation of the original layout. Until then, game units whose
  only gap is this order can't match honestly.
- Verification (2026-11-03, branch lead/lead-jumptable): unit set Matching,
  `ninja` -> `build/G4SE01/fsa_game_code.dol: OK` (SHA-1); honest report
  tu_80222CC4 7/7 functions, every section 100 % (.data 83.7 % -> 100 %); game
  functions 1745 -> 1746. Set back to NonMatching: `ninja` SHA-1 OK,
  `python3 tools/gate.py --all-units --base origin/matching` -> `gate: ok`.
- Reach: the same trick works on a jump-table unit only if deferred's two-way
  inlining doesn't inline a callee that sits LATER in .text. In the original
  asm, 90 of the 165 jump-table game units have no call to a later function of
  their own unit (safe for this trick); the other 75 do (risky: deferred inlines
  the callee if it is small, e.g. act_shadow_link_bow below).

## game/tu_80221B44.cpp: same trick, .data only (unit stays NonMatching)

- Trick: as tu_80222CC4 (`#pragma defer_codegen on`, functions reversed:
  ctor, init, vfunc_0x90, vfunc_0x9C, vfunc_0x88, dtor), marked
  `FAKEMATCH(crack-pass):`.
- Why: same jump-table / vtable order as tu_80222CC4.
- Honest fix: as tu_80222CC4.
- Verification (2026-11-03): .data 82.0 % -> 97.1 %, vfunc_0x90 99.945 % ->
  99.977 %, no function lost, `ninja` SHA-1 OK, gate ok. The unit stays
  NonMatching: vfunc_0x90 still differs in the stack slots of its position
  locals (progress/game__tu_80221B44.md), so it was not checked Matching.

## Not applied: game/act_shadow_link_bow.cpp

The same trick inlines fn_80222668 (the state change, later in .text) into
vfunc_0x90, which the original calls out of line: vfunc_0x90 and fn_80222668
drop out of the report (game functions -1). Reverted. Would need a second trick
(e.g. `#pragma dont_inline` around fn_80222668) on top; not tried.
