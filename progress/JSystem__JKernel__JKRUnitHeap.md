# JSystem/JKernel/JKRUnitHeap.cpp (w15-jsgap)

New unit for dtk's unsplit block 0x80085A10..0x80086A54 (between JKRThread and
JSUInputStream). Identified as JKRUnitHeap: heap-check strings at .rodata
0x8045E820/0x8045E854, the 8-byte bitTable at .sdata2 0x8053B770, getHeapType
returns 'UNIT', vtable 0x80498FB8 has JKRHeap's 24 slots. No mkdd/tp copy;
started from Petari's src/JSystem/JKernel/JKRUnitHeap.cpp (CC0), adapted to
mkdd's JKRHeap.h (mHeapSize, do_fillFreeArea, JKRAllocFromHeap). do_alloc and
do_free (absent in Petari's file) written from the asm.

Splits: .text 0x80085A10-0x80086A54, .rodata 0x8045E820-0x8045E878, .data
0x80498FB8-0x80499018, .sdata2 0x8053B770-0x8053B788. No .bss/.sbss.
symbols.txt: 29 fn_ renamed (28 JKRUnitHeap methods, __dt__10JSUIosBaseFv at
0x80086A54), lbl_80498FB8 -> __vt__11JKRUnitHeap, lbl_8053B770 -> bitTable
(scope:local data:byte noreloc; dtk no longer reads it as a pointer).

## Matched: 25 / 28 (data 100 %), NonMatching
Resistant:
- create (92.77 %): ALIGN_NEXT(unitSize, alignment) — original does
  `add r3, align, unitSize; subi r0, r3, 1`, ours reuses align-1
  (`add r0, r3, r0`); callee-saved registers shifted by one. Tried explicit
  mask local, `alignment + unitSize - 1`, Petari's `#pragma optimization_level 3`.
- setUnitUsed (87.50 %): `clrlwi r0, r4, 29` scheduled at +0x24 instead of
  +0x2C. Tried pointer local, `|=` vs `= |`, positive if, `% 8`.
- check (97.33 %): `lbz r0, bitTable@sda21` before `li r4, bitTable` instead
  of after. Tried pointer walk, operand order.

Notes:
- find1FreeBlock must stay out of line in findFreeBlock: computing
  `wordCount` in both branches (natural, symmetric) keeps it over the
  auto-inline limit and matches both functions.
- do_free's mask is `bitTable[i] ^ 0xff` (xori); `~` gives andc/nor.

## Left out: __dt__10JSUIosBaseFv (0x80086A54) and __vt__10JSUIosBase (0x80499018)
The kept weak copies of JSUIosBase sit after JKRUnitHeap's code and data and
before JSUInputStream.cpp's (which emits its own copy at the END of its
.text, so they are not JSUInputStream's). Nothing in a unit heap uses a
stream, so they most likely come from a TU between the two whose own
functions were dead-stripped (same pattern as the J3DMtxCalc weak copies at
0x80079034). Kept outside this unit so it can still become Matching; the
symbol is named from the vtable slot.

SHARED-EDITS: none beyond this unit's splits/configure/symbols lines.
