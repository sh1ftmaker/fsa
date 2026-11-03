# JSystem/JKernel/JKRDvdRipper.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4795 / 11561 functions, 864540 / 4442088 bytes of code (19.46 %)
(includes the JKRDvdFile placeholder names below, which this branch repeats)

Status: 10/10 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
No source change; symbols only.
- The three loadToMainRAM overloads were blocked by JKRDvdFile's placeholder
  names (bl fn_8007EA0C = open__10JKRDvdFileFPCc, etc.). This branch carries
  the same seven JKRDvdFile lines as unit/JSystem__JKernel__JKRDvdFile
  (identical edits, merge cleanly).
- .rodata lbl_8045E2A0 (0x60 blob) split into its three strings, so the
  `+0x14`/`+0x38` references resolve to their own objects:
  lbl_8045E2A0 "JKRDvdRipper.cpp" (0x11), lbl_8045E2B4 "Sorry, not applied
  for SZP archive." (0x24), lbl_8045E2D8 "Not support SZP with offset read" (0x21).
- 0x800813BC __sinit_JKRDvdRipper_cpp (the .ctors entry; initiates/registers
  sDvdAsyncList), 0x80081400 __dt__23JSUList<12JKRDMCommand>Fv (the
  destructor it registers), .bss lbl_804BA020 -> @138 (0xC, scope:local, the
  __register_global_object chain entry, as the compiler names it).

## SHARED-EDITS
- symbols.txt JKRDvdFile range (0x8007EA0C..0x8007ED18, .bss 0x804B9FE8):
  same lines as unit/JSystem__JKernel__JKRDvdFile (also mine).
