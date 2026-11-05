# game/act_zora_escort checkpoint (2026-11-05)

Baseline 8093 total, unit 9/23. After: 8104, unit 20/23.

Written: vfunc 0x54/0x58/0x5C/0x64/0x70/0x74/0x84/0x9C/0xC4, init, dtor, ctor, plus the key tables (lbl_ statics).
Unwritten: fn_80120174 (vfunc_0x6C, 1020 B), fn_80120570 (vfunc_0x68, 2504 B) and one more over 160 B.
vfunc_0x58 is 99.84 % (reloc-name only, literal label).
SHARED-EDITS: tu_801213EC.h: EscortBase vfunc_0x58 returns u8 (the caller tests it with clrlwi.; also act_escort_woman.h, tu_801213EC.cpp);
vfunc_0x9C(s32, f32 x5) declared from fn_80123394's register use (symbol renamed vfunc_0x9C__19EscortBase_8049E0C8Flfffff, base body unwritten);
act_zora_escort.h: ctor/dtor/init declared, field_0x3c4 carved into u8 + three s32 (size unchanged 0x3D4).
Note: vfunc_0x58 has an empty `case 13: case 14: break;` to reproduce the original's extra range test.
