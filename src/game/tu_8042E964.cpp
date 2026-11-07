#include <string.h>
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

// The window the warning text slides over: built inline by cls_804B1634's constructor.
inline cls_804B1600::cls_804B1600() {
    JUTResFont* font;
    lbl_8053AEC4 = this;
    field_0x4 = new J2DScreen;
    field_0x4->setPriority("bat_stage_select_window.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x8 = new PaneCtrl_804AF86C;
    field_0x8->setPane_80402F04(field_0x4->search('Pmmwk02'));
    field_0xc = new PaneCtrl_804AF86C;
    field_0xc->setPane_80402F04(field_0x4->search('Pbl01'));
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x4->search('Tmmt01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x4->search('Tmmt02'))->setFont(font);
    field_0x10 = 0;
    field_0x12 = 0;
}

// FIXME(shotgun): 93.05 %, missing/extra (loop counter and pointer registers)
cls_804B1634::cls_804B1634() {
    JUTResFont* font;
    lbl_8053AEC0 = this;
    field_0x4 = new cls_804B1600;
    field_0x8 = new J2DScreen;
    field_0x8->setPriority("warning_window.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0xc = new PaneCtrl_804AF86C;
    field_0xc->setPane_80402F04(field_0x8->search('Ndata01'));
    field_0x10[0] = new PaneCtrl_804AF86C;
    field_0x10[0]->setPane_80402F04(field_0x8->search('Pwbb01'));
    field_0x10[1] = new PaneCtrl_804AF86C;
    field_0x10[1]->setPane_80402F04(field_0x8->search('Pwbb02'));
    field_0x10[2] = new PaneCtrl_804AF86C;
    field_0x10[2]->setPane_80402F04(field_0x8->search('Pwbb03'));
    field_0x10[3] = new PaneCtrl_804AF86C;
    field_0x10[3]->setPane_80402F04(field_0x8->search('Pwbb04'));
    field_0x10[4] = new PaneCtrl_804AF86C;
    field_0x10[4]->setPane_80402F04(field_0x8->search('Pwbb05'));
    field_0x24[0] = new PaneCtrl_804AF86C;
    field_0x24[0]->setPane_80402F04(field_0x8->search('Pwb01'));
    field_0x24[1] = new PaneCtrl_804AF86C;
    field_0x24[1]->setPane_80402F04(field_0x8->search('Pwb02'));
    field_0x24[2] = new PaneCtrl_804AF86C;
    field_0x24[2]->setPane_80402F04(field_0x8->search('Pwb03'));
    field_0x24[3] = new PaneCtrl_804AF86C;
    field_0x24[3]->setPane_80402F04(field_0x8->search('Pwb04'));
    field_0x24[4] = new PaneCtrl_804AF86C;
    field_0x24[4]->setPane_80402F04(field_0x8->search('Pwb05'));
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x8->search('Twt01'))->setFont(font);
    for (s32 i = 0; i < 4; i++) {
        u8 k = 0;
        for (s32 j = 0; j < 2; j++) {
            const JGeometry::TVec2<s16>* a = reinterpret_cast<const JGeometry::TVec2<s16>*>(
                reinterpret_cast<const u8*>(field_0x10[i]->mPane) + 0x112);
            const JGeometry::TVec2<s16>* b = reinterpret_cast<const JGeometry::TVec2<s16>*>(
                reinterpret_cast<const u8*>(field_0x24[i]->mPane) + 0x112);
            field_0x38[i * 4 + j * 2] = a[k];
            field_0x78[i * 4 + j * 2] = b[k];
            k++;
            a = reinterpret_cast<const JGeometry::TVec2<s16>*>(reinterpret_cast<const u8*>(field_0x10[i]->mPane) + 0x112);
            b = reinterpret_cast<const JGeometry::TVec2<s16>*>(reinterpret_cast<const u8*>(field_0x24[i]->mPane) + 0x112);
            field_0x38[i * 4 + j * 2 + 1] = a[k];
            field_0x78[i * 4 + j * 2 + 1] = b[k];
            k++;
        }
    }
    field_0xe8 = 0;
    field_0xec = 0;
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
        field_0xc->moveFromCentre_80402E08(0.0f, -480.0f);
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
        field_0xc->moveFromCentre_80402E08(0.0f, -480.0f);
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

// FIXME: 99.76. Code identical; only the .sdata2 literal names differ (same pool
// order cause as fn_80431740).
BOOL cls_804B1634::fn_804317AC() {
    if (field_0xec != 0) {
        field_0xe8++;
        if (field_0xe8 < 10) {
            s32 n = (10 - field_0xe8) * (10 - field_0xe8);
            field_0xc->moveFromCentre_80402E08(0.0f, 20.0f + -500.0f * n / 100.0f);
        } else {
            s32 n = (13 - field_0xe8) * (13 - field_0xe8);
            field_0xc->moveFromCentre_80402E08(0.0f, 20.0f * n / 9.0f);
            if (field_0xe8 >= 13) {
                field_0xe8 = 10 + (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                field_0xc->field_0x22 = 0;
                return TRUE;
            }
        }
    }
    return FALSE;
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

void cls_804B1634::fn_804316EC() {
    if (field_0xec != 0) {
        gGameRoot2_8053AB10->fn_8013B490();
        field_0x8->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}

// FIXME: 99.46. Not a counted match: the .sdata2 literals (1.0f, 11.0f, 0.5f) pair
// only once fn_8042EF9C / fn_8042F6DC put theirs first, and the temporaries
// f0-f3 of the cursor placement are numbered differently (regalloc only).
bool cls_804B1634::fn_80431174(u8 cursor) {
    if (field_0x8->search(0x0050776375723031ull)->mIsVisible) {
        field_0xe8--;
        if (field_0xe8 <= 0) {
            field_0xe8 = 10 + (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
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

void cls_804B1634::fn_804310CC() {
    field_0x8->search(0x0050776375723031ull)->hide();
    field_0x8->search(0x0050776375723032ull)->hide();
    field_0xc->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0xc->field_0x22 = 0;
    field_0xe8 = 0;
    field_0xeb = 0;
    field_0xec = 0;
}

void cls_804B1634::fn_804310BC() {
    field_0xe8 = 0;
    field_0xec = 0;
}

// FIXME(shotgun): 99.10 %, operands (string literal pool offsets)
f32 cls_804B1634::fn_80430368(u8 noun, s32 count, char* out) {
    switch (noun) {
    case 1:
        if (count != 1) {
            strcat(out, " Force Gems");
        } else {
            strcat(out, " Force Gem");
        }
        break;
    case 2:
        if (count != 1) {
            strcat(out, " Cuccos");
        } else {
            strcat(out, " Cucco");
        }
        break;
    case 3:
        if (count != 1) {
            strcat(out, " baddies");
        } else {
            strcat(out, " baddy");
        }
        break;
    case 4:
        if (count != 1) {
            strcat(out, " times");
        } else {
            strcat(out, " time");
        }
        break;
    case 5:
        if (count != 1) {
            strcat(out, " systems");
        } else {
            strcat(out, " system");
        }
        break;
    case 6:
        if (count != 1) {
            strcat(out, " theves");
        } else {
            strcat(out, " thief");
        }
        break;
    case 7:
        if (count != 1) {
            strcat(out, " moles");
        } else {
            strcat(out, " mole");
        }
        break;
    case 8:
        if (count != 1) {
            strcat(out, " returns");
        } else {
            strcat(out, " return");
        }
        break;
    case 9:
        if (count != 1) {
            strcat(out, " Force Fairies");
        } else {
            strcat(out, " Force Fairy");
        }
        break;
    }
    return 0.0f;
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

bool cls_804B1600::fn_8042EDAC(u32 message, u8 value) {
    if (field_0x12 == 0) {
        getMessageText_803832BC(((J2DTextBox*)field_0x4->search(0x0000546d6d743031ull))->getStringPtr(), message, value, NULL, NULL);
        getMessageText_803832BC(((J2DTextBox*)field_0x4->search(0x0000546d6d743032ull))->getStringPtr(), message, value, NULL, NULL);
        field_0x8->moveFromCentre_80402E08(0.0f, -480.0f);
        field_0x8->field_0x22 = 0;
        field_0xc->mPane->setAlpha(0);
        field_0xc->field_0x26 = 0;
        field_0x10 = 0;
        field_0x12 = 1;
        return true;
    }
    return false;
}

BOOL cls_804B1600::fn_8042EC7C() {
    if (field_0x12 != 0) {
        field_0x10++;
        if (field_0x10 < 8) {
            s32 n = (8 - field_0x10) * (8 - field_0x10);
            field_0x8->moveFromCentre_80402E08(0.0f, -500.0f * n / 64.0f + 26.0f);
            field_0xc->fn_80402628(8, 0, field_0xc->field_0x28);
        } else {
            s32 n = (11 - field_0x10) * (11 - field_0x10);
            field_0x8->moveFromCentre_80402E08(0.0f, 26.0f * n / 9.0f);
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

void cls_804B1600::fn_8042EB98() {
    if (field_0x12 != 0) {
        gGameRoot2_8053AB10->fn_8013B490();
        field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}

void cls_804B1600::fn_8042EB24() {
    field_0x8->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0x8->field_0x22 = 0;
    field_0xc->mPane->setAlpha(0);
    field_0xc->field_0x26 = 0;
    field_0x10 = 0;
    field_0x12 = 0;
}

void cls_804B1600::fn_8042EB14() {
    field_0x10 = 0;
    field_0x12 = 0;
}
