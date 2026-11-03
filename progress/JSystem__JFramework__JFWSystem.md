# JSystem/JFramework/JFWSystem.cpp

Baseline progress: total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4320 / 11561 functions, 773192 / 4442088 bytes of code (17.41 %)

Status: 1/1 functions, all sections 100 %, `Matching`, gate ok (link SHA-1 ok).
No source change: only callee/data names were placeholders.

## Names (symbols.txt)
- init__10JUTGamePadFv (0x8008D110): PADSetSpec(5), sAnalogMode = 3,
  PADSetAnalogMode(3), PADInit(); called where JFWSystem::init calls
  JUTGamePad::init(). Same line as on unit/JSystem__JUtility__JUTGamePad.
- create__12JUTExceptionFP14JUTDirectPrint (0x80089FF0): news a JUTException
  thread with the passed JUTDirectPrint once (sErrorManager == 0) and installs
  errorHandler via OSSetErrorHandler; JFWSystem passes JUTDirectPrint::start()'s result.
- createConsole__12JUTExceptionFPvUl (0x8008C944): getLineFromObjectSize(size, 50),
  JUTConsole::create(50, buf, size), setDirectConsole; JFWSystem passes the
  exConsoleBufferSize block.
- JUTResFONT_Ascfont_fix12 (0x8045EEE0, 0x4160 bytes, starts "FONTbfn1"): what
  CSetUpParam::systemFontRes points to (same size as TWW's array).

firstInit() is inlined into init() and dead-stripped from the DOL; it stays in
the source (the link still matches).

## SHARED-EDITS
- symbols.txt names in JUTException's and JUTGamePad's ranges and the font blob
  (no unit owns 0x8045EEE0).
