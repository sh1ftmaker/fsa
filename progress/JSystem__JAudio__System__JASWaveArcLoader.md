# JSystem/JAudio/System/JASWaveArcLoader.cpp

Baseline (progress.py): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
After: total: 4504 / 11561 functions, 808104 / 4442088 bytes of code (18.19 %); gate: ok.

Status: **Matching** (8/8 functions, .text/.data/.sbss 100 %), gate: ok, main link SHA-1 OK.
No source change: splits and names. The flip relies on the mkdd_JAudio block's `-sym on`
(infra/jaudio-sym, which also removed mkdd's `#line`s); the per-unit flag the review rejected is gone.

## The flag: `-sym on` (evidence: the link)
With plain flags (first attempt, below) the link fails. MWCC 2.6 with `-sym on` puts the out-of-line
copies of inline/weak functions in a separate .text section after the file's own functions, which
is exactly FSA's layout (weak copies at the end of each file, as in TP, whose JSystem is built with
`-sym on`, and SMG); mkdd's map shows the immediate placement. With `-sym on` the trial link is
SHA-1 OK. Caveat for other units: with `-sym on`, every `#line` directive (mkdd's assert-line fakes,
in this .cpp and in shared headers such as JASGadget.h, JASHeapCtrl.h, linklist.h, JUTGamePad.h)
starts a new .text section, which reorders functions. Here only the dead-stripped
getRootHeap/setRootHeap land in such a section, so it doesn't matter; JASBasicInst needs the
`#line`s gone from its includes first (see its checkpoint).

## First attempt without the flag (why it couldn't link)
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

## SHARED-EDITS (applied in infra/jaudio-sym)
- JASBank.cpp's stanza starts at 0x80031484 (noteOn).
