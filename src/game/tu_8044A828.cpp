#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804B1D00.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"

// Pane tags, as u64: 'Pfi01', 'Pfik01', 'Pf01', 'Pfk01', 'Pf02', 'Pfk02',
// 'Pf03', 'Pfk03', then 'Ndata01'.
static const u64 lbl_80490DB8[9] = {
    0x0000005066693031ull, 0x00005066696B3031ull, 0x0000000050663031ull,
    0x00000050666B3031ull, 0x0000000050663032ull, 0x00000050666B3032ull,
    0x0000000050663033ull, 0x00000050666B3033ull, 0x004E646174613031ull,
};

// Pane tags, as u64: 'Ptim01', 'Ptim02', 'Ptmd01', 'Ptmdk01', then 'Pt01',
// 'Ptk01', 'Pt02', 'Ptk02', 'Pt03', 'Ptk03', 'Pt04', 'Ptk04', 'Ndata00'.
static const u64 lbl_80490E00[13] = {
    0x00005074696D3031ull, 0x00005074696D3032ull, 0x000050746D643031ull,
    0x0050746D646B3031ull, 0x0000000050743031ull, 0x00000050746B3031ull,
    0x0000000050743032ull, 0x00000050746B3032ull, 0x0000000050743033ull,
    0x00000050746B3033ull, 0x0000000050743034ull, 0x00000050746B3034ull,
    0x004E646174613030ull,
};

// The 'TIMG' names of the digit pictures (kk_rupyNum_00 to _09).
static const char* lbl_804B1CD8[10] = {
    "kk_rupyNum_00.bti", "kk_rupyNum_01.bti", "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti", "kk_rupyNum_04.bti", "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti", "kk_rupyNum_07.bti", "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};

cls_804B1D00::cls_804B1D00() {
    lbl_8053AF28 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("mg_volleyball.blo", 0x1100000, lbl_80529DEC.field_0x10);
    mPanes[0] = new PaneCtrl_804AF86C();
    mPanes[0]->setPane_80402F04(mScreen->search(0x004E646174613030ull /* "Ndata00" */));
    mPanes[1] = new PaneCtrl_804AF86C();
    mPanes[1]->setPane_80402F04(mScreen->search(0x004E646174613031ull /* "Ndata01" */));
    field_0x10 = 0;
    field_0x12 = 0;
    field_0x13 = 0;
    field_0x14 = 0;
}

cls_804B1D00::~cls_804B1D00() {
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 2; i++) {
        delete mPanes[i];
        mPanes[i] = NULL;
    }
    lbl_8053AF28 = NULL;
}

void cls_804B1D00::fn_8044AFD8() {
    field_0x14 &= ~1;
    const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[0]));
    static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[2]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[3]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[4]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[5]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[6]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[7]))->changeTexture(res, 0);
    field_0x14 &= ~2;
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[0]));
    static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[4]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[5]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[6]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[7]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[8]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[9]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[10]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[11]))->changeTexture(res, 0);
    mScreen->search(lbl_80490DB8[8])->hide();
    mScreen->search(lbl_80490E00[12])->hide();
}

void cls_804B1D00::fn_8044AF90() {
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

// FIXME: 90.50 %. Register allocation in the digit blocks: the original's
// hundreds index comes from the same mulhw result (r7) as the tens, and it
// saves r27-r31 where ours saves r26-r31 (our hundreds lives in r29 across
// the first fn_804030C4 call). The tens of field_0x13 and units of field_0x12
// are scheduled in other registers too (arg_mismatch rows from 0x4C8).
void cls_804B1D00::fn_8044A828() {
    for (s32 i = 0; i < 2; i++) {
        mPanes[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
    }
    if (field_0x14 & 1) {
        if (!mScreen->search(lbl_80490DB8[8])->isVisible()) {
            mScreen->search(lbl_80490DB8[8])->show();
        }
    } else {
        if (mScreen->search(lbl_80490DB8[8])->isVisible()) {
            mScreen->search(lbl_80490DB8[8])->hide();
        }
    }
    if (mScreen->search(lbl_80490DB8[8])->isVisible()) {
        if (field_0x10 >= 1000) {
            field_0x10 = 999;
        }
        s32 value = field_0x10;
        s32 hundreds = value / 100;
        s32 tens = (value - hundreds * 100) / 10;
        s32 units = value - (value / 10) * 10;
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[hundreds]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[2]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[3]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[tens]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[4]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[5]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[value - (value / 10) * 10]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[6]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490DB8[7]))->changeTexture(res, 0);
    }
    if (field_0x14 & 2) {
        if (!mScreen->search(lbl_80490E00[12])->isVisible()) {
            mScreen->search(lbl_80490E00[12])->show();
        }
    } else {
        if (mScreen->search(lbl_80490E00[12])->isVisible()) {
            mScreen->search(lbl_80490E00[12])->hide();
        }
    }
    if (mScreen->search(lbl_80490E00[12])->isVisible()) {
        if (field_0x12 >= 100) {
            field_0x12 = 99;
            field_0x13 = 99;
        }
        if (field_0x13 >= 100) {
            field_0x13 = 99;
        }
        s32 value12 = field_0x12;
        s32 value13 = field_0x13;
        s32 tens12 = value12 / 10;
        s32 units12 = value12 - tens12 * 10;
        s32 tens13 = value13 / 10;
        s32 units13 = value13 - tens13 * 10;
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[tens12]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[4]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[5]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[units12]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[6]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[7]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[tens13]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[8]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[9]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1CD8[units13]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[10]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490E00[11]))->changeTexture(res, 0);
    }
    field_0x14 = 0;
}
