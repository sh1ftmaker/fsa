#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/act/act_ganon.h"
#include "JSystem/JParticle/JPAEmitter.h"

// .bss tables indexed by player (the unwritten update / init use them; the __sinit constructs
// the two vector tables).
s32 lbl_804C1BC0[4];
JGeometry::TVec3f lbl_804C1BD0[4];
JGeometry::TVec3f lbl_804C1C00[4];

void Ganon_804A25F0::vfunc_0x24() {
    startSound_801F0E34(0x40006, 0);
    vfunc_0x28(2, 0.0f, 0.0f);
}

Ganon_804A25F0::~Ganon_804A25F0() {
    for (s32 i = 0; i < 5; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// Out-of-line copy of JPABaseEmitter::setGlobalScale(const TVec3f&).
void fn_80196CF4(JPABaseEmitter* emitter, const JGeometry::TVec3f* scale) {
    emitter->setGlobalScale(*scale);
}

// Sets the emitter's particle scale (0xB0, 0xB4).
void fn_80196D18(JPABaseEmitter* emitter, f32 x, f32 y) {
    emitter->mGlobalPScl.set(x, y);
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_80196D24(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

void fn_80196D30(s32 a, s32 b) {
    gGameRoot_8053AAF8->fn_80134DE4(a, b);
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_80196D60(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_80196D70(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

void fn_80196D8C(s32 index, bool flag, f32 a, f32 b) {
    getGameView_802D6A9C()->fn_802D79E8(index, a, b, flag);
}

u8 fn_80196DF0() {
    return lbl_80529DEC.field_0x24f;
}

void fn_80196E00() {
    lbl_8053AED0->field_0xdc |= 0x10;
}
