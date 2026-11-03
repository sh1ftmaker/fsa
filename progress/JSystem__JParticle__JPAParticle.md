# JSystem/JParticle/JPAParticle.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4323 / 11561 functions, 774400 / 4442088 bytes of code (17.43 %)

Status: Matching. 6/6 functions (was 4/6), .text/.data/.sdata2 100 %. gate: ok (link SHA-1 OK).

## Changes
- init_c: added the final `mTexAnmIdx = 0;` (stb 0 at +0x94; tp and pikmin2 have it). Matches.
- symbols.txt:
  - `fn_800EFC0C` -> `init_c__15JPABaseParticleFP18JPAEmitterWorkDataP15JPABaseParticle`
    (between init_p and calc_p, called from JPAEmitter; byte-identical to our init_c).
  - `fn_800F059C` -> `execute__19JPAParticleCallBackFP14JPABaseEmitterP15JPABaseParticle`
    (blr; slot 3 of the vtable below; our weak inline execute lands at the same spot).
  - `fn_800E9F24` -> `draw__19JPAParticleCallBackFP14JPABaseEmitterP15JPABaseParticle`
    (blr in JPABaseShape's range; slot 4 of the same vtable; weak, emitted where first used).
  - `lbl_8049C178` (0x28) -> `__vt__19JPAParticleCallBack` (0x14): {0, 0, 0 (pure virtual
    dtor), execute, draw}, identical to the vtable our object emits; the following 0x14 zeros are
    alignment padding before JPAResource's 32-aligned jpa_pos.
- splits.txt: JPAParticle owns `.data 0x8049C178-0x8049C18C` (the vtable).
- Our object also has the out-of-line `~JPAParticleCallBack` and the getCalcCurrentPosition*/
  getWidth/getHeight functions, dead-stripped in FSA (the link is byte-identical).

## SHARED-EDITS
- symbols.txt rename `fn_800E9F24` -> `draw__19JPAParticleCallBack...` sits in JPABaseShape.cpp's
  .text range (same identical line also on branch unit/JSystem__JParticle__JPABaseShape).
