#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_dodongo.h"

extern const char* lbl_804A3208[];
extern const s32 lbl_804689C0[];
extern const f32 lbl_804A31C8[];
extern s32 lbl_8053ABE0;

void Dodongo_804A3334::fn_801AE5AC() {
    if (mTimers[0] != 0) {
        return;
    }

    switch (field_0x290) {
    case 0:
        mTimers[0] = 6;
        field_0x258 = field_0x28c;
        field_0x258 += field_0x98 * 3 + field_0x24c;
        field_0x24c++;
        if (field_0x24c > 2) {
            field_0x24c = 0;
            startSound_801F0E34(0x50029, 0);
        }
        break;
    case 1:
        mTimers[0] = 9;
        field_0x258 = field_0x28c;
        field_0x258 += field_0x98 * 3 + field_0x24c;
        if (field_0x24c == 0) {
            startSound_801F0E34(0x5002A, 0);
        }
        field_0x24c++;
        if (field_0x24c > 2) {
            field_0x24c = 2;
            field_0x298 = 1;
        }
        break;
    case 2:
        mTimers[0] = 9;
        field_0x258 = field_0x28c;
        field_0x258 += field_0x98 * 2 + field_0x24c;
        field_0x24c++;
        if (field_0x24c > 1) {
            field_0x24c = 1;
            field_0x298 = 1;
        }
        break;
    case 3:
        field_0x258 = field_0x28c;
        field_0x258 += field_0x98;
        break;
    case 4:
        if (field_0x24c > 2) {
            field_0x24c = 3;
            field_0x298 = 1;
        } else {
            field_0x258 = field_0x28c;
            field_0x258 = field_0x258 + field_0x24c;
            mTimers[0] = 10;
            field_0x24c++;
        }
        break;
    }
}

// FIXME: 96.62 %. different: same length, 3 opcodes differ. Register allocation (f30/f31, r29/r30) and boxData indexing.
void Dodongo_804A3334::fn_801ACCF0() {
    f32 scale = 1.0f;
    if (field_0x294 == 1) {
        scale = 1.5f;
    }

    switch (field_0x230) {
    case 12:
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1ac, 1, 1, NULL, NULL, true, scale);
        if (mEmitters[0] == NULL) {
            return;
        }
        mTimers[0] = 8;
        field_0x230 = 13;
        // fallthrough
    case 13:
        if (mEmitters[0] != NULL) {
            s32 rotX = 0;
            if (field_0x98 == 2) {
                rotX = 0x4000;
            }
            setEmitterPos_8013CB44(mEmitters[0], &mPos, 1, 1, true);
            JPAGetXYZRotateMtx(rotX, lbl_804689C0[field_0x98], -0x8000, mEmitters[0]->mGlobalRot);
            startSound_801F0E34(0x30011, 0);
        }

        if (mTimers[0] == 0) {
            const f32* boxData = &lbl_804A31C8[field_0x98 * 4];
            mBox.i.x = boxData[0] * scale;
            mBox.i.y = boxData[1] * scale;
            mBox.f.x = boxData[2] * scale;
            mBox.f.y = boxData[3] * scale;
            fn_801F2B7C(field_0x114, -1, 9);
            fn_801F2B20(5);

            JGeometry::TVec3f pos = mPos;
            if (field_0x254 == 0) {
                field_0x254 = 1;
                f32 stepX = (mBox.f.x - mBox.i.x) * 0.125f;
                f32 stepY = (mBox.f.y - mBox.i.y) * 0.125f;
                pos.x = mPos.x + mBox.i.x;
                for (s32 i = 0; i < 8; i++) {
                    pos.y = mPos.y + mBox.i.y;
                    for (s32 j = 0; j < 8; j++) {
                        if ((s32)fn_8022461C(&pos, field_0x4) == 0x21 || fn_8022517C(&pos, field_0x4) == 1) {
                            getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &pos, 0xE0C0, -1, -1);
                        }
                        pos.y += stepY;
                    }
                    pos.x += stepX;
                }
            }
        }

        if (mLife <= 0) {
            for (s32 i = 0; i < 2; i++) {
                if (mEmitters[i] != NULL) {
                    mEmitters[i]->becomeInvalidEmitter();
                    mEmitters[i] = NULL;
                }
            }
            mAlive = false;
        }
        break;
    }
}

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
