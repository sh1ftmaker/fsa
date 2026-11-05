# game/tu_8013C5C8 (EffectSystem_8049F178 + three JPAEmitterCallBack classes)

Baseline: 8015 / 11557. After: 8030 (15/27 in the unit, +15 in the honest report).

Classes: EffectSystem_8049F178 (vtable 0x8049F178; ctor 0x8013F3E4), cls_8049F140 (execute spawns 'PEFF'
actors), cls_8049F15C (executeAfter = fn_8013C958 queue), cls_8049F184 (draw/drawAfter swap the particle
camera matrix; global lbl_8053AB18 is one, its sinit is fn_8013F758).

Matched: fn_8013C824, fn_8013C830, fn_8013C8CC, fn_8013C958, fn_8013CAA4, fn_8013CB44 (reloc-name only),
fn_8013D5B0, fn_8013DC1C, fn_8013DC44, __dt__ EffectSystem, __dt__ x3 callbacks, executeAfter, ctor (same-text).

Resistant: execute (98.7 %, `bne; b end` for the mode 0xC test + r28/r29 register split), drawAfter (97.7 %,
regalloc), sinit (99.6 %, bss registration offsets need the 0xC object of the unwritten big functions).

Unwritten: fn_8013CC50 (0x960), fn_8013D5D4/D6E4/D908/DA10/DB2C (draw passes: need JPADrawInfo-typed
Camera_804AA9E4::field_0xe4 and the identity-initialised JPADrawInfo pair), fn_8013F5B0 draw (psq matrix copy),
fn_8013DDEC (0x158C).

SHARED-EDITS: include/game/Effect.h (mHeap is JKRSolidHeap*, fn_8013C958 declared, fn_8013C824 / CAA4 / CB44
moved out of extern "C" and CAA4 / CB44 return u8, comments), include/game/RotMatrix.h (fn_8010F668 declaration,
owner act_princess_zelda_demo). Symbols renamed in symbols.txt (callbacks, vtables, Entry ctor, sinit, the
three free functions).
