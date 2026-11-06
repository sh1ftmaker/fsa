# game/act_switch
Baseline: total: 8902 / 11557 functions, 1793240 / 4442032 bytes of code (40.37 %)
After: 8906 / 11557, 1794032 bytes of code (40.39 %)
Matched:
- init__15Switch_804A7F38Fv (100.00 %, 416 bytes)
- draw__15Switch_804A7F38Fv (100.00 %, 4 bytes)
- vfunc_0x54__15Switch_804A7F38Fl (100.00 %, 276 bytes)
- __dt__15Switch_804A7F38Fv (100.00 %, 96 bytes)
- __sinit_act_switch_cpp (100.00 %, 548 bytes)

Sub-100 %:
- vfunc_0x10__15Switch_804A7F38Fv (99.69 %, 900 bytes, regalloc-only: r30->r31, r4->r5, r5->r4, r31->r30)
- update__15Switch_804A7F38Fv (99.71 %, 2608 bytes, operands: regalloc r27/r28, and fn_802302C4 3rd arg 0xFF vs bool in shared Player.h)

Data sections:
- .ctors (100.00 %)
- .rodata (100.00 %)
- .data (100.00 %)
- .bss (100.00 %)
- .sdata2 (100.00 %)

SHARED-EDITS:
- include/game/Player.h: fn_802302C4 declared as `bool fn_802302C4(s32 player, s32 a, bool b);` should take `u8 b` (or s32). In the binary, both call sites at 0x802A71EC and 0x802A740C pass `li r5, 0xff`, which is stored via `stb r31, 0xca2(r29)` in Player_804A6F38::fn_8024DA64. Because Player.h currently declares it as bool, MWCC coerces 0xFF to 1 (`li r5, 1`).
- config/G4SE01/gen_game.tsv: deleted row for act_switch as it is now handwritten.
