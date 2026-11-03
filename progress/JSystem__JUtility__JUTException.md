# JSystem/JUtility/JUTException.cpp

Status: **Matching** (24/24 functions, all sections 100 % in report_honest; gate ok).
Baseline progress.py: 4501 / 11561 functions, 808092 bytes (18.19 %).
After: 4522 / 11561 functions, 818928 bytes (18.44 %).

## What changed
- Unit moved from the TWW-derived src/JSystem/JUtility/JUTException.cpp
  (deleted) to the mkdd copy already in libs/jsystem (mkdd_JUtility block,
  -O4,p). FSA's layout equals pikmin2's GPVE01 JUTException exactly (same
  24 functions, same sizes, same order; .bss 0x2E8, .sdata2 0x48, .data 0x98).
- Source edits vs mkdd: search_name_part walks a separate pointer (tp/pikmin2
  form) instead of reassigning src (fixed showGPRMap's register allocation);
  printContext's final scroll loop and queryMapAddress_single use pikmin2's
  loop form (no goto, mkdd's "fake match" / "memes" comments removed).
- splits.txt: .bss end 0x804BA1D0 -> 0x804BA498 (panic_f_va's static
  OSContext, 0x2C8), .sbss end 0x8053A918 -> 0x8053A920 (msr, fpscr; approved
  by the JUTGamePad review), new .sdata2 0x8053B848-0x8053B890.
- symbols.txt: 17 fn_ placeholders named from pikmin2 (identical size/order)
  + our object's mangled names; data placeholders named (sMessageQueue,
  c3bcnt, sCpuExpName, __vt__12JUTException (0x14 -> 0x10), exCallbackObject,
  sMessageBuffer (0x8 -> 0x4), sErrorManager..fpscr); the two .rodata string
  blobs lbl_8045E934/lbl_8045EC1C split into one lbl_ per string.

## Notes
- showFloatSub keeps mkdd's `*(u8*)&value & 0x80` (MSL's __signbitf
  expansion; our MSL math_api.h lacks a __signbit macro).
- lbl_804BA1C4 (sMapFileList destructor-chain object) and lbl_804BA1D0
  (panic_f_va's static context) left as placeholders (compiler-local names).

## SHARED-EDITS
none
