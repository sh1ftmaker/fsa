# JSystem/J3DGraphBase/J3DGD.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4324 / 11561 functions, 773352 / 4442088 bytes of code (17.41 %)

Status: NonMatching. 27/29 functions (was 26/33; 4 functions moved to J3DMatBlock, see below),
.data/.sdata/.sdata2 100 %, .text 99.66 %.

## Changes
- J3DGDSetTexCoordGen: tp's case order/meaning (GX_TG_MTX2x4 -> tgType 0; GX_TG_MTX3x4 ->
  tgType 0, proj 1). pikmin2 had proj on the wrong case.
- J3DFifoLoadNrmMtxToTexMtx(3x3): the 0.0f words via tp's J3DGXCmd1f32 (union), mtx words via
  a plain `*(u32*)` read (tp's J3DGXCmd1f32ptr). 77 % -> 87 %.
- splits.txt: the J3DGD/J3DMatBlock boundary 0x800D3690 -> 0x800D3470. 0x800D3470-0x800D3690 is
  J3DColorBlockLightOff/AmbientOn/LightOn::initialize and J3DTexGenBlockPatched::initialize, the
  first four functions of J3DMatBlock.cpp in pikmin2 and tp; all four match there (100 %).

## Resistant
- J3DFifoLoadNrmMtxToTexMtx / ...3x3 (87 %): target reloads each stacked 0.0f word right before
  its FIFO store (`lwz r0,0x10(r1); stw r0,-0x8000(r6)`), we hoist the reloads into r4/r5 earlier.
  Tried: pikmin2's locals + volatile read (77 %), volatile read of the inline's parameter (87 %),
  union (87 %). Needs another way to write the inline.

## SHARED-EDITS
- splits.txt: J3DMatBlock.cpp .text start 0x800D3690 -> 0x800D3470 (that unit gains 4 matched
  functions, 408 -> 412; it is not claimed right now).

## Wave 8 (w8-j3d), 2026-11-03
Baseline: total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %). Unit 27/29, unchanged.
No source change. Stays NonMatching.

J3DFifoLoadNrmMtxToTexMtx / ...3x3 (87.03 %), genuinely codegen (nothing outside the unit blocks it).
Target: the three `stfs f0` of 0.0f go to 0x10/0xc/0x8(r1) early, interleaved with the first row,
and each `lwz r0,<slot>(r1)` comes right before its FIFO store. MWCC hoists our stack reloads
(it knows a stack slot can't alias the FIFO store; mtx loads through r3 stay put).
Variants tried in a scratch harness with the unit's flags (diff lines vs target, 0 = match):
- current (union J3DGXCmd1f32 + `*(u32*)p` WriteF32ToGX): 32
- `*(u32*)&data` on the parameter: 32; tp's GXCmd1u32 inline wrapper: 32
- B(f32 d){ A(&d); }: 32; copy to a local first: 32; macro with block local: 32
- `GXWGFifo.f32 = data` (stfs straight to FIFO): 16, no stack at all
- `const f32&` parameter: 12, but the 0.0f become .rodata constants (`@NNN`), no stack
- locals x/y/z at the top or before use, plain read: 32; array zero[3] in a loop: 27
- pikmin2's `*(volatile u32*)` read: 14 (closest: first two stores early, reloads still hoisted
  by one store); same with locals at top, B(&d), union+A: 14; volatile local: 38
None reproduces it; FIXME in the source stays.
