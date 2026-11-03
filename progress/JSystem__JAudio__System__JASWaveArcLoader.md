# JSystem/JAudio/System/JASWaveArcLoader.cpp

Baseline (progress.py): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4504 / 11561 functions, 808104 / 4442088 bytes of code (18.19 %); gate: ok.

Status: 8/8 functions, .text/.data/.sbss 100 % in the honest report, but **NonMatching**: the
flipped link fails (SHA-1). No source change: splits and names only.

## Why it can't link (resistant: layout)
Our object emits the weak __dt__11JASDisposerFv right after the JASWaveArc ctor (first use, as in
mkdd's map); FSA has it at the end of the file (0x8003143C, after setFileName), as TP does.
Its copy is the one the link keeps, so every call to it (JASBasicWaveBank, JASSimpleWaveBank,
our dtor) shifts. Same compiler-emission-order issue as JASBasicInst/JASTrack/JASBasicBank
(see the JASBasicInst checkpoint for what was tried). Project-level problem.

## Done
- .text end 0x80031750 -> 0x80031484: 0x80031484 is JASBank::noteOn (first function of
  JASBank.cpp in TP; JASBank's range starts at noteOnOsc 0x80031750). FSA's file ends with the
  weak __dt__11JASDisposer, as weak copies end files in FSA.
- Names from the vtables (.data relocations): fn_80030EE8 / fn_80030EEC -> onLoadDone / onEraseDone
  __10JASWaveArcFv (slots 4/5 of __vt__10JASWaveArc; the header's inline empty virtuals, kept from
  JASSimpleWaveBank's copies), fn_80028D9C -> onDispose__11JASDisposerFv (slot 3 of __vt__11JASDisposer,
  JASDisposer.h's empty inline). Data 92.8 % -> 100 %.

## SHARED-EDITS
- JASBank.cpp's stanza should start at 0x80031484 (noteOn); the gap is an auto split now.
