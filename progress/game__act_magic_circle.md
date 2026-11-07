# `game/act_magic_circle.cpp`

## Status

- `__dt__20MagicCircle_804A7960Fv`: 100.00% (116 bytes)
- `init__20MagicCircle_804A7960Fv`: 99.89% (700 bytes, `regalloc-only`)
- `vfunc_0x10__20MagicCircle_804A7960Fv`: 100.00% (184 bytes)
- `fn_8028B850__20MagicCircle_804A7960Fv`: 100.00% (524 bytes)
- `update__20MagicCircle_804A7960Fv`: 99.81% (2596 bytes, `scheduling+regalloc`)
- `draw__20MagicCircle_804A7960Fv`: 93.63% (688 bytes, `scheduling+regalloc`)
- `__sinit_act_magic_circle_cpp`: 100.00% (548 bytes)
- Sections `.ctors`, `.rodata`, `.data`, `.bss`, `.sdata2`: 100.00%

## Evidence & Proven Layout

- `MagicCircle_804A7960` (`sizeof == 0x27C`, from `spawnActor_801F9484`'s `operator new(0x27C)` for `'CIRC'` at `0x801FBDD4`):
  - `0x230`: `s32 field_0x230` (timer up to 600)
  - `0x234`: `bool field_0x234` (active state; `fn_8028B850` returns `bool`, stored with `stb` and tested without `clrlwi` in `update`)
  - `0x238`: `s32 field_0x238` (post-warp cooldown timer, 180 frames)
  - `0x23C`: `s32 field_0x23c` (warp use count)
  - `0x240`: `f32 field_0x240` (appear/disappear scale in `[0.0f, 1.0f]`)
  - `0x244`: `s32 field_0x244` (frame counter in `[0, 0x6E]` written to `GameRootSub20_801902C0::MagicCircle::field_0x10`)
  - `0x248`: `s32 field_0x248` (startup delay timer, 120 in `init`, 10 when reaching scale 1.0f)
  - `0x24C`: `s32 field_0x24c` (activation mode `(field_0x90 >> 4) & 0xF`)
  - `0x250`: `u8 field_0x250`
  - `0x251`: `u8 field_0x251`
  - `0x252`: `u8 field_0x252[4]` (per-player overlap latch)
  - `0x258`: `s32 field_0x258`
  - `0x25C`: `s32 field_0x25c` (circle color/type `field_0x90 & 7`)
  - `0x260`: `u8 field_0x260` (`tu_802242F4.cpp` reads `0x262` via `lbz` at `0x80227060`)
  - `0x261`: `u8 field_0x261`
  - `0x262`: `u8 field_0x262`
  - `0x263`: `u8 field_0x263`
  - `0x264`: `u8 field_0x264`
  - `0x268`: `JGeometry::TVec3f field_0x268` (warp target position)
  - `0x274`: `JPABaseEmitter* mEmitter274` (emitter `0x2A6`, released in dtor)
  - `0x278`: `JPABaseEmitter* mEmitter278` (emitter `0x2A7`, released in dtor)
- Symbol rename in `config/G4SE01/symbols.txt`:
  - `0x8028B850`: `fn_8028B850` -> `fn_8028B850__20MagicCircle_804A7960Fv` (takes `MagicCircle_804A7960*` in `r3`, called by `vfunc_0x10` at `0x8028BAA0` and `update` at `0x8028AE58`).

## SHARED-EDITS

- `include/game/GameRootSub20.h`:
  - In `GameRootSub20_801902C0::MagicCircle`, replaced `/* 0x0C */ u8 field_0xc[8];` with `/* 0x0C */ s32 field_0xc;` and `/* 0x10 */ f32 field_0x10;` (proven by `MagicCircle_804A7960::update` at `0x8028AFF0-0x8028B028` and `0x8028B340-0x8028B380` storing color index as `stw` at `+0x0C` and frame converted to `f32` via `stfs` at `+0x10`, and `GameRootSub20_801902C0::fn_8018DE2C` reading `+0x0C` with `lwz` and `+0x10` with `lfs`).
- `config/G4SE01/gen_game.tsv`:
  - Removed the two `act_magic_circle` rows (`dtor_emit` and `sinit`) because `src/game/act_magic_circle.cpp` and `include/game/act/act_magic_circle.h` are now hand-maintained (`tools/gen_game.py --check`).
