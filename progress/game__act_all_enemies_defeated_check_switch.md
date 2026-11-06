# game/act_all_enemies_defeated_check_switch.cpp (ZNSW, AllEnemiesDefeatedCheckSwitch_804A8AC8)

Baseline (progress.py first line at start): total: 9020 / 11557 functions, 1864324 / 4442032 bytes of code (41.97 %)
After: total: 9024 / 11557 functions, 1866460 / 4442032 bytes of code (42.02 %)

Status: Matching. 6/6 functions (`init`, `fn_802BDCC0`, `update`, `draw`, `~AllEnemiesDefeatedCheckSwitch_804A8AC8`, `__sinit_act_all_enemies_defeated_check_switch_cpp`), .text/.ctors/.data/.bss/.sdata2 100 %.

## Proved
- `AllEnemiesDefeatedCheckSwitch_804A8AC8` layout (`0x230..0x260`, size `0x260`):
  - `0x230`: `s32 field_0x230` (state, 0 or 1)
  - `0x234`: `s32 field_0x234` (`field_0x90 & 0x1F`, mode 0..9)
  - `0x238`: `s32 field_0x238` (`(field_0x90 >> 7) & 0x1F`)
  - `0x23C`: `JGeometry::TBox2f field_0x23c` (check area box)
  - `0x24C`: `u32 field_0x24c` (saved `gAudioMgr_8053AF80->field_0x58`)
  - `0x250`: `u8 field_0x250` (initialized flag)
  - `0x254`: `s32 field_0x254` (`(field_0x90 >> 12) & 0x1F`)
  - `0x258`: `s32 field_0x258` (`((field_0x90 >> 22) & 0x1F) + 1`, half-width in 8-unit tiles)
  - `0x25C`: `s32 field_0x25c` (`((field_0x90 >> 17) & 0x1F) + 1`, half-height in 8-unit tiles)
- `fn_802BDCC0` is `AllEnemiesDefeatedCheckSwitch_804A8AC8::fn_802BDCC0()`, called by `init()` in modes 4, 5, 8, 9 to compute `field_0x23c` from `layerBox(field_0x4)` on layers `< 8` or by scanning tile walls via `fn_80226850` on layers `>= 8`.

## SHARED-EDITS
- `config/G4SE01/symbols.txt`: `fn_802BDCC0` -> `fn_802BDCC0__38AllEnemiesDefeatedCheckSwitch_804A8AC8Fv`.
- `config/G4SE01/gen_game.tsv`: removed both `act_all_enemies_defeated_check_switch` rows.
