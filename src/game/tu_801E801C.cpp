#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/tu_801E9AFC.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/tu_801E6D48.h"
#include "game/act/tu_801E801C.h"

f32 fn_801CD6D8(f32 p0, f32 p1, f32 p2, f32 p3, f32 t);




cls_804A5568::cls_804A5568() {
    mMask = 0;
    field_0x2ac = 0;
    mCount = 0;
    field_0x3b0 = 0;
    field_0x3b4 = 0.0f;
    field_0x3b8 = -1;
    field_0x3bc = 0;
    mEmitter3c0 = NULL;
}

cls_804A5568::~cls_804A5568() {
    releaseEmitter_801EE434(mEmitter3c0);
}

// FIXME(shotgun): 86.04 %, different + reloc-name
void cls_804A5568::init() {
    EnemyBase_804A4808::init();
    field_0x2ac = field_0x90;
    mBox.set(-16.0f, -16.0f, 16.0f, 16.0f);
    mBox2.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x280;
    mLife = 1000;
    mLifeBackup = mLife;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
}

// FIXME(shotgun): 98.01 %, missing/extra + reloc-name
void cls_804A5568::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0: {
        u16 trigger = field_0x94;
        if (trigger != 0) {
            if (getGameView_802D6A9C()->fn_802D800C(trigger)) {
                mFlags |= 0x2000;
                if (mEmitter3c0 != NULL) {
                    getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitter3c0);
                }
                mEmitter3c0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x260, 0, -2, NULL, NULL, true, 1.0f);
                mStateTimer = 0;
                mState = 1;
            }
        }
        break;
    }
    case 1:
        fn_801E9224();
        setEmitterPos_8013CB44(mEmitter3c0, &mPos, 0, -2, true);
        startSound_801F0E34(0x3003E, 0);
        if (mFlags & 0x4000) {
            s32 serial = field_0x3b8;
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    ((cls_804A5418*)actor)->fn_801E6EAC();
                }
            }
            mFlags &= ~0x2000;
            releaseEmitter_801EE434(mEmitter3c0);
            mEmitter3c0 = NULL;
            mStateTimer = 0;
            mState = 2;
        }
        break;
    }
}

u8 cls_804A5568::vfunc_0x5C() {
    return 0;
}

// FIXME(shotgun): 93.58 %, missing/extra + reloc-name
u8 cls_804A5568::vfunc_0x60() {
    if (mState == 0 && mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        if (fn_801E9B58() >= 0) {
            mFlags |= 0x2000;
            if (mEmitter3c0 != NULL) {
                getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitter3c0);
            }
            mEmitter3c0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x260, 0, -2, NULL, NULL, true, 1.0f);
            mStateTimer = 0;
            mState = 1;
        }
    }
    return 0;
}

// FIXME(shotgun): 36.78 %, missing/extra + reloc-name
u8 cls_804A5568::vfunc_0x68() {
    u8 result = 0;
    if (mFlags & 0x2000) {
        JGeometry::TBox2f saved = mBox;
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        result = fn_801F2B7C(0, -1, 0x16);
        mBox = saved;
    }
    return result;
}

void cls_804A5568::vfunc_0xA0() {
    if (mFlags & 0x2000) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}

void cls_804A5568::fn_801E8EB4(u8 index, s32 arg, const JGeometry::TVec2f* pos) {
    mMask |= 1 << index;
    field_0x2b0[index].x = pos->x;
    field_0x2b0[index].y = pos->y;
}

// FIXME(shotgun): 95.77 %, missing/extra + reloc-name
void cls_804A5568::fn_801E8C74(s32 arg) {
    field_0x3b8 = arg;
    mCount = 0;
    bool run = true;
    for (s32 i = 0; i < 8; i++) {
        if (mMask & (1 << i)) {
            if (run) {
                mCount++;
            }
        } else {
            run = false;
        }
    }
    mPos.x = field_0x2b0[0].x;
    mPos.y = field_0x2b0[0].y;
    mPos.z = 0.0f;
    f32 y2 = field_0x2b0[0].y;
    f32 x0 = field_0x2b0[0].x;
    f32 x1 = field_0x2b0[1].x;
    f32 y1 = field_0x2b0[1].y;
    f32 x2;
    f32 y3;
    if (mCount == 2) {
        x2 = x1;
        y3 = y1;
    } else {
        x2 = field_0x2b0[2].x;
        y3 = field_0x2b0[2].y;
    }
    f32 ry = fn_801CD6D8(y2, y2, y1, y3, 0.0625f);
    f32 rx = fn_801CD6D8(x0, x0, x1, x2, 0.0625f);
    field_0x3bc = (s32)(JMath::atanTable_.atan2_(mPos.x - rx, mPos.y - ry) * 10430.378f);
    JGeometry::TVec2f off;
    fn_801E9C00(field_0x3bc, &off);
    mBox.i.x += off.x;
    mBox.i.y += off.y;
    mBox.f.x += off.x;
    mBox.f.y += off.y;
}

// FIXME(shotgun): 99.20 %, reloc-name
void cls_804A5568::fn_801E801C() {
    static f32 sZ = -256.0f;
    if (mState == 0) {
        fn_801E9AFC()->fn_801EA108((const JGeometry::TVec2f*)&mPos, field_0x3bc, sZ);
    }
}

