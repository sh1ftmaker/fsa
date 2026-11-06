# game/act_lotus

## Baseline
- `total: 9020 / 11557 functions, 1864324 / 4442032 bytes of code (41.97 %)`
- `game/act_lotus`: 2/5 functions matched (`__dt__14Lotus_804AC330Fv`, `__sinit_act_lotus_cpp`).

## Matched (3/5)
- `init__14Lotus_804AC330Fv` (`0x8033F0E8`, size `0x13c`): 100.00%
- `__dt__14Lotus_804AC330Fv` (`0x8033E638`, size `0x60`): 100.00%
- `__sinit_act_lotus_cpp` (`0x8033F224`, size `0xc`): 100.00%

## Resistant (2/5, merged above bar)
- `update__14Lotus_804AC330Fv` (`0x8033EA54`, size `0x694`): 97.17% (`missing/extra`: 422 vs 421 instrs).
  - Register allocation in the player loop copies `i` into `r26` (1 extra `mr`), and `contains()` loads `mBox.i.y`/`f.x`/`f.y` per branch instead of upfront.
- `draw__14Lotus_804AC330Fv` (`0x8033E698`, size `0x3bc`): 98.22% (`regalloc-only`: `target->ours f29->f31, f31->f26, f3->f2, f28->f26`).
  - Float register allocation inside `setRotScaleMtx` (same regalloc permutation as `act_hardhat_beetle.cpp`).

## Data sections
- `.rodata`, `.data`, `.bss`, `.ctors`, `.sdata2`: 100.00%

## Class layout (`Lotus_804AC330`, size `0x264`)
- `0x230`: `s32 field_0x230;` (state: 0 = idle, 1 = moving, 2 = appearing/disappearing)
- `0x234`: `JGeometry::TVec3f field_0x234;` (offset from spawn pos; `y` bobbing wave)
- `0x240`: `f32 field_0x240;` (scale, 0.0f..1.0f)
- `0x244`: `s32 field_0x244;` (`field_0x90 & 0xF`, size variant 0..2)
- `0x248`: `s32 field_0x248;` (`(field_0x90 >> 4) & 0xF`, speed)
- `0x24C`: `s32 field_0x24c;` (cooldown timer)
- `0x250`: `s32 field_0x250;` (`(field_0x90 >> 8) & 0xF`, initial direction)
- `0x254`: `s32 field_0x254;` (current direction 0..3)
- `0x258`: `s32 field_0x258;` (`(field_0x90 >> 12) & 0xFF`, travel distance in 16px tiles)
- `0x25C`: `u16 field_0x25c;` (bobbing angle, `+= 0x800`)
- `0x25E`: `u8 field_0x25e[4];` (per-player touching flag)
- `0x262`: `u8 field_0x262;` (bobbing active flag)

## SHARED-EDITS
- `config/G4SE01/gen_game.tsv`: Removed the two `act_lotus` rows (`dtor` and `sinit`) as `src/game/act_lotus.cpp` and `include/game/act/act_lotus.h` are now hand-maintained.
