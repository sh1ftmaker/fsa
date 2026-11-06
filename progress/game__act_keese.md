# game/act_keese.cpp

## Baseline
- `total: 8960 / 11557 functions, 1821976 / 4442032 bytes of code (41.02 %)`
- `Game code 4247 / 6789 functions 27.26 %`
- `game/act_keese`: 2/6 functions matched (`__dt__14Keese_804A02B8Fv`, `__sinit_act_keese_cpp`)
- `donor_leads.py game/act_keese.cpp`: `NO LEAD`

## Class Layout (`Keese_804A02B8`, size `0x28C`)
- Base: `ActorBase_804A5770` (`0x230` bytes), vtable at `0x804A02B8` (`0x54` bytes)
- `0x230`: `s32 field_0x230` (state: 0..3)
- `0x234`: `s32 field_0x234` (animation frame timer)
- `0x238`: `s32 field_0x238` (cooldown timer)
- `0x23C`: `s32 field_0x23c` (state 2 timer)
- `0x240`: `s32 field_0x240`
- `0x244`: `s32 field_0x244` (target angle / orbit angle)
- `0x248`: `s32 field_0x248` (state 2 dive phase flag)
- `0x24C`: `s32 field_0x24c` (axis flag in mode 3)
- `0x250`: `s32 field_0x250` (oscillation angle in mode 3)
- `0x254`: `s32 field_0x254` (oscillation step in mode 3)
- `0x258`: `s32 field_0x258` (sprite frame offset 0..2)
- `0x25C`: `u16 field_0x25c` (sprite rotation angle)
- `0x25E`: `u8 field_0x25e` (ceiling/perched flag from `field_0x90 & 0x100`)
- `0x260`: `s32 field_0x260` (large scale flag `(field_0x90 >> 4) & 1`)
- `0x264`: `s32 field_0x264` (behavior mode `field_0x90 & 0xF`)
- `0x268`: `u16 field_0x268` (flight direction angle)
- `0x26A`: `u16 field_0x26a` (saved tile code in mode 1, `0xFFFF` initially)
- `0x26C`: `f32 field_0x26c` (target y / orbit radius)
- `0x270`: `f32 field_0x270` (target orbit radius)
- `0x274`: `f32 field_0x274` (target velocity x)
- `0x278`: `f32 field_0x278` (target velocity y)
- `0x27C`: `f32 field_0x27c` (target velocity z)
- `0x280`: `JGeometry::TVec3f field_0x280` (initial position copy)

## Functions
- `__dt__14Keese_804A02B8Fv` (0x8015C860): 100%
- `vfunc_0x4C__14Keese_804A02B8FlPCQ29JGeometry8TVec3<f>` (0x8015C8BC): 100%
- `draw__14Keese_804A02B8Fv` (0x8015C8DC): 98.70% (`regalloc-only`: `setRotScaleMtx` saved-FPR assignment `f30->f31, f31->f27, f29->f27, f27->f31` across the `sin`/`cos` calls)
- `update__14Keese_804A02B8Fv` (0x8015CD18): 100%
- `init__14Keese_804A02B8Fv` (0x8015D844): 99.36% (`regalloc-only`: `gGameRoot_8053AAF8` pointer and `0x19660D` LCG multiplier swap `r5`/`r6`)
- `__sinit_act_keese_cpp` (0x8015DBEC): 100%

## SHARED-EDITS
- `include/game/act/act_keese.h`: `Keese_804A02B8` member layout (`0x230`..`0x28C`)
- `config/G4SE01/gen_game.tsv`: removed `act_keese` rows after taking ownership of `src/game/act_keese.cpp` and `include/game/act/act_keese.h`
