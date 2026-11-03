# game/tu_801D11FC

Baseline (progress.py): total 6340 / 11557 functions, 1270548 bytes.
After: 6341 / 11557, 1270676 bytes. gate: ok.

Matched: __ct__12cls_804A46F4Fv (0x801D1974), __sinit.
Resistant (cls_804A46F4 members):
- fn_801D11FC 96.15 % regalloc-only (r5/r3/r4 and f-register naming around the angle compare).
- fn_801D12CC 86.4 % missing/extra: front-pop loop unrolls differently (original recomputes the
  source index, walking store pointer), final field_0x32c.set(mPath[0]) clamps the index twice,
  pos push loads pos.x after the address.
- fn_801D183C 99.24 % scheduling+regalloc: result/player in r30/r31 swapped.
extab 55 %, extabindex 0 %: follow the function sizes.

SHARED-EDITS: cls_804A46F4.h (ctor, fn_801D11FC, fn_801D183C, mPathCount/mPath[16] at 0x2A8/0x2AC),
EnemyBase.h (fn_801CF888 first arg const TVec2f*: the callee reads x, y only, 8-byte stack slots),
tu_801D3320.cpp (cast at its fn_801CF888 call), Player.h (fn_8022E114 extern "C", owner tu_8022AD64),
gen_game.tsv row removed, symbols.txt renames (fn_801D11FC, fn_801D183C, fn_801D1974 -> __ct__,
fn_801CF888 mangled arg, fn_801546E4 -> __ct__Q29JGeometry8TVec2<f>Fv, lbl_804A46F4 -> __vt__12cls_804A46F4).
