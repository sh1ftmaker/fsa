#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/cls_804B1600.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DGrafContext.h"

cls_804B1634* lbl_8053AEC0;
static cls_804B1600* lbl_8053AEC4;

void cls_804B1600::fn_8042EB14() {
    field_0x10 = 0;
    field_0x12 = 0;
}

void cls_804B1600::fn_8042EB24() {
    field_0x8->fn_80402E08(0.0f, -480.0f);
    field_0x8->field_0x22 = 0;
    field_0xc->mPane->setAlpha(0);
    field_0xc->field_0x26 = 0;
    field_0x10 = 0;
    field_0x12 = 0;
}

void cls_804B1600::fn_8042EB98() {
    if (field_0x12 != 0) {
        lbl_8053AB10->fn_8013B490();
        field_0x4->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
    }
}

bool cls_804B1600::fn_8042EBEC() {
    if (field_0x12 != 0) {
        u8 done = field_0x8->fn_80402940(10, 0, 0.0f, -480.0f);
        field_0xc->fn_80402628(10, field_0xc->field_0x28, 0);
        if (done == 0) {
            return false;
        }
        field_0x10 = 0;
        field_0x12 = 0;
    }
    return true;
}

// FIXME: 98.7 %. Only the float registers differ: the original loads the
// -500.0f and 0.015625f constants into f5 and f2 (fmadds f2, f3, f2, f0), we
// use f4 and f5 (fmadds f2, f5, f2, f0).
bool cls_804B1600::fn_8042EC7C() {
    if (field_0x12 != 0) {
        field_0x10++;
        if (field_0x10 < 8) {
            s32 n = (8 - field_0x10) * (8 - field_0x10);
            field_0x8->fn_80402E08(0.0f, 26.0f + 0.015625f * (-500.0f * n));
            field_0xc->fn_80402628(8, 0, field_0xc->field_0x28);
        } else {
            s32 n = (11 - field_0x10) * (11 - field_0x10);
            field_0x8->fn_80402E08(0.0f, 26.0f * n / 9.0f);
            field_0xc->mPane->setAlpha(field_0xc->field_0x28);
            if (field_0x10 >= 11) {
                field_0x8->field_0x22 = 0;
                field_0xc->field_0x26 = 0;
                return true;
            }
        }
    }
    return false;
}

bool cls_804B1600::fn_8042EDAC(u32 message, u8 value) {
    if (field_0x12 == 0) {
        fn_803832BC(((J2DTextBox*)field_0x4->search(0x0000546d6d743031ull))->getStringPtr(), message, value, NULL, NULL);
        fn_803832BC(((J2DTextBox*)field_0x4->search(0x0000546d6d743032ull))->getStringPtr(), message, value, NULL, NULL);
        field_0x8->fn_80402E08(0.0f, -480.0f);
        field_0x8->field_0x22 = 0;
        field_0xc->mPane->setAlpha(0);
        field_0xc->field_0x26 = 0;
        field_0x10 = 0;
        field_0x12 = 1;
        return true;
    }
    return false;
}

cls_804B1600::~cls_804B1600() {
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    lbl_8053AEC4 = NULL;
}

void cls_804B1634::fn_804310BC() {
    field_0xe8 = 0;
    field_0xec = 0;
}

void cls_804B1634::fn_804310CC() {
    field_0x8->search(0x0050776375723031ull)->hide();
    field_0x8->search(0x0050776375723032ull)->hide();
    field_0xc->fn_80402E08(0.0f, -480.0f);
    field_0xc->field_0x22 = 0;
    field_0xe8 = 0;
    field_0xeb = 0;
    field_0xec = 0;
}

void cls_804B1634::fn_804316EC() {
    if (field_0xec != 0) {
        lbl_8053AB10->fn_8013B490();
        field_0x8->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
    }
}

BOOL cls_804B1634::fn_80431740() {
    if (field_0xec != 0) {
        if (field_0xc->fn_80402940(10, 0, 0.0f, 480.0f) == 0) {
            return FALSE;
        }
        field_0xe8 = 0;
        field_0xec = 0;
    }
    return TRUE;
}

BOOL cls_804B1634::fn_804317AC() {
    if (field_0xec != 0) {
        field_0xe8++;
        if (field_0xe8 < 10) {
            s32 n = (10 - field_0xe8) * (10 - field_0xe8);
            field_0xc->fn_80402E08(0.0f, 20.0f + -500.0f * n / 100.0f);
        } else {
            s32 n = (13 - field_0xe8) * (13 - field_0xe8);
            field_0xc->fn_80402E08(0.0f, 20.0f * n / 9.0f);
            if (field_0xe8 >= 13) {
                field_0xe8 = 10 + (s32)(11.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
                field_0xc->field_0x22 = 0;
                return TRUE;
            }
        }
    }
    return FALSE;
}
