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

// FIXME: 74.75 %. The flags test loads 0x90 into r3 and reuses it; the
// original loads it into r0 for the test and reloads 0x90 (r31) for the oris.
void Lift_804A6F6C::init() {
    if (((field_0x90 >> 17) & 0x1F) == 0) {
        field_0x90 = field_0x90 | 0x20000;
    }
    field_0x23c = gGameRoot_8053AAF8->field_0xa4->getResource(0x3F3F3F3F /* "????": a char constant would be a trigraph */, lbl_804A6F48[getMode()]);
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

// FIXME(shotgun): 66.9 %, missing/extra: the player-position vector arithmetic, the atan2 argument order and the tile-index math are scheduled differently
bool Lift_804A6F6C::fn_802565BC(s32 index) {
    if (getPlayerLayer_8023DE58(index) != field_0x4) {
        return false;
    }
    for (s32 i = 0; i < 8; i++) {
        JGeometry::TVec3f p;
        fn_8022E088(index, i, &p);
        p.x -= mPos.x;
        p.y -= mPos.y;
        p.z -= mPos.z;
        p.z = 0.0f;
        s32 angle = JMath::atanTable_.atan2_(p.y, p.x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        angle -= field_0x238;
        f32 len = JGeometry::TVec2f(p.x, p.y).length();
        JGeometry::TVec3f q(len * JMath::sincosTable_.cosShort(angle), len * JMath::sincosTable_.sinShort(angle), 0.0f);
        bool inside = false;
        if (q.x >= mBox.i.x && q.y >= mBox.i.y && q.x < mBox.f.x && q.y < mBox.f.y) {
            inside = true;
        }
        if (inside) {
            ResTIMG* res = static_cast<ResTIMG*>(field_0x23c);
            s32 px = (s32)q.x + (res->mSizeX >> 1);
            s32 py = (s32)q.y + (res->mSizeY >> 1);
            bool hit = false;
            if (px >= 0 && px < res->mSizeX && py >= 0 && py < res->mSizeY && res->mTextureFormat == 8) {
                if (res->mLutFormat == 2) {
                    s32 tilesPerRow = ((res->mSizeX + 7) & 0xFFF8) >> 3;
                    s32 offset = (((px >> 3) + (py >> 3) * tilesPerRow) << 6) + ((py & 7) << 3) + (px & 7);
                    u8 byte = ((u8*)res)[res->mImageDataOffset + (offset >> 1)];
                    u32 paletteIndex = (px & 1) == 0 ? (byte >> 4) & 7 : byte & 7;
                    u16 color = *(u16*)((u8*)res + res->mPaletteOffset + (paletteIndex & 0xFF) * 2);
                    if ((color & 0x8000) || (color & 0xF000) == 0x7000) {
                        hit = true;
                    }
                } else if (res->mLutFormat < 2 && res->mLutFormat >= 1) {
                    hit = true;
                }
            }
            if (hit) {
                fn_8022DF48(index);
                return true;
            }
        }
    }
    return false;
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
        d.set(mPos - *getPlayerPos_8023E724(i));
        s32 angle = JMath::atanTable_.atan2_(d.x, d.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        f32 len = JGeometry::TVec2f(d.x, d.y).length();
        s16 a = angle - 0x40;
        push.set(JGeometry::TVec3f(len * JMath::sincosTable_.cosShort(a), len * JMath::sincosTable_.sinShort(a), 0.0f) - d);
        fn_8023DFF4(i, &push);
    }
}

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
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', name);
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
    }
}

Lift_804A6F6C::~Lift_804A6F6C() {}
