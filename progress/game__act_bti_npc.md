# game/act_bti_npc.cpp

## Status
- 4 / 5 functions at 100 % (`init__15BTINPC_804A927CFv`, `update__15BTINPC_804A927CFv`, `__dt__15BTINPC_804A927CFv`, `__sinit_act_bti_npc_cpp`)
- 1 function sub-100 %:
  - `draw__15BTINPC_804A927CFv`: 91.39 % (`missing/extra + reloc-name`, 490 vs 487 instructions; the original hoists `lbl_804A90F0`'s base into `r29` and schedules `setRotScaleMtxS16`'s `sincosTable_` products differently)
- Sections at 100 %: `.data`, `.rodata`, `.sdata2`, `.bss`, `.ctors`

## Proven
- `BTINPC_804A927C` layout (`0x230`..`0x25C`, size `0x25C`):
  - `0x230`: `s32 field_0x230` (state: 0 = hidden, 1 = visible, 2 = done)
  - `0x234`: `s32 field_0x234` (`field_0x90 & 0xFFF`, BTI index into `lbl_804A90F0`)
  - `0x238`: `s32 field_0x238` (`(field_0x90 >> 16) & 0xF`, x-offset index into `lbl_80540E28`)
  - `0x23C`: `s32 field_0x23c` (`(field_0x90 >> 12) & 0xF`, y-offset index into `lbl_80540E28`)
  - `0x240`: `s32 field_0x240` (`(field_0x90 >> 20) & 0xF`, mode)
  - `0x244`: `s32 field_0x244` (`(field_0x90 >> 25) & 1`, layer offset flag)
  - `0x248`: `s16 field_0x248` (z-rotation angle for BTI indices `0x1B`..`0x1E`)
  - `0x24C`: `f32 field_0x24c` (scaleX)
  - `0x250`: `f32 field_0x250` (scaleY)
  - `0x254`: `f32 field_0x254` (scaleZ)
  - `0x258`: `u8 field_0x258` (smoke/sound effect flag from `field_0x90 & 0x4000000`)
  - `0x259`: `u8 field_0x259` (layer offset `0` or `1` from `field_0x244 != 0`)
- Split `.rodata` string literals at `0x8046DF10`..`0x8046DFB8` (`lbl_8046DF10`..`lbl_8046DFA4`) and named `extab`/`extabindex` entries (`@895`/`@896`, `@1056`/`@1057`, `@1089`/`@1090`, `@1117`/`@1118`, `@1183`/`@1184`) in `config/G4SE01/symbols.txt`.
