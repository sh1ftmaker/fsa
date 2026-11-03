# game/tu_8021FDB8
Baseline: total: 5734 / 11557 functions, 1152552 / 4442032 bytes of code (25.95 %)
Matched: __sinit_tu_8021FDB8_cpp (generated; gen_game.tsv rows deleted).
Resistant: cls_804A51E4::fn_8021FDB8 98.43 % scheduling. The lwz of mTargetSerial (0x2BC) into r31 comes after
`mr r30, r3` in ours; the original loads it before `stw r30` (hunk: 0x10 delete / 0x1C insert).
extab/extabindex are 30.77 % (follow the function).
SHARED-EDITS: symbols.txt renames of fn_8021FDB8 and fn_801E1D9C (tu_801DE714's) to __12cls_804A51E4Fv;
new include/game/cls_804A51E4.h (TINK class, vtable 0x804A51E4 from ctor fn_801E22A4).
