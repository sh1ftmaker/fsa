# JSystem/JMath/JMath.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4785 / 11561 functions, 863564 / 4442088 bytes of code (19.44 %)

Status: 2/3 functions (was 1/4), .text 76 %, .sdata2 50 %, `NonMatching`, gate ok.

## Done
- JMAMTXApplyScale: .sdata2 0x8053B940 lbl_8053B940 -> @120 (1.0f,
  scope:local, as the compiler names it; same convention as the other
  @N sdata2 constants). Now 100 %.
- .text end 0x80093340 -> 0x80093338: __ct__Q25JMath13TRandom_fast_FUl
  (0x80093338, `stw r4,0(r3)`) is JMath/random.cpp, not JMath.cpp (mkdd
  MarioClub_us splits.txt has random.cpp as its own unit between JMath.cpp
  and JMATrigonometric.cpp, the same layout as FSA). It now falls to dtk's
  auto split.

## Resistant
- fn_80093280 (0x2C, called from game/act_maiden_ending): squared length of a
  Vec via psq_l/ps_madd/ps_sum0, then `sq == 0 ? sq : sq * frsqrte(sq)`.
  Not in any donor's JMath.cpp (mkdd/tp/tww/pikmin2 have JMAFastVECNormalize
  at this position, which stores a vector; this returns f32). JMAFastSqrt in
  the header tests `x > 0.0f`, the target tests `== 0`, so it is not that
  inline either. No name evidence; not written (FIXME in source). Blocked by
  naming/missing donor, not by another unit. Its 0.0f is .sdata2 0x8053B944
  (lbl_8053B944), the other half of .sdata2.

## SHARED-EDITS
- Proposed new unit JSystem/JMath/random.cpp: .text 0x80093338..0x80093340
  (TRandom_fast_ ctor). libs/jsystem/JSystem/JMath/random.cpp already exists
  (mkdd); needs a configure.py Object and a splits.txt stanza (coordinator).
