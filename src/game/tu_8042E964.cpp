#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/cls_804B1600.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DGrafContext.h"

cls_804B1634* lbl_8053AEC0;
cls_804B1600* lbl_8053AEC4;

// Two 4-entry tables read with lhax by byte index (0xEB); used by fn_80431174 / fn_8042F6DC (not written yet).
static const s16 lbl_80543AE8[4] = {0x8, 0x154, 0x5, 0x0};
static const s16 lbl_80543AF0[4] = {0x164, 0xA, 0x160, 0x0};

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

BOOL cls_804B1600::fn_8042EBEC() {
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

BOOL cls_804B1600::fn_8042EC7C() {
    if (field_0x12 != 0) {
        field_0x10++;
        if (field_0x10 < 8) {
            s32 n = (8 - field_0x10) * (8 - field_0x10);
            field_0x8->fn_80402E08(0.0f, -500.0f * n / 64.0f + 26.0f);
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

// FIXME: 99.46. Not a counted match: the .sdata2 literals (1.0f, 11.0f, 0.5f) pair
// only once fn_8042EF9C / fn_8042F6DC put theirs first, and the temporaries
// f0-f3 of the cursor placement are numbered differently (regalloc only).
bool cls_804B1634::fn_80431174(u8 cursor) {
    if (field_0x8->search(0x0050776375723031ull)->mIsVisible) {
        field_0xe8--;
        if (field_0xe8 <= 0) {
            field_0xe8 = 10 + (s32)(11.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
            if (field_0xeb < 2) {
                field_0xeb++;
            } else {
                field_0xeb = 0;
            }
        }
        // y is declared first: the original keeps y in f31 and x in f30.
        f32 y;
        f32 x;
        J2DPane* textX = field_0x8->search(0x0000005477743031ull);
        x = ((J2DTextBox*)field_0x8->search(0x0000005477743031ull))->getBounds()->i.x + textX->getWidth() * 0.5f;
        J2DPane* textY = field_0x8->search(0x0000005477743031ull);
        y = ((J2DTextBox*)field_0x8->search(0x0000005477743031ull))->getBounds()->i.y + textY->getHeight() * 0.5f;
        if (cursor == 0) {
            x += (field_0xbc - field_0xc8 - field_0x8->search(0x0050776375723031ull)->getWidth()) * 0.5f;
        } else {
            x += (field_0xc0 - field_0xc8 - field_0x8->search(0x0050776375723031ull)->getWidth()) * 0.5f +
                 (field_0xd0 - field_0xcc);
        }
        f32 curHeight = field_0x8->search(0x0050776375723031ull)->getHeight();
        y += (field_0xd8[cursor] - (f32)field_0xe4 * 0.5f - 1.0f) *
                 ((J2DTextBox*)field_0x8->search(0x0000005477743031ull))->getLineSpace() +
             (((J2DTextBox*)field_0x8->search(0x0000005477743031ull))->getLineSpace() - curHeight) * 0.5f;
        field_0x8->search(0x0050776375723031ull)->move(x, y);
        field_0x8->search(0x0050776375723032ull)->move(x, y);
        f32 height1 = field_0x8->search(0x0050776375723031ull)->getHeight();
        J2DPane* cur1 = field_0x8->search(0x0050776375723031ull);
        field_0x8->search(0x0050776375723031ull)
            ->rotate(0.5f * cur1->getWidth(), 0.5f * height1, J2DROTATE_Z, lbl_80543AE8[field_0xeb]);
        f32 height2 = field_0x8->search(0x0050776375723032ull)->getHeight();
        J2DPane* cur2 = field_0x8->search(0x0050776375723032ull);
        field_0x8->search(0x0050776375723032ull)
            ->rotate(0.5f * cur2->getWidth(), 0.5f * height2, J2DROTATE_Z, lbl_80543AF0[field_0xeb]);
    }
    return false;
}

void cls_804B1634::fn_804316EC() {
    if (field_0xec != 0) {
        lbl_8053AB10->fn_8013B490();
        field_0x8->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
    }
}

// FIXME: 99.81. Code identical; only the .sdata2 literal names differ (the pool
// order also needs the literals of the unwritten fn_8042EF9C / fn_8042F6DC first).
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

// FIXME: 99.76. Code identical; only the .sdata2 literal names differ (same pool
// order cause as fn_80431740).
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

bool cls_804B1634::fn_80431900(s32 id, u8 value, u8* out) {
    if (field_0xec == 0) {
        fn_8042EF9C(((J2DTextBox*)field_0x8->search(0x0000005477743031ull))->getStringPtr(), id, value);
        if (out != NULL) {
            field_0x8->search(0x0050776375723031ull)->show();
            field_0x8->search(0x0050776375723032ull)->show();
        } else {
            field_0x8->search(0x0050776375723031ull)->hide();
            field_0x8->search(0x0050776375723032ull)->hide();
        }
        field_0xc->fn_80402E08(0.0f, -480.0f);
        field_0xc->field_0x22 = 0;
        field_0xe8 = 0;
        field_0xeb = 0;
        field_0xec = 1;
        fn_8042F6DC();
        if (out != NULL) {
            *out = field_0xea;
        }
        return true;
    }
    return false;
}

bool cls_804B1634::fn_80431A9C(s32 id, u8 value, u8* out) {
    if (field_0xec == 0) {
        fn_8042EF9C(((J2DTextBox*)field_0x8->search(0x0000005477743031ull))->getStringPtr(), id, value);
        if (out != NULL) {
            field_0x8->search(0x0050776375723031ull)->show();
            field_0x8->search(0x0050776375723032ull)->show();
        } else {
            field_0x8->search(0x0050776375723031ull)->hide();
            field_0x8->search(0x0050776375723032ull)->hide();
        }
        field_0xc->fn_80402E08(0.0f, -480.0f);
        field_0xc->field_0x22 = 0;
        field_0xe8 = 0;
        field_0xeb = 0;
        field_0xec = 1;
        fn_8042F6DC();
        if (out != NULL) {
            *out = field_0xea;
        }
        return true;
    }
    return false;
}

cls_804B1634::~cls_804B1634() {
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    for (s32 i = 0; i < 5; i++) {
        delete field_0x10[i];
        field_0x10[i] = NULL;
        delete field_0x24[i];
        field_0x24[i] = NULL;
    }
    lbl_8053AEC0 = NULL;
}
