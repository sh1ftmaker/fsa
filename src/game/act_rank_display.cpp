#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_rank_display.h"
#include "game/tu_80379F84.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Stops an emitter (it runs out) and forgets it.
static inline void invalidateEmitter(JPABaseEmitter*& emitter) {
    if (emitter != NULL) {
        emitter->becomeInvalidEmitter();
        emitter = NULL;
    }
}

// Whether player i takes part (the bit of GameRoot's field_0x82).
static inline bool playerTakesPart(s32 i) {
    u8 playerBit = 1 << i;
    return gGameRoot_8053AAF8->field_0x82 & playerBit;
}

// The rank colours: red, green and blue, each indexed by field_0x244.
static const u8 lbl_80540D88[4] = {0xB9, 0xF2, 0xFF, 0xD8};
static const u8 lbl_80540D8C[4] = {0x12, 0x48, 0xFF, 0xFF};
static const u8 lbl_80540D90[4] = {0x12, 0x09, 0x77, 0xA1};

// The rank pictures (field_0x244 picks one) and the heart pictures (field_0x28c
// 4 to 8 picks one).
static const char* lbl_804A8F50[] = {
    "kk_mainRank_01.bti", "kk_mainRank_02.bti", "kk_mainRank_03.bti", "kk_mainRank_04.bti",
};
static const char* lbl_804A8F60[] = {
    "kk_heart_04_2.bti", "kk_heart_03.bti", "kk_heart_02.bti", "kk_heart_01.bti", "kk_heart_00.bti",
};

void RankDisplay_804A8F74::draw() {
    for (s32 i = 0; i < 4; i++) {
        if (getGameView_802D6A9C()->field_0x131ef != 0) {
            continue;
        }
        if (field_0x27c[i] == 0) {
            continue;
        }
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        if (playerTakesPart(i) != true) {
            continue;
        }
        switch (field_0x28c[i]) {
        case 1:
            mtx[0][3] = 0.0f;
            mtx[1][3] = 0.0f;
            mtx[2][3] = 0.0f;
            params.field_0x1c = 1;
            params.field_0x1d = 1;
            params.field_0x2d = 1;
            params.field_0x10.mColor.r = lbl_80540D88[field_0x244[i]];
            params.field_0x10.mColor.g = lbl_80540D8C[field_0x244[i]];
            params.field_0x10.mColor.b = lbl_80540D90[field_0x244[i]];
            params.field_0x10.mColor.a = field_0x274[i];
            mtx[0][0] = 0.4f;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 0.4f;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = 0.4f;
            params.mMtx = mtx;
            mPos = *getPlayerPos_8023E724(fn_8022EA0C(i));
            {
                void* res = fn_804030C4('TIMG', lbl_804A8F50[field_0x244[i]]);
                JGeometry::TVec3f pos(mPos.x, mPos.y - 28.0f, mPos.z);
                if (field_0x4 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
                }
            }
            break;
        case 2:
            mtx[0][3] = 0.0f;
            mtx[1][3] = 0.0f;
            mtx[2][3] = 0.0f;
            params.field_0x1c = 1;
            params.field_0x1d = 1;
            params.field_0x2d = 1;
            params.field_0x10.mColor.r = 0xF2;
            params.field_0x10.mColor.g = 0x49;
            params.field_0x10.mColor.b = 0x12;
            params.field_0x10.mColor.a = field_0x274[i];
            mtx[0][0] = 0.4f;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 0.4f;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = 0.4f;
            params.mMtx = mtx;
            mPos = *getPlayerPos_8023E724(fn_8022EA0C(i));
            {
                void* res = fn_804030C4('TIMG', "kk_noItem_01.bti");
                JGeometry::TVec3f pos(mPos.x, mPos.y - 28.0f, mPos.z);
                if (field_0x4 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
                }
            }
            break;
        case 3:
            mtx[0][3] = 0.0f;
            mtx[1][3] = 0.0f;
            mtx[2][3] = 0.0f;
            params.field_0x1c = 1;
            params.field_0x1d = 1;
            params.field_0x2d = 1;
            params.field_0x10.mColor.r = 0xF2;
            params.field_0x10.mColor.g = 0x49;
            params.field_0x10.mColor.b = 0x12;
            params.field_0x10.mColor.a = field_0x274[i];
            mtx[0][0] = 0.4f;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 0.4f;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = 0.4f;
            params.mMtx = mtx;
            mPos = *getPlayerPos_8023E724(fn_8022EA0C(i));
            {
                void* res = fn_804030C4('TIMG', "kk_noMagic_01.bti");
                JGeometry::TVec3f pos(mPos.x, mPos.y - 28.0f, mPos.z);
                if (field_0x4 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
                }
            }
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            mtx[0][3] = 0.0f;
            mtx[1][3] = 0.0f;
            mtx[2][3] = 0.0f;
            params.field_0x1c = 1;
            params.field_0x1d = 1;
            params.field_0x2d = 1;
            params.field_0x10.mColor.r = 0xFF;
            params.field_0x10.mColor.g = 0xFF;
            params.field_0x10.mColor.b = 0xFF;
            params.field_0x10.mColor.a = field_0x274[i];
            mtx[0][0] = 0.5f;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 0.5f;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = 0.5f;
            params.mMtx = mtx;
            mPos = *getPlayerPos_8023E724(fn_8022EA0C(i));
            {
                void* res = fn_804030C4('TIMG', lbl_804A8F60[field_0x28c[i] - 4]);
                JGeometry::TVec3f pos(mPos.x, mPos.y - 28.0f, mPos.z);
                if (field_0x4 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
                }
            }
            break;
        default:
            break;
        }
    }
}

// FIXME: 99.2 %. Register allocation only: the loop counters and the read-once
// rank values sit in other callee-saved registers (r23/r24/r31/r29 swapped),
// the zero stored for absent players comes from the counter register in the
// original (mr r27, r31), and the rank counter is cleared one slot earlier.
void RankDisplay_804A8F74::update() {
    JGeometry::TVec3f* player = getPlayerPos_8023E724(0);
    mPos.x = player->x;
    mPos.y = player->y;
    mPos.z = player->z;
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            // One time per player that takes part in the stage, ranked by
            // fn_80236100 (the lowest time first).
            s32 times[4];
            for (s32 i = 0; i < gGameRoot_8053AAF8->fn_80138A30(); i++) {
                times[i] = fn_80236100(i);
            }
            for (s32 i = 0; i < gGameRoot_8053AAF8->fn_80138A30(); i++) {
                s32 rank = 0;
                s32 time = times[i];
                for (s32 j = 0; j < gGameRoot_8053AAF8->fn_80138A30(); j++) {
                    if (i != j && time < times[j]) {
                        rank++;
                    }
                }
                field_0x244[i] = rank;
            }
        } else {
            s32 pl[4];
            for (s32 i = 0; i < 4; i++) {
                if (playerTakesPart(i) == true) {
                    pl[i] = fn_80237774(i);
                } else {
                    pl[i] = 0;
                }
            }
            for (s32 k = 0; k < 4; k++) {
                s32 rank = 0;
                s32 value = pl[k];
                for (s32 j = 0; j < 4; j++) {
                    if (k != j && value < pl[j]) {
                        rank++;
                    }
                }
                field_0x244[k] = rank;
            }
        }
    }

    for (s32 i = 0; i < 4; i++) {
        if (field_0x244[i] != field_0x254[i]) {
            if (field_0x278 != 0 && !fn_8022AF5C()) {
                for (s32 k = 0; k < 4; k++) {
                    if (playerTakesPart(k) == false) {
                        continue;
                    }
                    u8 mode = gGameRoot_8053AAF8->field_0x7f;
                    if (!((mode == 1 && lbl_8050EC80[lbl_8050EC80->field_0x10].field_0x88 != 0) ||
                          (mode == 0 && lbl_8050EC80[lbl_8050EC80->field_0x10].field_0x89 != 0))) {
                        continue;
                    }
                    if (getPlayerLayer_8023DE58(k) >= 8) {
                        continue;
                    }
                    field_0x27c[k] = 180;
                    field_0x28c[k] = 1;
                    if (field_0x244[k] < field_0x254[k]) {
                        invalidateEmitter(mEmitters[k]);
                        if (mEmitters[k] == NULL) {
                            JGeometry::TVec3f pos(getPlayerPos_8023E724(k)->x, getPlayerPos_8023E724(k)->y - 28.0f, getPlayerPos_8023E724(k)->z);
                            mEmitters[k] = spawnEmitter_8013CC50(0, &pos, 0x3A6, 0, 0, NULL, NULL, true, 0.4f);
                        }
                    } else if (field_0x244[k] > field_0x254[k]) {
                        invalidateEmitter(mEmitters[k]);
                        if (mEmitters[k] == NULL) {
                            JGeometry::TVec3f pos(getPlayerPos_8023E724(k)->x, getPlayerPos_8023E724(k)->y - 28.0f, getPlayerPos_8023E724(k)->z);
                            mEmitters[k] = spawnEmitter_8013CC50(0, &pos, 0x3A5, 0, 0, NULL, NULL, true, 0.4f);
                        }
                    }
                }
            }
            field_0x254[i] = field_0x244[i];
        }

        if (fn_8023BE0C(i)) {
            if (getPlayerLayer_8023DE58(i) < 8 && getGameView_802D6A9C()->fn_802D46D8(i, 0xEB, false) == true) {
                field_0x27c[i] = 30;
                field_0x28c[i] = 2;
            }
        }
    }

    for (s32 k = 0; k < 4; k++) {
        field_0x274[k] = 0xFF;
        if (fn_8022D75C(k)) {
            field_0x274[k] = fn_8023BCD4(k);
        }
        if (field_0x27c[k] != 0) {
            field_0x27c[k]--;
        } else {
            field_0x28c[k] = 0;
        }
        if (field_0x28c[k] == 1) {
            if (mEmitters[k] != NULL) {
                mEmitters[k]->setGlobalTranslation(getPlayerPos_8023E724(k)->x, getPlayerPos_8023E724(k)->y - 28.0f, getPlayerPos_8023E724(k)->z);
                mEmitters[k]->setScale(0.4f);
            }
        } else {
            invalidateEmitter(mEmitters[k]);
        }
    }
}

void RankDisplay_804A8F74::init() {
    if (gGameRoot_8053AAF8->field_0x7e == 1 && !fn_802DEBF8() && !fn_802DEBA0() && !fn_802DE814()) {
        field_0x278 = 1;
    } else {
        field_0x278 = 0;
    }

    for (s32 i = 0; i < 4; i++) {
        mEmitters[i] = NULL;
        u8 playerBit = 1 << i;
        field_0x244[i] = 0;
        field_0x254[i] = 0;
        field_0x274[i] = 0xFF;
        if (gGameRoot_8053AAF8->field_0x82 & playerBit) {
            field_0x264[i] = fn_80237774(i);
        } else {
            field_0x264[i] = 0;
        }
    }

    for (s32 i = 0; i < 4; i++) {
        field_0x27c[i] = 0;
        field_0x28c[i] = 0;
    }
    field_0x11f = 1;
}

RankDisplay_804A8F74::~RankDisplay_804A8F74() {
    for (s32 i = 0; i < 4; i++) {
        if (mEmitters[i] != NULL) {
            mEmitters[i]->becomeInvalidEmitter();
            mEmitters[i] = NULL;
        }
    }
}
