# JSystem/JParticle/JPAEmitter.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4787 / 11561 functions, 864604 / 4442088 bytes of code (19.46 %)
Unit: 7/7 -> 9/9 functions, code 1272/1272 -> 2192/2192, data 0/24 -> 24/24.
Side effect: JPAEmitterManager 13 -> 14 functions (its call to JPABaseEmitter::init now resolves by name). No unit lower (full honest-report diff).

## Status: Matching (9/9, .text/.sdata2 100 %, link_check --unit SHA-1 OK, gate ok)

## Gaps found
1. The unit's first two functions were outside its split: 0x800EB208..0x800EB5A0
   was an unsplit `auto_03_800EB208_text` holding fn_800EB208 / fn_800EB250, and
   they used this unit's .sdata2 constants, so the trial link failed (undefined
   lbl_8053BD94/BDA0). .text now starts at 0x800EB208.
   - fn_800EB208 -> `__dt__18JPAEmitterCallBackFv`: deleting dtor that stores
     the vtable at 0x80498978 and calls `__dl__FPv`; first function of every
     donor JPAEmitter.cpp (tp, mkdd).
   - fn_800EB250 -> `init__14JPABaseEmitterFP17JPAEmitterManagerP11JPAResource`:
     matches our JPABaseEmitter::init byte for byte.
2. init was one store short: tp's init ends with `mTexAnmIdx = 0;`
   (`stb r4, 0x111(r30)`), added.
3. .sdata2: placeholder `lbl_8053BDA0` 0x8 -> 0x4 (padding before
   JPAEmitterManager.o's 8-aligned .sdata2). Map:
       00000df0 000014 8053bd90  1 .sdata2   JPAEmitter.o
       00000e00 000004 8053bda0  4 @625      JPAEmitter.o
       00000e08 000018 8053bda8  1 .sdata2   JPAEmitterManager.o
4. Weak vtable / weak virtuals of JPAEmitterCallBack: FSA keeps another TU's
   copy (unsplit auto_07_80498978_data / auto_03_80077124_text). Named from
   the vtable slots: lbl_80498978 = {0, 0, 0 (pure virtual dtor),
   fn_80077130, fn_80077124, fn_80077128, fn_8007712C} = __vt__18JPAEmitterCallBack
   (0x1C; the 4 trailing bytes are padding) with slots execute, executeAfter,
   draw, drawAfter (declaration order in tp/mkdd JPAEmitter.h). The trial map
   lists our object's copies as UNREFERENCED DUPLICATE. Our target split has no
   .data, so the report doesn't count the weak vtable.

## SHARED-EDITS
- config/G4SE01/splits.txt: JPAEmitter.cpp .text start 0x800EB5A0 -> 0x800EB208 (unsplit gap).
- config/G4SE01/symbols.txt: fn_800EB208, fn_800EB250 named (this unit);
  lbl_80498978 -> __vt__18JPAEmitterCallBack size 0x20 -> 0x1C, and
  fn_80077124/28/2C/30 -> executeAfter/draw/drawAfter/execute__18JPAEmitterCallBackFP14JPABaseEmitter
  (placeholders in unsplit areas owned by no unit yet); lbl_8053BDA0 0x8 -> 0x4.

## Resistant
None.
