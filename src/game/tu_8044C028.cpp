#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/act/tu_8044C028.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"

cls_804B1D20::cls_804B1D20() {
    lbl_8053AF38 = this;
    mScreen[0] = new J2DScreen();
    mScreen[0]->setPriority("op_prologue.blo", 0x1100000, lbl_80529DEC.field_0x10);
    mScreen[1] = new J2DScreen();
    mScreen[1]->setPriority("op_prologue_m.blo", 0x1100000, lbl_80529DEC.field_0x10);
    JUTResFont* font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreen[1]->search('Tp01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreen[1]->search('Tp02'))->setFont(font);
    getMessageText_803832BC(((J2DTextBox*)mScreen[1]->search('Tp01'))->getStringPtr(), 0x14A, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreen[1]->search('Tp02'))->getStringPtr(), 0x14A, 0, NULL, NULL);
    field_0xc = 308.0f + mScreen[1]->search('Tp01')->getBounds()->i.y;
    field_0x10 = 308.0f + mScreen[1]->search('Tp02')->getBounds()->i.y;
    f32 height = mScreen[1]->search('Tp01')->getHeight();
    J2DPane* pane = mScreen[1]->search('Tp01');
    mScreen[1]->search('Tp01')->resize(pane->getWidth(), 6.0f * height);
    pane = mScreen[1]->search('Tp02');
    mScreen[1]->search('Tp02')->resize(pane->getWidth(), 6.0f * height);
    ((J2DPicture*)mScreen[1]->search(0x50626c6b3031ull /* "Pblk01" */))
        ->setBlackWhite(JUtility::TColor(0), JUtility::TColor(0));
    field_0x18 = false;
    field_0x14 = 0.0f;
}

cls_804B1D20::~cls_804B1D20() {
    for (s32 i = 0; i < 2; i++) {
        delete mScreen[i];
        mScreen[i] = NULL;
    }
    lbl_8053AF38 = NULL;
}

void cls_804B1D20::fn_8044C14C() {
    if (field_0x18) {
        gGameRoot2_8053AB10->fn_8013B490();
        mScreen[0]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
        mScreen[1]->setScissor(true);
        mScreen[1]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
        field_0x18 = false;
    }
}

void cls_804B1D20::fn_8044C038() {
    if (field_0x18) {
        f32 x = mScreen[1]->search('Tp01')->getBounds()->i.x;
        mScreen[1]->search('Tp01')->move(x, field_0xc - field_0x14);
        x = mScreen[1]->search('Tp02')->getBounds()->i.x;
        mScreen[1]->search('Tp02')->move(x, field_0x10 - field_0x14);
    }
}

void cls_804B1D20::fn_8044C028(f32 offset) {
    field_0x18 = true;
    field_0x14 = offset;
}
