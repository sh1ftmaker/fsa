# JSystem/JAudio/System/JASDSPInterface.cpp

Baseline (progress.py): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4505 / 11561 functions, 808304 / 4442088 bytes of code (18.20 %); gate: ok.

Status: 32/32 functions 100 %; .text/.rodata/.data/.sbss/.sdata2 100 %, .sdata 66.67 % (4 of 8 bytes,
see below). NonMatching until one coordinator-approved size change; with it the unit is 100 %
everywhere and the main link is SHA-1 OK (tested: gate's only complaint was the size change itself).

## Needed (coordinator, approved_symbol_changes.txt)
    one__11JASWaveInfo  0x8  0x4  (u32 JASWaveInfo::one = 1; the other 4 bytes are .sdata
    alignment padding before 0x80539BE0, like FatalFunc's)
Then flip configure.py to Matching.

## Done
- The 1.4 KB "data gap" was a false relocation: dtk read DSPRES_FILTER's word 0x80018800
  (offset 0x2E0, a filter coefficient pair in the middle of the descending 0x..01 run) as a
  pointer into .text and zeroed it in the split. `noreloc` on DSPRES_FILTER__6JASDsp (as on
  jdsp and JUTDirectPrint's font data) -> .rodata 100 %.
- Names: fn_8002CDCC releaseHalt__6JASDspFUl (DSPReleaseHalt2), fn_8002CDEC finishWork__6JASDspFUs
  (DspFinishWork), fn_8002CE0C syncFrame__6JASDspFUlUlUl (DsyncFrame2): byte-identical bodies at the
  same offsets as this file's functions; JASAiCtrl's finishDSPFrame calls syncFrame (the JASAiCtrl
  branch makes the same syncFrame rename, identical line).
- Split .sdata 0x80539BD8-0x80539BE0 (JASWaveInfo::one, defined in this file; without it the link
  has a duplicate JASWaveInfo::one).

## SHARED-EDITS
- approved_symbol_changes.txt line above.
