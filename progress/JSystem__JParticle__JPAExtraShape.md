# JSystem/JParticle/JPAExtraShape.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 12/12 functions, code 1392/1392, data 0/40 -> 40/40.

## Status: Matching (12/12, .text/.sdata2 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gap and fix
.sdata2 was 94.74 %: dtk had sized the last constant `lbl_8053BDE0` 8
bytes, swallowing the 4 bytes of alignment padding before JPAFieldBlock.cpp's
.sdata2 (8-aligned at 0x8053BDE8). Our object emits 0x24 bytes; the last
literal @678 (0.0f) is 4 bytes.

Evidence (trial-link map, build/G4SE01/weakdedup_evidence/JSystem__JParticle__JPAExtraShape.MAP, SHA-1 OK):
    00000e20 000024 8053bdc0  1 .sdata2   JPAExtraShape.o
    00000e40 000004 8053bde0  4 @678      JPAExtraShape.o
    00000e48 000018 8053bde8  1 .sdata2   JPAFieldBlock.o

Placeholder resized 0x8 -> 0x4 (precedent: cc82c68, mtx.c's lbl_8053B300).

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8053BDE0 size 0x8 -> 0x4 (this unit's own .sdata2).

## Resistant
None.
