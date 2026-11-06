# game/act_rainbow_bridge.cpp

## Baseline
- `total: 9000 / 11557 functions, 1846220 / 4442032 bytes of code (41.56 %)`
- `game/act_rainbow_bridge`: 1/5 functions matched (`__sinit_act_rainbow_bridge_cpp`)

## Current Progress
- `game/act_rainbow_bridge`: 2/5 functions at 100%, 3/5 at 87.42%–99.78% (`.text` 98.29%)
  - `cls_8049E610::cls_8049E610()` (`0x8012A364`): 100%
  - `__sinit_act_rainbow_bridge_cpp` (`0x8012A3C8`): 100%
  - `cls_8049E610::init()` (`0x8012A008`): 99.78% (`reloc-name`: `1.0f` `.sdata2` literal offset)
  - `cls_8049E610::update()` (`0x80129B0C`): 97.69% (`missing/extra + reloc-name`: 1 more instruction from `r26`/`r27` register allocation in the player loop, plus `.sdata2` literal offsets)
  - `cls_8049E610::~cls_8049E610()` (`0x8012A2E0`): 87.42% (`different`: same length, `r29`/`r30` register swap in the emitter loop)

## Class & Symbol Evidence
- `cls_8049E610` (`include/game/act/act_rainbow_bridge.h`, included from `include/game/act/tu_801F9484.h`):
  - Spawned for `'RNBW'` ("Rainbow Bridge", `docs/names/actors.tsv`, high confidence).
  - Vtable `__vt__12cls_8049E610` at `0x8049E610` overrides slot `0x08` (`~cls_8049E610`, `0x8012A2E0`), slot `0x0C` (`init`, `0x8012A008`), and slot `0x14` (`update`, `0x80129B0C`).
  - Renamed in `config/G4SE01/symbols.txt`:
    - `fn_80129B0C` -> `update__12cls_8049E610Fv`
    - `fn_8012A008` -> `init__12cls_8049E610Fv`
    - `fn_8012A2E0` -> `__dt__12cls_8049E610Fv`
- Removed `act_rainbow_bridge` row from `config/G4SE01/gen_game.tsv` now that `src/game/act_rainbow_bridge.cpp` is hand-written.
