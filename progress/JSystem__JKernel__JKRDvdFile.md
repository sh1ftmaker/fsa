# JSystem/JKernel/JKRDvdFile.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4793 / 11561 functions, 864388 / 4442088 bytes of code (19.46 %)

Status: 14/14 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
No source change. All six misses were placeholder names in symbols.txt:
- 0x8007EA0C open__10JKRDvdFileFPCc (calls DVDOpen; vtable slot 0x0C, as JKRFile::open(const char*))
- 0x8007EA84 open__10JKRDvdFileFl (calls DVDFastOpen; vtable slot 0x20)
- 0x8007EC40 writeData__10JKRDvdFileFPCvll (`li r3,-1`; vtable slot 0x18, between readData and getFileSize)
- 0x8007ECCC getFileSize__10JKRDvdFileCFv (weak; `lwz r3,0x90` = mDvdFileInfo.length; vtable slot 0x1C)
- 0x8007ECD4 __sinit_JKRDvdFile_cpp (the .ctors entry; initiates/registers sDvdList)
- 0x8007ED18 __dt__21JSUList<10JKRDvdFile>Fv (the destructor __sinit registers)
- .bss 0x804B9FE8 lbl_804B9FE8 (0x10) -> @130 (0xC, scope:local): the
  __register_global_object chain entry, named as the compiler emits it (same
  convention as @123/@150 in JKRAramHeap/JKRFileFinder).
Names and order match mkdd's MarioClub_us symbols.txt for the same unit.

## SHARED-EDITS
None (symbols.txt only within this unit's ranges).
