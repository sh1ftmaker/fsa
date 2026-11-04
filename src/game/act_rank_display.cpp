#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_rank_display.h"
#include "game/tu_80379F84.h"

// Emitters are released as fn_801EE434 does (the nearest helper, as in
// act_warp_pillar). FIXME: the original clears the emitter's status word
// directly (word 0xF4 |= 1, word 0x24 = 1) and nulls the member itself, inline
// (the destructor unrolled for the four players, update in three places), so
// those sites differ.
static inline void releaseEmitter(JPABaseEmitter*& emitter) {
    if (emitter != NULL) {
        fn_801EE434(emitter);
        emitter = NULL;
    }
}

// Whether player i takes part (the bit of GameRoot's field_0x82).
static inline bool playerTakesPart(s32 i) {
    u8 playerBit = 1 << i;
    return lbl_8053AAF8->field_0x82 & playerBit;
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
        if (fn_802D6A9C()->field_0x131ef != 0) {
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
            mPos = *fn_8023E724(fn_8022EA0C(i));
            {
                void* res = fn_804030C4('TIMG', lbl_804A8F50[field_0x244[i]]);
                JGeometry::TVec3f pos(mPos.x, mPos.y - 28.0f, mPos.z);
                if (field_0x4 < 8) {
                    lbl_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
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
            mPos = *fn_8023E724(fn_8022EA0C(i));
            {
                void* res = fn_804030C4('TIMG', "kk_noItem_01.bti");
                JGeometry::TVec3f pos(mPos.x, mPos.y - 28.0f, mPos.z);
                if (field_0x4 < 8) {
                    lbl_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
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
            mPos = *fn_8023E724(fn_8022EA0C(i));
            {
                void* res = fn_804030C4('TIMG', "kk_noMagic_01.bti");
                JGeometry::TVec3f pos(mPos.x, mPos.y - 28.0f, mPos.z);
                if (field_0x4 < 8) {
                    lbl_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
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
            mPos = *fn_8023E724(fn_8022EA0C(i));
            {
                void* res = fn_804030C4('TIMG', lbl_804A8F60[field_0x28c[i] - 4]);
                JGeometry::TVec3f pos(mPos.x, mPos.y - 28.0f, mPos.z);
                if (field_0x4 < 8) {
                    lbl_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
                }
            }
            break;
        default:
            break;
        }
    }
}

// FIXME: 70.6 %. The three inline emitter releases (lwz 0xF4; ori 1; stw 0xF4;
// stw 1 to 0x24) are bl fn_801EE434 here; the emitter position and scale words
// (0x98-0xB4) are not written (see below), so the original keeps f30 and f31
// live across the fn_8023E724 calls (frame 0x90, ours 0x60); the rank loops
// differ in register numbers.
void RankDisplay_804A8F74::update() {
    JGeometry::TVec3f* player = fn_8023E724(0);
    mPos.x = player->x;
    mPos.y = player->y;
    mPos.z = player->z;
    if (lbl_8053AAF8->field_0x7e == 1) {
        if (lbl_8053AAF8->field_0x7f == 1) {
            // One time per player that takes part in the stage, ranked by
            // fn_80236100 (the lowest time first).
            s32 times[4];
            for (s32 i = 0; i < lbl_8053AAF8->fn_80138A30(); i++) {
                times[i] = fn_80236100(i);
            }
            for (s32 i = 0; i < lbl_8053AAF8->fn_80138A30(); i++) {
                s32 rank = 0;
                for (s32 j = 0; j < lbl_8053AAF8->fn_80138A30(); j++) {
                    if (i != j && times[i] < times[j]) {
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
            // The four players' ranks, unrolled in the original.
            for (s32 k = 0; k < 4; k++) {
                s32 rank = 0;
                if (k != 0 && pl[k] < pl[0]) {
                    rank++;
                }
                if (k != 1 && pl[k] < pl[1]) {
                    rank++;
                }
                if (k != 2 && pl[k] < pl[2]) {
                    rank++;
                }
                if (k != 3 && pl[k] < pl[3]) {
                    rank++;
                }
                field_0x244[k] = rank;
            }
        }
    }

    for (s32 i = 0; i < 4; i++) {
        if (field_0x244[i] != field_0x254[i]) {
            if (field_0x278 != 0 && !fn_8022AF5C()) {
                for (s32 k = 0; k < 4; k++) {
                    if (playerTakesPart(k) != true) {
                        continue;
                    }
                    SaveRow_8050EC80& row = lbl_8050EC80[lbl_8050EC80->field_0x10];
                    if (lbl_8053AAF8->field_0x7f == 1) {
                        if (row.field_0x7a[0x88 - 0x7A] == 0) {
                            continue;
                        }
                    } else if (lbl_8053AAF8->field_0x7f == 0) {
                        if (row.field_0x7a[0x89 - 0x7A] == 0) {
                            continue;
                        }
                    } else {
                        continue;
                    }
                    if (fn_8023DE58(k) >= 8) {
                        continue;
                    }
                    field_0x27c[k] = 180;
                    field_0x28c[k] = 1;
                    if (field_0x244[k] < field_0x254[k]) {
                        releaseEmitter(mEmitters[k]);
                        if (mEmitters[k] == NULL) {
                            JGeometry::TVec3f pos;
                            pos.z = fn_8023E724(k)->z;
                            pos.y = fn_8023E724(k)->y - 28.0f;
                            pos.x = fn_8023E724(k)->x;
                            mEmitters[k] = fn_8013CC50(0, &pos, 0x3A6, 0, 0, NULL, NULL, true, 0.4f);
                        }
                    } else if (field_0x244[k] > field_0x254[k]) {
                        releaseEmitter(mEmitters[k]);
                        if (mEmitters[k] == NULL) {
                            JGeometry::TVec3f pos;
                            pos.z = fn_8023E724(k)->z;
                            pos.y = fn_8023E724(k)->y - 28.0f;
                            pos.x = fn_8023E724(k)->x;
                            mEmitters[k] = fn_8013CC50(0, &pos, 0x3A5, 0, 0, NULL, NULL, true, 0.4f);
                        }
                    }
                }
            }
            field_0x254[i] = field_0x244[i];
        }

        if (fn_8023BE0C(i)) {
            if (fn_8023DE58(i) < 8 && fn_802D6A9C()->fn_802D46D8(i, 0xEB, false) == true) {
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
        if (field_0x27c[k] == 0) {
            field_0x28c[k] = 0;
        } else {
            field_0x27c[k]--;
        }
        if (field_0x28c[k] == 1) {
            if (mEmitters[k] != NULL) {
                // FIXME: the original writes the emitter's position words
                // (0xA4-0xAC, from fn_8023E724(k) as z, y - 28, x) and five
                // 0.4f words at 0x98-0xB4 here. JPABaseEmitter names none of
                // them, so they are left out.
            }
        } else {
            releaseEmitter(mEmitters[k]);
        }
    }
}

void RankDisplay_804A8F74::init() {
    if (lbl_8053AAF8->field_0x7e == 1 && !fn_802DEBF8() && !fn_802DEBA0() && !fn_802DE814()) {
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
        if (lbl_8053AAF8->field_0x82 & playerBit) {
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

// FIXME: 69.3 %. The four emitter releases are inline in the original (word
// 0xF4 |= 1, word 0x24 = 1, member nulled), each a bl fn_801EE434 here.
RankDisplay_804A8F74::~RankDisplay_804A8F74() {
    releaseEmitter(mEmitters[0]);
    releaseEmitter(mEmitters[1]);
    releaseEmitter(mEmitters[2]);
    releaseEmitter(mEmitters[3]);
}
