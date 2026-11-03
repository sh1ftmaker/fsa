#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/ActorManager.h"
#include "game/act/act_enemy_generator_3.h"

// Actor ids indexed by field_0x90 & 0x7F.
static const u32 lbl_8046F398[46] = {
    'STAL', 'KETH', 'SLR1', 'SLR1', 'DDGO', 'GOMA', 'DDRK', 'BZBR', 'SLR2', 'PAME',
    'LPLK', 'MIRA', 'SLR4', 'RIVR', 'GDMN', 'MOZO', 'PEAC', 'ROPE', 'NZMI', 'OKTA',
    'FAGN', 'SLR3', 'HBMB', 'BMTY', 'BMTY', 'HINO', 'SLR1', 'SLR1', 'SLR2', 'SLR4',
    'Poon', 'WIZR', 'ZASL', 'SLR3', 'SLR1', 'ZORA', 'SLR1', 'WIZR', 'PAME', 'PAME',
    'PAME', 'PAME', 'PAME', 'HART', 'DDGO', 'KETH',
};

// Spawn parameter (the spawned actor's field_0x90) by the same index.
static const u32 lbl_8046F450[46] = {
    0, 0, 0x100000, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 3, 0, 0x102, 0x1000004, 0x1000000, 0x1008800, 0, 5, 0, 4, 0, 0, 6, 4, 3, 5,
    7, 9, 1, 0, 0x12, 3,
};

EnemyGenerator3_804AA1D0::~EnemyGenerator3_804AA1D0() {}

void EnemyGenerator3_804AA1D0::draw() {}

// FIXME: 97.8 %. The original's 30-iteration search loop falls straight into
// the "mRemaining = mCount; mState = 4" block when it runs out and branches
// to the spawn code when fn_801F78F4 returns < 0 (no second count test after
// the loop; it compares r3 before copying it to the serial register); ours
// re-tests count after the loop and uses mr. Registers for field_0x4 follow
// (r29 here, r30 in the original).
void EnemyGenerator3_804AA1D0::update() {
    switch (mState) {
    case 0:
        if (vfunc_0x34()) {
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
                do {
                    found = fn_801F856C()->fn_801F78F4(mActorId, found);
                    if (found < 0) {
                        break;
                    }
                    count++;
                } while (count < 30);
                if (count >= 30) {
                    mRemaining = mCount;
                    mState = 4;
                    break;
                }
            }
            s32 layer = field_0x4;
            if (layer >= 8) {
                fn_801F856C()->fn_801F9484('KMRI', layer, &mPos, 3, -1, -1);
            } else {
                fn_8013CC50(layer, &mPos, 0x35c, 1, 1, NULL, NULL, true, 1.0f);
            }
            mState = 2;
            mTimer = 2;
        }
        break;
    case 2:
        mTimer--;
        if (mTimer == 0) {
            u32 param = lbl_8046F450[mTypeIndex];
            fn_801F856C()->fn_801F9484(mActorId, field_0x4, &mPos, param, -1, -1);
            if (lbl_8053AF80->field_0x58 != 0x1000021 && lbl_8053AAF8->field_0x7f == 1 &&
                mFlag == 0) {
                lbl_8053AF80->fn_80458FF0(0x1000021, 1, false);
            }
            mTimer = mInterval;
            mRemaining--;
            if (mRemaining == 0) {
                if (mMode == 0) {
                    fn_801F35AC();
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
        if (!vfunc_0x34()) {
            mState = 0;
        }
        break;
    }
}

void EnemyGenerator3_804AA1D0::init() {
    mTypeIndex = field_0x90 & 0x7F;
    mActorId = lbl_8046F398[mTypeIndex];
    mInterval = (field_0x90 >> 7) & 0x1FF;
    mCount = (field_0x90 >> 16) & 0xF;
    mMode = (field_0x90 >> 20) & 0xF;
    mFlag = (field_0x90 >> 24) & 1;
    mRemaining = mCount;
    mTimer = 1;
    mState = 0;
}
