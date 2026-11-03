#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/act/act_hardhat_beetle.h"

cls_804A06C0::~cls_804A06C0() {}

// FIXME: 99.0 %. regalloc-only: the original keeps mScale in f29 and cos x
// in f31, ours the other way round, and sin x / sin z get f28 / f26 there,
// f26 / f28 here.
void cls_804A06C0::draw() {
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
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
    } else {
        fn_801F06F0("enemy_gc_sob.bin", mFrame + 0x92, NULL, 0, 1.0f, 1.0f);
    }
}
