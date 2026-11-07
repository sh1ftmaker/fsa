#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/act/tu_80440BF4.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JFramework/JFWDisplay.h"

// Rotates a pane of the screen about its centre.
static inline void rotateArrow(J2DScreen* screen, u64 tag, f32 angle) {
    f32 h = screen->search(tag)->getHeight();
    J2DPane* pane = screen->search(tag);
    f32 w = pane->getWidth();
    screen->search(tag)->rotate(0.5f * w, 0.5f * h, J2DROTATE_Z, angle);
}

cls_804AF860::~cls_804AF860() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    for (s32 i = 0; i < 6; i++) {
        delete field_0xc[i];
        field_0xc[i] = NULL;
    }
    delete field_0x24;
    field_0x24 = NULL;
    delete field_0x28;
    field_0x28 = NULL;
    delete field_0x2c;
    field_0x2c = NULL;
    delete field_0x30;
    field_0x30 = NULL;
    lbl_8053AD90 = NULL;
}

// 0x803A7280.
// FIXME(shotgun): 76.56 %, missing/extra
void cls_804AF860::fn_803A7280() {
    field_0x64 = 0;
    field_0x68 = 0;
    field_0x76 = 0;
    field_0x8->moveFromCentre_80402E08(0.0f, -448.0f);
    field_0x8->field_0x22 = 0;
    for (s32 i = 0; i < 6; i++) {
        field_0xc[i]->moveFromCentre_80402E08(0.0f, -448.0f);
        field_0xc[i]->field_0x22 = 0;
    }
    PaneCtrl_804AF86C** arrows = &field_0x24;
    for (s32 i = 0; i < 2; i++) {
        arrows[i]->moveFromCentre_80402E08(0.0f, -448.0f);
        arrows[i]->field_0x22 = 0;
    }
    field_0x2c->moveFromCentre_80402E08(0.0f, -448.0f);
    field_0x2c->field_0x22 = 0;
    field_0x30->moveFromCentre_80402E08(0.0f, -448.0f);
    field_0x30->field_0x22 = 0;
    field_0x34->moveFromCentre_80402E08(0.0f, -448.0f);
    field_0x34->field_0x22 = 0;
    field_0x77 = lbl_80529DEC.field_0x245;
    field_0x78 = lbl_80529DEC.field_0x246[0];
    if (field_0x77 == 1) {
        getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tso02'))->getStringPtr(), 0x98, 0, NULL, NULL);
    } else {
        getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tso02'))->getStringPtr(), 0x99, 0, NULL, NULL);
    }
    if (field_0x78 == 1) {
        getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tsi02'))->getStringPtr(), 0x38, 0, NULL, NULL);
    } else {
        getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tsi02'))->getStringPtr(), 0x39, 0, NULL, NULL);
    }
    rotateArrow(field_0x4, 'Par01', (f32)(field_0x70 + field_0x74));
    rotateArrow(field_0x4, 'Par02', (f32)(field_0x72 + field_0x74));
    field_0x6a = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
    field_0x6c = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
    field_0x6e = 0;
    field_0x79 = 0;
    field_0x7a = 10;
    field_0x7b = 0xFF;
    field_0x7c = 0xC0;
    field_0x80 = 5;
}

// FIXME(shotgun): 99.86 %, reloc-name
void cls_804AF860::fn_803A71F0() {
    JFWDisplay::getManager()->setClearColor(JUtility::TColor(0, 0, 0, 0));
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
}

s32 fn_803A65EC() {
    if (lbl_8053AD90 != NULL) {
        return lbl_8053AD90->field_0x80;
    }
    return 5;
}

// 0x803A60E4.
// FIXME(shotgun): 50.3 %, missing/extra + reloc-name
void cls_804AF860::fn_803A60E4() {
    if (field_0x6e == 0) {
        field_0x6c--;
        if (field_0x6c <= 0) {
            field_0x6c = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
            if (field_0x74 == -6) {
                field_0x74 = 6;
            } else {
                field_0x74 = -6;
            }
            rotateArrow(field_0x4, 'Par01', (f32)(field_0x70 + field_0x74));
            rotateArrow(field_0x4, 'Par02', (f32)(field_0x72 + field_0x74));
        }
        return;
    }
    if (field_0x6e == 1 || field_0x6e == -1) {
        field_0x74 = 0;
        rotateArrow(field_0x4, 'Par01', (f32)(field_0x70 + field_0x74));
        rotateArrow(field_0x4, 'Par02', (f32)(field_0x72 + field_0x74));
    }
    if (field_0x6e < 0) {
        s32 t;
        if (field_0x6e > -5) {
            t = -field_0x6e;
        } else {
            t = field_0x6e + 10;
            if (field_0x6e == -10) {
                field_0x6e = 0;
            }
        }
        field_0x24->moveFromCentre_80402E08(-5.0f * (f32)field_0x64 / 25.0f, field_0x60 * (f32)(t * t));
        if (field_0x6e != 0) {
            field_0x6e--;
        }
    } else if (field_0x6e > 0) {
        s32 t = field_0x6e;
        if (field_0x6e >= 5) {
            t = 10 - field_0x6e;
            if (field_0x6e == 10) {
                field_0x6e = 0;
            }
        }
        field_0x28->moveFromCentre_80402E08(5.0f * (f32)field_0x64 / 25.0f, field_0x60 * (f32)(t * t));
        if (field_0x6e != 0) {
            field_0x6e++;
        }
    }
}
