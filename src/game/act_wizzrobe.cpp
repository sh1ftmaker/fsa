#include "JSystem/JParticle/JPAEmitter.h"
#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/act/act_wizzrobe.h"
#include "JSystem/JMath/JMath.h"

extern const ActorId lbl_80466838[7];
const ActorId lbl_80466838[7] = { ACTOR_WIZR, ACTOR_FIRE, ACTOR_BLZE, ACTOR_DDGO, ACTOR_FLME, ACTOR_THCL, ACTOR_NULL };

s32 lbl_804A0D50[20] = { 0, 1, 2, 3, 2, 4, 5, 6, 7, 6, 8, 9, 10, 11, 10, 12, 13, 14, 15, 14 };
s32 lbl_804A0DA0[23] = { 2, 1, 20, 20, 10, 16, 17, 18, 19, 16, 17, 18, 19, 20, 2, 3, 3, 4, 4, 5, 6, 7, 10 };
s32 lbl_804A0DFC[20] = { 20, 2, 1, 0, 20, 6, 5, 4, 20, 10, 9, 8, 20, 14, 13, 12, 2, 1, 1, 10 };
s32 lbl_804A0E4C[16] = { 0, 1, 2, 20, 4, 5, 6, 20, 8, 9, 10, 20, 12, 13, 14, 20 };
s32 lbl_804A0E8C[4] = { 10, 1, 1, 2 };
s32 lbl_804A0E9C[4] = { 16, 17, 18, 19 };
s32 lbl_804A0EAC[4] = { 0x4000, 0xC000, 0x8000, 0 };
s32 lbl_804A0EBC[8] = { 1, 5, 20, 50, 100, 150, 200, 300 };

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_80178F10(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_80178F00(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&) on the emitter's mLocalDir (offset 0x18).
void fn_80178EE4(JPABaseEmitter* emitter, const JGeometry::TVec3f* trans) {
    emitter->mLocalDir.set(*trans);
}

// Out-of-line copy of TVec3<s16>::set(x, y, z).
void fn_80178ED4(JGeometry::TVec3<s16>* v, s16 x, s16 y, s16 z) {
    v->set(x, y, z);
}

// Out-of-line copy of JPABaseEmitter::setGlobalTranslation(const TVec3&).
void fn_80178EB8(JPABaseEmitter* emitter, const JGeometry::TVec3f* trans) {
    emitter->setGlobalTranslation(*trans);
}

// Shared by the destructor and vfunc_0x44.
inline void Wizzrobe_804A0EDC::killLinkedActor() {
    if (field_0x94 == 0 || (field_0x94 == 4 || field_0x94 == 5) || (field_0x94 == 1 || field_0x94 == 2) || field_0x94 == 3) {
        s32 serial = field_0x268;
        if (serial >= 0) {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (target != NULL) {
                field_0x268 = -1;
                target->mLife = 0;
            }
        }
    }
}

Wizzrobe_804A0EDC::~Wizzrobe_804A0EDC() {
    releaseEmitter_801EE434(mEmitters[0]);
    releaseEmitter_801EE434(mEmitters[1]);
    releaseEmitter_801EE434(mEmitters[2]);
    killLinkedActor();
}

void Wizzrobe_804A0EDC::vfunc_0x44() {
    killLinkedActor();
    field_0x238 = 0;
    field_0x2e4 = 0;
    field_0x244 = 0;
    field_0x230 = 8;
}

void Wizzrobe_804A0EDC::init() {
    field_0x2c8 = (field_0x90 >> 22) & 0x1F;
    field_0x94 = field_0x90 & 0xFF;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    for (int i = 0; i < 20; i++) {
        field_0x270[i] = -1;
    }
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    field_0x260 = 0;
    field_0x2e4 = 0;
    field_0x2d0 = 0.0f;
    field_0x2d4 = 0.0f;
    field_0x2d8 = 5.0f;
    field_0x268 = -1;
    field_0x26c = -1;
    field_0x2e0 = 0.0f;
    field_0x2cc = 0;
    field_0x264 = 0;
    field_0x234 = 0;
    field_0x2c0 = 0;
    field_0x2c4 = 200;
    field_0x2dc = 1.0f;
    field_0x2e8.z = 0.0f;
    field_0x2e8.y = 0.0f;
    field_0x2e8.x = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x2f4.set(mPos);
    field_0x2e5 = 0;
    switch (field_0x94) {
    case 0:
    default:
        field_0x250 = 3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        field_0x250 &= 3;
        if (field_0x90 >> 27) {
            if (!vfunc_0x34()) {
                field_0x1a8 = 0;
                field_0x2e5 = 1;
            }
        }
        field_0x230 = 0;
        break;
    case 10:
    case 11:
        field_0x1a8 = 0;
        mBox.set(-6.0f, -6.0f, 6.0f, 6.0f);
        field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
        mLife = 10000;
        field_0x238 = 10;
        field_0x23c = 10;
        field_0x240 = 200;
        mPos.z = 8.0f;
        field_0x230 = 12;
        break;
    case 20:
        field_0x2dc = 1.2f;
        // fallthrough
    case 21:
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x506, 0, 1, NULL, NULL, true, field_0x2dc);
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x507, 0, 1, NULL, NULL, true, field_0x2dc);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x508, 0, 1, NULL, NULL, true, 1.0f);
        field_0x1a8 = 0;
        field_0x238 = 10;
        mLife = 10000;
        field_0x230 = 13;
        break;
    case 22:
    case 31:
        field_0x2dc = 1.2f;
        // fallthrough
    case 23:
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x506, 0, 1, NULL, NULL, true, field_0x2dc);
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x507, 0, 1, NULL, NULL, true, field_0x2dc);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x508, 0, 1, NULL, NULL, true, 1.0f);
        field_0x1a8 = 0;
        field_0x238 = 30;
        mLife = 10000;
        field_0x230 = 15;
        break;
    case 30:
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x50d, 1, 1, NULL, NULL, true, field_0x2dc);
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x50e, 1, 1, NULL, NULL, true, field_0x2dc);
        field_0x1a8 = 0;
        field_0x23c = 40;
        mLife = 10000;
        field_0x230 = 16;
        break;
    }
}

void Wizzrobe_804A0EDC::draw() {
    if (field_0x94 == 20 || (field_0x94 == 21 || field_0x94 == 22 || field_0x94 == 23) || (field_0x94 == 30 || field_0x94 == 31)) {
        return;
    }
    if (field_0x230 == 1) {
        return;
    }
    if (field_0x230 == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    if (field_0x94 == 10 || field_0x94 == 11) {
        return;
    }
    params.field_0x10.a = (u8)(s32)field_0x2d0;
    if (field_0x234 == 1) {
        params.field_0x30 = 0x11;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x260 + 233, &params, 0, 1.0f, 1.0f);
}
