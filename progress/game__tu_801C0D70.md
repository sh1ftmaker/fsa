# game/tu_801C0D70.cpp (cls_801C0D70, the Shadow Link fight tracker)

Baseline 8093 / 11557 (0/14 written). After: 8103 (+10); 10/14 functions 100 %.
Resistant (FIXMEs in source): fn_801C0DCC 94.93 % regalloc only (this r30 vs r31); fn_801C0FEC 96.46 % scheduling of the loads after normalize; fn_801C1374 94.12 % (bool test materialised once in r3, cmpwi 3); fn_801C1440 96.54 % (loop counter copies found's zero: mr r5, r6).
const methods (loads hoisted): fn_801C0FEC, fn_801C1224. Ctor defined inline in the cpp above the getter (inlined into fn_801C0D70, body duplicates fn_801C151C as in the original).
SHARED-EDITS: include/game/GameGlobals.h cls_801C0D70 (size 0x60 not 0x160; fields 0x0-0x8 u16, mCount 0xC, mSerials[16] 0x10, field_0x50 u32, field_0x54 TVec3f; new members; fn_801C0FEC const); include/game/act/act_shadow_link_2.h (fn_801C272C member, symbol renamed in symbols.txt). symbols.txt: member renames, sinit, bss split (lbl_804C7E70$436 0x60 + 9 @680.. + 9 pools), init$437.
