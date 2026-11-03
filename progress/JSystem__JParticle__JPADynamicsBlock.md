# JSystem/JParticle/JPADynamicsBlock.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4321 / 11561 functions, 773348 / 4442088 bytes of code (17.41 %)

Status: Matching. 10/10 functions (was 9/12 with 2 foreign functions in the range), .text/.data/.sdata2 100 %.
gate: ok (link SHA-1 OK).

## Changes
- symbols.txt: `fn_800EAF38` -> `__ct__16JPADynamicsBlockFPCUc` (byte-identical to our ctor;
  JPAResourceLoader::load_jpc calls it in the 'BEM1' case after `new JPADynamicsBlock`; the same
  line is on branch unit/JSystem__JParticle__JPAResourceLoader).
- splits.txt: .text end 0x800EB5A0 -> 0x800EB208. The two functions after create() belong to
  JPAEmitter.cpp (tp's JPAEmitter.cpp starts with them, in this order):
  - fn_800EB208 (0x48): stores vtable lbl_80498978 and optionally deletes: `~JPAEmitterCallBack`
    (called from game code), our JPAEmitter.cpp line 8.
  - fn_800EB250 (0x350): called from JPAEmitterManager::createSimpleEmitterID, stores manager/resource
    at +0xE4/+0xE8 and reads lbl_8053BD90 (JPAEmitter's .sdata2): `JPABaseEmitter::init`.
  They now fall into an auto split until JPAEmitter.cpp's stanza is extended.
- init_jpa (pikmin2's empty "UNUSED FUNCTION" stub) is not in FSA; dead-stripped at link.

## SHARED-EDITS
- splits.txt: JPAEmitter.cpp .text start 0x800EB5A0 -> 0x800EB208 (covers ~JPAEmitterCallBack and
  JPABaseEmitter::init, which its source already defines); then name fn_800EB208 / fn_800EB250
  from its compiled object.
