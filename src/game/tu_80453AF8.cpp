#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
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

// The three rows' colours: the selected one is lit.
static inline void setRowColors(cls_804B1DA8* self, s32 selected) {
    for (s32 i = 0; i < 3; i++) {
        if (i == selected) {
            static_cast<J2DPicture*>(self->mScreen->search(lbl_80491470[i]))->setWhite(JUtility::TColor(0xFF, 0xED, 0xA0, 0xFF));
            static_cast<J2DPicture*>(self->mScreen->search(lbl_80491470[i]))->setBlack(JUtility::TColor(0x27, 0x10, 0, 0));
        } else {
            static_cast<J2DPicture*>(self->mScreen->search(lbl_80491470[i]))->setWhite(JUtility::TColor(0xA7, 0x4C, 0x2A, 0xFF));
            static_cast<J2DPicture*>(self->mScreen->search(lbl_80491470[i]))->setBlack(JUtility::TColor(0, 0, 0, 0));
        }
    }
}

// FIXME(shotgun): 66.2 %, missing/extra + reloc-name (skeleton: the 14-state pause-screen machine with its real calls; the
// per-state animation maths of states 5 / 10 and the row-move positions are approximated)
s32 cls_804B1DA8::fn_80453B14() {
    field_0x58 = 9;
    switch (field_0x1e) {
    case 2:
        field_0x8->fn_80402628(lbl_8050DB0C.field_0x4c[32], 0, field_0x8->field_0x28);
        if (field_0xc->fn_80402940(lbl_8050DB0C.field_0x4c[32], 0, 0.0f, (f32)lbl_8050DB0C.field_0x4c[34])) {
            field_0xc->field_0x22 = 0;
            field_0x1e = 3;
        }
        break;
    case 3:
        if (field_0xc->fn_80402940(lbl_8050DB0C.field_0x4c[35], 2, (f32)lbl_8050DB0C.field_0x4c[34], 1.0f)) {
            field_0x1e = 5;
        }
        break;
    case 5:
        if (field_0x20 != 0) {
            field_0x20++;
            if (field_0x20 > 10) {
                field_0x20 = 0;
            }
        }
        if (lbl_8053AD30->fn_80309A9C(0x8, -1) && field_0x1c > 0) {
            field_0x1c--;
            field_0x20 = 1;
            setRowColors(this, field_0x1c);
            gAudioMgr_8053AF80->fn_80458880(0, -1);
        } else if (lbl_8053AD30->fn_80309A9C(0x4, -1) && field_0x1c < 2) {
            field_0x1c++;
            field_0x20 = 1;
            setRowColors(this, field_0x1c);
            gAudioMgr_8053AF80->fn_80458880(0, -1);
        } else if (lbl_8053AD30->fn_80309C4C(0x1100, -1)) {
            field_0x1e = 0xA;
            field_0x20 = 1;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else if (lbl_8053AD30->fn_80309C4C(0x200, -1)) {
            field_0x1c = 0;
            setRowColors(this, field_0x1c);
            field_0x1e = 0xA;
            field_0x20 = 1;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        }
        for (s32 i = 0; i < 3; i++) {
            if (i == field_0x1c) {
                field_0x10[i]->moveFromCentre_80402E08(0.0f, (f32)(field_0x20 * field_0x20) * 4.0f / 25.0f);
            } else {
                field_0x10[i]->moveFromCentre_80402E08(0.0f, 0.0f);
            }
        }
        break;
    case 10:
        field_0x20++;
        for (s32 i = 0; i < 3; i++) {
            field_0x10[i]->moveFromCentre_80402E08(0.0f, 0.0f);
        }
        if (field_0x20 >= 15) {
            if (field_0x1c == 0) {
                field_0x1e = 0xB;
                field_0x8->field_0x26 = 0;
            } else {
                field_0x1e = 0xD;
            }
            field_0x20 = 0;
            field_0xc->field_0x22 = 0;
        }
        break;
    case 11:
        if (field_0xc->fn_80402940(lbl_8050DB0C.field_0x4c[36], 2, 0.0f, (f32)lbl_8050DB0C.field_0x4c[33])) {
            field_0xc->field_0x22 = 0;
            field_0x1e = 0xC;
        }
        break;
    case 12:
        field_0x8->fn_80402628(lbl_8050DB0C.field_0x4c[37], 0, field_0x8->field_0x28);
        if (field_0xc->fn_80402940(lbl_8050DB0C.field_0x4c[36], 0, (f32)lbl_8050DB0C.field_0x4c[34], 0.0f)) {
            field_0x1e = 2;
            field_0xc->moveFromCentre_80402E08(0.0f, -480.0f);
            field_0xc->field_0x22 = 0;
            field_0x58 = 7;
            gAudioMgr_8053AF80->fn_80458880(2, -1);
            gAudioMgr_8053AF80->fn_80459274();
        }
        break;
    case 13:
        if (field_0xc->fn_80402940(10, 0, 0.0f, 0.0f)) {
            field_0x1e = 7;
            lbl_8053AEC0->fn_80431A9C(0x10A, 0, &field_0x55);
        }
        break;
    case 7:
        if (lbl_8053AEC0->fn_804317AC()) {
            field_0x1e = 8;
        }
        break;
    case 8:
        if (lbl_8053AD30->fn_80309A9C(0x2, -1)) {
            if (field_0x55 == 0) {
                field_0x55 = 1;
                gAudioMgr_8053AF80->fn_80458880(0, -1);
            }
        } else if (lbl_8053AD30->fn_80309A9C(0x1, -1)) {
            if (field_0x55 == 1) {
                field_0x55 = 0;
                gAudioMgr_8053AF80->fn_80458880(0, -1);
            }
        } else if (lbl_8053AD30->fn_80309C4C(0x1100, -1)) {
            field_0x1e = 9;
            if (field_0x55 == 1) {
                gAudioMgr_8053AF80->fn_80458880(0xB, -1);
            }
        } else if (lbl_8053AD30->fn_80309C4C(0x200, -1)) {
            field_0x1e = 9;
            field_0x55 = 1;
            if (field_0x55 == 1) {
                gAudioMgr_8053AF80->fn_80458880(0xB, -1);
            }
        }
        lbl_8053AEC0->fn_80431174(field_0x55);
        break;
    case 9:
        if (lbl_8053AEC0->fn_80431740()) {
            if (field_0x55 == 0) {
                if (field_0x1c == 1) {
                    gAudioMgr_8053AF80->fn_80458880(0xC, -1);
                }
                gAudioMgr_8053AF80->fn_80458F9C(0);
                gAudioMgr_8053AF80->fn_80459274();
                field_0x58 = 6;
            } else {
                field_0x58 = 2;
                gAudioMgr_8053AF80->fn_80458880(0xC, -1);
                gAudioMgr_8053AF80->fn_80458F9C(0);
                gAudioMgr_8053AF80->fn_80459274();
            }
        }
        break;
    }
    return 0;
}

