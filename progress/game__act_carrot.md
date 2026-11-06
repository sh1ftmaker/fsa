# Checkpoint: game/act_carrot.cpp

## Baseline
`total: 9046 / 11557 functions, 1881952 / 4442032 bytes of code (42.37 %)`

## Matched (5/6 functions, 100 % data)
- `Carrot_804AC6B0::init` (0x80347B20): 100 %
- `Carrot_804AC6B0::vfunc_0x4C` (0x803477DC): 100 %
- `Carrot_804AC6B0::update` (0x803471A0): 100 %
- `Carrot_804AC6B0::~Carrot_804AC6B0` (0x80346E24): 100 % (pre-existing)
- `__sinit_act_carrot_cpp` (0x80347C6C): 100 % (pre-existing)
- `.rodata` (`lbl_80472010`, `"enemy_both2_sob.bin"`), `.data`, `.bss`, `.sdata2`, `.ctors`: 100 %

## Resistant (1/6 functions)
- `Carrot_804AC6B0::draw` (0x80346E84): 99.05 % (`regalloc-only`). Same `setRotScaleMtx` saved-FPR assignment difference across the `sin`/`cos` calls as `act_hardhat_beetle::draw` (`f29->f31`, `f28->f26`, `f26->f28`, `f31->f29`).

```
  15C  lfs f30, 0x268(r31)                | lfs f30, 0x268(r31)
> 160  lfs f29, 0x26c(r31)                | lfs f31, 0x26c(r31)                arg_mismatch
  164  lfs f1, lbl_80542438@sda21         | lfs f1, @876@sda21
  168  bl sin                             | bl sin
> 16C  frsp f28, f1                       | frsp f26, f1                       arg_mismatch
```

## SHARED-EDITS
- `config/G4SE01/gen_game.tsv`: removed `act_carrot` (`dtor` and `sinit`) rows as `src/game/act_carrot.cpp` and `include/game/act/act_carrot.h` are now hand-written.

## Next
- Resolve the `setRotScaleMtx` saved-FPR register allocation across all 19 units using it to flip `draw` to 100 % and mark `game/act_carrot.cpp` `Matching`.
