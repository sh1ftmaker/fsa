# JSystem/J3DGraphAnimator/J3DModelData.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4785 / 11561 functions, 863536 / 4442088 bytes of code (19.44 %)

Status: **Matching**. 7/7 functions (was 6/7), .text/.data/.sbss 100 %; `link_check.py --unit`
SHA-1 OK; gate ok.

## Changes
- symbols.txt: fn_800DF798 -> __dt__13J3DShapeTableFv. Evidence: it is the only slot (dtor slot,
  index 2) of __vt__13J3DShapeTable at 0x8049BA98 in this unit's .data, and it stores
  __vt__13J3DShapeTable before the conditional `operator delete` (standard deleting dtor).
  Our object emits the same weak dtor at the same position (after the ctor).

## SHARED-EDITS
None.
