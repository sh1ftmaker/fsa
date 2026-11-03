# JSystem/J3DGraphBase/J3DDrawBuffer.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 14/14 functions, code 2392/2392, data 212/220 (220/220 with the two proposed resizes below).

## Status: NonMatching: waiting on two reviewed resizes (verified locally: 100 % and link_check --unit SHA-1 OK)

## Data gaps
1. .data, committed: placeholder `lbl_8049A7F8` (0x48) is the six 0xC
   pointer-to-member constants (@383..@388) __sinit copies into
   sortFuncTable. Split into lbl_8049A7F8/804/810/81C/828/834 (0xC each).
   __sinit 98.2 % -> 99.5 %.
2. .data, needs review: real symbol `sortFuncTable__13J3DDrawBuffer` is sized
   0x60 but is 6 PTMFs = 0x48; the next 0x18 are the two draw PTMFs @389/@390
   (__sinit's `lwz r8..r0, 0x90..0xa4(r16)`, copied into drawFuncTable).
   Trial map (with the resize):
       000091a0 000048 8049a840  4 sortFuncTable__13J3DDrawBuffer J3DDrawBuffer.o
       000091e8 00000c 8049a888  4 @389   J3DDrawBuffer.o
       000091f4 00000c 8049a894  4 @390   J3DDrawBuffer.o
       00009200 000018 8049a8a0  4 drawFuncTable__13J3DDrawBuffer J3DDrawBuffer.o
3. .sbss (50 %), needs review: real symbol `entryNum__13J3DDrawBuffer` is
   sized 0x8; it is a 4-byte int (tp/tww: size 0x4) plus padding before
   J3DJoint.o's .sbss:
       00000760 000004 8053a9e0  4 entryNum__13J3DDrawBuffer J3DDrawBuffer.o
       00000768 000010 8053a9e8  1 .sbss  J3DJoint.o

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8049A7F8 split into 6 x 0xC (committed).
- PROPOSED (approved_symbol_changes.txt, needs review):
  `sortFuncTable__13J3DDrawBuffer 0x60 0x48` plus new placeholders
  lbl_8049A888 / lbl_8049A894 (0xC each) for @389/@390;
  `entryNum__13J3DDrawBuffer 0x8 0x4`. With both, the unit is 220/220 and
  links byte-identical (tested, reverted); then flip to Matching.

## Resistant
None in code.
