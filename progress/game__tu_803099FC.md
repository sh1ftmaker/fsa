# game/tu_803099FC (cls_804AA8C8, the pad input object)

## 2026-11-05
Baseline 7964 functions (unit 0/27). After: 7983 (unit 19/27, +19). Not Matching.
Matched: fn_803099FC, 80309A00, A38, A54, A78, D24, D48, D6C, 80309F20, FCC, 8030A078, A2E4, A308, A364, A388, AAF4 (free), AB38, dtor, ctor.
Resistant:
- fn_80309D90 99.30 % regalloc-only (x/y stick floats f1/f2 swapped).
- fn_8030A3B0 87.14 % missing/extra (inlined fn_8030A308 tail is `cmplwi 1` + branches in the original).
- __sinit 99.61 % operands: the first 0x10 bytes of the unit's .bss (0x805076E8) belong to a static of an unwritten function (fn_8030A43C), so the pool objects sit 0x10 lower in ours.
Unwritten: fn_80309A9C / 80309B74 / 80309C4C (each inlines getPlayerInputSlot_8030AAF4 and a later-defined accessor, D48 / D24 / D6C; with -inline auto they come out as calls, 44 %, so not written; need -inline deferred, which is a crack-pass decision), fn_8030A258 (reads lbl_804DB798 / lbl_8053ACE4 of other units), fn_8030A43C (0x6B8, pad poll).
SHARED-EDITS: include/game/GameGlobals.h: cls_804AA8C8 fields (arrays of four per slot, offsets 0x4..0xE6, object size 0xEC from tu_803FF2C4's new), ctor/virtual dtor, accessors, getPlayerInputSlot_8030AAF4 moved out of extern "C" (now C++, mangled getPlayerInputSlot_8030AAF4__Fl), lbl_8053AD34[4] extern (owner tu_80301CD0). symbols.txt renames: 10 mangled placeholders, __dt__/__ct__/__vt__12cls_804AA8C8, __sinit_tu_803099FC_cpp.
