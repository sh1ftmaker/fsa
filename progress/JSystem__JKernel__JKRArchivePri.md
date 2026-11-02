# JSystem/JKernel/JKRArchivePri.cpp

Baseline: `total: 1307 / 11560 functions, 241984 / 4434096 bytes of code (5.46 %)`
After:    `total: 1322 / 11560 functions, 245360 / 4434096 bytes of code (5.53 %)`

Unit: 6/11 -> 18/18 functions, .text and .sbss 100 %. Now `Matching`; `gate.py` ok.

- `.text` 0x8007BC30-0x8007C8F0: default ctor, ctor(long, EMountMode), dtor
  before isSameName; store x2, setExpandSize, getExpandSize after
  findIdResource (same order as mkdd; findResType is dead-stripped).
- `.sbss` 0x8053A7F0-0x8053A7F8: `sCurrentDirID__10JKRArchive` (written by
  the ctor next to sCurrentVolume; placeholder resized 0x8 -> 0x4, rest is padding).
- Source: added `JKRArchive::JKRArchive()` (tp/tww body: mIsMounted = false,
  mMountDirection = MOUNT_DIRECTION_HEAD; target stores 0 at 0x30, 1 at 0x60).
- Names: `__ct__10JKRArchiveFv`, ctor/dtor, set/getExpandSize (vtable slots
  18/19 of `__vt__10JKRArchive` @0x80498C60, which the ctors store),
  `strcmp` (MSL string.s, 0x8006D828), `__dt__13JKRFileLoaderFv` (0x80083178:
  resets __vt__13JKRFileLoader, clears sCurrentVolume, JSUPtrLink dtor).

SHARED-EDITS (separate commit): `libs/sdk/include/JSystem/JKernel/JKRArchive.h`
moves `mCompression` (0x5C) / `mMountDirection` (0x60) from the four derived
archive classes into JKRArchive. All other objects in build/G4SE01/src are
byte-identical with and without it. symbols.txt placeholders outside the
unit: strcmp, JKRFileLoader dtor, __vt__10JKRArchive (the vtable's .data
stays unsplit; mkdd puts it in JKRArchivePub).

The same JKRArchive ctor/dtor/set/getExpandSize names are also on branch
unit/JSystem__JKernel__JKRCompArchive (identical lines).
