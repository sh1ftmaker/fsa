#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/THPMovie.h"
#include "game/act/tu_80440BF4.h"
#include "game/tu_803DAC3C.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "dolphin/dvd.h"
#include "dolphin/os.h"
#include "dolphin/pad.h"
#include <string.h>

// .rodata: the pane tag tables (J2DPane::search), in address order; the
// functions address them off one base (lbl_804774C8). Each has an alpha
// array of the same length in GameMode_804B0688 (field_0xa3 ...).
static const u64 lbl_804774C8[50] = {
    'Pmov01', 'Pexbk02', 'Pexbk01', 'Pexb07', 'Pexb06', 'Pexb05', 'Pexb04',
    'Pexb03', 'Pexb02', 'Pexb01', 'Pmf06', 'Pmf05', 'Pmf04', 'Pmf03',
    'Pmf02', 'Pmf01', 'Wgbco01', 'Pogb02', 'Pogb01', 'Tagb02', 'Tagb01',
    'Wnop01', 'Tnop02', 'Tnop01', 'Pexpb09', 'Pexpb08', 'Pexpb07', 'Pexpb06',
    'Pexpb05', 'Pexpb04', 'Pexpb03', 'Pexpb02', 'Pexpb01', 'Texp01', 'Wnop02',
    'Pcon04', 'Pcon03', 'Pgba04', 'Pgba03', 'Psls04', 'Psls03', 'Tnop04',
    'Tnop03', 'Wgbco02', 'Pogb03', 'Pogb04', 'Tagb03', 'Tagb04', 'Tnop05',
    'Tnop06',
};
static const u64 lbl_80477658[6] = {
    'Pmopb04', 'Pmopb03', 'Pmopb02', 'Pmopb01', 'Pmop02', 'Pmop01',
};
static const u64 lbl_80477688[17] = {
    'Pmadbb07', 'Pmadbb06', 'Pmadbb05', 'Pmadbb04', 'Pmadbb03', 'Pmadbb02',
    'Pmadbb01', 'Pmadb05', 'Pmadb04', 'Pmadb03', 'Pmadb02', 'Pmadb01',
    'Pmad02', 'Pmad01', 'Phy03', 'Phy02', 'Phy01',
};
static const u64 lbl_80477710[20] = {
    'Pmbabb07', 'Pmbabb06', 'Pmbabb05', 'Pmbabb04', 'Pmbabb03', 'Pmbabb02',
    'Pmbabb01', 'Pmbab05', 'Pmbab04', 'Pmbab03', 'Pmbab02', 'Pmbab01',
    'Pmbam02', 'Pmbam01', 'Pmbalk02', 'Pmbal02', 'Pmbalk01', 'Pmbal01',
    'Pmba02', 'Pmba01',
};
// No function of the unit reads it (field_0x100 would hold its alphas).
static const u64 lbl_804777B0[16] = {
    'Pmnvbb07', 'Pmnvbb06', 'Pmnvbb05', 'Pmnvbb04', 'Pmnvbb03', 'Pmnvbb02',
    'Pmnvbb01', 'Pmnvb05', 'Pmnvb04', 'Pmnvb03', 'Pmnvb02', 'Pmnvb01',
    'Pmnv02', 'Pmnv01', 'Pmte02', 'Pmte01',
};
static const u64 lbl_80477830[5] = {
    'Pstrb04', 'Pstrb03', 'Pstrb02', 'Pstrb01', 'Tstr01',
};
static const u64 lbl_80477858[5] = {
    'Pbacb04', 'Pbacb03', 'Pbacb02', 'Pbacb01', 'Tbac01',
};
// fn_803DEC44.
static const s32 lbl_80477880[3] = {0x34, 0x35, 0x36};
static const s32 lbl_8047788C[3] = {0xF3, 0xF4, 0xF3};

// Run each frame by fn_803DF810 (index: mState).
static GameMode_804B0688::StateFunc lbl_804B0568[8] = {
    &GameMode_804B0688::fn_803DF2B4, &GameMode_804B0688::fn_803DEC44,
    &GameMode_804B0688::fn_803DE58C, &GameMode_804B0688::fn_803DE05C,
    &GameMode_804B0688::fn_803DD970, &GameMode_804B0688::fn_803DD93C,
    &GameMode_804B0688::fn_803DD8E8, &GameMode_804B0688::fn_803DD764,
};
// Run each frame by fn_803DF624 (index: mState).
static GameMode_804B0688::StateFunc lbl_804B0628[8] = {
    &GameMode_804B0688::fn_803DD760, &GameMode_804B0688::fn_803DD3E0,
    &GameMode_804B0688::fn_803DD3DC, &GameMode_804B0688::fn_803DD1A4,
    &GameMode_804B0688::fn_803DD1A0, &GameMode_804B0688::fn_803DD19C,
    &GameMode_804B0688::fn_803DD19C, &GameMode_804B0688::fn_803DD19C,
};

GameMode_804B0688* lbl_8053ADD0;

s32 GameMode_804B0688::fn_803DAC3C() {
    if (lbl_8053ADD0 != NULL) {
        return lbl_8053ADD0->field_0x68;
    }
    return 2;
}

void GameMode_804B0688::fn_803DAC58(s32 entry, f32 alpha) {
    switch (entry) {
    case 0:
        static_cast<J2DPicture*>(mScreen->search('Pscurh01'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh02'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh03'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh04'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh05'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs02'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs03'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs04'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs05'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        mScreen->search('Pscurh05')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurh04')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurh03')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurh02')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurh01')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurs05')->setAlpha(100);
        mScreen->search('Pscurs04')->setAlpha(100);
        mScreen->search('Pscurs03')->setAlpha(100);
        mScreen->search('Pscurs02')->setAlpha(100);
        mScreen->search('Pscurs01')->setAlpha(100);
        static_cast<J2DPicture*>(mScreen->search('Pmad01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(255, 210, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pmba01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(130, 170, 0, 255));
        break;
    case 1:
        static_cast<J2DPicture*>(mScreen->search('Pscurh01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh02'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh03'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh04'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh05'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs01'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs02'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs03'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs04'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs05'))->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        mScreen->search('Pscurh05')->setAlpha(100);
        mScreen->search('Pscurh04')->setAlpha(100);
        mScreen->search('Pscurh03')->setAlpha(100);
        mScreen->search('Pscurh02')->setAlpha(100);
        mScreen->search('Pscurh01')->setAlpha(100);
        mScreen->search('Pscurs05')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurs04')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurs03')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurs02')->setAlpha(100.0f * alpha);
        mScreen->search('Pscurs01')->setAlpha(100.0f * alpha);
        static_cast<J2DPicture*>(mScreen->search('Pmad01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(130, 170, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pmba01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(255, 210, 0, 255));
        break;
    case 2:
        static_cast<J2DPicture*>(mScreen->search('Pscurh01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh02'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh03'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh04'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh05'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs02'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs03'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs04'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs05'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        mScreen->search('Pscurh05')->setAlpha(100);
        mScreen->search('Pscurh04')->setAlpha(100);
        mScreen->search('Pscurh03')->setAlpha(100);
        mScreen->search('Pscurh02')->setAlpha(100);
        mScreen->search('Pscurh01')->setAlpha(100);
        mScreen->search('Pscurs05')->setAlpha(100);
        mScreen->search('Pscurs04')->setAlpha(100);
        mScreen->search('Pscurs03')->setAlpha(100);
        mScreen->search('Pscurs02')->setAlpha(100);
        mScreen->search('Pscurs01')->setAlpha(100);
        static_cast<J2DPicture*>(mScreen->search('Pmad01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(130, 170, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pmba01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(130, 170, 0, 255));
        break;
    default:
        static_cast<J2DPicture*>(mScreen->search('Pscurh01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh02'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh03'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh04'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurh05'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs02'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs03'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs04'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pscurs05'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
        mScreen->search('Pscurh05')->setAlpha(100);
        mScreen->search('Pscurh04')->setAlpha(100);
        mScreen->search('Pscurh03')->setAlpha(100);
        mScreen->search('Pscurh02')->setAlpha(100);
        mScreen->search('Pscurh01')->setAlpha(100);
        mScreen->search('Pscurs05')->setAlpha(100);
        mScreen->search('Pscurs04')->setAlpha(100);
        mScreen->search('Pscurs03')->setAlpha(100);
        mScreen->search('Pscurs02')->setAlpha(100);
        mScreen->search('Pscurs01')->setAlpha(100);
        static_cast<J2DPicture*>(mScreen->search('Pmad01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(130, 170, 0, 255));
        static_cast<J2DPicture*>(mScreen->search('Pmba01'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(130, 170, 0, 255));
        mScreen->search('Pscuro02')->setAlpha(field_0xa2 * alpha);
        mScreen->search('Pscuro01')->setAlpha(field_0xa2 * alpha);
        break;
    }
}

// Cross-fades the movie screen's 'Pim' pictures from the previous entry
// (field_0x9f) to the selected one (field_0x9e) over the frames of
// lbl_8050DB0C's halfword 0x12E.
void GameMode_804B0688::fn_803DCCA4() {
    if (field_0x90 == 0) {
        return;
    }
    field_0x90++;
    f32 t = (f32)field_0x90 / (f32)lbl_8050DB0C.field_0x4c[0x71];
    if (field_0x90 >= lbl_8050DB0C.field_0x4c[0x71]) {
        field_0x90 = 0;
    }
    if (field_0x9e != 0 && field_0x9f != 0) {
        mMovieScreen->search('Pim01')->setAlpha(0);
        mMovieScreen->search('Pim02')->setAlpha(0);
    }
    if (field_0x9e != 1 && field_0x9f != 1) {
        mMovieScreen->search('Pim11')->setAlpha(0);
        mMovieScreen->search('Pim12')->setAlpha(0);
    }
    if (mState != 2) {
        switch (field_0x9e) {
        case 0:
            mMovieScreen->search('Pim01')->setAlpha(field_0x11a[0] * t);
            mMovieScreen->search('Pim02')->setAlpha(field_0x11a[1] * t);
            break;
        case 1:
            mMovieScreen->search('Pim11')->setAlpha(field_0x11a[2] * t);
            mMovieScreen->search('Pim12')->setAlpha(field_0x11a[3] * t);
            break;
        case 2: // entry 2 has no movie pictures
            break;
        }
    }
    t = 1.0f - t;
    if (mState == 1 && field_0x9e == field_0x9f) {
        return;
    }
    switch (field_0x9f) {
    case 0:
        mMovieScreen->search('Pim01')->setAlpha(field_0x11a[0] * t);
        mMovieScreen->search('Pim02')->setAlpha(field_0x11a[1] * t);
        break;
    case 1:
        mMovieScreen->search('Pim11')->setAlpha(field_0x11a[2] * t);
        mMovieScreen->search('Pim12')->setAlpha(field_0x11a[3] * t);
        break;
    case 2: // entry 2 has no movie pictures
        break;
    }
}

void GameMode_804B0688::fn_803DD19C() {}

void GameMode_804B0688::fn_803DD1A0() {}

void GameMode_804B0688::fn_803DD1A4() {
    f32 alpha;
    if (field_0x8c < 30) {
        alpha = (f32)(field_0x8c * field_0x8c) / 900.0f;
    } else {
        s32 t = field_0x8c - 30;
        alpha = 1.0f - (f32)(t * t) / 900.0f;
    }
    s32 frame = field_0x88;
    s32 step = frame < 5 ? frame : 10 - frame;
    f32 y = 5.0f * (f32)(step * step) / 25.0f;
    if (field_0x80 == 0) {
        field_0x50[0]->fn_80402E08(0.0f, y);
        field_0x50[1]->fn_80402E08(0.0f, 0.0f);
        field_0x58->fn_80402E08(0.0f, y);
    } else {
        field_0x50[0]->fn_80402E08(0.0f, 0.0f);
        field_0x50[1]->fn_80402E08(0.0f, y);
        field_0x58->fn_80402E08(field_0x50[1]->field_0x8 - field_0x50[0]->field_0x8, y);
    }
    mScreen->search('Paccur01')->setAlpha(field_0xa2 * alpha);
    mScreen->search('Paccur02')->setAlpha(field_0xa2 * alpha);
}

void GameMode_804B0688::fn_803DD3DC() {}

void GameMode_804B0688::fn_803DD3E0() {
    u8 entry = field_0x9e;
    f32 alpha;
    if (entry == 3 || mState != 1) {
        if (field_0x8c < 30) {
            alpha = (f32)(field_0x8c * field_0x8c) / 900.0f;
        } else {
            s32 t = field_0x8c - 30;
            alpha = 1.0f - (f32)(t * t) / 900.0f;
        }
    } else {
        if (field_0x8c < 60) {
            alpha = (f32)(field_0x8c * field_0x8c) / 3600.0f;
        } else {
            s32 t = field_0x8c - 60;
            alpha = 1.0f - (f32)(t * t) / 3600.0f;
        }
    }
    s32 frame = field_0x88;
    s32 step = frame < 5 ? frame : 10 - frame;
    f32 y = 5.0f * (f32)(step * step) / 25.0f;
    switch (entry) {
    case 0:
        field_0x34[0]->fn_80402E08(0.0f, y);
        field_0x34[1]->fn_80402E08(0.0f, 0.0f);
        field_0x30->fn_80402E08(0.0f, 0.0f);
        field_0x44[0]->fn_80402E08(0.0f, y);
        field_0x44[1]->fn_80402E08(0.0f, 0.0f);
        fn_803DAC58(field_0x9e, alpha);
        field_0x40->mPane->hide();
        break;
    case 1:
        field_0x34[0]->fn_80402E08(0.0f, 0.0f);
        field_0x34[1]->fn_80402E08(0.0f, y);
        field_0x30->fn_80402E08(0.0f, 0.0f);
        field_0x44[0]->fn_80402E08(0.0f, 0.0f);
        field_0x44[1]->fn_80402E08(0.0f, y);
        fn_803DAC58(field_0x9e, alpha);
        field_0x40->mPane->hide();
        break;
    case 2:
        field_0x34[0]->fn_80402E08(0.0f, 0.0f);
        field_0x34[1]->fn_80402E08(0.0f, 0.0f);
        field_0x30->fn_80402E08(0.0f, 0.0f);
        field_0x44[0]->fn_80402E08(0.0f, 0.0f);
        field_0x44[1]->fn_80402E08(0.0f, 0.0f);
        fn_803DAC58(field_0x9e, alpha);
        field_0x40->mPane->hide();
        break;
    default:
        field_0x34[0]->fn_80402E08(0.0f, 0.0f);
        field_0x34[1]->fn_80402E08(0.0f, 0.0f);
        field_0x44[0]->fn_80402E08(0.0f, 0.0f);
        field_0x44[1]->fn_80402E08(0.0f, 0.0f);
        field_0x30->fn_80402E08(0.0f, y);
        field_0x40->fn_80402E08(0.0f, y);
        fn_803DAC58(field_0x9e, alpha);
        field_0x40->mPane->show();
        break;
    }
}

void GameMode_804B0688::fn_803DD760() {}

// State 7: fades to black, waits for the sound to stop and the countdown
// field_0x98, then copies the save region to 0x80700000 (it survives the
// reset, see main) and reboots into "/DOL/framework.dol".
// FIXME: 93.9. The save-region copy loop: the original re-adds the index to
// lbl_80529DEC's address for every eight bytes (add r4, r5, r12), ours steps a
// source pointer (one instruction fewer); the same loop is unsolved in main
// (tu_801344C0) and cls_804A8578::fn_802B1DE8.
void GameMode_804B0688::fn_803DD764() {
    field_0x7c += 10.0f;
    if (field_0x7c >= 255.0f) {
        field_0x7c = 255.0f;
        if (lbl_8053AF80->fn_80457F80(JAISoundID(11), -2)) {
            if (--field_0x98 <= 0) {
                DVDFileInfo info;
                if (DVDOpen("/DOL/framework.dol", &info) == TRUE) {
                    GameState_804B0FE0* state = &lbl_80529DEC;
                    for (int i = 0; i < 0x20; i++) {
                        ((u8*)0x80700000)[i] = lbl_80529DEC.mSaveRegion.bytes[i];
                    }
                    OSSetSaveRegion((void*)0x80700000, (void*)0x8070001F);
                    __PADDisableRecalibration(TRUE);
                    lbl_8053ACA8->fn_802B1558();
                    OSSetBootDol(info.startAddr);
                    OSResetSystem(0, 0, FALSE);
                }
            }
        }
    }
}

void GameMode_804B0688::fn_803DD8E8() {
    field_0x78 += 10.0f;
    if (field_0x78 >= 255.0f) {
        field_0x78 = 255.0f;
        field_0x7c = 0.0f;
        if (++field_0x9a > 100) {
            mState = 7;
        }
    }
}

void GameMode_804B0688::fn_803DD93C() {
    field_0x7c += 10.0f;
    if (field_0x7c >= 255.0f) {
        field_0x7c = 255.0f;
        mState = 6;
    }
}

// State 1: the entry menu. A/Start picks the entry (3 leaves the screen),
// B returns, up / down move the cursor (entry 2 is skipped).
void GameMode_804B0688::fn_803DEC44() {
    if (field_0x88 != 0) {
        field_0x88++;
        if (field_0x88 > 10) {
            field_0x88 = 0;
        }
    }
    if (lbl_8053AD30->fn_80309C4C(0x1100, 1)) {
        if (field_0x9e == 3) {
            field_0x9f = field_0x9e;
            field_0x68 = 5;
        } else {
            field_0x9c = 0;
            for (s32 i = 0; i < 2; i++) {
                field_0x34[i]->fn_80402E08(0.0f, 0.0f);
                field_0x34[i]->field_0x20 = 0;
                field_0x34[i]->field_0x22 = 0;
                field_0x44[i]->fn_80402E08(0.0f, 0.0f);
                field_0x44[i]->field_0x20 = 0;
                field_0x44[i]->field_0x22 = 0;
            }
            field_0x30->fn_80402E08(0.0f, 0.0f);
            field_0x30->field_0x22 = 0;
            field_0x40->mPane->hide();
            for (s32 i = 0; i < 7; i++) {
                field_0x14[i]->mPane->show();
                field_0x14[i]->fn_80402E08(600.0f, 0.0f);
                field_0x14[i]->field_0x20 = 0;
            }
            for (s32 i = 0; i < 2; i++) {
                field_0x50[i]->mPane->show();
                field_0x50[i]->fn_80402E08(0.0f, 150.0f);
                field_0x50[i]->field_0x22 = 0;
            }
            switch (field_0x9e) {
            case 0:
                mScreen->search('Wgbco01')->hide();
                mScreen->search('Wnop01')->hide();
                mScreen->search('Wnop02')->hide();
                mScreen->search('Wgbco02')->show();
                break;
            case 1:
                mScreen->search('Wgbco01')->show();
                mScreen->search('Wnop01')->show();
                mScreen->search('Wnop02')->show();
                mScreen->search('Wgbco02')->hide();
                break;
            default:
                mScreen->search('Wgbco01')->show();
                mScreen->search('Wnop01')->show();
                mScreen->search('Wnop02')->show();
                mScreen->search('Wgbco02')->hide();
                break;
            }
            fn_803832BC(static_cast<J2DTextBox*>(mScreen->search('Tnop01'))->getStringPtr(),
                        lbl_8047788C[field_0x9e], 0, NULL, NULL);
            fn_803832BC(static_cast<J2DTextBox*>(mScreen->search('Tnop02'))->getStringPtr(),
                        lbl_8047788C[field_0x9e], 0, NULL, NULL);
            fn_803832BC(static_cast<J2DTextBox*>(mScreen->search('Texp01'))->getStringPtr(),
                        lbl_80477880[field_0x9e], 0, NULL, NULL);
            field_0x44[field_0x9e]->mPane->hide();
            mState = 2;
            field_0x9f = field_0x9e;
            field_0x90 = 1;
        }
        lbl_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1)) {
        field_0x68 = 1;
        lbl_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, 1)) {
        field_0x9f = field_0x9e;
        if (field_0x9e < 3) {
            field_0x9e++;
            if (field_0x9e == 2) {
                field_0x9e = 3;
            }
        } else {
            field_0x9e = 0;
        }
        field_0x88 = 1;
        if (field_0x9e == 3) {
            field_0x8c = 30;
        } else {
            field_0x8c = 60;
        }
        field_0x90 = 1;
        lbl_8053AF80->fn_80458880(0x26, -1);
    } else if (lbl_8053AD30->fn_80309A9C(8, 1)) {
        field_0x9f = field_0x9e;
        if (field_0x9e != 0) {
            field_0x9e--;
            if (field_0x9e == 2) {
                field_0x9e = 1;
            }
        } else {
            field_0x9e = 3;
        }
        field_0x88 = 1;
        if (field_0x9e == 3) {
            field_0x8c = 30;
        } else {
            field_0x8c = 60;
        }
        field_0x90 = 1;
        lbl_8053AF80->fn_80458880(0x26, -1);
    }
    field_0x8c++;
    if (field_0x9e == 3) {
        if (field_0x8c >= 60) {
            field_0x8c = 0;
        }
    } else {
        if (field_0x8c >= 120) {
            field_0x8c = 0;
        }
    }
}

// State 0: slides the entry panes in and fades the screen's panes up, then
// goes to state 1 once the three slides are done.
// FIXME: 99.25. Registers only: in the three alpha loops the original keeps
// the counter in r22 and the tag pointer in r21, ours the other way round.
void GameMode_804B0688::fn_803DF2B4() {
    u8 slid = false;
    u8 titleSlid = false;
    u8 windowSlid = false;
    field_0x9c++;
    for (s32 i = 0; i < 2; i++) {
        if (field_0x9c > i * 3) {
            slid = field_0x34[i]->fn_80402B10(20, 0, 600.0f, 0.0f);
            field_0x44[i]->fn_80402B10(20, 0, 600.0f, 0.0f);
            s32 t = field_0x9c - i * 3;
            if (t > 20) {
                t = 20;
            }
            f32 alpha = (f32)(t * t) / 400.0f;
            switch (i) {
            case 0:
                for (s32 j = 0; j < 17; j++) {
                    mScreen->search(lbl_80477688[j])->setAlpha(alpha * field_0xdb[j]);
                }
                break;
            case 1:
                for (s32 j = 0; j < 20; j++) {
                    mScreen->search(lbl_80477710[j])->setAlpha(alpha * field_0xec[j]);
                }
                break;
            }
        }
    }
    if (field_0x9c > 15) {
        titleSlid = field_0x30->fn_80402940(15, 0, 150.0f, 0.0f);
        s32 t = field_0x9c - 15;
        if (t > 15) {
            t = 15;
        }
        f32 alpha = (f32)(t * t) / 225.0f;
        for (s32 j = 0; j < 6; j++) {
            mScreen->search(lbl_80477658[j])->setAlpha(alpha * field_0xd5[j]);
        }
    }
    if (field_0x9c > 20) {
        windowSlid = field_0x5c->fn_80402940(25, 0, -200.0f, 0.0f);
        s32 t = field_0x9c - 20;
        if (t > 25) {
            t = 25;
        }
        lbl_8053AED8->fn_80440E0C((f32)(t * t) / 625.0f);
    }
    if (slid && titleSlid && windowSlid) {
        field_0x8c = 0;
        mState = 1;
        field_0x90 = 1;
    }
}

void GameMode_804B0688::fn_803DF624() {
    (lbl_8053ADD0->*lbl_804B0628[mState])();
    lbl_8053AB10->fn_8013B490();
    mMovieScreen->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
    if (mMovieState == 2) {
        mMovie->draw_80427D2C(50, 159, -356, -283);
    }
    lbl_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
    if (mState == 5) {
        field_0x10->setAlpha(field_0x7c);
        field_0x10->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    } else if (mState == 6) {
        field_0x10->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    } else if (mState == 7) {
        field_0x10->setAlpha(field_0x7c);
        field_0x10->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    }
    if (field_0x94 != 0) {
        if (lbl_80529DEC.field_0x1a0 > 0) {
            lbl_80529DEC.field_0x1a0--;
        } else {
            lbl_80529DEC.field_0x1a0 = 0;
        }
        field_0x94 = lbl_80529DEC.field_0x1a0;
    }
}

void GameMode_804B0688::fn_803DF810() {
    if (field_0x94 == 0 && mState != 7) {
        lbl_8053AF80->fn_80458FF0(0x100001A, false, false);
        lbl_8053AF80->fn_80458408(0x100001A, false);
        lbl_80529DEC.field_0x248 = 0;
    }
    (lbl_8053ADD0->*lbl_804B0568[mState])();
    fn_803DCCA4();
    if (mMovieState == 1) {
        if (mMovie->start_80427DB4(field_0x9e)) {
            field_0xa1 = 0;
            mMovieState = 2;
        } else {
            mMovie->quit_80427D94();
        }
    } else if (mMovieState == 2) {
        BOOL updated = mMovie->update_80427D60();
        if (IsMoviePlaying_804286A0() && field_0xa1 == 0) {
            mMovie->mSetVolume(0.0f);
            field_0xa1 = 1;
        }
        if (field_0x68 != 2 || lbl_8053ACA8->fn_802B1DCC()) {
            mMovie->quit_80427D94();
            mMovieState = 0;
        } else if (mMovie->mGetRestFrame() == 0 || updated == FALSE) {
            mMovie->quit_80427D94();
            if (mMovie->start_80427DB4(field_0x9e)) {
                field_0xa1 = 0;
            } else {
                mMovie->quit_80427D94();
                mMovieState = 1;
            }
        }
    } else if (mMovieState == 3) {
        mMovie->quit_80427D94();
        mMovieState = 0;
    }
}

void GameMode_804B0688::fn_803DF9E8() {
    field_0x88 = 0;
    field_0x8c = 0;
    field_0x90 = 0;
    field_0x9c = 0;
    field_0x7c = 0.0f;
    field_0x78 = 0.0f;
    field_0x9a = 0;
    field_0x98 = 60;
    field_0xa1 = 0;
    for (s32 i = 0; i < 2; i++) {
        field_0x34[i]->mPane->show();
        field_0x34[i]->fn_80402E08(600.0f, 0.0f);
        field_0x34[i]->field_0x20 = 0;
        field_0x44[i]->mPane->show();
        field_0x44[i]->fn_80402E08(600.0f, 0.0f);
        field_0x44[i]->field_0x20 = 0;
    }
    field_0x30->mPane->show();
    field_0x30->fn_80402E08(0.0f, 150.0f);
    field_0x30->field_0x22 = 0;
    field_0x5c->fn_80402E08(0.0f, -200.0f);
    field_0x5c->field_0x22 = 0;
    field_0x40->mPane->hide();
    for (s32 i = 0; i < 7; i++) {
        field_0x14[i]->mPane->hide();
    }
    field_0x50[0]->mPane->hide();
    field_0x50[1]->mPane->hide();
    field_0x58->mPane->hide();
    mMovieScreen->search('Pim01')->setAlpha(0);
    mMovieScreen->search('Pim02')->setAlpha(0);
    mMovieScreen->search('Pim11')->setAlpha(0);
    mMovieScreen->search('Pim12')->setAlpha(0);
    mState = 0;
    field_0x68 = 2;
}

GameMode_804B0688::~GameMode_804B0688() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    if (mMovieState == 2) {
        mMovie->quit_80427D94();
        mMovieState = 0;
    }
    delete field_0x60;
    field_0x60 = NULL;
    delete mMovie;
    mMovie = NULL;
    delete mScreen;
    mScreen = NULL;
    delete mMovieScreen;
    mMovieScreen = NULL;
    for (s32 i = 0; i < 7; i++) {
        delete field_0x14[i];
        field_0x14[i] = NULL;
    }
    delete field_0x30;
    field_0x30 = NULL;
    for (s32 i = 0; i < 2; i++) {
        delete field_0x34[i];
        field_0x34[i] = NULL;
    }
    delete field_0x40;
    field_0x40 = NULL;
    for (s32 i = 0; i < 2; i++) {
        delete field_0x44[i];
        field_0x44[i] = NULL;
    }
    for (s32 i = 0; i < 2; i++) {
        delete field_0x50[i];
        field_0x50[i] = NULL;
    }
    delete field_0x58;
    field_0x58 = NULL;
    delete field_0x10;
    field_0x10 = NULL;
    lbl_80529DEC.field_0x2c->unmount();
    lbl_8053ADD0 = NULL;
}
