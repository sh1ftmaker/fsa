# game/act_four_person_pull_switch_pswk

Baseline: total 9000 / 11557 functions, 1846220 / 4442032 bytes (41.56 %).
After: 9003 / 11557 functions, 1848408 / 4442032 bytes (41.61 %).

## Matched (100 %)
- `__dt__41FourPersonPullSwitchChainVersion_804AE5C8Fv` (`0x80373BD8`, 96 bytes)
- `fn_80373C38__41FourPersonPullSwitchChainVersion_804AE5C8Fv` (`0x80373C38`, 656 bytes)
- `draw__41FourPersonPullSwitchChainVersion_804AE5C8Fv` (`0x80373EC8`, 428 bytes)
- `update__41FourPersonPullSwitchChainVersion_804AE5C8Fv` (`0x80374074`, 1104 bytes)
- `__sinit_act_four_person_pull_switch_pswk_cpp` (`0x80374564`, 548 bytes)

## Resistant
- `init__41FourPersonPullSwitchChainVersion_804AE5C8Fv` (`0x803744C4`, 160 bytes): 99.70 % (`scheduling`: epilogue restores `r0` and `r31` in swapped order: `lwz r31, 0xc(r1); lwz r0, 0x14(r1)` vs `lwz r0, 0x14(r1); lwz r31, 0xc(r1)`).

## Sections
- `.text`: 99.98 %
- `.ctors`, `.rodata`, `.data`, `.bss`, `.sdata2`: 100.00 %

## Proven Names & Layout
- `FourPersonPullSwitchChainVersion_804AE5C8` (vtable `0x804AE5C8`, size `0x258`, constructed by `fn_801FFB00` in actor spawner for `'PSWK'`):
  - `0x230`: `JGeometry::TVec3f field_0x230`
  - `0x23C`: `s32 field_0x23c`
  - `0x240`: `u8 field_0x240`
  - `0x244`: `f32 field_0x244`
  - `0x248`: `s32 field_0x248`
  - `0x24C`: `s32 field_0x24c`
  - `0x250`: `s32 field_0x250`
  - `0x254`: `u8 field_0x254`
- `fn_80373C38__41FourPersonPullSwitchChainVersion_804AE5C8Fv`: member helper called by `FourPersonPullSwitchChainVersion_804AE5C8::update()` with `this` in `r3`.

## SHARED-EDITS
- `config/G4SE01/gen_game.tsv`: removed the two `act_four_person_pull_switch_pswk` rows (`dtor` and `sinit`) in the same commit as writing the `.cpp`.
