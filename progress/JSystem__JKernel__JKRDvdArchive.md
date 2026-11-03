# JSystem/JKernel/JKRDvdArchive.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 8/8 functions, code 3084/3084, data 88/208 -> 208/208.

## Status: Matching (8/8, .text/.rodata/.data/.sdata2 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gap and fix
.rodata (97 %) was one placeholder `lbl_8045E228` of 0x78 bytes covering the
unit's four string literals, so the two fetchResource_subroutine functions'
`addi rX, rY, off` relocations to `...rodata.0`+off didn't pair. Split the
placeholder into the strings it covers (same style as JKRCompArchive's
lbl_8045E1C0/D4/E8):
    lbl_8045E228 0x12 "JKRDvdArchive.cpp"
    lbl_8045E23C 0x25 "Sorry, not applied for SZP archive.\n"
    lbl_8045E264 0x12 "??? bad sequence\n"
    lbl_8045E278 0x21 "Sorry, not applied SZP archive.\n"
The rest (0x8045E299..0x8045E2A0) is padding. Trial map:
    00001bc8 000071 8045e228  1 .rodata   JKRDvdArchive.o

Also removed mkdd's `#ifdef DEBUG CW_FORCE_STRINGS(...)` block (never compiled,
but gate.py rejects it in a Matching unit).

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8045E228 (0x78) split into four string symbols (this unit's .rodata).

## Resistant
None.
