# Checkpoint: game/act_cheep_cheep.cpp

Baseline: `total: 9046 / 11557 functions, 1881952 / 4442032 bytes of code (42.37 %)`

## Matched (100%)
- `CheepCheep_804A11B8::~CheepCheep_804A11B8()` (0x8017B140)
- `CheepCheep_804A11B8::draw()` (0x8017B1A0)
- `CheepCheep_804A11B8::vfunc_0x4C(s32, const JGeometry::TVec3f*)` (0x8017BBA8)
- `__sinit_act_cheep_cheep_cpp` (0x8017BDF8)

## Resistant
- `CheepCheep_804A11B8::init()` (0x8017BBFC): 98.19% (`scheduling+regalloc`). The 0x19660d constant and `li 0` use swapped registers (`r5`/`r6`), and `fcmpu cr0, f1, f0` loads `0.0f` into `f1` instead of `f0`.
- `CheepCheep_804A11B8::update()` (0x8017B3B4): 98.81% (`different`, 2 opcodes). `fcmpu cr0, f1, f0` loads `0.0f` into `f1` and `hitVec.y` into `f0` (swapped vs. target), plus float register/stack slot swaps in case 1 and `std::sqrtf` and argument load scheduling in `fn_801F6758`.

## SHARED-EDITS
- `config/G4SE01/gen_game.tsv`: removed `act_cheep_cheep` generator rows (`dtor` and `sinit`) because `src/game/act_cheep_cheep.cpp` and `include/game/act/act_cheep_cheep.h` are now hand-written.

## Next
- Investigate if `fcmpu cr0, f1, f0` operand order in `init()` and `update()` can be matched naturally, and whether `get_ufloat_1()` float register/stack slot differences in `update()` can be resolved.
