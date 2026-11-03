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

## .text alignment of the four Task C units (w15-jsgap)
Status: all four units stay Matching; only their splits changed.

- dtk's four `pad_03_*_text` pieces (0x80039B7C 4 B, 0x80039D08 24 B,
  0x8003A028 24 B, 0x8003A11C 4 B) were zero fill, not functions: the four
  Task C files are built with `-func_align 32`, so their `.text` is
  32-aligned and the linker pads before each one. `align:32` on each unit's
  `.text` in splits.txt tells dtk so; the pieces disappear from the split
  (and from the report's function total, 11,561 -> 11,557). `ninja` SHA-1 OK.
- tools/site_data.py skips any `pad_` piece left in an auto split, so the
  site and progress.py never count padding as a function.

SHARED-EDITS: tools/site_data.py (sanctioned for this task).
