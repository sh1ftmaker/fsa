# game/act_force_gem_buried.cpp

Baseline: `total: 9020 / 11557 functions, 1864324 / 4442032 bytes of code (41.97 %)`
After: `total: 9023 / 11557 functions, 1867284 / 4442032 bytes of code (42.04 %)` (+3 functions, 5/5 at 100%, flipped to `Matching`).

## Matched functions (5/5, 100%)
- `BuriedForceGem_804AB258::init()` (0x8031F97C, 0x1C0 bytes): 100%
- `BuriedForceGem_804AB258::update()` (0x8031F190, 0x7EC bytes): 100%
- `BuriedForceGem_804AB258::draw()` (0x8031EFAC, 0x1E4 bytes): 100%
- `BuriedForceGem_804AB258::~BuriedForceGem_804AB258()` (0x8031EF4C, 0x60 bytes): 100%
- `__sinit_act_force_gem_buried_cpp` (0x8031FB3C, 0x224 bytes): 100%

## Data sections (all 100%)
- `.rodata`: `lbl_80471740` (`{8, 4, 0}`) and `"enemy_both_sob.bin"`
- `.data`: `lbl_804AB248` (`{0x114, 0x117, 0x11A, 0x11D}`) and `__vt__23BuriedForceGem_804AB258`
- `.sdata2`, `.bss`, `.ctors`, `extab`, `extabindex`: 100% (verified byte-identical link)

## SHARED-EDITS
- `config/G4SE01/gen_game.tsv`: removed the 2 generator rows for `act_force_gem_buried` (`dtor` and `sinit`) because `src/game/act_force_gem_buried.cpp` and `include/game/act/act_force_gem_buried.h` are now hand-written and `Matching`.
- `include/game/act/act_force_gem_buried.h` (unit-only header): carved `u8 field_0x230[0x40]` into proven fields `field_0x230`..`field_0x26c` (`sizeof(BuriedForceGem_804AB258) == 0x270`).
