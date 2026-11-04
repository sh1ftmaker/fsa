#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/act/tu_80166BD0.h"

// Two tables in .data, read by update() (not written yet).
s32 lbl_804A0638[6] = {0, 1, 2, 3, 2, 1};
s32 lbl_804A0650[6] = {12, 8, 8, 12, 8, 8};

// FIXME: 99.0 %. Register allocation only: the scale (0x258) sits in f31 and
// the first sin result in f26 where the original has f29 and f28.
void cls_804A0668::draw() {
    if (field_0x230 == 3) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        JGeometry::TVec3f rot(0.0f, field_0x25c, 0.0f);
        setRotScaleMtx(mtx, rot, field_0x258);
        params.mMtx = mtx;
        s32 frame = field_0x250 + 0x9d;
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
    } else {
        fn_801F06F0("enemy_gc_sob.bin", field_0x250 + 0x9d, NULL, 0, 1.0f, 1.0f);
    }
}

// FIXME: 99.6 %. Only the .sdata2 literals: not split (lbl_8053D838.. vs @NNN).
void cls_804A0668::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    mLife = 4;
    field_0x250 = 0;
    field_0x25c = 0.0f;
    field_0x258 = 1.0f;
    field_0x260 = 0.0f;
    field_0x264 = 0.0f;
    field_0x268 = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x254 = 255.0f;
    mEmitter26c = NULL;
    field_0xb0 = 0x11101;
    field_0x230 = 0;
}

void cls_804A0668::vfunc_0x44() {
    if (mEmitter26c != NULL) {
        mEmitter26c->becomeInvalidEmitter();
        mEmitter26c = NULL;
    }
}

cls_804A0668::~cls_804A0668() {
    fn_801EE434(mEmitter26c);
}
