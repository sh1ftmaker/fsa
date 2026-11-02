# JSystem/JUtility/JUTConsole.cpp

Baseline (progress.py): total 1307 / 11560 functions, 241984 / 4434096 bytes.
After: total 1340 / 11560 functions, 248940 / 4434096 bytes. gate: ok.

Status: Matching. 29/29 functions, .text/.data/.sbss/.sdata2 100 %.

## What changed
- splits.txt: .text end 0x80088BE4 -> 0x800891B8 (appendConsole .. JUTWarningConsole);
  added .data 0x804990B8-0x804990C8 (__vt__10JUTConsole: 0,0,~JUTConsole).
  0x800891B8-0x80089264 is JUTDbPrint::start (new 0x14, sbss 0x8053A8F0), not this unit.
- Source: removed the three `#ifdef DEBUG CW_FORCE_STRINGS` fakes. No dead-stripped
  replacement: FSA has none of dumpToTerminal's strings, and writing it out adds a
  "%s\n" .sdata2 literal FSA's object lacks. Comment left in its place.
- symbols.txt (evidence):
  - create__10JUTConsoleFUiPvUl (getLineFromObjectSize + ctor(.., false)), __dt__10JUTConsoleFv
    (vtable slot, removeConsole + ~JKRDisposer), __dt__13J2DOrthoGraphFv (weak, vtables),
    removeConsole/draw/drawDirect/setDirectConsole (bodies match mkdd source exactly),
    JUTSet/GetReportConsole, JUTSet/GetWarningConsole and sReportConsole/sWarningConsole
    (order of tests in removeConsole; JUTReportConsole_f reads 0x8053A8E4),
    sWarningConsole resized 8 -> 4, __vt__10JUTConsole (0x10 -> 0xC, rest is padding).
  - Externals: vsnprintf (MSL: __pformatter with string writer, NUL at min(n-1,len)),
    __nw__FUlP7JKRHeapi (sibling of __nwa__FUlP7JKRHeapi, same body), __dt__11JKRDisposerFv,
    erase__14JUTDirectPrintFiiii (fills 0x1080 rectangle; also named on the JUTDirectPrint branch).
- configure.py: JUTConsole -> Matching.

## SHARED-EDITS
- splits.txt: JSUFileStream.cpp .data end 0x804990C8 -> 0x804990B8 (the JUTConsole vtable
  was inside its range).
- symbols.txt: external renames above (MSL, JKRHeap, JKRDisposer, JUTDirectPrint).
- dtk proposed fn_8006B6D4 -> __StringWrite; not committed (not my unit).
