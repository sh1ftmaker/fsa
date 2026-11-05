#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Map.h"
#include "game/act/act_chief_soldier.h"

void ChiefSoldier_804A0B90::vfunc_0x24() {
    if (getType() == 6) {
        field_0x18c = 0;
        fn_801F856C()->fn_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
    }
    ActorBase_804A5770::vfunc_0x24();
}

// Sets the 3x3 part of m to a uniform scale; the translation column is the caller's.
static inline void setScaleMtx(Mtx m, f32 s) {
    m[0][0] = s;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = s;
    m[1][2] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = s;
}

void ChiefSoldier_804A0B90::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (lbl_8053AB60 != 1) {
        if (getType() == 7 || getType() == 8) {
            fn_80172BC8();
        } else {
            s32 hasMtx = 0;
            params.field_0x0[1] = field_0x234;
            if (field_0x230 == 6 && field_0x264 >= 3) {
                params.field_0x30 = 0x11;
            }
            if (field_0x4 >= 8) {
                if (field_0x23c == 1) {
                    setScaleMtx(mtx, 2.0f);
                    hasMtx = 1;
                    params.mMtx = mtx;
                }
                JGeometry::TBox2f box;
                box = fn_802D6A9C()->mVisibleBox;
                JGeometry::TVec3f pos;
                pos.set(mPos);
                pos.x += box.i.x;
                pos.y += box.i.y;
                if (getType() != 6) {
                    fn_801EF574("enemy_gc_sob.bin", field_0x280 + 0x226, &pos, &params);
                }
                fn_803075AC(mSerial, field_0x4, &mPos, field_0x280 + 0x318, hasMtx, 2, -1, 0, 0, 0);
            } else {
                fn_801F02BC("enemy_gc_sob.bin", field_0x280 + 0x226, &params, 7, 0, false, 1.0f,
                            1.0f);
            }
        }
    }
}

void ChiefSoldier_804A0B90::fn_80172BC8() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x90 != 7) {
        setScaleMtx(mtx, 2.0f);
        if (field_0x23c == 0) {
            setScaleMtx(mtx, 1.0f);
        }
        params.mMtx = mtx;
        s32 frame = field_0x280 + 0x3FD;
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
    }
}

void ChiefSoldier_804A0B90::vfunc_0x44() {
    if (field_0x90 == 2 || field_0x90 == 3) {
        field_0x90 += 2;
        field_0x240 = 0;
        field_0x264 = 0;
        field_0x250 = 0x14;
        if (field_0x23c == 0 && lbl_8053AAF8->field_0x7f == 1) {
            lbl_8053AF80->fn_80458FF0(0x1000021, false, false);
        }
        field_0x288 = 2;
        if (field_0x2a5 == 1) {
            fn_801F0E34(0x5001F, 0);
        }
        field_0x230 = 2;
    }
    if (getType() != 7 && getType() != 8 && mLife <= 0) {
        s32 serial = field_0x238;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
        field_0x238 = -1;
    }
}

ChiefSoldier_804A0B90::~ChiefSoldier_804A0B90() {
    if (getType() != 7 && getType() != 8) {
        if (getType() != 6 && lbl_8053AAF8->field_0x4c != 9 &&
            lbl_8053AAF8->field_0x4c != 0xB && mLife != 0) {
            lbl_8053AAF8->fn_80138B7C();
        }
        s32 serial = field_0x238;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
    }
}
