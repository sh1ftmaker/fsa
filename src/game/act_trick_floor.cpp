#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "JSystem/JMath/JMath.h"
#include <math.h>
#include "game/act/act_trick_floor.h"

// Tile rows / columns the kind lays down at init (rodata, in this unit).
extern const s32 lbl_8046C8B0[7];
extern const s32 lbl_8046C8CC[7];
extern const f32 lbl_8046C908[7];
extern const f32 lbl_8046C924[7];
extern const f32 lbl_8046C940[7];
extern const f32 lbl_8046C95C[7];

// FIXME(shotgun): 78.18 %, missing/extra + reloc-name; behaviour drift.
void TrickFloor_804A83A0::init() {
    field_0x234 = field_0x90 & 0x7F;
    field_0x238 = (field_0x90 >> 7) & 0x1F;
    field_0x240 = (field_0x90 >> 12) & 0xFFF;
    field_0x23c = (field_0x90 >> 24) & 3;
    field_0x248 = 0.0f;
    switch (field_0x234) {
    case 6:
    default:
        if (field_0x23c == 1) {
            mBox.set(-20.0f, -20.0f, 20.0f, 20.0f);
        } else {
            mBox.set(-32.0f, -32.0f, 32.0f, 32.0f);
        }
        break;
    case 2:
        mBox.i.x = -40.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 32.0f;
        break;
    case 3:
        mBox.i.x = -24.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 32.0f;
        break;
    case 4:
    case 5:
        mBox.i.x = -16.0f;
        mBox.i.y = -50.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 50.0f;
        field_0x248 = -0.06f;
        break;
    }
    field_0x244 = 20;
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        field_0x252 = 12;
    } else {
        field_0x252 = 13;
    }
    if (field_0x234 >= 2) {
        f32 px = mPos.x;
        s32 layer = field_0x4;
        GameView_804A9950* gameView = getGameView_802D6A9C();
        const JGeometry::TBox2f* src = (layer < 8) ? &gameView->mVisibleBox : &gameView->field_0xdc;
        const JGeometry::TBox2f view = *src;
        for (s32 i = 0; i < lbl_8046C8B0[field_0x234]; i++) {
            f32 py = mPos.y;
            s32 tx = (s32)(px - view.i.x);
            for (s32 j = 0; j < lbl_8046C8CC[field_0x234]; j++) {
                s32 x = tx;
                s32 y = (s32)(py - view.i.y);
                if (x < 0) {
                    x = 0;
                } else if (x >= 0x200) {
                    x = 0x1FF;
                }
                if (y < 0) {
                    y = 0;
                } else if (y >= 0x200) {
                    y = 0x1FF;
                }
                u16 idx = (x & 0x100) | ((y << 1) & 0x200) | (y & 0xF0) | ((x >> 4) & 0xF);
                gGameRoot2_8053AB10->field_0x20->fn_802F9974(field_0x4, idx, 6);
                py += 16.0f;
            }
            px += 16.0f;
        }
    }
    field_0x230 = 0;
}

// FIXME(shotgun): 17.89 %, missing/extra + reloc-name; behaviour drift.
// States 2-8 are not written.
void TrickFloor_804A83A0::update() {
    JGeometry::TVec3f savedPos = mPos;
    switch (field_0x234) {
    case 2:
        mPos.x += 48.0f;
        mPos.y += 40.0f;
        break;
    case 3:
        mPos.x += 32.0f;
        mPos.y += 40.0f;
        break;
    case 4:
    case 5: {
        field_0x80.i.x = -48.0f;
        field_0x80.i.y = -48.0f;
        field_0x80.f.x = 48.0f;
        field_0x80.f.y = 48.0f;
        JGeometry::TBox2f box = field_0x80;
        mPos.x += 48.0f;
        mPos.y += 48.0f;
        box.addPos(mPos.x, mPos.y);
        fn_80239914(&box, field_0x4, mSerial);
        mPos = savedPos;
        mPos.y += 48.0f;
        mBox.i.x = -16.0f;
        mBox.i.y = -50.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 50.0f;
        break;
    }
    default:
        mPos.x += 32.0f;
        mPos.y += 32.0f;
        break;
    }

    JGeometry::TBox2f box = mBox;
    box.addPos(mPos.x, mPos.y);
    s32 standing = -1;
    for (s32 i = 0; i < 4; i++) {
        const JGeometry::TVec3f* p = getPlayerPos_8023E724(i);
        if (p->x >= box.i.x && p->y >= box.i.y && p->x < box.f.x && p->y < box.f.y) {
            standing = i;
            if (!fn_8022F478(i)) {
                break;
            }
            standing = -1;
        }
    }
    if (field_0x23c == 1 && standing >= 0 && !fn_80234F68(standing)) {
        standing = -1;
    }
    s32 ball = fn_801F2718('SBAL');
    JGeometry::TVec2f hit;
    fn_8023A89C(&box, field_0x4, &hit);
    s32 pushers = 0;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023A97C(i, &box, field_0x4, &hit)) {
            pushers++;
        }
    }
    mPos = savedPos;

    switch (field_0x230) {
    case 0: {
        f32 px = mPos.x;
        f32 py = mPos.y;
        s32 layer = field_0x4;
        const JGeometry::TBox2f view = (layer < 8) ? getGameView_802D6A9C()->mVisibleBox : getGameView_802D6A9C()->field_0xdc;
        s32 x = (s32)(px - view.i.x);
        s32 y = (s32)(py - view.i.y);
        if (x < 0) {
            x = 0;
        } else if (x >= 0x200) {
            x = 0x1FF;
        }
        if (y < 0) {
            y = 0;
        } else if (y >= 0x200) {
            y = 0x1FF;
        }
        u16 idx = (x & 0x100) | ((y << 1) & 0x200) | (y & 0xF0) | ((x >> 4) & 0xF);
        field_0x250 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, idx);
        field_0x230 = 1;
        break;
    }
    case 1:
        if (field_0x238 != 0) {
            if (getGameView_802D6A9C()->fn_802D800C(field_0x238)) {
                field_0x230 = 2;
            }
        } else {
            field_0x230 = 2;
        }
        if (field_0x234 == 4 || field_0x234 == 5) {
            field_0x230 = 2;
            break;
        }
        if (field_0x230 == 2 && field_0x234 >= 2) {
            field_0x230 = 3;
            startSound_801F0E34(0x3001B, 0);
        }
        break;
    // FIXME(shotgun): states 2-8 not written
    }
}

static inline void setRotMtx(Mtx mtx, f32 rx, f32 ry, f32 rz) {
    f32 cz = cos(rz);
    f32 cy = cos(ry);
    f32 cx = cos(rx);
    f32 sz = sin(rz);
    f32 sy = sin(ry);
    f32 sx = sin(rx);
    mtx[0][0] = cy * cz;
    mtx[1][0] = cy * sz;
    f32 cxcz = cx * cz;
    f32 sx_sy = sx * sy;
    f32 cxsz = cx * sz;
    f32 sxsz = sx * sz;
    f32 sxcz = sx * cz;
    mtx[2][0] = -sy;
    mtx[0][1] = sx_sy * cz - cxsz;
    mtx[1][1] = sx_sy * sz + cxcz;
    mtx[2][1] = sx * cy;
    mtx[0][2] = cxcz * sy + sxsz;
    mtx[1][2] = cxsz * sy - sxcz;
    mtx[2][2] = cx * cy;
}

// FIXME(shotgun): 78.29 %, missing/extra + reloc-name; behaviour drift.
void TrickFloor_804A83A0::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;

    params.field_0x1c = -1;
    params.field_0x1d = -2;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][0] = lbl_8046C940[field_0x234];
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = lbl_8046C95C[field_0x234];
    params.mMtx = mtx;

    void* bti = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "otosiana_bg_s.bti");
    JGeometry::TVec3f bgPos(mPos.x + lbl_8046C908[field_0x234], mPos.y + lbl_8046C924[field_0x234], mPos.z);
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&bgPos, bti, &params);
    }

    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    setRotMtx(mtx, 0.0f, field_0x248, 0.0f);
    params.field_0x1c = -1;
    params.field_0x1d = -1;
    params.field_0x21 = 1;
    params.mMtx = mtx;

    switch (field_0x234) {
    case 1:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "mastersword_left_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&mPos, res, &params);
        }
        break;
    case 2:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "house_left_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&mPos, res, &params);
        }
        break;
    case 3:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "tree0_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&mPos, res, &params);
        }
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "tree1_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&mPos, res, &params);
        }
        break;
    case 4:
    case 5:
        break;
    case 6:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "field_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&mPos, res, &params);
        }
        break;
    default:
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "floor_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&mPos, res, &params);
        }
        break;
    }

    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x234 == 3 || field_0x234 == 4 || field_0x234 == 5) {
        setRotMtx(mtx, 0.0f, 3.1415927f - field_0x248, 0.0f);
    } else {
        setRotMtx(mtx, 0.0f, -field_0x248, 0.0f);
    }
    params.mMtx = mtx;

    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    switch (field_0x234) {
    case 1: {
        f32 s = sincos.sinRadian(field_0x248);
        f32 a = -field_0x248;
        if (a < 0.0f) {
            a = -a;
        }
        f32 c = sincos.mTable[(u16)(a * 325.9493f) & 0x7FF].second;
        JGeometry::TVec3f pos((mPos.x + 64.0f) - 32.0f * c, 32.0f * s + mPos.y, mPos.z);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "mastersword_right_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos, res, &params);
        }
        break;
    }
    case 2: {
        f32 s = sincos.sinRadian(field_0x248);
        f32 a = -field_0x248;
        if (a < 0.0f) {
            a = -a;
        }
        f32 c = sincos.mTable[(u16)(a * 325.9493f) & 0x7FF].second;
        JGeometry::TVec3f pos((mPos.x + 96.0f) - 48.0f * c, 48.0f * s + mPos.y, mPos.z);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "house_right_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos, res, &params);
        }
        break;
    }
    case 3: {
        JGeometry::TVec3f pos0(mPos.x + 64.0f, mPos.y, mPos.z);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "tree0_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos0, res, &params);
        }
        JGeometry::TVec3f pos1(mPos.x + 64.0f, mPos.y, mPos.z);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "tree1_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos1, res, &params);
        }
        break;
    }
    case 4:
    case 5:
        params.field_0x1c = 1;
        params.field_0x1d = -1;
        if (field_0x230 == 7) {
            JGeometry::TVec3f pos(mPos.x + 96.0f, mPos.y, mPos.z);
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "jimen_smp.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos, res, &params);
            }
        } else if (field_0x230 != 8) {
            JGeometry::TVec3f pos(mPos.x + 96.0f, mPos.y, mPos.z);
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "house_smp.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos, res, &params);
            }
        }
        break;
    case 6: {
        f32 s = sincos.sinRadian(field_0x248);
        f32 a = -field_0x248;
        if (a < 0.0f) {
            a = -a;
        }
        f32 c = sincos.mTable[(u16)(a * 325.9493f) & 0x7FF].second;
        JGeometry::TVec3f pos((mPos.x + 64.0f) - 32.0f * c, 32.0f * s + mPos.y, mPos.z);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "field_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos, res, &params);
        }
        break;
    }
    default: {
        f32 s = sincos.sinRadian(field_0x248);
        f32 a = -field_0x248;
        if (a < 0.0f) {
            a = -a;
        }
        f32 c = sincos.mTable[(u16)(a * 325.9493f) & 0x7FF].second;
        JGeometry::TVec3f pos((mPos.x + 64.0f) - 32.0f * c, 32.0f * s + mPos.y, mPos.z);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "floor_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&pos, res, &params);
        }
        break;
    }
    }
}

TrickFloor_804A83A0::~TrickFloor_804A83A0() {}
