# JSystem/J3DGraphAnimator/J3DJointTree.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 5/5 functions, code 972/972, data 96/104 -> 104/104.

## Status: Matching (all sections 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gap and fix
.sdata2 was short: dtk sized the unit's last constant `lbl_8053BCE0` 8 bytes,
swallowing the 4 bytes of alignment padding before J3DMtxBuffer.cpp's 8-aligned .sdata2.
Our object's last literal is 4 bytes. Trial-link map
(build/G4SE01/weakdedup_evidence/, DOL SHA-1 OK):
    00000d40 000004 8053bce0  1 .sdata2   J3DJointTree.o
    00000d40 000004 8053bce0  4 @495      J3DJointTree.o
    00000d48 000008 8053bce8  1 .sdata2   J3DMtxBuffer.o
Placeholder resized 0x8 -> 0x4 (precedent: cc82c68, mtx.c's lbl_8053B300).

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8053BCE0 size 0x8 -> 0x4 (this unit's own .sdata2).

## Resistant
None.
