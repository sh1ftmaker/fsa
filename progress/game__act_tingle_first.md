# game/act_tingle_first.cpp

Baseline: All: 43.52% matched, 19.37% linked (470 / 919 files), 9153 / 11557 functions (3/6 -> 5/6 in unit).

## Matched (5/6)
- `__sinit_act_tingle_first_cpp` (0x80317C94): 100%
- `cls_804AADD8::cls_804AADD8` (0x80317C38): 100%
- `cls_804AADD8::~cls_804AADD8` (0x80317BB8): 100%
- `cls_804AADD8::init` (0x80317B08): 99.43% -> 100% (`.sdata2` pool order matched)
- `cls_804AADD8::draw` (0x803172DC): 99.62% -> 100% (`.sdata2` pool order matched via dead-stripped `selectNearestPlayer` below `draw`)

## Resistant (1/6)
- `cls_804AADD8::update` (0x80317378): 98.00% (`missing/extra`, 487 vs 484 instrs).
  - The original uses a paired-single squared distance (`psq_l`/`ps_sub`/`ps_mul`/`ps_madd`/`ps_sum0`, 9 instrs vs 12 scalar) in `selectNearestPlayer`'s loop (`TVec3f::squared(const TVec3f&)` in `libs/sdk` is absent).
  - Swaps the stack slots (`0x14(r1)` vs `0x8(r1)`) of the case 5/7 `TVec3f` temporaries passed to `cls_8031709C::fn_80316E50`.

## Data & Symbols
- `.rodata` (100%): `lbl_804715B8`, `lbl_804715DC`, `lbl_804715F8`, `lbl_8047160C` (`Step_804AADC8` cel/frame tables) and `"enemy_both2_sob.bin"`.
- `.sdata2` (100%): `65536.0f` (`lbl_805418C0`, from `selectNearestPlayer` defined below `draw`) followed by `draw`, `update`, and `init` literals.
- `symbols.txt`: `fn_80317378` -> `update__12cls_804AADD8Fv` (vtable `0x804AADD8` slot `0x14`).
- `include/game/act/act_tingle_first.h`: `0x260` `JGeometry::TBox2f field_0x260`; class size `0x274` (`__nw__(0x274)` in `tu_801F9484.s` before `__ct__12cls_804AADD8Fv`).

## SHARED-EDITS
- None (`act_tingle_first.h` is only included by `act_tingle_first.cpp` and `tu_801F9484.cpp`).
