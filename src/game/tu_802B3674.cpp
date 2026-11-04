#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/GameView.h"
#include "game/cls_804A8638.h"
#include "game/tu_802EF76C.h"

void cls_804A8638::vfunc_0x5C() {
    field_0x310 += 4;
    if (field_0x2e0 > field_0x2f0) {
        field_0x2e0 += field_0x318;
    } else {
        field_0x2e0 = field_0x2f0;
    }
    if (field_0x2e4 > field_0x2f4) {
        field_0x2e4 += field_0x31c;
    } else {
        field_0x2e4 = field_0x2f4;
    }
    if (field_0x2e8 > field_0x2f8) {
        field_0x2e8 += field_0x320;
    } else {
        field_0x2e8 = field_0x2f8;
    }
    if (field_0x310 >= field_0x314) {
        field_0x310 = field_0x314;
        field_0x2e0 = field_0x2f0;
        field_0x2e4 = field_0x2f4;
        field_0x2e8 = field_0x2f8;
        field_0x240 = 9;
    }
    fn_80128C58((u8)field_0x310);
    u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
    fn_8013A884(&color);
}

void cls_804A8638::vfunc_0x58() {
    field_0x310 -= 4;
    if (field_0x2e0 > field_0x2f0) {
        field_0x2e0 += field_0x318;
    } else {
        field_0x2e0 = field_0x2f0;
    }
    if (field_0x2e4 > field_0x2f4) {
        field_0x2e4 += field_0x31c;
    } else {
        field_0x2e4 = field_0x2f4;
    }
    if (field_0x2e8 > field_0x2f8) {
        field_0x2e8 += field_0x320;
    } else {
        field_0x2e8 = field_0x2f8;
    }
    if (field_0x310 <= field_0x314) {
        field_0x310 = field_0x314;
        field_0x2e0 = field_0x2f0;
        field_0x2e4 = field_0x2f4;
        field_0x2e8 = field_0x2f8;
        field_0x240 = 9;
    }
    fn_80128C58((u8)field_0x310);
    u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
    fn_8013A884(&color);
}

void cls_804A8638::vfunc_0x54(s32 a, s32 b, s32 c, s32 d, s32 e, s32 state) {
    field_0x240 = state;
    if (state == 9) {
        field_0x2e0 = a;
        field_0x2e4 = b;
        field_0x2e8 = c;
        field_0x2ec = d;
        field_0x310 = e;
        return;
    }
    field_0x2f0 = a;
    field_0x2f4 = b;
    field_0x2f8 = c;
    field_0x2ec = d;
    field_0x314 = e;
    field_0x318 = (field_0x2f0 - field_0x2e0) / 4;
    field_0x31c = (field_0x2f4 - field_0x2e4) / 4;
    field_0x320 = (field_0x2f8 - field_0x2e8) / 4;
    if (field_0x310 >= field_0x314) {
        return;
    }
    field_0x240 = 0xc;
}

void cls_804A8638::fn_802B39E8() {
    field_0x290 = 0;
    deleteEmitter(&mEmitter);
}

void cls_804A8638::draw() {}

void cls_804A8638::vfunc_0x10() {
    if (field_0x288 != 4) {
        return;
    }
    fn_802D6A9C()->fn_802D6BF8(0);
    field_0x288 = 0;
    field_0x2e0 = field_0x300;
    field_0x2e4 = field_0x304;
    field_0x2e8 = field_0x308;
    deleteEmitter(&field_0x234);
    deleteEmitter(&field_0x238);
    u32 color = (field_0x2e0 << 24) | (field_0x2e4 << 16) | (field_0x2e8 << 8) | field_0x2ec;
    fn_8013A884(&color);
}
