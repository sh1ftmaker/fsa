#include "JSystem/JParticle/JPAEmitter.h"
#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_hyrule_castle_barrier.h"

HyruleCastleBarrier_8049DEE8::HyruleCastleBarrier_8049DEE8()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(0), field_0x240(false),
      field_0x241(false), field_0x250(NULL), field_0x254(NULL) {
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
}

HyruleCastleBarrier_8049DEE8::~HyruleCastleBarrier_8049DEE8() {
    for (s32 i = 0; i < 3; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
        mEmitters[i] = NULL;
    }
    releaseEmitter_801EE434(field_0x250);
    field_0x250 = NULL;
    releaseEmitter_801EE434(field_0x254);
    field_0x254 = NULL;
}

void HyruleCastleBarrier_8049DEE8::init() {
    ActorBase_804A5770::init();
    if ((field_0x90 & 1) == 1) {
        field_0x240 = true;
    }
    if ((field_0x90 >> 1 & 1) == 1) {
        field_0x241 = true;
    }
    field_0x234 = field_0x90 >> 4 & 0xF;
    field_0x238 = field_0x90 >> 22 & 0x1F;
    switch (field_0x234) {
    case 1:
        mBox.i.x = -16.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 40.0f;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        field_0x80.i.x = -14.0f;
        field_0x80.i.y = -40.0f;
        field_0x80.f.x = 14.0f;
        field_0x80.f.y = 40.0f;
        break;
    // Case 0 is written out with the default: the original's dispatch sends
    // both sides of the test against 1 to this block.
    case 0:
    default:
        mBox.i.x = -40.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 16.0f;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        field_0x80.i.x = -40.0f;
        field_0x80.i.y = -14.0f;
        field_0x80.f.x = 40.0f;
        field_0x80.f.y = 14.0f;
        break;
    }
    field_0x114 = 2;
    field_0x118 = 2;
    field_0xb8 = 1;
    field_0xbc = 3;
    field_0xb0 = 0x0A040304;
    fn_8011F314();
    if (vfunc_0x34()) {
        fn_8011F5C0(1);
    }
}

void HyruleCastleBarrier_8049DEE8::update() {
    ActorBase_804A5770::update();
    JGeometry::TBox2f box(field_0x80);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    switch (field_0x230) {
    case 0: {
        startSound_801F0E34(0x50010, 0);
        if (vfunc_0x34()) {
            if (field_0x240) {
                fn_8011F5C0(1);
            } else {
                fn_8011F5C0(3);
            }
            break;
        }
        if (fn_801F2DB4(ACTOR_NULL, ACTOR_NULL, NULL)) {
            field_0xd8 = 0;
        }
        u8 hitMask = fn_801F2B7C(field_0x114, -1, 10);
        if (hitMask == 0) {
            fn_80239914(&box, field_0x4, mSerial);
        } else {
            for (s32 i = 0; i < 4; i++) {
                if (hitMask & (1 << i)) {
                    fn_80230FD0(i, 0x20042);
                }
            }
        }
        break;
    }
    case 1:
        if (!field_0x241) {
            fn_8011F5C0(2);
        } else {
            field_0x23c++;
            if (field_0x23c >= 55) {
                field_0x23c = 0;
                fn_8011F5C0(2);
            }
        }
        break;
    case 2:
        if (!vfunc_0x34()) {
            fn_8011F5C0(0);
        }
        break;
    case 3:
        if (field_0x241) {
            field_0x23c++;
            if (field_0x23c == 55) {
                startSound_801F0E34(0x50011, 0);
                fn_8011F5C0(4);
            }
        } else if (mEmitters[0]->isEnableDeleteEmitter()) {
            fn_8011F5C0(4);
        }
        break;
    case 4:
        break;
    }
}

void HyruleCastleBarrier_8049DEE8::fn_8011F5C0(s32 arg) {
    field_0x230 = arg;
    switch (arg) {
    case 0: {
        field_0xb0 = 0x0A040304;
        if (field_0x240) {
            for (s32 i = 0; i < 3; i++) {
                mEmitters[i]->clearStatus(JPAEMIT_StopDraw);
            }
            field_0x250->clearStatus(JPAEMIT_StopDraw);
            field_0x254->clearStatus(JPAEMIT_StopDraw);
            s32 bit = field_0x238;
            if (bit > 0) {
                getGameView_802D6A9C()->fn_802D8050(bit, false, 0xFF);
            }
        }
        break;
    }
    case 1:
        fn_8011EEEC();
        for (s32 i = 0; i < 3; i++) {
            mEmitters[i]->setStatus(JPAEMIT_StopDraw);
        }
        field_0x250->setStatus(JPAEMIT_StopDraw);
        field_0x254->setStatus(JPAEMIT_StopDraw);
        field_0x23c = 0;
        break;
    case 2: {
        startSound_801F0E34(0x50011, 0);
        field_0xb0 = 0;
        s32 bit = field_0x238;
        if (bit > 0) {
            getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
        }
        break;
    }
    case 3:
        if (!field_0x241) {
            startSound_801F0E34(0x50011, 0);
        }
        field_0x23c = 0;
        fn_8011EEEC();
        for (s32 i = 0; i < 3; i++) {
            if (mEmitters[i] != NULL) {
                mEmitters[i]->becomeInvalidEmitter();
            }
        }
        if (field_0x250 != NULL) {
            field_0x250->becomeInvalidEmitter();
        }
        if (field_0x254 != NULL) {
            field_0x254->becomeInvalidEmitter();
        }
        break;
    case 4: {
        s32 bit = field_0x238;
        if (bit > 0) {
            getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
        }
        if (!field_0x241) {
            fn_801F0D20(3);
        }
        fn_801F3668();
        break;
    }
    }
}

void HyruleCastleBarrier_8049DEE8::fn_8011F314() {
    s32 i;
    for (i = 0; i < 3; i++) {
        mEmitters[i] = spawnEmitter_8013CC50(field_0x4, &mPos, 0xC8, 0, 0, NULL, NULL, true, 1.0f);
    }
    field_0x250 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xC9, 0, 0, NULL, NULL, true, 1.0f);
    field_0x254 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xC9, 0, 0, NULL, NULL, true, 1.0f);
    JGeometry::TVec3f pos1(mPos);
    JGeometry::TVec3f pos2(mPos);
    switch (field_0x234) {
    case 1:
        for (i = 0; i < 3; i++) {
            pos1.x = mPos.x + 16.0f * (i - 1);
            mEmitters[i]->setGlobalTranslation(pos1);
            fn_8013CAA4(mEmitters[i], 1.5707964f);
        }
        pos2.y -= 32.0f;
        field_0x250->setGlobalTranslation(pos2);
        fn_8013CAA4(field_0x250, 1.5707964f);
        pos2.y += 64.0f;
        field_0x254->setGlobalTranslation(pos2);
        fn_8013CAA4(field_0x254, 1.5707964f);
        break;
    case 0:
    default:
        for (i = 0; i < 3; i++) {
            pos1.y = mPos.y + 16.0f * (i - 1);
            mEmitters[i]->setGlobalTranslation(pos1);
        }
        pos2.x -= 32.0f;
        field_0x250->setGlobalTranslation(pos2);
        pos2.x += 64.0f;
        field_0x254->setGlobalTranslation(pos2);
        break;
    }
}

void HyruleCastleBarrier_8049DEE8::fn_8011EEEC() {
    switch (field_0x234) {
    case 1: {
        JPABaseEmitter* e1 = spawnEmitter_8013CC50(field_0x4, getEndPos1(), 0xCA, 0, 0, NULL, NULL, true, 1.0f);
        JPABaseEmitter* e2 = spawnEmitter_8013CC50(field_0x4, getEndPos2(), 0xCA, 0, 0, NULL, NULL, true, 1.0f);
        if (e1 != NULL) {
            fn_8013CAA4(e1, 1.5707964f);
        }
        if (e2 != NULL) {
            fn_8013CAA4(e2, 1.5707964f);
        }
        break;
    }
    default:
        if (field_0x241) {
            for (s32 i = 0; i < 3; i++) {
                spawnEmitter_8013CC50(field_0x4, getMidPos(i), 0x125, 0, 0, NULL, NULL, true, 1.0f);
            }
            spawnEmitter_8013CC50(field_0x4, getEndPos1(), 0x126, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, getEndPos2(), 0x126, 0, 0, NULL, NULL, true, 1.0f);
        } else {
            spawnEmitter_8013CC50(field_0x4, getEndPos1(), 0xCA, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, getEndPos2(), 0xCA, 0, 0, NULL, NULL, true, 1.0f);
        }
        break;
    }
}

// The particle positions fn_8011EEEC inlines: plain functions at the bottom of the
// file (their out-of-line copies were dead-stripped). Their literals lead the
// unit's .sdata2 (32.0f, then 16.0f / 0.0f / the int-to-float double) ahead of
// fn_8011EEEC's own 1.0f and pi / 2 (docs/RULES_CARD.md, "Literals before their
// first visible use" (3)).
const JGeometry::TVec3f* HyruleCastleBarrier_8049DEE8::getMidPos(s32 i) const {
    return &JGeometry::TVec3f(mPos.x, mPos.y + 16.0f * (i - 1), 0.0f);
}

const JGeometry::TVec3f* HyruleCastleBarrier_8049DEE8::getEndPos2() const {
    const JGeometry::TVec3f* p;
    switch (field_0x234) {
    case 1:
        p = &JGeometry::TVec3f(mPos.x, mPos.y - 32.0f, mPos.z);
        break;
    default:
        p = &JGeometry::TVec3f(mPos.x + 32.0f, mPos.y, mPos.z);
        break;
    }
    return p;
}

const JGeometry::TVec3f* HyruleCastleBarrier_8049DEE8::getEndPos1() const {
    const JGeometry::TVec3f* p;
    switch (field_0x234) {
    case 1:
        p = &JGeometry::TVec3f(mPos.x, mPos.y + 32.0f, mPos.z);
        break;
    default:
        p = &JGeometry::TVec3f(mPos.x - 32.0f, mPos.y, mPos.z);
        break;
    }
    return p;
}
