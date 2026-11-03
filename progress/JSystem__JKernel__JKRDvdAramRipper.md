# JSystem/JKernel/JKRDvdAramRipper.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4796 / 11561 functions, 864652 / 4442088 bytes of code (19.46 %)
(includes the JKRDvdFile placeholder names below, which this branch repeats)

Status: 15/15 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
No source change; symbols only.
- loadToAram(long, ...) was blocked by JKRDvdFile's placeholder (bl
  fn_8007EA84 = open__10JKRDvdFileFl). This branch carries the same seven
  JKRDvdFile lines as unit/JSystem__JKernel__JKRDvdFile (identical edits).
- 0x8007F2B4 __dt__18JSUFileInputStreamFv (weak; stores __vt__18JSUFileInputStream
  then __vt__20JSURandomInputStream, calls __dt__14JSUInputStreamFv; instantiated
  by callCommand_Async's local JSUFileInputStream)
- 0x8007FAD0 __sinit_JKRDvdAramRipper_cpp (the .ctors entry; sDvdAramAsyncList)
- 0x8007FB14 __dt__23JSUList<12JKRADCommand>Fv (the destructor __sinit registers)
- .bss lbl_804B9FF8 -> @137 (0xC, scope:local, __register_global_object chain entry)
- .bss lbl_804BA004 (0x1C) -> decompMutex (0x18, scope:local; the file-static
  OSMutex, same name as JKRDvdRipper's/JKRDecomp's)

## SHARED-EDITS
- symbols.txt JKRDvdFile range: same lines as unit/JSystem__JKernel__JKRDvdFile (also mine).
