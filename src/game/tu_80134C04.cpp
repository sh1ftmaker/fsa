#include "game/GameAudio.h"
#include "game/GameGlobals.h"

void GameRoot_8049EF20::fn_80134CC0(s32 value) {
    field_0x108 = value;
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

void GameRoot_8049EF20::fn_801365C8(u8 value) {
    field_0xfe = value;
    if (value != 1) {
        return;
    }
    field_0xfc = 0;
    field_0xf8 = 0;
}

void GameRoot_8049EF20::fn_80134D14(u8 value) {
    lbl_8053AAF8->field_0x88 = value;
}

void GameRoot_8049EF20::fn_80134DC0() {
    if (field_0x86 == 0) {
        field_0x86 = 1;
    } else {
        field_0x86 = 0;
    }
}

// The 2-bit field of field_0x83 for `index` (0-3), as the caller reads it.
s32 fn_8013997C(s32 index) {
    return (lbl_8053AAF8->field_0x83 >> (index * 2)) & 3;
}

void fn_801365A8() {
    fn_80021840();
}

void fn_80139958() {
    fn_8037A708(lbl_8053AD68);
}

// JUTGamePad C3ButtonReset callback (JUTResetBtnCb).
void fn_80139994(int port, void* arg) {
    lbl_8053ACA8->fn_802B1610(port, arg);
}
