# JSystem/J2DGraph/J2DMaterialFactory.cpp

Baseline (progress.py first line): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: unchanged (no new matches). Unit stays NonMatching: 38/40, .text 99.67 %, data 100 %.

## Resistant (both blocked by codegen, not by another unit)

### __ct__11J2DTevStageFRC15J2DTevStageInfo (87.08 %), weak, from J2DTevStage.h
Same miss as J2DBloSaver's `__ct__11J2DTevStageFv` (88.08 %): inlined `setTevSwapModeInfo(j2dDefaultTevSwapMode)`.
```
1C  lbz r4, 0x7(r31)          | lbz r0, 0x7(r31)
20  lbz r0, 0x1(r3)           | lbz r4, 0x1(r3)
2C                            | lbz r0, j2dDefaultTevSwapMode@sda21   (hoisted above the stb)
3C  lbz r4, 0x7(r31)          |
40  lbz r0, j2dDefaultTevSwapMode@sda21 | lbz r4, 0x7(r31)
```
Cause, proven by experiment: the original treats the store to `_07` as possibly aliasing
`j2dDefaultTevSwapMode`, i.e. it doesn't see the object as const. Declaring it
`extern J2DTevSwapModeInfo j2dDefaultTevSwapMode;` (non-const, as pikmin2 and ac-decomp do,
pikmin2 calling it a "workaround") gives 100 %, but the definition in J2DTevs.cpp is const
and sits in .sdata2, so the declaration would contradict it: rejected as a trick.
Tried without effect: `u8` instead of `const u8` setter params (TP form); non-const
`mRasSel`/`mTexSel` members in J2DTevSwapModeInfo. mkdd has this unit Equivalent.

### newIndTexMtx__18J2DMaterialFactoryCFii (96.61 %)
```
20                            | lbz r0, 0x18(r4)   (ours: right after lfs 0x8)
3C  lbz r0, 0x18(r4)          |                    (target: after lfs 0x14)
```
Inlined `J2DIndTexMtx()` copy of j2dDefaultIndTexMtxInfo; scheduling of the mScale byte.
pikmin2 matches only with `mScale = *(s8*)&other.mScale; // don't even ask.` (pointer cast, banned).
Tried: TP's nested `if` form (no change). `__ct__12J2DIndTexMtxFv` in J2DMaterial matches
with the current operator=, so a header change there must keep it. FIXME left in source.

## SHARED-EDITS
None made. Possible future: a natural J2DTypes.h/J2DTevStage.h form that makes
j2dDefaultTevSwapMode look non-const to GC/2.6 would win this unit and J2DBloSaver's ctor.
