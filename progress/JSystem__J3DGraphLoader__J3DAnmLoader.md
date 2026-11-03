# JSystem/J3DGraphLoader/J3DAnmLoader.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 41/41 functions, code 6684/6684, data 96/104 (unchanged; 104/104 once the tooling below exists).

## Status: NonMatching: code 100 %, link_check --unit SHA-1 OK; .sdata2 blocked by tooling

## Data gap
.sdata2 target = 0.0f + 4 bytes padding. Our object has 0.0f (@576) and 1.0f
(@866). @866 is used only by our weak copy of
`getWeight__13J3DAnmClusterCFUs` (an inline from J3DAnimation.h, `lfs f1,
1.0f; blr`), which the linker deduplicates: FSA keeps J3DAnimation.o's copy
(0x800C6DD4). So @866 is data of a deduplicated weak function, a case the
honest report doesn't strip yet (manifest note: "@866 kept: the map lists it
UNUSED but live getWeight__13J3DAnmClusterCFUs references it").

Trial-link map (build/G4SE01/weakdedup_evidence/JSystem__J3DGraphLoader__J3DAnmLoader.MAP, SHA-1 OK):
    21432:            11] getWeight__13J3DAnmClusterCFUs (func,global) found in J3DAnimation.o
    21433:            11] >>> UNREFERENCED DUPLICATE getWeight__13J3DAnmClusterCFUs
    21434:            11] >>> (func,weak) found in J3DAnmLoader.o
    69922:  00000ce0 000008 8053bc80  1 .sdata2   J3DAnmLoader.o
    69923:  00000ce0 000004 8053bc80  4 @576      J3DAnmLoader.o
    69924:  UNUSED   000004 ........ @866 J3DAnmLoader.o
    69925:  00000ce8 000040 8053bc88  1 .sdata2   J3DGD.o

Committed: placeholder `lbl_8053BC80` 0x8 -> 0x4 (the map's @576 is 4 bytes;
the rest is padding before J3DGD.o), so the unit reaches 100 % as soon as
@866 is stripped.

## SHARED-EDITS
- config/G4SE01/symbols.txt: placeholder lbl_8053BC80 0x8 -> 0x4 (committed).
- TOOLING PROPOSAL (honest_report.py / gate.py, not touched): in the dead_data
  rule, accept as a dead referencer a WEAK FUNC of this object that the
  verified map lists as `>>> UNREFERENCED DUPLICATE <name>` followed by
  `>>> (func,weak) found in <this object>` (the same evidence the weak-extab
  removal already uses), in addition to referencers listed `UNUSED`. Here the
  object is @866 (4 bytes, .sdata2 +0x4), its only referencer
  getWeight__13J3DAnmClusterCFUs (map lines 21432-21434), its own UNUSED line
  69924, and the layout lines 69922/69923/69925 (next object 0 bytes after
  the stripped end). Then flip J3DAnmLoader.cpp to Matching.

## Resistant
None in code.
