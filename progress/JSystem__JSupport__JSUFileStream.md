# JSystem/JSupport/JSUFileStream.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4786 / 11561 functions, 863584 / 4442088 bytes of code (19.44 %)

Status: 5/5 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical; the weak dtor copy is deduplicated, the
linker keeps JKRDvdAramRipper's).

## What it took
No source change; symbols only:
- 0x8008770C fn_8008770C -> getPosition__18JSUFileInputStreamCFv (weak;
  `lwz r3,0xc(r3)`; __vt__18JSUFileInputStream slot 0x1C, after getLength)
- 0x8007F2B4 fn_8007F2B4 -> __dt__18JSUFileInputStreamFv (weak; the vtable's
  slot 0x08 points there). That address lies in JKRDvdAramRipper's .text; the
  same line is in unit/JSystem__JKernel__JKRDvdAramRipper (also mine).

## SHARED-EDITS
- symbols.txt 0x8007F2B4 (JKRDvdAramRipper range): identical to the
  JKRDvdAramRipper branch.
