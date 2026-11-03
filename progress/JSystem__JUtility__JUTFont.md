# JSystem/JUtility/JUTFont.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4788 / 11561 functions, 863668 / 4442088 bytes of code (19.44 %)

Status: 9/9 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
No source change. The four misses were placeholder names of the header's weak
inlines, identified by their __vt__7JUTFont slots and the virtual they call
(include/JSystem/JUtility/JUTFont.h slot comments; mkdd MarioClub_us has the
same four as scope:weak):
- 0x8008CE88 __dt__7JUTFontFv (vtable 0x08; stores __vt__7JUTFont, __dl__FPv)
- 0x8008CED0 setGX__7JUTFontFQ28JUtility6TColorQ28JUtility6TColor (vtable 0x10; calls slot 0x0C setGX())
- 0x8008CEFC getCellWidth__7JUTFontCFv (vtable 0x30; calls slot 0x28 getWidth())
- 0x8008CF28 getCellHeight__7JUTFontCFv (vtable 0x34; calls slot 0x24 getHeight())

## SHARED-EDITS
None.
