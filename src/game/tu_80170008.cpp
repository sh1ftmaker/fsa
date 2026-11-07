#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/act/tu_801F9484.h"

// 0x80171AF8.
// FIXME(shotgun): 90.12 %, missing/extra + reloc-name
void cls_804A0810::init() {
    field_0x94 = field_0x90 & 0xFF;
    field_0x270 = (field_0x90 >> 8) & 1;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    for (s32 i = 0; i < 10; i++) {
        field_0x29c[i] = mPos;
    }
    field_0x290[2] = 0.0f;
    field_0x290[1] = 0.0f;
    field_0x290[0] = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x280 = 0.3f;
    field_0x278 = 0.0f;
    field_0x27c = 1.5f;
    field_0xb0 = 0xA002204;
    field_0xbc = 3;
    field_0x28c = NULL;
    field_0x264 = 0;
    if (fn_802DE814() == true) {
        mLife = 0x10;
    }
    if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 3) {
        mLife = 0x18;
    }
    field_0x268 = mLife;
    field_0x258 = 0;
    field_0x284 = 0.0f;
    field_0x288 = 70.0f;
    field_0x118 = 0x10;
    field_0x32c[0] = mPos.x;
    field_0x32c[1] = mPos.y;
    field_0x32c[2] = mPos.z;
    if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 3 && getGameView_802D6A9C()->field_0x18 == 3 &&
        getGameView_802D6A9C()->field_0x14 == 1) {
        field_0x244 = 100;
    }
    field_0x260 = 0;
    field_0x25c = 0;
    field_0x98 = 2;
    field_0x250 = 0x47E;
    field_0x23c = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 100;
    field_0x274 = 8;
    field_0x26c = 0;
    field_0x230 = 0;
    switch (field_0x94) {
    case 0:
        field_0x250 = 0x190;
        break;
    case 1:
        field_0x274 = 4;
        field_0x27c = 1.0f;
        field_0x288 = 10.0f;
        field_0x230 = 1;
        break;
    case 2:
        field_0x274 = 2;
        break;
    case 3:
        field_0x274 = 6;
        field_0x23c = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
        field_0x250 = 100;
        field_0x27c = 1.0f;
        field_0x288 = 10.0f;
        field_0x230 = 2;
        break;
    case 4:
        field_0x23c = 50;
        field_0x274 = 3;
        field_0x230 = 3;
        break;
    case 5:
        field_0x114 = 4;
        mLife = 10000;
        field_0x1a8 = false;
        break;
    }
    if ((field_0x90 >> 27) != 0) {
        field_0xb0 = 0;
    } else if (field_0x94 != 5) {
        field_0x26c = getActorManager_801F856C()->spawnActor_801F9484('SLR3', field_0x4, &mPos, 5, -1, -1);
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            gAudioMgr_8053AF80->fn_80458FF0(0x1000021, false, 0);
        }
    }
}

// 0x80171A28.
cls_804A0810::~cls_804A0810() {
    if (field_0x94 != 5) {
        if (gGameRoot_8053AAF8->field_0x4c != 9 && gGameRoot_8053AAF8->field_0x4c != 0xB && mLife != 0) {
            gGameRoot_8053AAF8->fn_80138B7C();
        }
        s32 serial = field_0x26c;
        if (serial != 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
    }
    releaseEmitter_801EE434(field_0x28c);
}

// 0x801719C0.
void cls_804A0810::vfunc_0x44() {
    if (mLife <= 0) {
        s32 serial = field_0x26c;
        if (serial != 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
                field_0x26c = 0;
            }
        }
    }
}
