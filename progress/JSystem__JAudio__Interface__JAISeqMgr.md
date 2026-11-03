# JSystem/JAudio/Interface/JAISeqMgr.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 11/11 functions, code 2196/2196, data 28/92 -> 92/92.

## Status: Matching (all sections 100 %, link_check --unit SHA-1 OK, gate ok)

## Data gaps and fixes
1. .bss (0 %): placeholder `lbl_804B2328` (0x30) is the four 0xC
   destructor-chain objects __sinit registers (@718..@721). Split into
   lbl_804B2328/2334/2340/234C (0xC each). Trial map:
       000001e8 000030 804b2328  1 .bss   JAISeqMgr.o
       000001e8 00000c 804b2328  4 @718 ... 0000020c 00000c 804b234c  4 @721
   This also matched __sinit's relocations.
2. .sdata2: `lbl_8053AFD8` 0x8 -> 0x4 (0.5f plus padding before JAISound.o's .sdata2):
       00000030 00000c 8053afd0  1 .sdata2   JAISeqMgr.o
       00000038 000004 8053afd8  4 @430      JAISeqMgr.o
       00000040 000020 8053afe0  1 .sdata2   JAISound.o

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholders lbl_804B2328 split (4 x 0xC) and lbl_8053AFD8 0x8 -> 0x4 (this unit's own data).

## Resistant
None.
