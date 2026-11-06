#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_life_meter.h"

// The five heart images, picked by field_0x24c.
static const char* lbl_804B1738[] = {
    "kk_heart_05.bti", "kk_heart_03.bti", "kk_heart_02.bti", "kk_heart_01.bti", "kk_heart_00.bti",
};

void LifeMeter_804B174C::init() {
    for (s32 i = 0; i < 6; i++) {
        mTimers[i] = 0;
    }
    field_0x250 = mSerial;
    if ((field_0x90 & 0xF) != 0) {
        field_0x250 = field_0x90 - 1;
    }
    field_0x24c = 4;
    field_0x258 = 0.0f;
    field_0x254 = 0.0f;
    field_0x25c = 0.5f;
    field_0x260 = 0.5f;
    field_0x264 = 0.5f;
    field_0x268 = 0.5f;
    field_0x26c = 0.5f;
    field_0x270 = 0.5f;
    field_0x11e = 1;
    field_0x230 = 0;
}

// FIXME: 99%. The null check of the findActorBySerial_801F666C result is `mr. r30, r3` here; the original has
// `cmplwi r3, 0; mr r30, r3` (same registers otherwise). A `found = lookup; actor = found;`
// alias form matches but was rejected as steering.
void LifeMeter_804B174C::update() {
    for (s32 i = 0; i < 3; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
    mPos += field_0x3c;
    switch (field_0x230) {
    case 0:
        field_0x230 = 1;
        // fall through
    case 1: {
        JGeometry::TVec3f pos;
        ActorBase_804A5770* actor;
        if ((field_0x90 & 0xF) == 0) {
            actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x250);
            if (!actor || field_0x24c < 0) {
                mAlive = false;
                break;
            }
        } else {
            if (field_0x250 < 0 || field_0x250 > 3 || !fn_8023F264(field_0x250) ||
                field_0x24c < 0) {
                mAlive = false;
                break;
            }
        }
        if ((field_0x90 & 0xF) == 0) {
            pos.x = actor->mPos.x;
            pos.y = actor->mPos.y;
            pos.z = actor->mPos.z;
        } else {
            JGeometry::TVec3f* player = getPlayerPos_8023E724(field_0x250);
            pos.x = player->x;
            pos.y = player->y;
            pos.z = player->z;
        }
        pos.y += field_0x254;
        if (field_0x24c >= 0 && field_0x24c < 5) {
            pos.y -= 4 - field_0x24c;
            if (mTimers[0] == 0) {
                f32 scale = field_0x268;
                scale *= 0.3f * (5 - field_0x24c) + 1.0f;
                if (field_0x24c != 0) {
                    mTimers[0] = field_0x24c * 10;
                }
                field_0x25c = scale;
                field_0x260 = scale;
                field_0x264 = scale;
            }
            if (field_0x24c == 0) {
                field_0x25c = 0.5f;
                field_0x260 = 0.5f;
                field_0x264 = 0.5f;
                field_0x268 = 0.5f;
                field_0x26c = 0.5f;
                field_0x270 = 0.5f;
            }
            addCalc2_801F71A4(&field_0x25c, field_0x268, 0.2f, 0.4f);
            field_0x260 = field_0x25c;
        }
        mPos.x = pos.x;
        mPos.y = pos.y;
        mPos.z = pos.z;
        break;
    }
    }
}

void LifeMeter_804B174C::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    if (field_0x24c > 4 || field_0x24c < 0) {
        return;
    }
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1c = 1;
    params.field_0x1d = 7;
    f32 sy = field_0x260;
    mtx[0][0] = field_0x25c;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = sy;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = sy;
    params.mMtx = mtx;
    void* res = fn_804030C4('TIMG', lbl_804B1738[field_0x24c]);
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
    }
}

LifeMeter_804B174C::~LifeMeter_804B174C() {}
