# JSystem/JKernel/JKRDecomp.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4787 / 11561 functions, 863776 / 4442088 bytes of code (19.45 %)

Status: 12/12 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
No source change; placeholder names only:
- 0x8007E0E4 __dt__9JKRDecompFv (vtable slot 0x08; stores the vtable, calls
  __dt__9JKRThreadFv)
- 0x8007E20C sendCommand__9JKRDecompFP16JKRDecompCommand (OSSendMessage to
  sMessageQueue; called from JKRAramPiece, as in mkdd's JKRDecomp.cpp)
- .data 0x80498D40 lbl_80498D40 -> __vt__9JKRDecomp (dtor + run__9JKRDecompFv)
The JKRAramPiece caller of sendCommand also gains a match (+1 function).

## SHARED-EDITS
None.
