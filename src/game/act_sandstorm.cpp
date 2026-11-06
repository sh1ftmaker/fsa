// Actor SDSM ("Sandstorm"): shows a sandstorm effect that follows the camera.
// When the view reports field_0xc9 it ends the effect, starts the next one
// and, 100 frames later, changes the stage.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_sandstorm.h"
#include "JSystem/JParticle/JPAEmitter.h"

void Sandstorm_804AA280::init() {
    mKind = field_0x90 & 0xF;
    field_0x254 = (field_0x90 >> 4) & 0xF;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    mPos244.x = -camera->field_0x4[0][3];
    mPos244.y = -camera->field_0x4[1][3];
    mPos244.z = 0.0f;
    if ((mKind & 1) != 0) {
        mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos244, 0x3E6, 1, 1, NULL, NULL, true, 1.0f);
    } else if ((mKind & 2) != 0) {
        mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos244, 0x3E5, 1, 1, NULL, NULL, true, 1.0f);
    } else if ((mKind & 4) != 0) {
        mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos244, 0x3E4, 1, 1, NULL, NULL, true, 1.0f);
    } else {
        mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos244, 0x3EB, 1, 1, NULL, NULL, true, 1.0f);
    }
    mTimer = 0;
    mStarted = 0;
    mSound = new ActorSoundObj_804578F4(NULL, NULL, 8);
}

Sandstorm_804AA280::~Sandstorm_804AA280() {
    if (mEmitter != NULL) {
        releaseEmitter_801EE434(mEmitter);
    }
    if (mSound != NULL) {
        delete mSound;
    }
}

void Sandstorm_804AA280::update() {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    mPos244.x = -camera->field_0x4[0][3];
    mPos244.y = -camera->field_0x4[1][3];
    mPos244.z = 0.0f;
    setEmitterPos_8013CB44(mEmitter, &mPos244, 1, 1, true);
    if (mTimer != 0) {
        mTimer--;
        if (mTimer == 0) {
            fn_80230534(false);
            u32 bits = field_0x90;
            getGameView_802D6A9C()->fn_802D9FF4((bits >> 8) & 0xF, (bits >> 12) & 0xF);
            gGameRoot_8053AAF8->fn_80136798(13, 0);
        }
    } else {
        if (gGameRoot_8053AAF8->field_0x4c != 11) {
            mSound->startSound(0x50079);
        }
        if (mStarted == 0 && getGameView_802D6A9C()->field_0xc9 != 0) {
            mStarted = 1;
            mEmitter->becomeInvalidEmitter();
            mEmitter = NULL;
            if ((mKind & 1) != 0) {
                mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos244, 0x3E9, 1, 1, NULL, NULL, true, 1.0f);
            } else if ((mKind & 2) != 0) {
                mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos244, 0x3E8, 1, 1, NULL, NULL, true, 1.0f);
            } else if ((mKind & 4) != 0) {
                mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos244, 0x3E7, 1, 1, NULL, NULL, true, 1.0f);
            } else {
                mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos244, 0x3EA, 1, 1, NULL, NULL, true, 1.0f);
            }
            mTimer = 100;
            fn_8023077C();
            mSound->startSound(0x400B9);
        }
    }
}

void Sandstorm_804AA280::draw() {}
