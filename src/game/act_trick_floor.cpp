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

// FIXME(shotgun): 77.37 %, missing/extra + reloc-name
void TrickFloor_804A83A0::init() {
    field_0x234 = field_0x90 & 0x7F;
    field_0x238 = (field_0x90 >> 7) & 0x1F;
    field_0x240 = (field_0x90 >> 12) & 0xFFF;
    field_0x23c = (field_0x90 >> 24) & 3;
    field_0x248 = 0.0f;
    switch (field_0x234) {
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
    default:
        if (field_0x23c == 1) {
            mBox.i.x = -20.0f;
            mBox.i.y = -20.0f;
            mBox.f.x = 20.0f;
            mBox.f.y = 20.0f;
        } else {
            mBox.i.x = -32.0f;
            mBox.i.y = -32.0f;
            mBox.f.x = 32.0f;
            mBox.f.y = 32.0f;
        }
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
        const JGeometry::TBox2f view = (layer < 8) ? getGameView_802D6A9C()->mVisibleBox : getGameView_802D6A9C()->field_0xdc;
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

bool fn_80234F68(s32 player);

// FIXME(shotgun): 16.59 %, missing/extra. States 3-8 (the tile rewrite and the fall) are not written.
void TrickFloor_804A83A0::update() {
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
            if (fn_8022F478(i)) {
                standing = -1;
            }
            break;
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
    }
}

// FIXME(shotgun): not written (size 0xD14): draw (0x802ACF3C); a first attempt did not follow the asm per kind and was removed

TrickFloor_804A83A0::~TrickFloor_804A83A0() {}
