#include "game/GameAudio.h"
#include "game/act/act_liftable_item.h"
#include "game/act/act_stop_npc.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"

s32 lbl_804A0170[12] = {
    1, 5, 20, 50, 100, 150, 200, 300, 1000, 1500, 2000, 3000,
};
s32 lbl_804A01A0[12] = {
    1, 5, 9, 13, 17, 21, 25, 29, 0x45A, 0x456, 0x462, 0x45E,
};

void LiftableItem_804A01D0::init() {
    field_0x234 = field_0x90 & 0xFFF;
    field_0x250 = (field_0x90 >> 24) & 3;
    field_0x24c = (field_0x90 >> 26) & 3;
    field_0x254 = (field_0x90 >> 28) & 1;
    field_0x268 = (field_0x90 >> 23) & 1;
    field_0x26c = (field_0x90 >> 22) & 1;
    field_0x270 = (field_0x90 >> 21) & 1;
    field_0x258 = (field_0x90 >> 31) & 1;
    field_0x25c = (field_0x90 >> 30) & 1;
    field_0x264 = (field_0x90 >> 29) & 1;
    mEmitter2a8 = NULL;
    mEmitter2ac = NULL;
    mEmitter2b0 = NULL;
    field_0x290 = -1;
    field_0x244 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 26;
    field_0x29c.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        field_0x238 = 90;
    } else {
        field_0x238 = 45;
    }
    field_0x288 = 0;
    field_0x27c = *getPlayerPos_8023E724(field_0x250);
    field_0x274 = field_0x250;
    if (fn_8022F514(field_0x250)) {
        field_0x274 = fn_8022F3DC(field_0x250);
    }
    field_0x28c = 10;
    switch (field_0x234) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (field_0x25c == 0) {
            fn_802362C4(field_0x274, lbl_804A0170[field_0x234]);
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        if (field_0x25c == 0) {
            lbl_8053AED0->fn_8043D874(field_0x274, lbl_804A0170[field_0x234], 0);
        }
        if (field_0x270 != 0) {
            field_0x28c = 2;
        }
        break;
    case 0x2E:
        mPos.z = field_0x27c.z + 22.0f;
        field_0x290 = getActorManager_801F856C()->spawnActor_801F9484('BBMB', field_0x4, &mPos, 0x20000, -1, -1);
        break;
    }
    field_0x230 = 0;
    if (field_0x258 == 0) {
        if (field_0x28c != 2) {
            field_0x260 = getActorManager_801F856C()->spawnActor_801F9484('STPN', field_0x4, &mPos, 0, -1, -1);
            if (gGameRoot_8053AAF8->field_0x96 == 0) {
                field_0x260 = 0;
            }
        } else {
            if (gGameRoot_8053AAF8->field_0x7e == 0) {
                field_0x238 = 45;
            } else {
                field_0x28c = 10;
            }
            field_0x260 = 0;
        }
    } else {
        field_0x260 = 0;
    }
    field_0x278 = -1;
    field_0x294 = -1;
}

LiftableItem_804A01D0::~LiftableItem_804A01D0() {
    releaseEmitter_801EE434(mEmitter2a8);
    releaseEmitter_801EE434(mEmitter2ac);
    releaseEmitter_801EE434(mEmitter2b0);
}

void LiftableItem_804A01D0::draw() {
    if (!fn_8023188C(field_0x250)) {
        return;
    }
    s32 r30 = 0;
    if (field_0x234 >= 8 && field_0x234 <= 11) {
        r30 = 1;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1d = 1;
    params.field_0x1c = 1;
    params.field_0x2d = 1;

    if (field_0x234 == 0x2f) {
        return;
    }

    if (field_0x234 <= 7) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + lbl_804A01A0[field_0x234], &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + lbl_804A01A0[field_0x234], r30 != 0, 2, -1, true, 0, 0);
    } else if (field_0x234 <= 11) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x244 + lbl_804A01A0[field_0x234], &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + lbl_804A01A0[field_0x234 - 4], r30 != 0, 2, -1, true, 0, 0);
    } else if (field_0x234 >= 0x10 && field_0x234 <= 0x19) {
        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
        s32 id = field_0x234 + 0x1d;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234 + 0x1d, false, 2, -1, true, 0, 0);
    } else if (field_0x234 == 0x20) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x90, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x90, false, 2, -1, true, 0, 0);
    } else if (field_0x234 == 0x21) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x2af, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x2af, false, 2, -1, true, 0, 0);
    } else if (field_0x234 == 0x22) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x8c, &params, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x8c, false, 2, -1, false, 0, 0);
    } else if (field_0x234 == 0x23) {
        params.field_0x2d = 1;
        drawSprite_801F06F0("enemy_both2_sob.bin", 0x2aa, &params, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, 0x2aa, false, 2, -1, false, 0, 0);
    } else if (field_0x234 == 0x24) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x2c7, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x2c7, false, 2, -1, true, 0, 0);
    } else if (field_0x234 == 0x25) {
        drawSprite_801F06F0("enemy_both2_sob.bin", 0x98, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, 0x98, false, 2, -1, true, 0, 0);
    } else if (field_0x234 >= 0x2a && field_0x234 <= 0x2d) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x234 + 0x255, &params, -1, 1.0f, 1.0f);
    } else if (field_0x234 == 0x2e) {
        return;
    } else if (field_0x234 < 0x40) {
        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
        s32 id = field_0x234 + 0x11;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234 + 0x11, false, 2, -1, true, 0, 0);
    } else {
        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
        s32 id = field_0x234 + 0x1cd;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234 + 0x1cd, false, 2, -1, true, 0, 0);
    }
}

void LiftableItem_804A01D0::vfunc_0x24() {
    if (mEmitter2a8 != NULL) {
        mEmitter2a8->becomeInvalidEmitter();
        mEmitter2a8->stopCreateParticle();
        mEmitter2a8 = NULL;
    }
    if (mEmitter2ac != NULL) {
        mEmitter2ac->becomeInvalidEmitter();
        mEmitter2ac->stopCreateParticle();
        mEmitter2ac = NULL;
    }
    if (mEmitter2b0 != NULL) {
        mEmitter2b0->becomeInvalidEmitter();
        mEmitter2b0->stopCreateParticle();
        mEmitter2b0 = NULL;
    }
    s32 serial = field_0x260;
    if (serial != 0) {
        StopNPC_804AA2D8* actor = (StopNPC_804AA2D8*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->fn_802ED140();
        }
    }
    mAlive = false;
    serial = field_0x278;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x278 = -1;
    }
    if (field_0x294 >= 0) {
        field_0x27c = *getPlayerPos_8023E724(field_0x250);
        getActorManager_801F856C()->spawnActor_801F9484('LSTI', field_0x4, &field_0x27c, (field_0x298 << 22) | field_0x294, -1, -1);
    }
}

void LiftableItem_804A01D0::fn_801597EC() {
    s32 item = field_0x234;
    if (item == 0x10 || (u32)(item - 0x11) <= 1 || (u32)(item - 0x16) <= 2 || item == 0x19) {
        getActorManager_801F856C()->spawnActor_801F9484('KEY0', field_0x4, &mPos, item + 0xF0 + (field_0x250 << 12), -1, -1);
    } else if (item == 0x13) {
        fn_80236DB0(field_0x274);
    } else if (item == 0x15) {
        fn_80236C88(field_0x274);
    } else if (item == 0x20) {
        getActorManager_801F856C()->spawnActor_801F9484('CRTL', field_0x4, &mPos, (field_0x250 << 16) | 1, -1, -1);
    } else if (item == 0x21) {
        getActorManager_801F856C()->spawnActor_801F9484('NIWA', field_0x4, &mPos, (field_0x250 << 8) | 5, -1, -1);
    } else if (item == 0x22) {
        getActorManager_801F856C()->spawnActor_801F9484('BMTY', field_0x4, &mPos, (field_0x250 << 16) | 5, -1, -1);
    } else if (item == 0x23) {
        getActorManager_801F856C()->spawnActor_801F9484('NNJN', field_0x4, &mPos, 0, -1, -1);
    } else if (item == 0x24) {
        getActorManager_801F856C()->spawnActor_801F9484('NIWA', field_0x4, &mPos, (field_0x250 << 8) | 0x1005, -1, -1);
    } else if (item == 0x25) {
        getActorManager_801F856C()->spawnActor_801F9484('CRTL', field_0x4, &mPos, (field_0x250 << 16) | 3, -1, -1);
    } else if (item >= 0x30) {
        if (item == 0x4a) {
            fn_80236ED8(field_0x274, 2);
        } else {
            s32 r29 = 1;
            s32 r28 = item - 0x2f;
            if (item >= 0x40) {
                r28 = item - 0x3f;
                r29 = 2;
            }
            if (field_0x264 == 0) {
                if (field_0x254 == 0 && fn_802373EC(field_0x250) != 0 && gGameRoot_8053AAF8->field_0x7f == 1 && field_0x258 == 0) {
                    field_0x294 = fn_802373EC(field_0x274);
                    field_0x298 = fn_8023725C(field_0x274);
                }
                fn_802372F8(field_0x250, r28, r29);
            }
        }
    }
}

void LiftableItem_804A01D0::fn_801597C0() {
    vfunc_0x24();
}

s32 LiftableItem_804A01D0::fn_801597B8() {
    return field_0x230;
}
