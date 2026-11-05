#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/act/tu_80440BF4.h"
#include "game/cls_804B16A8.h"
#include "game/cls_804B20F8.h"
#include "game/tu_803F0A54.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DAnimation.h"

// Entered when the state changes (index: the new state).
static cls_804B0F10::StateFunc lbl_804B0BDC[13] = {
    &cls_804B0F10::fn_803F3324, &cls_804B0F10::fn_803F3310, &cls_804B0F10::fn_803F32E8,
    &cls_804B0F10::fn_803F2B78, &cls_804B0F10::fn_803F2B5C, &cls_804B0F10::fn_803F2B48,
    &cls_804B0F10::fn_803F2AEC, &cls_804B0F10::fn_803F267C, &cls_804B0F10::fn_803F2660,
    &cls_804B0F10::fn_803F2580, &cls_804B0F10::fn_803F1C70, &cls_804B0F10::fn_803F1C5C,
    &cls_804B0F10::fn_803F1C18,
};

// Run each frame by fn_803FC304 (index: the state).
static cls_804B0F10::StateFunc lbl_804B0D14[13] = {
    &cls_804B0F10::fn_803FB9E4, &cls_804B0F10::fn_803FB6F4, &cls_804B0F10::fn_803FAA88,
    &cls_804B0F10::fn_803F9EA8, &cls_804B0F10::fn_803F943C, &cls_804B0F10::fn_803F8AE4,
    &cls_804B0F10::fn_803F8FBC, &cls_804B0F10::fn_803F8354, &cls_804B0F10::fn_803F8050,
    &cls_804B0F10::fn_803F726C, &cls_804B0F10::fn_803F715C, &cls_804B0F10::fn_803F5A0C,
    &cls_804B0F10::fn_803F59A0,
};

// Run each frame by fn_803FC46C (index: the state).
static cls_804B0F10::StateFunc lbl_804B0E4C[13] = {
    &cls_804B0F10::fn_803F5410, &cls_804B0F10::fn_803F5410, &cls_804B0F10::fn_803F5410,
    &cls_804B0F10::fn_803F4120, &cls_804B0F10::fn_803F4120, &cls_804B0F10::fn_803F4120,
    &cls_804B0F10::fn_803F4120, &cls_804B0F10::fn_803F38A4, &cls_804B0F10::fn_803F38A4,
    &cls_804B0F10::fn_803F38A4, &cls_804B0F10::fn_803F35B0, &cls_804B0F10::fn_803F35B0,
    &cls_804B0F10::fn_803F358C,
};

void cls_804B0F10::fn_803F1C18() {
    field_0x58->fn_8043670C(1);
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 12;
}

void cls_804B0F10::fn_803F1C5C() {
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 11;
}

void cls_804B0F10::fn_803F2660() {
    field_0x240[0] = 0;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 8;
}

void cls_804B0F10::fn_803F2AEC() {
    fn_80431A9C(lbl_8053AEC0, 0xC4, field_0x1ac + 2, NULL);
    field_0x25c = 0;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 6;
}

void cls_804B0F10::fn_803F2B48() {
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 5;
}

void cls_804B0F10::fn_803F2B5C() {
    field_0x240[0] = 0;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 4;
}

void cls_804B0F10::fn_803F32E8() {
    field_0x210[0] = 0;
    field_0x240[0] = 0;
    field_0x1d0[0] = 5;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 2;
}

void cls_804B0F10::fn_803F3310() {
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 1;
}

void cls_804B0F10::fn_803F358C() {
    field_0x58->fn_80436668();
}

void cls_804B0F10::fn_803F59A0() {
    if (field_0x58->fn_804366B8() != 0) {
        if (field_0x25e == 0) {
            field_0x1cc = 7;
            field_0x25e = 1;
        }
        field_0x1a8 = 0;
        lbl_8053AF80->fn_80458880(11, -1);
    }
}

void cls_804B0F10::fn_803FC304() {
    field_0x1c0 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i <= field_0x1ac + 1) {
            if (lbl_8053AD30->fn_8030A308(fn_8030AAF4(i))) {
                lbl_8053AAF8->field_0x82 |= (u8)(1 << (i + 4));
                field_0x1c0++;
            } else {
                lbl_8053AAF8->field_0x82 &= ~(u8)(1 << (i + 4));
            }
        } else {
            lbl_8053AAF8->field_0x82 &= ~(u8)(1 << (i + 4));
        }
    }
    if (field_0x25e == 0 || field_0x1c4 == 4) {
        if (field_0x1c4 != 13) {
            (this->*lbl_804B0D14[field_0x1c4])();
        } else {
            (this->*lbl_804B0D14[field_0x1c8])();
        }
        for (s32 i = 0; i < 4; i++) {
            field_0x78[i]->fn_80456154();
        }
    }
}

void cls_804B0F10::fn_803FC46C() {
    lbl_8053AB10->fn_8013B490();
    field_0x14->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
    if (field_0x1c4 != 13) {
        (this->*lbl_804B0E4C[field_0x1c4])();
    } else {
        (this->*lbl_804B0E4C[field_0x1c8])();
    }
    if (field_0x25e != 0) {
        switch (field_0x25e) {
        case 1:
            if (field_0x25d >= 245) {
                field_0x25d = 255;
                field_0x25e = 2;
                (this->*lbl_804B0BDC[field_0x1cc])();
            } else {
                field_0x25d += 10;
            }
            break;
        case 2:
            if (field_0x25d <= 10) {
                field_0x25d = 0;
                field_0x25e = 0;
            } else {
                field_0x25d -= 10;
            }
            break;
        }
        lbl_8053AB10->fn_8013B490();
        field_0x5c->setAlpha(field_0x25d);
        field_0x5c->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    }
}

void cls_804B0F10::fn_803FC5E4(u8 flag) {
    fn_804310CC(lbl_8053AEC0);
    if (flag) {
        field_0x1c4 = 7;
        fn_803F267C();
    } else {
        if (lbl_8053AD30->fn_8030A308(fn_8030AAF4(0)) && lbl_8053AD30->fn_8030A308(fn_8030AAF4(1))) {
            field_0x1ac = 0;
            for (s32 i = 2; i < 4; i++) {
                if (!lbl_8053AD30->fn_8030A308(fn_8030AAF4(i))) {
                    break;
                }
                field_0x1ac++;
            }
        }
        field_0x1c4 = 0;
        field_0x110 = -150.0f;
        mScreen->search(0x506D626162623037ull /* "Pmbabb07" */)->move(field_0x108, field_0x10c + field_0x110);
        field_0x114 = -100.0f;
        field_0x74->fn_80402E08(0.0f, field_0x114);
        field_0x130 = -350.0f;
        mScreen->search(0x00506E6F70623038ull /* "Pnopb08" */)->move(field_0x118 + field_0x130, field_0x124);
        field_0x134 = -350.0f;
        mScreen->search(0x00506E6F70623132ull /* "Pnopb12" */)->move(field_0x11c + field_0x134, field_0x128);
        field_0x138 = -350.0f;
        mScreen->search(0x00506E6F70623136ull /* "Pnopb16" */)->move(field_0x120 + field_0x138, field_0x12c);
        field_0x144 = 400.0f;
        mScreen->search(0x004E646174613032ull /* "Ndata02" */)->move(field_0x13c + field_0x144, field_0x140);
        field_0x158 = 400.0f;
        mScreen->search(0x004E646174613035ull /* "Ndata05" */)->move(field_0x148 + field_0x158, field_0x14c);
        mScreen->search(0x004E646174613037ull /* "Ndata07" */)->move(field_0x150 + field_0x158, field_0x154);
        field_0x210[0] = 0;
        field_0x1d0[0] = 0;
        field_0x240[0] = 0;
        field_0x1c8 = field_0x1c4;
        field_0x1c4 = 0;
    }
    field_0x350 = 4;
}

cls_804B0F10::~cls_804B0F10() {
    delete field_0x8;
    field_0x8 = NULL;
    delete mScreen;
    mScreen = NULL;
    delete field_0xc;
    field_0xc = NULL;
    delete field_0x10;
    field_0x10 = NULL;
    delete field_0x14;
    field_0x14 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x18[i];
        field_0x18[i] = NULL;
        delete field_0x38[i];
        field_0x38[i] = NULL;
        delete field_0x28[i];
        field_0x28[i] = NULL;
        delete field_0x48[i];
        field_0x48[i] = NULL;
    }
    delete field_0x60;
    field_0x60 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x64[i];
        field_0x64[i] = NULL;
    }
    delete field_0x58;
    field_0x58 = NULL;
    delete field_0x5c;
    field_0x5c = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x78[i];
        field_0x78[i] = NULL;
    }
}
