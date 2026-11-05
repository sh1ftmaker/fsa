#include "game/GameAudio.h"
#include "game/act/act_view_mode.h"
#include "game/GameView.h"
#include "game/Player.h"

ViewMode_804A9A18::~ViewMode_804A9A18() {}

void ViewMode_804A9A18::draw() {}

void ViewMode_804A9A18::update() {
    s32 bit;
    if (field_0x90 & 2) {
        bool found = false;
        for (s32 i = 0; i < 4; i++) {
            if (field_0x4 == getPlayerLayer_8023DE58(i) && fn_802373EC(i) == 2) {
                found = true;
                break;
            }
        }
        if (getGameView_802D6A9C()->fn_802D800C(field_0x230) || found) {
            getGameView_802D6A9C()->fn_802D7790(field_0x4, 2);
        } else {
            getGameView_802D6A9C()->fn_802D77BC(field_0x4, 2);
        }
    }
    if (field_0x90 & 0xC) {
        bit = field_0x230;
        if (bit != 0) {
            if (getGameView_802D6A9C()->fn_802D800C(bit)) {
                getGameView_802D6A9C()->fn_802D77BC(field_0x4, field_0x90 & 0xC);
            } else {
                getGameView_802D6A9C()->fn_802D7790(field_0x4, 0xC);
            }
        }
    }
    if (field_0x90 & 0x3000) {
        bit = field_0x230;
        if (bit != 0) {
            if (getGameView_802D6A9C()->fn_802D800C(bit)) {
                getGameView_802D6A9C()->fn_802D77BC(field_0x4, field_0x90 & 0x3000);
            } else {
                getGameView_802D6A9C()->fn_802D7790(field_0x4, 0x3000);
            }
        }
    }
}

// FIXME: 99%. Only the epilogue is scheduled differently: the original
// reloads the saved LR (lwz r0, 0x14(r1)) before restoring r31, ours the
// other way round.
void ViewMode_804A9A18::init() {
    getGameView_802D6A9C()->fn_802D77E4(field_0x4, (u16)field_0x90);
    field_0x230 = field_0x90 >> 27;
}
