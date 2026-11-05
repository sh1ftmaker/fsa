#ifndef GAME_EFFECT_H
#define GAME_EFFECT_H

// The particle-effect system of game/tu_8013C5C8.cpp: one instance,
// allocated with __nw__(0x411C, heap, 0) and built by its constructor
// fn_8013F3E4 in game/tu_8013AAD4.cpp (0x8013C29C), stored at
// lbl_8053AB10->0x28; reach it with getEffectSystem_8013C824(). The constructor creates
// a JKRSolidHeap, loads "particle.jpc" (its OSReport "***** particle.jpc
// load *****", lbl_80465A28) into a JPAResourceManager and an
// JPAEmitterManager. 229 game units use the unit, 224 of them through the
// emitter spawner spawnEmitter_8013CC50.
//
// "EffectSystem_8049F178" is a provisional name carrying its vtable address
// (docs/PLAN.md, "Placeholder names"; it was EffectSystem_8013C824, after
// the getter, until the vtable was found). Naming and signature conventions
// follow ActorBase.h: members for functions called with the system in r3
// (mangled names in symbols.txt), extern "C" for free functions.
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "types.h"
#include "JSystem/JGeometry.h"

class JKRHeap;
class JKRSolidHeap;
class TileMap_804AA858;
class JPABaseEmitter;
class JPAEmitterManager;
class JPAResourceManager;
// FSA's JParticle callback bases (__vt__18JPAEmitterCallBack at 0x80498978;
// slots dtor, execute, executeAfter, draw, drawAfter), not declared in
// include/JSystem/JParticle yet.
class JPAEmitterCallBack;
class JPAParticleCallBack;
namespace JUtility {
struct TColor;
}

class EffectSystem_8049F178 {
public:
    // A position queued for an effect id: spawnEmitter_8013CC50 fills the first entry
    // whose count is 0 (id, count 1, position); the executeAfter callback of
    // vtable 0x8049F15C (fn_8013C958) emits the particles there and counts
    // it down. Its empty out-of-line constructor is fn_8013F584 (the
    // constructor's __construct_array, 0x400 entries of 0x10 bytes).
    struct Entry {
        /* 0x0 */ u16 mId;
        /* 0x2 */ u16 mCount;
        /* 0x4 */ JGeometry::TVec3f mPos;
    }; // Size: 0x10

    // fn_8013F3E4 (1 unit: tu_8013AAD4).
    EffectSystem_8049F178(JKRHeap* heap);
    // Virtual table at 0x8049F178: only the destructor (fn_8013F378:
    // clearResourceManager, then destroys mHeap).
    virtual ~EffectSystem_8049F178();

    // 2 units (act_all_enemies_defeated, tu_801EE24C). For player 0-3 and a
    // non-NULL emitter, copies the colour mColors[player] to the emitter's
    // bytes 0xB8-0xBA.
    void fn_8013C830(s32 player, JPABaseEmitter* emitter);
    // 1 unit (tu_802D39B8, 6 calls). deleteAllParticle() on every live
    // emitter whose id is 0x349-0x34B.
    void fn_8013C8CC();
    // The executeAfter callback of vtable 0x8049F15C (fn_8013F588 passes the
    // emitter): see Entry.
    void fn_8013C958(JPABaseEmitter* emitter);
    // The rest are called once each by GameRoot2 (game/tu_8013AAD4.cpp,
    // lbl_8053AB10->0x28 in r3); none reads another argument.
    // mEmitterManager->forceDeleteAllEmitter().
    void fn_8013D5B0();
    // Draw passes: JPAEmitterManager::draw with group 2 (unless paused,
    // fn_80134CC8), 8 and 10 in an orthographic light matrix.
    void fn_8013D5D4();
    // Draw groups 0, 3, 5 and 1 (with a JUTTexture::capture between).
    void fn_8013D6E4();
    // Draw group 7.
    void fn_8013D908();
    void fn_8013DA10();
    void fn_8013DB2C();
    // mEmitterManager->calc(10).
    void fn_8013DC1C();
    void fn_8013DC44();
    // 0x158C bytes; builds the callbacks of vtable 0x8049F15C.
    void fn_8013DDEC();

    /* 0x0000 vtable */
    /* 0x0004 */ JKRSolidHeap* mHeap; // JKRSolidHeap::create(-1, heap)
    /* 0x0008 */ JPAResourceManager* mResourceManager;
    // new JPAEmitterManager(0x1C00, 0x200, heap, 0xB, 8); 48 units use it
    // (e.g. forceDeleteEmitter in ActorBase_804A5770's destructor).
    /* 0x000C */ JPAEmitterManager* mEmitterManager;
    /* 0x0010 */ Entry mEntries[0x400];
    // The emitters the callbacks track, with their effect ids and a word
    // each (fn_8013C8CC, fn_8013C958); mEmitterNum is the count in use.
    /* 0x4010 */ JPABaseEmitter* mEmitters[25];
    /* 0x4074 */ u16 mEmitterIds[25];
    /* 0x40A8 */ s32 field_0x40a8[25];
    /* 0x410C */ s32 mEmitterNum;
    // One RGB triple per player, set by the constructor: (C8 FA AC),
    // (FA AE AC), (B4 D2 E6), (F0 B4 F0).
    /* 0x4110 */ u8 mColors[4][3];
}; // Size: 0x411C

// Effect callbacks other units embed. Vtable 0x8049F140 derives from
// JPAEmitterCallBack (0x10 bytes), cls_8049F140 in game/tu_8013C5C8.cpp:
// destructor and execute (every 8 frames, while the stage allows it, spawns a 'PEFF'
// actor at player field_0x4's position; field_0x8 0 or 1 picks the spawn
// parameter, field_0xc counts the frames). tu_8023FBF0 holds two at 0x1398
// and 0x13A8 and inlines their constructor (both vtables stored, field_0xc
// zeroed). Declare it once JPAEmitterCallBack is in the JParticle headers.

// 48 units / 226 calls. lbl_8053AB10->0x28.
EffectSystem_8049F178* getEffectSystem_8013C824(void);

// 7 calls in 2 units (act_hyrule_castle_barrier, ...). For a non-NULL
// emitter, builds a rotation of `angle` plus a constant (fn_8010F668), scales
// it and sets it as the emitter's matrix at 0x68 (JPASetRMtxfromMtx);
// returns 0 or 1.
u8 fn_8013CAA4(JPABaseEmitter* emitter, f32 angle);

// 66 units / 197 calls. Moves a non-NULL emitter to pos, lifted by
// height * 1024 + depth * 128 in y and z (with `adjust`, a positive pos.z
// raises height below 1 by one and lowers depth by one) and with z taken
// from the ground under pos (TileMap_804AA858::fn_802F5E9C, game/Map.h, on lbl_8053AB10->0x20); stores
// the result in the emitter's translation (0xA4-0xAC). Returns 0 or 1.
u8 setEmitterPos_8013CB44(JPABaseEmitter* emitter, const JGeometry::TVec3f* pos, s32 height, s32 depth,
                 bool adjust);

// game/tu_801CB270.cpp (4 caller units: tu_801EE24C,
// tu_8021AF28, tu_801E5FC8, tu_8020F798). Moves the emitter in *emitter to pos with
// setEmitterPos_8013CB44 (adjust true); or, when it is flagged (bit 0x8 of its word
// 0xF4) and has no particles left (0xD0 + 0xDC == 0), deletes it with
// JPAEmitterManager::forceDeleteEmitter and sets *emitter to NULL. Does
// nothing for a NULL emitter.
void fn_801CB71C(JPABaseEmitter** emitter, const JGeometry::TVec3f* pos, s32 height, s32 depth);

// 224 units / 1866 calls. A free function: r3 is the layer, not the system.
// Spawns effect `id` at pos and returns its emitter (NULL when scale <= 0
// or nothing was made). On layer 0 it places pos as
// setEmitterPos_8013CB44 does, picks the draw group from the id (a switch; 0xFF skips
// the emitter) and on some stages a substitute id (getGameView_802D6A9C()'s
// field_0x131f1/field_0x131f2, fn_802DEABC, fn_802DE9A0), queues the
// position in a free mEntries slot (0x8013D20C), creates the emitter
// (JPAEmitterManager::createSimpleEmitterID(pos, id, group, 0, emitterCB,
// particleCB); emitterCB becomes the unit's static callback lbl_8053AB18
// when the id's JPAResourceManager user work has bit 0) and scales it by
// `scale`. On any other layer it spawns helper actors instead
// ('KMRI', 'DMGE', 'PEFF' via fn_801F9484) for a few ids. Callers: r3 the
// actor's field_0x4, r5 li or a clrlwi 16 id, r6/r7 li or extsb, r8/r9
// almost always 0, r10 li 1, f1 a constant (ActorBase_804A5770::vfunc_0x28
// passes its int as height, depth 0, no callbacks and adjust 1).
JPABaseEmitter* spawnEmitter_8013CC50(s32 layer, const JGeometry::TVec3f* pos, u16 id, s32 height,
                            s32 depth, JPAEmitterCallBack* emitterCB,
                            JPAParticleCallBack* particleCB, bool adjust, f32 scale);

// game/tu_8013AAD4.cpp. Draws a lit sphere of `scale` at `pos` in `color`; act_beamos
// draw() passes a null matrix. a / b are summed into a depth offset.
void fn_8013AC90(f32 scale, const Vec* pos, const JUtility::TColor* color, s32 a, s32 b, const Mtx m);

// game/tu_801CB270.cpp (3 caller units: tu_801EE24C, tu_8021AF28,
// tu_8020F798). NULL-checks the emitter, then makes it immortal and invalid
// (status bits 0x40 and 0x1 of word 0xF4, word 0x24 = 1).
void fn_801CB7A0(JPABaseEmitter* emitter);

#endif
