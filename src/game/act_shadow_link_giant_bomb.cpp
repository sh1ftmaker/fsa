#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ViewLayer.h"
#include "game/act/act_shadow_link_giant_bomb.h"

cls_804AC4D0::~cls_804AC4D0() {
    if (lbl_8053AAF8->field_0x7f == 1) {
        s32 mode = lbl_8053AAF8->field_0x4c;
        if (mode != 9 && mode != 0xB) {
            lbl_8053AAF8->fn_80138B7C();
        }
    }
}

// FIXME: 99.88%. Only the two float literals differ: their relocations are
// lbl_805422E0 and lbl_805422C4 in the original, unnamed (@NNN) in ours; the
// literal split of this unit, not the code.
void cls_804AC4D0::init() {
    field_0x240 = (field_0x90 >> 12) & 0xFF;
    field_0x23c = 0;
    field_0x244 = -1;
    if (field_0x240 == 0) {
        field_0x254 = 1;
    } else {
        field_0x254 = 0;
        field_0x240--;
    }
    field_0x248 = (field_0x90 >> 22) & 0x1F;
    if (field_0x90 >> 27) {
        mState = 0;
        field_0x238 = 0;
        mStateTimer = 0;
    } else {
        mState = 1;
        field_0x238 = 0;
        mStateTimer = field_0x90 & 0x7F;
    }
    if (lbl_8053AAF8->field_0x7f == 0) {
        s32 layer = field_0x4;
        if (viewLayerFlag(layer) == 1) {
            mPos.y += 4.0f;
            mSpawnPos = mPos;
        }
    }
    field_0x250 = 1.0f;
    GameView_804A9950* view = fn_802D6A9C();
    bool inView = view->fn_802D800C(field_0x248);
    if (!inView && lbl_8053AAF8->field_0x7f == 1) {
        field_0x255 = 1;
    } else {
        field_0x255 = 0;
    }
}

// A diagonal scale in the sprite's 3x3, every off-diagonal entry stored as 0.
static inline void setScaleMtx(Mtx mtx, f32 x, f32 yz) {
    mtx[0][0] = x;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = yz;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = yz;
}

// FIXME: 99.74%. Only the float literals (1.0f, 2.0f, 3.0f, 0.0f) and the
// "player_sob.bin" string differ: their relocations are lbl_ names in the
// original and @NNN in ours; the literal split, not the code.
void cls_804AC4D0::draw() {
    if (mState == 8 || mState == 0xb) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        PSMTXIdentity(mtx);
        params.field_0x2e = 0x40;
        if (mState == 8) {
            setScaleMtx(mtx, field_0x250, 3.0f - 2.0f * field_0x250);
        } else {
            setScaleMtx(mtx, field_0x250, 2.0f - field_0x250);
        }
        params.mMtx = mtx;
        fn_801F06F0("player_sob.bin", field_0x23c, &params, 7, 1.0f, 1.0f);
    } else {
        if (mState >= 4 && mState <= 7) {
            fn_801F06F0("player_sob.bin", field_0x23c, NULL, 7, 1.0f, 1.0f);
        }
    }
}
