#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/Player.h"
#include "game/act/tu_802E9428.h"
#include "game/act/tu_80362634.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_kaepora_gaebora_ending.h"


// One step of the sequence: a negative value ends the sequence and restarts it.
struct KaeporaStep {
    s32 mValue;
    s32 mCount;
};

// FIXME: the original object is 0x4C bytes; its last 0x14 bytes (after the
// terminator) are not written yet, as no code here reads them.
static const KaeporaStep lbl_804742A8[] = {
    {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
    {-1, -1},
};

// Facing offsets of the sprite, indexed by field_0x274 (fn_80361424).
static const u16 lbl_805429F8[4] = {0, 0x8000, 0xC000, 0x4000};

// The update() steps, indexed by mPtmfIndex; built at static init time.
void (KaeporaGaeboraEnding_804ADAC8::*lbl_804ADA50[6])() = {
    &KaeporaGaeboraEnding_804ADAC8::fn_8036003C, &KaeporaGaeboraEnding_804ADAC8::fn_8035FF70, &KaeporaGaeboraEnding_804ADAC8::fn_8035FD6C,
    &KaeporaGaeboraEnding_804ADAC8::fn_8035FCD8, &KaeporaGaeboraEnding_804ADAC8::fn_8035FB64, &KaeporaGaeboraEnding_804ADAC8::fn_8035F9C8,
};

// Per-kind tables of this unit's data and rodata, indexed by field_0x250 or
// field_0x270 (init).
// FIXME(shotgun): these live in this unit; their contents are not written yet.
extern const u8 lbl_80474290[];          // 1: spawn an EOWL partner
extern const KaeporaWaypoint lbl_80473FD8[];
extern const s32 lbl_80474008[];         // sprite frame base per field_0x270 - 1
extern const f32* const lbl_804AD900[];  // field_0x284 (flag 0x100 set)
extern const f32* const lbl_804AD958[];  // field_0x284
extern const KaeporaWaypoint* const lbl_804AD9B0[];  // field_0x298

// FIXME(shotgun): 84.2 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::init() {
    field_0x3c.z = field_0x3c.y = field_0x3c.x = 0.0f;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23C = 0;
    mStep = 0;
    field_0x248 = 0;
    field_0x244 = 0;
    field_0x254 = 0;
    field_0x25C = field_0x258 = 0.0f;
    field_0x264 = field_0x260 = 0;
    field_0x268 = 0.0f;
    field_0x26C = 0.0f;
    field_0x270 = field_0x90 & 7;
    field_0x274 = 0;
    field_0x250 = (field_0x90 >> 3) & 0x1F;
    field_0x2B0 = 1;
    field_0x280 = 0;
    field_0x290 = field_0x28C = field_0x288 = 0.0f;
    field_0x294 = 0;
    field_0x2B4 = NULL;
    field_0x2B8 = NULL;
    field_0x29C = NULL;
    if (!(field_0x90 & 0x100)) {
        if (lbl_80474290[field_0x250] == 1) {
            field_0x2B0 = 0;
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_EOWL, field_0x4, &mPos, field_0x90 | 0x100, -1, -1);
            field_0x298 = lbl_804AD9B0[field_0x250];
            field_0x284 = lbl_804AD958[field_0x250];
        } else {
            field_0x298 = lbl_804AD9B0[field_0x250];
            field_0x284 = lbl_804AD958[field_0x250];
        }
    } else {
        field_0x298 = lbl_80473FD8;
        field_0x284 = lbl_804AD900[field_0x250];
    }
    mHeight = 1;
    mDepth = 1;
    if (field_0x270 == 0) {
        fn_80360914();
    } else {
        field_0x238 = lbl_80474008[field_0x270 - 1];
        fn_80360914();
        if (field_0x90 & 0x100) {
            field_0x2B4 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32C, mHeight, mDepth, NULL, NULL, true, 1.0f);
            field_0x2B8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x171, mHeight, mDepth, NULL, NULL, true, 1.0f);
            if (field_0x2B4 != NULL) {
                // FIXME(shotgun): not written: copies the 3-byte colour
                // rodata 0x804742E0[field_0x270 - 1] to emitter +0xBC..0xBE
                // (no such field declared in JPABaseEmitter yet).
            }
        }
    }
    if (!(field_0x90 & 0x100)) {
        if (field_0x250 == 0x12) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_ELK2, field_0x4, &mPos, 0, -1, -1);
            static JGeometry::TVec3f offsets[4] = {
                JGeometry::TVec3f(-18.0f, -126.0f, 0.0f),
                JGeometry::TVec3f(18.0f, -126.0f, 0.0f),
                JGeometry::TVec3f(-26.0f, -156.0f, 0.0f),
                JGeometry::TVec3f(26.0f, -156.0f, 0.0f),
            };
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = mPos + offsets[i];
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_EETC, field_0x4, &pos,
                                                                (((i + 0x26) << 5) | 4) | 0x80000000, -1, -1);
            }
        } else if (field_0x250 == 0x13) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_EETC, field_0x4, &mPos, 0x10, -1, -1);
            field_0x230 = 1;
            JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
            fn_80230498(0, &zero, field_0x238 + field_0x23C, 0xFF, -0x80, -0x80, true);
        }
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == ACTOR_ENDM && actor->mAlive == true) {
                field_0x29C = (UnknownENDM_804AA0FC*)actor;
                break;
            }
        }
    }
}

void KaeporaGaeboraEnding_804ADAC8::update() {
    if (field_0x280 != 0) {
        field_0x280--;
    }
    if (field_0x234 != 0) {
        field_0x234--;
    }
    while (fn_8035F3C0()) {
    }
    (this->*lbl_804ADA50[mPtmfIndex])();
    if (field_0x2B0 == 0) {
        if (field_0x2B4 != 0) {
            setEmitterPos_8013CB44(field_0x2B4, &mPos, -2, -2, true);
        }
        if (field_0x2B8 != 0) {
            setEmitterPos_8013CB44(field_0x2B8, &mPos, -2, -2, true);
        }
    } else {
        if (field_0x2B4 != 0) {
            setEmitterPos_8013CB44(field_0x2B4, &mPos, mHeight, mDepth, true);
        }
        if (field_0x2B8 != 0) {
            setEmitterPos_8013CB44(field_0x2B8, &mPos, mHeight, mDepth, true);
        }
    }
}

// FIXME(shotgun): 94.2 %, different
void KaeporaGaeboraEnding_804ADAC8::draw() {
    if (field_0x2B0 != 0) {
        if (field_0x270 == 0) {
            if (mPtmfIndex == 1) {
                fn_803616FC();
            } else if (mPtmfIndex == 0) {
                fn_80361424();
            } else {
                SpriteParams_801F5AE0 params;
                params.field_0x1f = 0;
                drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23C + 0x25C, &params, -1, 1.0f, 1.0f);
                params.field_0x10.a = 200;
                if (mSpriteParams.mMtx == NULL) {
                    fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", field_0x23C + 0x25C, NULL);
                }
            }
        } else {
            SpriteParams_801F5AE0 params;
            params.field_0x1f = 0;
            params.field_0x1c = mHeight;
            params.field_0x1d = mDepth;
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x238 + field_0x23C, &params, -1, 1.0f, 1.0f);
            if (mHeight < 0) {
                params.field_0x1c = 0;
            }
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_both2_sob.bin", field_0x238 + field_0x23C, NULL);
            }
        }
    }
}

// FIXME(shotgun): 90.7 %, scheduling+regalloc
void KaeporaGaeboraEnding_804ADAC8::fn_803616FC() {
    SpriteParams_801F5AE0 params;
    params.field_0x1f = 0;
    JGeometry::TVec3f savedPos = mPos;
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x279, &params, -1, 1.0f, 1.0f);
    params.field_0x10.a = 200;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", 0x279, NULL);
    }
    params.reset();
    params.field_0x1f = 0;
    params.field_0x1d = 1;
    Mtx mtx;
    f32 s = JMath::sincosTable_.sinShort(mSway.mAngle);
    f32 c = JMath::sincosTable_.cosShort(mSway.mAngle);
    mtx[0][0] = c;
    mtx[0][1] = 0.0f;
    mtx[0][2] = s;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = -s;
    mtx[2][1] = 0.0f;
    mtx[2][2] = c;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;
    mPos.y += -21.0f;
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x27A, &params, -1, 1.0f, 1.0f);
    mPos.y -= -21.0f;
    params.field_0x10.a = 200;
    mPos.y += -10.5f;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", 0x27A, NULL);
    }
    mPos.y -= -10.5f;
    mPos = savedPos;
}

// FIXME(shotgun): 96.3 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::fn_80361424() {
    SpriteParams_801F5AE0 params;
    bool shadow = false;
    if (field_0x250 == 1) {
        if (mPos.y >= 580.0f && mPos.y < 740.0f) {
            params.field_0x1c = 0;
            shadow = false;
            params.field_0x1d = 1;
        } else if (mPos.y >= 740.0f && mPos.y < 952.0f) {
            params.field_0x1c = 1;
            shadow = true;
            params.field_0x1d = 2;
        } else {
            params.field_0x1c = 1;
            shadow = false;
            params.field_0x1d = 2;
        }
    } else if (field_0x250 == 0x10) {
        params.field_0x1d = 2;
    } else if (field_0x250 == 0) {
        params.field_0x1c = 1;
        params.field_0x1d = 2;
    }
    params.field_0x1f = 0;
    Mtx mtx;
    s16 angle = field_0x254 - lbl_805429F8[field_0x274];
    f32 s = JMath::sincosTable_.sinShort(angle);
    f32 c = JMath::sincosTable_.cosShort(angle);
    mtx[0][0] = c;
    mtx[0][1] = 0.0f;
    mtx[0][2] = s;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = -s;
    mtx[2][1] = 0.0f;
    mtx[2][2] = c;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;
    if (!shadow) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23C + 0x3E0, &params, 7, 1.0f, 1.0f);
    } else {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23C + 0x3E0, &params, -1, 1.0f, 1.0f);
        params.field_0x1c = 0;
        params.field_0x1d = -1;
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", field_0x23C + 0x3E0, NULL);
        }
    }
}

void KaeporaGaeboraEnding_804ADAC8::fn_803613D0() {
    if (field_0x2B4 != NULL) {
        field_0x2B4->becomeInvalidEmitter();
        field_0x2B4 = NULL;
    }
    if (field_0x2B8 != NULL) {
        field_0x2B8->becomeInvalidEmitter();
        field_0x2B8 = NULL;
    }
}


// The step tables (lbl_804ADA98, 8 of them) are in this unit's data.
extern const KaeporaStep* lbl_804ADA98[8];

// Counts the current step down and loads the next one from the table
// lbl_804ADA98[field_0x244]; true at the table's terminator.
inline bool KaeporaGaeboraEnding_804ADAC8::advanceStep() {
    s32 idx = field_0x244;
    if (field_0x234 == 0) {
        const KaeporaStep* table = lbl_804ADA98[idx];
        if (table[mStep].mValue < 0) {
            return true;
        }
        field_0x234 = table[mStep].mCount;
        field_0x23C = table[mStep].mValue;
        mStep++;
    }
    return false;
}

// The head directions (lbl_80474020, 4 of them) are in this unit's data.
extern const s32 lbl_80474020[4];

// The facing (0-3) of the velocity field_0x3c (fn_80360914, fn_8036003C).
inline void KaeporaGaeboraEnding_804ADAC8::updateFacing() {
    u16 a = (s32)(JMath::atanTable_.atan2_(field_0x3c.y, field_0x3c.x) * 10430.378f);
    if (a >= 0x2000 && a < 0x6000) {
        field_0x274 = 3;
    } else if (a >= 0x6000 && a < 0xa000) {
        field_0x274 = 1;
    } else if (a >= 0xa000 && a < 0xe000) {
        field_0x274 = 2;
    } else {
        field_0x274 = 0;
    }
}

// Picks the flap step table for the new facing (fn_80360914, fn_8036003C).
inline void KaeporaGaeboraEnding_804ADAC8::pickStepTable() {
    if ((field_0x248 & 1) == 1 || fn_801CD664(100) < 5 ||
        (field_0x250 == 0 && field_0x27C != 0 && field_0x27C != 1 && field_0x26C < 100.0f)) {
        field_0x244++;
        if (!(field_0x248 & 1)) {
            field_0x234 = 0;
            mStep = 0;
            advanceStep();
            mStep = 4;
            field_0x234 = 0;
            advanceStep();
            mStep = 0;
            field_0x234 = 6;
        } else {
            const KaeporaStep* table = lbl_804ADA98[field_0x244];
            if (mStep == 0) {
                field_0x23C = table[4].mValue;
            } else {
                field_0x23C = table[mStep - 1].mValue;
            }
        }
    } else {
        field_0x234 = 0;
        mStep = 0;
        advanceStep();
    }
}

// FIXME(shotgun): 0 % in objdiff (689 vs 687 instrs: our shared tail of commands 0-5 sits after the switch, the original has it right after case 0), missing/extra
// Reads the next command of the script field_0x284 and starts it.
void KaeporaGaeboraEnding_804ADAC8::fn_80360914() {
    field_0x26C = 0.0f;
    field_0x264 = 0;
    field_0x25C = 0.0f;
    if (field_0x270 != 0 && field_0x27C == 6 && field_0x2B8 != NULL) {
        field_0x2B8->becomeInvalidEmitter();
        field_0x2B8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x171, mHeight, mDepth, NULL, NULL, true, 1.0f);
    }
    field_0x27C = (s32)*field_0x284++;
    switch (field_0x27C) {
    case 0:
        mPtmfIndex = 0;
        field_0x268 = *field_0x284++;
        field_0x288 = *field_0x284++;
        if (-1000.0f == field_0x288) {
            field_0x288 = mPos.x;
        }
        field_0x28C = *field_0x284++;
        if (-1000.0f == field_0x28C) {
            field_0x28C = mPos.y;
        }
        field_0x290 = *field_0x284++;
        field_0x254 = (s32)(JMath::atanTable_.atan2_(field_0x28C - mPos.y, field_0x288 - mPos.x) * 10430.378f);
        break;
    case 1:
        mPtmfIndex = 0;
        field_0x268 = *field_0x284++;
        field_0x258 = *field_0x284++;
        break;
    case 2:
    case 4:
        mPtmfIndex = 0;
        field_0x268 = *field_0x284++;
        field_0x260 = (s32)*field_0x284++;
        if (field_0x27C == 2) {
            u16 a = field_0x254;
            if (a <= (u16)field_0x260) {
                field_0x260 = field_0x260 - a;
            } else {
                field_0x260 = 0x10000 - a + field_0x260;
            }
        } else {
            u16 a = field_0x254;
            if (a >= (u16)field_0x260) {
                field_0x260 = a - field_0x260;
            } else {
                field_0x260 = 0x10000 - field_0x260 + a;
            }
        }
        field_0x26C = *field_0x284++;
        break;
    case 3:
    case 5:
        mPtmfIndex = 0;
        field_0x268 = *field_0x284++;
        field_0x260 = (s32)*field_0x284++;
        field_0x26C = *field_0x284++;
        break;
    case 6:
        mPtmfIndex = 1;
        field_0x280 = (s32)*field_0x284++;
        if (field_0x270 == 0) {
            field_0x234 = 0;
            mStep = 0;
            mSway.fn_8021ABC0();
        } else if (field_0x2B8 != NULL) {
            field_0x2B8->becomeInvalidEmitter();
            field_0x2B8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, mHeight, mDepth, NULL, NULL, true, 1.0f);
        }
        field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, 0.0f);
        field_0x254 = 0;
        return;
    case 7:
        if (field_0x270 != 0) {
            fn_803613D0();
            mAlive = false;
        } else {
            mPtmfIndex = 3;
            field_0x244 = fn_801CD664(2) + 8;
            field_0x234 = 0;
            mStep = 0;
            advanceStep();
        }
        return;
    case 8:
        if (field_0x270 != 0) {
            fn_803613D0();
            mAlive = false;
        } else {
            mPtmfIndex = 2;
            field_0x268 = *field_0x284++;
            field_0x288 = *field_0x284++;
            field_0x28C = *field_0x284++;
            field_0x290 = *field_0x284++;
            field_0x244 = fn_801CD664(2) + 10;
            field_0x234 = 0;
            mStep = 0;
            advanceStep();
        }
        return;
    case 9:
        if (field_0x270 == 0) {
            fn_803613D0();
            mAlive = false;
        } else {
            mPtmfIndex = 4;
            field_0x26C = 3.0f;
        }
        return;
    case 10:
        if (field_0x270 == 0) {
            fn_803613D0();
            mAlive = false;
        } else {
            mPtmfIndex = 5;
            field_0x26C = 15.0f;
        }
        return;
    case 11:
        fn_803613D0();
        mAlive = false;
        return;
    case 12:
        mPos.x = *field_0x284++;
        mPos.y = *field_0x284++;
        mPos.z = *field_0x284++;
        fn_80360914();
        return;
    case 13:
        field_0x294 = 1;
        fn_80360914();
        return;
    case 14:
        field_0x2B0 = 0;
        fn_80360914();
        return;
    case 15:
        field_0x2B0 = 1;
        fn_80360914();
        return;
    default:
        return;
    }
    field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
    field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
    field_0x248 = field_0x244;
    if (field_0x270 == 0) {
        updateFacing();
        field_0x244 = lbl_80474020[field_0x274];
        if (field_0x2B0 == 1) {
            pickStepTable();
        }
    }
}

// FIXME(shotgun): 96.6 %, missing/extra
// update() step 0: flies along the current command (fn_80360914).
void KaeporaGaeboraEnding_804ADAC8::fn_8036003C() {
    s32 facing = field_0x274;
    field_0x248 = field_0x244;
    if (field_0x270 == 0) {
        if (advanceStep() == true && field_0x2B0 == 1) {
            field_0x244 = lbl_80474020[field_0x274];
            if (fn_801CD664(100) < 5 ||
                (field_0x250 == 0 && field_0x27C != 0 && field_0x27C != 1 && field_0x26C < 100.0f)) {
                field_0x244++;
            }
            field_0x234 = 0;
            mStep = 0;
            advanceStep();
            if (!(field_0x248 & 1) && (field_0x244 & 1)) {
                mStep = 4;
                field_0x234 = 0;
                advanceStep();
                mStep = 0;
                field_0x234 = 6;
            }
        }
    } else {
        if (field_0x250 == 9 && mPos.y >= 2304.0f) {
            mHeight = -1;
            mDepth = 0;
        } else {
            mHeight = 1;
            mDepth = 1;
        }
        if (field_0x234 == 0) {
            if (lbl_804742A8[mStep].mValue < 0) {
                mStep = 0;
            }
            field_0x234 = lbl_804742A8[mStep].mCount;
            field_0x23C = lbl_804742A8[mStep].mValue;
            mStep++;
        }
    }
    if (field_0x27C == 0) {
        mPos += field_0x3c;
        if (fabs(field_0x288 - mPos.x) < field_0x268 && fabs(field_0x28C - mPos.y) < field_0x268) {
            mPos.x = field_0x288;
            mPos.y = field_0x28C;
            mPos.z = field_0x290;
            fn_80360914();
            return;
        }
    } else if (field_0x27C == 1) {
        mPos += field_0x3c;
        field_0x25C += PSVECMag(&field_0x3c);
        if (field_0x25C >= field_0x258) {
            field_0x25C = 0.0f;
            fn_80360914();
            return;
        }
    } else if (field_0x27C == 2 || field_0x27C == 4) {
        s32 step = (s32)(65536.0f * (field_0x268 / (6.283184f * field_0x26C)));
        JGeometry::TVec3f savedPos = mPos;
        if (field_0x27C == 2) {
            field_0x254 += step;
        } else {
            field_0x254 -= step;
        }
        field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
        field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
        mPos += field_0x3c;
        field_0x264 += (s16)step;
        if (field_0x264 >= field_0x260) {
            s16 over = field_0x264 - field_0x260;
            if (field_0x27C == 2) {
                over = field_0x260 - field_0x264;
            }
            if (over != 0) {
                mPos = savedPos;
                field_0x254 += over;
                field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
                field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
                mPos += field_0x3c;
            }
            fn_80360914();
            return;
        }
    } else {
        s32 step = (s32)(65536.0f * (field_0x268 / (6.283184f * field_0x26C)));
        if (field_0x27C == 3) {
            field_0x254 += step;
        } else {
            field_0x254 -= step;
        }
        field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
        field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
        mPos += field_0x3c;
        field_0x264 += (s16)step;
        if (field_0x264 >= field_0x260) {
            fn_80360914();
            return;
        }
    }
    updateFacing();
    if (field_0x270 == 0 && field_0x2B0 == 1 && field_0x274 != facing) {
        field_0x244 = lbl_80474020[field_0x274];
        pickStepTable();
    }
}

void KaeporaGaeboraEnding_804ADAC8::fn_8035FF70() {
    if (field_0x2B0 == 1) {
        if (field_0x270 == 0) {
            mSway.fn_8021A570(true);
        } else if (field_0x234 == 0) {
            if (lbl_804742A8[mStep].mValue < 0) {
                mStep = 0;
            }
            field_0x234 = lbl_804742A8[mStep].mCount;
            field_0x23C = lbl_804742A8[mStep].mValue;
            mStep++;
        }
    }
    if (field_0x280 == 0) {
        fn_80360914();
    }
}

// FIXME(shotgun): 98.4 %, scheduling+regalloc
void KaeporaGaeboraEnding_804ADAC8::fn_8035FD6C() {
    if (advanceStep() == true) {
        mPtmfIndex = 0;
        field_0x27C = 0;
        field_0x254 = (s32)(JMath::atanTable_.atan2_(field_0x28C - mPos.y, field_0x288 - mPos.x) * 10430.378f);
        field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
        field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
        u16 a = field_0x254;
        if (a >= 0x2000 && a < 0x6000) {
            field_0x274 = 3;
        } else if (a >= 0x6000 && a < 0xa000) {
            field_0x274 = 1;
        } else if (a >= 0xa000 && a < 0xe000) {
            field_0x274 = 2;
        } else {
            field_0x274 = 0;
        }
        field_0x244 = lbl_80474020[field_0x274];
        field_0x234 = 0;
        mStep = 0;
        advanceStep();
    }
}

void KaeporaGaeboraEnding_804ADAC8::fn_8035FCD8() {
    if (advanceStep() == true) {
        fn_80360914();
    }
}

// FIXME(shotgun): 97.9 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::fn_8035FB64() {
    if (field_0x234 == 0) {
        if (lbl_804742A8[mStep].mValue < 0) {
            mStep = 0;
        }
        field_0x234 = lbl_804742A8[mStep].mCount;
        field_0x23C = lbl_804742A8[mStep].mValue;
        mStep++;
    }
    if (field_0x26C < 16.0f) {
        field_0x26C += 0.2f;
    }
    s16 prev = field_0x254;
    s32 step = (s32)(65536.0f * (1.75f / (6.283184f * field_0x26C)));
    field_0x254 = prev - step;
    field_0x3c.x = 1.75f * JMath::sincosTable_.cosShort(field_0x254);
    field_0x3c.y = 1.75f * JMath::sincosTable_.sinShort(field_0x254);
    field_0x3c.z = 0.2f;
    mPos += field_0x3c;
    field_0x264 += (s16)step;
    if (field_0x26C >= 16.0f) {
        fn_80360914();
        field_0x3c.z = 0.0f;
    }
}


// Runs the current waypoint's action and moves to the next one.
inline bool KaeporaGaeboraEnding_804ADAC8::fireWaypoint() {
    const KaeporaWaypoint* wp = field_0x298;
    if (wp->mKind < 0) {
        if (wp->mKind == -1) {
            field_0x29C->fn_802E948C(wp->mX, (s32)wp->mY);
        } else {
            field_0x230 = wp->mKind + 5;
        }
    } else {
        s8 id = wp->mKind;
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            cls_804ADF28* actor = (cls_804ADF28*)getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == ACTOR_EETC && id == actor->field_0x230) {
                actor->field_0x230 = 0;
                break;
            }
        }
    }
    field_0x294 = field_0x298->mNext;
    field_0x298++;
    if (field_0x298->mKind == -1000) {
        field_0x298 = NULL;
    }
    return true;
}

// FIXME(shotgun): 98.1 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::fn_8035F9C8() {
    if (field_0x234 == 0) {
        if (lbl_804742A8[mStep].mValue < 0) {
            mStep = 0;
        }
        field_0x234 = lbl_804742A8[mStep].mCount;
        field_0x23C = lbl_804742A8[mStep].mValue;
        mStep++;
    }
    if (field_0x26C > 3.0f) {
        field_0x26C -= 0.2f;
    }
    s16 prev = field_0x254;
    s32 step = (s32)(65536.0f * (1.3f / (6.283184f * field_0x26C)));
    field_0x254 = prev - step;
    field_0x3c.x = 1.3f * JMath::sincosTable_.cosShort(field_0x254);
    field_0x3c.y = 1.3f * JMath::sincosTable_.sinShort(field_0x254);
    field_0x3c.z = -0.2f;
    mPos += field_0x3c;
    field_0x264 += (s16)step;
    if (field_0x26C < 3.0f) {
        fn_803613D0();
        mAlive = false;
    }
}

// FIXME(shotgun): 98.4 %, scheduling+regalloc
u8 KaeporaGaeboraEnding_804ADAC8::fn_8035F3C0() {
    if (field_0x294 == 0 || field_0x298 == NULL) {
        return false;
    }
    JGeometry::TVec3f pos = mPos;
    pos.z = 0.0f;
    JGeometry::TVec3f target;
    const KaeporaWaypoint* wp = field_0x298;
    if (wp->mKind == -1) {
        if (wp->mRadius == -1.0f || wp->mRadius == -2.0f) {
            target.x = wp->mZ;
            target.y = 0.0f;
            target.z = 0.0f;
        } else if (wp->mRadius == -3.0f || wp->mRadius == -4.0f) {
            target.y = wp->mZ;
            target.x = 0.0f;
            target.z = 0.0f;
        } else {
            field_0x298++;
            if (field_0x298->mKind == -1000) {
                field_0x298 = NULL;
            }
            return true;
        }
    } else {
        target.x = wp->mX;
        target.y = wp->mY;
        target.z = wp->mZ;
    }
    f32 radius = field_0x298->mRadius;
    if (radius > 0.0f) {
        if (PSVECDistance(&pos, &target) < field_0x298->mRadius) {
            return fireWaypoint();
        }
    } else if (radius == -1.0f) {
        if (pos.x < target.x) {
            return fireWaypoint();
        }
    } else if (radius == -2.0f) {
        if (pos.x > target.x) {
            return fireWaypoint();
        }
    } else if (radius == -3.0f) {
        if (pos.y < target.y) {
            return fireWaypoint();
        }
    } else {
        if (pos.y > target.y) {
            return fireWaypoint();
        }
    }
    return false;
}

KaeporaGaeboraEnding_804ADAC8::~KaeporaGaeboraEnding_804ADAC8() {}
