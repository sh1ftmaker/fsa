#include "game/GameAudio.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_gravestone.h"

// Tile codes written by update case 2, four per field_0x234 (0x8046E9B0).
static const u16 lbl_8046E9B0[2][4] = {
    {0x276, 0x279, 0x275, 0x27A},
    {0xDF, 0xE0, 0xE1, 0xE2},
};

void Gravestone_804A99C0::init() {
    mBox.i.x = -16.0f;
    mBox.i.y = -16.0f;
    mBox.f.x = 16.0f;
    mBox.f.y = 0.0f;
    field_0x234 = field_0x90 & 0x3F;
    field_0x238 = -1;
    field_0x23c = 0.0f;
    field_0x230 = 0;
}

void Gravestone_804A99C0::update() {
    JGeometry::TVec2<f32> p;
    if (field_0x230 < 2) {
        p.set(16.0f + mPos.x, 16.0f + mPos.y);
    } else {
        p.set(mPos.x, mPos.y);
    }
    JGeometry::TBox2f box(mBox);
    box.addPos(p);
    fn_80239914(&box, field_0x4, mSerial);
    s32 hit = fn_80239558(&box, field_0x4, mSerial);
    switch (field_0x230) {
    case 0: {
        s32 player = fn_8023AF14(&box, field_0x4, false, 0.0f);
        if (hit >= 0 && fn_80230CFC(hit) == 3) {
            field_0x230 = 1;
            field_0x238 = hit;
            field_0x23c = 0.0f;
        } else if (player >= 0 && fn_8023513C(player) && fn_80230CFC(player) == 3) {
            field_0x238 = player;
            field_0x23c = 0.0f;
            field_0x230 = 2;
            field_0x240 = mPos;
            mPos.x += 16.0f;
            mPos.y += 16.0f;
            mBox.i.x = -16.0f;
            mBox.i.y = -16.0f;
            mBox.f.x = 16.0f;
            mBox.f.y = 0.0f;
        }
        break;
    }
    case 1:
        if (hit >= 0 && field_0x238 == hit && fn_80230CFC(hit) == 3) {
            field_0x23c += 1.0f;
            if (field_0x23c >= 25.0f) {
                field_0x230 = 2;
                field_0x240 = mPos;
                mPos.x += 16.0f;
                mPos.y += 16.0f;
                mBox.i.x = -16.0f;
                mBox.i.y = -16.0f;
                mBox.f.x = 16.0f;
                mBox.f.y = 0.0f;
            }
        } else if (field_0x238 != hit) {
            field_0x238 = -1;
            field_0x230 = 0;
        }
        break;
    case 2: {
        gBgRewrite_8053AC98->fn_8029E050(&field_0x240, field_0x4, lbl_8046E9B0[field_0x234][0],
                                  lbl_8046E9B0[field_0x234][1], lbl_8046E9B0[field_0x234][2],
                                  lbl_8046E9B0[field_0x234][3]);
        field_0x230 = 3;
        field_0x23c = 0.0f;
        break;
    }
    case 3:
        if (((s32)field_0x23c & 3) == 0) {
            startSound_801F0E34(0x3000E, 0);
        }
        field_0x23c += 0.5;
        mPos.y -= 0.5;
        if (field_0x23c >= 16.0f) {
            field_0x240.y = mPos.y - 16.0f;
            gBgRewrite_8053AC98->fn_802A3948(&field_0x240, field_0x238, field_0x4, 0x266);
            field_0x240.x += 16.0f;
            gBgRewrite_8053AC98->fn_802A3948(&field_0x240, field_0x238, field_0x4, 0x269);
            field_0x230 = 4;
        }
        break;
    case 4:
        if (field_0x234 == 1) {
            fn_801F0D20(3);
        }
        mAlive = false;
        break;
    }
}

void Gravestone_804A99C0::draw() {
    if (field_0x230 >= 2) {
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x16d, NULL, 7, 1.0f, 1.0f);
    }
}

Gravestone_804A99C0::~Gravestone_804A99C0() {}
