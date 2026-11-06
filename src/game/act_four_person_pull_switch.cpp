#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_four_person_pull_switch.h"

static const s32 lbl_80474E70[15] = {
    30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1,
};

// result = a + b, the operands by value.
static inline void sum(JGeometry::TVec3f& result, JGeometry::TVec3f a,
                       const JGeometry::TVec3f& b) {
    a.add(b);
    result = a;
}

// result = a - b, the operands by value.
static inline void difference(JGeometry::TVec3f& result, JGeometry::TVec3f a,
                              const JGeometry::TVec3f& b) {
    a.sub(b);
    result = a;
}

// FIXME: 98.77%. different + reloc-name (mr vs lwz on field_0x234[i], sum temporary stack offsets).
void FourPersonPullSwitch_804AE230::init() {
    if (((field_0x90 >> 4) & 3) == 0) {
        for (s32 i = 0; i < 3; i++) {
            field_0x234[i] = -1;
            u32 arg = (field_0x90 & 0xF800000F) | ((i + 1) << 4);
            JGeometry::TVec3f pos;
            sum(pos, mPos, JGeometry::TVec3f(16.0f * (i + 1), 0.0f, 0.0f));
            field_0x234[i] = getActorManager_801F856C()->spawnActor_801F9484(
                'PSW2', field_0x4, &pos, arg, -1, -1);
            if (field_0x234[i] >= 0) {
                FourPersonPullSwitch_804AE230* child = static_cast<FourPersonPullSwitch_804AE230*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(field_0x234[i]));
                child->field_0x230 = mSerial;
            } else {
                fn_801F3668();
            }
        }
    }
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
    field_0x24c = 0;
    field_0x250 = vfunc_0x34();
    if (field_0x250) {
        field_0x254 = 8.0f;
        mPos.y = mSpawnPos.y + field_0x254;
    } else {
        field_0x254 = 0.0f;
    }
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x258 = 0;
    field_0x25c = -1;
    field_0x260 = 0;
}

// FIXME: 99.44%. operands + reloc-name (stack frame 0xa0 vs 0xb0, regalloc r29/r30; .sdata2 pool order).
void FourPersonPullSwitch_804AE230::update() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    if (field_0x258 > 0) {
        field_0x258--;
    }
    fn_8036CBBC();
    s32 player = fn_80239470(&box, field_0x4, mSerial);
    if (player >= 0) {
        JGeometry::TVec3f playerPos;
        fn_80239914(&box, field_0x4, mSerial);
        f32 dx = mPos.x - getPlayerPos_8023E724(player)->x;
        if (__fabsf(dx) >= 2.0f) {
            JGeometry::TVec3f push(dx, 0.0f, 0.0f);
            fn_8023DFF4(player, &push);
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
                    field_0x3c.x = 0.0f;
                    field_0x3c.y = 4.0f;
                    field_0x3c.z = 0.0f;
                    if (fn_8023077C()) {
                        field_0x258 = 50;
                        field_0x25c = 4;
                        field_0x260 = 0;
                        for (s32 i = 0; i < 4; i++) {
                            JGeometry::TVec3f pos;
                            playerPos = *getPlayerPos_8023E724(i);
                            sum(pos, playerPos, JGeometry::TVec3f(0.0f, 7.0f, 0.0f));
                            fn_80230498(i, &pos, 0x2E2, 0xFF, -0x80, -0x80, true);
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
                JGeometry::TVec3f diff;
                playerPos = *getPlayerPos_8023E724(player);
                difference(diff, field_0x240, playerPos);
                fn_8023DFF4(player, &diff);
            }
            field_0x254 += 0.25f;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.25f;
            field_0x3c.z = 0.0f;
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
        field_0x3c.x = 0.0f;
        field_0x3c.y = -0.25f;
        field_0x3c.z = 0.0f;
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

// FIXME: 99.90%. reloc-name only (.sdata2 pool order: 1.0f/8.0f literals).
void FourPersonPullSwitch_804AE230::draw() {
    s32 frame = field_0x24c + 0x147;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mSpawnPos, res, frame, NULL, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mSpawnPos, field_0x24c + 0x147, false, 1, -1, 0, 0, 0);
}

// Inlined into update(); the original has no out-of-line copy. Without a separate emitted
// copy the .sdata2 pool order (8.0f vs draw's 1.0f) differs from the original (reloc-name only).
inline bool FourPersonPullSwitch_804AE230::checkAllPulled() {
    if (((field_0x90 >> 4) & 3) != 0) {
        return false;
    }
    for (s32 i = 0; i < 3; i++) {
        FourPersonPullSwitch_804AE230* child = static_cast<FourPersonPullSwitch_804AE230*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(field_0x234[i]));
        if (child->field_0x254 < 8.0f) {
            return false;
        }
    }
    for (s32 i = 0; i < 3; i++) {
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
    if (field_0x25c < 4) {
        if (fn_8023077C()) {
            field_0x25c = 4;
            field_0x258 = 50;
            field_0x260 = 0;
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
                JGeometry::TVec3f pos;
                sum(pos, playerPos, JGeometry::TVec3f(0.0f, 7.0f, 0.0f));
                fn_80230498(i, &pos, 0x2E2, 0xFF, -0x80, -0x80, true);
            }
        }
    } else if (field_0x260 == 0 && field_0x258 > 30) {
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
            fn_80230498(i, &playerPos, 0x2E2, 0xFF, -0x80, -0x80, true);
        }
    } else if (field_0x260 < 13) {
        if (field_0x258 == 0) {
            field_0x260++;
            field_0x258 = lbl_80474E70[field_0x260];
            for (s32 i = 0; i < 4; i++) {
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
        vfunc_0x30(0xFF, true);
    }
}
#pragma auto_inline reset

FourPersonPullSwitch_804AE230::~FourPersonPullSwitch_804AE230() {}
