# game/act_buzz_blob

- **Unit**: `src/game/act_buzz_blob.cpp` (`include/game/act/act_buzz_blob.h`)
- **Status**: NonMatching (3/5 functions at 100%, 2/5 >99% regalloc-only; `.text` 99.72%, `.rodata` 100%, `.data` 100%)
- **Before**: 2/5 functions matched (`__dt__17BuzzBlob_8049F444Fv`, `__sinit_act_buzz_blob_cpp`)
- **After**: 3/5 functions at 100% (`init__17BuzzBlob_8049F444Fv`, `__dt__17BuzzBlob_8049F444Fv`, `__sinit_act_buzz_blob_cpp`), 2 functions >99% `regalloc-only`

## Functions

| Address | Symbol | Size | Match | Notes |
|---|---|---|---|---|
| `0x8013DE44` | `draw__17BuzzBlob_8049F444Fv` | `0x160` | 99.32% | `regalloc-only`: `f26`..`f31` in `setRotScaleMtx` and `f0`/`f1` in `mPos.z != 0.0f` |
| `0x8013DFA4` | `update__17BuzzBlob_8049F444Fv` | `0x78C` | 99.82% | `regalloc-only`: `r4`/`r5` swapped in `mRandom` loads after `dir.scale(0.3f)` in case 0 |
| `0x8013E730` | `init__17BuzzBlob_8049F444Fv` | `0xAC` | 100.0% | Matched |
| `0x8013E7DC` | `__dt__17BuzzBlob_8049F444Fv` | `0x5C` | 100.0% | Matched |
| `0x8013E838` | `__sinit_act_buzz_blob_cpp` | `0x28` | 100.0% | Matched |

## Class Layout (`BuzzBlob_8049F444`, size `0x264`)

- `0x000`..`0x230`: `ActorBase_804A5770`
- `0x230`: `s32 field_0x230` (state: 0 = wander/idle, 1 = prepare jump, 2 = jumping, 3 = shocked/electrified, 4 =pit fall)
- `0x234`: `s32 field_0x234` (walk animation timer)
- `0x238`: `s32 field_0x238` (wander direction timer)
- `0x23C`: `s32 field_0x23c` (shock animation timer)
- `0x240`: `s32 field_0x240` (jump windup / cooldown timer)
- `0x244`: `s32 field_0x244` (frame table index into `lbl_8049F408` / `lbl_8049F418`)
- `0x248`: `s32 field_0x248`
- `0x24C`: `s32 field_0x24c`
- `0x250`: `f32 field_0x250` (pit fall scale)
- `0x254`: `f32 field_0x254` (pit fall Y rotation angle)
- `0x258`: `JGeometry::TVec3f field_0x258` (sprite scale vector)

## SHARED-EDITS
- Removed `act_buzz_blob` rows from `config/G4SE01/gen_game.tsv` (unit is now hand-written).
- None needed in shared headers.
