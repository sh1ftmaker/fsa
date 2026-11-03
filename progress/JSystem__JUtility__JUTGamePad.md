# JSystem/JUtility/JUTGamePad.cpp

Baseline progress: total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4338 / 11561 functions, 777488 / 4442088 bytes of code (17.50 %)

Status: 32/32 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok).

## What it took
- Source moved from mkdd's (LGWheels) version to FSA's: TP/pikmin2-style read(),
  update() with the _A8 gate, inlined checkResetCallback without port check,
  clearForReset, CRumble patterned rumble/setEnabled/getGamePad.
- splits: .sbss starts at 0x8053A920 (0x8053A918/0x8053A91C are JUTException's,
  written by its error handler at 0x8008A250); .sdata2 0x8053B890-0x8053B8C8 added.
- symbols: 10 fn_ and 6 lbl_ placeholders named (mangled names from TP/pikmin2/TWW,
  each confirmed by its use in the bytes).
- Header commit: JUTGamePad.h (_A8 field, isEnabledPort without range check,
  setEnabled etc., LGWheels removed), std/math.h (sqrtf constants not static).

## SHARED-EDITS
- libs/sdk/include/std/math.h: sqrtf's _half/_three no longer static (only compiled
  caller is JUTGamePad.cpp; FSA has no $localstatic objects).
- libs/sdk/include/JSystem/JUtility/JUTGamePad.h: also included by JFWSystem.cpp.
- JUTException.cpp stanza: .sbss end should be 0x8053A920 (now 0x8053A918), so it
  owns lbl_8053A918/lbl_8053A91C (TP: JUTException msr/fpscr-style statics).
