# game/tu_801CB270 checkpoint (2026-11-05)

Baseline 8093 total, unit 20/51. After: 8096, unit 23/51.

Matched (new): fn_801CB7CC (angle mirror), fn_801CCDA4, fn_801CD8E4.
Written, below 100 % (FIXMEs in the source):
- fn_801CB4A4 / fn_801CB538 / fn_801CB688 (86.5 %, scheduling+regalloc): saved registers assigned r31 layer, r30 depth, r29 height, r28 pos in the target; ours reversed.
- fn_801CB71C (93.9 %): the target has no second NULL test from the inlined deleteEmitter().
- fn_801CD0D0 / fn_801CD150 (79.1 %): target subtracts x and y first, then adds box corners, loads z last.
- fn_801CD970 (90.6 %): with (s16, s16) parameters it is 100 % but fn_801CD930's callers pass unextended s32 (original is likely C linkage with mismatched prototypes).
- fn_801CCCC4, fn_801CCD34, fn_801CD6D8, fn_801CD9F0: code identical, only literal labels (@NNN vs lbl_) differ.
Symbol renames (symbols.txt): fn_801CB4A4/538/688/71C/7CB7CC, CCCC4, CCD34, CCDA4, CD0D0, CD150, CD6D8, CD8E4, CD970, CD9F0 got their mangled names; the header declarations were moved out of extern "C".
Next: unwritten functions > 160 B (fn_801CB5CC, fn_801CB85C, ...).
