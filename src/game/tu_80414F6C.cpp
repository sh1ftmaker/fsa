#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/tu_80379F84.h"
#include "game/act/tu_80440BF4.h"
#include "game/cls_804B1600.h"
#include "game/cls_804B1378.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include <string.h>

// .rodata: the pane tag tables (J2DPane::search), in address order; the
// constructor addresses them off one base (lbl_8048CD50). The first five
// have an alpha array of the same length in BattleStageSelect_804B1378 (field_0x123 ...).
static const u64 lbl_8048CD50[11] = {
    'Pbsb01', 'Pbsb02', 'Pbsb03', 'Pbsb04', 'Pbsb05', 'Pbsb06',
    'Pbsb07', 'Pbsb08', 'Pbsb09', 'Pbsbk01', 'Pbsbk02',
};
static const u64 lbl_8048CDA8[11] = {
    'Tsn01', 'Tsn02', 'Psnb01', 'Psnbk01', 'Psnb02', 'Psnbk02',
    'Psnb03', 'Psnb04', 'Psnb05', 'Psnbk03', 'Psnbk04',
};
static const u64 lbl_8048CE00[9] = {
    'Pss01', 'Pss02', 'Pssb01', 'Pssb02', 'Pssb03', 'Pssb04',
    'Pssbk01', 'Pssbk02', 'Pssbk03',
};
static const u64 lbl_8048CE48[6] = {
    'Tom01', 'Tom02', 'Pomb01', 'Pomb02', 'Pomb03', 'Pomb04',
};
// The panes of each stage's button; row 6 has no tenth tag (zero in the
// original, skipped by fn_80416BD0).
static const u64 lbl_8048CE78[10][10] = {
    {'Pbsq01', 'Pbsqb01', 'Pbs01', 'Pbs01b01', 'Pbs01b02', 'Pbs01b03', 'Pbs01b04', 'Pbs01b05', 'Pbs01b06', 'Pbs01b07'},
    {'Pbsq02', 'Pbsqb02', 'Pbs02', 'Pbs02b01', 'Pbs02b02', 'Pbs02b03', 'Pbs02b04', 'Pbs02b05', 'Pbs02b06', 'Pbs02b07'},
    {'Pbsq03', 'Pbsqb03', 'Pbs03', 'Pbs03b01', 'Pbs03b02', 'Pbs03b03', 'Pbs03b04', 'Pbs03b05', 'Pbs03b06', 'Pbs03b07'},
    {'Pbsq04', 'Pbsqb04', 'Pbs04', 'Pbs04b01', 'Pbs04b02', 'Pbs04b03', 'Pbs04b04', 'Pbs04b05', 'Pbs04b06', 'Pbs04b07'},
    {'Pbsq05', 'Pbsqb05', 'Pbs05', 'Pbs05b01', 'Pbs05b02', 'Pbs05b03', 'Pbs05b04', 'Pbs05b05', 'Pbs05b06', 'Pbs05b07'},
    {'Pbsq06', 'Pbsqb06', 'Pbs06', 'Pbs06b01', 'Pbs06b02', 'Pbs06b03', 'Pbs06b04', 'Pbs06b05', 'Pbs06b06', 'Pbs06b07'},
    {'Pbsq07', 'Pbsqb07', 'Pbs07', 'Pbs07b01', 'Pbs07b02', 'Pbs07b03', 'Pbs07b04', 'Pbs07b05', 'Pbs07b06'},
    {'Pbsq08', 'Pbsqb08', 'Pbs08', 'Pbs08b01', 'Pbs08b02', 'Pbs08b03', 'Pbs08b04', 'Pbs08b05', 'Pbs08b06', 'Pbs08b07'},
    {'Pbsq09', 'Pbsqb09', 'Pbs09', 'Pbs09b01', 'Pbs09b02', 'Pbs09b03', 'Pbs09b04', 'Pbs09b05', 'Pbs09b06', 'Pbs09b07'},
    {'Pbsq10', 'Pbsqb10', 'Pbs10', 'Pbs10b01', 'Pbs10b02', 'Pbs10b03', 'Pbs10b04', 'Pbs10b05', 'Pbs10b06', 'Pbs10b07'},
};
// The stages' 'Ndata' panes (field_0x3c; setStageText moves the cursor there).
static const u64 lbl_8048D198[10] = {
    'Ndata06', 'Ndata07', 'Ndata08', 'Ndata09', 'Ndata10',
    'Ndata11', 'Ndata12', 'Ndata13', 'Ndata14', 'Ndata15',
};
// The stages' lock panes (field_0x6c); the constructor hides those of the
// open stages.
static const u64 lbl_8048D1E8[10] = {
    'Pbsqb01', 'Pbsqb02', 'Pbsqb03', 'Pbsqb04', 'Pbsqb05',
    'Pbsqb06', 'Pbsqb07', 'Pbsqb08', 'Pbsqb09', 'Pbsqb10',
};
// field_0xa4.
static const u64 lbl_8048D238[15] = {
    'Pbsk01', 'Pdot03', 'Pdot02', 'Pdot01', 'Pbsbb05', 'Pbsbb04', 'Pbsbb03',
    'Pbsbb02', 'Pbsbb01', 'Pbsb15', 'Pbsb14', 'Pbsb13', 'Pbsb12', 'Pbsb11',
    'Pbs11',
};
// The stages' name messages (setStageText).
static const s32 lbl_8048D2B0[12] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
};

// Run each frame by fn_804170B0 (index: field_0x109).
static BattleStageSelect_804B1378::StateFunc lbl_804B1354[3] = {
    &BattleStageSelect_804B1378::fn_80416038,
    &BattleStageSelect_804B1378::fn_80415028,
    &BattleStageSelect_804B1378::fn_80414F78,
};

s32 fn_80414F6C() {
    return lbl_8053AE00->field_0xe0;
}

void BattleStageSelect_804B1378::fn_80414F78() {
    switch (field_0x10a) {
    case 0:
        if (lbl_8053AEC4->fn_8042EC7C()) {
            field_0x10a = 1;
        }
        break;
    case 1:
        if (lbl_8053AD30->fn_80309C4C(0x1300, -1)) {
            field_0x10a = 2;
        }
        break;
    case 2:
        if (lbl_8053AEC4->fn_8042EBEC()) {
            field_0x10a = 0;
            field_0x109 = 1;
        }
        break;
    }
}

// Set the alpha of each pane of a tag table to its initial alpha (field_0x123
// ...) times rate, clamped to [0, 1]. No out-of-line copies: inlined into
// fn_80416038 and fn_80416BD0.
inline void BattleStageSelect_804B1378::fadeCD50(f32 rate) {
    if (rate < 0.0f) {
        rate = 0.0f;
    } else if (rate > 1.0f) {
        rate = 1.0f;
    }
    for (int i = 0; i < 11; i++) {
        mScreen->search(lbl_8048CD50[i])->setAlpha(field_0x123[i] * rate);
    }
}

inline void BattleStageSelect_804B1378::fadeCDA8(f32 rate) {
    if (rate < 0.0f) {
        rate = 0.0f;
    } else if (rate > 1.0f) {
        rate = 1.0f;
    }
    for (int i = 0; i < 11; i++) {
        mScreen->search(lbl_8048CDA8[i])->setAlpha(field_0x12e[i] * rate);
    }
}

inline void BattleStageSelect_804B1378::fadeCE00(f32 rate) {
    if (rate < 0.0f) {
        rate = 0.0f;
    } else if (rate > 1.0f) {
        rate = 1.0f;
    }
    for (int i = 0; i < 9; i++) {
        mScreen->search(lbl_8048CE00[i])->setAlpha(field_0x139[i] * rate);
    }
}

inline void BattleStageSelect_804B1378::fadeCE48(f32 rate) {
    if (rate < 0.0f) {
        rate = 0.0f;
    } else if (rate > 1.0f) {
        rate = 1.0f;
    }
    for (int i = 0; i < 6; i++) {
        mScreen->search(lbl_8048CE48[i])->setAlpha(field_0x142[i] * rate);
    }
}

// The button of stage `stage`; every button uses the alphas of the first one
// (field_0x148).
inline void BattleStageSelect_804B1378::fadeStage(int stage, f32 rate) {
    if (rate < 0.0f) {
        rate = 0.0f;
    } else if (rate > 1.0f) {
        rate = 1.0f;
    }
    for (int i = 0; i < 10; i++) {
        if (lbl_8048CE78[stage][i] != 0) {
            mScreen->search(lbl_8048CE78[stage][i])->setAlpha(field_0x148[i] * rate);
        }
    }
}

// The stage masks are u16 like field_0x106 (clrlwi 16 before the and / or).
inline void BattleStageSelect_804B1378::setStageText() {
    s32 stage = field_0xc * 5 + field_0x10;
    if (stage >= 10) {
        field_0x14 = 30;
        field_0x18 = 1;
        getMessageText_803832BC(field_0xe4, 0x109, 0, NULL, NULL);
        strcpy(static_cast<J2DTextBox*>(mScreen->search('Tsn01'))->getStringPtr(), field_0xe4);
        strcpy(static_cast<J2DTextBox*>(mScreen->search('Tsn02'))->getStringPtr(), field_0xe4);
    } else {
        if (field_0x14 > 30) {
            field_0x14 = 0;
        }
        if (field_0x106 & (u16)(1 << stage)) {
            getMessageText_803832BC(field_0xe4, lbl_8048D2B0[stage], 0, NULL, NULL);
        } else {
            field_0xe4[0] = 0;
        }
        strcpy(static_cast<J2DTextBox*>(mScreen->search('Tsn01'))->getStringPtr(), field_0xe4);
        strcpy(static_cast<J2DTextBox*>(mScreen->search('Tsn02'))->getStringPtr(), field_0xe4);
        f32 x = mScreen->search(lbl_8048D198[stage])->getBounds()->i.x;
        f32 y = mScreen->search(lbl_8048D198[stage])->getBounds()->i.y;
        mScreen->search('Ndata04')->move(x, y);
    }
}

void BattleStageSelect_804B1378::fn_80415028() {
    s32 row = field_0xc;
    s32 col = field_0x10;
    if (field_0x1c >= 0) {
        field_0x20--;
        if (field_0x1c > 0 && field_0x20 <= 0) {
            s32 pick = (field_0x104 - 1) * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            if (pick >= field_0x104 - 1) {
                pick = field_0x104 - 2;
            }
            u8 stage = field_0x117[pick];
            s32 n = 0;
            for (int i = 0; i < 10; i++) {
                if ((field_0x106 & (u16)(1 << i)) && i != stage) {
                    field_0x117[n++] = i;
                }
            }
            field_0x10 = stage % 5;
            field_0xc = stage / 5;
            setStageText();
            field_0x1c--;
            if (field_0x1c == 0) {
                field_0x20 = 20;
            } else {
                field_0x20 = 5;
            }
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
        if (field_0x1c != 0 || field_0x20 != 0) {
            return;
        }
    }
    if (field_0x1c == 0) {
        s32 stage = field_0xc * 5 + field_0x10;
        if (stage >= 5) {
            lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x81 = 1;
        } else {
            lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x81 = 0;
        }
        lbl_80529DEC.field_0x128 = stage / 4;
        lbl_80529DEC.field_0x12c = stage % 4;
        gAudioMgr_8053AF80->fn_80458F9C(10);
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        field_0xe0 = 7;
        lbl_80529DEC.field_0x242 = 0;
        lbl_80529DEC.field_0x252 = 1;
    } else if (lbl_8053AD30->fn_80309C4C(0x1000, -1)) {
        field_0xe0 = 4;
        lbl_80529DEC.field_0x237 = 3;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        s32 stage = field_0xc * 5 + field_0x10;
        if (stage >= 5) {
            lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x81 = 1;
        } else {
            lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x81 = 0;
        }
        if (stage < 10) {
            if (field_0x106 & (u16)(1 << stage)) {
                lbl_80529DEC.field_0x128 = stage / 4;
                lbl_80529DEC.field_0x12c = stage % 4;
                gAudioMgr_8053AF80->fn_80458F9C(10);
                gAudioMgr_8053AF80->fn_80458880(0xB, -1);
                field_0xe0 = 7;
                lbl_80529DEC.field_0x242 = 0;
                lbl_80529DEC.field_0x252 = 0;
            } else {
                lbl_8053AEC4->fn_8042EDAC(0x102, 0);
                field_0x10a = 0;
                field_0x109 = 2;
            }
        } else if (field_0x1c == -1) {
            field_0x1c = 25;
            field_0x20 = 1;
            field_0x18 = 1;
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x200, -1)) {
        field_0xe0 = 4;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    } else if (lbl_8053AD30->fn_80309A9C(1, -1)) {
        field_0x10--;
        if (field_0x10 < 0) {
            field_0x10 = 4;
        }
        if (field_0x10 != col) {
            s32 stage = field_0xc * 5 + field_0x10;
            if (stage < 10 || col + field_0xc * 5 < 10) {
                setStageText();
                gAudioMgr_8053AF80->fn_80458880(0x26, -1);
            }
        }
    } else if (lbl_8053AD30->fn_80309A9C(2, -1)) {
        field_0x10++;
        if (field_0xc < 2 && field_0x10 > 4) {
            field_0x10 = 0;
        }
        if (field_0x10 != col) {
            s32 stage = field_0xc * 5 + field_0x10;
            if (stage < 10 || col + field_0xc * 5 < 10) {
                setStageText();
                gAudioMgr_8053AF80->fn_80458880(0x26, -1);
            }
        }
    } else if (lbl_8053AD30->fn_80309A9C(8, -1)) {
        field_0xc--;
        if (field_0xc < 0) {
            field_0xc = 2;
        }
        if (field_0xc != row) {
            setStageText();
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309A9C(4, -1)) {
        field_0xc++;
        if (field_0xc > 2) {
            field_0xc = 0;
        }
        if (field_0xc != row) {
            setStageText();
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    }
}

void BattleStageSelect_804B1378::fn_80416038() {
    u8 done0 = false;
    u8 done1 = false;
    u8 done2 = false;
    u8 done3 = false;
    u8 done4 = false;
    u8 done5 = false;
    u8 done6 = false;
    field_0x102++;
    if (field_0x102 > 0) {
        for (int i = 0; i < 2; i++) {
            field_0x28[i]->fn_80402940(15, 0, 300.0f, 0.0f);
        }
        done0 = field_0x28[2]->fn_80402940(15, 0, 300.0f, 0.0f);
        if (field_0x102 < 15) {
            fadeCD50(field_0x102 / 15.0f);
        } else {
            fadeCD50(1.0f);
        }
    }
    if (field_0x102 > 0) {
        done1 = field_0x34->fn_80402940(25, 0, -100.0f, 0.0f);
        if (field_0x102 < 25) {
            fadeCE00(field_0x102 / 25.0f);
        } else {
            fadeCE00(1.0f);
        }
    }
    if (field_0x102 > 0) {
        done2 = field_0x38->fn_80402940(15, 0, -200.0f, 0.0f);
        for (int i = 0; i < 15; i++) {
            field_0xa4[i]->fn_80402940(15, 0, -200.0f, 0.0f);
        }
        if (field_0x102 < 15) {
            f32 rate = field_0x102 / 15.0f;
            fadeCDA8(rate);
            for (int i = 0; i < 15; i++) {
                field_0xa4[i]->mPane->setAlpha(rate * field_0xa4[i]->field_0x28);
            }
        } else {
            for (int i = 0; i < 15; i++) {
                field_0xa4[i]->mPane->setAlpha(field_0xa4[i]->field_0x28);
            }
            fadeCDA8(1.0f);
        }
    }
    for (int i = 0; i < 5; i++) {
        if (field_0x102 > i * 2) {
            field_0x3c[i]->fn_80402940(15, 0, 300.0f, 0.0f);
            if (i == 4) {
                done5 = field_0x6c[i]->fn_80402940(15, 0, 300.0f, 0.0f);
            } else {
                field_0x6c[i]->fn_80402940(15, 0, 300.0f, 0.0f);
            }
            if (field_0x102 - i * 2 < 15) {
                fadeStage(i, (field_0x102 - i * 2) / 15.0f);
            } else {
                fadeStage(i, 1.0f);
            }
        }
    }
    for (int i = 5; i < 10; i++) {
        if (field_0x102 > (i - 5) * 2 + 2) {
            field_0x3c[i]->fn_80402940(15, 0, 300.0f, 0.0f);
            if (i == 9) {
                done6 = field_0x6c[i]->fn_80402940(15, 0, 300.0f, 0.0f);
            } else {
                field_0x6c[i]->fn_80402940(15, 0, 300.0f, 0.0f);
            }
            if (field_0x102 - ((i - 5) * 2 + 2) < 15) {
                fadeStage(i, (field_0x102 - ((i - 5) * 2 + 2)) / 15.0f);
            } else {
                fadeStage(i, 1.0f);
            }
        }
    }
    if (field_0x102 > 8) {
        done3 = field_0x9c->fn_80402940(15, 0, 300.0f, 0.0f);
        if (field_0x102 - 8 < 15) {
            fadeCE48((field_0x102 - 8) / 15.0f);
        } else {
            fadeCE48(1.0f);
        }
    }
    if (field_0x102 > 20) {
        done4 = field_0xa0->fn_80402940(25, 0, -200.0f, 0.0f);
    }
    if (done0 && done1 && done2 && done3 && done4 && done5 && done6) {
        field_0x109 = 1;
    }
}

void BattleStageSelect_804B1378::fn_80416BD0() {
    lbl_8053AEC4->fn_8042EB24();
    field_0x1c = -1;
    field_0x20 = 5;
    field_0x109 = 0;
    field_0x10a = 0;
    field_0x102 = 0;
    fadeCD50(0.0f);
    fadeCDA8(0.0f);
    fadeCE00(0.0f);
    fadeCE48(0.0f);
    for (int j = 0; j < 10; j++) {
        fadeStage(j, 0.0f);
    }
    for (int i = 0; i < 3; i++) {
        field_0x28[i]->moveFromCentre_80402E08(0.0f, 300.0f);
        field_0x28[i]->field_0x22 = 0;
    }
    field_0x34->moveFromCentre_80402E08(0.0f, -100.0f);
    field_0x34->field_0x22 = 0;
    field_0x38->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x38->field_0x22 = 0;
    for (int i = 0; i < 10; i++) {
        field_0x3c[i]->moveFromCentre_80402E08(0.0f, 300.0f);
        field_0x3c[i]->field_0x22 = 0;
        field_0x6c[i]->moveFromCentre_80402E08(0.0f, 300.0f);
        field_0x6c[i]->field_0x22 = 0;
    }
    field_0x9c->moveFromCentre_80402E08(0.0f, 300.0f);
    field_0x9c->field_0x22 = 0;
    field_0xa0->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0xa0->field_0x22 = 0;
    mScreen->search('Pscur01')->hide();
    mScreen->search('Pscur02')->hide();
    mScreen->search('Pscur03')->hide();
    mScreen->search('Pocur01')->setAlpha(0);
    mScreen->search('Pocur02')->setAlpha(0);
    field_0xe0 = 6;
}

void BattleStageSelect_804B1378::fn_804170B0() {
    (lbl_8053AE00->*lbl_804B1354[field_0x109])();
}

// FIXME: 99.93 %. Only the frame differs: ours is 0x60 bytes, the original's
// 0x50; the int-to-float temporaries of the 30..59 blink branch and the
// offset sit 8 / 16 bytes higher than the original's 0x10 / 0x18 slots.
void BattleStageSelect_804B1378::fn_804170EC() {
    s32 stage = field_0xc * 5 + field_0x10;
    JFWDisplay::getManager()->setClearColor(JUtility::TColor(0, 0, 0, 0));
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0x109 != 0) {
        u8 alpha;
        if (stage < 10) {
            field_0x14++;
            if (field_0x14 >= 30) {
                field_0x14 = 0;
            }
            if (field_0x14 < 10) {
                mScreen->search('Pscur01')->show();
                mScreen->search('Pscur02')->hide();
                mScreen->search('Pscur03')->hide();
            } else if (field_0x14 < 20) {
                mScreen->search('Pscur01')->hide();
                mScreen->search('Pscur02')->show();
                mScreen->search('Pscur03')->hide();
            } else {
                mScreen->search('Pscur01')->hide();
                mScreen->search('Pscur02')->hide();
                mScreen->search('Pscur03')->show();
            }
            alpha = 0;
        } else {
            f32 rate;
            field_0x14++;
            if (field_0x14 >= 60) {
                field_0x14 = 0;
            }
            if (field_0x14 < 30) {
                rate = (f32)(field_0x14 * field_0x14) / 900.0f;
            } else {
                rate = 1.0f - (f32)((field_0x14 - 30) * (field_0x14 - 30)) / 900.0f;
            }
            mScreen->search('Pscur01')->hide();
            mScreen->search('Pscur02')->hide();
            mScreen->search('Pscur03')->hide();
            alpha = rate * field_0x108;
        }
        if (field_0x18 > 0) {
            s32 t = 10 - field_0x18;
            if (field_0x18 < 5) {
                t = field_0x18;
            }
            if (field_0x18 >= 10) {
                field_0x18 = 0;
            } else {
                field_0x18++;
            }
            f32 offset = 5.0f * (t * t) / 25.0f;
            f32 x = mScreen->search('Ndata05')->getBounds()->i.x;
            mScreen->search('Ndata05')->move(x, field_0x4 + offset);
            x = mScreen->search('Pocur02')->getBounds()->i.x;
            mScreen->search('Pocur02')->move(x, field_0x8 + offset);
        }
        mScreen->search('Pocur01')->setAlpha(alpha);
        mScreen->search('Pocur02')->setAlpha(alpha);
    }
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x109 == 2) {
        lbl_8053AEC4->fn_8042EB98();
    }
    lbl_8053AED8->fn_80440EE8();
}

BattleStageSelect_804B1378::~BattleStageSelect_804B1378() {
    lbl_8053AEC4->fn_8042EB14();
    delete mScreen;
    mScreen = NULL;
    for (int i = 0; i < 3; i++) {
        delete field_0x28[i];
        field_0x28[i] = NULL;
    }
    delete field_0x34;
    field_0x34 = NULL;
    delete field_0x38;
    field_0x38 = NULL;
    for (int i = 0; i < 10; i++) {
        delete field_0x3c[i];
        field_0x3c[i] = NULL;
        delete field_0x6c[i];
        field_0x6c[i] = NULL;
    }
    delete field_0x9c;
    field_0x9c = NULL;
    for (int i = 0; i < 15; i++) {
        delete field_0xa4[i];
        field_0xa4[i] = NULL;
    }
    lbl_8053AE00 = NULL;
}

// FIXME: 99.96 %, regalloc only: `font` is in r27 here, r29 in the original.
BattleStageSelect_804B1378::BattleStageSelect_804B1378() {
    lbl_8053AE00 = this;
    field_0xe4[0] = 0;
    mScreen = new J2DScreen;
    mScreen->setPriority("bat_stage_select.blo", 0x1100000, lbl_80529DEC.field_0x34);
    JUTResFont* font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(mScreen->search('Tom01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(mScreen->search('Tom02'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(mScreen->search('Tsn01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(mScreen->search('Tsn02'))->setFont(font);
    getMessageText_803832BC(static_cast<J2DTextBox*>(mScreen->search('Tom01'))->getStringPtr(), 0x100, 0, NULL, NULL);
    getMessageText_803832BC(static_cast<J2DTextBox*>(mScreen->search('Tom02'))->getStringPtr(), 0x100, 0, NULL, NULL);
    for (int i = 0; i < 11; i++) {
        field_0x123[i] = mScreen->search(lbl_8048CD50[i])->getAlpha();
        field_0x12e[i] = mScreen->search(lbl_8048CDA8[i])->getAlpha();
    }
    for (int i = 0; i < 9; i++) {
        field_0x139[i] = mScreen->search(lbl_8048CE00[i])->getAlpha();
    }
    for (int i = 0; i < 6; i++) {
        field_0x142[i] = mScreen->search(lbl_8048CE48[i])->getAlpha();
    }
    for (int i = 0; i < 10; i++) {
        field_0x148[i] = mScreen->search(lbl_8048CE78[0][i])->getAlpha();
    }
    field_0x28[0] = new PaneCtrl_804AF86C;
    field_0x28[0]->setPane_80402F04(mScreen->search('Pbsbk02'));
    field_0x28[1] = new PaneCtrl_804AF86C;
    field_0x28[1]->setPane_80402F04(mScreen->search('Pbsbk01'));
    field_0x28[2] = new PaneCtrl_804AF86C;
    field_0x28[2]->setPane_80402F04(mScreen->search('Pbsb09'));
    field_0x34 = new PaneCtrl_804AF86C;
    field_0x34->setPane_80402F04(mScreen->search('Ndata03'));
    field_0x38 = new PaneCtrl_804AF86C;
    field_0x38->setPane_80402F04(mScreen->search('Ndata02'));
    // The tags are immediates here, not loads from lbl_8048D198 / lbl_8048D1E8.
    field_0x3c[0] = new PaneCtrl_804AF86C;
    field_0x3c[0]->setPane_80402F04(mScreen->search('Ndata06'));
    field_0x6c[0] = new PaneCtrl_804AF86C;
    field_0x6c[0]->setPane_80402F04(mScreen->search('Pbsqb01'));
    field_0x3c[1] = new PaneCtrl_804AF86C;
    field_0x3c[1]->setPane_80402F04(mScreen->search('Ndata07'));
    field_0x6c[1] = new PaneCtrl_804AF86C;
    field_0x6c[1]->setPane_80402F04(mScreen->search('Pbsqb02'));
    field_0x3c[2] = new PaneCtrl_804AF86C;
    field_0x3c[2]->setPane_80402F04(mScreen->search('Ndata08'));
    field_0x6c[2] = new PaneCtrl_804AF86C;
    field_0x6c[2]->setPane_80402F04(mScreen->search('Pbsqb03'));
    field_0x3c[3] = new PaneCtrl_804AF86C;
    field_0x3c[3]->setPane_80402F04(mScreen->search('Ndata09'));
    field_0x6c[3] = new PaneCtrl_804AF86C;
    field_0x6c[3]->setPane_80402F04(mScreen->search('Pbsqb04'));
    field_0x3c[4] = new PaneCtrl_804AF86C;
    field_0x3c[4]->setPane_80402F04(mScreen->search('Ndata10'));
    field_0x6c[4] = new PaneCtrl_804AF86C;
    field_0x6c[4]->setPane_80402F04(mScreen->search('Pbsqb05'));
    field_0x3c[5] = new PaneCtrl_804AF86C;
    field_0x3c[5]->setPane_80402F04(mScreen->search('Ndata11'));
    field_0x6c[5] = new PaneCtrl_804AF86C;
    field_0x6c[5]->setPane_80402F04(mScreen->search('Pbsqb06'));
    field_0x3c[6] = new PaneCtrl_804AF86C;
    field_0x3c[6]->setPane_80402F04(mScreen->search('Ndata12'));
    field_0x6c[6] = new PaneCtrl_804AF86C;
    field_0x6c[6]->setPane_80402F04(mScreen->search('Pbsqb07'));
    field_0x3c[7] = new PaneCtrl_804AF86C;
    field_0x3c[7]->setPane_80402F04(mScreen->search('Ndata13'));
    field_0x6c[7] = new PaneCtrl_804AF86C;
    field_0x6c[7]->setPane_80402F04(mScreen->search('Pbsqb08'));
    field_0x3c[8] = new PaneCtrl_804AF86C;
    field_0x3c[8]->setPane_80402F04(mScreen->search('Ndata14'));
    field_0x6c[8] = new PaneCtrl_804AF86C;
    field_0x6c[8]->setPane_80402F04(mScreen->search('Pbsqb09'));
    field_0x3c[9] = new PaneCtrl_804AF86C;
    field_0x3c[9]->setPane_80402F04(mScreen->search('Ndata15'));
    field_0x6c[9] = new PaneCtrl_804AF86C;
    field_0x6c[9]->setPane_80402F04(mScreen->search('Pbsqb10'));
    field_0x9c = new PaneCtrl_804AF86C;
    field_0x9c->setPane_80402F04(mScreen->search('Ndata05'));
    for (int i = 0; i < 15; i++) {
        field_0xa4[i] = new PaneCtrl_804AF86C;
        field_0xa4[i]->setPane_80402F04(mScreen->search(lbl_8048D238[i]));
        field_0xa4[i]->mPane->setAlpha(0);
    }
    field_0xa0 = lbl_8053AED8->field_0x8;
    const ResTIMG* tex = (const ResTIMG*)fn_804030C4('TIMG', "kk_kettei_01.bti");
    lbl_8053AED8->fn_80440D78(tex);
    lbl_8053AED8->fn_80440CE4((const ResTIMG*)fn_804030C4('TIMG', "kk_modoru_01.bti"));
    field_0x106 = 3;
    mScreen->search(lbl_8048D1E8[0])->hide();
    mScreen->search(lbl_8048D1E8[1])->hide();
    for (int i = 0; i < 8; i++) {
        if (fn_803B9120(i)) {
            mScreen->search(lbl_8048D1E8[i + 2])->hide();
            field_0x106 |= (u16)(1 << (i + 2));
        } else {
            mScreen->search(lbl_8048D1E8[i + 2])->show();
        }
    }
    field_0x104 = 0;
    for (int i = 0; i < 10; i++) {
        field_0x10b[i] = 0;
        field_0x117[i] = 0;
        if (field_0x106 & (u16)(1 << i)) {
            field_0x117[field_0x104] = i;
            field_0x10b[field_0x104] = i;
            field_0x104++;
        }
    }
    field_0x108 = 156;
    field_0x4 = mScreen->search('Ndata05')->getBounds()->i.y;
    field_0x8 = mScreen->search('Pocur02')->getBounds()->i.y;
    if (lbl_80529DEC.field_0x252 == 0) {
        s32 stage = lbl_80529DEC.field_0x128 * 4 + lbl_80529DEC.field_0x12c;
        field_0x10 = stage % 5;
        field_0xc = stage / 5;
    } else {
        field_0x10 = 0;
        field_0xc = 2;
    }
    field_0x14 = 0;
    field_0x18 = 0;
    field_0x1c = -1;
    field_0x20 = 0;
    setStageText();
    field_0xe0 = 6;
}
