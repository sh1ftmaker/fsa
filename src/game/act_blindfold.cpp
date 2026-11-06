#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_blindfold.h"


static inline bool contains(const JGeometry::TBox2f& box, JGeometry::TVec3f p) {
    bool result = false;
    if (p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y) {
        result = true;
    }
    return result;
}

void Blindfold_804A8EF8::init() {
    field_0x234 = field_0x90 & 0xFF;
    field_0x238 = (field_0x90 >> 8 & 0xFF) + 1;
    field_0x23c = (field_0x90 >> 16 & 0xFF) + 1;
    if (field_0x90 & 0x1000000) {
        field_0x240 = true;
    } else {
        field_0x240 = false;
    }
    if (field_0x90 & 0x2000000) {
        field_0x241 = true;
    } else {
        field_0x241 = false;
    }
    f32 halfHeight = (field_0x23c * 8 >> 1) - 8.0f;
    f32 halfWidth = (field_0x238 * 8 >> 1) - 8.0f;
    mBox.i.x = -halfWidth;
    mBox.i.y = -halfHeight;
    mBox.f.x = halfWidth;
    mBox.f.y = halfHeight;
    if (field_0x90 & 0x4000000) {
        field_0x242 = true;
        field_0x230 = 4;
    } else {
        field_0x242 = false;
        field_0x230 = 0;
    }
    field_0x244 = 0xFF;
}

void Blindfold_804A8EF8::update() {
    JGeometry::TVec2<f32> offset(mPos.x, mPos.y);
    JGeometry::TBox2f box(mBox);
    box.addPos(offset);
    bool covered = false;
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        for (s32 i = 0; i < 4; i++) {
            bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i);
            // Both branches are the same: the original has two full copies of the body (separate stack slots).
            if (takesPart == true) {
                if (contains(box, *getPlayerPos_8023E724(fn_8022EA0C(i)))) {
                    covered = true;
                    break;
                }
            } else {
                if (contains(box, *getPlayerPos_8023E724(fn_8022EA0C(i)))) {
                    covered = true;
                    break;
                }
            }
        }
    } else {
        if (contains(box, *getPlayerPos_8023E724(fn_8022EA0C(0)))) {
            covered = true;
        }
    }
    switch (field_0x230) {
    case 0:
        if (covered) {
            field_0x230 = 1;
        }
        break;
    case 1:
        if (field_0x240) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x564, 0, 0, NULL, NULL, true, 0.5f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30003, 0);
            field_0x240 = false;
            mAlive = false;
        }
        if (field_0x241) {
            field_0x244 -= 12;
            if (field_0x244 < 0) {
                field_0x244 = 0;
                field_0x230 = 2;
            }
        }
        if (!covered) {
            field_0x230 = 3;
        }
        break;
    case 2:
        if (!covered) {
            field_0x230 = 3;
        }
        break;
    case 3:
        // The always-true <= 0xFF test on the u8 is in the original (cmplwi 0xff; bgt).
        if (field_0x241 <= 0xFF) {
            field_0x244 += 12;
            if (field_0x244 >= 0xFF) {
                field_0x244 = 0xFF;
                field_0x230 = 0;
            }
        }
        if (covered) {
            field_0x230 = 1;
        }
        break;
    case 4: // init sets it for bit 26; nothing leaves it
        break;
    }
}

// FIXME: 94%. Same instructions; the allocation differs: the original forms the high half of the
// "black.bti" address (lis) and loads the 0x23c conversion into r4 before the colour stores, with
// 0xff and 0 in r8/r7; here the string address comes after the colours and 0xff lands in r8 late.
void Blindfold_804A8EF8::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x242 == false) {
        params.field_0x1c = 1;
        params.field_0x1d = 1;
    } else {
        params.field_0x1c = -1;
        params.field_0x1d = 0;
    }
    f32 height = field_0x23c;
    mtx[0][0] = field_0x238;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = height;
    params.field_0x10.r = 0xFF;
    params.field_0x10.g = 0xFF;
    params.field_0x10.b = 0xFF;
    params.field_0x10.a = field_0x244;
    params.field_0x14.r = 0;
    params.field_0x14.g = 0;
    params.field_0x14.b = 0;
    params.field_0x14.a = field_0x244;
    params.mMtx = mtx;
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "black.bti");
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
    }
}

Blindfold_804A8EF8::~Blindfold_804A8EF8() {}
