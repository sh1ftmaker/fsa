#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/act/tu_80166BD0.h"

// Two tables in .data, read by update() (not written yet).
s32 lbl_804A0638[6] = {0, 1, 2, 3, 2, 1};
s32 lbl_804A0650[6] = {12, 8, 8, 12, 8, 8};

// setRotScaleMtx (game/RotMatrix.h) with the angles as scalars: the original
// passes them straight from the constants, with no TVec3f on the stack.
static inline void setRotScaleMtxAngles(Mtx m, f32 rx, f32 ry, f32 rz, f32 scale) {
    f32 sz = sin(rz);
    f32 sy = sin(ry);
    f32 sx = sin(rx);
    f32 cx = cos(rx);
    f32 cy = cos(ry);
    f32 cz = cos(rz);
    m[0][0] = scale * (cy * cz);
    m[1][0] = scale * (cy * sz);
    m[2][0] = scale * -sy;
    m[0][1] = scale * (sx * sy * cz - cx * sz);
    m[1][1] = scale * (sx * sy * sz + cx * cz);
    m[2][1] = scale * (sx * cy);
    m[0][2] = scale * (cx * cz * sy + sx * sz);
    m[1][2] = scale * (cx * sz * sy - sx * cz);
    m[2][2] = scale * (cx * cy);
}

// FIXME: 97.49 %. The original loads the zero angles as the float 0.0f (lfs)
// for each sin/cos call; ours folds them into a double constant (lfd). The
// scale (0x258) also sits in f31 and the sin results in f26/f28 where the
// original has it in f29 and f26/f28 swapped.
void cls_804A0668::draw() {
    if (field_0x230 == 3) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        f32 angleY = field_0x25c;
        setRotScaleMtxAngles(mtx, 0.0f, angleY, 0.0f, field_0x258);
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

// FIXME: 78.00 %. Stops the emitter and drops it. The original sets bit 0x1 of the
// emitter's status word at 0xF4 and writes 1 to 0x24 (as fn_801CB7A0 does), but
// include/JSystem/JParticle/JPAEmitter.h puts the status at 0x20C and has
// mEmitterRot at 0x24, so the layouts disagree. Needs the JPABaseEmitter layout
// fixed in the shared JSystem header.
void cls_804A0668::vfunc_0x44() {
    if (mEmitter26c != NULL) {
        mEmitter26c->stopCreateParticle();
        mEmitter26c = NULL;
    }
}

cls_804A0668::~cls_804A0668() {
    fn_801EE434(mEmitter26c);
}
