#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/DrawShadow.h"
#include "game/Effect.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_inextinguishable_flame.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Offsets of the eight neighbouring flames (update) and of the four
// neighbours tested by vfunc_0x58.
static const f32 lbl_8046F0C8[16] = {
    -16.0f, 0.0f, 16.0f, -16.0f, 16.0f, -16.0f, 0.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};
static const f32 lbl_8046F108[16] = {
    -16.0f, -16.0f, -16.0f, 0.0f, 0.0f, 16.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};
static const f32 lbl_8046F148[16] = {
    0.0f, -16.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};
static const f32 lbl_8046F188[16] = {
    -16.0f, 0.0f, 0.0f, 16.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};

// Returns 1 when one of the four neighbours of pos is a tile in the
// fn_801EE3C0 set.
// FIXME: 97.81 %. Same instructions; the original loads the table bases in
// the other order (lis lbl_8046F148 first, its pointer in r31), ours loads
// lbl_8046F188 first.
bool InextinguishableFlame_804A9D28::vfunc_0x58(JGeometry::TVec3f* pos) {
    bool result = false;
    for (s32 i = 0; i < 4; i++) {
        f32 y = pos->y + lbl_8046F188[i];
        f32 x = pos->x + lbl_8046F148[i];
        JGeometry::TVec3f p(x, y, pos->z);
        s32 tile = fn_80226850(&p, field_0x4);
        if (fn_801EE3C0(tile) == true) {
            result = true;
            break;
        }
    }
    return result;
}

void InextinguishableFlame_804A9D28::vfunc_0x24() {
    vfunc_0x54();
    fn_801F35AC();
}

static inline void invalidateEmitter(JPABaseEmitter*& emitter) {
    if (emitter != NULL) {
        emitter->becomeInvalidEmitter();
        emitter = NULL;
    }
}

void InextinguishableFlame_804A9D28::vfunc_0x54() {
    for (s32 i = 0; i < 16; i++) {
        invalidateEmitter(mEmitters[0][i]);
        invalidateEmitter(mEmitters[1][i]);
        invalidateEmitter(mEmitters[2][i]);
    }
}

void InextinguishableFlame_804A9D28::draw() {
    JGeometry::TVec3f pos = mPos;
    if (mState >= 1) {
        for (s32 i = 0; i < mCountX; i++) {
            for (s32 j = 0; j < mCountY; j++) {
                fn_8013CC50(field_0x4, &pos, 0x34c, 0, 0, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &pos, 0x34d, 0, 0, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &pos, 0x54d, 1, 0, NULL, NULL, true, 1.0f);
                drawShadow(field_0x4, &pos, JUtility::TColor(0, 0, 0, 0x80), 32.0f);
                pos.y += 16.0f;
            }
            pos.y = mPos.y;
            pos.x += 16.0f;
        }
    }
}

// The state machine. States: 0 wait for the view (field_0x300), 1 search for a
// tile, 2 spawn the grid, 3 hit, 4 spawn a BLZE actor, 5 count down, 6 wait
// for the player's hit. The grids are walked in the order of the spawn loops.
// FIXME: 89.77 %. The original copies mPos once more before the state test
// (dead, overwritten by the copy before fn_801F2718) and builds mBox + mPos
// on the stack before fn_801F2B7C although nothing reads it; MWCC drops the
// unread local here, so those stores are missing. The jump table is @785
// (dtk: jumptable_804A9D84), a data split.
void InextinguishableFlame_804A9D28::update() {
    if (mState >= 1) {
        fn_8020D58C()->addBox(mSerial);
    }
    JGeometry::TVec3f pos = mPos;
    fn_801F2718('TUBO');

    switch (mState) {
    case 0: {
        if (mFlag300 != 0 && vfunc_0x34() != true) {
            break;
        }
        JGeometry::TVec3f grid = mPos;
        s32 n = 0;
        for (s32 i = 0; i < mCountX; i++) {
            for (s32 j = 0; j < mCountY; j++) {
                mEmitters[0][n] = fn_8013CC50(field_0x4, &grid, 0x34c, 0, 0, NULL, NULL, true, 1.0f);
                mEmitters[1][n] = fn_8013CC50(field_0x4, &grid, 0x34d, 0, 0, NULL, NULL, true, 1.0f);
                mEmitters[2][n] = fn_8013CC50(field_0x4, &grid, 0x54d, 1, 0, NULL, NULL, true, 1.0f);
                grid.y += 16.0f;
                n++;
            }
            grid.y = mPos.y;
            grid.x += 16.0f;
        }
        if (mKind == 0) {
            mState = 2;
        } else if (mKind == 3) {
            mState = 6;
        } else {
            mState = 1;
        }
        field_0x94 = 1;
        break;
    }
    case 1:
        for (s32 i = 0; i < 8; i++) {
            f32 x = mPos.x + lbl_8046F0C8[i];
            f32 y = mPos.y + lbl_8046F108[i];
            JGeometry::TVec3f tile(x, y, mPos.z);
            if (fn_801EE3C0(fn_80226850(&tile, field_0x4)) == true) {
                fn_801F856C()->fn_801F9484('BLZE', field_0x4, &tile, (field_0x98 << 4) | 0xF, -1, -1);
                mActorId = 'BLZE';
                mState = 4;
                mTimer = 40;
                break;
            }
        }
        break;
    case 2:
        if (field_0x94 == 2 || (mKind == 2 && vfunc_0x34() == true)) {
            fn_802D6A9C()->fn_802D8050(mViewBit, true, 0xFF);
            vfunc_0x54();
            s32 n = 0;
            for (s32 i = 0; i < mCountX; i++) {
                for (s32 j = 0; j < mCountY; j++) {
                    mEmitters[0][n] = fn_8013CC50(field_0x4, &pos, 0x4f2, 1, 0, NULL, NULL, true, 1.0f);
                    n++;
                    pos.y += 16.0f;
                }
                pos.y = mPos.y;
                pos.x += 16.0f;
            }
            mState = 5;
            mTimer = 60;
            field_0x94 = 0;
            break;
        }
        {
            JGeometry::TVec3f origin = mPos;
            for (s32 i = 0; i < mCountX; i++) {
                for (s32 j = 0; j < mCountY; j++) {
                    JGeometry::TVec3f probe = mPos;
                    if (vfunc_0x58(&probe) == true) {
                        mState = 3;
                        return;
                    }
                    fn_801F2B7C(field_0x114, -1, 7);
                    JGeometry::TVec2<f32> p(mPos.x, mPos.y);
                    JGeometry::TBox2f box(field_0x80);
                    box.addPos(p);
                    fn_80239914(&box, field_0x4, mSerial);
                    mPos.y += 16.0f;
                }
                mPos.y = origin.y;
                mPos.x += 16.0f;
            }
            mPos.x = origin.x;
            mPos.y = origin.y;
            mPos.z = origin.z;
        }
        break;
    case 3:
        vfunc_0x54();
        for (s32 i = 0; i < mCountX; i++) {
            for (s32 j = 0; j < mCountY; j++) {
                fn_801F856C()->fn_801F9484('BLZE', field_0x4, &pos, (field_0x98 << 4) | 0xF, -1, -1);
                mActorId = 'BLZE';
                pos.y += 16.0f;
            }
            pos.y = mPos.y;
            pos.x += 16.0f;
        }
        field_0x94 = 0;
        vfunc_0x24();
        break;
    case 4:
        mTimer--;
        if (mTimer == 0) {
            vfunc_0x24();
        }
        break;
    case 5:
        mTimer--;
        if (mTimer == 0) {
            vfunc_0x24();
        } else if (mTimer == 0x34) {
            fn_801F0E34(0x30061, 0);
        }
        break;
    case 6:
        if (fn_801F2B7C(field_0x114, -1, 7)) {
            if (fn_80234CF8(0)) {
                mTimer = 0;
            } else {
                mTimer = 1;
            }
        }
        if (mFlag300 != 0 && vfunc_0x34() != true) {
            vfunc_0x54();
            field_0x94 = 0;
            mState = 0;
        }
        break;
    default:
        break;
    }
}

InextinguishableFlame_804A9D28::~InextinguishableFlame_804A9D28() {
    for (s32 i = 0; i < 16; i++) {
        if (mEmitters[0][i] != NULL) {
            fn_801EE434(mEmitters[0][i]);
        }
        if (mEmitters[1][i] != NULL) {
            fn_801EE434(mEmitters[1][i]);
        }
        if (mEmitters[2][i] != NULL) {
            fn_801EE434(mEmitters[2][i]);
        }
    }
}

// FIXME: 98.37 %. One instruction more than the original: it sets the zero it
// reuses for the state-0 store (`li r28, 0`) before the first loop's
// float literal load, ours has it later.
void InextinguishableFlame_804A9D28::init() {
    mCountX = ((field_0x90 >> 22) & 0x1F) + 1;
    mCountY = ((field_0x90 >> 17) & 0x1F) + 1;
    mKind = field_0x90 & 0xF;
    mFlag300 = (field_0x90 >> 4) & 1;
    mViewBit = (field_0x90 >> 12) & 0x1F;

    for (int i = 0; i < 16; i++) {
        mEmitters[0][i] = NULL;
        mEmitters[1][i] = NULL;
        mEmitters[2][i] = NULL;
    }

    if (mFlag300 == 0) {
        JGeometry::TVec3f pos = mPos;
        s32 n = 0;
        for (s32 i = 0; i < mCountX; i++) {
            for (s32 j = 0; j < mCountY; j++) {
                fn_8013CC50(field_0x4, &pos, 0x34c, 0, 0, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &pos, 0x34d, 0, 0, NULL, NULL, true, 1.0f);
                mEmitters[2][n] = fn_8013CC50(field_0x4, &pos, 0x54d, 1, 0, NULL, NULL, true, 1.0f);
                pos.y += 16.0f;
                n++;
            }
            pos.y = mPos.y;
            pos.x += 16.0f;
        }
        mState = 2;
        field_0x94 = 1;
    } else {
        mState = 0;
        field_0x94 = 0;
    }

    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -7.0f;
    field_0x80.i.y = -7.0f;
    field_0x80.f.x = 7.0f;
    field_0x80.f.y = 7.0f;
    field_0x114 = 1;
}
