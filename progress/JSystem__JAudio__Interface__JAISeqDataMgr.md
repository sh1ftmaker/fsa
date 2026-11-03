# JSystem/JAudio/Interface/JAISeqDataMgr.cpp

New unit (was unsplit, 0x80024B50..0x80024BE0, between JAISeq and JAISeqMgr).
mkdd's file as it is. 0/2 -> 2/2 functions, Matching; `link_check` SHA-1 OK.

- .text 0x80024B50-0x80024BE0: ~JAISeqDataUser, ~JAISeqDataMgr.
- .data 0x80491790-0x804917C0: __vt__13JAISeqDataMgr, __vt__14JAISeqDataUser.
  mkdd's .rodata/.sdata2 (assert strings) don't exist in FSA's release build.
- Matches with and without `-sym off` (tested; links SHA-1 OK both ways).

## SHARED-EDITS
- symbols.txt: `__vt__14JAISeqDataUser` 0x18 -> 0x14 (a real symbol: needs an
  approved_symbol_changes.txt line). 2 header words + dtor + isUsingSeqData +
  releaseSeqData = 0x14; +0x14 is 0, the .balign 8 pad before
  __vt__13JAISeqMgr's unit at 0x804917C0; same pattern as
  `__vt__10JAIAudible 0x18 0x14`. With 0x18, .data is 96.1 %.
