#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/ActorManager.h"
#include "game/act/act_enemy_generator_3.h"

// Actor ids indexed by mSpawnParam_0x90 & 0x7F. SLR2 and SLR3 are not in ActorId.h.
static const u32 lbl_8046F398[46] = {
    ACTOR_STAL, ACTOR_KETH, ACTOR_SLR1, ACTOR_SLR1, ACTOR_DDGO, ACTOR_GOMA, ACTOR_DDRK, ACTOR_BZBR, 'SLR2', ACTOR_PAME,
    ACTOR_LPLK, ACTOR_MIRA, ACTOR_SLR4, ACTOR_RIVR, ACTOR_GDMN, ACTOR_MOZO, ACTOR_PEAC, ACTOR_ROPE, ACTOR_NZMI, ACTOR_OKTA,
    ACTOR_FAGN, 'SLR3', ACTOR_HBMB, ACTOR_BMTY, ACTOR_BMTY, ACTOR_HINO, ACTOR_SLR1, ACTOR_SLR1, 'SLR2', ACTOR_SLR4,
    ACTOR_Poon, ACTOR_WIZR, ACTOR_ZASL, 'SLR3', ACTOR_SLR1, ACTOR_ZORA, ACTOR_SLR1, ACTOR_WIZR, ACTOR_PAME, ACTOR_PAME,
    ACTOR_PAME, ACTOR_PAME, ACTOR_PAME, ACTOR_HART, ACTOR_DDGO, ACTOR_KETH,
};

// Spawn parameter (the spawned actor's mSpawnParam_0x90) by the same index.
static const u32 lbl_8046F450[46] = {
    0, 0, 0x100000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 3, 0, 0x102, 0x1000004, 0x1000000, 0x1008800, 0, 5, 0, 4, 0, 0, 6, 4, 3, 5,
    7, 9, 1, 0, 0x12, 3,
};

void EnemyGenerator3_804AA1D0::init() {
    mTypeIndex = mSpawnParam_0x90 & 0x7F;
    mActorId = lbl_8046F398[mTypeIndex];
    mInterval = (mSpawnParam_0x90 >> 7) & 0x1FF;
    mCount = (mSpawnParam_0x90 >> 16) & 0xF;
    mMode = (mSpawnParam_0x90 >> 20) & 0xF;
    mFlag = (mSpawnParam_0x90 >> 24) & 1;
    mRemaining = mCount;
    mTimer = 1;
    mState = 0;
}

static inline void spawnEnemy(EnemyGenerator3_804AA1D0* self) {
    u32 param = lbl_8046F450[self->mTypeIndex];
    getActorManager_801F856C()->spawnActor_801F9484(self->mActorId, self->mLayer_0x4, &self->mPos, param, -1, -1);
}

// The mLayer_0x4 register differs (r29 here, r30 in the original).
void EnemyGenerator3_804AA1D0::update() {
    s32 layer;
    switch (mState) {
    case 0:
        if (isScreenFlagSet_0x34()) {
            mState = 1;
            mTimer = 1;
        }
        break;
    case 1:
        mTimer--;
        if (mTimer == 0) {
            s32 count = 0;
            if (mMode == 1) {
                s32 found = -1;
                while (true) {
                    s32 next = getActorManager_801F856C()->findNextActorById_801F78F4(mActorId, found);
                    found = next;
                    if (next < 0) {
                        break;
                    }
                    count++;
                    if (count >= 30) {
                        mRemaining = mCount;
                        mState = 4;
                        return;
                    }
                }
            }
            layer = mLayer_0x4;
            if (layer >= 8) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, layer, &mPos, 3, -1, -1);
            } else {
                spawnEmitter_8013CC50(layer, &mPos, 0x35c, 1, 1, NULL, NULL, true, 1.0f);
            }
        }
        mState = 2;
        mTimer = 2;
        break;
    case 2:
        mTimer--;
        if (mTimer == 0) {
            spawnEnemy(this);
            if (gAudioMgr_8053AF80->field_0x58 != 0x1000021 && gGameRoot_8053AAF8->field_0x7f == 1 &&
                mFlag == 0) {
                gAudioMgr_8053AF80->startBgm_80458FF0(0x1000021, 1, false);
            }
            mTimer = mInterval;
            mRemaining--;
            if (mRemaining == 0) {
                if (mMode == 0) {
                    markDead_801F35AC();
                } else if (mMode == 1) {
                    mRemaining = mCount;
                    mState = 4;
                }
            } else {
                mState = 1;
            }
        }
        break;
    case 4:
        if (!isScreenFlagSet_0x34()) {
            mState = 0;
        }
        break;
    }
}

void EnemyGenerator3_804AA1D0::draw() {}

EnemyGenerator3_804AA1D0::~EnemyGenerator3_804AA1D0() {}
