#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804B1DA8.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"

// Pane tags of the three rows ('Pcntn01', 'Psc01', 'Psg01').
static const u64 lbl_80491470[3] = {
    0x0050636E746E3031ull, /* "Pcntn01" */
    0x0000005073633031ull, /* "Psc01" */
    0x0000005073673031ull, /* "Psg01" */
};

// FIXME(shotgun): 99.97 %, reloc-name
cls_804B1DA8::cls_804B1DA8() {
    lbl_8053AF48 = this;
    mScreen = new J2DScreen;
    mScreen->setPriority("pause.blo", 0x1100000, lbl_80529DEC.field_0x4c);
    field_0x24[0] = mScreen->search('Pkenc01')->getBounds()->i.x;
    field_0x30[0] = mScreen->search('Pkenc01')->getBounds()->i.y;
    field_0x3c[0] = mScreen->search('Pkenc02')->getBounds()->i.x;
    field_0x48[0] = mScreen->search('Pkenc02')->getBounds()->i.y;
    f32 dx = mScreen->search(lbl_80491470[0])->getBounds()->i.x - field_0x24[0];
    f32 dy = field_0x3c[0] - mScreen->search(lbl_80491470[0])->getBounds()->f.x;
    field_0x24[1] = mScreen->search(lbl_80491470[1])->getBounds()->i.x - dx;
    field_0x30[1] = mScreen->search(lbl_80491470[1])->getBounds()->i.y;
    field_0x3c[1] = dy + mScreen->search(lbl_80491470[1])->getBounds()->f.x;
    field_0x48[1] = mScreen->search(lbl_80491470[1])->getBounds()->i.y;
    field_0x24[2] = mScreen->search(lbl_80491470[2])->getBounds()->i.x - dx;
    field_0x30[2] = mScreen->search(lbl_80491470[2])->getBounds()->i.y;
    field_0x3c[2] = dy + mScreen->search(lbl_80491470[2])->getBounds()->f.x;
    field_0x48[2] = mScreen->search(lbl_80491470[2])->getBounds()->i.y;
    field_0x8 = new PaneCtrl_804AF86C;
    field_0x8->setPane_80402F04(mScreen->search('Pblack'));
    field_0xc = new PaneCtrl_804AF86C;
    field_0xc->setPane_80402F04(mScreen->search('Ndata01'));
    for (s32 i = 0; i < 3; i++) {
        field_0x10[i] = new PaneCtrl_804AF86C;
        field_0x10[i]->setPane_80402F04(mScreen->search(lbl_80491470[i]));
    }
    field_0x1c = 0;
    field_0x1e = 2;
    field_0x20 = 0;
    field_0x8->mPane->setAlpha(0);
    field_0x8->field_0x26 = 0;
    field_0xc->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0xc->field_0x22 = 0;
    for (s32 i = 0; i < 3; i++) {
        if (i == field_0x1c) {
            static_cast<J2DPicture*>(mScreen->search(lbl_80491470[i]))->setWhite(JUtility::TColor(0xFF, 0xED, 0xA0, 0xFF));
            static_cast<J2DPicture*>(mScreen->search(lbl_80491470[i]))->setBlack(JUtility::TColor(0x27, 0x10, 0, 0));
        } else {
            static_cast<J2DPicture*>(mScreen->search(lbl_80491470[i]))->setWhite(JUtility::TColor(0xA7, 0x4C, 0x2A, 0xFF));
            static_cast<J2DPicture*>(mScreen->search(lbl_80491470[i]))->setBlack(JUtility::TColor(0, 0, 0, 0));
        }
    }
    field_0x55 = 0;
    field_0x54 = 1;
}

cls_804B1DA8::~cls_804B1DA8() {
    gAudioMgr_8053AF80->fn_80459274();
    delete mScreen;
    mScreen = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    for (s32 i = 0; i < 3; i++) {
        delete field_0x10[i];
        field_0x10[i] = NULL;
    }
    lbl_8053AF48 = NULL;
}

// FIXME(shotgun): 99.94 %, reloc-name
void cls_804B1DA8::fn_80454A6C() {
    lbl_8053AEC0->fn_804310CC();
    field_0x1c = 0;
    field_0x1e = 2;
    field_0x20 = 0;
    field_0x8->mPane->setAlpha(0);
    field_0x8->field_0x26 = 0;
    field_0xc->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0xc->field_0x22 = 0;
    const ResTIMG* image = static_cast<const ResTIMG*>(fn_80403084('TIMG', "kk_pCselect_01.bti"));
    static_cast<J2DPicture*>(mScreen->search('Psc01'))->changeTexture(image, 0);
    for (s32 i = 0; i < 3; i++) {
        if (i == field_0x1c) {
            static_cast<J2DPicture*>(mScreen->search(lbl_80491470[i]))->setWhite(JUtility::TColor(0xFF, 0xED, 0xA0, 0xFF));
            static_cast<J2DPicture*>(mScreen->search(lbl_80491470[i]))->setBlack(JUtility::TColor(0x27, 0x10, 0, 0));
        } else {
            static_cast<J2DPicture*>(mScreen->search(lbl_80491470[i]))->setWhite(JUtility::TColor(0xA7, 0x4C, 0x2A, 0xFF));
            static_cast<J2DPicture*>(mScreen->search(lbl_80491470[i]))->setBlack(JUtility::TColor(0, 0, 0, 0));
        }
    }
    field_0x58 = 9;
}

// FIXME(shotgun): 99.81 %, reloc-name
void cls_804B1DA8::fn_80454A00() {
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x1e == 7 || field_0x1e == 8 || field_0x1e == 9) {
        lbl_8053AEC0->fn_804316EC();
    }
}

s32 fn_80453AF8() {
    if (lbl_8053AF48 != NULL) {
        return lbl_8053AF48->field_0x58;
    }
    return 9;
}
