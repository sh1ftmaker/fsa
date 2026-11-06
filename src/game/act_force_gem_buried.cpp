#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_force_gem_buried.h"

static const s32 lbl_80471740[3] = {8, 4, 0};

s32 lbl_804AB248[4] = {0x114, 0x117, 0x11A, 0x11D};

void BuriedForceGem_804AB258::init() {
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        fn_801F3668();
        return;
    }
    field_0x244 = field_0x90 & 3;
    field_0x248 = (field_0x90 >> 8) & 0xFF;
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x26c = 0;
    field_0x238 = 0;
    field_0x234 = 0;
    for (int i = 0; i < 4; i++) {
        field_0x24c[i] = 0;
        field_0x25c[i] = 0;
    }
    mLife = 10000;
    field_0x23c = 0;
    field_0x240 = -1;
    field_0x230 = 0;
    mPos.x = 16.0f * ((s32)mPos.x / 16) + 8.0f;
    mPos.y = 16.0f * ((s32)mPos.y / 16) + 8.0f;
    if (field_0x94 == 1 || field_0x94 == 4) {
        field_0x230 = 1;
        field_0x26c = fn_80226850(&mPos, field_0x4);
    }
    if (field_0x94 == 2) {
        field_0x230 = 2;
    }
    if (field_0x94 == 3 || field_0x94 == 5) {
        field_0x230 = 2;
    }
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    mBox.i.x = -4.0f;
    mBox.i.y = -4.0f;
    mBox.f.x = 4.0f;
    mBox.f.y = 4.0f;
}

void BuriedForceGem_804AB258::update() {
    JGeometry::TVec2f p(mPos.x, mPos.y);
    mPos += field_0x3c;
    JGeometry::TBox2f box(mBox);
    box.addPos(p);
    switch (field_0x230) {
    case 0: {
        if (field_0x94 == 2) {
            if (!vfunc_0x34()) {
                field_0x230 = 2;
                break;
            }
        }
        JGeometry::TVec2f out;
        s32 player = fn_8023A89C(&box, field_0x4, &out);
        if (player >= 0) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(player);
            JGeometry::TVec3f vel;
            fn_8023A654(player);
            s32 dir = fn_80230CFC(player);
            if (dir == 0) {
                pos.x += 14.0f;
                vel.x = 1.0f;
                vel.y = 0.0f;
                vel.z = 3.0f;
            } else if (dir == 1) {
                pos.x -= 14.0f;
                vel.x = -1.0f;
                vel.y = 0.0f;
                vel.z = 3.0f;
            } else if (dir == 2) {
                pos.y += 14.0f;
                vel.x = 0.0f;
                vel.y = 1.0f;
                vel.z = 3.0f;
            } else {
                pos.y -= 14.0f;
                vel.x = 0.0f;
                vel.y = -1.0f;
                vel.z = 3.0f;
            }
            s32 serial;
            if (field_0x248 == 0) {
                serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &pos, field_0x244 + 4);
            } else if (field_0x248 == 1) {
                serial = getActorManager_801F856C()->fn_801F8544(ACTOR_ESRY, field_0x4, &pos, field_0x244 + 0x10000);
            }
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->field_0x3c = vel;
                }
            }
            fn_801F3668();
        }
        break;
    }
    case 1: {
        if (field_0x94 == 3 || field_0x94 == 5) {
            if (!vfunc_0x34()) {
                field_0x230 = 2;
                break;
            }
        }
        if (field_0x26c != fn_80226850(&mPos, field_0x4)) {
            if (field_0x94 == 4 || field_0x94 == 5) {
                fn_801F0D20(3);
                fn_801F3668();
                return;
            }
            fn_801F0D20(0x42);
            field_0x230 = 0;
            break;
        }
        for (int i = 0; i < 4; i++) {
            if (!fn_8022F478(i) && fn_802373EC(i) == 9 && fn_8023725C(i) == 2) {
                JGeometry::TVec3f pPos = *getPlayerPos_8023E724(i);
                switch (fn_80230CFC(i)) {
                case 0:
                    pPos += JGeometry::TVec3f(16.0f, 0.0f, 0.0f);
                    break;
                case 1:
                    pPos += JGeometry::TVec3f(-16.0f, 0.0f, 0.0f);
                    break;
                case 2:
                    pPos += JGeometry::TVec3f(0.0f, 16.0f, 0.0f);
                    break;
                case 3:
                    pPos += JGeometry::TVec3f(0.0f, -16.0f, 0.0f);
                    break;
                }
                if (PSVECDistance(&mPos, &pPos) < 8.0f) {
                    if (field_0x25c[i] < 3) {
                        s32 idx = fn_8022F340(i);
                        if ((u32)((gGameRoot_8053AAF8->field_0x83 >> (idx * 2)) & 3) == 2) {
                            gGameRoot_8053AAF8->fn_80134DE4(8, 0);
                            startSound_801F0E34(0x30080, 0);
                        } else {
                            fn_803037D4(i, 3);
                        }
                        field_0x24c[i] = 0x18;
                        field_0x25c[i] = 3;
                    }
                } else if (PSVECDistance(&mPos, &pPos) < 16.0f) {
                    if (field_0x25c[i] < 2) {
                        s32 idx = fn_8022F340(i);
                        if ((u32)((gGameRoot_8053AAF8->field_0x83 >> (idx * 2)) & 3) == 2) {
                            gGameRoot_8053AAF8->fn_80134DE4(7, 0);
                            startSound_801F0E34(0x3007F, 0);
                        } else {
                            fn_803037D4(i, 2);
                        }
                        field_0x24c[i] = 0x38;
                        field_0x25c[i] = 2;
                    }
                } else if (PSVECDistance(&mPos, &pPos) < 32.0f) {
                    if (field_0x25c[i] < 1) {
                        s32 idx = fn_8022F340(i);
                        if ((u32)((gGameRoot_8053AAF8->field_0x83 >> (idx * 2)) & 3) == 2) {
                            gGameRoot_8053AAF8->fn_80134DE4(6, 0);
                            startSound_801F0E34(0x3007E, 0);
                        } else {
                            fn_803037D4(i, 1);
                        }
                        field_0x24c[i] = 0x4A;
                        field_0x25c[i] = 1;
                    }
                }
            }
        }
        break;
    }
    case 2: {
        if (vfunc_0x34() == true) {
            if (field_0x94 == 2) {
                fn_801F0D20(3);
                field_0x230 = 0;
            } else if (field_0x94 == 3 || field_0x94 == 5) {
                field_0x26c = fn_80226850(&mPos, field_0x4);
                field_0x230 = 1;
            }
        }
        break;
    }
    }
    if (field_0x234 <= lbl_80471740[field_0x238]) {
        field_0x238++;
        if (field_0x238 >= 3) {
            field_0x238 = 0;
            field_0x234 = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 60;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (field_0x24c[i] > 0) {
            field_0x24c[i]--;
            if (field_0x24c[i] <= 0) {
                field_0x25c[i] = 0;
            }
        }
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
}

void BuriedForceGem_804AB258::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -2;
    params.field_0x1c = 0;
    params.field_0x2d = 1;
    if (field_0x230 == 0) {
        s32 frame = field_0x238 + lbl_804AB248[field_0x244];
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x238 + lbl_804AB248[field_0x244], 0, 0, -1);
    }
}

BuriedForceGem_804AB258::~BuriedForceGem_804AB258() {}
