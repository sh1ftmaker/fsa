# JSystem/JKernel/JKRThread.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4787 / 11561 functions, 863640 / 4442088 bytes of code (19.44 %)

Status: 12/12 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
- 0x80085878 fn_80085878 -> run__9JKRThreadFv (weak, `li r3,0`; __vt__9JKRThread slot 0x0C).
- .text end 0x80085968 -> 0x80085A10: the two weak JSUList dtors
  (__dt__17JSUList<7JKRTask>Fv, __dt__19JSUList<9JKRThread>Fv) that this
  unit's __sinit registers sat in the auto split after it; they are the tail
  of this object (same order in the compiled .o).
- .bss lbl_804BA058/lbl_804BA064 -> @117/@118 (0xC, scope:local), the two
  __register_global_object chain entries __sinit passes.
- .rodata 0x8045E820..0x8045E878 and .sdata2 0x8053B770..0x8053B788 removed
  from this stanza: no JKRThread function references them; their only users
  are in the unsplit code at 0x80085A10.. (fn_80085CB8.. uses lbl_8053B770, a
  {0x80,0x40,...,0x01} bit table; fn_80086890 uses "check: bad total memory
  block size" at lbl_8045E820). Those are a different unit (stmw prologues, a
  heap with 0x88-byte headers), not JKRThread.
- .sbss: deleted placeholder lbl_8053A8D4 (4 bytes, nothing references it).
  The trial-link map (build/G4SE01/weakdedup_evidence/JSystem__JKernel__JKRThread.MAP)
  puts sManager__15JKRThreadSwitch at 0x8053A8D0 as the only kept object of
  JKRThread.o's .sbss and starts JUTAssert.o's .sbss at 0x8053A8D8: the 4
  bytes are alignment padding.
- Removed two `#ifdef DEBUG CW_FORCE_STRINGS(...)` lines (no release effect;
  the gate rejects them in a Matching unit).

## SHARED-EDITS
- The .rodata 0x8045E820..0x8045E878 and .sdata2 0x8053B770..0x8053B788 now
  fall to dtk's auto split; whoever splits the code at 0x80085A10..0x80086A9C
  should claim them.
