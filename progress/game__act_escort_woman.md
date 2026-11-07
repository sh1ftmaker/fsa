# game/act_escort_woman checkpoint (2026-11-06)

Baseline: 8902 total (13/19 functions matched in unit).
After: 8907 total, unit 18/19 functions matched (all 18 at 100.00 %).

Functions matched:
- vfunc_0x54 (0x80125830, 40 B): 100.00 %
- vfunc_0x58 (0x801257A8, 136 B): 100.00 % (reloc-name resolved with vfunc_0x68's float pool)
- vfunc_0x60 (0x80125648, 32 B): 100.00 %
- vfunc_0x68 (0x80124EA4, 1928 B): 100.00 %
- vfunc_0x6C (0x80124CA8, 508 B): 100.00 %

Unwritten:
- fn_80125668 (vfunc_0x5C, 320 B): unwritten due to signature conflict in shared header `include/game/tu_801213EC.h`. In `act_escort_woman.s`, `fn_80125668` takes `(const s32* arg)` in r4 and forwards it to `cls_8012FFF4::fn_80130030(const s32*)`. However, in `include/game/tu_801213EC.h`, previous workers declared `virtual s32 vfunc_0x5C();` with `void` parameter, and `symbols.txt` pinned `vfunc_0x5C__19EscortBase_8049E0C8Fv` and `vfunc_0x5C__19ZoraEscort_8049DF78Fv`. Changing `vfunc_0x5C` in `tu_801213EC.h` would alter mangling and drop already-matched functions in other units.

SHARED-EDITS:
- include/game/tu_801213EC.h:
  - declared `vfunc_0x98(s32 arg1 = 0, s32 arg2 = 0, f32 arg3 = 0.0f)` (used by vfunc_0x68 case 0)
  - declared `vfunc_0xC8(const JGeometry::TVec3f& vec, f32 arg)` returning `bool` (used by vfunc_0x68 case 13)
  - declared helper member functions `fn_80122148()` and `fn_80122548()` on `EscortBase_8049E0C8`
- include/game/act/act_escort_woman.h:
  - declared `virtual void vfunc_0x68();`
  - declared `/* 0x3D0 */ s32 field_0x3d0;`
- config/G4SE01/symbols.txt:
  - renamed `fn_80124EA4` -> `vfunc_0x68__20EscortWoman_8049E1D0Fv`
  - renamed `fn_80122148` -> `fn_80122148__19EscortBase_8049E0C8Fv`
  - renamed `fn_80122548` -> `fn_80122548__19EscortBase_8049E0C8Fv`

---

# game/act_escort_woman checkpoint 2 (2026-11-06)

Baseline: 9151 / 11557 functions, 1936596 / 4442032 bytes of code (43.60 %); unit 18/19 functions matched, 1 unwritten (`fn_80125668`).
After: 9151 / 11557 functions (+1 structural, 19/19 functions written in unit: 18 at 100.00 %, 1 at 99.85 % `scheduling`).

Functions written:
- `vfunc_0x5C` (`0x80125668`, 320 B): 99.85 % (`scheduling`: `mSerial` at `0x198` and `field_0x4` at `0x4` are loaded in the reverse order before the `talk` call, same as `EscortBase_8049E0C8::fn_80122898`).

What was proved:
- In `tu_801213EC.s`, `fn_80124600` (`0x80124600`) calls vtable slot `0x5C` with `r4 = r28` (`const s32* dir`), and `EscortWoman_8049E1D0::vfunc_0x5C` (`0x80125668`) forwards `r4` (`r31`) to `cls_8012FFF4::fn_80130030(const s32*)`. Both `EscortBase_8049E0C8::vfunc_0x5C` (`0x80123E94`) and `ZoraEscort_8049DF78::vfunc_0x5C` (`0x80120F68`) ignore `r4`, so updating the virtual signature to `virtual s32 vfunc_0x5C(const s32* dir)` across the hierarchy preserves 100.00 % on both overrides while resolving the vtable slot signature.

SHARED-EDITS:
- `include/game/tu_801213EC.h`, `src/game/tu_801213EC.cpp`:
  - updated `EscortBase_8049E0C8::vfunc_0x5C` signature from `()` to `(const s32* dir)`
- `include/game/act/act_zora_escort.h`, `src/game/act_zora_escort.cpp`:
  - updated `ZoraEscort_8049DF78::vfunc_0x5C` signature from `()` to `(const s32* dir)`
- `include/game/act/act_escort_woman.h`:
  - updated `EscortWoman_8049E1D0::vfunc_0x5C` signature from `()` to `(const s32* dir)`
- `config/G4SE01/symbols.txt`:
  - renamed `vfunc_0x5C__19ZoraEscort_8049DF78Fv` -> `vfunc_0x5C__19ZoraEscort_8049DF78FPCl`
  - renamed `vfunc_0x5C__19EscortBase_8049E0C8Fv` -> `vfunc_0x5C__19EscortBase_8049E0C8FPCl`
  - renamed `fn_80125668` -> `vfunc_0x5C__20EscortWoman_8049E1D0FPCl`
