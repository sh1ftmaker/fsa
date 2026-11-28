#include "game/EmitterDelete.h"
#include "game/GameAudio.h"
#include "game/Message.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80379F84.h"
#include "game/act/button_icons_80440BF4.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DAnmLoader.h"

static const u64 lbl_80476F80[4][16] = {
    { 'P1ka01', 'P1ka02', 'P1ka03', 'P1ka04', 'P1ka05', 'P1ka06', 'P1ka07', 'P1ka08',
      'P1ka09', 'P1ka10', 'P1ka11', 'P1ka12', 'P1ka13', 'P1ka14', 'P1ka15', 'P1ka16' },
    { 'P2ka01', 'P2ka02', 'P2ka03', 'P2ka04', 'P2ka05', 'P2ka06', 'P2ka07', 'P2ka08',
      'P2ka09', 'P2ka10', 'P2ka11', 'P2ka12', 'P2ka13', 'P2ka14', 'P2ka15', 'P2ka16' },
    { 'P3ka01', 'P3ka02', 'P3ka03', 'P3ka04', 'P3ka05', 'P3ka06', 'P3ka07', 'P3ka08',
      'P3ka09', 'P3ka10', 'P3ka11', 'P3ka12', 'P3ka13', 'P3ka14', 'P3ka15', 'P3ka16' },
    { 'P4ka01', 'P4ka02', 'P4ka03', 'P4ka04', 'P4ka05', 'P4ka06', 'P4ka07', 'P4ka08',
      'P4ka09', 'P4ka10', 'P4ka11', 'P4ka12', 'P4ka13', 'P4ka14', 'P4ka15', 'P4ka16' },
};

static const u64 lbl_80477180[4][16] = {
    { 'P1kak01', 'P1kak02', 'P1kak03', 'P1kak04', 'P1kak05', 'P1kak06', 'P1kak07', 'P1kak08',
      'P1kak09', 'P1kak10', 'P1kak11', 'P1kak12', 'P1kak13', 'P1kak14', 'P1kak15', 'P1kak16' },
    { 'P2kak01', 'P2kak02', 'P2kak03', 'P2kak04', 'P2kak05', 'P2kak06', 'P2kak07', 'P2kak08',
      'P2kak09', 'P2kak10', 'P2kak11', 'P2kak12', 'P2kak13', 'P2kak14', 'P2kak15', 'P2kak16' },
    { 'P3kak01', 'P3kak02', 'P3kak03', 'P3kak04', 'P3kak05', 'P3kak06', 'P3kak07', 'P3kak08',
      'P3kak09', 'P3kak10', 'P3kak11', 'P3kak12', 'P3kak13', 'P3kak14', 'P3kak15', 'P3kak16' },
    { 'P4kak01', 'P4kak02', 'P4kak03', 'P4kak04', 'P4kak05', 'P4kak06', 'P4kak07', 'P4kak08',
      'P4kak09', 'P4kak10', 'P4kak11', 'P4kak12', 'P4kak13', 'P4kak14', 'P4kak15', 'P4kak16' },
};

static const u64 lbl_80477380[4] = { 'Pbcur01', 'Pbcur02', 'Pbcur03', 'Pbcur04' };
static const u64 lbl_804773A0[3] = { 'Pblk01', 'Pblk02', 'Pblk03' };
static const u64 lbl_804773B8[3] = { 'Ndata002', 'Ndata003', 'Ndata004' };
static const u64 lbl_804773D0[3] = { 'P2P02', 'P3P02', 'P4P02' };
static const u64 lbl_804773E8[4] = { 'Pp1i03', 'Pp2i03', 'Pp3i03', 'Pp4i03' };
static const u64 lbl_80477408[4] = { 'Ndata11', 'Ndata12', 'Ndata13', 'Ndata14' };

// FIXME(shotgun): 99.94 %, same-text; behaviour drift.
TotalScreen_804B03D8::TotalScreen_804B03D8() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    lbl_8053ADC0 = this;
    field_0x4 = new J2DScreen;
    field_0x4->setPriority("total.blo", 0x1100000, lbl_8053AD68->field_0x14);
    field_0x8 = new J2DScreen;
    field_0x8->setPriority("total_star.blo", 0x1100000, lbl_8053AD68->field_0x14);
    const ResTIMG* tex = (const ResTIMG*)getMain2DResource_804030C4('TIMG', "kk_black.bti");
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
    field_0x4->search('Ndata05')->hide();
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
    field_0x7b = field_0x4->search(lbl_80476F80[0][0])->mAlpha;
    field_0x7c = field_0x4->search(lbl_80477180[0][0])->mAlpha;
    field_0x7d = field_0x8->search('Pstarli')->mAlpha;
    field_0x7e = 0;
    field_0x7f = 0;
    field_0x80 = 0;
    field_0x81 = 0;
}

TotalScreen_804B03D8::~TotalScreen_804B03D8() {
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

static inline void showPlayerRow(TotalScreen_804B03D8* self, s32 k) {
    u8 mask = 1 << k;
    s32 count = lbl_80529DEC.field_0x140[k];
    if (lbl_80529DEC.isField0x243(mask)) {
        self->field_0x68 = k;
        self->field_0x4->search(lbl_804773E8[k])->show();
        self->field_0x4->search(lbl_80477380[k])->show();
    } else {
        self->field_0x4->search(lbl_804773E8[k])->hide();
        self->field_0x4->search(lbl_80477380[k])->hide();
    }
    for (s32 j = 0; j < 16; j++) {
        if (j < count && (gGameRoot_8053AAF8->mPlayerMask & mask) != 0) {
            if ((lbl_80529DEC.field_0x243[0] & mask) != 0 && j == count - 1) {
                self->field_0x6a = j;
                self->field_0x4->search(lbl_80476F80[k][j])->hide();
                self->field_0x4->search(lbl_80477180[k][j])->hide();
            } else {
                self->field_0x4->search(lbl_80476F80[k][j])->show();
                self->field_0x4->search(lbl_80477180[k][j])->show();
                self->field_0x4->search(lbl_80477180[k][j])->setAlpha(0);
            }
        } else {
            self->field_0x4->search(lbl_80476F80[k][j])->hide();
            self->field_0x4->search(lbl_80477180[k][j])->hide();
        }
    }
}

// Resets the screen for a new round: shows the rows of the players that take part and slides the
// remaining panes out of view.
void TotalScreen_804B03D8::fn_803D87C0() {
    lbl_8053AEC0->fn_804310CC();
    for (s32 i = 0; i < 3; i++) {
        if (i >= gGameRoot_8053AAF8->getPlayerCount_80138A30() - 1) {
            field_0x4->search(lbl_804773A0[i])->show();
            field_0x4->search(lbl_804773B8[i])->hide();
            field_0x4->search(lbl_804773D0[i])->hide();
        } else {
            field_0x4->search(lbl_804773A0[i])->hide();
            field_0x4->search(lbl_804773B8[i])->show();
            field_0x4->search(lbl_804773D0[i])->show();
        }
    }
    for (s32 k = 0; k < 4; k++) {
        showPlayerRow(this, k);
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
    const ResTIMG* tex = (const ResTIMG*)getMain2DResource_804030C4('TIMG', "kk_kettei_01.bti");
    lbl_8053AED8->fn_80440D78(tex);
    getMain2DResource_804030C4('TIMG', "kk_modoru_01.bti");
    lbl_8053AED8->fn_80440BF4(false);
    field_0x2c->mPane->hide();
    field_0x2c->mPane->setAlpha(0);
    field_0x4->search('Paccur01')->setAlpha(0);
    for (s32 i = 0; i < 2; i++) {
        field_0x24[i]->moveFromCentre_80402E08(0.0f, 200.0f);
        field_0x24[i]->field_0x22 = 0;
    }
    lbl_80529DEC.field_0x243[0] = 0;
    gAudioMgr_8053AF80->startBgm_80458FF0(0x1000005, true, 0);
}

void TotalScreen_804B03D8::fn_803D8718() {
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

void TotalScreen_804B03D8::fn_803D8670() {
    gGameRoot2_8053AB10->setPort_8013B490();
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x8->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x1c->setAlpha(field_0x77);
    field_0x1c->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}

void TotalScreen_804B03D8::fn_803D8364() {
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
            if (field_0x79 == 0) {
                field_0x2c->moveFromCentre_80402E08(0.0f, bounce);
                field_0x24[0]->moveFromCentre_80402E08(0.0f, bounce);
                field_0x24[1]->moveFromCentre_80402E08(0.0f, 0.0f);
            } else {
                field_0x2c->moveFromCentre_80402E08(field_0x24[1]->field_0x8 - field_0x24[0]->field_0x8, bounce);
                field_0x24[0]->moveFromCentre_80402E08(0.0f, 0.0f);
                field_0x24[1]->moveFromCentre_80402E08(0.0f, bounce);
            }
            if (0.0f == bounce && field_0x79 != field_0x78) {
                field_0x79 = field_0x78;
            }
        }
        field_0x2c->mPane->setAlpha(alpha);
        field_0x4->search('Paccur01')->setAlpha(alpha);
        lbl_8053AED8->draw_80440EE8();
    }
}

void TotalScreen_804B03D8::fn_803D82EC() {
    gGameRoot2_8053AB10->setPort_8013B490();
    field_0x1c->setAlpha(field_0x77);
    field_0x1c->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}

// FIXME(shotgun): 99.87 %, regalloc-only; behaviour clean.
void TotalScreen_804B03D8::fn_803D75B0() {
    if (field_0x80 == 1) {
        lbl_8053AEC0->fn_80431174(field_0x78);
    }
    switch (field_0x7f) {
    case 0:
        fn_803D7098();
        break;
    case 1:
        if (lbl_8053AD30->fn_80309C4C(1, -1)) {
            if (field_0x78 != 0) {
                field_0x78--;
                field_0x79 = field_0x78;
                field_0x6e = 1;
                gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
            }
        } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
            if (field_0x78 < 1) {
                field_0x78++;
                field_0x79 = field_0x78;
                field_0x6e = 1;
                gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
            }
        } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
            field_0x72 = 0;
            field_0x7a = 2;
            field_0x79 = field_0x78;
            if (field_0x78 == 0) {
                field_0x76 = 8;
                gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
            } else {
                lbl_8053AEC0->fn_80431A9C(0x10B, 0, &field_0x78);
                field_0x7f = 2;
                gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
            }
            field_0x6e = 1;
        }
        break;
    case 2:
        if (field_0x81 > 0x48) {
            field_0x81 = 0x50;
        } else {
            field_0x81 += 8;
        }
        if (lbl_8053AEC0->fn_804317AC()) {
            field_0x81 = 0x50;
            field_0x7f = 3;
        }
        break;
    case 3:
        lbl_8053AEC0->fn_80431174(field_0x78);
        if (lbl_8053AD30->fn_80309A9C(2, -1)) {
            if (field_0x78 == 0) {
                field_0x78 = 1;
                gAudioMgr_8053AF80->playSystemSound_80458880(0, -1);
            }
        } else if (lbl_8053AD30->fn_80309A9C(1, -1)) {
            if (field_0x78 == 1) {
                field_0x78 = 0;
                gAudioMgr_8053AF80->playSystemSound_80458880(0, -1);
            }
        } else if (lbl_8053AD30->fn_80309C4C(0x1100, -1)) {
            field_0x7f = 4;
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        }
        break;
    case 4:
        if (field_0x81 < 8) {
            field_0x81 = 0;
        } else {
            field_0x81 -= 8;
        }
        if (lbl_8053AEC0->fn_80431740()) {
            field_0x81 = 0;
            if (field_0x78 == 0) {
                lbl_8053AEC0->fn_804310BC();
                field_0x76 = 7;
            } else {
                field_0x7f = 1;
            }
        }
        break;
    }
    if (field_0x70 != 0) {
        field_0x64++;
        if (field_0x64 >= 0x8C) {
            field_0x64 = 0x14;
        }
        if (field_0x7e == 0) {
            if (field_0x64 <= 0x14) {
                if (field_0x6a >= 0) {
                    f32 t = (f32)field_0x64 / 20.0f;
                    field_0x8->search('P0ka00')->setAlpha(((f32)field_0x7b * t));
                    static_cast<J2DPicture*>(field_0x8->search('P0ka00'))->setBlackWhite(
                        static_cast<J2DPicture*>(field_0x4->search(lbl_80476F80[field_0x68][field_0x6a]))->getBlack(),
                        static_cast<J2DPicture*>(field_0x4->search(lbl_80476F80[field_0x68][field_0x6a]))->getWhite());
                    J2DPicture::TCornerColor cornerColor;
                    static_cast<J2DPicture*>(field_0x4->search(lbl_80476F80[field_0x68][field_0x6a]))->getCornerColor(cornerColor);
                    static_cast<J2DPicture*>(field_0x8->search('P0ka00'))->setCornerColor(cornerColor);
                    field_0x8->search('Pstarli')->setAlpha(((f32)field_0x7d * t));
                }
            } else if (field_0x64 <= 0x28) {
                f32 t = (f32)(field_0x64 - 0x14) / 20.0f;
                for (s32 i = 0; i < 4; i++) {
                    for (s32 j = 0; j < 16; j++) {
                        if (field_0x4->search(lbl_80477180[i][j])->mIsVisible) {
                            field_0x4->search(lbl_80477180[i][j])->setAlpha(((f32)field_0x7c * t));
                        }
                    }
                }
                if (field_0x6a >= 0) {
                    field_0x8->search('P0kak00')->setAlpha(((f32)field_0x7c * t));
                }
            } else {
                field_0x7e = 1;
            }
        }
        if (field_0x6a >= 0) {
            f32 x = (field_0x58 + field_0x8->search('P0kak00')->getBounds()->i.x +
                     field_0x8->search('P0ka00')->getBounds()->i.x) +
                    field_0x8->search('P0ka00')->getWidth() * 0.5f;
            f32 y = (field_0x5c + field_0x8->search('P0kak00')->getBounds()->i.y +
                     field_0x8->search('P0ka00')->getBounds()->i.y) +
                    field_0x8->search('P0ka00')->getHeight() * 0.5f;
            if (field_0x64 == 0x50 && field_0x4c[2] == NULL) {
                field_0x4c[2] = spawnEmitter_8013CC50(0, &JGeometry::TVec3f(x, y, 0.0f), 0x4D0, 0, 0, NULL, NULL, true, 1.0f);
                field_0x4c[2]->setStatus(0x40);
            }
            field_0x10->setFrame(field_0x64);
            field_0x8->search('P0kak00')->animationTransform();
            if (field_0x64 > 1 && field_0x64 < 0x14) {
                if (field_0x4c[0] == NULL) {
                    field_0x4c[0] = spawnEmitter_8013CC50(0, &JGeometry::TVec3f(x, y, 0.0f), 0x4CF, 0, 0, NULL, NULL, true, 1.0f);
                }
                field_0x4c[0]->setGlobalTranslation(x, y, 0.0f);
                if (field_0x64 == 0x13 && field_0x4c[1] == NULL) {
                    field_0x4c[1] = spawnEmitter_8013CC50(0, &JGeometry::TVec3f(x, y, 0.0f), 0x4D1, 0, 0, NULL, NULL, true, 1.0f);
                    field_0x4c[1]->setStatus(0x40);
                }
            } else if (field_0x4c[0] != NULL) {
                field_0x4c[0]->becomeInvalidEmitter();
                field_0x4c[0] = NULL;
            }
            if (field_0x64 >= 0x14) {
                field_0x18->setFrame(field_0x64);
                field_0x8->animation();
            }
        }
    }
    if (field_0x68 >= 0 && field_0x68 < 4) {
        field_0x74++;
        if (field_0x74 >= 0x78) {
            field_0x74 = 0;
        }
        s32 step = field_0x74 < 0x3C ? field_0x74 : 0x78 - field_0x74;
        field_0x4->search(lbl_80477380[field_0x68])->setAlpha((80.0f * ((f32)step / 60.0f)));
    }
    if (field_0x80 == 2) {
        if (field_0x81 > 0x48) {
            field_0x81 = 0x50;
        } else {
            field_0x81 += 8;
        }
        if (lbl_8053AEC0->fn_804317AC()) {
            field_0x81 = 0x50;
            field_0x80 = 1;
        }
    } else if (field_0x80 == 3) {
        if (field_0x81 < 8) {
            field_0x81 = 0;
        } else {
            field_0x81 -= 8;
        }
        if (lbl_8053AEC0->fn_80431740()) {
            field_0x81 = 0;
            field_0x80 = 0;
        }
    }
    for (s32 i = 1; i < 3; i++) {
        if (field_0x4c[i] != NULL && field_0x4c[i]->isEnableDeleteEmitter()) {
            JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
            manager->forceDeleteEmitter(field_0x4c[i]);
            field_0x4c[i] = NULL;
        }
    }
}

// Slides the panes in (the first frames of the screen); once all four have arrived, places the
// cursor (field_0x44) over the pane of the selected row.
void TotalScreen_804B03D8::fn_803D7098() {
    u8 a = false;
    u8 b = false;
    u8 c = false;
    u8 d = false;
    field_0x60++;
    if (field_0x60 > 4) {
        a = field_0x30->animateMoveY_80402940(0x16, 0, -200.0f, 0.0f);
    }
    if (field_0x60 > 0x10) {
        b = field_0x48->animateMoveY_80402940(0x18, 0, -200.0f, 0.0f);
    }
    if (field_0x60 > 0) {
        for (s32 i = 0; i < 4; i++) {
            c = field_0x34[i]->animateMoveY_80402940(0x16, 0, 500.0f, 0.0f);
        }
    }
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        d = true;
    } else {
        for (s32 i = 0; i < 2; i++) {
            if (field_0x60 > i * 5) {
                d = field_0x24[i]->animateMoveY_80402940(0x10, 0, 200.0f, 0.0f);
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
        if (field_0x68 >= 0 && field_0x68 < 4 && field_0x6a >= 0) {
            field_0x58 =
                (((field_0x4->search(lbl_80477180[0][0])->getBounds()->i.x +
                   (field_0x4->search('Ndata03')->getBounds()->i.x +
                    field_0x4->search(lbl_80477408[0])->getBounds()->i.x)) -
                  field_0x8->search('P0kak00')->getBounds()->i.x) +
                 (field_0x4->search(lbl_80477408[field_0x68])->getBounds()->i.x +
                  field_0x4->search(lbl_80477180[field_0x68][field_0x6a])->getBounds()->i.x)) -
                (field_0x4->search(lbl_80477408[0])->getBounds()->i.x +
                 field_0x4->search(lbl_80477180[0][0])->getBounds()->i.x);
            field_0x5c =
                (((field_0x4->search(lbl_80477180[0][0])->getBounds()->i.y +
                   (field_0x4->search('Ndata03')->getBounds()->i.y +
                    field_0x4->search(lbl_80477408[0])->getBounds()->i.y)) -
                  field_0x8->search('P0kak00')->getBounds()->i.y) +
                 (field_0x4->search(lbl_80477408[field_0x68])->getBounds()->i.y +
                  field_0x4->search(lbl_80477180[field_0x68][field_0x6a])->getBounds()->i.y)) -
                (field_0x4->search(lbl_80477408[0])->getBounds()->i.y +
                 field_0x4->search(lbl_80477180[0][0])->getBounds()->i.y);
            field_0x44->moveFromCentre_80402E08(field_0x58, field_0x5c);
        }
    }
}
