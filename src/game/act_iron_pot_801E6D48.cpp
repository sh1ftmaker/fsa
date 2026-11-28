#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/tu_801F9484.h"
#include "game/act/act_iron_pot_801E6D48.h"
#include "game/tu_801E9AFC.h"

IronPot_804A5418::IronPot_804A5418() {
    field_0x2ac = -1;
    field_0x2b0 = 0;
    field_0x2b4 = 0;
}

void IronPot_804A5418::init() {
    EnemyBase_804A4808::init();
    mDir_0x98 = mSpawnParam_0x90 & 3;
    field_0x2b4 = (mSpawnParam_0x90 >> 2) & 0x1F;
    if ((mSpawnParam_0x90 >> 21) & 1) {
        mFlags |= 0x8000;
    } else if ((mSpawnParam_0x90 >> 23) & 1) {
        mFlags |= 0x2000;
    } else if ((mSpawnParam_0x90 >> 22) & 1) {
        mFlags |= 0x4000;
    }
    mMapBox_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    if (mFlags & 0x8000) {
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2.set(mBox);
        field_0xb0 = 0;
    } else {
        fn_80210990(mDir_0x98, &mBox);
        field_0xb0 = 0x40004;
    }
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x2b0 = dirToAngle_801F6874(mDir_0x98);
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
}

void IronPot_804A5418::vfunc_0x10() {
    u32 flags = mSpawnParam_0x90 >> 24;
    s32 serial = mSerial;
    cls_801E9AFC* slots = fn_801E9AFC();
    field_0x2ac = slots->field_0x0[slots->field_0xf0].fn_801EA6D8(flags, serial);
}

// Per group: the rupee kind and how many.
const s32 lbl_80469C40[4][2] = {{4, 3}, {3, 2}, {2, 8}, {0, 20}};

// FIXME(shotgun): 98.90 %, missing/extra; behaviour near.
void IronPot_804A5418::fn_801E728C(int state) {
    switch (state) {
    case 1:
        if ((mFlags & 0x4000) && !(mFlags & 0x10000)) {
            getActorManager_801F856C()->spawnActor_801F9484('IBBM', mLayer_0x4, &mPos, mSpawnParam_0x90, field_0x16c, -1);
            mAlive = false;
        } else {
            f32 savedIx = mBox.i.x;
            f32 savedIy = mBox.i.y;
            f32 savedFx = mBox.f.x;
            f32 savedFy = mBox.f.y;
            fn_80210A30(&mBox);
            mBox.i.x *= 1.5f;
            mBox.i.y *= 1.5f;
            mBox.f.x *= 1.5f;
            mBox.f.y *= 1.5f;
            damagePlayersInBox_801F2B7C(4, -1, 0x16);
            JGeometry::TVec2f spot;
            JGeometry::TVec2f center;
            getBoxCenter_801F3128(&center);
            spot.y = center.y - 16.0f;
            for (s32 j = 0; j < 3; j++) {
                spot.x = center.x - 16.0f;
                for (s32 i = 0; i < 3; i++) {
                    if (fn_801EE3C0(getTileAt_80226998(spot, mLayer_0x4))) {
                        JGeometry::TVec3f at(spot.x, spot.y, 0.0f);
                        getActorManager_801F856C()->spawnActor_801F9484('BLZE', mLayer_0x4, &at, 15, -1, -1);
                    }
                    spot.x += 16.0f;
                }
                spot.y += 16.0f;
            }
            mBox.i.x = savedIx;
            mBox.i.y = savedIy;
            mBox.f.x = savedFx;
            mBox.f.y = savedFy;
            getGameView_802D6A9C()->fn_802D79E8(mLayer_0x4, 8.0f, 0.0f, false);
            gGameRoot_8053AAF8->startRumble_80134DE4(2, 0);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x59, 0, 0, NULL, NULL, true, 2.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x5A, 0, 0, NULL, NULL, true, 2.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x5B, 0, 0, NULL, NULL, true, 2.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x5C, 0, 0, NULL, NULL, true, 1.0f);
            if (mFlags & 0x2000) {
                spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x262, 0, 0, NULL, NULL, true, 1.0f);
                for (s32 g = 0; g < 4; g++) {
                    s32 count = lbl_80469C40[g][1];
                    for (s32 k = 0; k < count; k++) {
                        s32 serial = getActorManager_801F856C()->spawnActorSimple_801F8544(ACTOR_RUPY, mLayer_0x4, &mPos, lbl_80469C40[g][0]);
                        if (serial >= 0) {
                            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                            if (gem != NULL) {
                                // MWCC evaluates the TVec3f arguments right to left, so the original draws z, y, x;
                                // named in that order so every compiler draws them the same way
                                f32 vz = 6.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 6.0f;
                                f32 vy = 0.6f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
                                f32 vx = 0.6f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
                                gem->mVel_0x3c = JGeometry::TVec3f(vx, vy, vz);
                            }
                        }
                    }
                }
            } else {
                spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x261, 0, 0, NULL, NULL, true, 1.0f);
            }
            s32 bit = field_0x2b4;
            if (bit != 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(bit, true, 0xFF);
            }
            startSound_801F0E34(0x30001, 0);
            markDead_801F3668();
        }
        break;
    case 2:
        break;
    }
    mStateTimer = 0;
    mState = state;
}

void IronPot_804A5418::updateState_0x90(u8 mapHit) {
    switch (mState) {
    case 2:
        if (fn_80210A6C()) {
            mFlags |= 0x10000;
            fn_801E728C(1);
        }
        break;
    case 1:
        break;
    }
}

u8 IronPot_804A5418::checkHitByActor_0x60() {
    s32 serial;
    s32 skip = -1;
    if (mState == 0 && field_0x2ac == -1) {
        f32 savedIx = mBox.i.x;
        f32 savedIy = mBox.i.y;
        f32 savedFx = mBox.f.x;
        f32 savedFy = mBox.f.y;
        f32 cx = fn_80210B34(mDir_0x98);
        f32 cy = fn_80210AF8(mDir_0x98);
        mBox.i.x = cx - lbl_8053F650;
        mBox.i.y = cy - lbl_8053F650;
        mBox.f.x = cx + lbl_8053F650;
        mBox.f.y = cy + lbl_8053F650;
        JGeometry::TVec2f off;
        fn_801E9C00(field_0x2b0, &off);
        mBox.i.x += off.x;
        mBox.i.y += off.y;
        mBox.f.x += off.x;
        mBox.f.y += off.y;
        skip = fn_801E9B58();
        if (skip >= 0) {
            fn_801E728C(1);
        }
        mBox.i.x = savedIx;
        mBox.i.y = savedIy;
        mBox.f.x = savedFx;
        mBox.f.y = savedFy;
    }
    if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        serial = getActorManager_801F856C()->findOverlappingActor_801F82CC(this, field_0x195, 32.0f);
        if (serial >= 0 && serial != mSerial && serial != skip) {
            ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (other != NULL) {
                if (other->mActorId == ACTOR_FIRE && ((FireRodFire_804A89F0*)other)->fn_802BA9F8() == 1) {
                    if (mState == 0) {
                        fn_801E728C(2);
                    }
                } else if (other->mActorId == 'SWBM') {
                    fn_801CB538(mLayer_0x4, &other->mPos, 0, 0);
                } else if (field_0x1dc.fn_801EAC54(other->mSerial)) {
                    switch (other->mActorId) {
                    case ACTOR_FIRE:
                        startSound_801F0E34(0x3002E, 0);
                        break;
                    }
                }
                other->field_0x164 = 1;
            }
        }
    }
    return 0;
}

void IronPot_804A5418::update() {
    if ((mFlags & 0x8000) == 0) {
        EnemyBase_804A4808::update();
    }
}

void IronPot_804A5418::draw() {
    if ((mFlags & 0x8000) == 0) {
        s32 arg = 0;
        if (mState == 2) {
            arg = (s32)field_0x2a8;
        }
        fn_80210630((mFlags >> 13) & 1, 0, 0, arg);
    }
}

void IronPot_804A5418::fn_801E6EAC() {
    if (mFlags & 0x8000) {
        s32 bit = field_0x2b4;
        if (bit != 0) {
            getGameView_802D6A9C()->setSwitchBit_802D8050(bit, true, 0xFF);
        }
        markDead_801F3668();
    } else if (mState != 1) {
        fn_801E728C(1);
    }
}

void IronPot_804A5418::fn_801E6DE8() {
    if (mFlags & 0x8000) {
        return;
    }
    static f32 lbl_8053AC38 = 128.0f;
    switch (mState) {
    case 0:
    case 2: {
        f32 dx = fn_80210B34(mDir_0x98);
        f32 dy = fn_80210AF8(mDir_0x98);
        JGeometry::TVec2f pos(mPos.x + dx, mPos.y + dy);
        fn_801E9AFC()->fn_801EA108(&pos, field_0x2b0, lbl_8053AC38);
        break;
    }
    }
}

IronPot_804A5418::~IronPot_804A5418() {}

