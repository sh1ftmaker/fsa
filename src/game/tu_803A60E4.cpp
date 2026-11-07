#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
void* fn_804030C4(u32 type, const char* name);
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

// Fonts, texts and the arrows' base angles of the options screen: the constructor of the
// object at 0x804AF860 (0x84 bytes).
// FIXME(shotgun): 86.46 %, missing/extra + reloc-name
cls_804AF860::cls_804AF860() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    field_0x4 = new J2DScreen;
    field_0x4->setPriority("option.blo", 0x1100000, lbl_80529DEC.field_0x2c);
    field_0x8 = new PaneCtrl_804AF86C;
    field_0x8->setPane_80402F04(field_0x4->search('Popbbk03'));
    field_0xc[0] = new PaneCtrl_804AF86C;
    field_0xc[0]->setPane_80402F04(field_0x4->search('Pocur01'));
    field_0xc[1] = new PaneCtrl_804AF86C;
    field_0xc[1]->setPane_80402F04(field_0x4->search('Pocur02'));
    field_0xc[2] = new PaneCtrl_804AF86C;
    field_0xc[2]->setPane_80402F04(field_0x4->search('Pocur03'));
    field_0xc[3] = new PaneCtrl_804AF86C;
    field_0xc[3]->setPane_80402F04(field_0x4->search('Pocur04'));
    field_0xc[4] = new PaneCtrl_804AF86C;
    field_0xc[4]->setPane_80402F04(field_0x4->search('Pocur05'));
    field_0xc[5] = new PaneCtrl_804AF86C;
    field_0xc[5]->setPane_80402F04(field_0x4->search('Pocur06'));
    field_0x24 = new PaneCtrl_804AF86C;
    field_0x24->setPane_80402F04(field_0x4->search('Par01'));
    field_0x28 = new PaneCtrl_804AF86C;
    field_0x28->setPane_80402F04(field_0x4->search('Par02'));
    field_0x2c = new PaneCtrl_804AF86C;
    field_0x2c->setPane_80402F04(field_0x4->search('Ndata03'));
    field_0x30 = new PaneCtrl_804AF86C;
    field_0x30->setPane_80402F04(field_0x4->search('Ptopbb07'));
    field_0x34 = lbl_8053AED8->field_0x8;
    lbl_8053AED8->fn_80440D78((const ResTIMG*)fn_804030C4('TIMG', "kk_kettei_01.bti"));
    lbl_8053AED8->fn_80440CE4((const ResTIMG*)fn_804030C4('TIMG', "kk_modoru_01.bti"));
    ((J2DTextBox*)field_0x4->search('Tsi01'))->setFont(lbl_8053AD78->mFont);
    ((J2DTextBox*)field_0x4->search('Tsi02'))->setFont(lbl_8053AD78->mFont);
    ((J2DTextBox*)field_0x4->search('Tso01'))->setFont(lbl_8053AD78->mFont);
    ((J2DTextBox*)field_0x4->search('Tso02'))->setFont(lbl_8053AD78->mFont);
    getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tso01'))->getStringPtr(), 0x97, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tsi01'))->getStringPtr(), 0x37, 0, NULL, NULL);
    field_0x4->search('Pocur01')->show();
    field_0x4->search('Pocur02')->show();
    field_0x4->search('Pocur03')->hide();
    field_0x4->search('Pocur04')->hide();
    field_0x4->search('Pocur05')->hide();
    field_0x4->search('Pocur06')->hide();
    field_0x70 = (s32)field_0x4->search('Par01')->getRotate();
    field_0x72 = (s32)field_0x4->search('Par02')->getRotate();
    f32 y0 = field_0x4->search('Tso01')->getBounds()->i.y;
    f32 y1 = field_0x4->search('Tsi01')->getBounds()->i.y;
    field_0x60 = y1 - y0;
    field_0x64 = 0;
    field_0x68 = 0;
    field_0x74 = -6;
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
    field_0x76 = 0;
    field_0x79 = 0;
    field_0x7a = 10;
    field_0x7b = 0xFF;
    field_0x7c = 0xC0;
    field_0x80 = 5;
    lbl_8053AD90 = this;
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

// Shows or hides one of the six cursor panes Pocur01-06.
static inline void setCursor(J2DScreen* screen, u64 tag, bool show) {
    if (show) {
        screen->search(tag)->show();
    } else {
        screen->search(tag)->hide();
    }
}

// 0x803A6608. The options screen: left / right choose the arrow, up / down step the pane
// offsets, the sound mode and the rumble setting toggle with messages 0x98 / 0x99 and
// 0x38 / 0x39; the six cursor panes blink on a random timer.
// FIXME(shotgun): 60.97 %, missing/extra + reloc-name (the blink block's show / hide pattern of the six panes is approximate)
void cls_804AF860::fn_803A6608() {
    field_0x80 = 5;
    if (!field_0x76) {
        bool a = false;
        bool b = false;
        bool c = false;
        field_0x68++;
        if (field_0x68 > 0) {
            a = field_0x8->fn_80402940(0x16, 0, -448.0f, 0.0f);
            for (s32 i = 0; i < 6; i++) {
                field_0xc[i]->fn_80402940(0x16, 0, -448.0f, 0.0f);
            }
            PaneCtrl_804AF86C** arrows = &field_0x24;
            for (s32 i = 0; i < 2; i++) {
                arrows[i]->fn_80402940(0x16, 0, -448.0f, 0.0f);
            }
            field_0x2c->fn_80402940(0x16, 0, -448.0f, 0.0f);
        }
        if (field_0x68 > 0) {
            b = field_0x30->fn_80402940(0x1A, 0, -448.0f, 0.0f);
        }
        if (field_0x68 > 10) {
            c = field_0x34->fn_80402940(0x16, 0, -448.0f, 0.0f);
        }
        if (a && b && c) {
            field_0x76 = 1;
        }
        return;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1100, 1)) {
        lbl_80529DEC.field_0x245 = field_0x77;
        lbl_80529DEC.field_0x246[0] = field_0x78;
        field_0x80 = 2;
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1)) {
        if (lbl_80529DEC.field_0x245 == 1) {
            gAudioMgr_8053AF80->fn_804594E4(1);
            OSSetSoundMode(1);
        } else {
            gAudioMgr_8053AF80->fn_804594E4(0);
            OSSetSoundMode(0);
        }
        field_0x80 = 2;
        gAudioMgr_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, 1)) {
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        field_0x64++;
        if (field_0x64 > 1) {
            field_0x64 = 0;
        }
        for (s32 i = 0; i < 6; i++) {
            field_0xc[i]->moveFromCentre_80402E08(0.0f, field_0x60 * (f32)field_0x64);
        }
        field_0x6e = 0;
        PaneCtrl_804AF86C** arrows = &field_0x24;
        for (s32 i = 0; i < 2; i++) {
            arrows[i]->moveFromCentre_80402E08(0.0f, field_0x60 * (f32)field_0x64);
        }
    } else if (lbl_8053AD30->fn_80309A9C(8, 1)) {
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        field_0x64--;
        if (field_0x64 < 0) {
            field_0x64 = 1;
        }
        for (s32 i = 0; i < 6; i++) {
            field_0xc[i]->moveFromCentre_80402E08(0.0f, field_0x60 * (f32)field_0x64);
        }
        field_0x6e = 0;
        PaneCtrl_804AF86C** arrows = &field_0x24;
        for (s32 i = 0; i < 2; i++) {
            arrows[i]->moveFromCentre_80402E08(0.0f, field_0x60 * (f32)field_0x64);
        }
    } else if (lbl_8053AD30->fn_80309A9C(1, 1)) {
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        field_0x6e = -1;
        if (field_0x64 == 0) {
            if (field_0x77 == 1) {
                field_0x77 = 0;
                getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tso02'))->getStringPtr(), 0x99, 0, NULL, NULL);
                gAudioMgr_8053AF80->fn_804594E4(0);
                gAudioMgr_8053AF80->fn_80458880(0x43, -1);
                OSSetSoundMode(0);
            } else {
                field_0x77 = 1;
                getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tso02'))->getStringPtr(), 0x98, 0, NULL, NULL);
                gAudioMgr_8053AF80->fn_804594E4(1);
                gAudioMgr_8053AF80->fn_80458880(0x43, -1);
                OSSetSoundMode(1);
            }
        } else {
            if (field_0x78 == 1) {
                field_0x78 = 0;
                getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tsi02'))->getStringPtr(), 0x39, 0, NULL, NULL);
                if (lbl_8053AD30->fn_8030A388(getPlayerInputSlot_8030AAF4(0))) {
                    gGameRoot_8053AAF8->fn_80134CC0(0);
                }
            } else {
                field_0x78 = 1;
                getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tsi02'))->getStringPtr(), 0x38, 0, NULL, NULL);
                if (lbl_8053AD30->fn_8030A388(getPlayerInputSlot_8030AAF4(0))) {
                    gGameRoot_8053AAF8->fn_80134CC0(0x1E);
                }
            }
        }
    } else if (lbl_8053AD30->fn_80309A9C(2, 1)) {
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        field_0x6e = 1;
        // same toggles as the right press, in the other direction
        if (field_0x64 == 0) {
            field_0x77 = field_0x77 == 1 ? 0 : 1;
            getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tso02'))->getStringPtr(),
                                    field_0x77 == 1 ? 0x98 : 0x99, 0, NULL, NULL);
            gAudioMgr_8053AF80->fn_804594E4(field_0x77);
            gAudioMgr_8053AF80->fn_80458880(0x43, -1);
            OSSetSoundMode(field_0x77);
        } else {
            field_0x78 = field_0x78 == 1 ? 0 : 1;
            getMessageText_803832BC(((J2DTextBox*)field_0x4->search('Tsi02'))->getStringPtr(),
                                    field_0x78 == 1 ? 0x38 : 0x39, 0, NULL, NULL);
            if (lbl_8053AD30->fn_8030A388(getPlayerInputSlot_8030AAF4(0))) {
                gGameRoot_8053AAF8->fn_80134CC0(field_0x78 == 1 ? 0x1E : 0);
            }
        }
    }
    field_0x6a--;
    if (field_0x6a <= 0) {
        field_0x6a = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
        bool lit = field_0x4->search('Pocur01')->isVisible();
        for (s32 n = 1; n <= 6; n++) {
            u64 tag = 0x506F637572303000ull + n;
            setCursor(field_0x4, tag, !lit);
        }
    }
    fn_803A60E4();
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
