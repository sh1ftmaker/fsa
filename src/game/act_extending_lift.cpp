#include "game/GameAudio.h"
#include "game/act/act_extending_lift.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/ResTIMG.h"

static const char* lbl_8053A198[] = {"lift_00.bti", NULL};

static inline void setRotScaleMtxS16(Mtx m, s16 x, s16 y, s16 z, f32 scaleX, f32 scaleY, f32 scaleZ) {
    f32 sx = JMath::sincosTable_.sinShort(x);
    f32 sy = JMath::sincosTable_.sinShort(y);
    f32 sz = JMath::sincosTable_.sinShort(z);
    f32 cx = JMath::sincosTable_.cosShort(x);
    f32 cy = JMath::sincosTable_.cosShort(y);
    f32 cz = JMath::sincosTable_.cosShort(z);
    m[0][0] = scaleX * (cy * cz);
    m[1][0] = scaleX * (cy * sz);
    m[2][0] = scaleX * -sy;
    m[0][1] = scaleY * (sx * sy * cz - cx * sz);
    m[1][1] = scaleY * (sx * sy * sz + cx * cz);
    m[2][1] = scaleY * (sx * cy);
    m[0][2] = scaleZ * (cx * cz * sy + sx * sz);
    m[1][2] = scaleZ * (cx * sz * sy - sx * cz);
    m[2][2] = scaleZ * (cx * cy);
}

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

inline bool ExtendingLift_804A78B0::isPlayerOn(s32 player) {
    if (field_0x4 != getPlayerLayer_8023DE58(player)) {
        return false;
    }
    for (s32 j = 0; j < 8; j++) {
        JGeometry::TVec3f pt;
        fn_8022E088(player, j, &pt);
        pt -= mPos;
        pt.z = 0.0f;
        if (contains(mBox, pt)) {
            fn_8022DF48(player);
            return true;
        }
    }
    return false;
}

// FIXME: 88.87%, missing/extra: ours has 1 fewer instrs (106 vs 107). The flags test loads 0x90 into r3 and reuses it; the original loads it into r0 for the test and reloads 0x90 (r31) for the oris.
void ExtendingLift_804A78B0::init() {
    if (!((field_0x90 >> 7) & 0x1F)) {
        field_0x90 |= 0x80;
    }
    if (!((field_0x90 >> 17) & 0x1F)) {
        field_0x90 |= 0x20000;
    }

    field_0x240 = static_cast<const ResTIMG*>(gGameRoot_8053AAF8->field_0xa4->getResource('????', lbl_8053A198[0]));
    mLife = 0;

    switch ((field_0x90 >> 12) & 0x1F) {
    case 0:
        break;
    case 1:
        mPos.x += 16.0f;
        break;
    case 2:
        break;
    case 3:
        mPos.y += 16.0f;
        break;
    }

    if ((s32)((field_0x90 >> 12) & 0x1F) < 2) {
        field_0x23c = 16.0f * (s32)((field_0x90 >> 7) & 0x1F);
        field_0x238 = 0.0f;
    } else {
        field_0x238 = 16.0f * (s32)((field_0x90 >> 7) & 0x1F);
        field_0x23c = 0.0f;
    }

    u16 sizeX = field_0x240->mSizeX;
    u16 sizeY = field_0x240->mSizeY;
    field_0x230 = field_0x238 / sizeX;
    field_0x234 = field_0x23c / sizeY;
    field_0x244 = 0;
}

// FIXME: 90.34%, missing/extra: ours has 5 more instrs (318 vs 313). In player collision loop, player induction variable uses extra register r28.
void ExtendingLift_804A78B0::update() {
    if (vfunc_0x34()) {
        if (!field_0x244) {
            startSound_801F0E34(0x30041, 0);
        }
        field_0x244 = 1;
    } else {
        if (!(field_0x90 & 0x7F)) {
            if (field_0x244 == 1) {
                startSound_801F0E34(0x30041, 0);
            }
            field_0x244 = 0;
        }
    }

    if (field_0x244) {
        if ((s32)((field_0x90 >> 12) & 0x1F) < 2) {
            field_0x238 += 1.0f;
            if (field_0x238 >= 16 * (s32)((field_0x90 >> 17) & 0x1F)) {
                field_0x238 = 16 * (s32)((field_0x90 >> 17) & 0x1F);
            }
        } else {
            field_0x23c += 1.0f;
            if (field_0x23c >= 16 * (s32)((field_0x90 >> 17) & 0x1F)) {
                field_0x23c = 16 * (s32)((field_0x90 >> 17) & 0x1F);
            }
        }
    } else {
        if ((s32)((field_0x90 >> 12) & 0x1F) < 2) {
            field_0x238 -= 1.0f;
            if (field_0x238 <= 0.0f) {
                field_0x238 = 0.0f;
            }
        } else {
            field_0x23c -= 1.0f;
            if (field_0x23c <= 0.0f) {
                field_0x23c = 0.0f;
            }
        }
    }

    JGeometry::TVec3f oldCenter((mBox.i.x - mBox.f.x) * 0.5f + mBox.f.x,
                                (mBox.i.y - mBox.f.y) * 0.5f + mBox.f.y,
                                0.0f);

    switch ((field_0x90 >> 12) & 0x1F) {
    case 0:
        mBox.set(0.0f, 3.0f, field_0x238, field_0x23c - 3.0f);
        break;
    case 1:
        mBox.set(-field_0x238, 3.0f, 0.0f, field_0x23c - 3.0f);
        break;
    case 2:
        mBox.set(3.0f, 0.0f, field_0x238 - 3.0f, field_0x23c);
        break;
    case 3:
        mBox.set(3.0f, -field_0x23c, field_0x238 - 3.0f, 0.0f);
        break;
    }

    u16 sizeX = field_0x240->mSizeX;
    u16 sizeY = field_0x240->mSizeY;
    field_0x230 = field_0x238 / sizeX;
    field_0x234 = field_0x23c / sizeY;

    JGeometry::TVec3f newCenter((mBox.i.x - mBox.f.x) * 0.5f + mBox.f.x,
                                (mBox.i.y - mBox.f.y) * 0.5f + mBox.f.y,
                                0.0f);

    for (int i = 0; i < 4; i++) {
        if (isPlayerOn(i)) {
            if (!(newCenter - oldCenter).isZero()) {
                JGeometry::TVec3f delta = newCenter - oldCenter;
                fn_8023DF68(i, &delta);
            }
        }
    }
}

// FIXME: 95.52%, scheduling+regalloc: instruction scheduling and register allocation in setRotScaleMtxS16 and sprite setup.
void ExtendingLift_804A78B0::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -1;

    Mtx mtx;
    switch ((field_0x90 >> 12) & 0x1F) {
    case 0:
        PSMTXTrans(mtx,
                   0.5f * (field_0x240->mSizeX * field_0x230),
                   0.0f,
                   0.5f * (-(s32)field_0x240->mSizeY * field_0x234));
        break;
    case 1:
        PSMTXTrans(mtx,
                   0.5f * (-(s32)field_0x240->mSizeX * field_0x230),
                   0.0f,
                   0.5f * (-(s32)field_0x240->mSizeY * field_0x234));
        break;
    case 2:
        PSMTXTrans(mtx,
                   0.5f * (field_0x240->mSizeX * field_0x230),
                   0.0f,
                   0.5f * (-(s32)field_0x240->mSizeY * field_0x234));
        break;
    case 3:
        PSMTXTrans(mtx,
                   0.5f * (field_0x240->mSizeX * field_0x230),
                   0.0f,
                   0.5f * (field_0x240->mSizeY * field_0x234));
        break;
    default:
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        break;
    }

    setRotScaleMtxS16(mtx, 0, 0, 0, field_0x230, 1.0f, field_0x234);

    params.mMtx = mtx;
    params.field_0x21 = 1;
    params.field_0x1d = -2;

    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_8053A198[0]);
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
    }
}

ExtendingLift_804A78B0::~ExtendingLift_804A78B0() {}
