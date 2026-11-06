# `game/act_vaati_demo.cpp`

## Functions matched / written

| Address | Symbol | Status | Notes |
| --- | --- | --- | --- |
| `0x80116BE8` | `__ct__12cls_8049D940Fv` | 100.0% | Constructor |
| `0x80116B08` | `__dt__12cls_8049D940Fv` | 100.0% | Destructor |
| `0x801169E8` | `init__12cls_8049D940Fv` | 99.93% (`reloc-name`) | `.sdata2` literal symbol name |
| `0x8011661C` | `update__12cls_8049D940Fv` | 99.79% (`reloc-name`) | 100% instruction match; only `.sdata2` / `.sbss` symbol names differ |
| `0x80116458` | `draw__12cls_8049D940Fv` | 98.09% (`scheduling + reloc-name`) | `&mPos` computed after layer check |
| `0x801162E0` | `vfunc_0x54__12cls_8049D940Fv` | 100.0% | Cutscene timer dispatch |
| `0x801162D8` | `vfunc_0x58__12cls_8049D940Fv` | 100.0% | Returns `field_0x230` |
| `0x801162B8` | `vfunc_0x5C__12cls_8049D940Fv` | 100.0% | Returns `1` in state 11, else `0` |
| `0x80115F9C` | `fn_80115F9C__12cls_8049D940Fl` | 94.36% (`scheduling+regalloc + reloc-name`) | State transition handler |
| `0x80115D8C` | `fn_80115D8C__12cls_8049D940FPCQ29JGeometry8TVec3<f>PQ29JGeometry8TVec3<f>` | 98.22% (`regalloc-only + reloc-name`) | Tangential spin velocity calculation |
| `0x80115BC4` | `fn_80115BC4__12cls_8049D940FPCQ29JGeometry8TVec3<f>PQ29JGeometry8TVec3<f>` | 92.46% (`missing/extra + reloc-name`) | Radial pull + tangential spin velocity (`fsel` vs branch on `min(dist, speed)`) |
| `0x80115A7C` | `fn_80115A7C__12cls_8049D940Fv` | 94.39% (`missing/extra`) | Player vortex pull loop |
| `0x801158B8` | `fn_801158B8__12cls_8049D940Fv` | 88.70% (`missing/extra + reloc-name`) | Black overlay quad draw |
| `0x80116C68` | `__sinit_act_vaati_demo_cpp` | 100.0% | Static audio pool init |

## Evidence for new symbol names

- `update__12cls_8049D940Fv` (`0x8011661C`): Occupies slot `0x14` (`ActorBase_804A5770::update`) of `__vt__12cls_8049D940` (`0x8049D940`), and begins by calling `cls_8049D140::update()` (`update__12cls_8049D140Fv`).
- `fn_80115D8C__12cls_8049D940FPCQ29JGeometry8TVec3<f>PQ29JGeometry8TVec3<f>` (`0x80115D8C`): Member of `cls_8049D940` (`r3` = `this`, reads `this->mPos` at `+0x18`/`+0x1C`, `r4` = `const JGeometry::TVec3f*`, `r5` = `JGeometry::TVec3f*`).
- `fn_801EB418__12cls_801EB8CCFPCQ29JGeometry8TVec3<f>` (`0x801EB418`): Called on `&this->field_0x2a8` (`cls_801EB8CC`) with `&this->field_0x308` (`const JGeometry::TVec3f*`); inside `0x801EB418`, copies `r4->x, y, z` into `this+0x40..0x48` and updates `cls_801EB8CC` timers.
- `fn_802B3A3C__12cls_804A8638Fv` (`0x802B3A3C`): Called on the `'SWEV'` actor (`cls_804A8638`, constructed by `fn_802B3A48` which sets `__vt__12cls_804A8638`); stores 0 to `this->field_0x2b8` (`li r0,0; stw r0,0x2b8(r3)`).

## Shared edits

SHARED-EDITS:
- `include/game/cls_801EB8CC.h`: Added member declaration `void fn_801EB418(const JGeometry::TVec3f* pos);`.
- `include/game/cls_804A8638.h`: Carved `/* 0x2B8 */ s32 field_0x2b8;` out of `field_0x294` padding (preserving size `0x33C`) and added member declaration `void fn_802B3A3C();`.
