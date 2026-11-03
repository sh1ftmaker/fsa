# JSystem/JKernel/JKRHeap.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4788 / 11561 functions, 863600 / 4442088 bytes of code (19.44 %)

Status: 48/48 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
- Source: added `s32 JKRHeap::getSize(void*) { return do_getSize(ptr); }`
  between resize() and getFreeSize() (tww's JKRHeap.cpp has it at the same
  place; mkdd dead-strips it). Declared already in JKRHeap.h.
- Placeholders named, each a thin wrapper identified by the JKRHeap vtable
  slot it calls (header slot comments) and its position in the tww/mkdd order:
  - 0x800839B8 freeAll__7JKRHeapFv (calls vt 0x2C do_freeAll)
  - 0x80083A10 getSize__7JKRHeapFPv (calls vt 0x3C do_getSize)
  - 0x80083A94 changeGroupID__7JKRHeapFUc (calls vt 0x4C do_changeGroupID)
  - 0x800845C8 state_register__7JKRHeapCFPQ27JKRHeap6TStateUl (`blr`; it is
    __vt__7JKRHeap slot 0x54, between do_getCurrentGroupId and state_compare)

## SHARED-EDITS
None.
