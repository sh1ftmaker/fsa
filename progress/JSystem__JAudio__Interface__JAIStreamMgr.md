# JSystem/JAudio/Interface/JAIStreamMgr.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 7/7 functions, code 1588/1588, data 4/68 -> 68/68.

## Status: Matching (all sections 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gaps and fixes
1. .bss (0 %): placeholder `lbl_804B2440` (0x30) is the four 0xC
   destructor-chain objects __sinit registers (@631..@634; `addi r5, r31,
   0xc/0x18/0x24` in __sinit). Split into lbl_804B2440/244C/2458/2464 (0xC each).
   Trial map: `00000300 000030 804b2440  1 .bss JAIStreamMgr.o` then @631..@634 at
   804b2440/244c/2458/2464, 0xC each. This also matched __sinit's relocations.
2. .sdata2: `lbl_8053B030` 0x8 -> 0x4 (the 0.5f plus padding before JASCalc.o's
   8-aligned .sdata2):
       00000088 00000c 8053b028  1 .sdata2   JAIStreamMgr.o
       00000090 000004 8053b030  4 @395      JAIStreamMgr.o
       00000098 000030 8053b038  1 .sdata2   JASCalc.o
3. Removed mkdd's never-compiled `#ifdef DEBUG CW_FORCE_STRINGS(...)` (gate rejects it in a Matching unit).

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholders lbl_804B2440 split (4 x 0xC) and lbl_8053B030 0x8 -> 0x4 (this unit's own data).

## Resistant
None.
