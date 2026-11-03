#include "game/GameAudio.h"
#include "game/cls_804AE9B0.h"
#include "game/cls_804B1D58.h"
#include "game/cls_804B1DA8.h"
#include "game/GameGlobals.h"

cls_804AE9B0* lbl_8053AD70;

u8 cls_804AE9B0::fn_8037ACC4() {
    if (lbl_8053AAF8->field_0x7f) {
        return lbl_8053AF40->fn_8044CB00();
    }
    return 0;
}

void cls_804AE9B0::fn_8037AD00(int arg) {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053AF40->fn_8044CB08(arg);
    }
}

u8 cls_804AE9B0::fn_8037AD34() {
    if (lbl_8053AAF8->field_0x7f) {
        return lbl_8053AF40->fn_8044D290();
    }
    return 0;
}

void cls_804AE9B0::fn_8037AD70(int arg) {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053AF40->fn_8044D298(arg);
    }
}

void cls_804AE9B0::fn_8037ADA4() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053AF40->fn_8044D894();
    }
}

u8 cls_804AE9B0::fn_8037ADD8() {
    if (lbl_8053AAF8->field_0x7f) {
        return lbl_8053AF40->field_0x13f;
    }
    return lbl_8053AF48->field_0x54;
}

void cls_804AE9B0::fn_8037AE00() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053AF40->field_0x13f = 0;
    } else {
        lbl_8053AF48->field_0x54 = 0;
    }
}

void cls_804AE9B0::fn_8037AE30() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053AF40->field_0x13f = 1;
    } else {
        lbl_8053AF48->field_0x54 = 1;
    }
}

s32 fn_8037AE60() {
    if (lbl_8053AD70 != NULL) {
        if (lbl_8053AAF8->field_0x7f) {
            return fn_8044F664();
        }
        return fn_80453AF8();
    }
    return 9;
}

// FIXME: 92.5%. The original copies the result into r31 in each branch
// (mr r31, r3 right after the two calls); ours does one mr after the join
// and schedules it after the lbl_8053AD70 load. if/else, ternary and an
// inline getter all give ours.
s32 cls_804AE9B0::fn_8037AEAC() {
    s32 result;
    if (lbl_8053AAF8->field_0x7f) {
        result = lbl_8053AF40->fn_8044F680();
    } else {
        result = lbl_8053AF48->fn_80453B14();
    }
    if (fn_8037AE60() != 9) {
        field_0xc = 0;
    }
    return result;
}

void cls_804AE9B0::fn_8037AF4C() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053AF40->fn_804522C8();
    }
}

void cls_804AE9B0::fn_8037AF80() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053AF40->fn_80452308();
    } else {
        lbl_8053AF48->fn_80454A00();
    }
}

void cls_804AE9B0::fn_8037AFC0() {
    if (lbl_8053AAF8->field_0x7f) {
        field_0x4->fn_80452350();
    } else {
        field_0x8->fn_80454A6C();
    }
}

cls_804AE9B0::~cls_804AE9B0() {
    if (field_0x4) {
        delete field_0x4;
        field_0x4 = NULL;
    }
    if (field_0x8) {
        delete field_0x8;
        field_0x8 = NULL;
    }
    lbl_8053AD70 = NULL;
}

cls_804AE9B0::cls_804AE9B0() {
    lbl_8053AD70 = this;
    field_0x4 = NULL;
    field_0x8 = NULL;
    field_0xc = 0;
    if (lbl_8053AAF8->field_0x7f) {
        field_0x4 = new cls_804B1D58;
    } else {
        field_0x8 = new cls_804B1DA8;
    }
}
