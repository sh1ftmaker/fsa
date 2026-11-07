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

// FIXME(shotgun): 99.86 %, reloc-name
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

// Rotates a pane of the screen about its centre.
static inline void rotateArrow(J2DScreen* screen, u64 tag, f32 angle) {
    f32 h = screen->search(tag)->getHeight();
    J2DPane* pane = screen->search(tag);
    f32 w = pane->getWidth();
    screen->search(tag)->rotate(0.5f * w, 0.5f * h, J2DROTATE_Z, angle);
}

// 0x803A60E4.
// FIXME(shotgun): 50.3 %, missing/extra + reloc-name
void cls_804AF860::fn_803A60E4() {
    if (field_0x6e == 0) {
        field_0x6c--;
        if (field_0x6c <= 0) {
            field_0x6c = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
            if (field_0x74 == -6) {
                field_0x74 = 6;
            } else {
                field_0x74 = -6;
            }
            rotateArrow(field_0x4, 'Par01', (f32)(field_0x70 + field_0x74));
            rotateArrow(field_0x4, 'Par02', (f32)(field_0x72 + field_0x74));
        }
        return;
    }
    if (field_0x6e == 1 || field_0x6e == -1) {
        field_0x74 = 0;
        rotateArrow(field_0x4, 'Par01', (f32)(field_0x70 + field_0x74));
        rotateArrow(field_0x4, 'Par02', (f32)(field_0x72 + field_0x74));
    }
    if (field_0x6e < 0) {
        s32 t;
        if (field_0x6e > -5) {
            t = -field_0x6e;
        } else {
            t = field_0x6e + 10;
            if (field_0x6e == -10) {
                field_0x6e = 0;
            }
        }
        field_0x24->moveFromCentre_80402E08(-5.0f * (f32)field_0x64 / 25.0f, field_0x60 * (f32)(t * t));
        if (field_0x6e != 0) {
            field_0x6e--;
        }
    } else if (field_0x6e > 0) {
        s32 t = field_0x6e;
        if (field_0x6e >= 5) {
            t = 10 - field_0x6e;
            if (field_0x6e == 10) {
                field_0x6e = 0;
            }
        }
        field_0x28->moveFromCentre_80402E08(5.0f * (f32)field_0x64 / 25.0f, field_0x60 * (f32)(t * t));
        if (field_0x6e != 0) {
            field_0x6e++;
        }
    }
}
