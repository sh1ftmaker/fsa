#include "game/GameAudio.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_item_pedestal.h"

// 0x802AC2F4.
void ItemPedestal_804A82A8::init() {
    field_0x234 = (field_0x90 >> 12) & 0xF;
    field_0x23c = (field_0x90 >> 4) & 1;
    field_0x230 = 0;
    field_0x238 = 0x28;
    field_0x240 = field_0x234;
    if (fn_8022F514(field_0x234)) {
        field_0x240 = fn_8022F3DC(field_0x234);
    }
}

// 0x802AC0AC.
void ItemPedestal_804A82A8::update() {
    switch (field_0x230) {
    case 0:
        if (field_0x238 == 0) {
            if (fn_80388EEC(field_0x234)) {
                u32 param = field_0x90;
                s32 cell = param >> 16;
                s32 offset = (param >> 8) & 0xF;
                switch (param & 0xF) {
                case 1:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0x18, 0x28);
                    break;
                case 2:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0x19, 0x29);
                    break;
                case 3:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0x1A, 0x2A);
                    break;
                case 4:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0x1B, 0x2B);
                    break;
                case 5:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0x1C, 0x2C);
                    break;
                case 6:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0x1D, 0x2D);
                    break;
                case 7:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0x1E, 0x2E);
                    break;
                case 8:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0x1F, 0x2F);
                    break;
                case 9:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0xA0, 0xB0);
                    break;
                case 10:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0xA1, 0xB1);
                    break;
                case 11:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0xA2, 0xB2);
                    break;
                case 12:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0xA3, 0xB3);
                    break;
                case 13:
                    gBgRewrite_8053AC98->fn_8029EC84(cell, field_0x4, offset, 0xFFF, 0x100F);
                    break;
                }
                if (field_0x23c != 0) {
                    field_0x230 = 1;
                } else {
                    mAlive = false;
                }
            }
        } else if (field_0x238 > 0) {
            field_0x238--;
        }
        break;
    case 1:
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            if (fn_8023061C(field_0x240, false) == true) {
                mAlive = false;
            }
        } else if (fn_80230534(false) == true) {
            mAlive = false;
        }
        break;
    }
}

// 0x802AC0A8.
void ItemPedestal_804A82A8::draw() {}

// 0x802AC09C.
void ItemPedestal_804A82A8::fn_802AC09C() {
    field_0x238 = 0;
}

// Destructor at 0x802AC03C.
ItemPedestal_804A82A8::~ItemPedestal_804A82A8() {}
