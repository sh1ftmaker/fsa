# JSystem/JAudio/System/JASCalc.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 5/5 functions, code 1876/1876, data 1048/1096 -> 1096/1096.

## Status: Matching (all sections 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gap and fix
.sdata2 was short: dtk sized the unit's last constant `lbl_8053B060` 8 bytes,
swallowing the 4 bytes of alignment padding before JASProbe.cpp's 8-aligned .sdata2.
Our object's last literal is 4 bytes. Trial-link map
(build/G4SE01/weakdedup_evidence/, DOL SHA-1 OK):
    00000098 00002c 8053b038  1 .sdata2   JASCalc.o
    000000c0 000004 8053b060  4 @304      JASCalc.o
    000000c8 000018 8053b068  1 .sdata2   JASProbe.o
Placeholder resized 0x8 -> 0x4 (precedent: cc82c68, mtx.c's lbl_8053B300).

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8053B060 size 0x8 -> 0x4 (this unit's own .sdata2).

## Resistant
None.
