# Crack pass

Tricks to revisit at the end; each verified byte-identical with the unit Matching.

Every trick line in the source carries a `FAKEMATCH(crack-pass):` comment. `tools/gate.py`
rejects a Matching unit containing "fakematch", so these units stay NonMatching until the
trick is replaced by an honest fix (or the project decides to accept it).

## JSystem/JAudio/System/JASTrack.cpp (unblocks game/tu_80021840.cpp)

- **Trick:** a dummy function template `JASTrack_orderDummy<T>` at the end of
  `libs/sdk/include/JSystem/JAudio/System/JASTrack.h`, compiled only when
  `JASTRACK_ORDER_DUMMY` is defined (JASTrack.cpp defines it before its first include, so
  no other unit sees it and their `@NNN` labels don't shift), and an unused
  `static void JASTrack_orderDummyUse()` at the end of JASTrack.cpp that calls it. Both are
  unreferenced, so the linker dead-strips them (map: `UNUSED ... JASTrack_orderDummyUse__Fv`,
  `UNUSED ... JASTrack_orderDummy<i>__Fi`).
- **Why:** with `-sym on` MWCC puts weak functions in one `.text` section per defining file,
  ordered by first emission; `__sinit` and implicit dtors join the section of the last thing
  the end-of-TU template pass instantiated. Without the trick that is the memPool_
  definition in JASHeapCtrl.h, so `__sinit` lands after `free`/`__dl` (every `bl` into them
  shifts by 0x18C and the link breaks). A function template from JASTrack.h, instantiated
  after the template statics, moves `__sinit` into JASTrack.h's section right after
  `MoveParam_` ct, which is FSA's order.
- **Honest fix would need:** evidence of how FSA's headers really placed this: e.g. a real
  function template or weak inline in FSA's JASTrack.h that JASTrack.cpp instantiates and an
  earlier-linked unit also emits (searched: no JASTrack.h function exists out of line
  outside JASTrack.o in FSA, no bl from JASTrack to one, nothing in mkdd/TP/SMG JASTrack.h),
  or a different home for the `JASPoolAllocObject_MultiThreaded<T>::memPool_` definition that
  every pool user still sees (see progress/JSystem__JAudio__System__JASTrack.md).
- **Verification (2026-11-03):** JASTrack.cpp Matching: `ninja` + `sha1sum -c
  config/G4SE01/build.sha1` OK (d86b3d9f785566e34264035e140ef3daf3cc19f0). JASTrack.cpp and
  game/tu_80021840.cpp both Matching: OK, same SHA-1; honest report 100 % for both units;
  `tools/gen_game.py --check` clean. Set back to NonMatching with the trick in the source:
  `python3 tools/gate.py --all-units --base origin/matching` prints `gate: ok`.

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

## Open questions to revisit once more code is written (2026-11-05)

Not tricks: patterns no honest form explains yet. Revisit after the
surrounding code exists, since finishing the rest of a unit may show the cause.

- **Literals pooled before their first visible use.** tu_8016A2B4 (ten GXColor
  objects then 0.125/1/0.5/0 ahead of fn_8016A7DC's literals), tu_801213EC (8.0f
  before 0.0f, 10430.378f before -24f), tu_803BC6F0 (0, -200, 200 first, used
  only by later functions) and tu_8027E5BC (1.5f, 3.0f). Literals are created per
  function in compile order, and an inline helper's aggregate initializers when
  the inline is first read. Candidates: early inline helpers, dead code the
  optimizer removed, or a deferred/reordered codegen mode. Each unit's
  checkpoint has the pool layout.
- **Inlining of later-defined functions.** tu_803099FC (fn_80309A9C/B74/C4C,
  fn_80309D90), tu_8043A0E8 (fn_804408F0), tu_80256C74, tu_8016A2B4
  (fn_8016DA6C), tu_802F5040 (fn_802F5570) and tu_803944C8 (do_reset,
  fn_803A2F74): the original inlines a function defined after the caller,
  which `-inline auto` can't. Same root as the tu_80222CC4 defer_codegen
  finding above; decide them together.
- **Weak destructor emitted first.** tu_8045A640: the original's .text starts
  with the weak __dt__Q29GameAudio16CustomSoundTableFv; ours emits it after
  getBgmSeqResourceID, so the unit can't link although every function matches.
- **Save-data layout.** lbl_8050EC80 is a 0x20-byte header, three 0xC8-byte
  rows at 0x20 and three 0x9C4-byte blocks at 0x280 (size 0x21E0), but users
  still view it as SaveRow_8050EC80[] shifted by 0x20. Migrating all users to
  one SaveData object drops tu_801DE714 __dt__12cls_804A51E4Fv (100 → 98.6 %) and
  act_fairy init (97.86 → 97.39 %); FileSelect's constructor omits its
  save-data loop until this is settled (progress/game__tu_803BC6F0.md).
- **Data of dead-stripped functions.** tu_803A3484 (title screen): a 0x34-byte .rodata table and "enemy_both2_sob.bin" sit before
  the title's strings, and 0.5f / 120.0f lead its .sdata2, but no surviving function references them; the table's tail {3,4,5,4}
  matches tu_803536C4's lbl_80472260, also followed by that string. Likely a function the linker dropped (dead code or a weak
  duplicate). Keeps fn_803A5380 and the ctor at 99.78 %. Same family as the leading-literal items above.
- **The 12 units still out of address order** (tools/order_check.py): reordering
  changes inlining or the .sdata2 pool in each; same family as the two items
  above.
