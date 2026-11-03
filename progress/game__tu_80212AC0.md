# game/tu_80212AC0.cpp

Baseline: total: 5901 / 11557 functions, 1181784 / 4442032 bytes (26.60 %). After: 5905 / 11557, 1182500 bytes.

Matched (4/5): fn_80212AC0 (getter of the guarded static), fn_80212BA0, fn_80212BFC, __sinit_tu_80212AC0_cpp (generated; bss/sbss split by symbols.txt).

Resistant: cls_80212AC0::fn_80212AF0 (86%, missing/extra, STOP after 6 tries).
Original loads mCount once into r31 before the loop and numbers registers
pos r26, this r27, actor r28, found r29, i r30. Ours (mCount in the test, or a cached local in 3 variants) gets this r31, found r30, i r28 etc. mCount-cached variants were 84.7-85.1%, direct 86.25%.

SHARED-EDITS: include/game/GameGlobals.h: fn_80211000 / fn_80210E90 (owner act_quicksand.cpp) extern "C" decls.
symbols.txt: renames of this unit's placeholders; bss split into lbl_804CCDB8$423 (0x84), @479-487 records, 9 memPool lbl_ entries; init$424 for the guard.
Next: AF0 register allocation; the extab/extabindex % follow once AF0 matches.
