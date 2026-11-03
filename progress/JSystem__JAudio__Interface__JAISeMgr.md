# JSystem/JAudio/Interface/JAISeMgr.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 22/22 functions, code 4736/4736, data 84/132 -> 132/132 (honest report).

## Status: NonMatching: report 100 %, trial link fails with the block-wide -sym on

## Data gap and fix (committed)
.bss (0 %): placeholder `lbl_804B22B8` (0x30) is the four 0xC destructor-chain
objects __sinit registers (@1616..@1619). Split into lbl_804B22B8/22C4/22D0/22DC
(0xC each); also matches __sinit's relocations.

## Blocker: -sym on (block-wide JAudio flag), not code
`link_check --unit` with the current flags: DOL SHA-1 698d3557... != build.sha1.
FSA places the weak `__ct__16JAISeCategoryMgrFv` at 0x80022DBC, right after
the implicit `__dt__16JAISeCategoryMgrFv` that follows JAISeMgr::JAISeMgr,
and `__dt__8JAISeMgrFv` at the end of the main .text before __sinit. With
-sym on our object puts the explicit-inline ctor, JSUList<JAISe> dtor and
~JAISeMgr into a separate .text section (#10), so everything from 0x80022DBC
shifts (vtable words 0x80491724/28 differ too).
Experiment (reverted, not committed): appending "-sym off" for this object
only gives the target order (dt, ct, deduped JSUList/JASGlobalInstance dtors,
..., ~JAISeMgr at the end) and `link_check --unit` reports byte-identical
(SHA-1 d86b3d9f...). Per-unit flags are not allowed, so this needs a
coordinator decision on which JAudio TUs were built with -sym on (JAISeq.cpp
shows the same thing).

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_804B22B8 split (4 x 0xC) (committed).
- PROPOSED (coordinator, configure.py): JAISeMgr.cpp (and JAISeq.cpp) were not
  built with -sym on in FSA; with "-sym off" the unit links SHA-1 OK and can
  flip to Matching.

## Resistant
- Link only; all functions and data 100 % in the honest report.
