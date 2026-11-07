# game/act_stalfos.cpp

## Status
- `init__16Stalfos_804A0CD8Fv` (`0x80176538`): 100.00%
- `update__16Stalfos_804A0CD8Fv` (`0x801757D4`): 99.70% (`scheduling+regalloc` in `try.py`)
- `draw__16Stalfos_804A0CD8Fv` (`0x801756E4`): 100.00%
- `__dt__16Stalfos_804A0CD8Fv` (`0x80175684`): 100.00%
- `__sinit_act_stalfos_cpp` (`0x80176780`): 100.00%
- `.data`, `.rodata`, `.sdata2`, `.ctors`, `.bss`: 100.00%

## Proven
- `Stalfos_804A0CD8` size `0x284` (from `factory_Stalfos` allocation in `tu_8021656C.cpp`).
- Split `lbl_804A0C10` (`0xC8` bytes in `.data`) into 4 animation frame/duration tables (`lbl_804A0C10`, `lbl_804A0C70`, `lbl_804A0C88`, `lbl_804A0CC8`).
- Removed `act_stalfos` `dtor` and `sinit` rows from `config/G4SE01/gen_game.tsv`.
