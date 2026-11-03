# JSystem/J3DGraphBase/J3DTransform.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 13/13 functions, code 2484/2484, data 112/128 -> 128/128.

## Status: Matching (all sections 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gap and fix
1. .sdata2: `lbl_8053BB88` 0x8 -> 0x4 (0.5f plus padding before J3DShape.o's 8-aligned .sdata2):
       00000be0 00000c 8053bb80  1 .sdata2   J3DTransform.o
       00000be8 000004 8053bb88  4 @377      J3DTransform.o
       00000bf0 000008 8053bb90  1 .sdata2   J3DShape.o
2. Named `lbl_804633E8` -> `j3dDefaultTransformInfo` (0x20, first .rodata
   object of the unit; our object defines it there with the same bytes; tp
   and tww symbols.txt name it the same with size 0x20).

## SHARED-EDITS
- config/G4SE01/symbols.txt: lbl_8053BB88 0x8 -> 0x4; lbl_804633E8 -> j3dDefaultTransformInfo
  (J3DJoint.cpp references it; its numbers are unchanged).

## Resistant
None.
