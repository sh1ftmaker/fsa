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

void GameRoot_8049EF20::fn_80134D14(u8 value) {
    lbl_8053AAF8->field_0x88 = value;
}

// The 2-bit field of field_0x83 for `index` (0-3), as the caller reads it.
extern "C" s32 fn_8013997C(s32 index) {
    return (lbl_8053AAF8->field_0x83 >> (index * 2)) & 3;
}

s32 fn_80021840();

extern "C" void fn_801365A8() {
    fn_80021840();
}
