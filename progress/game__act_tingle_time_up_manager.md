# game/act_tingle_time_up_manager.cpp

- Unit: `game/act_tingle_time_up_manager.cpp` (`0x8035280C..0x803536C4`)
- Actor: `TingleTimeUpManager_804ACBF0` (`ACTOR_TUTM`, size `0x294`, vtable `0x804ACBF0`)

## Functions

| Address | Function | Size | Match | Notes |
|---|---|---|---|---|
| `0x8035280C` | `TingleTimeUpManager_804ACBF0::~TingleTimeUpManager_804ACBF0()` | `0x60` | 100.0 % | matched |
| `0x8035286C` | `TingleTimeUpManager_804ACBF0::draw()` | `0x4` | 100.0 % | matched (empty `blr`) |
| `0x80352870` | `TingleTimeUpManager_804ACBF0::update()` | `0xAEC` | 91.2 % | `missing/extra + reloc-name` (698 vs 699 instrs): `this` in r30 vs r28, state 3/4 `TVec3f` ctor scheduling around `__cvt_fp2unsigned`, state 2 loop init 1 instr shorter |
| `0x8035335C` | `TingleTimeUpManager_804ACBF0::init()` | `0x144` | 99.8 % | `scheduling`: epilogue reloads `r0` and `r31` in the opposite order |
| `0x803534A0` | `__sinit_act_tingle_time_up_manager_cpp` | `0x224` | 100.0 % | matched |

## Class Layout (`TingleTimeUpManager_804ACBF0`, size `0x294`)

- `0x230`: `s32 mState` — state machine (0..7)
- `0x234`: `s32 mTimer` — countdown timer, decremented at the end of `update()` when `> 0`
- `0x238`: `s32 field_0x238` — spawned `TUTL` count in state 5
- `0x23C`: `u32 field_0x23c` — initial RGBA colour word from `fn_8018330C()`
- `0x240`: `u32 field_0x240` — saved RGBA colour word restored in states 6 and 7
- `0x244..0x250`: `f32 field_0x244`, `field_0x248`, `field_0x24c`, `field_0x250` — current RGBA channels as floats
- `0x254..0x25C`: `f32 field_0x254`, `field_0x258`, `field_0x25c` — per-frame RGB steps
- `0x260`: `f32 field_0x260` — `field_0x264 / field_0x268`
- `0x264`: `f32 field_0x264` — `field_0x288.y - field_0x27c.y`
- `0x268`: `f32 field_0x268` — `mViewBox.f.y - mViewBox.i.y`
- `0x26C`: `JGeometry::TBox2f mViewBox` — copy of `getGameView_802D6A9C()->mVisibleBox`
- `0x27C`: `JGeometry::TVec3f field_0x27c` — first vector written by `Camera_804AA9E4::fn_8030C380`
- `0x288`: `JGeometry::TVec3f field_0x288` — second vector written by `Camera_804AA9E4::fn_8030C380`

## SHARED-EDITS

- `config/G4SE01/gen_game.tsv`: removed generator rows for `act_tingle_time_up_manager`, `act_force_gem_buried`, `act_force_gem_fleeing`, and `act_force_gem_winged`.
- `config/G4SE01/symbols.txt`: renamed placeholder `fn_8030C380` (`0x8030C380`) to `fn_8030C380__15Camera_804AA9E4FPQ29JGeometry8TVec3<f>`.
- `include/game/Camera.h`: declared `void fn_8030C380(JGeometry::TVec3f* out);` on `Camera_804AA9E4`.
- `include/game/act/tu_803536C4.h`: `0x254` is now a `JGeometry::TVec3f field_0x254` and `0x260` a `JGeometry::TVec3f field_0x260` on `cls_804ACC68` (`TUTL`), copied by `TingleTimeUpManager_804ACBF0::update()` (`0x80352A20..0x80352A3C`); `tu_803536C4.cpp`'s one read is now `field_0x254.x`.
- `include/game/act/act_force_gem.h`: changed `ForceGem_804AB6D8::field_0x230` from `u32` to `s32` (`cmpwi` at `0x80352D88`).
- `include/game/act/act_force_gem_fleeing.h`: added `/* 0x230 */ s32 field_0x230;` to `FleeingForceGem_804AB170` (`lwz`/`cmpwi` at `0x80352DCC`).
- `include/game/act/act_force_gem_winged.h`: added `/* 0x230 */ s32 field_0x230;` and `/* 0x281 */ u8 field_0x281;` to `WingedForceGem_804AB098` (`lwz`/`lbz` at `0x80352E00`, `0x80352E1C`).
- `include/game/act/act_force_gem_buried.h`: added `/* 0x230 */ s32 field_0x230;` to `BuriedForceGem_804AB258` (`lwz`/`cmpwi` at `0x80352E50`).
