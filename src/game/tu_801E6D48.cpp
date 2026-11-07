#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/tu_801F9484.h"
#include "game/act/tu_801E6D48.h"
#include "game/tu_801E9AFC.h"

cls_804A5418::cls_804A5418() {
    field_0x2ac = -1;
    field_0x2b0 = 0;
    field_0x2b4 = 0;
}

// FIXME(shotgun): 92.02 %, different + reloc-name
void cls_804A5418::init() {
    EnemyBase_804A4808::init();
    field_0x98 = field_0x90 & 3;
    field_0x2b4 = (field_0x90 >> 2) & 0x1F;
    if (field_0x90 & 0x200000) {
        mFlags |= 0x8000;
    } else if (field_0x90 & 0x800000) {
        mFlags |= 0x2000;
    } else if (field_0x90 & 0x400000) {
        mFlags |= 0x4000;
    }
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    if (mFlags & 0x8000) {
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        field_0xb0 = 0;
    } else {
        fn_80210990(field_0x98, &mBox);
        field_0xb0 = 0x40004;
    }
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x2b0 = fn_801F6874(field_0x98);
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
}

void cls_804A5418::vfunc_0x10() {
    u32 flags = field_0x90 >> 24;
    s32 serial = mSerial;
    cls_801E9AFC* slots = fn_801E9AFC();
    field_0x2ac = slots->field_0x0[slots->field_0xf0].fn_801EA6D8(flags, serial);
}

extern const s32 lbl_80469C40[4][2];

// FIXME(shotgun): 85.01 %, missing/extra + reloc-name
void cls_804A5418::fn_801E728C(int state) {
    if (state == 1) {
        if ((mFlags & 0x4000) && !(mFlags & 0x10000)) {
            getActorManager_801F856C()->spawnActor_801F9484('IBBM', field_0x4, &mPos, field_0x90, field_0x16c, -1);
            mAlive = false;
        } else {
            JGeometry::TBox2f saved = mBox;
            fn_80210A30(&mBox);
            mBox.i.x *= 1.5f;
            mBox.i.y *= 1.5f;
            mBox.f.x *= 1.5f;
            mBox.f.y *= 1.5f;
            fn_801F2B7C(4, -1, 0x16);
            JGeometry::TVec2f center;
            fn_801F3128(&center);
            JGeometry::TVec3f spot;
            spot.y = center.y - 16.0f;
            for (s32 j = 0; j < 3; j++) {
                spot.x = center.x - 16.0f;
                for (s32 i = 0; i < 3; i++) {
                    if (fn_801EE3C0(fn_80226998(&spot, field_0x4))) {
                        JGeometry::TVec3f at(spot.x, spot.y, 0.0f);
                        getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &at, 15, -1, -1);
                    }
                    spot.x += 16.0f;
                }
                spot.y += 16.0f;
            }
            mBox = saved;
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 8.0f, 0.0f, false);
            gGameRoot_8053AAF8->fn_80134DE4(2, 0);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x59, 0, 0, NULL, NULL, true, 2.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x5A, 0, 0, NULL, NULL, true, 2.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x5B, 0, 0, NULL, NULL, true, 2.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x5C, 0, 0, NULL, NULL, true, 1.0f);
            if (mFlags & 0x2000) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x262, 0, 0, NULL, NULL, true, 1.0f);
                for (s32 g = 0; g < 4; g++) {
                    for (s32 k = 0; k < lbl_80469C40[g][1]; k++) {
                        s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, lbl_80469C40[g][0]);
                        if (serial >= 0) {
                            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                            if (gem != NULL) {
                                f32 z = 6.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 6.0f;
                                f32 y = 0.6f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
                                f32 x = 0.6f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
                                gem->field_0x3c.x = x;
                                gem->field_0x3c.y = y;
                                gem->field_0x3c.z = z;
                            }
                        }
                    }
                }
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x261, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0x2b4 != 0) {
                getGameView_802D6A9C()->fn_802D8050(field_0x2b4, true, 0xFF);
            }
            startSound_801F0E34(0x30001, 0);
            fn_801F3668();
        }
    }
    mStateTimer = 0;
    mState = state;
}

// FIXME(shotgun): 95.83 %, missing/extra
void cls_804A5418::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 2:
        if (fn_80210A6C()) {
            mFlags |= 0x10000;
            fn_801E728C(1);
        }
        break;
    }
}

// FIXME(shotgun): 77.48 %, missing/extra + reloc-name
u8 cls_804A5418::vfunc_0x60() {
    s32 skip = -1;
    if (mState == 0 && field_0x2ac == -1) {
        JGeometry::TBox2f saved = mBox;
        f32 cx = fn_80210B34(field_0x98);
        f32 cy = fn_80210AF8(field_0x98);
        mBox.i.x = cx - 1.0f;
        mBox.i.y = cy - 1.0f;
        mBox.f.x = cx + 1.0f;
        mBox.f.y = cy + 1.0f;
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
        mBox = saved;
    }
    if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        s32 serial = getActorManager_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
        if (serial >= 0 && serial != mSerial && serial != skip) {
            ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (other != NULL) {
                if (other->mActorId == ACTOR_FIRE) {
                    if (((FireRodFire_804A89F0*)other)->fn_802BA9F8() == 1) {
                        if (mState == 0) {
                            fn_801E728C(2);
                        }
                    }
                } else if (other->mActorId == 'SWBM') {
                    fn_801CB538(field_0x4, &other->mPos, 0, 0);
                } else if (field_0x1dc.fn_801EAC54(other->mSerial)) {
                    if (other->mActorId == ACTOR_FIRE) {
                        startSound_801F0E34(0x3002E, 0);
                    }
                }
                other->field_0x164 = 1;
            }
        }
    }
    return 0;
}

void cls_804A5418::update() {
    if ((mFlags & 0x8000) == 0) {
        EnemyBase_804A4808::update();
    }
}

void cls_804A5418::draw() {
    if ((mFlags & 0x8000) == 0) {
        s32 arg = 0;
        if (mState == 2) {
            arg = (s32)field_0x2a8;
        }
        fn_80210630((mFlags >> 13) & 1, 0, 0, arg);
    }
}

void cls_804A5418::fn_801E6EAC() {
    if (mFlags & 0x8000) {
        s32 bit = field_0x2b4;
        if (bit != 0) {
            getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
        }
        fn_801F3668();
    } else if (mState != 1) {
        fn_801E728C(1);
    }
}

void cls_804A5418::fn_801E6DE8() {
    if (mFlags & 0x8000) {
        return;
    }
    static f32 lbl_8053AC38 = 128.0f;
    switch (mState) {
    case 0:
    case 2: {
        f32 dx = fn_80210B34(field_0x98);
        f32 dy = fn_80210AF8(field_0x98);
        JGeometry::TVec2f pos(mPos.x + dx, mPos.y + dy);
        fn_801E9AFC()->fn_801EA108(&pos, field_0x2b0, lbl_8053AC38);
        break;
    }
    }
}

cls_804A5418::~cls_804A5418() {}

