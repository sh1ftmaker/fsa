#include "game/ActorId.h"
#include "game/GameAudio.h"
#include "game/act/act_all_enemies_defeated.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ViewLayer.h"
#include "game/Player.h"
#include "game/cls_804AE9B0.h"

u8 lbl_8053ABB0[8];
u8 lbl_8053ABB8[8];

static JGeometry::TVec3f lbl_804C1E30[1024];
static s32 lbl_804C4E30[1024];
static u8 lbl_804C5E30[1024];

void AllEnemiesDefeated_804A2AB0::init() {
    for (int i = 0; i < 1024; i++) {
        lbl_804C1E30[i].z = 0.0f;
        lbl_804C1E30[i].y = 0.0f;
        lbl_804C1E30[i].x = 0.0f;
        lbl_804C4E30[i] = -1;
        lbl_804C5E30[i] = 0;
    }

    mField_0x238 = field_0x90 & 0xF;
    mField_0x234 = (field_0x90 >> 4) & 3;
    mField_0x248 = 0;
    mField_0x24c = 0;
    mField_0x250 = 0;
    mField_0x254 = 0;
    mField_0x258 = 0;
    mField_0x25c = 0;
    mTimers[0] = 0;
    mTimers[1] = 0;
    mTimers[2] = 0;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;

    if (lbl_8053ABB0[0] == 1) {
        mAlive = false;
        return;
    }

    switch (mField_0x238) {
    case 0:
        mField_0x230 = 0;
        break;
    case 1:
        lbl_8053AD70->fn_8037AE00();
        lbl_8053ABB0[0] = 1;
        mField_0x248 = 1;
        mField_0x230 = 1;
        break;
    case 2:
    case 3:
        lbl_8053AD70->fn_8037AE00();
        lbl_8053ABB0[0] = 1;
        mField_0x248 = 1;
        mField_0x230 = 3;
        break;
    }
}

AllEnemiesDefeated_804A2AB0::~AllEnemiesDefeated_804A2AB0() {
    for (int i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }

    if (mField_0x248 == 1) {
        fn_8022ADCC();
        mField_0x248 = 0;
        gGameRoot_8053AAF8->fn_80134FC4(0);
        lbl_8053ABB0[0] = 0;
        lbl_8053AD70->fn_8037AE30();
    }
}

// FIXME: 97.80 %, regalloc-only. The original keeps count in r31, the i*4 offset in r29 and
// the actor in r30; ours uses r24, r31 and r22 and shifts the array-base registers to match.
void AllEnemiesDefeated_804A2AB0::fn_8019DC5C() {
    s32 count = getActorManager_801F856C()->mActorEnd;

    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor == NULL) {
            continue;
        }
        if (actor->field_0x4 != field_0x4) {
            continue;
        }
        if (actor->field_0x1a8 != 1) {
            continue;
        }
        if (actor->field_0x196 != 0) {
            continue;
        }

        lbl_804C4E30[i] = actor->field_0x16c;
        lbl_804C5E30[i] = 1;
        lbl_804C1E30[i].x = actor->mPos.x;
        lbl_804C1E30[i].y = actor->mPos.y;
        lbl_804C1E30[i].z = actor->mPos.z;

        switch (mField_0x238) {
        case 0: {
            s32 layer = actor->field_0x4;
            f32 scale1 = actor->fn_801EE4F0();
            spawnEmitter_8013CC50(layer, &actor->mPos, 0x172, 1, 1, NULL, NULL, true, scale1);
            layer = actor->field_0x4;
            f32 scale2 = actor->fn_801EE4F0();
            spawnEmitter_8013CC50(layer, &actor->mPos, 0x173, 1, 1, NULL, NULL, true, scale2);
            actor->fn_801F4694();
            actor->fn_801F3668();
            break;
        }
        case 1: {
            s32 layer = actor->field_0x4;
            f32 scale = actor->fn_801EE4F0();
            spawnEmitter_8013CC50(layer, &actor->mPos, 0x35c, 1, 1, NULL, NULL, true, scale);
            actor->mAlive = false;
            break;
        }
        case 2:
        case 3:
            lbl_804C4E30[i] = 4;
            mField_0x250++;
            mField_0x254 = count;
            break;
        }
    }

    for (s32 j = 0; j < count; j++) {
        if (lbl_804C5E30[j] == 1) {
            switch (mField_0x238) {
            case 0:
                getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &lbl_804C1E30[j], 2);
                break;
            case 1:
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_ZASL, field_0x4, &lbl_804C1E30[j], lbl_804C4E30[j], 0, -1);
                break;
            }
        }
    }
}

void AllEnemiesDefeated_804A2AB0::fn_8019D958() {
    JGeometry::TVec3f pos;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    pos.x = -camera->field_0x4[0][3];
    pos.y = -camera->field_0x4[1][3];
    pos.z = 0.0f;

    switch (mField_0x230) {
    case 1:
        gGameRoot_8053AAF8->fn_80134FC4(1);
        fn_8022ADE4();
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x8E, 1, 7, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x8F, 1, 7, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x90, 1, 7, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x421, 1, 7, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x422, 1, 7, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x423, 1, 7, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x425, 1, 7, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30001, 0);
        gGameRoot_8053AAF8->fn_80134DE4(3, 0);
        s32 layer;  // Declared outside the loop: matches the original's register allocation.
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                layer = field_0x4;
                if (layer == getPlayerLayer_8023DE58(i)) {
                    spawnEmitter_8013CC50(field_0x4, getPlayerPos_8023E724(i), 0x424, 1, 4, NULL, NULL, true, 1.0f);
                }
            }
        }
        spawnEmitter_8013CC50(field_0x4, &pos, 0x550, 1, 7, NULL, NULL, true, 1.0f);
        mField_0x230 = 2;
        break;
    case 2:
        startSound_801F0E34(0x3005B, 0);
        mField_0x24c++;
        if (mField_0x24c == 190) {
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 8.0f, 0.0f, false);
            startSound_801F0E34(0x3005C, 0);
            fn_8019DC5C();
        } else {
            fn_801F0D20(0x4F);
            if (mField_0x24c >= 260) {
                mAlive = false;
                gGameRoot_8053AAF8->fn_80134FC4(0);
                fn_8022ADCC();
                lbl_8053AD70->fn_8037AE30();
                mField_0x248 = 0;
                lbl_8053ABB0[0] = 0;
            }
        }
        break;
    }
}

// FIXME: 98.90 %, scheduling+regalloc. The original allocates this to r29 and actor
// to r28; ours swaps this to r28 and actor to r29.
void AllEnemiesDefeated_804A2AB0::fn_8019D2AC() {
    JGeometry::TVec3f pos;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    pos.x = -camera->field_0x4[0][3];
    pos.y = -camera->field_0x4[1][3];
    pos.z = 0.0f;

    switch (mField_0x230) {
    case 3:
        gGameRoot_8053AAF8->fn_80134FC4(1);
        if (mField_0x25c == 0) {
            if (mField_0x238 != 3) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x73, 1, 7, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x74, 1, 7, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x75, 1, 7, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x30001, 0);
            }
            fn_8022ADE4();
        }
        mField_0x25c++;
        if (mField_0x25c >= 60) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x61, 1, 7, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x62, 1, 7, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x63, 1, 7, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x64, 1, 7, NULL, NULL, true, 1.0f);
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &pos, 0x45C, 1, 7, NULL, NULL, true, 1.0f);
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &pos, 0x45D, 1, 7, NULL, NULL, true, 1.0f);
            fn_8019DC5C();
            mField_0x230 = 4;
        }
        break;
    case 4:
        mField_0x24c++;
        if (mField_0x24c == 2) {
            startSound_801F0E34(0x3005D, 0);
            gGameRoot_8053AAF8->fn_80134DE4(2, 0);
        }
        if (mField_0x24c < 90) {
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 8.0f, 0.0f, false);
            break;
        }
        mField_0x24c = 2;
        mField_0x258 = 0;
        mField_0x230 = 5;
        // fallthrough
    case 5:
        mField_0x24c++;
        if (mField_0x24c > 2) {
            mField_0x24c = 0;
            if (mField_0x258 < mField_0x250) {
                for (s32 i = 0; i < mField_0x254; i++) {
                    if (lbl_804C5E30[i] == 1) {
                        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                        if (actor != NULL) {
                            if (lbl_804C4E30[i] == 4) {
                                pos.x = lbl_804C1E30[i].x;
                                pos.y = lbl_804C1E30[i].y;
                                pos.z = lbl_804C1E30[i].z;
                                spawnEmitter_8013CC50(field_0x4, &pos, 0x18, 1, 7, NULL, NULL, true, 1.0f);
                                JPABaseEmitter* emitter = spawnEmitter_8013CC50(field_0x4, &pos, 0x19, 1, 7, NULL, NULL, true, actor->fn_801EE4F0());
                                startSound_801F0E34(0x3005E, 0);
                                if (emitter != NULL) {
                                    getEffectSystem_8013C824()->fn_8013C830(mField_0x234, emitter);
                                }
                                spawnEmitter_8013CC50(field_0x4, &pos, 0x1A, 1, 7, NULL, NULL, true, actor->fn_801EE4F0());
                                lbl_804C4E30[i]--;
                                break;
                            } else {
                                lbl_804C4E30[i]--;
                                if (lbl_804C4E30[i] <= 0) {
                                    lbl_804C5E30[i] = 0;
                                    mField_0x258++;
                                    actor->fn_801F4694();
                                    actor->fn_801F3668();
                                    break;
                                }
                            }
                        } else {
                            lbl_804C5E30[i] = 0;
                            mField_0x258++;
                            break;
                        }
                    }
                }
            }
            if (mField_0x258 >= mField_0x250) {
                mField_0x254 = 0;
                mField_0x230 = 6;
            }
        }
        break;
    case 6:
        if (mField_0x258 < 50) {
            mField_0x24c++;
            if (mField_0x24c > 2) {
                mField_0x24c = 0;
                pos.x += 150.0f - 300.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                pos.y += 100.0f - 200.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                spawnEmitter_8013CC50(field_0x4, &pos, 0x18, 1, 7, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x3005F, 0);
                mField_0x258++;
            }
        } else {
            if (mField_0x254 == 0) {
                spawnEmitter_8013CC50(field_0x4, &pos, 0x65, 1, 7, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x30090, 0);
                for (s32 i = 0; i < 2; i++) {
                    if (mEmitters[i] != NULL) {
                        mEmitters[i]->becomeInvalidEmitter();
                        mEmitters[i] = NULL;
                    }
                }
            }
            mField_0x254++;
            if (mField_0x254 >= 50) {
                mAlive = false;
                gGameRoot_8053AAF8->fn_80134FC4(0);
                lbl_8053AD70->fn_8037AE30();
                fn_8022ADCC();
                mField_0x248 = 0;
                lbl_8053ABB0[0] = 0;
            }
        }
        break;
    }
}

void AllEnemiesDefeated_804A2AB0::update() {
    JGeometry::TBox2f localBox;
    localBox = *layerBox(field_0x4);

    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 5;
        camera->field_0x808.set(localBox);
        camera->field_0x7f6 = 0;
    }

    switch (mField_0x238) {
    case 0:
        switch (mField_0x230) {
        case 0:
            if (vfunc_0x34()) {
                fn_801F0D20(0x42);
                fn_8019DC5C();
                mAlive = false;
            }
            break;
        }
        break;
    case 1:
        fn_8019D958();
        break;
    case 2:
    case 3:
        fn_8019D2AC();
        break;
    }

    for (s32 i = 0; i < 3; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}
