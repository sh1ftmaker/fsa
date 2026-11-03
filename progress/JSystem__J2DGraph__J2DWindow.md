# JSystem/J2DGraph/J2DWindow.cpp

Baseline (progress.py first line at start): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %) (data only)

Status: NonMatching. 32/33 functions (unchanged); .rodata 100 % (was 97.14 %). gate: ok.

## Changes
- symbols.txt: .rodata placeholders lbl_80463040/50/60/70 -> @465, @482, @1416, @1425
  (scope:local), the const tables our object emits at the same offsets; lbl_80463070
  (0x18) becomes @1425 (0x14), the last 4 bytes are padding before the next unit.
- FIXME comment on drawFrameTexture(JUTTexture*, f32, f32, bool, bool, bool).

## Resistant
- drawFrameTexture__9J2DWindowFP10JUTTextureffbbb 90.60 %: register allocation of the
  inlined 10-argument drawFrameTexture. Target: texture r31, doInvertU r30, doInvertV r29,
  this r28, and the FIFO base `lis r31,0xcc01` reuses r31 after the load/setTevMode
  calls; ours: texture r29, doInvertU r31, doInvertV r30, FIFO base r8. Tried:
  tp's form (four u16 locals set by if/else before the call): 52 % (the masks get computed
  before the call into r26-r31); width/height locals before the call: no change;
  volatile GXWGFifo (see J2DPictureEx's checkpoint): 83 %, and drawFrameTexture(...UsUsUsUsb)
  drops to 92 %. Genuine codegen difference, not blocked by another unit.

## SHARED-EDITS
- none.
