#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/Camera.h"
#include "game/act/tu_802F4AEC.h"

void UnknownGSCK_804AA800::init() {
    field_0x230 = 0;
    update();
}

void UnknownGSCK_804AA800::update() {
    // FIXME: 99.34%. Registers only: the original keeps bit3 (field_0x90 >> 7) in r28,
    // bit1 in r30 and bit2 in r27; here bit3 gets r30, bit1 r28, and the player
    // loop counters get r27 (original r28/r30).
    u32 args = field_0x90;
    u32 bit3 = (args >> 7) & 0x1F;

    switch (field_0x230) {
    case 0: {
        u32 bit1 = (args >> 17) & 0x1F;
        u32 bit2 = (args >> 12) & 0x1F;
        if (!getGameView_802D6A9C()->fn_802D7F40((args >> 22) & 0x1F)) {
            break;
        }
        if (!getGameView_802D6A9C()->fn_802D7F40(bit1)) {
            break;
        }
        if (!getGameView_802D6A9C()->fn_802D7F40(bit2)) {
            break;
        }
        if (!getGameView_802D6A9C()->fn_802D7F40(bit3)) {
            if (fn_8023077C()) {
                field_0x230 = 2;
                field_0x234 = 0;
                field_0x238 = 30;
            } else {
                field_0x230 = 1;
            }
        } else {
            fn_801F3668();
        }
        break;
    }
    case 1:
        if (fn_8023077C()) {
            field_0x230 = 2;
            field_0x234 = 0;
            field_0x238 = 30;
        }
        break;
    case 2:
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
        }
        field_0x238--;
        if (field_0x238 == 0) {
            field_0x234++;
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                fn_80230498(i, &pos, field_0x234 + 0xDB, 0xFF, -0x80, -0x80, true);
                if (field_0x234 == 1) {
                    fn_80230FD0(i, 0x10001);
                }
            }
            field_0x238 = 2;
            if (field_0x234 == 0xD) {
                field_0x230 = 3;
                field_0x238 = 10;
            }
        }
        break;
    case 3:
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
        }
        if (field_0x238 == 0) {
            if (fn_80230534(false)) {
                getGameView_802D6A9C()->fn_802D7F84(bit3, true);
                fn_801F0D20(3);
                fn_801F3668();
            }
        } else {
            field_0x238--;
        }
        break;
    }
}

void UnknownGSCK_804AA800::draw() {}

UnknownGSCK_804AA800::~UnknownGSCK_804AA800() {}
