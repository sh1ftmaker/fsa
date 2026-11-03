# JSystem/JUtility/JUTProcBar.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 7/7 functions, code 8608/8608, data 32/104 -> 104/104.

## Status: Matching (7/7, .text/.sdata/.sbss/.sdata2 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gap and fix
Only .sdata2 was short (97.14 %): dtk had sized the last constant
`lbl_8053B908` 8 bytes, swallowing the 4 bytes of alignment padding before
JUTResFont.cpp's .sdata2 (which starts 8-aligned at 0x8053B910). Our object
emits 0x44 bytes of .sdata2 (last literal @743 = 3.0f, 4 bytes).

Evidence (trial-link map, build/G4SE01/weakdedup_evidence/JSystem__JUtility__JUTProcBar.MAP, SHA-1 OK):
    00000928 000044 8053b8c8  1 .sdata2   JUTProcBar.o
    00000968 000004 8053b908  4 @743      JUTProcBar.o
    00000970 000018 8053b910  1 .sdata2   JUTResFont.o

Placeholder `lbl_8053B908` resized 0x8 -> 0x4 in symbols.txt (same fix as
cc82c68 for mtx.c's lbl_8053B300); dtk now emits a gap symbol for the padding.

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8053B908 size 0x8 -> 0x4 (this unit's own .sdata2).

## Resistant
None.
