// cls_80217F80, the emitter set at 0x448 of Vaati (see its header). The unit's
// __sinit (the 9 JAudio pools) comes from the GameAudio.h include.

#include "game/GameAudio.h"
#include <float.h>
#include "game/GameGlobals.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/act/act_vaati_boss.h"

// The emitter scale: 1.7f on each of the three axes. Written to each emitter's
// bytes 0x98..0xB4 by the spawners below (not written yet: those bytes are
// not named in JPAEmitter.h).
static f32 lbl_804CD838[66] = {lbl_8053F0BC, lbl_8053F0BC, lbl_8053F0BC};

// Effect ids for the six emitters of fn_80217ED4.
static const u16 lbl_8046A5A0[8] = {0x022A, 0x022B, 0x022C, 0x022D, 0x0549, 0x022E, 0x0000, 0x0000};

cls_80217F80::cls_80217F80() {
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;
    mEmitters[4] = NULL;
    mEmitters[5] = NULL;
    mOwner = NULL;
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

void cls_80217F80::fn_8021798C() {
    fn_8013CB44(mEmitters[0], &mOwner->mPos, 1, 1, true);
    fn_8013CB44(mEmitters[1], &mOwner->mPos, 1, 1, true);
}

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
    fn_8013CB44(mEmitters[4], &pos, 1, 1, true);
    fn_8013CB44(mEmitters[5], &pos, 1, 1, true);
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
    fn_8013CB44(mEmitters[2], &pos, 1, 1, true);
    fn_8013CB44(mEmitters[3], &pos, 1, 1, true);
}

// FIXME: 58.44 %. The spawners also store lbl_804CD838 (1.7f on each axis) into
// the emitter's bytes 0x98..0xB4, which JPAEmitter.h doesn't name for JPABaseEmitter
// (mEmitterDirMtx at 0x98 belongs to JPAEmitterInfo), so those stores are missing.
void cls_80217F80::fn_80217AB8() {
    if (mEmitters[4] == NULL) {
        mEmitters[4] = fn_8013CC50(mOwner->field_0x5fc, &mOwner->field_0x60c, 0x224, 1, 1, NULL,
                                   NULL, true, 1.0f);
    }
    if (mEmitters[5] == NULL) {
        mEmitters[5] = fn_8013CC50(mOwner->field_0x5fc, &mOwner->field_0x60c, 0x225, 1, 1, NULL,
                                   NULL, true, 1.0f);
    }
}

// FIXME: 0.00 %. Same as fn_80217AB8: the emitter scale stores are missing.
void cls_80217F80::fn_80217BC8() {
    if (mEmitters[2] == NULL) {
        mEmitters[2] = fn_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x22F, 1, 1, NULL, NULL,
                                   true, 1.0f);
    }
    if (mEmitters[3] == NULL) {
        mEmitters[3] = fn_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x230, 1, 1, NULL, NULL,
                                   true, 1.0f);
    }
}

// FIXME: 0.00 %. Same as fn_80217AB8: the emitter scale stores are missing.
void cls_80217F80::fn_80217CD8() {
    if (mEmitters[0] == NULL) {
        mEmitters[0] = fn_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x228, 1, 1, NULL, NULL,
                                   true, 1.0f);
    }
    if (mEmitters[1] == NULL) {
        mEmitters[1] = fn_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x229, 1, 1, NULL, NULL,
                                   true, 1.0f);
    }
}

// FIXME: 0.00 %. Same as fn_80217AB8: the emitter scale stores are missing.
void cls_80217F80::fn_80217DE8() {
    fn_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x231, 1, 1, NULL, NULL, true, 1.0f);
    fn_8013CC50(mOwner->field_0x4, &mOwner->mPos, 0x232, 1, 1, NULL, NULL, true, 1.0f);
}

// FIXME: 0.00 %. Same as fn_80217AB8: the emitter scale stores are missing.
void cls_80217F80::fn_80217ED4() {
    for (s32 i = 0; i < 6; i++) {
        fn_8013CC50(mOwner->field_0x4, &mOwner->mPos, lbl_8046A5A0[i], 1, 1, NULL, NULL, true,
                    1.0f);
    }
}
