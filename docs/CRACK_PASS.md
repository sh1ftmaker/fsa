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

## Retired 2026-11-05: tu_80222CC4, tu_80221B44 (`defer_codegen`) and act_shadow_link_bow

Game code is now compiled `-inline auto,deferred` project-wide (docs/INLINING.md),
which is what the `#pragma defer_codegen on` + reversed-functions trick reproduced
for these two units. The pragmas and their FAKEMATCH(crack-pass) tags are gone; the
sources keep their (now project-standard) descending order. act_shadow_link_bow's
"second trick" is the evidenced whole-file `#pragma auto_inline off` of pragma rule 2;
its vfunc_0x90 is at 100 %.

## Open questions to revisit once more code is written (2026-11-05)

Not tricks: patterns no honest form explains yet. Revisit after the
surrounding code exists, since finishing the rest of a unit may show the cause.

- **Literals pooled before their first visible use.** PARTLY SOLVED 2026-11-05 (tu_8016A2B4): file-scope static const aggregates, and helpers defined early whose out-of-line copy was dead-stripped (see RULES_CARD "Known game-code shapes"); revisit the other units below with that in mind. tu_8016A2B4 (ten GXColor
  objects then 0.125/1/0.5/0 ahead of fn_8016A7DC's literals), tu_801213EC (8.0f
  before 0.0f, 10430.378f before -24f), tu_803BC6F0 (0, -200, 200 first, used
  only by later functions) and tu_8027E5BC (1.5f, 3.0f). Literals are created per
  function in compile order, and an inline helper's aggregate initializers when
  the inline is first read. Candidates: early inline helpers, dead code the
  optimizer removed, or a deferred/reordered codegen mode. Each unit's
  checkpoint has the pool layout.
- **Inlining of later-defined functions.** ANSWERED 2026-11-05: the original was
  compiled with deferred inlining, now the project mode (docs/INLINING.md). The units
  listed here (tu_803099FC, tu_8043A0E8, tu_80256C74, tu_8016A2B4, tu_802F5040,
  tu_803944C8) can write the callee as a plain function.
- **Weak destructor emitted first.** ANSWERED 2026-11-05 (deferred inlining):
  tu_8045A640 now emits the weak __dt__Q29GameAudio16CustomSoundTableFv first;
  every function, .data and .sdata2 are 100 % (extab 71 %: check the weak-dedup
  trial link before flipping it).
- **Save-data layout.** lbl_8050EC80 (cls_804AFB30, game/tu_803B8C74.h) is a 0x20-byte
  header, three 0xC8-byte SaveFile_803B950C at 0x20 and three 0x9C4-byte bodies at 0x280;
  every user reads it through getFile / getCurrentFile. Two functions read and write one field
  of the current file through a pointer the original holds at lbl_8050EC80 + file * 0xC8
  (fields at +0x20): tu_801DE714 __dt__12cls_804A51E4Fv (clamp of 0x2C) and act_fairy init
  (bit 0 of 0x87). Every access through mFiles adds an `addi 0x20` (reference/pointer local,
  pointer accessor, inline SaveFile member, nested table struct: 97.2-98.6 / 95.4-97.4 %) or
  CSEs the field address into an lwzu (direct expressions, 98.36 %).
  - **Trick:** `FAKEMATCH(crack-pass)` SaveFileRow_CrackPass (two fields, row offsets) and
    cls_804AFB30::getCurrentRow_CrackPass (`reinterpret_cast<SaveFileRow_CrackPass*>(this)
    [mCurrentFile]`), used only by those two functions: 100 % and 97.86 % (their values
    under the old SaveRow_8050EC80 view).
  - **Honest fix would need:** the original's declaration that gives a 0xC8-stride object
    based at lbl_8050EC80 itself (FSA probably viewed the save data as rows somewhere; no
    donor has this class).
- **Data of dead-stripped functions.** tu_803A3484 (title screen): a 0x34-byte .rodata table and "enemy_both2_sob.bin" sit before
  the title's strings, and 0.5f / 120.0f lead its .sdata2, but no surviving function references them; the table's tail {3,4,5,4}
  matches tu_803536C4's lbl_80472260, also followed by that string. Likely a function the linker dropped (dead code or a weak
  duplicate). Keeps fn_803A5380 and the ctor at 99.78 %. Same family as the leading-literal items above.
- **Overflow trap on the player life word (tu_8026FC20 fn_80270DDC).** The original
  does `old = x; x++; if (x == old) for (;;); x = old;` on Player 0xBFC (stw, reload,
  `b .` self-branch, restore) before healing. It is written literally with a comment; no
  assert macro, busy-wait or donor idiom (grep of src/ and donors) yields it. Not a
  FAKEMATCH (nothing is steered), but the source form is unknown; revisit if another
  function shows the same increment-compare-spin shape. It now appears a second time:
  tu_8023FBF0 addCounter_8024ED4C (fn_8024ED4C, Player 0xC04), preceded there by an
  `if (c > 9999) for (;;)` check that fn_8024ECCC also has; the same unexplained assert-like
  halts, fields differ, so no shared inline is written.
- **The 11 units still out of address order** (tools/order_check.py; 12 before the
  deferred switch, act_vaati_demo fixed by it): reordering changed inlining or the
  .sdata2 pool in each under `-inline auto`; revisit each with deferred inlining
  and the auto_inline rule (docs/INLINING.md).
