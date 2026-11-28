#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_four_person_pull_switch.h"

static const s32 lbl_80474E70[15] = {
    30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1,
};

static inline void linkChild(FourPersonPullSwitch_804AE230* self, s32 i) {
    s32 serial = self->field_0x234[i];
    if (serial >= 0) {
        FourPersonPullSwitch_804AE230* child =
            (FourPersonPullSwitch_804AE230*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        child->field_0x230 = self->mSerial;
    } else {
        self->markDead_801F3668();
    }
}

void FourPersonPullSwitch_804AE230::init() {
    if (((mSpawnParam_0x90 >> 4) & 3) == 0) {
        for (s32 i = 0; i < 3; i++) {
            field_0x234[i] = -1;
            u32 param = (mSpawnParam_0x90 & 0xF800000F) | ((i + 1) << 4);
            field_0x234[i] = getActorManager_801F856C()->spawnActor_801F9484(
                ACTOR_PSW2, mLayer_0x4, &(mPos + JGeometry::TVec3f((f32)(i + 1) * 16.0f, 0.0f, 0.0f)), param, -1, -1);
            linkChild(this, i);
        }
    }
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mMapBox_0x80.i.x = 0.0f;
    mMapBox_0x80.i.y = 0.0f;
    mMapBox_0x80.f.x = 0.0f;
    mMapBox_0x80.f.y = 0.0f;
    mLife = 0;
    field_0x24c = 0;
    field_0x250 = isScreenFlagSet_0x34();
    if (field_0x250) {
        field_0x254 = 8.0f;
        mPos.y = mSpawnPos.y + field_0x254;
    } else {
        field_0x254 = 0.0f;
    }
    mVel_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x258 = 0;
    field_0x25c = -1;
    field_0x260 = 0;
}

void FourPersonPullSwitch_804AE230::update() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    mVel_0x3c.x = 0.0f;
    mVel_0x3c.y = 0.0f;
    mVel_0x3c.z = 0.0f;
    if (field_0x258 > 0) {
        field_0x258--;
    }
    fn_8036CBBC();
    s32 player = fn_80239470(&box, mLayer_0x4, mSerial);
    if (player >= 0) {
        fn_80239914(&box, mLayer_0x4, mSerial);
        f32 dx = mPos.x - getPlayerPos_8023E724(player)->x;
        if (__fabsf(dx) >= 2.0f) {
            fn_8023DFF4(player, &JGeometry::TVec3f(dx, 0.0f, 0.0f));
        }
        if (field_0x250) {
            field_0x254 = 8.0f;
            mPos.y = mSpawnPos.y + field_0x254;
        } else if (field_0x254 >= 4.0f) {
            field_0x254 += 0.1f;
            if (field_0x254 >= 8.0f) {
                field_0x254 = 8.0f;
                if (checkAllPulled() == true) {
                    field_0x250 = 1;
                    startSound_801F0E34(0x3004B, 0);
                    mPos.y = mSpawnPos.y + field_0x254;
                    mVel_0x3c.x = 0.0f;
                    mVel_0x3c.y = 4.0f;
                    mVel_0x3c.z = 0.0f;
                    if (fn_8023077C()) {
                        field_0x258 = 50;
                        field_0x25c = 4;
                        field_0x260 = 0;
                        for (s32 i = 0; i < 4; i++) {
                            JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
                            fn_80230498(i, &(playerPos + JGeometry::TVec3f(0.0f, 7.0f, 0.0f)), 0x2E2, 0xFF, -0x80, -0x80, true);
                        }
                    } else {
                        field_0x25c = 0;
                    }
                }
            } else {
                mPos.y = 4.0f + mSpawnPos.y;
            }
        } else {
            if (field_0x254 <= 0.0f) {
                field_0x240 = mPos;
                field_0x240.y += 16.0f;
                JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(player);
                fn_8023DFF4(player, &(field_0x240 - playerPos));
            }
            field_0x254 += 0.25f;
            mVel_0x3c.x = 0.0f;
            mVel_0x3c.y = 0.25f;
            mVel_0x3c.z = 0.0f;
            mPos.y = mSpawnPos.y + field_0x254;
        }
    } else if (field_0x250) {
        field_0x254 = 8.0f;
        mPos.y = mSpawnPos.y + field_0x254;
    } else {
        field_0x254 -= 0.25f;
        if (field_0x254 < 0.0f) {
            field_0x254 = 0.0f;
        }
        mVel_0x3c.x = 0.0f;
        mVel_0x3c.y = -0.25f;
        mVel_0x3c.z = 0.0f;
        mPos.y = mSpawnPos.y + field_0x254;
    }
    if (field_0x254 < 1.0f) {
        field_0x24c = 0;
    } else if (field_0x254 < 3.0f) {
        field_0x24c = 1;
    } else if (field_0x254 < 8.0f) {
        field_0x24c = 2;
    } else {
        field_0x24c = 4;
    }
}

void FourPersonPullSwitch_804AE230::draw() {
    s32 frame = field_0x24c + 0x147;
    if (mLayer_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&mSpawnPos, res, frame, NULL, 1.0f, 1.0f);
    }
    showGbaSprite_803075AC(mSerial, mLayer_0x4, &mSpawnPos, field_0x24c + 0x147, false, 1, -1, 0, 0, 0);
}

// A plain function at its place between fn_8036CBBC and draw, inlined into update; the
// linker dropped its out-of-line copy. Compiled there it pools 8.0f between fn_8036CBBC's
// 0 / 7 and draw's 1.0f, as in the original.
// FAKEMATCH(crack-pass): pool helper failing R2 (1 call site, 19-line definition): split out only where the .sdata2 order needs it.
bool FourPersonPullSwitch_804AE230::checkAllPulled() {
    if (((mSpawnParam_0x90 >> 4) & 3) != 0) {
        return false;
    }
    s32 i;
    for (i = 0; i < 3; i++) {
        FourPersonPullSwitch_804AE230* child = static_cast<FourPersonPullSwitch_804AE230*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(field_0x234[i]));
        if (child->field_0x254 < 8.0f) {
            return false;
        }
    }
    for (i = 0; i < 3; i++) {
        FourPersonPullSwitch_804AE230* child = static_cast<FourPersonPullSwitch_804AE230*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(field_0x234[i]));
        child->field_0x250 = 1;
    }
    return true;
}

// FIXME: 99.54% (operands: same opcodes, an immediate or offset differs; stack offsets for pos/playerPos and regalloc r28..r30).
// Called out of line by FourPersonPullSwitch_804AE230::update (0x8036CF84: bl fn_8036CBBC).
#pragma auto_inline off
void FourPersonPullSwitch_804AE230::fn_8036CBBC() {
    if (field_0x25c < 0) {
        return;
    }
    s32 i;
    if (field_0x25c < 4) {
        if (fn_8023077C()) {
            field_0x25c = 4;
            field_0x258 = 50;
            field_0x260 = 0;
            for (i = 0; i < 4; i++) {
                JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
                fn_80230498(i, &(playerPos + JGeometry::TVec3f(0.0f, 7.0f, 0.0f)), 0x2E2, 0xFF, -0x80, -0x80, true);
            }
        }
    } else if (field_0x260 == 0 && field_0x258 > 30) {
        for (i = 0; i < 4; i++) {
            JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
            fn_80230498(i, &playerPos, 0x2E2, 0xFF, -0x80, -0x80, true);
        }
    } else if (field_0x260 < 13) {
        if (field_0x258 == 0) {
            field_0x260++;
            field_0x258 = lbl_80474E70[field_0x260];
            for (i = 0; i < 4; i++) {
                JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
                fn_80230498(i, &playerPos, field_0x260 + 0xDB, 0xFF, -0x80, -0x80, true);
                if (field_0x260 == 1) {
                    fn_80230FD0(i, 0x10001);
                }
            }
            if (field_0x260 == 13) {
                field_0x258 = 25;
            }
        }
    } else if (field_0x258 == 0) {
        if (fn_80230534(false)) {
            field_0x25c = -1;
        }
    } else if (field_0x258 == 15) {
        setScreenFlag_0x30(0xFF, true);
    }
}
#pragma auto_inline reset

FourPersonPullSwitch_804AE230::~FourPersonPullSwitch_804AE230() {}
