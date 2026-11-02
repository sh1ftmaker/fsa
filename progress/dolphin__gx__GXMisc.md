# dolphin/gx/GXMisc.c

Baseline (progress.py first line at start): total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1177 / 11560 functions, 213228 / 4434096 bytes of code (4.81 %)

Status: Matching (21/21 functions, .text 100 %, .sbss 100 %).

- .text extended to 0x8005B978-0x8005C1CC: GXSetMisc (first function of the donor file) was in an
  auto split before GXFlush, __GXPEInit after the handlers; the two static interrupt handlers
  lost their _addr suffixes.
- fn_8005BFA4 is GXPeekARGB, not GXPeekZ: the bytes insert 0 into bits 22-23 of the EFB address
  (rlwimi r3, r0(=0), 22, 8, 9); GXPeekZ inserts 1. Added GXPeekARGB from dolsdk2004.

SHARED-EDITS: none.
