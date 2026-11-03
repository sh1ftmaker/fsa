# JSystem/JAudio/System/JASLfo.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 5/5 functions, code 392/392, data 4/28 -> 28/28.

## Status: Matching (all sections 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gap and fix
.sdata2 was short: dtk sized the unit's last constant `lbl_8053B148` 8 bytes,
swallowing the 4 bytes of alignment padding before JASOscillator.cpp's 8-aligned .sdata2.
Our object's last literal is 4 bytes. Trial-link map
(build/G4SE01/weakdedup_evidence/, DOL SHA-1 OK):
    00000198 000014 8053b138  1 .sdata2   JASLfo.o
    000001a8 000004 8053b148  4 @102      JASLfo.o
    000001b0 000028 8053b150  1 .sdata2   JASOscillator.o
Placeholder resized 0x8 -> 0x4 (precedent: cc82c68, mtx.c's lbl_8053B300).

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8053B148 size 0x8 -> 0x4 (this unit's own .sdata2).

## Resistant
None.
