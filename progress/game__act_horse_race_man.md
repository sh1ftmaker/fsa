# game/act_horse_race_man

Unit: game/act_horse_race_man.cpp (HorseRaceMan_804A5630, NPC minigame manager)
Status: .text 99.67% (5976 / 5996 bytes), .rodata 100%, .sdata2 100%, .data 98.14%, .ctors 100%, .bss 100%.

Functions matched at 100 % (8 / 10):
- draw (0x801EBC84, 232 bytes): 100.00 %
- vfunc_0x9C (0x801EBD6C, 208 bytes): 100.00 %
- vfunc_0x94 (0x801EBE3C, 64 bytes): 100.00 %
- vfunc_0x90 (0x801EBE7C, 2364 bytes): 100.00 %
- init (0x801ECFBC, 240 bytes): 100.00 %
- __dt (0x801ED0AC, 152 bytes): 100.00 %
- __ct (0x801ED144, 116 bytes): 100.00 %
- __sinit_act_horse_race_man_cpp (0x801ED1B8, 548 bytes): 100.00 %

Sub-100 % functions (2 / 10, both regalloc-only):
- fn_801ECB08 (0x801ECB08, 1204 bytes): 99.55 % (try.py: regalloc-only: target r29/r28/r31 vs ours r28/r29/r25 in case 7 and case 13 loops)
- fn_801EC7B8 (0x801EC7B8, 848 bytes): 98.28 % (try.py: regalloc-only: target r26/r27/r23 vs ours r27/r28/r24 for this, loop counter, and field_0x4)

SHARED-EDITS:
- config/G4SE01/symbols.txt: Evidence-backed symbol renames for vfunc_0x90 (fn_801EBE7C), fn_801EC7B8, and fn_801ECB08.
- include/game/tu_80215A94.h, include/game/cls_8021C024.h, src/game/tu_8021BFA8.cpp: cls_8021C024 moved to a shared header (tu_8021BFA8.cpp includes it); field_0x94 typed; fn_80215A94/fn_80215C6C/fn_80215F30/fn_802160F4 are cls_80215E48 members (symbols.txt renamed to the mangled names).
