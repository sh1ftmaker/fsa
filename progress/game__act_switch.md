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
- include/game/act/tu_801F9484.h: Switch_804A7F38 fields, virtuals and readTileId().
- config/G4SE01/gen_game.tsv: deleted row for act_switch as it is now handwritten.
- Not done: fn_802302C4's `bool b` should be a byte (callers pass 0xFF); the mangled name (__Fllb) and the generated tu_8022AD64 forbid changing it.
