# JSystem/JKernel/JKRMemArchive.cpp

Baseline progress: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4785 / 11561 functions, 863524 / 4442088 bytes of code (19.44 %)

Status: 9/9 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok,
`link_check --unit` byte-identical).

## What it took
No source change. 0x80084A08 fn_80084A08 -> removeResource__13JKRMemArchiveFPv
(calls findPtrResource, clears the entry's data pointer, returns true; sits
between removeResourceAll and fetchResource_subroutine as in mkdd; the
vtable's removeResource slot points at it).

## SHARED-EDITS
None.
