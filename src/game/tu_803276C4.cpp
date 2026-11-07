#include "game/GameAudio.h"
#include "game/act/tu_803276C4.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "dolphin/mtx.h"

// The "TIMG" texture names (by field_0x250).
static const char* const lbl_804AB518[4] = {"said_a.bti", "said_b.bti", "said_c.bti", "said_d.bti"};

// FIXME(shotgun): 85.7 %, missing/extra: the OSIN search loop and the box constants
void cls_804AB528::init() {
    field_0x94 = field_0x90 & 0xF;
    field_0x240 = (field_0x90 >> 4) & 0xF;
    field_0x248 = (field_0x90 >> 8) & 0xF;
    field_0x25a = (field_0x90 >> 12) & 1;
    s32 serial = -1;
    while ((serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_OSIN, serial)) >= 0) {
        if (serial == mSerial) {
            continue;
        }
        cls_804AB528* other = static_cast<cls_804AB528*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (other != NULL && other->field_0x248 == field_0x248) {
            if (!fn_801F6758(&other->mPos, other->field_0x4, 0.0f)) {
                mAlive = false;
            }
        }
    }
    mLife = 10000;
    mPos.y -= 8.0f;
    field_0x260 = mPos;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x244 = 0;
    field_0x25c = 1.0f;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x24c = 10;
    field_0x250 = 0;
    field_0x258 = 0;
    field_0x259 = 0;
    field_0x254 = 0;
    field_0x255 = 0;
    field_0x256 = 0;
    field_0x257 = 0;
    if (field_0x25a == 1) {
        field_0x25c = 0.0f;
    }
    if (field_0x240 == 0) {
        field_0x80.i.x = -15.0f;
        field_0x80.i.y = -7.0f;
        field_0x80.f.x = 15.0f;
        field_0x80.f.y = 7.0f;
        mBox.i.x = -15.0f;
        mBox.i.y = -7.0f;
        mBox.f.x = 15.0f;
        mBox.f.y = 7.0f;
    } else if (field_0x240 == 1) {
        field_0x80.i.x = -31.0f;
        field_0x80.i.y = -15.0f;
        field_0x80.f.x = 31.0f;
        field_0x80.f.y = 15.0f;
        mBox.i.x = -31.0f;
        mBox.i.y = -15.0f;
        mBox.f.x = 31.0f;
        mBox.f.y = 15.0f;
    }
}

// FIXME(shotgun): 70.2 %, missing/extra: the Mtx diagonal setup and the sprite call order
void cls_804AB528::draw() {
    if (field_0x25c == 0.0f) {
        return;
    }
    if (getGameView_802D6A9C()->field_0x4 == 7 && getGameView_802D6A9C()->field_0x8 == 2) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804AB518[field_0x250]);
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, NULL);
        }
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    PSMTXIdentity(mtx);
    f32 scale = (f32)(field_0x240 + 1) * field_0x25c;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][2] = 0.0f;
    mtx[0][0] = scale;
    mtx[1][1] = scale;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scale;
    params.mMtx = mtx;
    s32 frame = field_0x234 + 0x141;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, frame, 0, 2, -1, field_0x240 != 0, 0, 0);
}

cls_804AB528::~cls_804AB528() {}
