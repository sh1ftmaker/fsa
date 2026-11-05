#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/tu_80441318.h"
#include "JSystem/JUtility/JUTVideo.h"

static inline s32 centreX() {
    return (JUTVideo::getManager()->getRenderMode()->fbWidth - 365) / 2;
}

static inline s32 centreY() {
    return (JUTVideo::getManager()->getRenderMode()->efbHeight - 261) / 2;
}

// Opens window `index` of the map object, 365 x 261 at (x, y).
static inline void setWindow(s32 index, s32 flag, s32 x, s32 y) {
    TileMap_804AA858* map = lbl_8053AB10->field_0x20;
    map->field_0x22c = index;
    map->field_0x238 = x;
    map->field_0x23c = y;
    map->field_0x240 = 365;
    map->field_0x244 = 261;
    map->field_0x234 = flag;
}

static inline void setWindow(s32 index, s32 flag) {
    s32 x = centreX();
    s32 y = centreY();
    setWindow(index, flag, x, y);
}

// FIXME: 98.2 %. Only the registers of the centred-window arithmetic differ
// (setWindow's two-argument form, inlined at the end of this function):
// the target loads the render-mode pointer into the register that held
// JUTVideo::sManager (lwz r6, 0x4(r6)) and keeps the map object in r8,
// ours uses r7 and r10, so the temporaries (r5-r8) rotate.
void cls_804B19E0::fn_80441318() {
    s32 player = fn_80231A50();
    if (player < 0) {
        if (field_0x8 < 3) {
            field_0x8++;
        } else {
            field_0x4 = player;
        }
    } else {
        field_0x4 = fn_8022F340(player);
        field_0x8 = 0;
    }
    if (field_0x4 >= 0) {
        setWindow(field_0x4, 0);
    } else {
        mState = 1;
    }
}

void cls_804B19E0::fn_804413F0() {
    if (mTimer < 300) {
        mTimer++;
    } else if (fn_80231A50() >= 0) {
        mState = 2;
    }
}

void cls_804B19E0::fn_80441444() {
    if (!getGameView_802D6A9C()->fn_802D39CC()) {
        mState = 1;
        mTimer = 0;
    }
}

void cls_804B19E0::fn_8044148C() {}

static void (cls_804B19E0::*lbl_804B19BC[3])() = {
    &cls_804B19E0::fn_80441444,
    &cls_804B19E0::fn_804413F0,
    &cls_804B19E0::fn_80441318,
};

// FIXME: 99.2 %. Same register rotation as fn_80441318, in the last
// setWindow (lbl_8053A160 branch): the render-mode pointer and the map
// object are r7 / r10 here, r6 / r8 in the target.
void cls_804B19E0::fn_80441490() {
    if (lbl_8053AAF8->field_0x7f == 0) {
        field_0x4 = -1;
        s32 count = 0;
        s32 x = centreX();
        s32 y = centreY();
        s32 player = lbl_80529DEC.field_0x1a8;
        if (player >= 0 && player < 4) {
            if (fn_8023DE58(player) >= 8) {
                field_0x4 = lbl_80529DEC.field_0x1a8;
            }
        } else if (player == 4) {
            for (s32 i = 0; i < 4; i++) {
                if (lbl_8053AAF8->field_0x82 & (u8)(1 << i)) {
                    if (fn_8023DE58(i) >= 8) {
                        count++;
                    }
                }
            }
            if (count == lbl_8053AAF8->fn_80138A30()) {
                field_0x4 = 0;
            }
        }
        if (field_0x4 >= 0) {
            setWindow(field_0x4, 0, x, y);
        }
    } else if (lbl_8053AD78->field_0xed != 0) {
        s32 x = centreX();
        s32 y = centreY();
        field_0x4 = lbl_8053AD78->field_0x110;
        if (field_0x4 >= 0) {
            setWindow(field_0x4, lbl_8053AD78->field_0xee, x, y);
        }
    } else if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 2) {
        field_0x4 = lbl_8053A160;
        if (field_0x4 >= 0) {
            setWindow(field_0x4, 0);
        }
    } else {
        if (getGameView_802D6A9C()->fn_802DCD0C() == true) {
            mState = 0;
        }
        (this->*lbl_804B19BC[mState])();
    }
}

void cls_804B19E0::fn_8044172C() {}

cls_804B19E0::~cls_804B19E0() {}

cls_804B19E0::cls_804B19E0() {
    mTimer = 0;
    mState = 0;
    field_0xf = 0;
    field_0x4 = -1;
    field_0x8 = 0;
}
