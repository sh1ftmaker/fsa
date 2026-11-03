# JSystem/JParticle/JPABaseShape.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4321 / 11561 functions, 772352 / 4442088 bytes of code (17.39 %)

Status: NonMatching. 80/81 functions (was 78/81), code 15556/15980, data 488/488 (was 64/488). gate: ok.

## Changes (symbols.txt only; the source is unchanged)
- `fn_800E77A0` -> `noLoadPrj__FPC18JPAEmitterWorkDataPA4_Cf` (local): entry 0 of p_prj
  {fn_800E77A0, loadPrj, loadPrjAnm}; blr, our static noLoadPrj at the same offset 0x120C.
- `fn_800E9F24` -> `draw__19JPAParticleCallBackFP14JPABaseEmitterP15JPABaseParticle`: slot 4 of
  JPAParticleCallBack's vtable (0x8049C178); our weak draw lands at the same offset 0x3994.
  Same line on branch unit/JSystem__JParticle__JPAParticle.
- .data placeholders split into the objects our .data holds at the same offsets/sizes:
  lbl_8049BEC0 -> jpa_dl, lbl_8049BEE0 -> jpa_dl_x, lbl_8049BF20 (0x148) -> p_rot, p_plane,
  st_bm/st_bf/st_lo/st_c/st_ao/st_ca/st_aa__12JPABaseShape (0x4 trailing bytes are padding before
  JPADynamicsBlock's jump table).
- .sdata2 placeholders 0x8053BD28-0x8053BD5C -> the compiler locals @450 ... @1466 (same sizes).

## Resistant
- JPADrawLine 97.75 %: instruction scheduling of the second vertex. Target loads position2
  y, z, x (0xc, 0x10, 0x8) after the first tex coord and stores x, y, z together; ours loads x
  first and stores it before the 0.0f tex coord store. Also 0.0f lands in f4 (ours f3).
  Tried pikmin2's `zero`/`one` locals: no change.

## SHARED-EDITS
- none beyond the draw__19JPAParticleCallBack line shared with the JPAParticle branch.

## Wave 8 (2026-11-02)

Baseline: total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
Result: unchanged, 80/81, data 100 %. Only the comment on JPADrawLine became a FIXME with
the exact diff.

JPADrawLine (97.75 %), variants tried this wave (none changed the result):
- `f32 zero = 0.0f` local for the tex coords (pikmin2's form, again with one literal).
- without `GXEnd()` (pikmin2 omits it; it is an empty inline).
- `volatile` GXWGFifo in GXVert.h (the SDK's form; see J2DPictureEx's checkpoint):
  93.9 %, worse, and regresses JPADrawPoint/JPADrawStripe/JPADrawStripeX.
tp, Petari and mkdd have the same body as ours. A genuine scheduling difference, not
blocked by another unit.
