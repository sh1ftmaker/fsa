#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Map.h"
#include "game/Player.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DAnmLoader.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J3D/J3DAnmTextureSRTKey.h"
#include "JSystem/JKernel/JKRFileLoader.h"

static const char* lbl_804B1640[10] = {
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

// The six pane tags the constructor reads the alphas of (Ptim01 twice).
static const u64 lbl_8048F438[6] = {
    0x00000050746B3031ull, /* "Ptk01" */
    0x0000000050743031ull, /* "Pt01" */
    0x00000050746B3032ull, /* "Ptk02" */
    0x0000000050743032ull, /* "Pt02" */
    0x00005074696D3031ull, /* "Ptim01" */
    0x00005074696D3031ull, /* "Ptim01" */
};

// FIXME(shotgun): 99.79 %, operands (the vtable symbol name)
cls_804B1668::cls_804B1668() {
    field_0x4[0] = new J2DScreen;
    field_0x4[0]->setPriority("main_gba_window.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x10[0] = static_cast<J2DAnmTextureSRTKey*>(
        J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource("main_gba_window.btk", lbl_80529DEC.field_0x20)));
    field_0x4[0]->setAnimation(field_0x10[0]);
    field_0x18[0] = new PaneCtrl_804AF86C;
    field_0x18[0]->setPane_80402F04(field_0x4[0]->search('Psp01'));
    field_0x18[1] = new PaneCtrl_804AF86C;
    field_0x18[1]->setPane_80402F04(field_0x4[0]->search('Psp02'));
    field_0x18[2] = new PaneCtrl_804AF86C;
    field_0x18[2]->setPane_80402F04(field_0x4[0]->search('Psp03'));
    field_0x18[3] = new PaneCtrl_804AF86C;
    field_0x18[3]->setPane_80402F04(field_0x4[0]->search('Psp04'));
    field_0x34 = new PaneCtrl_804AF86C;
    field_0x34->setPane_80402F04(field_0x4[0]->search('Ndata02'));
    field_0x34->mPane->hide();
    f32 width = field_0x18[2]->mPane->getWidth();
    f32 centreX = field_0x18[2]->field_0x10;
    field_0x40[0].x = (centreX + field_0x4[0]->search('Ndata01')->getBounds()->i.x) - width * 0.5f;
    f32 height = field_0x18[2]->mPane->getHeight();
    f32 centreY = field_0x18[2]->field_0x14;
    field_0x40[0].y = (centreY + field_0x4[0]->search('Ndata01')->getBounds()->i.y) - height * 0.5f;
    field_0x40[0].z = 0.0f;
    field_0x70[0] = field_0x18[2]->mPane->getWidth();
    field_0x70[2] = field_0x18[2]->mPane->getHeight();
    centreX = field_0x18[0]->field_0x8;
    field_0x58[0].x = centreX + field_0x4[0]->search('Ndata01')->getBounds()->i.x;
    centreY = field_0x18[0]->field_0xc;
    field_0x58[0].y = centreY + field_0x4[0]->search('Ndata01')->getBounds()->i.y;
    field_0x58[0].z = 0.0f;
    field_0x80[0] = field_0x18[0]->mPane->getWidth();
    field_0x80[2] = field_0x18[0]->mPane->getHeight();
    field_0x4[1] = new J2DScreen;
    field_0x4[1]->setPriority("main_gba_window_s.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x10[1] = static_cast<J2DAnmTextureSRTKey*>(
        J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource("main_gba_window_s.btk", lbl_80529DEC.field_0x20)));
    field_0x4[1]->setAnimation(field_0x10[1]);
    field_0x28[0] = new PaneCtrl_804AF86C;
    field_0x28[0]->setPane_80402F04(field_0x4[1]->search('Psp01'));
    field_0x28[1] = new PaneCtrl_804AF86C;
    field_0x28[1]->setPane_80402F04(field_0x4[1]->search('Psp03'));
    field_0x28[2] = new PaneCtrl_804AF86C;
    field_0x28[2]->setPane_80402F04(field_0x4[1]->search('Psp04'));
    width = field_0x28[1]->mPane->getWidth();
    centreX = field_0x28[1]->field_0x10;
    field_0x40[1].x = (centreX + field_0x4[1]->search('Ndata01')->getBounds()->i.x) - width * 0.5f;
    height = field_0x28[1]->mPane->getHeight();
    centreY = field_0x28[1]->field_0x14;
    field_0x40[1].y = (centreY + field_0x4[1]->search('Ndata01')->getBounds()->i.y) - height * 0.5f;
    field_0x40[1].z = 0.0f;
    field_0x70[1] = field_0x28[1]->mPane->getWidth();
    field_0x70[3] = field_0x28[1]->mPane->getHeight();
    centreX = field_0x28[0]->field_0x8;
    field_0x58[1].x = centreX + field_0x4[1]->search('Ndata01')->getBounds()->i.x;
    centreY = field_0x28[0]->field_0xc;
    field_0x58[1].y = centreY + field_0x4[1]->search('Ndata01')->getBounds()->i.y;
    field_0x58[1].z = 0.0f;
    field_0x80[1] = field_0x28[0]->mPane->getWidth();
    field_0x80[3] = field_0x28[0]->mPane->getHeight();
    field_0xc = new J2DScreen;
    field_0xc->setPriority("main_diving.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x3c = new PaneCtrl_804AF86C;
    field_0x3c->setPane_80402F04(field_0xc->search('Ndata00'));
    field_0x90 = 0;
    field_0x94 = 0;
    field_0x9e = 0;
    field_0xa2 = 0;
    field_0xa3 = 0;
    field_0x9c = 0;
    field_0xa0 = 0xFFFF;
    field_0xa4 = 0;
    field_0xa5 = 0;
    field_0x98 = 0;
    field_0xac = 0;
    for (s32 i = 0; i < 6; i++) {
        field_0xa6[i] = field_0xc->search(lbl_8048F438[i])->getAlpha();
    }
}

// FIXME(shotgun): 99.91 %, reloc-name
cls_804B1668::~cls_804B1668() {
    for (s32 i = 0; i < 2; i++) {
        delete field_0x4[i];
        field_0x4[i] = NULL;
        delete field_0x10[i];
        field_0x10[i] = NULL;
    }
    for (s32 i = 0; i < 4; i++) {
        delete field_0x18[i];
        field_0x18[i] = NULL;
    }
    for (s32 i = 0; i < 3; i++) {
        delete field_0x28[i];
        field_0x28[i] = NULL;
    }
    delete field_0x34;
    field_0x34 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    delete field_0x3c;
    field_0x3c = NULL;
}

// FIXME(shotgun): 99.47 %, regalloc-only
void cls_804B1668::fn_80433718() {
    if (field_0xa0 != getGameView_802D6A9C()->field_0x131dc) {
        field_0xa0 = getGameView_802D6A9C()->field_0x131dc;
        field_0x9e = getGameView_802D6A9C()->field_0x131dc;
        if (field_0x9e & 0x8000) {
            field_0xa4 = 1;
            field_0x9e &= 0x7FFF;
            getGameView_802D6A9C()->fn_802D7870(field_0x9e);
        } else {
            field_0xa4 = 0;
        }
    }
}

// FIXME(shotgun): 99.41 %
void cls_804B1668::fn_80432AF8() {
    field_0x90++;
    field_0x94++;
    if (field_0x90 >= field_0x10[0]->getFrameMax()) {
        field_0x90 = 0;
    }
    field_0x10[0]->setFrame(field_0x90);
    field_0x4[0]->animation();
    if (field_0x94 >= field_0x10[1]->getFrameMax()) {
        field_0x94 = 0;
    }
    field_0x10[1]->setFrame(field_0x94);
    field_0x4[1]->animation();
    field_0xac = 0;
    s32 player = gGameRoot_8053AAF8->field_0x7e == 0 ? 0 : gGameRoot2_8053AB10->field_0x20->field_0x230;
    if (player < 0) {
        return;
    }
    if (!(gGameRoot_8053AAF8->field_0x4c == 11 ? true : (gGameRoot_8053AAF8->field_0x88 == 2)) &&
        fn_80236768(fn_8022EA0C(player)) > 0 && fn_80236768(fn_8022EA0C(player)) < 900) {
        s32 val = fn_80236768(fn_8022EA0C(player)) / 60 + 1;
        if (val != field_0x98) {
            if (val > 99) {
                val = 99;
            }
            if (val < 0) {
                val = 0;
            }
            if (val < 10) {
                const ResTIMG* img = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[0]))->changeTexture(img, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[1]))->changeTexture(img, 0);
                if (field_0xc->search(lbl_8048F438[2])->isVisible() == true) {
                    field_0xc->search(lbl_8048F438[2])->hide();
                    field_0xc->search(lbl_8048F438[3])->hide();
                }
            } else {
                const ResTIMG* img10 = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val / 10]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[0]))->changeTexture(img10, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[1]))->changeTexture(img10, 0);
                const ResTIMG* img1 = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val % 10]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[2]))->changeTexture(img1, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[3]))->changeTexture(img1, 0);
                if (field_0xc->search(lbl_8048F438[2])->isVisible() == false) {
                    field_0xc->search(lbl_8048F438[2])->show();
                    field_0xc->search(lbl_8048F438[3])->show();
                }
            }
        }
        field_0x98 = val;
        field_0xac = 1;
    } else if (!(gGameRoot_8053AAF8->field_0x4c == 11 ? true : (gGameRoot_8053AAF8->field_0x88 == 2)) &&
               fn_80236630(fn_8022EA0C(player)) > 0 && fn_80236630(fn_8022EA0C(player)) < 1200) {
        s32 val = fn_80236630(fn_8022EA0C(player)) / 60 + 1;
        if (val != field_0x98) {
            if (val > 99) {
                val = 99;
            }
            if (val < 0) {
                val = 0;
            }
            if (val < 10) {
                const ResTIMG* img = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[0]))->changeTexture(img, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[1]))->changeTexture(img, 0);
                if (field_0xc->search(lbl_8048F438[2])->isVisible() == true) {
                    field_0xc->search(lbl_8048F438[2])->hide();
                    field_0xc->search(lbl_8048F438[3])->hide();
                }
            } else {
                const ResTIMG* img10 = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val / 10]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[0]))->changeTexture(img10, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[1]))->changeTexture(img10, 0);
                const ResTIMG* img1 = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val % 10]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[2]))->changeTexture(img1, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[3]))->changeTexture(img1, 0);
                if (field_0xc->search(lbl_8048F438[2])->isVisible() == false) {
                    field_0xc->search(lbl_8048F438[2])->show();
                    field_0xc->search(lbl_8048F438[3])->show();
                }
            }
        }
        field_0x98 = val;
        field_0xac = 1;
    } else if (!(gGameRoot_8053AAF8->field_0x4c == 11 ? true : (gGameRoot_8053AAF8->field_0x88 == 2)) &&
               fn_802366CC(fn_8022EA0C(player)) > 0 && fn_802366CC(fn_8022EA0C(player)) < 300) {
        s32 val = fn_802366CC(fn_8022EA0C(player)) / 60 + 1;
        if (val != field_0x98) {
            if (val > 99) {
                val = 99;
            }
            if (val < 0) {
                val = 0;
            }
            if (val < 10) {
                const ResTIMG* img = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[0]))->changeTexture(img, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[1]))->changeTexture(img, 0);
                if (field_0xc->search(lbl_8048F438[2])->isVisible() == true) {
                    field_0xc->search(lbl_8048F438[2])->hide();
                    field_0xc->search(lbl_8048F438[3])->hide();
                }
            } else {
                const ResTIMG* img10 = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val / 10]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[0]))->changeTexture(img10, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[1]))->changeTexture(img10, 0);
                const ResTIMG* img1 = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B1640[val % 10]);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[2]))->changeTexture(img1, 0);
                static_cast<J2DPicture*>(field_0xc->search(lbl_8048F438[3]))->changeTexture(img1, 0);
                if (field_0xc->search(lbl_8048F438[2])->isVisible() == false) {
                    field_0xc->search(lbl_8048F438[2])->show();
                    field_0xc->search(lbl_8048F438[3])->show();
                }
            }
        }
        field_0x98 = val;
        field_0xac = 1;
    }
}

// FIXME(shotgun): 98.72 %, regalloc-only
void cls_804B1668::fn_80432750(bool all, f32 x, f32 y, f32 w, f32 h) {
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0xa4 == 0) {
        f32 scaleX = w / field_0x18[2]->field_0x18;
        f32 scaleY = h / field_0x18[2]->field_0x1c;
        f32 centreX = field_0x18[2]->field_0x10;
        f32 left = field_0x4[0]->search('Ndata01')->getBounds()->i.x;
        f32 moveX = (w * 0.5f + x) - (centreX + left);
        f32 centreY = field_0x18[2]->field_0x14;
        f32 top = field_0x4[0]->search('Ndata01')->getBounds()->i.y;
        f32 moveY = (h * 0.5f + y) - (centreY + top);
        for (s32 i = 0; i < 4; i++) {
            field_0x18[i]->moveFromCentre_80402E08(moveX, moveY);
            field_0x18[i]->fn_80402CE0(scaleX, scaleY);
        }
        if (all && 1.0f == scaleX) {
            if (field_0x9e == 1 || field_0x9e == 2 || field_0x9e == 3) {
                field_0xa2 = 1;
            } else {
                field_0xa2 = 0;
            }
            if (field_0xa2 == field_0xa3) {
                if (!field_0x34->mPane->isVisible()) {
                    field_0x34->mPane->show();
                }
                if (field_0xa2 == 1) {
                    moveY += 290.0f;
                }
                field_0x34->moveFromCentre_80402E08(moveX, moveY);
            } else {
                if (field_0x34->mPane->isVisible()) {
                    field_0x34->mPane->hide();
                }
                field_0xa3 = field_0xa2;
            }
        } else {
            if (field_0x34->mPane->isVisible()) {
                field_0x34->mPane->hide();
            }
            if (1.0f != scaleX && field_0xac == 1) {
                field_0xac = 0;
            }
        }
        field_0x4[0]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    } else {
        f32 scaleX = w / field_0x28[1]->field_0x18;
        f32 scaleY = h / field_0x28[1]->field_0x1c;
        f32 centreX = field_0x28[1]->field_0x10;
        f32 left = field_0x4[1]->search('Ndata01')->getBounds()->i.x;
        f32 moveX = (w * 0.5f + x) - (centreX + left);
        f32 centreY = field_0x28[1]->field_0x14;
        f32 top = field_0x4[1]->search('Ndata01')->getBounds()->i.y;
        f32 moveY = (h * 0.5f + y) - (centreY + top);
        for (s32 i = 0; i < 3; i++) {
            field_0x28[i]->moveFromCentre_80402E08(moveX, moveY);
            field_0x28[i]->fn_80402CE0(scaleX, scaleY);
        }
        field_0x4[1]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}

// FIXME(shotgun): 99.14 %, regalloc-only
void cls_804B1668::fn_80432638() {
    if (field_0xac != 0) {
        f32 x = field_0x18[0]->mPane->getBounds()->i.x;
        f32 left = field_0x18[0]->field_0x8;
        f32 y = field_0x18[0]->mPane->getBounds()->i.y;
        f32 top = field_0x18[0]->field_0xc;
        gGameRoot2_8053AB10->fn_8013B490();
        field_0x3c->moveFromCentre_80402E08((x - left) + (f32)lbl_8050DB0C.field_0x4c[(0x23E - 0x4C) / 2],
                                            (y - top) + (f32)lbl_8050DB0C.field_0x4c[(0x240 - 0x4C) / 2]);
        field_0xc->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}
