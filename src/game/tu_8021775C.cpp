// cls_80217F80, the emitter set at 0x448 of Vaati (see its header). The unit's
// __sinit (the 9 JAudio pools) comes from the GameAudio.h include.

#include "game/GameAudio.h"
#include <float.h>
#include "game/GameGlobals.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_vaati_boss.h"

// The emitter scale the spawners below give their emitters.
static JGeometry::TVec3f lbl_804CD838(lbl_8053F0BC, lbl_8053F0BC, lbl_8053F0BC);

// Effect ids for the six emitters of fn_80217ED4.
static const u16 lbl_8046A5A0[8] = {0x022A, 0x022B, 0x022C, 0x022D, 0x0549, 0x022E, 0x0000, 0x0000};

// FIXME: 90.63 %. Same instructions as the original except the y and z blocks:
// the original loads from.y and from.z before the FLT_EPSILON compare (into the
// load gap after the epsilon load) and uses other registers; ours loads them in
// the branch.
void cls_80217F80::fn_8021775C(f32 t) {
    JGeometry::TVec3f pos = mOwner->field_0x60c;
    JGeometry::TVec3f from = mOwner->field_0x600;
    if (t < FLT_EPSILON) {
        pos.x = from.x;
    } else {
        pos.x = (1.0f / t) * (from.x - pos.x) + pos.x;
    }
    if (t < FLT_EPSILON) {
        pos.y = from.y;
    } else {
        pos.y = (1.0f / t) * (from.y - pos.y) + pos.y;
    }
    if (t < FLT_EPSILON) {
        pos.z = from.z;
    } else {
        pos.z = (1.0f / t) * (from.z - pos.z) + pos.z;
    }
    setEmitterPos_8013CB44(mEmitters[4], &pos, 1, 1, true);
    setEmitterPos_8013CB44(mEmitters[5], &pos, 1, 1, true);
}

// FIXME: 90.63 %. Same as fn_8021775C: the original hoists the from.y and from.z
// loads above the epsilon compare.
void cls_80217F80::fn_80217874(f32 t) {
    JGeometry::TVec3f pos = mOwner->mPos;
    JGeometry::TVec3f from = mOwner->field_0x60c;
    if (t < FLT_EPSILON) {
        pos.x = from.x;
    } else {
        pos.x = (1.0f / t) * (from.x - pos.x) + pos.x;
    }
    if (t < FLT_EPSILON) {
        pos.y = from.y;
    } else {
        pos.y = (1.0f / t) * (from.y - pos.y) + pos.y;
    }
    if (t < FLT_EPSILON) {
        pos.z = from.z;
    } else {
        pos.z = (1.0f / t) * (from.z - pos.z) + pos.z;
    }
    setEmitterPos_8013CB44(mEmitters[2], &pos, 1, 1, true);
    setEmitterPos_8013CB44(mEmitters[3], &pos, 1, 1, true);
}

void cls_80217F80::fn_8021798C() {
    setEmitterPos_8013CB44(mEmitters[0], &mOwner->mPos, 1, 1, true);
    setEmitterPos_8013CB44(mEmitters[1], &mOwner->mPos, 1, 1, true);
}

void cls_80217F80::fn_802179EC() {
    fn_801EE434(mEmitters[4]);
    fn_801EE434(mEmitters[5]);
    mEmitters[4] = NULL;
    mEmitters[5] = NULL;
}

void cls_80217F80::fn_80217A30() {
    fn_801EE434(mEmitters[2]);
    fn_801EE434(mEmitters[3]);
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;
}

void cls_80217F80::fn_80217A74() {
    fn_801EE434(mEmitters[0]);
    fn_801EE434(mEmitters[1]);
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
}

void cls_80217F80::fn_80217AB8() {
    if (mEmitters[4] == NULL) {
        mEmitters[4] = spawnEmitter_8013CC50(mOwner->field_0x5fc, &mOwner->field_0x60c, 0x224, 1, 1, NULL, NULL, true, 1.0f);
        if (mEmitters[4] != NULL) {
            mEmitters[4]->setGlobalScale(lbl_804CD838);
        }
    }
    if (mEmitters[5] == NULL) {
        mEmitters[5] = spawnEmitter_8013CC50(mOwner->field_0x5fc, &mOwner->field_0x60c, 0x225, 1, 1, NULL, NULL, true, 1.0f);
        if (mEmitters[5] != NULL) {
            mEmitters[5]->setGlobalScale(lbl_804CD838);
        }
    }
}

// FIXME: 93.82 %. Register choice only: the original loads mOwner into r3 and
// takes &mPos before the layer; ours loads it into r4.
void cls_80217F80::fn_80217BC8() {
    if (mEmitters[2] == NULL) {
        mEmitters[2] = spawnEmitter_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x22F, 1, 1, NULL, NULL, true, 1.0f);
        if (mEmitters[2] != NULL) {
            mEmitters[2]->setGlobalScale(lbl_804CD838);
        }
    }
    if (mEmitters[3] == NULL) {
        mEmitters[3] = spawnEmitter_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x230, 1, 1, NULL, NULL, true, 1.0f);
        if (mEmitters[3] != NULL) {
            mEmitters[3]->setGlobalScale(lbl_804CD838);
        }
    }
}

// FIXME: 93.82 %. Register choice only: the original loads mOwner into r3 and
// takes &mPos before the layer; ours loads it into r4.
void cls_80217F80::fn_80217CD8() {
    if (mEmitters[0] == NULL) {
        mEmitters[0] = spawnEmitter_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x228, 1, 1, NULL, NULL, true, 1.0f);
        if (mEmitters[0] != NULL) {
            mEmitters[0]->setGlobalScale(lbl_804CD838);
        }
    }
    if (mEmitters[1] == NULL) {
        mEmitters[1] = spawnEmitter_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x229, 1, 1, NULL, NULL, true, 1.0f);
        if (mEmitters[1] != NULL) {
            mEmitters[1]->setGlobalScale(lbl_804CD838);
        }
    }
}

// FIXME: 92.88 %. Register choice: the original keeps the first emitter in r31
// and reloads mOwner into r6 for the second spawn; ours differs in registers.
void cls_80217F80::fn_80217DE8() {
    JPABaseEmitter* emitter =
        spawnEmitter_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x231, 1, 1, NULL, NULL, true, 1.0f);
    JPABaseEmitter* emitter2 =
        spawnEmitter_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x232, 1, 1, NULL, NULL, true, 1.0f);
    if (emitter != NULL) {
        emitter->setGlobalScale(lbl_804CD838);
    }
    if (emitter2 != NULL) {
        emitter2->setGlobalScale(lbl_804CD838);
    }
}

// FIXME: 98.95 %. Register allocation only.
void cls_80217F80::fn_80217ED4() {
    VaatiBoss_804A4958* owner = mOwner;
    const JGeometry::TVec3f* pos = &owner->mPos;
    for (s32 i = 0; i < 6; i++) {
        JPABaseEmitter* emitter =
            spawnEmitter_8013CC50(owner->field_0x4, pos, lbl_8046A5A0[i], 1, 1, NULL, NULL, true, 1.0f);
        if (emitter != NULL) {
            emitter->setGlobalScale(lbl_804CD838);
        }
    }
}

cls_80217F80::~cls_80217F80() {
    fn_801EE434(mEmitters[0]);
    fn_801EE434(mEmitters[1]);
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    fn_801EE434(mEmitters[2]);
    fn_801EE434(mEmitters[3]);
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;
    fn_801EE434(mEmitters[4]);
    fn_801EE434(mEmitters[5]);
    mEmitters[4] = NULL;
    mEmitters[5] = NULL;
}

cls_80217F80::cls_80217F80() {
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;
    mEmitters[4] = NULL;
    mEmitters[5] = NULL;
    mOwner = NULL;
}
