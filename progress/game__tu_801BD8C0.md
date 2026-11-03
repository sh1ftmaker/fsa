# game/tu_801BD8C0.cpp

Baseline (progress.py first line at start): total: 5866 / 11557 functions, 1174344 / 4442032 bytes of code (26.44 %)
After: total: 5871 / 11557 functions, 1175144 / 4442032 bytes of code (26.46 %)

Status: 6/7 functions match (was 1/7); .text 98.73 %, .data 99.38 % (vtable's trailing pad word, as in every vtable unit), .sdata2 and .bss 100 %. gate: ok. Not flagged Matching (draw).

Matched: fn_801BD8C0 (member called by act_fire_bar), update, init, dtor, ctor, __sinit.

Resistant:
- draw (0x801BD978), 79 %, `missing/extra`, STOP after 2 tries. Same shape as
  cls_804A6828::draw (tu_802201D8): original `li r7,0; addi r5,r3,0xc; ...; lwz r4,4(r3); lwz r3,lbl_8053AB10; lwz r3,0x2c(r3)`
  (this stays in r3, receiver loaded last); ours moves `this` to r7 and loads the receiver first.
  A local colour + local receiver gave 77 %.

SHARED-EDITS: include/game/GameGlobals.h (fn_801CB7CC declaration), config/G4SE01/gen_game.tsv (row removed),
symbols.txt (this unit's names, lbl_8053EC88 merged with lbl_8053EC8A into one 4-byte u16[2]).

Next: draw receiver-order; act_fire_bar can call cls_804A4038::fn_801BD8C0 (include game/act/tu_801BD8C0.h).
