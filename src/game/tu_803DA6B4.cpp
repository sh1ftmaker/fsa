#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/tu_803DA6B4.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"

void cls_804B04F8::fn_803DA6B4() {
    static const f32 lbl_804774A0[5] = {0.0f, 60.0f, 47.0f, -42.0f, 40.0f};
    field_0x18 = 0.0f;
    if (field_0x20 != 0) {
        field_0x1c++;
        if (field_0x1c <= 26) {
            s32 d = field_0x1c < 13 ? 13 - field_0x1c : field_0x1c - 13;
            field_0x18 = 50.0f / 255.0f + (205.0f / 255.0f) * (f32)(d * d) / 169.0f;
        } else {
            field_0x18 = 1.0f;
            if (field_0x1c >= 46) {
                field_0x1c = 0;
            }
        }
        if (field_0x21 != 1) {
            mPane2->move(field_0x10, field_0x14 + lbl_804774A0[field_0x21]);
            mPane1->setAlpha(field_0x1e * field_0x18);
            mPane2->setAlpha(field_0x1f * field_0x18);
            lbl_8053AB10->fn_8013B490();
            mScreen->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
        }
        field_0x20 = 0;
    } else if (field_0x1c != 0) {
        field_0x1c = 0;
    }
}

cls_804B04F8::~cls_804B04F8() {
    delete mScreen;
    mScreen = NULL;
    lbl_8053ADC8 = NULL;
}

cls_804B04F8::cls_804B04F8() {
    lbl_8053ADC8 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("pressstart.blo", 0x1100000, lbl_80529DEC.field_0x20);
    mPane1 = mScreen->search(0x7072737374727431ull /* "prsstrt1" */);
    mPane2 = mScreen->search(0x7072737374727432ull /* "prsstrt2" */);
    field_0x10 = mPane2->getBounds()->i.x;
    field_0x14 = mPane2->getBounds()->i.y;
    field_0x1e = mPane1->getAlpha();
    field_0x1f = mPane2->getAlpha();
    field_0x1c = 0;
    field_0x20 = 0;
    field_0x18 = 0.0f;
    field_0x21 = 0;
}
