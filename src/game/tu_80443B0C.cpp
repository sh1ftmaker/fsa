#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/cls_804B1B60.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JUtility/JUTFont.h"

static cls_804B1B60::StateFunc lbl_804B1B30[4] = {
    &cls_804B1B60::fn_804445E0,
    &cls_804B1B60::fn_80444590,
    &cls_804B1B60::fn_80443DAC,
    &cls_804B1B60::fn_80443D5C,
};

// FIXME(shotgun): 92.24 %, missing/extra + reloc-name
cls_804B1B60::cls_804B1B60() {
    mScreen = new J2DScreen();
    mScreen->setPriority("map_yagura_window.blo", 0x1100000, lbl_80529DEC.field_0x34);

    field_0x8 = new PaneCtrl_804AF86C();
    field_0x8->setPane_80402F04(mScreen->search('Nda01'));
    field_0x8->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0x8->field_0x22 = 0;

    field_0xc = new PaneCtrl_804AF86C();
    field_0xc->setPane_80402F04(mScreen->search('Pblack'));
    field_0xc->mPane->setAlpha(0);
    field_0xc->field_0x26 = 0;

    field_0x10[0] = new PaneCtrl_804AF86C();
    field_0x10[0]->setPane_80402F04(mScreen->search('Pocur01'));
    field_0x10[1] = new PaneCtrl_804AF86C();
    field_0x10[1]->setPane_80402F04(mScreen->search('Pocur02'));
    field_0x10[2] = new PaneCtrl_804AF86C();
    field_0x10[2]->setPane_80402F04(mScreen->search('Pocur03'));
    field_0x10[3] = new PaneCtrl_804AF86C();
    field_0x10[3]->setPane_80402F04(mScreen->search('Pocur04'));
    field_0x10[4] = new PaneCtrl_804AF86C();
    field_0x10[4]->setPane_80402F04(mScreen->search('Pocur05'));
    field_0x10[5] = new PaneCtrl_804AF86C();
    field_0x10[5]->setPane_80402F04(mScreen->search('Pocur06'));

    field_0x28[0] = new PaneCtrl_804AF86C();
    field_0x28[0]->setPane_80402F04(mScreen->search('Ptcur02'));
    field_0x28[1] = new PaneCtrl_804AF86C();
    field_0x28[1]->setPane_80402F04(mScreen->search('Ptcur04'));

    field_0x10[0]->mPane->hide();
    field_0x10[1]->mPane->hide();
    field_0x10[2]->mPane->hide();
    field_0x10[3]->mPane->hide();
    field_0x10[4]->mPane->hide();
    field_0x10[5]->mPane->hide();

    field_0x30 = static_cast<J2DTextBox*>(mScreen->search('Tmm05'))->getBlack();
    field_0x34 = static_cast<J2DTextBox*>(mScreen->search('Tmm05'))->getWhite();
    field_0x38 = static_cast<J2DTextBox*>(mScreen->search('Tmm03'))->getBlack();
    field_0x3c = static_cast<J2DTextBox*>(mScreen->search('Tmm03'))->getWhite();

    static_cast<J2DTextBox*>(mScreen->search('Tmm01'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(mScreen->search('Tmm02'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(mScreen->search('Tmm03'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(mScreen->search('Tmm04'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(mScreen->search('Tmm05'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(mScreen->search('Tmm06'))->setFont(lbl_8053AD78->mFont);

    f32 y5 = mScreen->search('Tmm05')->getBounds()->i.y;
    f32 y3 = mScreen->search('Tmm03')->getBounds()->i.y;
    field_0x40 = y3 - y5;
    field_0x44 = 0;
    field_0x48 = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
    field_0x4c = 0;
    field_0x4d = 0;
}

cls_804B1B60::~cls_804B1B60() {
    delete mScreen;
    mScreen = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    for (s32 i = 0; i < 6; i++) {
        delete field_0x10[i];
        field_0x10[i] = NULL;
    }
    for (s32 i = 0; i < 2; i++) {
        delete field_0x28[i];
        field_0x28[i] = NULL;
    }
}

// FIXME(shotgun): 99.88 %, reloc-name
void cls_804B1B60::fn_804446F0(s32 arg) {
    field_0x44 = arg;
    fn_80443B0C(static_cast<J2DTextBox*>(mScreen->search('Tmm01'))->getStringPtr(), 0x241, arg);
    fn_80443B0C(static_cast<J2DTextBox*>(mScreen->search('Tmm02'))->getStringPtr(), 0x241, arg);
    fn_80443B0C(static_cast<J2DTextBox*>(mScreen->search('Tmm03'))->getStringPtr(), 0x242, arg);
    fn_80443B0C(static_cast<J2DTextBox*>(mScreen->search('Tmm04'))->getStringPtr(), 0x242, arg);
    fn_80443B0C(static_cast<J2DTextBox*>(mScreen->search('Tmm05'))->getStringPtr(), 0x243, arg);
    fn_80443B0C(static_cast<J2DTextBox*>(mScreen->search('Tmm06'))->getStringPtr(), 0x243, arg);

    field_0x8->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0x8->field_0x22 = 0;
    field_0xc->mPane->setAlpha(0);
    field_0xc->field_0x26 = 0;

    field_0x10[0]->mPane->show();
    field_0x10[1]->mPane->show();
    field_0x48 = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
    field_0x4c = 0;

    static_cast<J2DTextBox*>(mScreen->search('Tmm03'))->setBlackWhite(field_0x30, field_0x34);
    static_cast<J2DTextBox*>(mScreen->search('Tmm05'))->setBlackWhite(field_0x38, field_0x3c);

    for (s32 i = 0; i < 6; i++) {
        field_0x10[i]->moveFromCentre_80402E08(0.0f, field_0x40);
    }
    for (s32 i = 0; i < 2; i++) {
        field_0x28[i]->moveFromCentre_80402E08(0.0f, field_0x40);
    }
    field_0x4d = 0;
}

bool cls_804B1B60::fn_804446A4() {
    if (field_0x4d != 4) {
        (this->*lbl_804B1B30[field_0x4d])();
        return false;
    }
    return true;
}

// FIXME(shotgun): 99.76 %, reloc-name
void cls_804B1B60::fn_80444650() {
    if (field_0x4d != 4) {
        gGameRoot2_8053AB10->fn_8013B490();
        mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}

void cls_804B1B60::fn_804445E0() {
    field_0xc->fn_80402628(0x10, 0, field_0xc->field_0x28);
    if (field_0x8->fn_80402940(0x10, 0, -480.0f, 26.0f)) {
        field_0x8->field_0x22 = 0;
        field_0x4d = 1;
    }
}

// FIXME(shotgun): 99.75 %, reloc-name
void cls_804B1B60::fn_80444590() {
    if (field_0x8->fn_80402940(5, 0, 26.0f, 0.0f)) {
        field_0x4d = 2;
    }
}

// FIXME(shotgun): 99.92 %, reloc-name
void cls_804B1B60::fn_80443DAC() {
    if (lbl_8053AD30->fn_80309C4C(4, -1)) {
        if (field_0x4c == 0) {
            field_0x4c = 1;
            static_cast<J2DTextBox*>(mScreen->search('Tmm03'))->setBlackWhite(field_0x38, field_0x3c);
            static_cast<J2DTextBox*>(mScreen->search('Tmm05'))->setBlackWhite(field_0x30, field_0x34);
            for (s32 i = 0; i < 6; i++) {
                field_0x10[i]->moveFromCentre_80402E08(0.0f, 0.0f);
            }
            for (s32 i = 0; i < 2; i++) {
                field_0x28[i]->moveFromCentre_80402E08(0.0f, 0.0f);
            }
            gAudioMgr_8053AF80->fn_80458880(0, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(8, -1)) {
        if (field_0x4c == 1) {
            field_0x4c = 0;
            static_cast<J2DTextBox*>(mScreen->search('Tmm03'))->setBlackWhite(field_0x30, field_0x34);
            static_cast<J2DTextBox*>(mScreen->search('Tmm05'))->setBlackWhite(field_0x38, field_0x3c);
            for (s32 i = 0; i < 6; i++) {
                field_0x10[i]->moveFromCentre_80402E08(0.0f, field_0x40);
            }
            for (s32 i = 0; i < 2; i++) {
                field_0x28[i]->moveFromCentre_80402E08(0.0f, field_0x40);
            }
            gAudioMgr_8053AF80->fn_80458880(0, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, -1)) {
        field_0x8->field_0x22 = 0;
        field_0x4d = 3;
        if (field_0x4c == 0) {
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x200, -1)) {
        field_0x4c = 1;
        static_cast<J2DTextBox*>(mScreen->search('Tmm03'))->setBlackWhite(field_0x38, field_0x3c);
        static_cast<J2DTextBox*>(mScreen->search('Tmm05'))->setBlackWhite(field_0x30, field_0x34);
        for (s32 i = 0; i < 6; i++) {
            field_0x10[i]->moveFromCentre_80402E08(0.0f, 0.0f);
        }
        for (s32 i = 0; i < 2; i++) {
            field_0x28[i]->moveFromCentre_80402E08(0.0f, 0.0f);
        }
        field_0x8->field_0x22 = 0;
        field_0x4d = 3;
        gAudioMgr_8053AF80->fn_80458880(0xC, -1);
    }

    if (--field_0x48 <= 0) {
        field_0x48 = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
        if (mScreen->search('Pocur01')->isVisible()) {
            mScreen->search('Pocur01')->hide();
            mScreen->search('Pocur02')->hide();
            mScreen->search('Pocur03')->show();
            mScreen->search('Pocur04')->show();
            mScreen->search('Pocur05')->hide();
            mScreen->search('Pocur06')->hide();
        } else if (mScreen->search('Pocur03')->isVisible()) {
            mScreen->search('Pocur01')->hide();
            mScreen->search('Pocur02')->hide();
            mScreen->search('Pocur03')->hide();
            mScreen->search('Pocur04')->hide();
            mScreen->search('Pocur05')->show();
            mScreen->search('Pocur06')->show();
        } else {
            mScreen->search('Pocur01')->show();
            mScreen->search('Pocur02')->show();
            mScreen->search('Pocur03')->hide();
            mScreen->search('Pocur04')->hide();
            mScreen->search('Pocur05')->hide();
            mScreen->search('Pocur06')->hide();
        }
    }
}

// FIXME(shotgun): 99.50 %, reloc-name
void cls_804B1B60::fn_80443D5C() {
    if (field_0x8->fn_80402940(10, 0, 0.0f, 480.0f)) {
        field_0x4d = 4;
    }
}

// FIXME(shotgun): 79.51 %, missing/extra
void cls_804B1B60::fn_80443B0C(char* dst, s32 msgId, s32 value) {
    MessageInf1_804AF070* inf1 =
        &static_cast<MessageFile_804AF070*>(gGameRoot_8053AAF8->field_0xa8->getResource('????', "gc_four_swords_text_e.bmg"))->inf1;
    const char* text = (const char*)inf1 + (inf1->size + inf1->entries[msgId].textOffset + 8);
    s32 i = 0;
    s32 j = 0;
    s32 d10000 = value / 10000;
    const char* p;
    while ((p = text + i)[0] != 0) {
        if (p[0] == 0x1A) {
            if ((u8)p[2] == 3) {
                s8 c3 = p[3];
                if ((c3 == 0 && (u8)p[4] == 3) || (c3 == 0 && (u8)p[4] == 4)) {
                    s32 rem = value;
                    if (value >= 10000) {
                        dst[j++] = d10000 + '0';
                        rem = value - (value / 10000) * 10000;
                    }
                    if (value >= 1000) {
                        dst[j++] = (rem / 1000) + '0';
                        rem -= (rem / 1000) * 1000;
                    }
                    if (value >= 100) {
                        dst[j++] = (rem / 100) + '0';
                        rem -= (rem / 100) * 100;
                    }
                    if (value >= 10) {
                        dst[j++] = (rem / 10) + '0';
                        rem -= (rem / 10) * 10;
                    }
                    dst[j++] = rem + '0';
                }
            }
            i += p[1];
        } else {
            dst[j++] = p[0];
            i++;
            if (lbl_8053AD78->mFont->isLeadByte((u8)p[0])) {
                dst[j++] = text[i++];
            }
        }
    }
    dst[j] = p[0];
}
