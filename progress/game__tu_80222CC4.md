# game/tu_80222CC4.cpp

Baseline (progress.py first line at start): total: 5980 / 11557 functions, 1201588 / 4442032 bytes of code (27.05 %)
After: total: 5985 / 11557 functions, 1202140 / 4442032 bytes of code (27.06 %)

Status: NonMatching. 6/7 functions at 100 % (__sinit generated); vfunc_0x90 is 99.7 % (reloc-name only).

## Matched
- cls_804A6C38 (vtable 0x804A6C38, sub-base cls_804A6A00): ctor, dtor, init, vfunc_0x6C, vfunc_0x88 (the DDGO shot release is an inline helper with the serial read once).
- vtable __vt__12cls_804A6C38 is 100 %.

## Resistant
- vfunc_0x90 (0x80222D5C): instructions identical; remaining gap is .data order (ours jump table, weak J3DFrameCtrl vtable, vtable; original vtable at 0, jump table at 0xA4). Names: jumptable_804A6CDC vs @544, and the float literals (lbl_8053FE70 = 2.5f is a named external constant, now declared extern in cls_804A6A00.h). .sdata2 83.3 %, extab/extabindex 56 %: weak-dedup tooling, .data 83.7 % (our object also emits __vt__12J3DFrameCtrl and the jumptable under another name).

## SHARED-EDITS
- include/game/cls_804A6A00.h (new): the sub-base of tu_80220F54 / tu_80221B44 / act_shadow_link_* / tu_802233A4 (vtable 0x804A6A00; ctor 0x802218C8 stores it and 0x2A8-0x2B4).
- symbols.txt placeholder renames: fn_802218C8 -> __ct__12cls_804A6A00Fv, fn_802217C8 -> init__, fn_80221184 -> vfunc_0x6C__, fn_80220F54 -> draw__, fn_80221158 -> vfunc_0x60__, fn_80221160 -> vfunc_0x74__, fn_80221210 -> vfunc_0x9C__...FUc, fn_80221114 -> vfunc_0xA0__, fn_80221250/802212AC/80221334/80221408 -> ..__12cls_804A6A00Fv, fn_8022149C/fn_802216E8 -> ..Fl, lbl_804A6A00 -> __vt__12cls_804A6A00 (all __12cls_804A6A00).
- gen_game.tsv: the tu_80222CC4 row deleted.

## Next
tu_80220F54 should adopt cls_804A6A00.h (its names are already in symbols.txt).

## Jump-table .data order (re-check 2026-11-03)
- Cause: `-inline auto` emits a function's jump table when the function is compiled and the class vtable at the
  end of the TU (weak J3DFrameCtrl vtable in between). The original (vt 0, jt 0xA4) is what `-inline auto,deferred`
  gives with the functions in reverse order (scratchpad research_jtorder.md; docs/GAME_PLAN.md "-inline auto vs deferred").
- Ruled out (new measurement, scratch TU jtorder/a.cpp, switch fn defined before ctor/dtor): GC/2.0, 2.5, 2.6, 2.7,
  3.0a3, 3.0a5.2 all put the jump table at .data 0 and the vtable after it under `-inline auto`; the compiler
  version is not the cause. vfunc_0x90 can't be an inline (weak) function either: it sits between strong
  functions in the original .text (0x80222D5C), before init/dtor/ctor.
- Reloc-name: not a separate problem. Matching units with switches (JASChannel, J2DPrint, vi) keep `@NNN` vs
  `jumptable_8...` and still score 100 %: objdiff pairs local data by position. Fixing the order fixes the name.
- Verdict: blocked on the project-wide `-inline` decision; nothing to change in the source.
