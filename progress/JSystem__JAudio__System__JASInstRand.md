# JSystem/JAudio/System/JASInstRand.cpp

**Update (review CHANGES): 1/2, NonMatching, on purpose.** effect() used mkdd's fabricated `getY`
inline and a dummy `val = FLT_EPSILON` (a fakematch). It now uses the natural form
`f32 max = 1.0f - FLT_EPSILON; f32 val = oRandom.get_ufloat_1() * 2.0f - max;` (97.56 %) with a
`// FIXME:`: FSA multiplies by an unfolded 1.0f (fmuls + fnmsubs, ours folds to fmsubs), and its
.sdata2 order is 2.0f, 1.0f (ours 1.0f, 2.0f; .sdata2 75 %). `getY` was removed from JASInst.h; nothing
else used it. __sinit naming and the .bss split stay. Progress drops by this one function (counted
before only through the fakematch). gate: ok.

Baseline (w10-jas start): progress.py total 4784 / 11561, 863464 bytes; unit 1/2 (__sinit missing).
Now: 2/2, all sections 100 %, `link_check --unit` SHA-1 OK, but NOT Matching: the gate rejects the
unit for mkdd's `// probably fakematch` in effect(). gate (NonMatching): ok.

## Done
- fn_80030960 -> __sinit_JASInstRand_cpp (local; .ctors 0x8045BD64 points at it; our object's sinit
  constructs the JASChannel pool the same way).
- .bss 0x804B28A8-0x804B28B4 split into the unit: the pool's destructor-chain node, lbl_804B28A8
  (0x10 placeholder incl. padding) -> @245, 0xC.

## Resistant
- effect (100 % but flagged): mkdd's body uses a fabricated inline `getY(in, rnd, mult)` called with
  mult = 1.0f and a dummy `val = FLT_EPSILON` temp. The target really computes
  `2*(u-1) - 1.0f*(1.0f - __float_epsilon)` with an unfolded 1.0f multiply (fnmsubs), so some inline with
  a 1.0f parameter is real, but which one is unknown (SMG's version has different code). Variants tried:
  `2.0f*r - (1.0f - FLT_EPSILON)` 82.2 %; `r*2.0f - 1.0f*(1.0f - FLT_EPSILON)` 82.2 %;
  `r = get*2.0f; r - 1.0f*(...)` 83.3 %; `max = 1.0f - FLT_EPSILON; get*2.0f - max` 97.6 % (fmsubs instead of
  fmuls + fnmsubs). Codegen/source-form problem; not blocked by another unit.

## SHARED-EDITS
None.
