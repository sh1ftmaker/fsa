#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/tu_80440BF4.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JFramework/JFWDisplay.h"

cls_804AF860::~cls_804AF860() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    for (s32 i = 0; i < 6; i++) {
        delete field_0xc[i];
        field_0xc[i] = NULL;
    }
    delete field_0x24;
    field_0x24 = NULL;
    delete field_0x28;
    field_0x28 = NULL;
    delete field_0x2c;
    field_0x2c = NULL;
    delete field_0x30;
    field_0x30 = NULL;
    lbl_8053AD90 = NULL;
}

void cls_804AF860::fn_803A71F0() {
    JFWDisplay::getManager()->setClearColor(JUtility::TColor(0, 0, 0, 0));
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
}

s32 fn_803A65EC() {
    if (lbl_8053AD90 != NULL) {
        return lbl_8053AD90->field_0x80;
    }
    return 5;
}
