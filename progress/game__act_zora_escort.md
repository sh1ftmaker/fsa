# game/act_zora_escort checkpoint (2026-11-05)

Baseline 8093 total, unit 9/23. After: 8104, unit 20/23.

Written: vfunc 0x54/0x58/0x5C/0x64/0x70/0x74/0x84/0x9C/0xC4, init, dtor, ctor, plus the key tables (lbl_ statics).
Unwritten: fn_80120174 (vfunc_0x6C, 1020 B), fn_80120570 (vfunc_0x68, 2504 B) and one more over 160 B.
vfunc_0x58 is 99.84 % (reloc-name only, literal label).
SHARED-EDITS: tu_801213EC.h: EscortBase vfunc_0x58 returns u8 (the caller tests it with clrlwi.; also act_escort_woman.h, tu_801213EC.cpp);
vfunc_0x9C(s32, f32 x5) declared from fn_80123394's register use (symbol renamed vfunc_0x9C__19EscortBase_8049E0C8Flfffff, base body unwritten);
act_zora_escort.h: ctor/dtor/init declared, field_0x3c4 carved into u8 + three s32 (size unchanged 0x3D4).
Note: vfunc_0x58 has an empty `case 13: case 14: break;` to reproduce the original's extra range test.

# game/act_zora_escort checkpoint (2026-11-06)

Unit: 20/23 -> 21/23 matched (all 23/23 functions written).
- `ZoraEscort_8049DF78::vfunc_0x58` (`0x80120FC0`): 99.84% -> 100.00% (`.sdata2` literal pool now matches).
- `ZoraEscort_8049DF78::vfunc_0x68` (`0x80120570`, 2504 B): 0% -> 99.57% (`scheduling+regalloc`: `atan2_` argument load order in `stepToPlayer`, and `r29`<->`r30` swap in `stepToPlayer` and `hasNearbyBari`).
- `ZoraEscort_8049DF78::vfunc_0x6C` (`0x80120174`, 1020 B): 0% -> 99.75% (`regalloc-only`: `f3`<->`f5` swap in the second `JGeometry::TUtilf::sqrt` in case 13).
- Dead-stripped same-unit helpers `hasNearbyBari`, `findNearbyPlayer`, and `stepToPlayer` placed in source order to reproduce both the inlined control flow in `vfunc_0x68` and the `.sdata2` float pool order (`300.0f` and `90000.0f` between `vfunc_0x68` and `vfunc_0x6C`, `20.0f` and `10430.378f` after `vfunc_0x6C`).
- SHARED-EDITS: `include/game/tu_801213EC.h`: declared `EscortBase_8049E0C8::fn_80122730(s32 dir, bool a, bool b)` (and renamed `fn_80122730` -> `fn_80122730__19EscortBase_8049E0C8Flbb` in `config/G4SE01/symbols.txt`).
