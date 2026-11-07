#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80379F84.h"
#include "game/act/tu_80440BF4.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"

void cls_804B03D8::fn_803D8718() {
    switch (field_0x76) {
    case 6:
        if (field_0x77 >= 245) {
            field_0x77 = 255;
            field_0x76 = 5;
        } else {
            field_0x77 += 10;
        }
        break;
    case 5:
        if (field_0x77 <= 10) {
            field_0x77 = 0;
            lbl_8053AD68->field_0x1d = 1;
            field_0x76 = 2;
        } else {
            field_0x77 -= 10;
        }
        break;
    case 2:
        fn_803D75B0();
        break;
    }
}

// FIXME(shotgun): 99.40 %, reloc-name
void cls_804B03D8::fn_803D8670() {
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x8->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x1c->setAlpha(field_0x77);
    field_0x1c->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}

// FIXME(shotgun): 95.73 %, missing/extra (one more saved register: the bounce step)
void cls_804B03D8::fn_803D8364() {
    f32 bounce;
    f32 alpha;
    if (field_0x7f == 1 || (u8)(field_0x7f - 2) <= 1 || field_0x7f == 4) {
        bounce = 0.0f;
        if (field_0x6e > 0) {
            if (field_0x6e == 1) {
                field_0x6c = 30;
            }
            s32 step = field_0x6e < 5 ? field_0x6e : 10 - field_0x6e;
            bounce = 5.0f * (f32)(step * step) / 25.0f;
            if (field_0x6e < 10) {
                field_0x6e++;
            } else {
                field_0x6e = 0;
            }
        }
        field_0x6c++;
        if (field_0x6c >= 60) {
            field_0x6c = 0;
        }
        if (field_0x7f == 2 || field_0x7f == 3 || field_0x7f == 4) {
            field_0x20->setAlpha(field_0x81);
            field_0x20->draw(-1.0f, -1.0f, 611.0f, 451.0f, false, false, false);
            lbl_8053AEC0->fn_804316EC();
            alpha = 156.0f;
        } else {
            if (field_0x6c < 30) {
                alpha = 156.0f * ((f32)(field_0x6c * field_0x6c) / 900.0f);
            } else {
                s32 d = field_0x6c - 30;
                alpha = 156.0f * (1.0f - (f32)(d * d) / 900.0f);
            }
        }
        if (field_0x79 == 0) {
            field_0x2c->moveFromCentre_80402E08(0.0f, bounce);
            field_0x24->moveFromCentre_80402E08(0.0f, bounce);
            field_0x28->moveFromCentre_80402E08(0.0f, 0.0f);
        } else {
            field_0x2c->moveFromCentre_80402E08(field_0x28->field_0x8 - field_0x24->field_0x8, bounce);
            field_0x24->moveFromCentre_80402E08(0.0f, 0.0f);
            field_0x28->moveFromCentre_80402E08(0.0f, bounce);
            if (0.0f == bounce && field_0x79 != field_0x78) {
                field_0x79 = field_0x78;
            }
        }
        field_0x2c->mPane->setAlpha((s32)alpha);
        field_0x4->search('Paccur01')->setAlpha((s32)alpha);
        lbl_8053AED8->fn_80440EE8();
    }
}

// FIXME(shotgun): 99.50 %, reloc-name
void cls_804B03D8::fn_803D82EC() {
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x1c->setAlpha(field_0x77);
    field_0x1c->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}
