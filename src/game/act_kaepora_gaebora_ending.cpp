#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/act/act_kaepora_gaebora_ending.h"

cls_804ADAC8::~cls_804ADAC8() {}

// One step of the sequence: a negative value ends the sequence and restarts it.
struct KaeporaStep {
    s32 mValue;
    s32 mCount;
};

// FIXME: the original object is 0x4C bytes; its last 0x14 bytes (after the
// terminator) are not written yet, as no code here reads them.
static const KaeporaStep lbl_804742A8[] = {
    {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
    {-1, -1},
};

void cls_804ADAC8::fn_8035FF70() {
    if (field_0x2B0 == 1) {
        if (field_0x270 == 0) {
            mSway.fn_8021A570(true);
        } else if (field_0x234 == 0) {
            if (lbl_804742A8[mStep].mValue < 0) {
                mStep = 0;
            }
            field_0x234 = lbl_804742A8[mStep].mCount;
            field_0x23C = lbl_804742A8[mStep].mValue;
            mStep++;
        }
    }
    if (field_0x280 == 0) {
        fn_80360914();
    }
}

// FIXME: the member-function table update() calls through (6 entries of 12
// bytes at 0x804ADA50, the unit's .data). Its entries are not identified yet,
// so it is declared here and not defined; the function's bytes do not depend
// on its contents.
extern void (cls_804ADAC8::*lbl_804ADA50[6])();

void cls_804ADAC8::update() {
    if (field_0x280 != 0) {
        field_0x280--;
    }
    if (field_0x234 != 0) {
        field_0x234--;
    }
    while (fn_8035F3C0()) {
    }
    (this->*lbl_804ADA50[mPtmfIndex])();
    if (field_0x2B0 == 0) {
        if (field_0x2B4 != 0) {
            fn_8013CB44(field_0x2B4, &mPos, -2, -2, true);
        }
        if (field_0x2B8 != 0) {
            fn_8013CB44(field_0x2B8, &mPos, -2, -2, true);
        }
    } else {
        if (field_0x2B4 != 0) {
            fn_8013CB44(field_0x2B4, &mPos, mHeight, mDepth, true);
        }
        if (field_0x2B8 != 0) {
            fn_8013CB44(field_0x2B8, &mPos, mHeight, mDepth, true);
        }
    }
}
