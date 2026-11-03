#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/act/act_force_gem_wooden.h"

static const char lbl_80471718[] = "enemy_both2_sob.bin";
static const char lbl_8047172C[] = "enemy_gc_sob.bin";

static const f32 lbl_80541A20[1] = {0.0f};
static const f32 lbl_80541A24[1] = {1.0f};
static const f32 lbl_80541A28[1] = {1.25f};
static const f32 lbl_80541A2C[1] = {3.0f};
static const f32 lbl_80541A30[1] = {0.1f};
static const f32 lbl_80541A34[1] = {0.02f};
static const f32 lbl_80541A38[1] = {60.0f};
static const f32 lbl_80541A3C[1] = {0.6f};
static const f32 lbl_80541A40[1] = {30.0f};
static const f32 lbl_80541A44[1] = {-6.0f};
static const f32 lbl_80541A48[1] = {6.0f};
static const f32 lbl_80541A4C[1] = {-7.0f};
static const f32 lbl_80541A50[2] = {7.0f, 0.0f};

// setRotScaleMtx (game/RotMatrix.h) with the angles as scalars: the original
// passes them straight from the constants, with no TVec3f on the stack.
static inline void setRotScaleMtxAngles(Mtx m, f32 rx, f32 ry, f32 rz, f32 scale) {
    f32 sx = sin(rx);
    f32 sy = sin(ry);
    f32 sz = sin(rz);
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

// The ids fn_801F26CC searches for: 'FIRE', 'BLZE', 'DDGO', then the 'NULL' end.
static const u32 lbl_80471708[4] = {0x46495245, 0x424C5A45, 0x4444474F, 0x4E554C4C};

// TRandom_fast_::get_ufloat_1 in [0, 1), with its 1.0f read from the named
// constant lbl_80541A24 as the original does (the header's `- 1` is an
// anonymous literal in every unit that inlines it).
static inline f32 randomUfloat(JMath::TRandom_fast_& random) {
    union {
        f32 f;
        u32 s;
    } out;
    out.s = (random.get() >> 9) | 0x3f800000;
    return out.f - lbl_80541A24[0];
}

WoodenForceGem_804AB1F0::~WoodenForceGem_804AB1F0() {
    fn_801EE434(mEmitter268);
}

void WoodenForceGem_804AB1F0::init() {
    field_0x258 = field_0x90 & 0xF;
    field_0x248 = (field_0x90 >> 16) & 0xF;
    mEmitter268 = NULL;
    field_0x260 = (field_0x90 >> 22) & 0x1F;
    field_0xb0 = 0x2300;
    field_0x230 = 0;
    field_0x264 = 0;
    mLife = 10000;
    field_0x244 = 0;
    field_0x254 = -1;
    field_0x98 = 0;
    field_0x238 = lbl_80541A24[0];
    field_0x23c = lbl_80541A20[0];
    field_0x25c = 0;
    field_0x24c = (s32)(randomUfloat(lbl_8053AAF8->mRandom) * lbl_80541A38[0]) + 60;
    field_0x250 = 0;
    if (field_0x258 == 4 || field_0x258 == 5) {
        field_0x240 = lbl_80541A3C[0];
    } else {
        field_0x240 = lbl_80541A24[0];
    }
    if (vfunc_0x34()) {
        fn_801F3668();
        return;
    }
    field_0x234 = lbl_80541A40[0];
    field_0x3c.x = lbl_80541A20[0];
    field_0x3c.y = lbl_80541A20[0];
    field_0x3c.z = lbl_80541A20[0];
    field_0x80.i.x = lbl_80541A44[0];
    field_0x80.i.y = lbl_80541A44[0];
    field_0x80.f.x = lbl_80541A48[0];
    field_0x80.f.y = lbl_80541A48[0];
    mBox.i.x = lbl_80541A4C[0];
    mBox.i.y = lbl_80541A4C[0];
    mBox.f.x = lbl_80541A50[0];
    mBox.f.y = lbl_80541A50[0];
    if (lbl_8053AAF8->field_0x7f == 0) {
        fn_801F3668();
    }
}

// FIXME: 98.27%. The emitter block in case 3 still differs: the original also stores 1
// at offset 0x24 of the emitter (after the status-bit store, which stopCreateParticle
// covers) and orders the pointer and status work differently, so the registers differ.
// include/JSystem/JParticle/JPAEmitter.h has no field at 0x24 (mEmitterRot is an s16
// vector there), so it needs a header decision outside include/game/. The original also
// stores and reloads field_0x250 near the end of the function (the field_0x248 == 1
// block), which ours does not.
void WoodenForceGem_804AB1F0::update() {
    JGeometry::TBox2f box(mBox.i, mBox.f);
    JGeometry::TVec2f p(mPos.x, mPos.y);
    box.addPos(p);
    switch (field_0x230) {
    case 0: {
        fn_80239914(&box, field_0x4, mSerial);
        s32 serial = mSerial;
        s32 layer = field_0x4;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        boxes->mLists[boxes->field_0x0].fn_8020D8C0(&box, layer, serial);
        if (fn_8023AB94(&box, field_0x4, &p) >= 0 || fn_801F26CC(lbl_80471708) >= 0) {
            if (field_0x248 == 0) {
                fn_801F0E34(0x30005, 0);
                fn_801F856C()->fn_801F9484(0x424C5A45, field_0x4, &mPos, 0, -1, -1);
            } else {
                fn_8013CC50(field_0x4, &mPos, 0x28D, 0, 0, NULL, NULL, true, lbl_80541A28[0] * field_0x240);
                fn_8013CC50(field_0x4, &mPos, 0x28E, 0, 0, NULL, NULL, true, lbl_80541A28[0] * field_0x240);
                fn_801F0E34(0x30061, 0);
            }
            field_0x230 = 1;
            field_0x25c = 0x28;
        }
        break;
    }
    case 1: {
        fn_80239914(&box, field_0x4, mSerial);
        field_0x25c--;
        if (field_0x25c <= 0) {
            s32 kind = field_0x258;
            s32 arg;
            if (kind == 4 || kind == 5) {
                arg = kind - 4;
            } else {
                arg = kind + 4;
            }
            s32 serial = fn_801F856C()->fn_801F8544(0x52555059, field_0x4, &mPos, arg);
            s32 bit = field_0x260;
            if (bit > 0) {
                fn_802D6A9C()->fn_802D8050(bit, true, 0xFF);
            }
            if (serial >= 0) {
                ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
                if (actor != NULL) {
                    JGeometry::TVec3f v(lbl_80541A20[0], lbl_80541A20[0], lbl_80541A2C[0]);
                    actor->field_0x3c = v;
                }
            }
            fn_8013CC50(field_0x4, &mPos, 0x19D, 0, 0, NULL, NULL, true, lbl_80541A30[0]);
            fn_801F3668();
        }
        break;
    }
    case 3:
        if (field_0x230 == 3 && mEmitter268 != NULL) {
            mEmitter268->stopCreateParticle();
            mEmitter268 = NULL;
        }
        // fall through
    case 2:
        field_0x23c += lbl_80541A30[0];
        field_0x238 -= lbl_80541A34[0];
        if (field_0x238 < lbl_80541A30[0]) {
            fn_801F3668();
        }
        break;
    }
    if (field_0x248 == 1) {
        field_0x24c--;
        if (field_0x24c <= 0) {
            field_0x250 = (field_0x250 + 1) & 3;
            if (field_0x250 == 0) {
                field_0x24c = (s32)(randomUfloat(lbl_8053AAF8->mRandom) * lbl_80541A38[0]) + 60;
            } else {
                field_0x24c = 2;
            }
        }
    }
}

// FIXME: 98.72%. Register allocation only. The scale (field_0x240 or the branch's
// field_0x238 * field_0x240) is in f31 where the original has it in f29, the sine
// results sit in other saved registers, and the products are in another operand order.
// The shape is right: the angles are scalars (see setRotScaleMtxAngles above), each
// constant is loaded where the original loads it, and params.mMtx is stored after the
// rotation. The parameter order (scale first) did not move the registers.
void WoodenForceGem_804AB1F0::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtxA;
    Mtx mtxB;
    JGeometry::TVec3f saved;
    const char* name = lbl_80471718;
    s32 frame = 0x89;
    if (field_0x248 == 1) {
        name = lbl_8047172C;
        frame = field_0x250 + 0x471;
    }
    params.field_0x2d = 1;
    params.field_0x1d = -1;
    if (field_0x230 == 2 || field_0x230 == 3) {
        f32 scale = field_0x238 * field_0x240;
        PSMTXIdentity(mtxA);
        setRotScaleMtxAngles(mtxA, lbl_80541A20[0], field_0x23c, lbl_80541A20[0], scale);
        params.mMtx = mtxA;
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', name);
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, lbl_80541A24[0], lbl_80541A24[0]);
        }
        return;
    }
    params.field_0x1d = 0;
    PSMTXIdentity(mtxB);
    setRotScaleMtxAngles(mtxB, lbl_80541A20[0], field_0x23c, lbl_80541A20[0], field_0x240);
    params.mMtx = mtxB;
    if (mPos.z != lbl_80541A20[0]) {
        params.field_0x1c = 1;
    }
    if (field_0x258 == 4 || field_0x258 == 5) {
        fn_801F06F0(name, frame, &params, -1, lbl_80541A24[0], lbl_80541A24[0]);
        saved = mPos;
        mPos.y -= lbl_80541A24[0];
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, 7, name, frame, NULL);
        }
        mPos = saved;
    } else {
        fn_801F06F0(name, frame, &params, 7, lbl_80541A24[0], lbl_80541A24[0]);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, frame, false, 2, -1, 0, 0, 0);
}

bool WoodenForceGem_804AB1F0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tile = fn_8022461C(pos, layer);
    if (tile == 0xC || tile == 3 || tile == 0xE) {
        field_0x3c.z = lbl_80541A20[0];
        field_0x3c.y = lbl_80541A20[0];
        field_0x3c.x = lbl_80541A20[0];
        field_0x230 = 2;
        return false;
    }
    if (tile == 0xD) {
        field_0x3c.z = lbl_80541A20[0];
        field_0x3c.y = lbl_80541A20[0];
        field_0x3c.x = lbl_80541A20[0];
        if (mEmitter268 == NULL) {
            mEmitter268 = fn_8013CC50(field_0x4, &mPos, 0x3D0, 0, 0, NULL, NULL, true, lbl_80541A24[0]);
            fn_801F0E34(0x30022, 0);
        }
        field_0x230 = 3;
        return false;
    }
    return tile != 0;
}
