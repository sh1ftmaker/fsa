# game/tu_8013C5C8 (EffectSystem_8049F178 + three JPAEmitterCallBack classes)

Baseline: 8015 / 11557. After: 8030 (15/27 in the unit, +15 in the honest report).

Classes: EffectSystem_8049F178 (vtable 0x8049F178; ctor 0x8013F3E4), cls_8049F140 (execute spawns 'PEFF'
actors), cls_8049F15C (executeAfter = fn_8013C958 queue), cls_8049F184 (draw/drawAfter swap the particle
camera matrix; global lbl_8053AB18 is one, its sinit is fn_8013F758).

Matched: getEffectSystem_8013C824, fn_8013C830, fn_8013C8CC, fn_8013C958, fn_8013CAA4, setEmitterPos_8013CB44 (reloc-name only),
fn_8013D5B0, fn_8013DC1C, fn_8013DC44, __dt__ EffectSystem, __dt__ x3 callbacks, executeAfter, ctor (same-text).

Resistant: execute (98.7 %, `bne; b end` for the mode 0xC test + r28/r29 register split), drawAfter (97.7 %,
regalloc), sinit (99.6 %, bss registration offsets need the 0xC object of the unwritten big functions).

Unwritten: spawnEmitter_8013CC50 (0x960), fn_8013D5D4/D6E4/D908/DA10/DB2C (draw passes: need JPADrawInfo-typed
Camera_804AA9E4::field_0xe4 and the identity-initialised JPADrawInfo pair), fn_8013F5B0 draw (psq matrix copy),
fn_8013DDEC (0x158C).

SHARED-EDITS: include/game/Effect.h (mHeap is JKRSolidHeap*, fn_8013C958 declared, getEffectSystem_8013C824 / CAA4 / CB44
moved out of extern "C" and CAA4 / CB44 return u8, comments), include/game/RotMatrix.h (fn_8010F668 declaration,
owner act_princess_zelda_demo). Symbols renamed in symbols.txt (callbacks, vtables, Entry ctor, sinit, the
three free functions).

## 2026-11-05 (Opus, leverage wave 1)

Baseline 8410 (matching 5babbf0b, after the final rebase) -> 8412 (+2 honest: fn_8013D908,
fn_8013DA10). Structural +2
(spawnEmitter_8013CC50, draw). Per-address compare against the matching report: 0 drops.

- spawnEmitter_8013CC50 (0x960): written, 99.86 %, code identical (try.py: scheduling+regalloc + reloc-name, the
  only differing lines are the two .sdata2 relocations). A free function, not an EffectSystem member:
  r3 is the layer (cmpwi r3, 0 picks the emitter path vs the helper-actor switch), the system is
  read through gGameRoot2_8053AB10->0x28. Signature unchanged from Effect.h (s32, const TVec3f*, u16,
  s32, s32, JPAEmitterCallBack*, JPAParticleCallBack*, bool, f32); now C++ linkage, symbols.txt
  renamed to spawnEmitter_8013CC50__FlPCQ29JGeometry8TVec3<f>UsllP18JPAEmitterCallBackP19JPAParticleCallBackbf.
  No caller changed %. Findings: the group test is `group != 2 && != 4 && != 5 && != 8 && != 10`
  (an || form folds 4/5 into subi/cmplwi, a switch gives a jump table); the queue loop is
  `Entry& entry = system->mEntries[i]` with system in a local; emitterCB is copied into its own
  local at the top of the layer-0 block (mr r28, r8), `u8 group` declared before it.
- fn_8013D908 / fn_8013DA10: 100 %. Camera_804AA9E4::field_0xe4 is a JPADrawInfo (0xE4-0x143,
  the old field_0xe4 / field_0x114 Mtx pair); JPADrawInfo moved to JParticle/JPADrawInfo.h (tp
  layout) so Camera.h can include it.
- draw (cls_8049F184, 0x8013F5B0): 99.85 %, code identical, .sdata2 relocations only.
- setEmitterPos_8013CB44: still 99.93 % reloc-name. Cause found: the original pool is 0.0, 1.0, pi, -1.0,
  0.5, (pad), double, 0.7071, 0.4142 - 0.5 sits before the int-to-float double although its only
  users are the draw passes fn_8013D5D4 / D6E4 / DB2C after setEmitterPos_8013CB44. Tested and ruled out:
  a header inline's 0.5f (fn_8013D5D4 through JPADrawInfo's ortho constructor) still pools 0.5
  after 0.7071. Something before setEmitterPos_8013CB44 (a dead-stripped or inlined function?) must use 0.5;
  once it is found, setEmitterPos_8013CB44, spawnEmitter_8013CC50 and draw should all reach 100 %. (The draw passes
  convert fbWidth / efbHeight signed: xoris, i.e. through int.)
- fn_8013D5D4 / D6E4 / DB2C unwritten: each default-constructs a JPADrawInfo (identity x2),
  calls fn_8013B490, default-constructs a SECOND JPADrawInfo that is never used (sp8/sp38), then
  PSMTXCopy(field_0x10->0x80 = J2DOrthoGraph posMtx, first.mCamMtx) and
  C_MTXLightOrtho(first.mPrjMtx, 0, efbHeight, 0, fbWidth, 0.5 x4). The unused second object
  points at an inline (a JPADrawInfo or GameRoot2 helper) not identified yet; writing a dead local
  would be a trick, so they stay unwritten.
- drawAfter: still 97.7 % regalloc (now through JPADrawInfo::getCamMtx).

SHARED-EDITS: include/game/Effect.h (spawnEmitter_8013CC50 out of extern "C"), include/game/Camera.h
(field_0xe4 JPADrawInfo, includes JSystem/JParticle/JPADrawInfo.h), include/game/ActorId.h
(ACTOR_DMGE), libs/sdk/include/JSystem/JParticle/JPAEmitter.h (setGlobalSRTMatrix pikmin2/tp
form, setGlobalParticleScale(f32, f32) tp form; JPADrawInfo moved out), new
libs/sdk/include/JSystem/JParticle/JPADrawInfo.h. symbols.txt: spawnEmitter_8013CC50 mangled.
