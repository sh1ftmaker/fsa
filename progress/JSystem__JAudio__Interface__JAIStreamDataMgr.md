# JSystem/JAudio/Interface/JAIStreamDataMgr.cpp

New unit (was unsplit, 0x80026FD8..0x80027020). mkdd's file as it is.
0/1 -> 1/1, Matching; `link_check` SHA-1 OK.

- .text: ~JAIStreamDataMgr; .data 0x80491870 __vt__16JAIStreamDataMgr.
- ~JAIStreamAramMgr and __vt__16JAIStreamAramMgr are compiled but dead-stripped
  in FSA: ninja's map lists both UNUSED for JAIStreamDataMgr.o.
- Matches with and without `-sym off` (tested; links SHA-1 OK both ways).
