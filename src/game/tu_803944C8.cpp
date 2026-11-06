#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/tu_803944C8.h"
#include "game/tu_80379F84.h"
#include "JSystem/JMessage/TControl.h"

void fn_803A3228(s32 id, s32 arg1, u32 arg2) {
    lbl_8053AED0->fn_8043D874(id, arg1, arg2);
}

s32 fn_803A3210() {
    return lbl_80529DEC.field_0x118;
}

s32 fn_803A3200() {
    return lbl_80529DEC.field_0x11c;
}

void fn_803A31AC(u8 index, s32 bit, bool set) {
    getGameView_802D6A9C()->fn_802D7EB0(index, bit, set);
}

bool fn_803A3168(u8 index, s32 bit) {
    return getGameView_802D6A9C()->fn_802D7E5C(index, bit);
}

void fn_803A3144() {
    gGameRoot_8053AAF8->fn_80138B7C();
}

u8 cls_804AF5A0::fn_803A313C() {
    return field_0x1aa;
}

void cls_804AF5A0::fn_803A3114() {
    if (field_0x1a0 == 0) {
        field_0x168++;
    }
    field_0x16c++;
}

s32 cls_804AF5A0::fn_803A310C() {
    return field_0x16c;
}

void cls_804AF5A0::fn_803A30FC(s32 index, s32 value) {
    field_0x13c[index] = value;
}

void cls_804AF5A0::fn_803A30F0() {
    field_0x1a0 = 2;
}

void cls_804AF5A0::fn_803A30E4() {
    field_0x1a1 = 0;
}

void cls_804AF5A0::fn_803A30DC(s32 value) {
    field_0x148 = value;
}

void cls_804AF5A0::fn_803A30D0() {
    field_0x1a1 = 1;
}

void cls_804AF5A0::fn_803A30C4() {
    field_0x1a0 = 3;
}

s32 cls_804AF5A0::fn_803A30BC() {
    return field_0x160;
}

void cls_804AF5A0::fn_803A30B0() {
    field_0x1a9 = 1;
}

void cls_804AF5A0::fn_803A30A4(u8 a, u32 b) {
    field_0x150 = b;
    field_0x19e = a;
}

cls_804B20F8* cls_804AF5A0::fn_803A309C() {
    return field_0x3bc;
}

s32 cls_804AF5A0::fn_803A3094() {
    return field_0x14c;
}

void cls_804AF5A0::fn_803A3084(s32 index, s32 value) {
    field_0x180[index] = value;
}

void cls_804AF5A0::fn_803A3078() {
    field_0x3b1 = 0;
}

void cls_804AF5A0::fn_803A3070(s32 value) {
    field_0x174 = value;
}

s16 cls_804AF5A0::fn_803A3068() {
    return field_0x190;
}

s16 cls_804AF5A0::fn_803A3060() {
    return field_0x192;
}

s32 cls_804AF5A0::fn_803A3058() {
    return field_0x178;
}

s32 cls_804AF5A0::fn_803A3050() {
    return field_0x17c;
}

u16 fn_803A3034() {
    return lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x34;
}

u16 fn_803A3018() {
    return lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x36;
}

u8 fn_803A300C() {
    return lbl_8053AD78->field_0xe9;
}

void fn_803A2FFC() {
    lbl_8053AD78->field_0xe9 = 0;
}

void fn_803A2FEC() {
    lbl_8053AD78->field_0x44 = 0;
}

s32 fn_803A2FE0() {
    return lbl_8053AD78->field_0x10c;
}

void cls_804AF774::do_begin(const void*, const char*) {
    field_0x5c = 1;
}

void cls_804AF774::do_end() {
    field_0x60->field_0x3af = 0;
    if (field_0x60->field_0x1aa == 0) {
        field_0x60->field_0x3ae = 8;
    } else {
        field_0x60->field_0x3ae = 3;
    }
}

bool cls_804AF774::do_jump_isReady() {
    return true;
}

void cls_804AF774::do_jump(const void*, const char*) {}

void cls_804AF72C::do_reset() {
    *field_0x2c0 = 0;
    *field_0x2c4 = 0;
}

void cls_804AF72C::do_end() {
    if (field_0x2d8->field_0x1aa == 0) {
        return;
    }
    field_0x2c0[field_0x2d0++] = -1;
    field_0x2c0[field_0x2d0++] = -2;
}

void cls_804AF6E4::do_end() {
    field_0x281++;
    field_0x202[field_0x281] = field_0x22c;
    field_0x257[field_0x281] = 0;
}

cls_804AF6E4::~cls_804AF6E4() {}

cls_804AF6D4::~cls_804AF6D4() {}

cls_804AF774::~cls_804AF774() {}

void cls_804AF5A0::fn_80396F94() {
    field_0x8->update();
    fn_80396884();
    fn_80395BE8();
    field_0x3b0 = 1;
}

void cls_804AF5A0::fn_803944C8(s32 value, u8 a, u8 b) {
    field_0x3b1 = 1;
    lbl_8053AD78->field_0x48 = value;
    lbl_8053AD78->field_0xea = a;
    lbl_8053AD78->field_0xeb = b;
}
