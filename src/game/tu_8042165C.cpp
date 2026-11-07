#include "game/GameAudio.h"
#include "game/cls_804B146C.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "game/act/tu_80441318.h"
#include "game/GameGlobals.h"
#include "game/tu_803B8C74.h"
#include "game/Camera.h"
#include "game/Message.h"

// FIXME(shotgun): pane-tag table (0x8048E300, 0xC20 bytes) not written out yet (exceeds gate literal limit).
extern const u64 lbl_8048E300[0xC20 / 8];

static const u64 lbl_8048EF20[4] = {
    0x0050313173743032ull,
    0x0050323173743032ull,
    0x0050333173743032ull,
    0x0050343173743032ull,
};

// FIXME(shotgun): pane-tag table (0x8048EF40, 0x218 bytes) not written out yet (exceeds gate literal limit).
extern const u64 lbl_8048EF40[0x218 / 8];

static const u64 lbl_8048F158[6] = {
    0x00000050746B3031ull,
    0x0000000050743031ull,
    0x00000050746B3032ull,
    0x0000000050743032ull,
    0x00005074696D3031ull,
    0x00005074696D3032ull,
};

static const f32 lbl_8048F188[10] = {
    102.5f, 502.5f, 102.5f, 502.5f, 302.5f,
    302.5f, 102.5f, 102.5f, 502.5f, 502.5f,
};

static const f32 lbl_8048F1B0[10] = {
    60.0f, 60.0f, 420.0f, 420.0f, 60.0f,
    420.0f, 180.0f, 300.0f, 180.0f, 300.0f,
};

static const u8 lbl_8048F1D8[4][10] = {
    { 0, 1, 2, 3, 4, 6, 7, 5, 8, 9 },
    { 1, 3, 0, 2, 4, 8, 9, 5, 6, 7 },
    { 2, 0, 3, 1, 5, 7, 6, 4, 9, 8 },
    { 3, 2, 1, 0, 5, 9, 8, 4, 7, 6 },
};

const char* lbl_804B1428[3] = {
    "kk_heart_01.bti",
    "kk_heart_02.bti",
    "kk_heart_03.bti",
};

const char* lbl_804B1434[10] = {
    "kk_rupyNum_00.bti",
    "kk_rupyNum_01.bti",
    "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti",
    "kk_rupyNum_04.bti",
    "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti",
    "kk_rupyNum_07.bti",
    "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};

const char* lbl_804B145C[4] = {
    "kk_mainRank_01.bti",
    "kk_mainRank_02.bti",
    "kk_mainRank_03.bti",
    "kk_mainRank_04.bti",
};

// FIXME(shotgun): 80.15 %, missing/extra + reloc-name
cls_804B146C::cls_804B146C()
    : field_0x210(0xFFFFFFFF)
    , field_0x214(0xFFFFFFFF)
{
    field_0x25d = 0;
    field_0x25c = 0;
    field_0x25e[0] = 0;
    field_0x25e[1] = 0;
    field_0x25e[2] = 0;
    field_0x25e[3] = 0;
    lbl_8053AED0->pad_0x10e[0] |= 1;
    field_0x4c = new J2DOrthoGraph(
        0.0f, 0.0f,
        (f32)JUTVideo::getManager()->getFbWidth(),
        (f32)JUTVideo::getManager()->getEfbHeight(),
        -10000.0f, 10000.0f
    );
    field_0xf0 = new cls_804B19E0();
    field_0x4 = new J2DScreen();
    field_0x4->setPriority("main_bat.blo", 0x1100000, lbl_80529DEC.field_0x4c);
    for (s32 i = 0; i < 4; i++) {
        field_0x8[i] = new J2DScreen();
        field_0x8[i]->setPriority("main_hubuki.blo", 0x1100000, lbl_80529DEC.field_0x4c);
        field_0x2c[i] = new J2DScreen();
        field_0x2c[i]->setPriority("main_diving.blo", 0x1100000, lbl_80529DEC.field_0x20);
    }
    mScreen = new J2DScreen();
    mScreen->setPriority("main_bat_ranking.blo", 0x1100000, lbl_80529DEC.field_0x4c);
    field_0x4->search('Pp1li01')->hide();
    field_0x4->search('Pp2li01')->hide();
    field_0x4->search('Pp3li01')->hide();
    field_0x4->search('Pp4li01')->hide();
    field_0xf4 = field_0x4->search(lbl_8048E300[0])->getWidth();
    field_0x214 = static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C]))->getWhite();
    field_0x210 = static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C]))->getBlack();
    for (s32 i = 0; i < 4; i++) {
        field_0x50[i] = new PaneCtrl_804AF86C();
        field_0x50[i]->setPane_80402F04(mScreen->search(lbl_8048EF20[i]));
        field_0x60[i] = new PaneCtrl_804AF86C();
        field_0x60[i]->setPane_80402F04(mScreen->search(lbl_8048EF40[0x4 + i]));
        field_0x70[i] = new PaneCtrl_804AF86C();
        field_0x70[i]->setPane_80402F04(field_0x4->search(lbl_8048EF40[0x8 + i]));
        field_0x80[i] = new PaneCtrl_804AF86C();
        field_0x80[i]->setPane_80402F04(field_0x4->search(lbl_8048EF40[0xC + i]));
        field_0x90[i] = new PaneCtrl_804AF86C();
        field_0x90[i]->setPane_80402F04(field_0x4->search(lbl_8048EF40[0x10 + i]));
        field_0xa0[i] = new PaneCtrl_804AF86C();
        field_0xa0[i]->setPane_80402F04(field_0x4->search(lbl_8048EF40[0x14 + i]));
        field_0xb0[i] = new PaneCtrl_804AF86C();
        field_0xb0[i]->setPane_80402F04(field_0x4->search(lbl_8048EF40[0x1C + i]));
        field_0xc0[i] = new PaneCtrl_804AF86C();
        field_0xc0[i]->setPane_80402F04(field_0x4->search(lbl_8048EF40[0x20 + i]));
        field_0xd0[i] = new PaneCtrl_804AF86C();
        field_0xd0[i]->setPane_80402F04(field_0x4->search(lbl_8048EF40[0x24 + i]));
        field_0xe0[i] = new PaneCtrl_804AF86C();
        field_0xe0[i]->setPane_80402F04(field_0x4->search(lbl_8048EF40[0x28 + i]));
    }
    field_0x218 = field_0x4->search(lbl_8048E300[0])->getAlpha();
    for (s32 i = 0; i < 3; i++) {
        field_0x219[i] = field_0x4->search(lbl_8048E300[0x10 + i])->getAlpha();
    }
    field_0x21c = field_0x4->search(lbl_8048E300[0x1C])->getAlpha();
    field_0x21d = field_0x4->search(lbl_8048E300[0x3C])->getAlpha();
    field_0x21e = field_0x4->search(lbl_8048E300[0x5C])->getAlpha();
    field_0x21f = field_0x4->search(lbl_8048E300[0x7C])->getAlpha();
    for (s32 i = 0; i < 3; i++) {
        field_0x220[i] = field_0x4->search(lbl_8048E300[0x9C + i])->getAlpha();
        field_0x223[i] = field_0x4->search(lbl_8048E300[0xA8 + i])->getAlpha();
    }
    field_0x226 = field_0x4->search(lbl_8048E300[0xB4])->getAlpha();
    field_0x227 = field_0x4->search(lbl_8048E300[0xE4])->getAlpha();
    field_0x228 = field_0x4->search(lbl_8048E300[0x114])->getAlpha();
    field_0x229 = field_0x4->search(lbl_8048E300[0x115])->getAlpha();
    field_0x22a = field_0x4->search(lbl_8048E300[0x11C])->getAlpha();
    field_0x22b = field_0x4->search(lbl_8048E300[0x130])->getAlpha();
    for (s32 i = 0; i < 6; i++) {
        field_0x22c[i] = field_0x4->search(lbl_8048E300[0x144 + i])->getAlpha();
    }
    field_0x232 = mScreen->search(lbl_8048EF40[0x4])->getAlpha();
    field_0x233 = field_0x4->search(lbl_8048EF40[0x8])->getAlpha();
    field_0x234 = mScreen->search(lbl_8048E300[0x15C])->getAlpha();
    field_0x235 = mScreen->search(lbl_8048E300[0x15D])->getAlpha();
    field_0x236 = field_0x4->search(lbl_8048EF40[0x14])->getAlpha();
    field_0x237 = field_0x4->search(lbl_8048EF40[0x18])->getAlpha();
    field_0x238 = field_0x4->search(lbl_8048EF40[0x28])->getAlpha();
    field_0x239 = field_0x4->search(lbl_8048EF40[0x2C])->getAlpha();
    field_0x23a = field_0x4->search(lbl_8048EF40[0x30])->getAlpha();
    field_0x23b = field_0x4->search(lbl_8048EF40[0x34])->getAlpha();
    for (s32 i = 0; i < 6; i++) {
        field_0x247[i] = field_0x2c[0]->search(lbl_8048F158[i])->getAlpha();
    }
    for (s32 p = 0; p < 4; p++) {
        for (s32 i = 0; i < 8; i++) {
            for (s32 k = 0; k < 3; k++) {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1428[k]));
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x1C + p * 8 + i]))->append(res, 1.0f);
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x3C + p * 8 + i]))->append(res, 1.0f);
            }
        }
    }
    for (s32 p = 0; p < 4; p++) {
        for (s32 k = 1; k < 4; k++) {
            const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B145C[k]));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF20[p]))->append(res, 1.0f);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[p]))->append(res, 1.0f);
        }
    }
    for (s32 p = 0; p < 4; p++) {
        for (s32 i = 0; i < 8; i++) {
            for (s32 k = 0; k < 3; k++) {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1428[k]));
                static_cast<J2DPicture*>(field_0x8[p]->search(lbl_8048E300[0x164 + i]))->append(res, 1.0f);
                static_cast<J2DPicture*>(field_0x8[p]->search(lbl_8048E300[0x174 + i]))->append(res, 1.0f);
            }
        }
    }
}

// FIXME(shotgun): 99.95 %, reloc-name
cls_804B146C::~cls_804B146C() {
    delete field_0x4c;
    field_0x4c = NULL;
    delete field_0xf0;
    field_0xf0 = NULL;
    delete field_0x4;
    field_0x4 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x8[i];
        field_0x8[i] = NULL;
        delete field_0x2c[i];
        field_0x2c[i] = NULL;
    }
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x50[i];
        field_0x50[i] = NULL;
        delete field_0x60[i];
        field_0x60[i] = NULL;
        delete field_0x70[i];
        field_0x70[i] = NULL;
        delete field_0x80[i];
        field_0x80[i] = NULL;
        delete field_0x90[i];
        field_0x90[i] = NULL;
        delete field_0xa0[i];
        field_0xa0[i] = NULL;
        delete field_0xb0[i];
        field_0xb0[i] = NULL;
        delete field_0xc0[i];
        field_0xc0[i] = NULL;
        delete field_0xd0[i];
        field_0xd0[i] = NULL;
        delete field_0xe0[i];
        field_0xe0[i] = NULL;
    }
}

// FIXME(shotgun): 88.87 %, missing/extra + reloc-name
void cls_804B146C::fn_804263A8() {
    for (s32 i = 0; i < 4; i++) {
        u8 bit = (u8)(1 << i);
        bool takesPart = gGameRoot_8053AAF8->mPlayerMask & bit;
        if (!takesPart) {
            field_0x25d &= ~bit;
        } else {
            bool flagged = lbl_8053AED0->field_0xdd & bit;
            if (flagged && !(field_0x25c & bit)) {
                field_0x25c |= bit;
                field_0x25d |= bit;
            } else if (lbl_8053AED0->field_0xdc & 0x10) {
                field_0x25d &= ~bit;
            } else if (gGameRoot_8053AAF8->field_0x4c == 11) {
                field_0x25d &= ~bit;
            }
        }
        lbl_80529DEC.field_0xd8[i] = fn_80237774(i);
        lbl_80529DEC.field_0xe8[i] = fn_80236100(i);
        lbl_80529DEC.field_0x108[i] = fn_802375B0(i);
        field_0x158[i] = 0x180;
        field_0x168[i] = 0;
        field_0x178[i] = 0;
        field_0x188[i] = 0;
        field_0xf8[i] = 0.0f;
        field_0x108[i] = 0.0f;
        field_0x118[i] = 0.0f;
        field_0x128[i] = 0.0f;
        field_0x138[i] = 0.0f;
        field_0x251[i] = i;
        field_0x24d[i] = 0xFF;
        field_0x1f0[i] = 0;
        field_0x198[i] = 0;
        field_0x1a8[i] = 0;
        field_0x1b8[i] = 0;
        field_0x1c8[i] = 0;
        field_0x1e8[i] = 0;
        field_0x1f8[i] = 0;
        field_0x200[i] = 0;
        field_0x1d8[i] = 0;
        field_0x208[i] = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            fn_80421A34(i, true);
        }
    }
    field_0x255 = 0xF;
    field_0x256 = 0xF;
    field_0x257 = 0xF;
    field_0x258 = 0;
    field_0x259 = 0;
    field_0x25a = 0;
    field_0x25b = 0;
    field_0x148[0] = -2;
    field_0x148[1] = -2;
    field_0x148[2] = -2;
    field_0x148[3] = -2;
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            mScreen->search(lbl_8048EF20[i])->hide();
            mScreen->search(lbl_8048EF40[0x4 + i])->hide();
            field_0x4->search(lbl_8048EF40[0x8 + i])->hide();
            field_0x4->search(lbl_8048EF40[0xC + i])->show();
            field_0x4->search(lbl_8048EF40[0x14 + i])->hide();
            field_0x4->search(lbl_8048EF40[0x1C + i])->show();
            if (lbl_8050EC80.getCurrentFile().field_0x68 != 0) {
                field_0x4->search(lbl_8048EF40[0x20 + i])->show();
            } else {
                field_0x4->search(lbl_8048EF40[0x20 + i])->hide();
            }
            if (((gGameRoot_8053AAF8->field_0x83 >> (i * 2)) & 3) == 2) {
                field_0x4->search(lbl_8048EF40[0x10 + i])->show();
                field_0x4->search(lbl_8048EF40[0x24 + i])->show();
                field_0x4->search(lbl_8048EF40[0x28 + i])->hide();
            } else {
                field_0x4->search(lbl_8048EF40[0x10 + i])->hide();
                field_0x4->search(lbl_8048EF40[0x24 + i])->hide();
                field_0x4->search(lbl_8048EF40[0x28 + i])->hide();
            }
        } else {
            mScreen->search(lbl_8048EF20[i])->hide();
            mScreen->search(lbl_8048EF40[0x4 + i])->hide();
            field_0x4->search(lbl_8048EF40[0x8 + i])->hide();
            field_0x4->search(lbl_8048EF40[0xC + i])->hide();
            field_0x4->search(lbl_8048EF40[0x10 + i])->hide();
            field_0x4->search(lbl_8048EF40[0x14 + i])->hide();
            field_0x4->search(lbl_8048EF40[0x1C + i])->hide();
            field_0x4->search(lbl_8048EF40[0x20 + i])->hide();
            field_0x4->search(lbl_8048EF40[0x24 + i])->hide();
            field_0x4->search(lbl_8048EF40[0x28 + i])->hide();
        }
    }
    for (s32 i = 0; i < 4; i++) {
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B145C[0]));
        static_cast<J2DPicture*>(mScreen->search(lbl_8048EF20[i]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))->changeTexture(res, 0);
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            fn_80424148(i, true);
        }
    }
    field_0xf0->fn_8044172C();
}

static inline bool isPaused() {
    if (gGameRoot_8053AAF8->field_0x4c == 11) {
        return true;
    }
    return gGameRoot_8053AAF8->field_0x88 == 2;
}

// FIXME(shotgun): 75.10 %, missing/extra + reloc-name
void cls_804B146C::fn_80425F6C() {
    f32 sx = lbl_8050DB0C.field_0x30[4] *
             ((f32)JUTVideo::getManager()->getFbWidth() / gGameRoot2_8053AB10->field_0x24->field_0x858);
    f32 sy = lbl_8050DB0C.field_0x30[4] *
             ((f32)JUTVideo::getManager()->getEfbHeight() / gGameRoot2_8053AB10->field_0x24->field_0x858);
    f32 w = 0.0f + sx;
    f32 h = 0.0f + sy;
    for (s32 i = 0; i < 4; i++) {
        if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) || getPlayerLayer_8023DE58(fn_8022EA0C(i)) >= 8) {
            continue;
        }
        if (field_0x1e8[i] > 0 && lbl_8050EC80.getCurrentFile().field_0x68 != 0) {
            Vec screen;
            gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(getPlayerPos_8023E724(fn_8022EA0C(i)), false, false, &screen);
            f32 x = sx * (screen.x / JUTVideo::getManager()->getFbWidth()) - JUTVideo::getManager()->getFbWidth() * 0.5f;
            f32 y = lbl_8050DB0C.field_0x4c[1] +
                    (sy * (screen.y / JUTVideo::getManager()->getEfbHeight()) - JUTVideo::getManager()->getEfbHeight() * 0.5f);
            JGeometry::TBox2f box(0.0f, 0.0f, w, h);
            field_0x4c->setOrtho(box, -10000.0f, 10000.0f);
            field_0x4c->setPort();
            field_0x8[i]->draw(x, y, field_0x4c);
            field_0x208[i] = 0;
        } else if (!isPaused() && fn_802366CC(fn_8022EA0C(i)) > 0 && fn_802366CC(fn_8022EA0C(i)) < 300) {
            Vec screen;
            gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(getPlayerPos_8023E724(fn_8022EA0C(i)), false, false, &screen);
            f32 x = sx * (screen.x / JUTVideo::getManager()->getFbWidth()) - JUTVideo::getManager()->getFbWidth() * 0.5f;
            f32 y = lbl_8050DB0C.field_0x4c[1] +
                    (sy * (screen.y / JUTVideo::getManager()->getEfbHeight()) - JUTVideo::getManager()->getEfbHeight() * 0.5f);
            JGeometry::TBox2f box(0.0f, 0.0f, w, h);
            field_0x4c->setOrtho(box, -10000.0f, 10000.0f);
            field_0x4c->setPort();
            field_0x2c[i]->draw(x, y, field_0x4c);
        }
    }
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0xf0->fn_8044148C();
}

void cls_804B146C::fn_80425F24() {
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

// FIXME(shotgun): 86.53 %, missing/extra + reloc-name
void cls_804B146C::fn_804257E4() {
    for (s32 i = 0; i < 4; i++) {
        u8 bit = (u8)(1 << i);
        bool takesPart = gGameRoot_8053AAF8->mPlayerMask & bit;
        if (!takesPart) {
            field_0x25d &= ~bit;
        } else {
            bool flagged = lbl_8053AED0->field_0xdd & bit;
            if (flagged && !(field_0x25c & bit)) {
                field_0x25c |= bit;
                field_0x25d |= bit;
            } else if (lbl_8053AED0->field_0xdc & 0x10) {
                field_0x25d &= ~bit;
            } else if (gGameRoot_8053AAF8->field_0x4c == 11) {
                field_0x25d &= ~bit;
            }
        }
        if (gGameRoot_8053AAF8->mPlayerMask & bit) {
            fn_804253E0(i);
            if (((gGameRoot_8053AAF8->field_0x83 >> (i * 2)) & 3) == 1) {
                if (getPlayerLayer_8023DE58(i) >= 8) {
                    if (!(field_0x258 & bit)) {
                        field_0x258 |= bit;
                        mScreen->search(lbl_8048EF40[0x4 + i])->show();
                        field_0x4->search(lbl_8048EF40[0x8 + i])->show();
                        field_0x4->search(lbl_8048EF40[0xC + i])->hide();
                    }
                } else if (field_0x258 & bit) {
                    field_0x258 &= ~bit;
                    mScreen->search(lbl_8048EF40[0x4 + i])->hide();
                    field_0x4->search(lbl_8048EF40[0x8 + i])->hide();
                    field_0x4->search(lbl_8048EF40[0xC + i])->show();
                }
            }
            fn_80423DF0(i);
            if (((gGameRoot_8053AAF8->field_0x83 >> (i * 2)) & 3) == 2) {
                if (!(field_0x25a & bit)) {
                    if (fn_8023BD70(i)) {
                        field_0x25a |= bit;
                        field_0x4->search(lbl_8048EF40[0x28 + i])->show();
                    }
                } else if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(i), 0x100) == 0 || (field_0x255 & bit)) {
                    field_0x25a &= ~bit;
                    field_0x4->search(lbl_8048EF40[0x28 + i])->hide();
                }
            }
            if (field_0x255 & bit) {
                s32 val = lbl_80529DEC.field_0x108[i];
                if (val > 0x180) {
                    val = 0x180;
                }
                if (val < 0) {
                    val = 0;
                }
                for (s32 j = 0; j < 4; j++) {
                    J2DPane* p = field_0x4->search(lbl_8048E300[i * 4 + j]);
                    if (val >= 0x60) {
                        field_0x4->search(lbl_8048E300[i * 4 + j])->resize(field_0xf4, p->getHeight());
                        val -= 0x60;
                    } else {
                        field_0x4->search(lbl_8048E300[i * 4 + j])->resize(field_0xf4 * ((f32)val / 96.0f), p->getHeight());
                        val = 0;
                    }
                }
            }
            fn_80423248(i);
            fn_80423804(i);
        }
    }
    fn_80422824();
    for (s32 i = 0; i < 4; i++) {
        fn_80424148(i, false);
        if (gGameRoot2_8053AB10->field_0x20->field_0x148[i] != 0) {
            s32 x = field_0x4->search(lbl_8048EF40[0x8 + i])->getBounds()->i.x;
            s32 y = field_0x4->search(lbl_8048EF40[0x8 + i])->getBounds()->i.y;
            f32 w = field_0x4->search(lbl_8048EF40[0x8 + i])->getWidth();
            f32 h = field_0x4->search(lbl_8048EF40[0x8 + i])->getHeight();
            cls_803004C0& rect = gGameRoot2_8053AB10->field_0x20->field_0x1d0[i];
            rect.field_0x0 = x;
            rect.field_0x4 = y;
            rect.field_0x8 = w;
            rect.field_0xc = h;
            if (i == 0 || i == 2) {
                if (field_0xf8[i] < 12.0f) {
                    field_0xf8[i] += 3.0f;
                } else {
                    field_0xf8[i] = 15.0f;
                }
            } else if (field_0xf8[i] > -12.0f) {
                field_0xf8[i] -= 3.0f;
            } else {
                field_0xf8[i] = -15.0f;
            }
        } else if (i == 0 || i == 2) {
            if (field_0xf8[i] < 3.0f) {
                field_0xf8[i] = 0.0f;
            } else {
                field_0xf8[i] -= 3.0f;
            }
        } else if (field_0xf8[i] > -3.0f) {
            field_0xf8[i] = 0.0f;
        } else {
            field_0xf8[i] += 3.0f;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            fn_8042215C(i);
            fn_80421D78(i);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            fn_80421A34(i, false);
        }
    }
    field_0xf0->fn_80441490();
    field_0x255 = 0;
    field_0x256 = 0;
    field_0x257 = 0;
}

// FIXME(shotgun): 86.63 %, missing/extra (counter pointer and register order)
void cls_804B146C::fn_804253E0(s32 player) {
    s32* counter = &lbl_80529DEC.field_0xd8[player];
    s32 shown = *counter;
    if (shown < fn_80237774(player)) {
        field_0x168[player]++;
        if (field_0x168[player] >= 10) {
            *counter += 1;
            for (s32 j = 0; j < 4; j++) {
                if (j != player && fn_8022F514(j) && player == fn_8022F3DC(j)) {
                    lbl_80529DEC.field_0xd8[j] = *counter;
                }
            }
            field_0x256 |= (u8)(1 << player);
            field_0x168[player] = 0;
            fn_80230EB8(player, 9);
            field_0x1e8[player] = lbl_8050DB0C.field_0x4c[(0x74 - 0x4C) / 2] * lbl_8050DB0C.field_0x4c[(0x76 - 0x4C) / 2] + 30;
            field_0x1b8[player] = field_0x1a8[player];
            field_0x1a8[player] = *counter;
            field_0x1c8[player] = fn_80237810(player);
        }
    } else if (*counter > fn_80237774(player)) {
        lbl_80529DEC.field_0x24a |= (u8)(1 << player);
        field_0x168[player]++;
        if (field_0x168[player] >= 10) {
            *counter -= 1;
            for (s32 j = 0; j < 4; j++) {
                if (j != player && fn_8022F514(j) && player == fn_8022F3DC(j)) {
                    lbl_80529DEC.field_0xd8[j] = *counter;
                }
            }
            field_0x256 |= (u8)(1 << player);
            field_0x168[player] = 0;
        }
        field_0x1e8[player] = lbl_8050DB0C.field_0x4c[(0x74 - 0x4C) / 2] * lbl_8050DB0C.field_0x4c[(0x76 - 0x4C) / 2] + 30;
        field_0x1b8[player] = field_0x1a8[player];
        field_0x1a8[player] = *counter;
        field_0x1c8[player] = fn_80237810(player);
    } else {
        field_0x168[player] = 0;
    }
    s32* life = &lbl_80529DEC.field_0xe8[player];
    if (*life < fn_80236100(player)) {
        field_0x178[player]++;
        if (field_0x178[player] >= 2) {
            *life += 2;
            if (*life >= fn_80236100(player)) {
                *life = fn_80236100(player);
                fn_80230EB8(player, 0x51);
            } else {
                fn_80230EB8(player, 4);
            }
            field_0x257 |= (u8)(1 << player);
            field_0x178[player] = 0;
        }
    } else if (*life > fn_80236100(player)) {
        field_0x178[player]++;
        if (field_0x178[player] >= 2) {
            *life -= 2;
            if (*life < fn_80236100(player)) {
                *life = fn_80236100(player);
                fn_80230EB8(player, 0x51);
            } else {
                fn_80230EB8(player, 0x4E);
            }
            field_0x257 |= (u8)(1 << player);
            field_0x178[player] = 0;
        }
    } else {
        field_0x178[player] = 0;
    }
    if (field_0x1f8[player] > 0) {
        if (field_0x200[player] == 0) {
            field_0x1f8[player]--;
            fn_80230FD0(player, 0x20034);
            if (field_0x1f8[player] != 0) {
                field_0x200[player] = 12;
            }
        } else {
            field_0x200[player]--;
        }
    }
}

// FIXME(shotgun): 99.58 %, regalloc-only
void cls_804B146C::fn_80425234(s32 id, s32 arg1, u32 arg2) {
    s32 target;
    if (fn_8022F514(id)) {
        target = fn_8022F3DC(id);
    } else {
        target = id;
    }
    if ((u8)arg2) {
        s32 sum = arg1 + fn_80236100(target);
        if (sum > 9999) {
            arg1 = 9999 - fn_80236228(target);
        } else if (sum < -5000) {
            arg1 = -5000 - fn_80236228(target);
        }
    } else {
        s32 sum = arg1 + fn_80236228(target);
        if (sum > 9999) {
            arg1 = 9999 - fn_80236228(target);
        } else if (sum < 0) {
            if (fn_80236228(target) <= 0) {
                arg1 = 0;
            } else {
                arg1 = -fn_80236228(target);
            }
        }
    }
    if (arg1 >= 1000 || arg1 <= -1000) {
        field_0x1f8[id] = __abs(arg1) / 100;
        field_0x200[id] = 0;
    } else {
        fn_80230FD0(id, 0x20034);
    }
    if ((u8)arg2) {
        fn_8023619C(target, arg1);
    } else {
        fn_802362C4(target, arg1);
    }
    lbl_80529DEC.field_0xe8[target] = fn_80236100(target);
    field_0x178[target] = 0;
    field_0x257 |= (u8)(1 << target);
}

// FIXME(shotgun): 98.36 %, missing/extra (the takesPart bool test is one cmplwi longer)
void cls_804B146C::fn_80425128() {
    for (s32 i = 0; i < 4; i++) {
        bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i);
        if (takesPart == true) {
            lbl_80529DEC.field_0xd8[i] = fn_80237774(i);
            for (s32 j = 0; j < 4; j++) {
                if (j != i && fn_8022F514(j) && i == fn_8022F3DC(j)) {
                    lbl_80529DEC.field_0xd8[j] = lbl_80529DEC.field_0xd8[i];
                }
            }
            field_0x256 |= (u8)(1 << i);
            field_0x168[i] = 0;
            lbl_80529DEC.field_0xe8[i] = fn_80236100(i);
            field_0x257 |= (u8)(1 << i);
            field_0x178[i] = 0;
        }
    }
}

// FIXME(shotgun): 79.63 %, missing/extra + reloc-name
void cls_804B146C::fn_80424148(s32 player, bool force) {
    f32 prevA = field_0x108[player];
    f32 prevB = field_0x128[player];
    f32 prevC = field_0x138[player];
    if (player >= 2) {
        field_0x50[player]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
        field_0x60[player]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
        field_0x70[player]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
        field_0x80[player]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
        field_0x90[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D57C());
        field_0xa0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D57C());
        field_0xb0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D57C());
        field_0xc0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D57C());
        field_0xd0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D57C());
        field_0xe0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D57C());
    } else {
        field_0x50[player]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
        field_0x60[player]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
        field_0x70[player]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
        field_0x80[player]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
        field_0x90[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D514());
        field_0xa0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D514());
        field_0xb0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D514());
        field_0xc0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D514());
        field_0xd0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D514());
        field_0xe0[player]->moveFromCentre_80402E08(field_0xf8[player], lbl_8053AED0->fn_8043D514());
    }
    fn_8042165C();
    fn_8037EB4C();
    if (lbl_8053AED0->field_0xdc & 0x10) {
        lbl_8053AED0->field_0xdc &= ~(u8)(1 << player);
    }
    u8 bit = (u8)(1 << player);
    if ((field_0x25d & bit) && !(lbl_8053AED0->field_0xdc & bit) &&
        gGameRoot2_8053AB10->field_0x30->field_0x284 == 0 && gGameRoot_8053AAF8->field_0x85 == 0) {
        if (lbl_80529DEC.field_0xd8[player] == 0) {
            if (field_0x108[player] > 0.6f) {
                field_0x108[player] -= 0.1f;
                if (field_0x108[player] <= 0.6f) {
                    field_0x108[player] = 0.6f;
                }
            } else if (field_0x108[player] < 0.6f) {
                field_0x108[player] += 0.1f;
                if (field_0x108[player] >= 0.6f) {
                    field_0x108[player] = 0.6f;
                }
            }
            if (gGameRoot2_8053AB10->field_0x20->field_0x148[player] != 0) {
                if (field_0x128[player] < 0.9f) {
                    field_0x128[player] += 0.1f;
                } else {
                    field_0x128[player] = 1.0f;
                }
                if (field_0x138[player] > 0.6f) {
                    field_0x138[player] -= 0.1f;
                    if (field_0x138[player] <= 0.6f) {
                        field_0x138[player] = 0.6f;
                    }
                } else if (field_0x138[player] < 0.6f) {
                    field_0x138[player] += 0.1f;
                    if (field_0x138[player] >= 0.6f) {
                        field_0x138[player] = 0.6f;
                    }
                }
            } else {
                if (field_0x128[player] > 0.1f) {
                    field_0x128[player] -= 0.1f;
                } else {
                    field_0x128[player] = 0.0f;
                }
                if (field_0x138[player] > 0.1f) {
                    field_0x138[player] -= 0.1f;
                } else {
                    field_0x138[player] = 0.0f;
                }
            }
        } else {
            if (field_0x108[player] < 0.9f) {
                field_0x108[player] += 0.1f;
            } else {
                field_0x108[player] = 1.0f;
            }
            if (field_0x128[player] < 0.9f) {
                field_0x128[player] += 0.1f;
            } else {
                field_0x128[player] = 1.0f;
            }
            if (field_0x138[player] < 0.9f) {
                field_0x138[player] += 0.1f;
            } else {
                field_0x138[player] = 1.0f;
            }
        }
    } else {
        if (field_0x108[player] > 0.1f) {
            field_0x108[player] -= 0.1f;
        } else {
            field_0x108[player] = 0.0f;
        }
        if (field_0x128[player] > 0.1f) {
            field_0x128[player] -= 0.1f;
        } else {
            field_0x128[player] = 0.0f;
        }
        if (field_0x138[player] > 0.1f) {
            field_0x138[player] -= 0.1f;
        } else {
            field_0x138[player] = 0.0f;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x217[player] = 1;
    }
    if (prevA == field_0x108[player] && prevB == field_0x128[player] && prevC == field_0x138[player] && !force) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x4->search(lbl_8048E300[player * 4 + i])->setAlpha(field_0x218 * field_0x108[player]);
    }
    for (s32 i = 0; i < 3; i++) {
        field_0x4->search(lbl_8048E300[0x10 + player * 3 + i])->setAlpha(field_0x219[i] * field_0x108[player]);
    }
    for (s32 i = 0; i < 8; i++) {
        field_0x4->search(lbl_8048E300[0x1C + player * 8 + i])->setAlpha(field_0x21c * field_0x108[player]);
        field_0x4->search(lbl_8048E300[0x3C + player * 8 + i])->setAlpha(field_0x21d * field_0x108[player]);
        field_0x4->search(lbl_8048E300[0x5C + player * 8 + i])->setAlpha(field_0x21e * field_0x108[player]);
        field_0x4->search(lbl_8048E300[0x7C + player * 8 + i])->setAlpha(field_0x21f * field_0x108[player]);
    }
    for (s32 i = 0; i < 3; i++) {
        field_0x4->search(lbl_8048E300[0x9C + player * 3 + i])->setAlpha(field_0x220[i] * field_0x108[player]);
        field_0x4->search(lbl_8048E300[0xA8 + player * 3 + i])->setAlpha(field_0x223[i] * field_0x108[player]);
    }
    for (s32 i = 0; i < 12; i++) {
        field_0x4->search(lbl_8048E300[0xB4 + player * 12 + i])->setAlpha(field_0x226 * field_0x108[player]);
        field_0x4->search(lbl_8048E300[0xE4 + player * 12 + i])->setAlpha(field_0x227 * field_0x108[player]);
    }
    field_0x4->search(lbl_8048E300[0x114 + player * 2])->setAlpha(field_0x228 * field_0x108[player]);
    field_0x4->search(lbl_8048E300[0x115 + player * 2])->setAlpha(field_0x229 * field_0x108[player]);
    for (s32 i = 0; i < 5; i++) {
        field_0x4->search(lbl_8048E300[0x11C + player * 5 + i])->setAlpha(field_0x22a * field_0x108[player]);
        field_0x4->search(lbl_8048E300[0x130 + player * 5 + i])->setAlpha(field_0x22b * field_0x108[player]);
    }
    for (s32 i = 0; i < 6; i++) {
        field_0x4->search(lbl_8048E300[0x144 + player * 6 + i])->setAlpha(field_0x22c[i] * field_0x108[player]);
    }
    mScreen->search(lbl_8048EF40[0x4 + player])->setAlpha(field_0x232 * field_0x138[player]);
    field_0x4->search(lbl_8048EF40[0x8 + player])->setAlpha(field_0x233 * field_0x128[player]);
    mScreen->search(lbl_8048E300[0x15C + player * 2])->setAlpha(field_0x234 * field_0x108[player]);
    mScreen->search(lbl_8048E300[0x15D + player * 2])->setAlpha(field_0x235 * field_0x108[player]);
    field_0x4->search(lbl_8048EF40[0x14 + player])->setAlpha(field_0x236 * field_0x108[player]);
    field_0x4->search(lbl_8048EF40[0x18 + player])->setAlpha(field_0x237 * field_0x108[player]);
    field_0x4->search(lbl_8048EF40[0x28 + player])->setAlpha(field_0x238 * field_0x108[player]);
    field_0x4->search(lbl_8048EF40[0x2C + player])->setAlpha(field_0x239 * field_0x108[player]);
    field_0x4->search(lbl_8048EF40[0x30 + player])->setAlpha(field_0x23a * field_0x108[player]);
    field_0x4->search(lbl_8048EF40[0x34 + player])->setAlpha(field_0x23b * field_0x108[player]);
}

// FIXME(shotgun): 68.84 %, missing/extra
void cls_804B146C::fn_80423DF0(s32 player) {
    u8 bit = (u8)(1 << player);
    s32 item = -1;
    bool active = field_0x259 & bit;
    if (!active && !fn_80230188(player)) {
        if (fn_8023BE0C(fn_8022EA0C(player)) && !(lbl_8053AED0->field_0x255 & bit)) {
            field_0x259 |= bit;
            field_0x4->search(lbl_8048EF40[0x14 + player])->show();
        }
    } else {
        if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(player), 0x100) == 0 ||
            (lbl_8053AED0->field_0x255 & bit) || fn_80230188(player)) {
            field_0x259 &= ~bit;
            field_0x4->search(lbl_8048EF40[0x14 + player])->hide();
        }
    }
    switch (fn_802373EC(player)) {
    case 0:
        break;
    case 1:
        item = 5;
        break;
    case 2:
        item = 11;
        break;
    case 3:
        item = 2;
        break;
    case 4:
        item = 3;
        break;
    case 5:
        item = 6;
        break;
    case 6:
        item = 7;
        break;
    case 7:
        item = 4;
        break;
    case 8:
        item = 8;
        break;
    case 9:
        item = 10;
        break;
    case 10:
        item = 1;
        break;
    case 11:
    case 12:
    case 13:
        item = 0;
        break;
    }
    if (item != field_0x148[player] || (lbl_8053AED0->field_0x255 & bit)) {
        field_0x148[player] = item;
        for (s32 i = 0; i < 12; i++) {
            if (i == item) {
                field_0x4->search(lbl_8048E300[0xB4 + player * 12 + i])->show();
                field_0x4->search(lbl_8048E300[0xE4 + player * 12 + i])->show();
            } else {
                field_0x4->search(lbl_8048E300[0xB4 + player * 12 + i])->hide();
                field_0x4->search(lbl_8048E300[0xE4 + player * 12 + i])->hide();
            }
        }
        if (fn_8023725C(player) == 2) {
            for (s32 i = 0; i < 3; i++) {
                field_0x4->search(lbl_8048E300[0xA8 + player * 3 + i])->show();
            }
        } else {
            for (s32 i = 0; i < 3; i++) {
                field_0x4->search(lbl_8048E300[0xA8 + player * 3 + i])->hide();
            }
        }
    }
}

// FIXME(shotgun): 63.29 %, missing/extra
void cls_804B146C::fn_80423804(s32 player) {
    if (!(field_0x256 & (u8)(1 << player))) {
        return;
    }
    s32 max = fn_80237810(player);
    s32 life = lbl_80529DEC.field_0xd8[player];
    if (max > 32) {
        max = 32;
    }
    if (max < 0) {
        max = 0;
    }
    if (max < life) {
        life = max;
    }
    if (life < 0) {
        life = 0;
    }
    for (s32 i = 0; i < 8; i++) {
        if (life >= 4) {
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x1C + player * 8 + i]))
                ->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x3C + player * 8 + i]))
                ->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x4->search(lbl_8048E300[0x5C + player * 8 + i])->hide();
            field_0x4->search(lbl_8048E300[0x7C + player * 8 + i])->hide();
            max -= 4;
            life -= 4;
        } else {
            if (life == 0) {
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x1C + player * 8 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x3C + player * 8 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                if (max > 0) {
                    field_0x4->search(lbl_8048E300[0x5C + player * 8 + i])->show();
                    field_0x4->search(lbl_8048E300[0x7C + player * 8 + i])->show();
                    max -= 4;
                } else {
                    field_0x4->search(lbl_8048E300[0x5C + player * 8 + i])->hide();
                    field_0x4->search(lbl_8048E300[0x7C + player * 8 + i])->hide();
                }
            } else if (life == 1) {
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x1C + player * 8 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x3C + player * 8 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                field_0x4->search(lbl_8048E300[0x5C + player * 8 + i])->hide();
                field_0x4->search(lbl_8048E300[0x7C + player * 8 + i])->hide();
                max -= 4;
            } else if (life == 2) {
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x1C + player * 8 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x3C + player * 8 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                field_0x4->search(lbl_8048E300[0x5C + player * 8 + i])->hide();
                field_0x4->search(lbl_8048E300[0x7C + player * 8 + i])->hide();
                max -= 4;
            } else if (life == 3) {
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x1C + player * 8 + i]))
                    ->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x3C + player * 8 + i]))
                    ->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                field_0x4->search(lbl_8048E300[0x5C + player * 8 + i])->hide();
                field_0x4->search(lbl_8048E300[0x7C + player * 8 + i])->hide();
                max -= 4;
            }
            life = 0;
        }
    }
}

// FIXME(shotgun): 63.23 %, missing/extra + reloc-name
void cls_804B146C::fn_80423248(s32 player) {
    if (!(field_0x257 & (u8)(1 << player))) {
        return;
    }
    s32 start = 0;
    s32 val = lbl_80529DEC.field_0xe8[player];
    if (val > 9999) {
        val = 9999;
    }
    if (val < 0) {
        for (s32 i = 0; i < 5; i++) {
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C + player * 5 + i]))
                ->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xFF, 0x55, 0x41, 0xFF));
        }
        if (val < -5000) {
            val = -5000;
        }
        val = -val;
        const ResTIMG* minus = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_rupyNum_10.bti"));
        static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C + player * 5]))->changeTexture(minus, 0);
        static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x130 + player * 5]))->changeTexture(minus, 0);
        field_0x4->search(lbl_8048E300[0x11C + player * 5 + 4])->show();
        field_0x4->search(lbl_8048E300[0x130 + player * 5 + 4])->show();
        start = 1;
    } else {
        for (s32 i = 0; i < 5; i++) {
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C + player * 5 + i]))
                ->setBlackWhite(field_0x210, field_0x214);
        }
        field_0x4->search(lbl_8048E300[0x11C + player * 5 + 4])->hide();
        field_0x4->search(lbl_8048E300[0x130 + player * 5 + 4])->hide();
    }
    const ResTIMG* d1000 = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1434[val / 1000]));
    val -= (val / 1000) * 1000;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C + player * 5 + start]))->changeTexture(d1000, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x130 + player * 5 + start]))->changeTexture(d1000, 0);

    const ResTIMG* d100 = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1434[val / 100]));
    val -= (val / 100) * 100;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C + player * 5 + start + 1]))->changeTexture(d100, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x130 + player * 5 + start + 1]))->changeTexture(d100, 0);

    const ResTIMG* d10 = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1434[val / 10]));
    val -= (val / 10) * 10;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C + player * 5 + start + 2]))->changeTexture(d10, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x130 + player * 5 + start + 2]))->changeTexture(d10, 0);

    const ResTIMG* d1 = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1434[val]));
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x11C + player * 5 + start + 3]))->changeTexture(d1, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048E300[0x130 + player * 5 + start + 3]))->changeTexture(d1, 0);
}

// FIXME(shotgun): 99.94 %, regalloc-only
void cls_804B146C::fn_80422824() {
    s32 count = 0;
    s32 ranked = 0;
    if (!(field_0x256 & 1) && !(field_0x256 & 2) && !(field_0x256 & 4) && !(field_0x256 & 8)) {
        return;
    }
    if (field_0x25b == 0) {
        field_0x25b = 1;
        return;
    }
    s32 ranks[4];
    s32 done[4];
    for (s32 i = 0; i < 4; i++) {
        ranks[i] = 0;
        done[i] = 0;
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            if (lbl_8050EC80.getCurrentFile().field_0x69 != 0) {
                mScreen->search(lbl_8048EF20[i])->show();
            } else {
                mScreen->search(lbl_8048EF20[i])->hide();
            }
            count++;
        }
    }
    do {
        s32 best = -5001;
        s32 tied = 0;
        for (s32 i = 0; i < count; i++) {
            if (done[i] == 0 && best < fn_80237774(i)) {
                best = fn_80237774(i);
            }
        }
        for (s32 i = 0; i < count; i++) {
            if (done[i] == 0 && best == fn_80237774(i)) {
                ranks[i] = ranked;
                tied++;
                done[i] = 1;
            }
        }
        ranked += tied;
    } while (ranked < count);

    for (s32 i = 0; i < count; i++) {
        switch (ranks[i]) {
        case 0:
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setCornerColor(
                    JUtility::TColor(0xB9, 0x12, 0x12, 0xFF),
                    JUtility::TColor(0xD8, 0x3A, 0x16, 0xFF),
                    JUtility::TColor(0xB9, 0x54, 0x12, 0xFF),
                    JUtility::TColor(0xFF, 0x9D, 0x10, 0xFF)
                );
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF20[i]))
                ->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 1:
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setCornerColor(
                    JUtility::TColor(0xF2, 0x48, 0x09, 0xFF),
                    JUtility::TColor(0xF2, 0x48, 0x09, 0xFF),
                    JUtility::TColor(0xF2, 0xAB, 0x24, 0xFF),
                    JUtility::TColor(0xFC, 0xFF, 0x21, 0xFF)
                );
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF20[i]))
                ->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 2:
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setWhite(JUtility::TColor(0xFF, 0xFF, 0x77, 0xFF));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setCornerColor(
                    JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                    JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                    JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                    JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF)
                );
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF20[i]))
                ->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 3:
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setWhite(JUtility::TColor(0xD8, 0xFF, 0xA1, 0xFF));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setCornerColor(
                    JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                    JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                    JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                    JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF)
                );
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF20[i]))
                ->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048EF40[i]))
                ->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        }
    }
}

// FIXME(shotgun): 81.76 %, missing/extra + reloc-name
void cls_804B146C::fn_8042215C(s32 player) {
    if (field_0x1e8[player] == 0) {
        field_0x1a8[player] = fn_80237774(player);
        field_0x1c8[player] = fn_80237810(player);
        return;
    }
    s32 max = field_0x1c8[player];
    s32 life = field_0x1a8[player];
    if (max > 32) {
        max = 32;
    }
    if (max < 0) {
        max = 0;
    }
    if (max < life) {
        life = max;
    }
    if (life < 0) {
        life = 0;
    }
    for (s32 i = 0; i < 8; i++) {
        if (life >= 4) {
            field_0x8[player]->search(lbl_8048E300[0x164 + i])->show();
            field_0x8[player]->search(lbl_8048E300[0x174 + i])->show();
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048E300[0x164 + i]))
                ->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048E300[0x174 + i]))
                ->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x8[player]->search(lbl_8048E300[0x16C + i])->hide();
            field_0x8[player]->search(lbl_8048E300[0x17C + i])->hide();
            max -= 4;
            life -= 4;
        } else {
            if (life == 0) {
                field_0x8[player]->search(lbl_8048E300[0x164 + i])->hide();
                field_0x8[player]->search(lbl_8048E300[0x174 + i])->hide();
                if (max > 0) {
                    field_0x8[player]->search(lbl_8048E300[0x16C + i])->show();
                    field_0x8[player]->search(lbl_8048E300[0x17C + i])->show();
                    max -= 4;
                } else {
                    field_0x8[player]->search(lbl_8048E300[0x16C + i])->hide();
                    field_0x8[player]->search(lbl_8048E300[0x17C + i])->hide();
                }
            } else if (life == 1) {
                field_0x8[player]->search(lbl_8048E300[0x164 + i])->show();
                field_0x8[player]->search(lbl_8048E300[0x174 + i])->show();
                static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048E300[0x164 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048E300[0x174 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                field_0x8[player]->search(lbl_8048E300[0x16C + i])->hide();
                field_0x8[player]->search(lbl_8048E300[0x17C + i])->hide();
                max -= 4;
            } else if (life == 2) {
                field_0x8[player]->search(lbl_8048E300[0x164 + i])->show();
                field_0x8[player]->search(lbl_8048E300[0x174 + i])->show();
                static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048E300[0x164 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048E300[0x174 + i]))
                    ->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                field_0x8[player]->search(lbl_8048E300[0x16C + i])->hide();
                field_0x8[player]->search(lbl_8048E300[0x17C + i])->hide();
                max -= 4;
            } else if (life == 3) {
                field_0x8[player]->search(lbl_8048E300[0x164 + i])->show();
                field_0x8[player]->search(lbl_8048E300[0x174 + i])->show();
                static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048E300[0x164 + i]))
                    ->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048E300[0x174 + i]))
                    ->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                field_0x8[player]->search(lbl_8048E300[0x16C + i])->hide();
                field_0x8[player]->search(lbl_8048E300[0x17C + i])->hide();
                max -= 4;
            }
            life = 0;
        }
    }
    field_0x1e8[player]--;
}

// FIXME(shotgun): 99.46 %, missing/extra
void cls_804B146C::fn_80421D78(s32 player) {
    if (fn_802366CC(fn_8022EA0C(player)) > 0 && fn_802366CC(fn_8022EA0C(player)) < 300) {
        s32 sec = fn_802366CC(fn_8022EA0C(player)) / 60 + 1;
        if (sec != field_0x25e[player]) {
            if (sec > 99) {
                sec = 99;
            }
            if (sec < 0) {
                sec = 0;
            }
            if (sec < 10) {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1434[sec]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048F158[0]))->changeTexture(res, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048F158[1]))->changeTexture(res, 0);
                if (field_0x2c[player]->search(lbl_8048F158[2])->isVisible() == true) {
                    field_0x2c[player]->search(lbl_8048F158[2])->hide();
                    field_0x2c[player]->search(lbl_8048F158[3])->hide();
                }
            } else {
                const ResTIMG* d10 = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1434[sec / 10]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048F158[0]))->changeTexture(d10, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048F158[1]))->changeTexture(d10, 0);
                const ResTIMG* d1 = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1434[sec % 10]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048F158[2]))->changeTexture(d1, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048F158[3]))->changeTexture(d1, 0);
                if (field_0x2c[player]->search(lbl_8048F158[2])->isVisible() == false) {
                    field_0x2c[player]->search(lbl_8048F158[2])->show();
                    field_0x2c[player]->search(lbl_8048F158[3])->show();
                }
            }
            field_0x25e[player] = sec;
        }
    } else {
        field_0x25e[player] = 0;
    }
    if (field_0x208[player] > 0) {
        field_0x208[player]--;
    }
}

// FIXME(shotgun): 93.22 %, missing/extra
void cls_804B146C::fn_80421A34(s32 player, bool force) {
    u8 bit = (u8)(1 << player);
    f32 prev = field_0x118[player];
    if (!(field_0x25d & bit)) {
        if (prev > 0.0f) {
            field_0x118[player] = prev - 0.1f;
        } else {
            field_0x118[player] = 0.0f;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x217[player] = 1;
    } else if (lbl_80529DEC.field_0xd8[player] == 0) {
        if (prev > 0.6f) {
            field_0x118[player] = prev - 0.1f;
            if (field_0x118[player] <= 0.6f) {
                field_0x118[player] = 0.6f;
            }
        } else if (prev < 0.6f) {
            field_0x118[player] = prev + 0.1f;
            if (field_0x118[player] >= 0.6f) {
                field_0x118[player] = 0.6f;
            }
        }
    } else {
        if (prev < 1.0f) {
            field_0x118[player] = prev + 0.1f;
        } else {
            field_0x118[player] = 1.0f;
        }
    }
    if (prev == field_0x118[player] && !force) {
        return;
    }
    for (s32 i = 0; i < 8; i++) {
        field_0x8[player]->search(lbl_8048E300[0x164 + i])->setAlpha(field_0x21c * field_0x118[player]);
        field_0x8[player]->search(lbl_8048E300[0x174 + i])->setAlpha(field_0x21d * field_0x118[player]);
        field_0x8[player]->search(lbl_8048E300[0x16C + i])->setAlpha(field_0x21e * field_0x118[player]);
        field_0x8[player]->search(lbl_8048E300[0x17C + i])->setAlpha(field_0x21f * field_0x118[player]);
    }
    for (s32 i = 0; i < 6; i++) {
        field_0x2c[player]->search(lbl_8048F158[i])->setAlpha(field_0x247[i] * field_0x118[player]);
    }
}

// FIXME(shotgun): 89.38 %, missing/extra
void cls_804B146C::fn_8042165C() {
    field_0x1f0[0] = 0;
    field_0x1f0[1] = 0;
    field_0x1f0[2] = 0;
    field_0x1f0[3] = 0;
    for (s32 i = 0; i < 4; i++) {
        u32 bit = 1 << i;
        bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)bit;
        if (takesPart && !fn_8022C8EC(i)) {
            u16 mask = (u16)bit;
            if (i != 0) {
                field_0x1f0[0] |= mask;
            }
            if (i != 1) {
                field_0x1f0[1] |= mask;
            }
            if (i != 2) {
                field_0x1f0[2] |= mask;
            }
            if (i != 3) {
                field_0x1f0[3] |= mask;
            }
            if (getPlayerLayer_8023DE58(i) < 8) {
                Vec screen;
                gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(getPlayerPos_8023E724(i), false, false, &screen);
                for (s32 j = 0; j < 10; j++) {
                    if (!lbl_8053AED0->fn_8043DF24(lbl_8048F188[j], lbl_8048F1B0[j], screen.x, screen.y, 0.0f)) {
                        u16 jbit = (u16)(1 << j);
                        field_0x1f0[0] |= jbit;
                        field_0x1f0[1] |= jbit;
                        field_0x1f0[2] |= jbit;
                        field_0x1f0[3] |= jbit;
                    }
                }
            }
        }
        if (field_0x24d[i] != 0xFF) {
            if (i != 0) {
                field_0x1f0[0] |= (u16)(1 << field_0x24d[i]);
            }
            if (i != 1) {
                field_0x1f0[1] |= (u16)(1 << field_0x24d[i]);
            }
            if (i != 2) {
                field_0x1f0[2] |= (u16)(1 << field_0x24d[i]);
            }
            if (i != 3) {
                field_0x1f0[3] |= (u16)(1 << field_0x24d[i]);
            }
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) && !fn_8022C8EC(i) && getPlayerLayer_8023DE58(i) < 8) {
            for (s32 j = 0; j < 10; j++) {
                u8 cand = lbl_8048F1D8[i][j];
                if (!(field_0x1f0[i] & (u16)(1 << cand))) {
                    if (field_0x251[i] != cand && field_0x24d[i] == 0xFF) {
                        field_0x24d[i] = cand;
                        field_0x198[i] = 0;
                        field_0x1f0[0] |= (u16)(1 << field_0x24d[i]);
                        field_0x1f0[1] |= (u16)(1 << field_0x24d[i]);
                        field_0x1f0[2] |= (u16)(1 << field_0x24d[i]);
                        field_0x1f0[3] |= (u16)(1 << field_0x24d[i]);
                    }
                    break;
                }
            }
            if (field_0x24d[i] != 0xFF) {
                field_0x198[i]++;
                if (field_0x198[i] >= 60) {
                    field_0x251[i] = field_0x24d[i];
                    field_0x24d[i] = 0xFF;
                }
            } else if (field_0x198[i] != 0) {
                field_0x198[i]++;
                if (field_0x198[i] >= 120) {
                    field_0x198[i] = 0;
                }
            }
        }
    }
}
