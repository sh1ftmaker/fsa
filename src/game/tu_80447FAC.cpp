#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804B1C58.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"

// Per player, 0x70 bytes of J2DPane::search tags (as u64): the digit pictures are at 0x48 / 0x50 (tens),
// 0x58 / 0x60 (ones), the frame panes at 0x30 and 0x68.
extern const u64 lbl_804906A8[4][14];
// The digit textures (kk_rupyNum_00.bti ...).
extern const char* lbl_804B1C30[10];

// FIXME(shotgun): 99.77 %, reloc-name
cls_804B1C58::cls_804B1C58() {
    lbl_8053AF10 = this;
    mScreen = new J2DScreen;
    mScreen->setPriority("mg_shooting.blo", 0x1100000, lbl_80529DEC.field_0x10);
    for (s32 i = 0; i < 4; i++) {
        field_0x8[i] = new PaneCtrl_804AF86C;
        field_0x8[i]->setPane_80402F04(mScreen->search(lbl_804906A8[i][6]));
        field_0x18[i] = new PaneCtrl_804AF86C;
        field_0x18[i]->setPane_80402F04(mScreen->search(lbl_804906A8[i][13]));
    }
    field_0x28[0] = 0;
    field_0x28[1] = 0;
    field_0x28[2] = 0;
    field_0x28[3] = 0;
    field_0x2c = 0;
}

// FIXME(shotgun): 99.84 %, reloc-name
cls_804B1C58::~cls_804B1C58() {
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x8[i];
        field_0x8[i] = NULL;
        delete field_0x18[i];
        field_0x18[i] = NULL;
    }
    lbl_8053AF10 = NULL;
}

// FIXME(shotgun): 99.50 %, regalloc-only
void cls_804B1C58::fn_80448334() {
    for (s32 i = 0; i < 4; i++) {
        field_0x2c &= ~(u8)(1 << i);
        const ResTIMG* zero = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C30[0]));
        static_cast<J2DPicture*>(mScreen->search(lbl_804906A8[i][9]))->changeTexture(zero, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_804906A8[i][10]))->changeTexture(zero, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_804906A8[i][11]))->changeTexture(zero, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_804906A8[i][12]))->changeTexture(zero, 0);
        mScreen->search(lbl_804906A8[i][6])->hide();
        mScreen->search(lbl_804906A8[i][13])->hide();
    }
}

void cls_804B1C58::fn_804482EC() {
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

// FIXME(shotgun): 99.88 %, regalloc-only
void cls_804B1C58::fn_80447FAC() {
    for (s32 i = 0; i < 4; i++) {
        if (i < 2) {
            field_0x8[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
            field_0x18[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
        } else {
            field_0x8[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
            field_0x18[i]->moveFromCentre_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
        }
        if (field_0x2c & (u8)(1 << i)) {
            if (!mScreen->search(lbl_804906A8[i][6])->isVisible()) {
                mScreen->search(lbl_804906A8[i][6])->show();
                mScreen->search(lbl_804906A8[i][13])->show();
            }
        } else if (mScreen->search(lbl_804906A8[i][6])->isVisible()) {
            mScreen->search(lbl_804906A8[i][6])->hide();
            mScreen->search(lbl_804906A8[i][13])->hide();
        }
        if (mScreen->search(lbl_804906A8[i][6])->isVisible()) {
            if (field_0x28[i] >= 100) {
                field_0x28[i] = 99;
            }
            s32 tens = field_0x28[i] / 10;
            s32 ones = field_0x28[i] % 10;
            const ResTIMG* tex = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C30[tens]));
            static_cast<J2DPicture*>(mScreen->search(lbl_804906A8[i][9]))->changeTexture(tex, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_804906A8[i][10]))->changeTexture(tex, 0);
            tex = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C30[ones]));
            static_cast<J2DPicture*>(mScreen->search(lbl_804906A8[i][11]))->changeTexture(tex, 0);
            static_cast<J2DPicture*>(mScreen->search(lbl_804906A8[i][12]))->changeTexture(tex, 0);
        }
    }
    field_0x2c = 0;
}
