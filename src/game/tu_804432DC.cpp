#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/tu_804432DC.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"

// Per-kind scales and the x and y offsets of the panes (kinds 0-7), in
// address order.
static const f32 lbl_8048FB88[7] = {
    1034.0f, 1277.0f, 1277.0f, 1034.0f, 1216.0f, 1277.0f, 1034.0f,
};

static const s16 lbl_8048FBA4[8] = {-138, -346, 22, -103, 303, 365, 56, 0};

static const s16 lbl_8048FBB4[8] = {29, -266, 235, -137, 10, -268, -130, 0};

// The original's .data here is zero, filled by __sinit (fn_80443894, not
// written yet, which copies the table at lbl_804B1AA0 into it); so .data
// doesn't match yet.
static void (cls_804B1AE8::*lbl_804B1AC4[3])() = {
    &cls_804B1AE8::fn_804435B8,
    &cls_804B1AE8::fn_804434B8,
    &cls_804B1AE8::fn_80443494,
};

cls_804B1AE8::cls_804B1AE8() {
    lbl_8053AEF0 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("map_ed.blo", 0x1100000, lbl_80529DEC.field_0x10);
    mPtestmap = new PaneCtrl_804AF86C();
    mPtestmap->setPane_80402F04(mScreen->search(0x50746573746d6170ull /* "Ptestmap" */));
    mPmapten = new PaneCtrl_804AF86C();
    mPmapten->setPane_80402F04(mScreen->search(0x506d617074656eull /* "Pmapten" */));
    mKind = 0;
    mTimer = 0;
    mHoldTimer = 0;
    mState = 3;
}

cls_804B1AE8::~cls_804B1AE8() {
    delete mScreen;
    mScreen = NULL;
    delete mPtestmap;
    mPtestmap = NULL;
    delete mPmapten;
    mPmapten = NULL;
    lbl_8053AEF0 = NULL;
}

// Empty; tu_8043A0E8 calls it on lbl_8053AEF0.
void cls_804B1AE8::fn_80443688() {}

// FIXME: 81.5 %. The target tests the state with a branchless "!= 3"
// (subfic / subi / or / srwi.) and keeps it in r4; ours compares it with cmpwi.
void cls_804B1AE8::fn_80443638() {
    if (mState != 3) {
        (lbl_8053AEF0->*lbl_804B1AC4[mState])();
    }
}

void cls_804B1AE8::fn_804435E4() {
    if (mState != 3) {
        gGameRoot2_8053AB10->fn_8013B490();
        mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}

void cls_804B1AE8::fn_804435B8() {
    if (mTimer > 0) {
        mTimer--;
        return;
    }
    mTimer = 0;
    mState = 1;
}

// FIXME: 96.6 %. Only the scale's division: the original loads the pane's
// field_0x18 before the table value (lfs f2 / lfsx f0); ours loads them the
// other way round.
void cls_804B1AE8::fn_804434B8() {
    mTimer++;
    mPtestmap->fn_80402B10(30, 0, 0.0f, (f32)lbl_8048FBA4[mKind]);
    mPtestmap->fn_80402940(30, 0, 0.0f, (f32)lbl_8048FBB4[mKind]);
    mPtestmap->fn_8040271C(30, 1.0f, lbl_8048FB88[mKind] / mPtestmap->field_0x18);
    if (mTimer >= 30) {
        mState = 2;
    }
}

void cls_804B1AE8::fn_80443494() {
    if (mHoldTimer > 0) {
        mHoldTimer--;
        return;
    }
    mState = 3;
}

// Starts the window animation of kind 0-7 with timer a and hold time b (only
// while idle). Returns false.
// FIXME: 98.0 %. Scheduling only: the target computes the table addresses
// (addi r5 / r4, lbl_8048FBA4 / lbl_8048FBB4) before the first sth, and its
// int-to-float constant is the named lbl_80543D68 where ours is an anonymous
// literal.
bool cls_804B1AE8::fn_804432DC(u8 kind, s16 a, s16 b) {
    if (kind < 8 && mState == 3) {
        mKind = kind;
        mTimer = a;
        mHoldTimer = b;
        if (kind == 0) {
            mPtestmap->field_0x20 = 0;
            mPtestmap->field_0x22 = 0;
            mPtestmap->field_0x24 = 0;
            mPtestmap->moveFromCentre_80402E08(0.0f, 0.0f);
            mPtestmap->fn_80402CE0(1.0f, 1.0f);
            mPtestmap->mPane->show();
            mPmapten->mPane->hide();
            mState = 0;
        } else if (kind == 7) {
            mPtestmap->mPane->hide();
            mPmapten->mPane->show();
            mState = 2;
        } else {
            mPtestmap->field_0x20 = 0;
            mPtestmap->field_0x22 = 0;
            mPtestmap->field_0x24 = 0;
            mPtestmap->moveFromCentre_80402E08((f32)lbl_8048FBA4[mKind], (f32)lbl_8048FBB4[mKind]);
            f32 scale = lbl_8048FB88[mKind] / mPtestmap->field_0x18;
            mPtestmap->fn_80402CE0(scale, scale);
            mPtestmap->mPane->show();
            mPmapten->mPane->hide();
            mState = 2;
        }
    }
    return false;
}
