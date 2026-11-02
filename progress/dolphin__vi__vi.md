# dolphin/vi/vi.c

Matched 23/24 functions (was 16/24). .data/.sdata/.sbss 100 %, .bss 40 %.
Unit stays NonMatching only because of placeholder data symbols whose sizes
the gate forbids changing; with the SHARED-EDITS below applied the whole unit
is 100 % (verified locally: every function and section 100 %).

## Done
- symbols.txt renames (donor vi.c order + bodies): VISetPreRetraceCallback,
  VISetPostRetraceCallback, VIConfigure, VIGetNextFrameBuffer,
  VIGetCurrentFrameBuffer, VIGetRetraceCount, message$331 (PrintDebugPalCaution
  static, name from our object), __shl2i / __shr2u (Runtime.c helpers; bodies
  match MSL Runtime order __div2u, __div2i, __mod2u, __mod2i, __shl2i, __shr2u).
- VIConfigure: panic call uses OSPanic(__FILE__, 1908, ...) directly (as tp's
  dolphin vi.c does); OSErrorLine expanded to __LINE__ without MATCHING.

## Resistant
- VIConfigure 99.92 %: `addi r3, r15, 0x260..0x368` arg_mismatch. objdiff
  resolves r15-relative refs to target lbl_80496648 (one 0x154 blob) vs our
  per-string @335..@340/@523. Fixed by the split below.

## SHARED-EDITS (config/G4SE01/symbols.txt, replace lines)
```
lbl_804963E8 = .data:0x804963E8; // type:object size:0x1F4
->
@3 = .data:0x804963E8; // type:object size:0x44 scope:local data:string
timing = .data:0x8049642C; // type:object size:0x17C scope:local
taps = .data:0x804965A8; // type:object size:0x32 scope:local data:2byte

lbl_80496648 = .data:0x80496648; // type:object size:0x154
->
@335 = .data:0x80496648; // type:object size:0x29 scope:local data:string
@336 = .data:0x80496674; // type:object size:0x29 scope:local data:string
@337 = .data:0x804966A0; // type:object size:0x29 scope:local data:string
@338 = .data:0x804966CC; // type:object size:0x29 scope:local data:string
@339 = .data:0x804966F8; // type:object size:0x29 scope:local data:string
@340 = .data:0x80496724; // type:object size:0x29 scope:local data:string
@523 = .data:0x80496750; // type:object size:0x4B scope:local data:string

lbl_804B5A08 = .bss:0x804B5A08; // type:object size:0xF0 data:2byte
->
regs = .bss:0x804B5A08; // type:object size:0x76 scope:local data:2byte
shdwRegs = .bss:0x804B5A80; // type:object size:0x76 scope:local data:2byte
```
Then rm build/G4SE01/config.json, rebuild, and set dolphin/vi/vi.c to Matching
in configure.py.
