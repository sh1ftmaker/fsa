# game/act_bee_manager.cpp

## Status
- 5 / 6 functions at 100.00% (`__sinit_act_bee_manager_cpp`, `~BeeManager_804AC810`, `init`, `draw`, `fn_80349914`)
- 1 / 6 function sub-100%: `BeeManager_804AC810::update` at 95.79% (`scheduling+regalloc`)

## Proven layout & symbols
- Class `BeeManager_804AC810` (`0x284` bytes, vtable `0x804AC810`, size `0x54`, inherits `ActorBase_804A5770`), spawned for `'BEMN'`:
  - `/* 0x230 */ s32 field_0x230;` (state 0..4)
  - `/* 0x234 */ s32 field_0x234;` (countdown timer)
  - `/* 0x238 */ s32 field_0x238;` (target player index, init -1)
  - `/* 0x23C */ s32 field_0x23c;` (saved player index, init -1)
  - `/* 0x240 */ s32 field_0x240[14];` (serials of 14 spawned `'FOBE'` bee actors, init -1)
  - `/* 0x278 */ s32 field_0x278;` (`field_0x90 & 0xF`)
  - `/* 0x27C */ s32 field_0x27c;` (player index set by `BeeFormation_804AC7B8::update`)
  - `/* 0x280 */ s16 field_0x280;` (angle stepped by `0x222`)
  - `/* 0x282 */ u8 field_0x282;` (retreat flag)
- Symbol renames in `config/G4SE01/symbols.txt`:
  - `fn_80349914` -> `fn_80349914__19BeeManager_804AC810FPQ29JGeometry8TVec3<f>` (non-virtual helper called by `update` at `0x8034A1F4`)
  - `fn_80349B14` -> `draw__19BeeManager_804AC810Fv` (vtable slot `0x20`)
  - `fn_80349B18` -> `update__19BeeManager_804AC810Fv` (vtable slot `0x14`)
  - `fn_8034A380` -> `init__19BeeManager_804AC810Fv` (vtable slot `0x0C`)
  - `fn_8034A3E8` -> `__dt__19BeeManager_804AC810Fv` (vtable slot `0x08`)
  - `__vt__19BeeManager_804AC810` resized from `0x58` to `0x54` (actual vtable size excluding 4 bytes of trailing section alignment padding)

## Shared edits
- `include/game/act/act_bee_manager.h`: new header for `BeeManager_804AC810`.
- `include/game/act/tu_801F9484.h`: include `game/act/act_bee_manager.h` in place of the stub `BeeManager_804AC810` definition.
- `include/game/act/tu_803489A4.h`: typed fields `0x230..0x2A4` of `BeeFormation_804AC7B8` (proven from `tu_803489A4.s` `init`/`update`).
- `config/G4SE01/gen_game.tsv`: removed generator rows for `act_bee_manager` and `tu_803489A4`.
