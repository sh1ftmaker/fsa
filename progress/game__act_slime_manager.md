# game/act_slime_manager.cpp

## Summary
- Unit: `game/act_slime_manager.cpp` (`cls_804A5BD8`, inherits `cls_804A5B28` at size `0x34C`)
- Matched functions: 11 / 14 at 100.00% (up from 10 / 14; all 4 previously unwritten functions implemented)
- `.text` match: 99.46%

## Functions
| Address | Symbol | Match | `try.py` Class | Notes |
|---|---|---|---|---|
| `0x80208C38` | `__sinit_act_slime_manager_cpp` | 100.00% | 100% | Existing |
| `0x80208BE0` | `cls_804A5BD8::cls_804A5BD8()` | 100.00% | 100% | Existing |
| `0x8020896C` | `cls_804A5BD8::init()` | 100.00% | 100% | Newly matched (`init__12cls_804A5BD8Fv`) |
| `0x80208924` | `cls_804A5BD8::vfunc_0x54()` | 100.00% | 100% | Renamed from `fn_80208924__12cls_804A5BD8Fv` (vtable slot `0x54`) |
| `0x802088B4` | `cls_804A5BD8::vfunc_0xA4()` | 100.00% | 100% | Renamed from `fn_802088B4__12cls_804A5BD8Fv` (vtable slot `0xA4`) |
| `0x80208834` | `cls_804A5BD8::vfunc_0xA8()` | 100.00% | 100% | Renamed from `fn_80208834__12cls_804A5BD8Fv` (vtable slot `0xA8`) |
| `0x80208520` | `cls_804A5BD8::fn_80208520(JGeometry::TVec2f*, JGeometry::TVec2f*)` | 98.86% | `regalloc-only` | `r25`-`r28` shifted by 1 in `calcCenter` loops; `f2`/`f3`/`f4` vs `f30`/`f31` in offset addition |
| `0x80208244` | `cls_804A5BD8::fn_80208244(int)` | 98.24% | `missing/extra` | 1 fewer instruction (182 vs 183: `mr r28, r3` in case 3), plus `r26`-`r29` regalloc and stack frame offset swap (`0xc` vs `0x1c`) |
| `0x80207F14` | `cls_804A5BD8::vfunc_0x90(u8)` | 99.80% | `regalloc-only` | `r29` vs `r27` for loop counter `i` in `checkTrigger` case 2 |
| `0x80207F00` | `cls_804A5BD8::vfunc_0x88()` | 100.00% | 100% | Existing |
| `0x80207ECC` | `cls_804A5BD8::fn_80207ECC()` | 100.00% | 100% | Existing |
| `0x80207E58` | `cls_804A5BD8::fn_80207E58()` | 100.00% | 100% | Existing |
| `0x80207E24` | `cls_804A5BD8::fn_80207E24()` | 100.00% | 100% | Existing |
| `0x80207D84` | `cls_804A5BD8::~cls_804A5BD8()` | 100.00% | 100% | Existing |

## Evidence for Symbol & Header Updates
- `init__12cls_804A5BD8Fv` (`0x8020896C`): vtable `0x804A5BD8` slot `0x0C` (`ActorBase_804A5770::init`), calls `EnemyBase_804A4808::init()`.
- `vfunc_0x54__12cls_804A5BD8Fv` (`0x80208924`): vtable `0x804A5BD8` slot `0x54` (overrides `ActorBase_804A5770::vfunc_0x54`).
- `vfunc_0x90__12cls_804A5BD8FUc` (`0x80207F14`): vtable `0x804A5BD8` slot `0x90` (overrides `EnemyBase_804A4808::vfunc_0x90(u8)`).
- `vfunc_0xA4__12cls_804A5BD8Fv` (`0x802088B4`): vtable `0x804A5BD8` slot `0xA4` (overrides `cls_804A5B28::vfunc_0xA4`).
- `vfunc_0xA8__12cls_804A5BD8Fv` (`0x80208834`): vtable `0x804A5BD8` slot `0xA8` (overrides `cls_804A5B28::vfunc_0xA8`).
- `fn_80208520__12cls_804A5BD8FPQ29JGeometry8TVec2<f>PQ29JGeometry8TVec2<f>` (`0x80208520`): member of `cls_804A5BD8` called by `fn_80208244` case 4 with two `JGeometry::TVec2f*` out-parameters.
- `fn_80209D20__12cls_804A5D60FlUc` (`0x80209D20`) and `fn_80209CF4__12cls_804A5D60FPCQ29JGeometry8TVec2<f>` (`0x80209CF4`): methods of `cls_804A5D60` in `game/tu_80209C14.cpp` called on slot actors in `fn_80208244` cases 3 and 4 (alongside existing `fn_80209D44` and `fn_80209CA4` in `include/game/act/tu_80209C14.h`).
