#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/act_enemy_spawn_area.h"

// Pairs of (actor ID, field_0x90). init indexes it by pair (field_0x90 & 0xFF);
// update's random pick indexes it by word (so it can straddle two pairs).
static u32 lbl_804A2C80[] = {
    'SLR1', 0x00, 'SLR1', 0x06, 'STAL', 0x04, 'STAL', 0x14, 'KETH', 0x100, 'KETH', 0x110,
};

EnemySpawnArea_804A2CB0::~EnemySpawnArea_804A2CB0() {}

void EnemySpawnArea_804A2CB0::draw() {}

// FIXME: 99.6%. Register allocation only: the two random-offset sequences use r4/r5
// swapped for the global pointer and the LCG multiplier, and in the second loop the
// counters i and gone are in r31/r30 here, r30/r31 in the original.
void EnemySpawnArea_804A2CB0::update() {
    switch (mState) {
    case 0:
        if (mTimer0 == 0) {
            for (s32 i = 0; i < mCount; i++) {
                if (mSerials[i] == -1) {
                    u32 id = mEnemyId;
                    u32 param = mEnemyParam;
                    if (mEnemyId == 0) {
                        s32 pick = (s32)(5.99f * lbl_8053AAF8->mRandom.get_ufloat_1());
                        id = lbl_804A2C80[pick];
                        param = lbl_804A2C80[pick + 1];
                    }
                    JGeometry::TVec3f pos;
                    pos.x = mPos.x;
                    pos.y = mPos.y;
                    pos.z = mPos.z;
                    if (i & 1) {
                        pos.x -= 16.0f * i;
                    } else {
                        pos.x += 16.0f * i;
                    }
                    pos.x += 8.0f - 16.0f * lbl_8053AAF8->mRandom.get_ufloat_1();
                    pos.y += 8.0f - 16.0f * lbl_8053AAF8->mRandom.get_ufloat_1();
                    mSerials[i] = getActorManager_801F856C()->spawnActor_801F9484(id, field_0x4, &pos, param, -1, -1);
                    if (mSerials[i] != 0) {
                        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HBMB, field_0x4, &pos, 0xB, -1, -1);
                    }
                }
            }
            mState = 1;
        }
        // fall through
    case 1:
        if (mTimer0 == 0) {
            s32 gone = 0;
            for (s32 i = 0; i < mCount; i++) {
                if (getActorManager_801F856C()->findActorBySerial_801F666C(mSerials[i]) == NULL) {
                    mSerials[i] = -1;
                    if (mMode == 0) {
                        mState = 0;
                        mTimer0 = 0x4B0;
                        return;
                    }
                    gone++;
                }
            }
            mTimer0 = 8;
            if (gone == mCount && mMode == 1) {
                mTimer0 = 0x4B0;
                mState = 0;
            }
        }
        break;
    }
    if (mTimer0 > 0) {
        mTimer0--;
    }
    if (mTimer1 > 0) {
        mTimer1--;
    }
    if (mTimer2 > 0) {
        mTimer2--;
    }
}

// FIXME: 92%. The original loads field_0x90 once (clrlslwi 24,1 -> r5) before the
// 0x234 store and keeps the zero in r4 for the timer stores; here the zero is in r0
// and the index shift is scheduled after the second load of field_0x90.
void EnemySpawnArea_804A2CB0::init() {
    field_0x28c = 0;
    field_0x290 = 0;
    field_0x294 = 0;
    mTimer0 = 0;
    mTimer1 = 0;
    mTimer2 = 0;
    mEnemyId = 0;
    mEnemyParam = 0;
    s32 idx = (field_0x90 & 0xFF) * 2;
    if ((field_0x90 & 0xFF) != 0xFF) {
        mEnemyId = lbl_804A2C80[idx];
        mEnemyParam = lbl_804A2C80[idx + 1];
    }
    if (((field_0x90 >> 8) & 0xF) == 0) {
        mMode = 0;
    } else {
        mMode = 1;
    }
    mCount = (field_0x90 >> 12) & 0xF;
    if (mCount == 0) {
        mCount = 3;
    }
    // Written out: a for loop is unrolled too but leaves a dead cmpwi 15/bge guard (88.4% vs 92.2%).
    mSerials[0] = -1;
    mSerials[1] = -1;
    mSerials[2] = -1;
    mSerials[3] = -1;
    mSerials[4] = -1;
    mSerials[5] = -1;
    mSerials[6] = -1;
    mSerials[7] = -1;
    mSerials[8] = -1;
    mSerials[9] = -1;
    mSerials[10] = -1;
    mSerials[11] = -1;
    mSerials[12] = -1;
    mSerials[13] = -1;
    mSerials[14] = -1;
    mState = 0;
}
