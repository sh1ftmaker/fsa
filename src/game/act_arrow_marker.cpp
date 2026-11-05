#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Camera.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_arrow_marker.h"

static const s32 lbl_80540968[2] = {4, -4};

ArrowMarker_804A8470::~ArrowMarker_804A8470() {}

void ArrowMarker_804A8470::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -3;
    switch (field_0x244) {
    case 0: {
        params.field_0x10.mColor.a = 0x60;
        s32 frame = field_0x240 + 0x10;
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x240 + 0x10, 0, 0, -1);
        break;
    }
    case 1:
    case 3: {
        if (field_0x250 == 0) {
            break;
        }
        params.field_0x10.mColor.a = field_0x254;
        s32 frame = field_0x240 + 0x10;
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x240 + 0x10, 0, 0, -1);
        break;
    }
    }
}

void ArrowMarker_804A8470::update() {
    JGeometry::TVec2<f32> p(field_0x234, field_0x238);
    JGeometry::TBox2f box(mBox);
    box.addPos(p);
    s32 player = fn_8023AF14(&box, field_0x4, false, 0.0f);
    switch (field_0x244) {
    case 0:
        field_0x24c = field_0x24c - 1;
        if (field_0x24c == 0) {
            field_0x24c = 8;
            field_0x250 = (field_0x250 + 1) & 3;
        }
        break;
    case 1:
        switch (field_0x230) {
        case 0:
            if (vfunc_0x34()) {
                field_0x250 = 1;
                field_0x254 = 0x14;
                field_0x258 = 0;
                startSound_801F0E34(0x30025, 0);
                field_0x230 = 1;
                if (field_0x248 == 0) {
                    lbl_8053AED0->fn_8043D67C(0x199, 0, 0);
                }
            } else if (player >= 0) {
                field_0x25c = 1;
                vfunc_0x30(0xFF, true);
            }
            break;
        case 1:
            field_0x254 = field_0x254 + lbl_80540968[field_0x258];
            lbl_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
            // The original copies mPos into a named local before the by-value call.
            JGeometry::TVec3f pos = mPos;
            lbl_8053AED0->fn_8043E7CC(pos);
            if (vfunc_0x34()) {
                if (field_0x258 == 0) {
                    if (field_0x254 >= 0xC8) {
                        field_0x254 = 0xC8;
                        field_0x258 = 1;
                    }
                } else if (field_0x254 <= 0x28) {
                    field_0x254 = 0x28;
                    field_0x258 = 0;
                    startSound_801F0E34(0x30025, 0);
                }
                if (player < 0) {
                    if (field_0x25c != 0) {
                        vfunc_0x30(0xFF, false);
                    }
                    field_0x25c = 0;
                } else {
                    field_0x25c = 1;
                    vfunc_0x30(0xFF, true);
                }
            } else {
                field_0x250 = 0;
                field_0x25c = 0;
                field_0x230 = 0;
            }
            break;
        }
        break;
    case 2:
        switch (field_0x230) {
        case 0:
            if (player >= 0) {
                field_0x230 = 1;
                fn_801F0D20(6);
            }
            break;
        case 1:
            if (player < 0) {
                field_0x230 = 0;
            }
            break;
        }
        break;
    case 3:
        switch (field_0x230) {
        case 0:
            if (vfunc_0x34()) {
                field_0x250 = 1;
                field_0x254 = 0x14;
                field_0x258 = 0;
                startSound_801F0E34(0x30025, 0);
                field_0x230 = 1;
                if (field_0x248 == 0) {
                    lbl_8053AED0->fn_8043D67C(0x199, 0, 0);
                }
            }
            break;
        case 1:
            field_0x254 = field_0x254 + lbl_80540968[field_0x258];
            lbl_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
            JGeometry::TVec3f pos = mPos;
            lbl_8053AED0->fn_8043E7CC(pos);
            if (vfunc_0x34()) {
                if (field_0x258 == 0) {
                    if (field_0x254 >= 0xC8) {
                        field_0x254 = 0xC8;
                        field_0x258 = 1;
                    }
                } else if (field_0x254 <= 0x28) {
                    field_0x254 = 0x28;
                    field_0x258 = 0;
                    startSound_801F0E34(0x30025, 0);
                }
            } else {
                field_0x250 = 0;
                field_0x25c = 0;
                field_0x230 = 0;
            }
            break;
        }
        break;
    }
}

void ArrowMarker_804A8470::init() {
    field_0x240 = field_0x90 & 0xF;
    field_0x244 = (field_0x90 >> 7) & 0x1F;
    field_0x248 = (field_0x90 >> 4) & 1;
    field_0x230 = 0;
    field_0x250 = 0;
    field_0x24c = 8;
    field_0x254 = 0x28;
    field_0x258 = 0;
    field_0x25c = 0;

    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f& box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
    field_0x23c = mPos.z;
    // Modes 0 and 1 and modes 2 and up store the same values, as separate blocks in the
    // original; `case 3` folds into the default block, which is how the original branches.
    switch (field_0x240) {
    case 0:
        field_0x234 = mPos.x;
        field_0x238 = 192.0f + box.i.y;
        mBox.i.x = -32.0f;
        mBox.i.y = -256.0f;
        mBox.f.x = 32.0f;
        mBox.f.y = 256.0f;
        break;
    case 1:
        field_0x234 = mPos.x;
        field_0x238 = 192.0f + box.i.y;
        mBox.i.x = -32.0f;
        mBox.i.y = -256.0f;
        mBox.f.x = 32.0f;
        mBox.f.y = 256.0f;
        break;
    case 2:
        field_0x234 = 256.0f + box.i.x;
        field_0x238 = mPos.y;
        mBox.i.x = -256.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 256.0f;
        mBox.f.y = 32.0f;
        break;
    case 3:
    default:
        field_0x234 = 256.0f + box.i.x;
        field_0x238 = mPos.y;
        mBox.i.x = -256.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 256.0f;
        mBox.f.y = 32.0f;
        break;
    }
}
