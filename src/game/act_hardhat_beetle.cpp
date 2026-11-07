#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/act/act_hardhat_beetle.h"

// FIXME(shotgun): update (0x80167C94, 2.3 KB) is not written.
// FIXME(shotgun): 89.88 %, missing/extra + reloc-name
void HardhatBeetle_804A06C0::init() {
    mBox.set(-6.0f, -6.0f, 6.0f, 6.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    mFrame = 0;
    field_0x278 = 0.2f;
    field_0x27c = 0.2f;
    field_0x280 = 0.2f;
    field_0x26c.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x25c = 0;
    mScale = 1.0f;
    mHome = mPos;
    field_0xb0 = 0x11101;
    field_0xb8 = 1;
    mState = 0;
}

// FIXME: 99.0 %. regalloc-only: the original keeps mScale in f29 and cos x
// in f31, ours the other way round, and sin x / sin z get f28 / f26 there,
// f26 / f28 here.
void HardhatBeetle_804A06C0::draw() {
    if (mState == 2) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        // The original stores &mtx to params.mMtx twice, here (0x80167AEC)
        // and after the rotation (0x80167B44).
        params.mMtx = mtx;
        JGeometry::TVec3f rot(0.0f, mAngle, 0.0f);
        setRotScaleMtx(mtx, rot, mScale);
        params.mMtx = mtx;
        s32 frame = mFrame + 0x92;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
    } else {
        drawSprite_801F06F0("enemy_gc_sob.bin", mFrame + 0x92, NULL, 0, 1.0f, 1.0f);
    }
}

HardhatBeetle_804A06C0::~HardhatBeetle_804A06C0() {}
