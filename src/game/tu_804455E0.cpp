#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_804455E0.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"

// Pane tags, as u64 (names zero-padded to 8 bytes), four groups of 24. The
// constructor searches entries 1, 8 and 23 of each group.
static const u64 lbl_8048FC08[4][24] = {
    {
        0x0050313173743031ull, 0x0050313173743032ull, 0x0000507031693031ull,
        0x0000507031693032ull, 0x0050703169623031ull, 0x0050703169623032ull,
        0x0050703169623033ull, 0x0050703169623034ull, 0x004E646174613031ull,
        0x0000503174643031ull, 0x00503174646B3031ull, 0x0000503174643032ull,
        0x00503174646B3032ull, 0x0000005031743031ull, 0x00005031746B3031ull,
        0x0000005031743032ull, 0x00005031746B3032ull, 0x0000005031743033ull,
        0x00005031746B3033ull, 0x0000005031743034ull, 0x00005031746B3034ull,
        0x0000005031743035ull, 0x00005031746B3035ull, 0x004E646174613230ull,
    },
    {
        0x0050323173743031ull, 0x0050323173743032ull, 0x0000507032693031ull,
        0x0000507032693032ull, 0x0050703269623031ull, 0x0050703269623032ull,
        0x0050703269623033ull, 0x0050703269623034ull, 0x004E646174613034ull,
        0x0000503274643031ull, 0x00503274646B3031ull, 0x0000503274643032ull,
        0x00503274646B3032ull, 0x0000005032743031ull, 0x00005032746B3031ull,
        0x0000005032743032ull, 0x00005032746B3032ull, 0x0000005032743033ull,
        0x00005032746B3033ull, 0x0000005032743034ull, 0x00005032746B3034ull,
        0x0000005032743035ull, 0x00005032746B3035ull, 0x004E646174613231ull,
    },
    {
        0x0050333173743031ull, 0x0050333173743032ull, 0x0000507033693031ull,
        0x0000507033693032ull, 0x0050703369623031ull, 0x0050703369623032ull,
        0x0050703369623033ull, 0x0050703369623034ull, 0x004E646174613037ull,
        0x0000503374643031ull, 0x00503374646B3031ull, 0x0000503374643032ull,
        0x00503374646B3032ull, 0x0000005033743031ull, 0x00005033746B3031ull,
        0x0000005033743032ull, 0x00005033746B3032ull, 0x0000005033743033ull,
        0x00005033746B3033ull, 0x0000005033743034ull, 0x00005033746B3034ull,
        0x0000005033743035ull, 0x00005033746B3035ull, 0x004E646174613232ull,
    },
    {
        0x0050343173743031ull, 0x0050343173743032ull, 0x0000507034693031ull,
        0x0000507034693032ull, 0x0050703469623031ull, 0x0050703469623032ull,
        0x0050703469623033ull, 0x0050703469623034ull, 0x004E646174613130ull,
        0x0000503474643031ull, 0x00503474646B3031ull, 0x0000503474643032ull,
        0x00503474646B3032ull, 0x0000005034743031ull, 0x00005034746B3031ull,
        0x0000005034743032ull, 0x00005034746B3032ull, 0x0000005034743033ull,
        0x00005034746B3033ull, 0x0000005034743034ull, 0x00005034746B3034ull,
        0x0000005034743035ull, 0x00005034746B3035ull, 0x004E646174613233ull,
    },
};

// The TIMG names of the digit pictures (kk_rupyNum_00 to _09) and of the rank
// pictures (kk_mainRank_01 to _04).
static const char* lbl_804B1B78[10] = {
    "kk_rupyNum_00.bti", "kk_rupyNum_01.bti", "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti", "kk_rupyNum_04.bti", "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti", "kk_rupyNum_07.bti", "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};

static const char* lbl_804B1BA0[4] = {
    "kk_mainRank_01.bti", "kk_mainRank_02.bti", "kk_mainRank_03.bti",
    "kk_mainRank_04.bti",
};

// FIXME: 91.85 %. Only the time-digit block (after the field_0x44 bit test, before
// the rank part) differs: the original loads field_0x3c's seconds into r8 (ours r7),
// schedules lis lbl_804B1B78 before lis 0x5449, and loads field_0x38 before the
// 0x6667 constant; it keeps the minutes picture in r23 (a ninth callee-saved
// register), while ours stores it in r25. Tried: separate minutes, seconds and
// centiseconds texture locals (90.9-91.05 %), a minutes local for the division.
void cls_804B1BB0::fn_804455E0() {
    for (s32 i = 0; i < 4; i++) {
        if (i < 2) {
            field_0x8[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
            field_0x18[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
            field_0x28[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
        } else {
            field_0x8[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
            field_0x18[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
            field_0x28[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
        }
        if (field_0x44 & (u8)(1 << i)) {
            if (!mScreen->search(lbl_8048FC08[i][23])->isVisible()) {
                mScreen->search(lbl_8048FC08[i][8])->show();
                mScreen->search(lbl_8048FC08[i][23])->show();
            }
        } else {
            if (mScreen->search(lbl_8048FC08[i][23])->isVisible()) {
                mScreen->search(lbl_8048FC08[i][8])->hide();
                mScreen->search(lbl_8048FC08[i][23])->hide();
            }
        }
        if (mScreen->search(lbl_8048FC08[i][23])->isVisible()) {
            if (field_0x38[i] >= 100) {
                field_0x38[i] = 99;
            }
            s32 seconds = field_0x3c[i];
            s32 secs = seconds % 60;
            s32 centis = field_0x38[i];
            s32 secTens = secs / 10;
            s32 centiTens = centis / 10;
            const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1B78[seconds / 60]));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][13]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][14]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1B78[secTens]));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][15]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][16]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1B78[secs % 10]));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][17]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][18]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1B78[centiTens]));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][19]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][20]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1B78[centis % 10]));
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][21]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][22]))->changeTexture(res, 0);
        }
        if (field_0x44 & (u8)(1 << (i + 4))) {
            if (!mScreen->search(lbl_8048FC08[i][1])->isVisible()) {
                mScreen->search(lbl_8048FC08[i][1])->show();
            }
        } else {
            if (mScreen->search(lbl_8048FC08[i][1])->isVisible()) {
                mScreen->search(lbl_8048FC08[i][1])->hide();
            }
        }
        if (mScreen->search(lbl_8048FC08[i][1])->isVisible()) {
            u8 rank = field_0x40[i];
            if (rank >= 1 && rank <= 4) {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1BA0[rank - 1]));
                static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][0]))->changeTexture(res, 0);
                static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][1]))->changeTexture(res, 0);
            }
        }
    }
    field_0x44 = 0;
}

void cls_804B1BB0::fn_80445CDC() {
    lbl_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
}

void cls_804B1BB0::fn_80445D24() {
    for (s32 i = 0; i < 4; i++) {
        field_0x44 &= ~(u8)(1 << i);
        field_0x44 &= ~(u8)(1 << (i + 4));
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1B78[0]));
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][13]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][14]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][15]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][16]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][17]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][18]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][19]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][20]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][21]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][22]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1BA0[0]));
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][0]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_8048FC08[i][1]))->changeTexture(res, 0);
        mScreen->search(lbl_8048FC08[i][8])->hide();
        mScreen->search(lbl_8048FC08[i][23])->hide();
        mScreen->search(lbl_8048FC08[i][1])->hide();
    }
}

cls_804B1BB0::~cls_804B1BB0() {
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x8[i];
        field_0x8[i] = NULL;
        delete field_0x18[i];
        field_0x18[i] = NULL;
        delete field_0x28[i];
        field_0x28[i] = NULL;
    }
    lbl_8053AEF8 = NULL;
}

cls_804B1BB0::cls_804B1BB0() {
    lbl_8053AEF8 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("mg_horse_race.blo", 0x1100000, lbl_80529DEC.field_0x10);
    for (s32 i = 0; i < 4; i++) {
        field_0x8[i] = new PaneCtrl_804AF86C();
        field_0x8[i]->setPane_80402F04(mScreen->search(lbl_8048FC08[i][1]));
        field_0x18[i] = new PaneCtrl_804AF86C();
        field_0x18[i]->setPane_80402F04(mScreen->search(lbl_8048FC08[i][8]));
        field_0x28[i] = new PaneCtrl_804AF86C();
        field_0x28[i]->setPane_80402F04(mScreen->search(lbl_8048FC08[i][23]));
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x38[i] = 0;
        field_0x3c[i] = 0;
        field_0x40[i] = 1;
    }
    field_0x44 = 0;
}
