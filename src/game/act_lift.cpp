#include "game/GameAudio.h"
#include "game/act/act_lift.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/ResTIMG.h"

// The "TIMG" texture names by mode (0x90 & 0x7F).
const char* lbl_804A6F48[9] = {
    "lift_00.bti", "lift_01.bti", "lift_01_ana.bti", "lift_01_edge.bti", "lift_02a.bti",
    "lift_03.bti", "lift_00.bti", "lift_00.bti",     "lift_00.bti",
};

// The rotation of game/RotMatrix.h's setRotScaleMtx, with the angles as s16
// read from JMath's sincos table.
static inline void setRotScaleMtxS16(Mtx m, s16 x, s16 y, s16 z, f32 scale) {
    f32 sx = JMath::sincosTable_.sinShort(x);
    f32 sy = JMath::sincosTable_.sinShort(y);
    f32 sz = JMath::sincosTable_.sinShort(z);
    f32 cx = JMath::sincosTable_.cosShort(x);
    f32 cy = JMath::sincosTable_.cosShort(y);
    f32 cz = JMath::sincosTable_.cosShort(z);
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

Lift_804A6F6C::~Lift_804A6F6C() {}

// FIXME: 84.93 %. Same instructions; scheduling: MWCC places the reset
// stores' li r9/r8 before the 0.0 lfs that the original loads first.
void Lift_804A6F6C::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.mMtx = mtx;
    params.field_0x21 = 1;
    params.field_0x1d = -2;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    setRotScaleMtxS16(mtx, 0, field_0x238, 0, 1.0f);
    const char* name = lbl_804A6F48[getMode()];
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('TIMG', name);
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
    }
}

// FIXME: 78.61 %. The frame is 0xA4 (original 0x80): the diff/d/push
// temporaries and the operator- results take more stack than the original;
// this sits in r31 and i in r30 (original: r30 and r31).
void Lift_804A6F6C::update() {
    // Declared in this order: the frame lays later locals lower.
    JGeometry::TVec3f push;
    JGeometry::TVec3f diff;
    JGeometry::TVec3f d;
    if (field_0x90 >> 27 == 0 || vfunc_0x34() == true) {
        fn_80256410();
    }
    for (s32 i = 0; i < 4; i++) {
        if (!fn_802565BC(i)) {
            continue;
        }
        if (field_0x90 >> 27 != 0 && vfunc_0x34() != true) {
            continue;
        }
        diff.set(mPos - mPrevPos);
        fn_8023DF68(i, &diff);
        if (getMode() >= 6) {
            continue;
        }
        d.set(mPos - *fn_8023E724(i));
        s32 angle = JMath::atanTable_.atan2_(d.x, d.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        f32 len = JGeometry::TVec2f(d.x, d.y).length();
        s16 a = angle - 0x40;
        push.set(JGeometry::TVec3f(len * JMath::sincosTable_.cosShort(a), len * JMath::sincosTable_.sinShort(a), 0.0f) - d);
        fn_8023DFF4(i, &push);
    }
}

void Lift_804A6F6C::fn_80256410() {
    if (field_0x234 != 0) {
        field_0x234--;
        return;
    }
    if (getMode() == 6 || getMode() == 7) {
        u32 limit = (field_0x90 & 0x3E0000) >> 12;
        if (field_0x230 < limit) {
            field_0x230++;
        } else {
            field_0x234 = 60;
            field_0x230 = 0;
            switch (field_0x98) {
            case 0:
                field_0x98 = 1;
                break;
            case 1:
                field_0x98 = 0;
                break;
            case 2:
                field_0x98 = 3;
                break;
            case 3:
                field_0x98 = 2;
                break;
            }
        }
    }
    if (getMode() < 6) {
        field_0x238 += 0x40;
    }
    if (getMode() == 6) {
        if (field_0x98 == 0) {
            mPos.x += 0.5f;
        } else {
            mPos.x -= 0.5f;
        }
    }
    if (getMode() == 7) {
        if (field_0x98 == 2) {
            mPos.y += 0.5f;
        } else {
            mPos.y -= 0.5f;
        }
    }
    if (getMode() != 8) {
        return;
    }
    field_0x23a += 0x40;
    mPos.x = mSpawnPos.x + 64.0f * JMath::sincosTable_.cosShort(field_0x23a);
    mPos.y = mSpawnPos.y + 64.0f * JMath::sincosTable_.sinShort(field_0x23a);
}

// FIXME: 74.75 %. The flags test loads 0x90 into r3 and reuses it; the
// original loads it into r0 for the test and reloads 0x90 (r31) for the oris.
void Lift_804A6F6C::init() {
    if (((field_0x90 >> 17) & 0x1F) == 0) {
        field_0x90 = field_0x90 | 0x20000;
    }
    field_0x23c = lbl_8053AAF8->field_0xa4->getResource(0x3F3F3F3F /* "????": a char constant would be a trigraph */, lbl_804A6F48[getMode()]);
    ResTIMG* res = static_cast<ResTIMG*>(field_0x23c);
    mBox.i.x = -(s32)res->mSizeX / 2;
    mBox.i.y = -(s32)res->mSizeY / 2;
    mBox.f.x = res->mSizeX / 2;
    mBox.f.y = res->mSizeY / 2;
    mLife = 0;
    field_0x230 = 0;
    field_0x238 = 0;
    field_0x23a = 0;
    if (getMode() == 8) {
        mPos.x = mSpawnPos.x + 64.0f * JMath::sincosTable_.cosShort(field_0x23a);
        mPos.y = mSpawnPos.y + 64.0f * JMath::sincosTable_.sinShort(field_0x23a);
    }
    if (getMode() == 6 || getMode() == 7) {
        field_0x234 = 60;
    } else {
        field_0x234 = 0;
    }
    if (getMode() == 7) {
        field_0x98 = 3;
    } else {
        field_0x98 = 0;
    }
}
