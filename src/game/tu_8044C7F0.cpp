#include "game/GameAudio.h"
#include "game/cls_804B1D58.h"
#include "game/GameGlobals.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"

cls_80453830::cls_80453830() {
    field_0x0 = -1;
    field_0x4 = -1;
    field_0x8 = -1;
    field_0xc = -1;
}

void cls_804B1D58::fn_80452308() {
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

void cls_804B1D58::fn_804522C8() {
    if (field_0xfa == 7 || field_0xfa == 8 || field_0xfa == 9) {
        lbl_8053AEC0->fn_804316EC();
    }
}

s32 fn_8044F664() {
    if (lbl_8053AF40 != NULL) {
        return lbl_8053AF40->field_0x184;
    }
    return 9;
}

u8 cls_804B1D58::fn_8044D290() {
    return field_0x140;
}

u8 cls_804B1D58::fn_8044CB00() {
    return field_0x140;
}
