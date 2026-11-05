#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/act/tu_8044B80C.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <stdio.h>

// Message ids of the staff-roll text pages.
static const u32 lbl_80490F48[8] = {0x85B, 0x85C, 0x85D, 0x85E, 0x85F, 0x860, 0x862, 0x863};

bool cls_804B1D10::fn_8044B80C(u8 idx) {
    if (idx >= 8) {
        return false;
    }
    if (mState == 3) {
        if (mPage == idx) {
            return true;
        }
        mPage = idx;
        mState = 1;
        JKRHeap* saved = JKRHeap::sCurrentHeap;
        gGameRoot_8053AAF8->field_0x4->field_0x30->becomeCurrentHeap();
        delete mScreen;
        mScreen = NULL;
        gGameRoot_8053AAF8->field_0x4->field_0x30->freeAll();
        saved->becomeCurrentHeap();
    } else if (mState == 0) {
        mPage = idx;
        mState = 1;
    }
    return false;
}

void cls_804B1D10::fn_8044B8EC() {
    if (mDraw && mState == 3 && mScreen != NULL) {
        gGameRoot2_8053AB10->fn_8013B490();
        mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
        mDraw = false;
    }
}

void cls_804B1D10::fn_8044B960() {
    if (mState == 1 || mState == 2) {
        char path[0x1F];
        char name[0x11];
        JKRHeap* saved = JKRHeap::sCurrentHeap;
        gGameRoot_8053AAF8->field_0x4->field_0x30->becomeCurrentHeap();
        if (mState == 1) {
            snprintf(path, 0x1F, "/GC4Sword_usa/staff/staff%d.arc", mPage + 1);
            mArchive = fn_80401E10(path, 0, gGameRoot_8053AAF8->field_0x4->field_0x30);
            mState = 2;
        }
        if (mArchive != NULL && mArchive->field_0x4 == 1) {
            mScreen = new J2DScreen();
            snprintf(name, 0x11, "staffroll_p%d.blo", mPage + 1);
            // field_0x14 is the loaded archive (fn_80401E10's object).
            mScreen->setPriority(name, 0x1100000, (JKRArchive*)mArchive->field_0x14);
            JUTResFont* font = lbl_8053AD78->mFont;
            ((J2DTextBox*)mScreen->search('Tstf01'))->setFont(font);
            font = lbl_8053AD78->mFont;
            ((J2DTextBox*)mScreen->search('Tstf02'))->setFont(font);
            getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tstf01'))->getStringPtr(), lbl_80490F48[mPage], 0, NULL, NULL);
            getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tstf02'))->getStringPtr(), lbl_80490F48[mPage], 0, NULL, NULL);
            if (mPage == 5) {
                JUTResFont* font2 = lbl_8053AD78->mFont;
                ((J2DTextBox*)mScreen->search('Tstf03'))->setFont(font2);
                font2 = lbl_8053AD78->mFont;
                ((J2DTextBox*)mScreen->search('Tstf04'))->setFont(font2);
                getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tstf03'))->getStringPtr(), 0x861, 0, NULL, NULL);
                getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tstf04'))->getStringPtr(), 0x861, 0, NULL, NULL);
            }
            delete mArchive;
            mState = 3;
        }
        saved->becomeCurrentHeap();
    }
}

cls_804B1D10::~cls_804B1D10() {
    if (field_0x4 != NULL) {
        JKRHeap* saved = JKRHeap::sCurrentHeap;
        gGameRoot_8053AAF8->field_0x4->field_0x30->becomeCurrentHeap();
        delete field_0x4;
        field_0x4 = NULL;
        gGameRoot_8053AAF8->field_0x4->field_0x30->freeAll();
        saved->becomeCurrentHeap();
    }
    if (mScreen != NULL) {
        JKRHeap* saved = JKRHeap::sCurrentHeap;
        gGameRoot_8053AAF8->field_0x4->field_0x30->becomeCurrentHeap();
        delete mScreen;
        mScreen = NULL;
        gGameRoot_8053AAF8->field_0x4->field_0x30->freeAll();
        saved->becomeCurrentHeap();
    }
    lbl_8053AF30 = NULL;
}

cls_804B1D10::cls_804B1D10() {
    lbl_8053AF30 = this;
    field_0x4 = NULL;
    mScreen = NULL;
    mArchive = NULL;
    field_0x10 = 0;
    mDraw = false;
    mPage = 0xFF;
    mState = 0;
}
