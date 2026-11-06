#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801E6D48.h"
#include "game/tu_801E9AFC.h"

cls_804A5418::cls_804A5418() {
    field_0x2ac = -1;
    field_0x2b0 = 0;
    field_0x2b4 = 0;
}

void cls_804A5418::vfunc_0x10() {
    u32 flags = field_0x90 >> 24;
    s32 serial = mSerial;
    cls_801E9AFC* slots = fn_801E9AFC();
    field_0x2ac = slots->field_0x0[slots->field_0xf0].fn_801EA6D8(flags, serial);
}

void cls_804A5418::update() {
    if ((mFlags & 0x8000) == 0) {
        EnemyBase_804A4808::update();
    }
}

void cls_804A5418::draw() {
    if ((mFlags & 0x8000) == 0) {
        s32 arg = 0;
        if (mState == 2) {
            arg = (s32)field_0x2a8;
        }
        fn_80210630((mFlags >> 13) & 1, 0, 0, arg);
    }
}

void cls_804A5418::fn_801E6EAC() {
    if (mFlags & 0x8000) {
        s32 bit = field_0x2b4;
        if (bit != 0) {
            getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
        }
        fn_801F3668();
    } else if (mState != 1) {
        fn_801E728C(1);
    }
}

void cls_804A5418::fn_801E6DE8() {
    if (mFlags & 0x8000) {
        return;
    }
    static f32 lbl_8053AC38 = 128.0f;
    switch (mState) {
    case 0:
    case 2: {
        f32 dx = fn_80210B34(field_0x98);
        f32 dy = fn_80210AF8(field_0x98);
        JGeometry::TVec2f pos(mPos.x + dx, mPos.y + dy);
        fn_801E9AFC()->fn_801EA108(&pos, field_0x2b0, lbl_8053AC38);
        break;
    }
    }
}

cls_804A5418::~cls_804A5418() {}
