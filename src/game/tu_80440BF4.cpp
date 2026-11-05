#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/tu_80440BF4.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"

// Tags of the seven panes: 'Pba04', 'Pba03', 'Pba02', 'Pba01', 'Pbb03',
// 'Pbb02', 'Pbb01'.
static const u64 lbl_8048FAA8[7] = {
    0x5062613034ull, 0x5062613033ull, 0x5062613032ull, 0x5062613031ull,
    0x5062623033ull, 0x5062623032ull, 0x5062623031ull,
};

void cls_804B1988::fn_80440BF4(bool show) {
    if (show) {
        mScreen->search('Pbb03')->show();
    } else {
        mScreen->search('Pbb03')->hide();
    }
}

void cls_804B1988::fn_80440C6C(bool show) {
    if (show) {
        mScreen->search('Pba04')->show();
    } else {
        mScreen->search('Pba04')->hide();
    }
}

void cls_804B1988::fn_80440CE4(const ResTIMG* tex) {
    mScreen->search('Pbb03')->show();
    ((J2DPicture*)mScreen->search('Pbb02'))->changeTexture(tex, 0);
}

void cls_804B1988::fn_80440D78(const ResTIMG* tex) {
    mScreen->search('Pba04')->show();
    ((J2DPicture*)mScreen->search('Pba02'))->changeTexture(tex, 0);
}

void cls_804B1988::fn_80440E0C(f32 alpha) {
    for (s32 i = 0; i < 7; i++) {
        mScreen->search(lbl_8048FAA8[i])->setAlpha(mAlpha[i] * alpha);
    }
}

void cls_804B1988::fn_80440EE8() {
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

cls_804B1988::~cls_804B1988() {
    delete mScreen;
    mScreen = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    lbl_8053AED8 = NULL;
}

cls_804B1988::cls_804B1988() {
    lbl_8053AED8 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("button.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x8 = new PaneCtrl_804AF86C();
    field_0x8->setPane_80402F04(mScreen->search(0x4e646174613031ull /* "Ndata01" */));
    for (s32 i = 0; i < 7; i++) {
        mAlpha[i] = mScreen->search(lbl_8048FAA8[i])->getAlpha();
    }
}
