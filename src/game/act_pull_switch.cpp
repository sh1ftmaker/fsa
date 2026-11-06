#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_pull_switch.h"

// result = a - b, the operands by value.
static inline void difference(JGeometry::TVec3f& result, JGeometry::TVec3f a,
                              const JGeometry::TVec3f& b) {
    a.sub(b);
    result = a;
}

void PullSwitch_804A7908::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
    field_0x23c = 0;
    field_0x240 = vfunc_0x34();
    if (field_0x240) {
        field_0x244 = 8.0f;
        mPos.y = mSpawnPos.y + field_0x244;
    } else {
        field_0x244 = 0.0f;
    }
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x248 = 0;
    field_0x24c = -1;
}

void PullSwitch_804A7908::update() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    if (field_0x248 > 0) {
        field_0x248--;
    }
    if (field_0x24c >= 0) {
        if (field_0x248 == 0) {
            if (fn_80230224(field_0x24c, false)) {
                field_0x24c = -1;
            }
        } else {
            JGeometry::TVec3f pos(field_0x230);
            pos.y += 8.0f;
            if (field_0x248 >= 0x70) {
                fn_80230498(field_0x24c, &pos, 0x2E2, 0xFF, -0x80, -0x80, true);
            } else if (field_0x248 >= 0x68) {
                fn_80230498(field_0x24c, &pos, 0x2E3, 0xFF, -0x80, -0x80, true);
            } else {
                fn_80230498(field_0x24c, &pos, 0x2E4, 0xFF, -0x80, -0x80, true);
            }
        }
    }
    s32 player = fn_80239470(&box, field_0x4, mSerial);
    if (player >= 0) {
        JGeometry::TVec3f playerPos;
        fn_80239914(&box, field_0x4, mSerial);
        f32 dx = mPos.x - getPlayerPos_8023E724(player)->x;
        if (__fabsf(dx) >= 2.0f) {
            JGeometry::TVec3f push(dx, 0.0f, 0.0f);
            fn_8023DFF4(player, &push);
        }
        if (field_0x240) {
            field_0x244 = 8.0f;
            mPos.y = mSpawnPos.y + field_0x244;
        } else if (field_0x244 >= 4.0f) {
            field_0x244 += 0.1f;
            if (field_0x244 >= 8.0f) {
                field_0x244 = 8.0f;
                field_0x240 = 1;
                startSound_801F0E34(0x3004B, 0);
                vfunc_0x30(0xFF, true);
                mPos.y = mSpawnPos.y + field_0x244;
                field_0x3c.x = 0.0f;
                field_0x3c.y = 4.0f;
                field_0x3c.z = 0.0f;
                if (fn_802300EC(player)) {
                    fn_80230AD4(player);
                    field_0x248 = 30;
                    field_0x24c = player;
                }
            } else {
                mPos.y = 4.0f + mSpawnPos.y;
            }
        } else {
            if (field_0x244 <= 0.0f) {
                field_0x230 = mPos;
                field_0x230.y += 16.0f;
                JGeometry::TVec3f diff;
                playerPos = *getPlayerPos_8023E724(player);
                difference(diff, field_0x230, playerPos);
                fn_8023DFF4(player, &diff);
            }
            field_0x244 += 0.25f;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.25f;
            field_0x3c.z = 0.0f;
            mPos.y = mSpawnPos.y + field_0x244;
        }
    } else if (field_0x240) {
        field_0x244 = 8.0f;
        mPos.y = mSpawnPos.y + field_0x244;
    } else {
        field_0x244 -= 0.25f;
        if (field_0x244 < 0.0f) {
            field_0x244 = 0.0f;
        }
        field_0x3c.x = 0.0f;
        field_0x3c.y = -0.25f;
        field_0x3c.z = 0.0f;
        mPos.y = mSpawnPos.y + field_0x244;
    }
    if (field_0x244 < 1.0f) {
        field_0x23c = 0;
    } else if (field_0x244 < 3.0f) {
        field_0x23c = 1;
    } else if (field_0x244 < 8.0f) {
        field_0x23c = 2;
    } else {
        field_0x23c = 4;
    }
}

void PullSwitch_804A7908::draw() {
    s32 frame = field_0x23c + 0x147;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mSpawnPos, res, frame, NULL, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mSpawnPos, field_0x23c + 0x147, false, 1, -1, 0, 0, 0);
}

PullSwitch_804A7908::~PullSwitch_804A7908() {}
