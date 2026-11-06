#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/act/act_dodongo.h"

extern const char* lbl_804A3208[];
extern s32 lbl_8053ABE0;

Dodongo_804A3334::~Dodongo_804A3334() {
    s32 serial;
    for (s32 i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }

    serial = mTargetSerial;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            mTargetSerial = -1;
            actor->mLife = 0;
        }
    }

    if (field_0x234 == 2) {
        lbl_8053ABE0--;
    }
}

void Dodongo_804A3334::update() {
    switch (field_0x234) {
    case 0:
    case 2:
        fn_801ACFF4();
        break;
    case 1:
        fn_801ACCF0();
        break;
    }

    for (s32 i = 0; i < 5; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

void Dodongo_804A3334::fn_801AC930() {
    SpriteParams_801F5AE0 params;
    f32 scale = 1.0f;
    s32 texIdx = field_0x258;
    if (mTimers[3] & 2) {
        return;
    }

    if ((u32)texIdx <= 2 || (u32)(texIdx - 12) <= 2 || (u32)(texIdx - 24) <= 1 || texIdx == 32) {
        scale = -1.0f;
    }

    Mtx mtx;
    f32 scaleX = field_0x274[0] * scale;
    f32 scaleY = field_0x274[1];
    mtx[0][0] = scaleX;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = scaleY;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scaleY;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;

    void* res;
    if (field_0x294 == 1) {
        s32 index = texIdx + 0x24;
        res = fn_804032E8('TIMG', lbl_804A3208[index]);
    } else {
        res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A3208[texIdx]);
    }
    fn_801EFD34(res, &params, 7, 0.0f, -2.0f, 0.0f);
}

void Dodongo_804A3334::draw() {
    switch (field_0x234) {
    case 0:
    case 2:
        fn_801AC930();
        break;
    }
}
