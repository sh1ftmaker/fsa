# dolphin/mtx/mtx.c

State: 13/13 functions match, .text 100 %, .sdata 100 %, .sdata2 84.62 %.
Unit stays NonMatching (see .sdata2 below).

## Matched
All 13: PSMTXIdentity, PSMTXCopy, PSMTXConcat, PSMTXInverse, PSMTXRotRad,
PSMTXRotTrig, __PSMTXRotAxisRadInternal, PSMTXRotAxisRad, PSMTXTrans,
PSMTXTransApply, PSMTXScale, PSMTXScaleApply, C_MTXLightOrtho.
No source change was needed; all misses were data/symbols:
- splits.txt: added `.sdata2 start:0x8053B2F0 end:0x8053B308` (1.0f, 0.0f,
  0.5f, 3.0f, 2.0f + 4 bytes alignment padding before mtx44.c's 0x8053B308).
- symbols.txt: fn_80046ABC -> PSMTXRotRad, fn_80046C8C -> PSMTXRotAxisRad
  (sinf/cosf of the angle, then bl PSMTXRotTrig / __PSMTXRotAxisRadInternal,
  exactly the donor bodies, same position in the file);
  fn_80070578 -> sinf, fn_8007059C -> cosf (each is `frsp(f(x))` around a
  double routine: fn_8007023C right after s_modf.c = sin, fn_8006FCD4 = cos;
  the call order sin-then-cos matches PSMTXRotTrig(m, axis, sinA, cosA)).

## Resistant
- .sdata2 84.62 %: our object also carries -1.0f and 0.01745329252f, the
  constants of C_MTXLightPerspective (dead-stripped in FSA, kept in source
  like the donors). Per COORDINATOR rake, stays NonMatching until the linked
  DOL can confirm.

## SHARED-EDITS
- config/G4SE01/symbols.txt: sinf (0x80070578), cosf (0x8007059C) live in the
  unsplit MSL math_ppc area (auto_03_8007023C). Owner of math_ppc.c should
  know. Candidates (not renamed, not needed here): fn_8007023C = sin,
  fn_8006FCD4 = cos.
- Note: dtk rewrites/reorders symbols.txt on each config regen; only the 4
  rename lines were committed.
