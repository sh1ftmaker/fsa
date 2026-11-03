# JSystem/J3DGraphBase/J3DSys.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4787 / 11561 functions, 863768 / 4442088 bytes of code (19.45 %)

Status: NonMatching, code complete. 14/14 functions (was 11/14); .text/.ctors/.rodata/.data/
.sdata/.sdata2 100 %, .sbss 50 %; `link_check.py --unit` SHA-1 OK; gate ok.
Only blocker: the real symbol `j3dDefaultViewNo` is size 0x8 in symbols.txt (SHARED-EDITS).

## Changes
- reinitTransform: tp/tww's `GXSetTexCoordGen(GX_TEXCOORDn, GX_TG_MTX2x4, GX_TG_TEXn, GX_IDENTITY)`
  (target passes `li r4,1` = GX_TG_MTX2x4; pikmin2's file had MTX3x4) and GX_PNMTX0.
- symbols.txt:
  - fn_800BB5EC -> ErrorReport__6J3DSysCF10J3DErrType (`blr`, size 4; pikmin2 GPVE01 has
    ErrorReport size 0x4 directly before __sinit_J3DSys_cpp, same layout).
  - fn_800BB5F0 -> __sinit_J3DSys_cpp (scope:local; its .ctors slot 0x8045BDDC, constructs
    j3dSys; tww/tp/mkdd/pikmin2 all have __sinit_J3DSys_cpp size 0x28 here).
  - lbl_804632A8 (0x38 placeholder) split into the compiler's @419/@420 (0x10 each) and @457
    (0x18), the three const tables drawInit copies to the stack (our object's layout, same
    bytes). This fixed drawInit's 10 relocation mismatches.
  - lbl_8053BB78 (placeholder) 0x8 -> 0x4: it is one float (0.1f); the other 4 bytes are
    section padding.

## SHARED-EDITS
- `j3dDefaultViewNo` (.sbss 0x8053A9C0) size 0x8 -> 0x4 (real symbol, needs reviewer approval
  in approved_symbol_changes.txt). Evidence: tww, tp, mkdd and pikmin2 all have
  `j3dDefaultViewNo ... size:0x4` with the next .sbss object 8 bytes later (sbss objects are
  8-aligned); FSA's next object sInterruptFlag__17J3DDisplayListObj is at 0x8053A9C8; our
  object's j3dDefaultViewNo is 4 bytes and the trial link is SHA-1 OK. After that change the
  unit can be flipped to Matching.

## Notes for review
- `fakeFunc` (pre-existing, from pikmin2's file) is the only thing that emits the J3DErrType
  message strings, which are in FSA's .rodata; it is local and dead-stripped. No donor
  has the real construct; left as it was.

## w12-follow: fakeFunc removed, Matching

pikmin2's `fakeFunc` (a dead local `char* msgs[]` whose only job was to emit the
J3DErrType strings and their pointer table) is gone. The table is a function-local
`static const char* const sErrMsg[]` in `J3DSys::ErrorReport(J3DErrType)`, the
function those messages belong to (its release body is empty). .rodata stays 100 %
(strings 0x804632E0..0x804633CC, then the 7-pointer table), every function 100 %,
`link_check.py --unit` byte-identical; flipped to Matching.
