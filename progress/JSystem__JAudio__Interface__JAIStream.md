# JSystem/JAudio/Interface/JAIStream.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 15/15 functions, code 2680/2680, data 52/132 -> 132/132.

## Status: Matching (all sections 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gaps and fixes
1. .bss (0 %): placeholder `lbl_804B2400` (0x40) is the five 0xC
   destructor-chain objects __sinit registers (@734..@738) plus 4 bytes of
   padding. Split into lbl_804B2400/240C/2418/2424/2430 (0xC each). Trial map:
       000002c0 00003c 804b2400  1 .bss   JAIStream.o
       000002c0 00000c 804b2400  4 @734 ... 000002f0 00000c 804b2430  4 @738
   This also matched __sinit's relocations.
2. .sdata2: `lbl_8053B020` 0x8 -> 0x4 (0.5f plus padding before JAIStreamMgr.o's .sdata2):
       00000078 00000c 8053b018  1 .sdata2   JAIStream.o
       00000080 000004 8053b020  4 @583      JAIStream.o
       00000088 000010 8053b028  1 .sdata2   JAIStreamMgr.o

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholders lbl_804B2400 split (5 x 0xC) and lbl_8053B020 0x8 -> 0x4 (this unit's own data).

## Resistant
None.
