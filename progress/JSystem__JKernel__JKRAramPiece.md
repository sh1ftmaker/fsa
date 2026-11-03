# JSystem/JKernel/JKRAramPiece.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4786 / 11561 functions, 863680 / 4442088 bytes of code (19.44 %)

Status: 7/7 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
No source change; symbols only.
- orderSync: .rodata lbl_8045E120 (0x70 blob) split into its six strings so
  the +0x10..+0x58 references resolve: lbl_8045E120 "direction = %x\n" (0x10),
  lbl_8045E130 "source = %x\n" (0xD), lbl_8045E140 "destination = %x\n" (0x12),
  lbl_8045E154 "length = %x\n" (0xD), lbl_8045E164 "JKRAramPiece.cpp" (0x11),
  lbl_8045E178 "illegal address. abort." (0x18).
- doneDMA: blocked by JKRDecomp's placeholder (bl fn_8007E20C). This branch
  carries the same line as unit/JSystem__JKernel__JKRDecomp (also mine):
  0x8007E20C -> sendCommand__9JKRDecompFP16JKRDecompCommand.

## SHARED-EDITS
- symbols.txt 0x8007E20C (JKRDecomp range): identical to the JKRDecomp branch.
