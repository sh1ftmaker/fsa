# JSystem/JKernel/JKRCompArchive.cpp

Baseline (before this work): `total: 1307 / 11560 functions, 241984 / 4434096 bytes of code (5.46 %)`
After: `total: 1327 / 11560 functions, 248156 / 4434096 bytes of code (5.60 %)`

Unit: 0/3 -> 8/8 functions, .text/.rodata/.data/.sdata2 all 100 %. Now `Matching`; `gate.py` ok (links, SHA-1).

No source change was needed. Fixes were splits and names:

- `.text` start moved 0x8007D4B8 -> 0x8007D2B8 (ctor and dtor sit before `open`),
  end 0x8007DD58 -> 0x8007E034 (removeResourceAll, removeResource,
  getExpandedResSize; sizes sum exactly to JKRDecomp's start).
- `.data` 0x80498CB0-0x80498D00: `__vt__14JKRCompArchive` (slot 2 is the dtor
  at 0x8007D368, slots 9/10/15/16 are this unit's functions).
- `.sdata2` 0x8053B6E8-0x8053B6F0: the "%s" panic format string `@449`.

Names (evidence):
- This unit's functions: from the vtable slots and the ctor/dtor bodies
  (ctor calls JKRArchive ctor with mode 4, stores vtable, calls `open`).
- JKRArchive virtuals in the vtable (becomeCurrent, readResource x2,
  detachResource, getResSize, countFile, getFirstFile, setExpandSize,
  getExpandSize): slot order of JKRArchive's declaration plus each body's
  callees (findFsResource / findNameResource+findTypeResource /
  findPtrResource / findDirectory / JKRArcFinder ctor / mExpandSizes at 0x50).
- `__ct__10JKRArchiveFlQ210JKRArchive10EMountMode` (stores `__vt` 0x80498C60,
  sets sCurrentVolume), `__dt__10JKRArchiveFv`.
- Callees: `__nw__FUlP7JKRHeapi` (heap->alloc via vtable, fallback sCurrentHeap),
  `copyMemory__7JKRHeapFPvPvUl` ((n+3)>>2 word copy, 8x unrolled),
  `alloc__11JKRAramHeap...` (mutex + allocFromHead/Tail),
  `__ct__10JKRDvdFileFl` (JKRDisposer ctor, JSUPtrLink, initiate, open),
  `loadToMainRAM__12JKRDvdRipperFl...`, `loadToAram__16JKRDvdAramRipperFl...`,
  `mainRamToAram`/`aramToMainRam__7JKRAram` (sizes and order as mkdd),
  `abs` (srawi/xor/subf), `sAramObject__7JKRAram` (written by JKRAram::create).

SHARED-EDITS: symbols.txt names outside this unit (placeholders only):
JKRArchivePri/Pub functions above, JKRAram, JKRAramHeap, JKRDvdFile,
JKRDvdRipper, JKRDvdAramRipper, JKRHeap (`__nw`, copyMemory), MSL `abs`.

Note: our object also emits weak `__dt__7JKRFileFv`/`__vt__7JKRFile`
(unreferenced); the link is still byte-identical.
