#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804A8390.h"
#include "game/cls_804AE9B0.h"
#include "game/tu_80379F84.h"
#include "JSystem/JUtility/JUTGamePad.h"

void GameRoot_8049EF20::fn_80134CC0(s32 value) {
    field_0x108 = value;
}

// Whether the pause fade object (lbl_8053AB10->0x2C) is idle, while paused.
u8 GameRoot_8049EF20::fn_80134CC8() {
    bool paused;
    if (field_0x4c == 11) {
        paused = true;
    } else {
        paused = field_0x88 == 2;
    }
    if (paused) {
        return lbl_8053AB10->field_0x2c->field_0xc == 0;
    }
    return false;
}

void GameRoot_8049EF20::fn_80134D14(u8 value) {
    lbl_8053AAF8->field_0x88 = value;
}

void GameRoot_8049EF20::fn_80134D20(bool arg) {
    field_0x4c = 9;
    lbl_8053AD70->fn_8037ADA4();
    lbl_8053AD70->field_0xc = 1;
    field_0xa0->fn_80458880(1, -1);
    if (arg) {
        field_0xa0->fn_804593D4();
    }
    JUTGamePad* pad = JUTGamePad::getGamePad(0);
    field_0x10c = 0;
    field_0x108 = 0;
    JUTGamePad::CRumble::stopMotor(pad->mPortNum, false);
    pad->mRumble.stopPatternedRumble(pad->mPortNum);
}

void GameRoot_8049EF20::fn_80134DC0() {
    if (field_0x86 == 0) {
        field_0x86 = 1;
    } else {
        field_0x86 = 0;
    }
}

void GameRoot_8049EF20::fn_80134FBC(u8 value) {
    field_0x96 = value;
}

void GameRoot_8049EF20::fn_80134FC4(u8 value) {
    field_0x94 = value;
    if (field_0x94 == 1) {
        return;
    }
    field_0x95 = 1;
}

// FIXME: 93.1. The target reloads field_0x94 into r4 and passes it on unconverted
// (no li r4, 1); ours loads it into r0 and materialises the true.
void GameRoot_8049EF20::fn_80134FE0(u8 value) {
    field_0x94 = value;
    if (field_0x94 == 1) {
        lbl_8053AF80->fn_80458658(true);
    } else {
        field_0x95 = 1;
    }
}

// Whether the game mode (field_0x4c) is one of 0 - 6, 9 or 12.
// FIXME: 85.9. The target tests mode == 0 separately and then folds 1-6 into one
// range (subi; cmplwi 5), reusing r3 for the mode; ours folds 0-6 into one range.
bool GameRoot_8049EF20::fn_80136320() {
    if (lbl_8053AAF8 != NULL) {
        s32 mode = field_0x4c;
        if (mode == 0 || (mode >= 1 && mode <= 6) || mode == 9 || mode == 12) {
            return true;
        }
    }
    return false;
}

void fn_801365A8() {
    fn_80021840();
}

void GameRoot_8049EF20::fn_801365C8(u8 value) {
    field_0xfe = value;
    if (value != 1) {
        return;
    }
    field_0xfc = 0;
    field_0xf8 = 0;
}

void GameRoot_8049EF20::fn_801365E8(s32 a, s32 b, s32 c) {
    field_0x50 = 7;
    field_0x58 = 1;
    lbl_8053ACA0->fn_802AC8AC(c);
    lbl_80529DEC.field_0x118 = a;
    lbl_80529DEC.field_0x11c = b;
    field_0x84 = 1;
}

void GameRoot_8049EF20::fn_8013670C(s32 value) {
    field_0x60 = 0;
    field_0x64 = 0;
    if (lbl_80529DEC.field_0x118 == 10 && lbl_80529DEC.field_0x11c == 2) {
        lbl_80529DEC.field_0x118 = lbl_80529DEC.field_0x120;
        lbl_80529DEC.field_0x11c = lbl_80529DEC.field_0x124;
        lbl_80529DEC.field_0x237 = 7;
    }
    field_0x50 = value;
    field_0x58 = 1;
    lbl_8053ACA0->fn_802AC8AC(0);
    field_0x84 = 0;
}

void GameRoot_8049EF20::fn_80136798(s32 a, s32 b) {
    field_0x50 = a;
    field_0x58 = 1;
    lbl_8053ACA0->fn_802AC8AC(b);
    field_0x84 = 0;
}

void GameRoot_8049EF20::fn_801367E0(s32 value) {
    if (value == 10) {
        lbl_8053ACA0->fn_802AC980();
    }
    field_0x4c = value;
}

// The number of players that take part: the set bits 0-3 of field_0x82.
s32 GameRoot_8049EF20::fn_80138A30() {
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (lbl_8053AAF8->field_0x82 & (u8)(1 << i)) {
            count++;
        }
    }
    return count;
}

void fn_80139958() {
    lbl_8053AD68->fn_8037A708();
}

// The 2-bit field of field_0x83 for `index` (0-3), as the caller reads it.
s32 fn_8013997C(s32 index) {
    return (lbl_8053AAF8->field_0x83 >> (index * 2)) & 3;
}

// JUTGamePad C3ButtonReset callback (JUTResetBtnCb).
void fn_80139994(int port, void* arg) {
    lbl_8053ACA8->fn_802B1610(port, arg);
}
