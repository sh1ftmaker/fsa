# JSystem/JAudio/Task/dspproc.c

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 5/5 functions, code 292/292, data 16/24 -> 24/24.

## Status: Matching (5/5, all sections 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gap and fix
.sdata2 (66.7 %): dtk sized the only constant `lbl_8053B290` 8 bytes; it is
4096.0f (4 bytes) plus padding before JAUAudioMgr.cpp's 8-aligned .sdata2.
Trial map:
    000002f0 000004 8053b290  1 .sdata2   dspproc.o
    000002f0 000004 8053b290  4 @128      dspproc.o
    000002f8 000010 8053b298  1 .sdata2   JAUAudioMgr.o
Placeholder resized 0x8 -> 0x4.

The `gap_03_*` entries fdiff lists as missing are the zero fill of
`-func_align 32` between functions, not code.

Note for review: the source keeps mkdd's empty `// UNUSED FUNCTION` stubs
(DSPSendCommands, Dswap, ...); FSA's linker dead-strips them as it did the
originals, so they don't reach the DOL (trial link SHA-1 OK).

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8053B290 size 0x8 -> 0x4 (this unit's own .sdata2).

## Resistant
None.
