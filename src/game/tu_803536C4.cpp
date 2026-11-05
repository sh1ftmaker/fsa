#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/act/tu_803536C4.h"

static const s32 lbl_80472240[8] = {1, 5, 9, 13, 17, 21, 25, 29};
static const s32 lbl_80472260[4] = {3, 4, 5, 4};

cls_804ACC68::~cls_804ACC68() {}

void cls_804ACC68::draw() {
    // The original copies mPos to 0x14(r1) and never reads it back (a dead local).
    JGeometry::TVec3f pos(mPos);
    SpriteParams_801F5AE0 params;
    params.field_0x2d = 1;
    params.field_0x1c = 2;
    params.field_0x1d = 1;
    if (field_0x24c < 8) {
        JGeometry::TVec3f p(mPos);
        p.x -= 2.5f;
        switch (field_0x24c) {
        case 2:
        case 3:
            p.y += 3.0f;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            p.y += 13.0f;
            break;
        }
        switch (field_0x23c) {
        case 1:
        case 3:
            p.y += 1.5f;
            break;
        case 2:
            p.y += 2.5f;
            break;
        }
        s32 frame = lbl_80472240[field_0x24c] + field_0x244;
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&p, res, frame, &params, 1.0f, 1.0f);
        }
    }
    switch (field_0x248) {
    case 0:
        params.field_0x0[2] = 2;
        break;
    case 1:
        params.field_0x0[2] = 3;
        break;
    case 2:
        params.field_0x0[2] = 4;
        break;
    case 3:
        params.field_0x0[2] = 9;
        break;
    }
    s32 frame = lbl_80472260[field_0x23c] + 0xC0;
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
}

void cls_804ACC68::update() {
    mPos += field_0x3c;
    if (field_0x254 - 32.0f > mPos.x) {
        fn_801F3668();
    }
    field_0x234--;
    field_0x238--;
    if (field_0x234 <= 0) {
        field_0x23c++;
        field_0x23c &= 3;
        field_0x234 = 9;
    }
    field_0x240--;
    if (field_0x240 <= 0) {
        field_0x244++;
        field_0x244 &= 3;
        if (field_0x244 == 0) {
            field_0x240 = (s32)(60.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 60;
        } else {
            field_0x240 = 2;
        }
    }
}

void cls_804ACC68::init() {
    field_0x23c = 0;
    field_0x234 = 9;
    field_0x238 = 300;
    field_0x248 = field_0x90 & 3;
    field_0x240 = 60;
    field_0x244 = 0;
    field_0x24c = field_0x90 >> 4 & 0xF;
    field_0x250 = (field_0x90 >> 8 & 3) != 0;
    if (!field_0x250) {
        startSound_801F0E34(0x50050, 0);
    } else {
        startSound_801F0E34(0x50055, 0);
    }
    field_0x230 = 0;
}
