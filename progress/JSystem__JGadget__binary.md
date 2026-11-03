# JSystem/JGadget/binary.cpp

Baseline (progress.py first line at start, this worktree): total: 4523 / 11561 functions, 819036 / 4442088 bytes of code (18.44 %)
After: total: 4524 / 11561 functions, 819108 / 4442088 bytes of code (18.44 %)

Status: **Matching**. 2/2 functions (was 1/1), .text 100 %. `link_check.py --unit` links byte-identical
(SHA-1 d86b3d9f...); `gate: ok` with the flag flipped.

## Why it broke the link
`link_check.py` with the unit Matching: `dup JGadget::binary::TParse_header_block::~TParse_header_block()`
(multiply-defined, previously in JSUFileStream.o). The dtor (0x80087714, 0x48) is binary.cpp's first
function in FSA, but the split gave it to JSUFileStream.cpp (.text end 0x8008775C), so dtk's split of
JSUFileStream defined it too.

## Fix
splits.txt: binary.cpp .text start 0x8008775C -> 0x80087714 (JSUFileStream.cpp .text end moves with it).
JSUFileStream.cpp (NonMatching) defines no function there; its numbers are unchanged.

## Notes
- Our object also has `parseVariableUInt_16_32_following` (unreferenced, dead-stripped by the link) and a
  weak `__vt__Q37JGadget6binary19TParse_header_block` in .data. FSA keeps the vtable at 0x804989D0 in
  JMessage/resource.cpp's .data (earlier in link order); binary.cpp has no .data split, and the link drops
  our weak copy, so the SHA-1 matches.

## SHARED-EDITS
- splits.txt: JSUFileStream.cpp .text end 0x8008775C -> 0x80087714 (the other side of this unit's start).
