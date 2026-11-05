#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_8044998C.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"

// Pane tags, as u64: 'Pfi01', 'Pfik01', 'Pf01', 'Pfk01', 'Pf02', 'Pfk02',
// 'Ndata01'.
static const u64 lbl_80490C40[7] = {
    0x0000005066693031ull, 0x00005066696B3031ull, 0x0000000050663031ull,
    0x00000050666B3031ull, 0x0000000050663032ull, 0x00000050666B3032ull,
    0x004E646174613031ull,
};

// Pane tags, as u64: 'Ptim01', 'Ptim02', 'Ptmd01', 'Ptmdk01', then 'Pt01',
// 'Ptk01', 'Pt02', 'Ptk02', 'Pt03', 'Ptk03', 'Pt04', 'Ptk04', 'Ndata00'.
static const u64 lbl_80490C78[13] = {
    0x00005074696D3031ull, 0x00005074696D3032ull, 0x000050746D643031ull,
    0x0050746D646B3031ull, 0x0000000050743031ull, 0x00000050746B3031ull,
    0x0000000050743032ull, 0x00000050746B3032ull, 0x0000000050743033ull,
    0x00000050746B3033ull, 0x0000000050743034ull, 0x00000050746B3034ull,
    0x004E646174613030ull,
};

// The 'TIMG' names of the digit pictures (kk_rupyNum_00 to _09).
static const char* lbl_804B1CA0[10] = {
    "kk_rupyNum_00.bti", "kk_rupyNum_01.bti", "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti", "kk_rupyNum_04.bti", "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti", "kk_rupyNum_07.bti", "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};

// Updates the digit pictures of the counters and shows or hides their panes.
// FIXME: 91.22 %. Mostly register assignment (arg_mismatch): the target keeps
// field_0x12 in r31 and its tens in r30, ours in r30 and r28. The lis/addi
// loads of lbl_804B1CA0 sit at other points in our schedule, so those reloc
// rows differ too.
void cls_804B1CC8::fn_8044998C() {
    for (s32 i = 0; i < 2; i++) {
        mPanes[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
    }
    if (field_0x13 & 1) {
        if (!mScreen->search(lbl_80490C40[6])->isVisible()) {
            mScreen->search(lbl_80490C40[6])->show();
        }
    } else {
        if (mScreen->search(lbl_80490C40[6])->isVisible()) {
            mScreen->search(lbl_80490C40[6])->hide();
        }
    }
    if (mScreen->search(lbl_80490C40[6])->isVisible()) {
        if (field_0x10 >= 100) {
            field_0x10 = 99;
        }
        s32 value = field_0x10;
        s32 units = value - (value / 10) * 10;
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CA0[value / 10]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C40[2]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C40[3]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CA0[units]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C40[4]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C40[5]))->changeTexture(res, 0);
    }
    if (field_0x13 & 2) {
        if (!mScreen->search(lbl_80490C78[12])->isVisible()) {
            mScreen->search(lbl_80490C78[12])->show();
        }
    } else {
        if (mScreen->search(lbl_80490C78[12])->isVisible()) {
            mScreen->search(lbl_80490C78[12])->hide();
        }
    }
    if (mScreen->search(lbl_80490C78[12])->isVisible()) {
        if (field_0x11 >= 100) {
            field_0x11 = 99;
            field_0x12 = 99;
        }
        if (field_0x12 >= 100) {
            field_0x12 = 99;
        }
        s32 a = field_0x11;
        s32 b = field_0x12;
        s32 bTens = b / 10;
        s32 aUnits = a - (a / 10) * 10;
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CA0[a / 10]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[4]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[5]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CA0[aUnits]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[6]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[7]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CA0[bTens]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[8]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[9]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CA0[b % 10]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[10]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[11]))->changeTexture(res, 0);
    }
    field_0x13 = 0;
}

void cls_804B1CC8::fn_8044A024() {
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

// Sets the digit pictures to the 'TIMG' name lbl_804B1CA0[0] and hides the
// panes 'Ndata01' and 'Ndata00'.
void cls_804B1CC8::fn_8044A06C() {
    field_0x13 &= ~1;
    const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CA0[0]));
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C40[2]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C40[3]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C40[4]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C40[5]))->changeTexture(res, 0);
    field_0x13 &= ~2;
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CA0[0]));
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[4]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[5]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[6]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[7]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[8]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[9]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[10]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490C78[11]))->changeTexture(res, 0);
    mScreen->search(lbl_80490C40[6])->hide();
    mScreen->search(lbl_80490C78[12])->hide();
}

cls_804B1CC8::~cls_804B1CC8() {
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 2; i++) {
        delete mPanes[i];
        mPanes[i] = NULL;
    }
    lbl_8053AF20 = NULL;
}

cls_804B1CC8::cls_804B1CC8() {
    lbl_8053AF20 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("mg_fishing.blo", 0x1100000, lbl_80529DEC.field_0x10);
    mPanes[0] = new PaneCtrl_804AF86C();
    mPanes[0]->setPane_80402F04(mScreen->search(0x004E646174613030ull /* "Ndata00" */));
    mPanes[1] = new PaneCtrl_804AF86C();
    mPanes[1]->setPane_80402F04(mScreen->search(0x004E646174613031ull /* "Ndata01" */));
    field_0x10 = 0;
    field_0x11 = 0;
    field_0x12 = 0;
    field_0x13 = 0;
}
