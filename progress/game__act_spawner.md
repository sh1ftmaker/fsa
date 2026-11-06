# game/act_spawner

Baseline: total 8976 / 11557 functions, 1828300 / 4442032 bytes of code (41.16 %).

Matched:
- SpawnerGenerator_804A9328::~SpawnerGenerator_804A9328() (100 %)
- __sinit_act_spawner_cpp (100 %)
- SpawnerGenerator_804A9328::draw() (100 %)
- SpawnerGenerator_804A9328::init() (100 %)

Resistant:
- SpawnerGenerator_804A9328::update() (99.96 %, regalloc-only). 1440 / 1444 bytes. Only 3 instructions differ: flag allocated to r29 instead of target's r28 at `li r28, -0x1`, `li r28, 0x1`, and `cmpwi r28, 0x0`.

SHARED-EDITS
- include/game/act/act_spawner.h: Declared typed members field_0x230 to field_0x674, matching sizeof(SpawnerGenerator_804A9328) == 0x678.
- config/G4SE01/gen_game.tsv: Deleted act_spawner rows for dtor and sinit.
- config/G4SE01/symbols.txt: Split placeholder .rodata lbl_8046DFD0 (0x70) into lbl_8046DFD0 (0x10), lbl_8046DFE0 (0x50), and lbl_8046E030 (0x10).
