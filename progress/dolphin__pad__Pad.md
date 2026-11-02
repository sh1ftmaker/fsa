# dolphin/pad/Pad.c — checkpoint

Baseline (start): `total: 1132 / 11560 functions, 202096 / 4434096 bytes of code (4.56 %)`
Now:              `total: 1135 / 11560 functions, 202744 / 4434096 bytes of code (4.57 %)`

## Status: Matching (all 20 functions, .text/.data/.bss/.sdata/.sbss 100 %)

- splits: .text end 0x8004F478 -> 0x8004F4F4 (pulls __PADDisableRecalibration out of
  auto_03_8004F478_text; fn_8004F4F4 left there, it is ai.c's AIRegisterDMACallback);
  added .data 0x804967B8-0x80496810 (version string + ResetFunctionInfo).
- symbols: fn_8004F308 -> OnReset (registered via ResetFunctionInfo, body = donor OnReset),
  lbl_80496800 -> ResetFunctionInfo ({OnReset, 127}), lbl_804B5B50 -> Type,
  new CmdProbeDevice = .bss:0x804B5B90 (PADInit stores 0x4D<<24|chan<<22|... there).
- Fakematch removed: `#pragma auto_inline off // fakematch most likely` around
  PADReset..PADRead. It compensated for PADReset/PADRecalibrate being small enough to be
  auto-inlined into PADInit/OnReset. The real source has
  `ASSERTMSG(!(mask & 0x0FFFFFFF), "PADReset(): invalid mask")` at the top of both
  (dolsdk2004 Pad.c lines 0x381/939, from the debug build); in release it is `((void)0)`
  but still counts toward the auto-inline size, so both stay out of line naturally.

## SHARED-EDITS
none
