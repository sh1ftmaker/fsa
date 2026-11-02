# JSystem/JUtility/JUTDirectPrint.cpp

Baseline (progress.py): total 1307 / 11560 functions, 241984 / 4434096 bytes.
After: total 1321 / 11560 functions, 246572 / 4434096 bytes. gate: ok.

Status: Matching. 8/8 functions, .text/.data/.sbss/.sdata2 100 %. No source change
(no CW_FORCE_* in this file; already -O4,p). The ctor and printSub are inlined in FSA.

## What changed (symbols.txt only, plus the flag)
- erase__14JUTDirectPrintFiiii (0x80089924: fills a rectangle with 0x1080, called by
  JUTConsole::doDraw with 4 ints), drawChar__14JUTDirectPrintFiii (0x80089A1C: font
  lookup /5 and *7 into sFontData/sFontData2, called from drawString_f).
- .data placeholder lbl_804990C8 (0x2C8) split into sAsciiTable (0x80), sFontData (0x100),
  sFontData2 (0x134), twiceBit$118 (0x10, drawChar's local static), 4 bytes padding;
  sizes are the arrays' own and the bytes are identical.
- sFontData/sFontData2 marked `noreloc`: two font words (0x80020800 at 0x80499220,
  0x80283C38 at 0x80499360) look like addresses and dtk made relocations of them.
- Externals: __nw__FUl (sCurrentHeap->alloc(size, 4); sibling of __nw__FUlP7JKRHeapi),
  vsnprintf (MSL printf.c body).
- configure.py: JUTDirectPrint -> Matching.

## SHARED-EDITS
- symbols.txt external renames (__nw__FUl in JKRHeap, vsnprintf in MSL). vsnprintf and
  erase__14JUTDirectPrintFiiii are named identically on the JUTConsole branch.
- dtk proposed fn_8006B6D4 -> __StringWrite; not committed (not my unit).
