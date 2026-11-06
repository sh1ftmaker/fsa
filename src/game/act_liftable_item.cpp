#include "game/GameAudio.h"
#include "game/act/act_liftable_item.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
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

void LiftableItem_804A01D0::fn_801597C0() {
    vfunc_0x24();
}

s32 LiftableItem_804A01D0::fn_801597B8() {
    return field_0x230;
}
