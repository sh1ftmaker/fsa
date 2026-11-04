#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_804465B8.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"

// Pane tags, as u64 (names zero-padded to 8 bytes), four groups of 24. The
// constructor searches entries 6 and 23 of each group.
static const u64 lbl_80490038[4][24] = {
    {
        0x0000507031693031ull, 0x0000507031693032ull, 0x0050703169623031ull,
        0x0050703169623032ull, 0x0050703169623033ull, 0x0050703169623034ull,
        0x004E646174613031ull, 0x00000050316F3031ull, 0x00000050316F3032ull,
        0x0000503174643031ull, 0x00503174646B3031ull, 0x0000503174643032ull,
        0x00503174646B3032ull, 0x0000005031743031ull, 0x00005031746B3031ull,
        0x0000005031743032ull, 0x00005031746B3032ull, 0x0000005031743033ull,
        0x00005031746B3033ull, 0x0000005031743034ull, 0x00005031746B3034ull,
        0x0000005031743035ull, 0x00005031746B3035ull, 0x004E646174613230ull,
    },
    {
        0x0000507032693031ull, 0x0000507032693032ull, 0x0050703269623031ull,
        0x0050703269623032ull, 0x0050703269623033ull, 0x0050703269623034ull,
        0x004E646174613034ull, 0x00000050326F3031ull, 0x00000050326F3032ull,
        0x0000503274643031ull, 0x00503274646B3031ull, 0x0000503274643032ull,
        0x00503274646B3032ull, 0x0000005032743031ull, 0x00005032746B3031ull,
        0x0000005032743032ull, 0x00005032746B3032ull, 0x0000005032743033ull,
        0x00005032746B3033ull, 0x0000005032743034ull, 0x00005032746B3034ull,
        0x0000005032743035ull, 0x00005032746B3035ull, 0x004E646174613231ull,
    },
    {
        0x0000507033693031ull, 0x0000507033693032ull, 0x0050703369623031ull,
        0x0050703369623032ull, 0x0050703369623033ull, 0x0050703369623034ull,
        0x004E646174613037ull, 0x00000050336F3031ull, 0x00000050336F3032ull,
        0x0000503374643031ull, 0x00503374646B3031ull, 0x0000503374643032ull,
        0x00503374646B3032ull, 0x0000005033743031ull, 0x00005033746B3031ull,
        0x0000005033743032ull, 0x00005033746B3032ull, 0x0000005033743033ull,
        0x00005033746B3033ull, 0x0000005033743034ull, 0x00005033746B3034ull,
        0x0000005033743035ull, 0x00005033746B3035ull, 0x004E646174613232ull,
    },
    {
        0x0000507034693031ull, 0x0000507034693032ull, 0x0050703469623031ull,
        0x0050703469623032ull, 0x0050703469623033ull, 0x0050703469623034ull,
        0x004E646174613130ull, 0x00000050346F3031ull, 0x00000050346F3032ull,
        0x0000503474643031ull, 0x00503474646B3031ull, 0x0000503474643032ull,
        0x00503474646B3032ull, 0x0000005034743031ull, 0x00005034746B3031ull,
        0x0000005034743032ull, 0x00005034746B3032ull, 0x0000005034743033ull,
        0x00005034746B3033ull, 0x0000005034743034ull, 0x00005034746B3034ull,
        0x0000005034743035ull, 0x00005034746B3035ull, 0x004E646174613233ull,
    },
};

// The TIMG names of the digit pictures (kk_rupyNum_00 to _09).
static const char* lbl_804B1BC0[10] = {
    "kk_rupyNum_00.bti", "kk_rupyNum_01.bti", "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti", "kk_rupyNum_04.bti", "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti", "kk_rupyNum_07.bti", "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};

// FIXME: 92.82 %. The original takes field_0x2c / 60 twice: once for the
// seconds (r29 = field_0x2c - 60 * q) and again as the minutes index into
// lbl_804B1BC0 (a second srawi/srwi pair on the quotient, after the seconds).
// Ours shares one quotient. The field byte sits in r7 where the original has
// r8, and lis/addi of lbl_804B1BC0 are scheduled before the 0x6666 constant.
void cls_804B1BE8::fn_804465B8() {
    for (s32 i = 0; i < 4; i++) {
        if (i < 2) {
            field_0x8[i]->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
            field_0x18[i]->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
        } else {
            field_0x8[i]->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
            field_0x18[i]->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
        }
        if (field_0x30 & (u8)(1 << i)) {
            if (!mScreen->search(lbl_80490038[i][6])->isVisible()) {
                mScreen->search(lbl_80490038[i][6])->show();
                mScreen->search(lbl_80490038[i][10])->show();
                mScreen->search(lbl_80490038[i][12])->show();
                mScreen->search(lbl_80490038[i][14])->show();
                mScreen->search(lbl_80490038[i][16])->show();
                mScreen->search(lbl_80490038[i][18])->show();
                mScreen->search(lbl_80490038[i][20])->show();
                mScreen->search(lbl_80490038[i][22])->show();
            }
        } else {
            if (mScreen->search(lbl_80490038[i][6])->isVisible()) {
                mScreen->search(lbl_80490038[i][6])->hide();
                mScreen->search(lbl_80490038[i][10])->hide();
                mScreen->search(lbl_80490038[i][12])->hide();
                mScreen->search(lbl_80490038[i][14])->hide();
                mScreen->search(lbl_80490038[i][16])->hide();
                mScreen->search(lbl_80490038[i][18])->hide();
                mScreen->search(lbl_80490038[i][20])->hide();
                mScreen->search(lbl_80490038[i][22])->hide();
            }
        }
        if (field_0x30 & (u8)(1 << (i + 4))) {
            if (!mScreen->search(lbl_80490038[i][8])->isVisible()) {
                mScreen->search(lbl_80490038[i][8])->show();
            }
        } else {
            if (mScreen->search(lbl_80490038[i][8])->isVisible()) {
                mScreen->search(lbl_80490038[i][8])->hide();
            }
        }
        if (mScreen->search(lbl_80490038[i][6])->isVisible()) {
            if (field_0x28[i] >= 100) {
                field_0x28[i] = 99;
            }
            s32 hundredths = field_0x28[i];
            s32 seconds = field_0x2c[i] % 60;
            s32 tensSeconds = seconds / 10;
            s32 tensHundredths = hundredths / 10;
            const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1BC0[field_0x2c[i] / 60]));
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][13]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][14]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1BC0[tensSeconds]));
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][15]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][16]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1BC0[seconds % 10]));
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][17]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][18]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1BC0[tensHundredths]));
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][19]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][20]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1BC0[hundredths % 10]));
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][21]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][22]))->changeTexture(res, 0);
        }
    }
    field_0x30 = 0;
}

void cls_804B1BE8::fn_80446D68() {
    lbl_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
}

void cls_804B1BE8::fn_80446DB0() {
    for (s32 i = 0; i < 4; i++) {
        field_0x30 &= ~(u8)(1 << i);
        field_0x30 &= ~(u8)(1 << (i + 4));
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1BC0[0]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][13]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][14]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][15]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][16]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][17]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][18]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][19]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][20]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][21]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490038[i][22]))->changeTexture(res, 0);
        mScreen->search(lbl_80490038[i][6])->hide();
        mScreen->search(lbl_80490038[i][10])->hide();
        mScreen->search(lbl_80490038[i][12])->hide();
        mScreen->search(lbl_80490038[i][14])->hide();
        mScreen->search(lbl_80490038[i][16])->hide();
        mScreen->search(lbl_80490038[i][18])->hide();
        mScreen->search(lbl_80490038[i][20])->hide();
        mScreen->search(lbl_80490038[i][22])->hide();
        mScreen->search(lbl_80490038[i][8])->hide();
    }
}

cls_804B1BE8::~cls_804B1BE8() {
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x8[i];
        field_0x8[i] = NULL;
        delete field_0x18[i];
        field_0x18[i] = NULL;
    }
    lbl_8053AF00 = NULL;
}

cls_804B1BE8::cls_804B1BE8() {
    lbl_8053AF00 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("mg_oni.blo", 0x1100000, lbl_80529DEC.field_0x10);
    for (s32 i = 0; i < 4; i++) {
        field_0x8[i] = new cls_804AF86C();
        field_0x8[i]->fn_80402F04(mScreen->search(lbl_80490038[i][6]));
        field_0x18[i] = new cls_804AF86C();
        field_0x18[i]->fn_80402F04(mScreen->search(lbl_80490038[i][23]));
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x28[i] = 0;
        field_0x2c[i] = 0;
    }
    field_0x30 = 0;
}
