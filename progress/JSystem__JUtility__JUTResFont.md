# JSystem/JUtility/JUTResFont.cpp

Baseline (before): `total: 1202 / 11560 functions, 219068 / 4434096 bytes of code (4.94 %)`; unit 12/24.
After: `total: 1232 / 11560 functions, 223908 / 4434096 bytes of code (5.05 %)`; unit 26/26, all sections 100 %, **Matching**, gate ok (main.dol SHA-1 OK).

## What fixed it
- Compiler flags: FSA's JUTResFont is `-O4,p`, not mkdd's `-O4,s` (target saves 3 regs with
  individual stw, uses stmw only for 7; `,s` produced stmw everywhere). Changed the unit's
  extra_cflags in configure.py.
- splits: `.text` starts at 0x80090DF0 (ctor(ResFONT*,JKRHeap*) and dtor sat outside the range);
  added `.data` 0x804994C0-0x80499510 (`__vt__10JUTResFont` + 4 bytes alignment).
- symbols (placeholders named, evidence = vtable at 0x804994C0 slot order + bytes vs donor code):
  ctor/dtor, countBlock, setBlock, setGX(TColor,TColor), getHeight, getWidthEntry, getFontCode,
  loadImage, getResFont, `__vt__10JUTResFont` (resized 0x50->0x4C), saoAboutEncoding_,
  @212/@235 strings, `halftofull$361` (0xC0->0xBE).
  Outside the unit: JUTFont::isLeadByte_{1Byte,2Byte,ShiftJIS} at 0x800772E4.. (weak copies,
  referenced by saoAboutEncoding_, bytes = header inlines); JUTReportConsole 0x80089084
  (calls f("%s", text)); JKRHeap.cpp operator new[](u32,JKRHeap*,int) 0x80084514,
  operator delete 0x80084580, operator delete[] 0x800845A4 (6 new variants in mkdd order, then delete/delete[]).
- Source: `GXPosition3f32(..., 0.0f)` literal (as ac-decomp/tww), m2c names in getFontCode
  renamed (low/high/mid/table/leading), `#line` lines removed (asserts compile out in FSA).
- Default ctor JUTResFont() kept (donor source); FSA's linker deadstrips it, link is byte-identical.

## SHARED-EDITS
- symbols.txt names outside the unit (above): JKRHeap.cpp operator new[]/delete/delete[],
  JUTConsole JUTReportConsole, JUTFont isLeadByte_* at 0x800772E4. Placeholders only.

## Resistant
None.
