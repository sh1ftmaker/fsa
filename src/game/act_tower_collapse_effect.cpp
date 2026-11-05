#include "game/GameAudio.h"
#include "game/act/act_tower_collapse_effect.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/RotMatrix.h"

// Turns a sprite to the angle (s16 units, as field_0x264 and friends hold it)
// about y, at the actor's current position.
static inline void setSpriteRotation(Mtx mtx, SpriteParams_801F5AE0& params, s32 angle) {
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    JGeometry::TVec3f rot(0.0f, (f32)angle / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    params.field_0x1d = 1;
}

// FIXME: 86.64 %. Scheduling: the original loads the rotation constants and
// field_0x23c/0x240 interleaved with the mPos stores and the int-to-float
// conversion; ours keeps them in source order (4 more instructions), and the
// literals are @NNN, not lbl_8053C1D0-lbl_8053C1E0.
void TowerCollapseEffect_8049CA40::fn_80103348() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    JGeometry::TVec3f savedPos = mPos;
    params.field_0x1d = -3;
    void* res = fn_804032E8('TIMG', "kowareyuka_a.bti");
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);

    mPos.set(field_0x238[0].x, field_0x238[0].y, field_0x238[0].z);
    setSpriteRotation(mtx, params, field_0x264[0]);
    res = fn_804032E8('TIMG', "kowareyuka_b.bti");
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);

    mPos.set(field_0x238[1].x, field_0x238[1].y, field_0x238[1].z);
    setSpriteRotation(mtx, params, field_0x264[1]);
    res = fn_804032E8('TIMG', "kowareyuka_c.bti");
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);

    mPos.set(field_0x238[2].x, field_0x238[2].y, field_0x238[2].z);
    setSpriteRotation(mtx, params, field_0x264[2]);
    res = fn_804032E8('TIMG', "kowareyuka_d.bti");
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);

    mPos.set(savedPos);
}

void TowerCollapseEffect_8049CA40::draw() {
    switch (mMode) {
    case 0:
        fn_80103348();
        break;
    case 1:
        if (field_0x230 == 2) {
            SpriteParams_801F5AE0 params;
            void* res = fn_804032E8('TIMG', "gareki.bti");
            fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);
        }
        break;
    case 2:
        // Nothing draws mode 2; the empty case is part of the original's dispatch.
        break;
    }
}

// FIXME: 99.66 %. The original's mode-1 dispatch has a second unconditional
// `b` to the end after the state-0 test (0x3E8); ours has one.
void TowerCollapseEffect_8049CA40::update() {
    switch (mMode) {
    case 0:
        switch (field_0x230) {
        case 0:
            if (vfunc_0x34()) {
                field_0x230 = 1;
                field_0x288 = 0;
                fn_8013CC50(field_0x4, &mPos, 0x499, 2, 2, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &mPos, 0x49a, 2, 2, NULL, NULL, true, 1.0f);
            }
            break;
        case 1:
            field_0x288++;
            fn_801F0E34(0x3006F, 0);
            if (field_0x288 >= 0xB4) {
                field_0x230 = 2;
                field_0x288 = 0;
            }
            break;
        case 2:
            fn_801F0E34(0x3006F, 0);
            field_0x288++;
            if (field_0x288 > 1) {
                field_0x28c.y += 0.08f;
                field_0x264[1] += 0x20;
            }
            if (field_0x288 > 0x1E) {
                field_0x28c.x += 0.08f;
                field_0x264[0] -= 0x40;
                field_0x238[0].x += 1.0f;
            }
            if (field_0x288 > 0x3C) {
                field_0x28c.z += 0.08f;
                field_0x264[2] -= 0x40;
            }
            if (field_0x288 == 1) {
                fn_801F0E34(0x30071, 0);
                fn_8013CC50(field_0x4, &mPos, 0x49d, 2, 2, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &mPos, 0x49e, 2, 2, NULL, NULL, true, 1.0f);
                lbl_8053AAF8->fn_80134DE4(1, 0);
            } else if (field_0x288 == 0x1E) {
                fn_801F0E34(0x30071, 0);
                fn_8013CC50(field_0x4, &mPos, 0x49b, 2, 2, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &mPos, 0x49c, 2, 2, NULL, NULL, true, 1.0f);
                lbl_8053AAF8->fn_80134DE4(1, 0);
            } else if (field_0x288 == 0x3C) {
                fn_801F0E34(0x30070, 0);
                fn_8013CC50(field_0x4, &mPos, 0x49f, 2, 2, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &mPos, 0x4a0, 2, 2, NULL, NULL, true, 1.0f);
                lbl_8053AAF8->fn_80134DE4(3, 0);
            }
            if (field_0x288 == 0x46) {
                u32 bit = field_0x25c;
                if (bit != 0) {
                    fn_802D6A9C()->fn_802D8050(bit, true, 0xFF);
                    vfunc_0x30(0xFF, false);
                }
            }
            if (field_0x288 > 0x12C) {
                mAlive = false;
            }
            field_0x238[0].y += field_0x28c.x;
            field_0x238[1].y += field_0x28c.y;
            field_0x238[2].y += field_0x28c.z;
            break;
        }
        break;
    case 1:
        switch (field_0x230) {
        case 0:
            if (vfunc_0x34()) {
                field_0x230 = 2;
                fn_8013CC50(field_0x4, &mPos, 0xd3, 2, 2, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &mPos, 0xd4, 2, 2, NULL, NULL, true, 1.0f);
                lbl_8053AAF8->fn_80134DE4(1, 0);
                fn_801F0E34(0x30001, 0);
            }
            break;
        case 1:
            break;
        }
        break;
    case 2:
        // Nothing updates mode 2; the empty case is part of the original's dispatch.
        break;
    }
}

TowerCollapseEffect_8049CA40::~TowerCollapseEffect_8049CA40() {
    if (mEmitter260 != NULL) {
        fn_801EE434(mEmitter260);
    }
}

void TowerCollapseEffect_8049CA40::init() {
    field_0x230 = 0;
    mMode = 0;
    mEmitter260 = NULL;
    mMode = field_0x90 & 0xF;
    field_0x25c = (field_0x90 >> 22) & 0x1F;
    switch (mMode) {
    case 0:
        field_0x288 = 0;
        field_0x264[0] = 0;
        field_0x264[1] = 0;
        field_0x264[2] = 0;
        field_0x28c.x = 0.0f;
        field_0x28c.y = 0.0f;
        field_0x28c.z = 0.0f;
        field_0x238[0] = mPos;
        field_0x238[1] = mPos;
        field_0x238[2] = mPos;
        break;
    case 2:
        mEmitter260 = fn_8013CC50(field_0x4, &mPos, 0x496, 1, 0, NULL, NULL, true, 1.0f);
        break;
    }
}
