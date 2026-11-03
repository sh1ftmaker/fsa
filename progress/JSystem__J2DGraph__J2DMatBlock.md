# JSystem/J2DGraph/J2DMatBlock.cpp

Baseline (progress.py first line at start): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After (branch as committed): total: 4503 / 11561 functions, 810028 / 4442088 bytes of code (18.24 %)

Status: NonMatching. 285/292 functions on the branch (was 283/292); 289/292 with the
SHARED-EDIT below applied. Data 100 %. gate: ok.

## Changes (unit source)
- J2DTevBlock8/16::insertTexture(u32, const ResTIMG*, JUTPalette*): the undelete-flag
  scratch array was `bool local_38[4]` but filled for 8 entries (out of bounds); tp has
  `[8]`. With the right size both match (the original reads mUndeleteFlag once and stores
  eight bits to 0x8..0xf).
- J2DTevBlock2/4/8/16::initialize: `mSwapModeTables[i].setTevSwapModeTableInfo(...)`
  (tp's and TevBlock1's form) instead of `mSwapModeTables[i] = j2dDefaultTevSwapModeTable`;
  the original's swap-table loop is fully unrolled like TevBlock1's.
- FIXME comments on TevBlock2/8::initialize.

## SHARED-EDITS (needed, not applied on the branch)
`libs/sdk/include/JSystem/J2D/J2DTypes.h`: give `J2DColorChanInfo` and
`J2DTevSwapModeTableInfo` the user-declared `operator=` that tp (J2DTevs.h:554, :607) and
pikmin2 (J2DTypes.h:128, :238) both have:

    struct J2DColorChanInfo { ...
        J2DColorChanInfo& operator=(const J2DColorChanInfo& other)
        { _00 = other._00; _01 = other._01; _02 = other._02; _03 = other._03; return *this; }
    };
    struct J2DTevSwapModeTableInfo {
        J2DTevSwapModeTableInfo& operator=(const J2DTevSwapModeTableInfo& other)
        { mR = other.mR; mG = other.mG; mB = other.mB; mA = other.mA; return *this; }
        ...
    };

Why: the original re-loads `j2dDefaultColorChanInfo._01` and the four
`j2dDefaultTevSwapModeTable` bytes after every store into `this` (no CSE). MWCC treats a
`const` object of a POD type as read-only and CSEs the loads; with a user-declared
operator= the type is no longer POD and the loads are repeated, exactly as in the target.
(Dropping `const` from the extern gives the same code but conflicts with the const
definition in J2DTevs.cpp; per-variable test: only these two types matter, making the
others non-const breaks J2DPEBlock::initialize and others.)
Effect, measured with a full build + report diff against the baseline: only
J2DMatBlock changes: J2DColorBlock::initialize, J2DTevBlock1/4/16::initialize 100 %;
TevBlock2 85 %, TevBlock8 88 %. No other function in any unit changes.

## Resistant
- J2DTevBlock2::initialize (85 % with the shared edit): scheduling. Target computes
  `lis/addi r4, j2dDefaultIndTevStageInfo` early among the mKColorSels stores and copies
  the first stage info +4, +8, +0; ours folds it into `lwzu r6, ...@l(r4)` at the copy.
  Tried: pikmin2 form (loops for indices/orders/stages/sels/palettes): no change; a loop
  over the two mIndStages: 90 % but only one stack temp (target has two, 0x1c and 0x10), so
  structurally wrong, not kept. Genuine codegen difference, not blocked by another unit.
- J2DTevBlock8::initialize (88 % with the shared edit): register allocation. Target loads
  j2dDefaultTevColor's four s16 at the very top (r5, r0, r7, r22; stmw r21, frame 0x50);
  ours after the tev-order copies into r18-r21 (stmw r18, frame 0x60). Tried pikmin2's
  `const GXColorS10& color` / `const J2DTevOrderInfo& order` locals: 87.4 / 87.7 %.
  Genuine codegen difference.
- setIndTexMtx__15J2DIndBlockFull (99.41 %, weak, defined in J2DMatBlock.h): target
  `mulli r0` / ours `mulli r6` because ours loads mScale (lbz r0) earlier. pikmin2 matches
  this with `mScale = *(s8*)&other.mScale; // don't even ask.` in J2DIndTexMtxInfo's
  operator=, a fakematch, not used. tp's `mTexMtxes[index] = texMtx;`: no change.
