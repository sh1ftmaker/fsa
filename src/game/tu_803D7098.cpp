#include "game/EmitterDelete.h"
#include "game/GameAudio.h"
#include "game/Message.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80379F84.h"
#include "game/act/tu_80440BF4.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DAnmLoader.h"

extern const u64 lbl_80476F80[];
extern const u64 lbl_80477180[];
extern const u64 lbl_80477408[];

// FIXME(shotgun): 95 %, missing/extra + reloc-name
cls_804B03D8::cls_804B03D8() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    lbl_8053ADC0 = this;
    field_0x4 = new J2DScreen;
    field_0x4->setPriority("total.blo", 0x1100000, lbl_8053AD68->field_0x14);
    field_0x8 = new J2DScreen;
    field_0x8->setPriority("total_star.blo", 0x1100000, lbl_8053AD68->field_0x14);
    const ResTIMG* tex = (const ResTIMG*)fn_804030C4('TIMG', "kk_black.bti");
    field_0x1c = new J2DPicture(tex);
    field_0x1c->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));
    field_0x20 = new J2DPicture(tex);
    field_0x20->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));
    JUTResFont* font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x4->search('Tno01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x4->search('Tyes01'))->setFont(font);
    getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x4->search('Tyes01'))->getStringPtr(), 0x10C, 0, NULL, NULL);
    getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x4->search('Tno01'))->getStringPtr(), 0x10D, 0, NULL, NULL);
    field_0x24[0] = new PaneCtrl_804AF86C;
    field_0x24[0]->setPane_80402F04(field_0x4->search('Pyesb04'));
    field_0x24[1] = new PaneCtrl_804AF86C;
    field_0x24[1]->setPane_80402F04(field_0x4->search('Pnob04'));
    field_0x2c = new PaneCtrl_804AF86C;
    field_0x2c->setPane_80402F04(field_0x4->search('Paccur02'));
    field_0x30 = new PaneCtrl_804AF86C;
    field_0x30->setPane_80402F04(field_0x4->search('Pttlbk05'));
    field_0x34[0] = new PaneCtrl_804AF86C;
    field_0x34[0]->setPane_80402F04(field_0x4->search('Ndata03'));
    field_0x34[1] = new PaneCtrl_804AF86C;
    field_0x34[1]->setPane_80402F04(field_0x4->search('Pblk03'));
    field_0x34[2] = new PaneCtrl_804AF86C;
    field_0x34[2]->setPane_80402F04(field_0x4->search('Pblk02'));
    field_0x34[3] = new PaneCtrl_804AF86C;
    field_0x34[3]->setPane_80402F04(field_0x4->search('Pblk01'));
    field_0x4->search('Ndata05')->mIsVisible = false;
    field_0x44 = new PaneCtrl_804AF86C;
    field_0x44->setPane_80402F04(field_0x8->search('ROOT'));
    field_0x48 = lbl_8053AED8->field_0x8;
    OSInitFastCast();
    field_0xc = fn_80403144('SCRN', "total_star.bck");
    field_0x10 = static_cast<J2DAnmTransform*>(J2DAnmLoaderDataBase::load(field_0xc));
    field_0x8->search('P0kak00')->setAnimation(field_0x10);
    field_0x14 = fn_80403144('SCRN', "total_star.brk");
    field_0x18 = static_cast<J2DAnmTevRegKey*>(J2DAnmLoaderDataBase::load(field_0x14));
    field_0x8->setAnimation(field_0x18);
    field_0x4c[0] = NULL;
    field_0x4c[1] = NULL;
    field_0x4c[2] = NULL;
    field_0x58 = 0.0f;
    field_0x5c = 0.0f;
    field_0x60 = 0;
    field_0x64 = 0;
    field_0x74 = 0;
    field_0x68 = -1;
    field_0x6a = -1;
    field_0x6c = 0;
    field_0x6e = 0;
    field_0x70 = 0;
    field_0x72 = 0;
    field_0x76 = 2;
    field_0x77 = 0;
    field_0x78 = 0;
    field_0x79 = 0;
    field_0x7a = 0;
    field_0x7b = field_0x4->search(lbl_80477180[0])->mAlpha;
    field_0x7c = field_0x4->search(lbl_80477180[1])->mAlpha;
    field_0x7d = field_0x8->search('Pstarli')->mAlpha;
    field_0x7e = 0;
    field_0x7f = 0;
    field_0x80 = 0;
    field_0x81 = 0;
}

// FIXME(shotgun): 100 %, matches
cls_804B03D8::~cls_804B03D8() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    if (field_0x4c[0] != NULL) {
        field_0x4c[0]->becomeInvalidEmitter();
        field_0x4c[0] = NULL;
    }
    for (s32 i = 1; i < 3; i++) {
        deleteEmitter(&field_0x4c[i]);
    }
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0x1c;
    field_0x1c = NULL;
    delete field_0x20;
    field_0x20 = NULL;
    for (s32 i = 0; i < 2; i++) {
        delete field_0x24[i];
        field_0x24[i] = NULL;
    }
    delete field_0x2c;
    field_0x2c = NULL;
    delete field_0x30;
    field_0x30 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x34[i];
        field_0x34[i] = NULL;
    }
    delete field_0x44;
    field_0x44 = NULL;
    delete field_0x10;
    field_0x10 = NULL;
    delete field_0x18;
    field_0x18 = NULL;
    lbl_8053ADC0 = NULL;
}

// Resets the screen for a new round: shows the rows of the players that take part and slides the
// remaining panes out of view.
// FIXME(shotgun): 73 %, missing/extra + reloc-name
void cls_804B03D8::fn_803D87C0() {
    lbl_8053AEC0->fn_804310CC();
    for (s32 i = 0; i < 3; i++) {
        if (i >= gGameRoot_8053AAF8->fn_80138A30() - 1) {
            field_0x4->search(lbl_80477180[68 + i])->mIsVisible = true;
            field_0x4->search(lbl_80477180[71 + i])->mIsVisible = false;
            field_0x4->search(lbl_80477180[74 + i])->mIsVisible = false;
        } else {
            field_0x4->search(lbl_80477180[68 + i])->mIsVisible = false;
            field_0x4->search(lbl_80477180[71 + i])->mIsVisible = true;
            field_0x4->search(lbl_80477180[74 + i])->mIsVisible = true;
        }
    }
    for (s32 k = 0; k < 4; k++) {
        u8 mask = 1 << k;
        s32 count = lbl_80529DEC.field_0x140[k];
        if ((lbl_80529DEC.field_0x243[0] & mask) != 0) {
            field_0x68 = k;
            field_0x4->search(lbl_80477180[77 + k])->mIsVisible = true;
            field_0x4->search(lbl_80477180[64 + k])->mIsVisible = true;
        } else {
            field_0x4->search(lbl_80477180[77 + k])->mIsVisible = false;
            field_0x4->search(lbl_80477180[64 + k])->mIsVisible = false;
        }
        for (s32 j = 0; j < 16; j++) {
            if (j < count && (gGameRoot_8053AAF8->mPlayerMask & mask) != 0) {
                if ((lbl_80529DEC.field_0x243[0] & mask) != 0 && j == count - 1) {
                    field_0x6a = j;
                    field_0x4->search(lbl_80476F80[k * 16 + j])->mIsVisible = false;
                    field_0x4->search(lbl_80477180[k * 16 + j])->mIsVisible = false;
                } else {
                    field_0x4->search(lbl_80476F80[k * 16 + j])->mIsVisible = true;
                    field_0x4->search(lbl_80477180[k * 16 + j])->mIsVisible = true;
                    field_0x4->search(lbl_80477180[k * 16 + j])->setAlpha(0);
                }
            } else {
                field_0x4->search(lbl_80476F80[k * 16 + j])->mIsVisible = false;
                field_0x4->search(lbl_80477180[k * 16 + j])->mIsVisible = false;
            }
        }
    }
    field_0x8->search('P0kak00')->setAlpha(0);
    field_0x8->search('P0ka00')->setAlpha(0);
    field_0x8->search('Pstarli')->setAlpha(0);
    field_0x30->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x30->field_0x22 = 0;
    field_0x48->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x48->field_0x22 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x34[i]->moveFromCentre_80402E08(0.0f, 500.0f);
        field_0x34[i]->field_0x22 = 0;
    }
    lbl_8053AED8->fn_80440D78((const ResTIMG*)fn_804030C4('TIMG', "kk_kettei_01.bti"));
    fn_804030C4('TIMG', "kk_modoru_01.bti");
    lbl_8053AED8->fn_80440BF4(false);
    field_0x2c->mPane->mIsVisible = false;
    field_0x2c->mPane->setAlpha(0);
    field_0x4->search('Paccur01')->setAlpha(0);
    for (s32 i = 0; i < 2; i++) {
        field_0x24[i]->moveFromCentre_80402E08(0.0f, 200.0f);
        field_0x24[i]->field_0x22 = 0;
    }
    lbl_80529DEC.field_0x243[0] = 0;
    gAudioMgr_8053AF80->fn_80458FF0(0x1000005, true, 0);
}

void cls_804B03D8::fn_803D8718() {
    switch (field_0x76) {
    case 6:
        if (field_0x77 >= 245) {
            field_0x77 = 255;
            field_0x76 = 5;
        } else {
            field_0x77 += 10;
        }
        break;
    case 5:
        if (field_0x77 <= 10) {
            field_0x77 = 0;
            lbl_8053AD68->field_0x1d = 1;
            field_0x76 = 2;
        } else {
            field_0x77 -= 10;
        }
        break;
    case 2:
        fn_803D75B0();
        break;
    }
}

// FIXME(shotgun): 99.40 %, reloc-name
void cls_804B03D8::fn_803D8670() {
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x8->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x1c->setAlpha(field_0x77);
    field_0x1c->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}

// FIXME(shotgun): 95.73 %, missing/extra (one more saved register: the bounce step)
void cls_804B03D8::fn_803D8364() {
    f32 bounce;
    f32 alpha;
    if (field_0x7f == 1 || (u8)(field_0x7f - 2) <= 1 || field_0x7f == 4) {
        bounce = 0.0f;
        if (field_0x6e > 0) {
            if (field_0x6e == 1) {
                field_0x6c = 30;
            }
            s32 step = field_0x6e < 5 ? field_0x6e : 10 - field_0x6e;
            bounce = 5.0f * (f32)(step * step) / 25.0f;
            if (field_0x6e < 10) {
                field_0x6e++;
            } else {
                field_0x6e = 0;
            }
        }
        field_0x6c++;
        if (field_0x6c >= 60) {
            field_0x6c = 0;
        }
        if (field_0x7f == 2 || field_0x7f == 3 || field_0x7f == 4) {
            field_0x20->setAlpha(field_0x81);
            field_0x20->draw(-1.0f, -1.0f, 611.0f, 451.0f, false, false, false);
            lbl_8053AEC0->fn_804316EC();
            alpha = 156.0f;
        } else {
            if (field_0x6c < 30) {
                alpha = 156.0f * ((f32)(field_0x6c * field_0x6c) / 900.0f);
            } else {
                s32 d = field_0x6c - 30;
                alpha = 156.0f * (1.0f - (f32)(d * d) / 900.0f);
            }
        }
        if (field_0x79 == 0) {
            field_0x2c->moveFromCentre_80402E08(0.0f, bounce);
            field_0x24[0]->moveFromCentre_80402E08(0.0f, bounce);
            field_0x24[1]->moveFromCentre_80402E08(0.0f, 0.0f);
        } else {
            field_0x2c->moveFromCentre_80402E08(field_0x24[1]->field_0x8 - field_0x24[0]->field_0x8, bounce);
            field_0x24[0]->moveFromCentre_80402E08(0.0f, 0.0f);
            field_0x24[1]->moveFromCentre_80402E08(0.0f, bounce);
            if (0.0f == bounce && field_0x79 != field_0x78) {
                field_0x79 = field_0x78;
            }
        }
        field_0x2c->mPane->setAlpha((s32)alpha);
        field_0x4->search('Paccur01')->setAlpha((s32)alpha);
        lbl_8053AED8->fn_80440EE8();
    }
}

// FIXME(shotgun): 99.50 %, reloc-name
void cls_804B03D8::fn_803D82EC() {
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x1c->setAlpha(field_0x77);
    field_0x1c->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}

// Slides the panes in (the first frames of the screen); once all four have arrived, places the
// cursor (field_0x44) over the pane of the selected row.
// FIXME(shotgun): 88 %, missing/extra + reloc-name
void cls_804B03D8::fn_803D7098() {
    bool a = false;
    bool b = false;
    bool c = false;
    bool d = false;
    field_0x60++;
    if (field_0x60 > 4) {
        a = field_0x30->fn_80402940(0x16, 0, -200.0f, 0.0f);
    }
    if (field_0x60 > 0x10) {
        b = field_0x48->fn_80402940(0x18, 0, -200.0f, 0.0f);
    }
    if (field_0x60 > 0) {
        for (s32 i = 0; i < 4; i++) {
            c = field_0x34[i]->fn_80402940(0x16, 0, 500.0f, 0.0f);
        }
    }
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        d = true;
    } else {
        for (s32 i = 0; i < 2; i++) {
            if (field_0x60 > i * 5) {
                d = field_0x24[i]->fn_80402940(0x10, 0, 200.0f, 0.0f);
            }
        }
    }
    if (a && b && c && d) {
        field_0x2c->mPane->mIsVisible = true;
        field_0x78 = 0;
        field_0x79 = field_0x78;
        field_0x6e = 0;
        field_0x7a = 3;
        field_0x7f = 1;
        field_0x70 = 1;
        s16 k = field_0x68;
        if (k >= 0 && k < 4) {
            s16 j = field_0x6a;
            if (j >= 0) {
                f32 x0 = field_0x4->search(lbl_80477180[0])->getBounds()->i.x +
                         field_0x4->search(lbl_80477408[0])->getBounds()->i.x;
                f32 x1 = field_0x4->search(lbl_80477408[field_0x68])->getBounds()->i.x +
                         field_0x4->search(lbl_80477180[k * 16 + j])->getBounds()->i.x;
                f32 x2 = field_0x8->search('P0kak00')->getBounds()->i.x;
                f32 x3 = field_0x4->search('Ndata03')->getBounds()->i.x +
                         field_0x4->search(lbl_80477408[0])->getBounds()->i.x;
                field_0x58 = (field_0x4->search(lbl_80477180[0])->getBounds()->i.x + x3) - x2 + x1 - x0;
                f32 y0 = field_0x4->search(lbl_80477408[0])->getBounds()->i.y +
                         field_0x4->search(lbl_80477180[0])->getBounds()->i.y;
                f32 y1 = field_0x4->search(lbl_80477408[field_0x68])->getBounds()->i.y +
                         field_0x4->search(lbl_80477180[k * 16 + j])->getBounds()->i.y;
                f32 y2 = field_0x8->search('P0kak00')->getBounds()->i.y;
                f32 y3 = field_0x4->search('Ndata03')->getBounds()->i.y +
                         field_0x4->search(lbl_80477408[0])->getBounds()->i.y;
                field_0x5c = (field_0x4->search(lbl_80477180[0])->getBounds()->i.y + y3) - y2 + y1 - y0;
                field_0x44->moveFromCentre_80402E08(field_0x58, field_0x5c);
            }
        }
    }
}
