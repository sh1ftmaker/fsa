#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_803B8C74.h"
#include "game/tu_80379F84.h"
#include "game/Message.h"
#include <string.h>
#include <dolphin/os.h>
#include "game/GameState.h"
#include "game/cls_804B1600.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "game/act/tu_80440BF4.h"

typedef void (cls_804B0288::*StateFunc_804B0288)();
extern StateFunc_804B0288 lbl_804B0168[24];

// FIXME(shotgun): 99.9 %, reloc-name (the vtable symbol)
cls_804B0288::~cls_804B0288() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    delete mScreen;
    mScreen = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0x14;
    field_0x14 = NULL;
    delete field_0x18;
    field_0x18 = NULL;
    for (s32 i = 0; i < 2; i++) {
        delete field_0x1c[i];
        field_0x1c[i] = NULL;
    }
    delete field_0x24;
    field_0x24 = NULL;
    delete field_0x28;
    field_0x28 = NULL;
    for (s32 i = 0; i < 8; i++) {
        delete field_0x2c[i];
        field_0x2c[i] = NULL;
    }
    lbl_8053ADB8 = NULL;
}

// FIXME(shotgun): 99.86 %, reloc-name
void cls_804B0288::fn_803D5554() {
    lbl_8053AEC0->fn_804310CC();
    field_0x54 = 0;
    field_0x50 = 0;
    field_0x216e = 0;
    field_0x78 = 0;
    field_0x88[0] = 0;
    field_0x2170 = 0;
    field_0x2171 = 0;
    field_0x7c = -1;
    field_0x84 = 0;
    field_0x216f = 0;
    field_0x80 = 0;
    field_0x82 = 0;
    field_0x86 = 0;
    field_0x2174 = 0;
    field_0x2173 = 0;
    field_0x2177 = 0;
    field_0x2175 = 0;
    field_0x1b4 = 0;
    field_0x2170 = 0;
    field_0x54 = 0x7B;
    field_0x2177 = 1;
    field_0x216c = 2;
    field_0x14->mPane->setAlpha(0);
    field_0x14->field_0x26 = 0;
    field_0x18->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0x18->field_0x22 = 0;
    for (s32 i = 0; i < 2; i++) {
        field_0x1c[i]->moveFromCentre_80402E08(0.0f, 200.0f);
        field_0x1c[i]->field_0x22 = 0;
    }
    field_0x24->moveFromCentre_80402E08(0.0f, -250.0f);
    field_0x24->field_0x22 = 0;
    field_0x28->mPane->hide();
    for (s32 i = 0; i < 8; i++) {
        field_0x2c[i]->fn_80402CE0(2.0f, 2.0f);
        field_0x2c[i]->field_0x24 = 0;
        field_0x2c[i]->mPane->setAlpha(0);
        field_0x2c[i]->field_0x26 = 0;
    }
    fn_803D49A4();
    lbl_80529DEC.field_0x243[0] = 0;
}

// FIXME(shotgun): 92.28 %, missing/extra (one more saved register: the bounce step)
void cls_804B0288::fn_803D521C() {
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0x216f == 3) {
        f32 bounce = 0.0f;
        if (field_0x82 > 0) {
            if (field_0x82 == 1) {
                field_0x80 = 30;
            }
            s32 step = field_0x82 < 5 ? field_0x82 : 10 - field_0x82;
            bounce = 5.0f * (f32)(step * step) / 25.0f;
            if (field_0x82 < 10) {
                field_0x82++;
            } else {
                field_0x82 = 0;
            }
        }
        field_0x80++;
        if (field_0x80 >= 60) {
            field_0x80 = 0;
        }
        f32 alpha;
        if (field_0x80 < 30) {
            alpha = ((f32)(field_0x80 * field_0x80) / 900.0f) * (f32)field_0x28->field_0x28;
        } else {
            s32 d = field_0x80 - 30;
            alpha = (1.0f - (f32)(d * d) / 900.0f) * (f32)field_0x28->field_0x28;
        }
        field_0x28->mPane->setAlpha((s32)alpha);
        mScreen->search('Paccur01')->setAlpha((s32)alpha);
        if (field_0x2171 == 0) {
            field_0x28->moveFromCentre_80402E08(0.0f, bounce);
            field_0x1c[0]->moveFromCentre_80402E08(0.0f, bounce);
            field_0x1c[1]->moveFromCentre_80402E08(0.0f, 0.0f);
        } else {
            field_0x28->moveFromCentre_80402E08(field_0x1c[1]->field_0x8 - field_0x1c[0]->field_0x8, bounce);
            field_0x1c[0]->moveFromCentre_80402E08(0.0f, 0.0f);
            field_0x1c[1]->moveFromCentre_80402E08(0.0f, bounce);
            if (0.0f == bounce && field_0x2171 != field_0x2170) {
                field_0x2171 = field_0x2170;
            }
        }
    }
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x2174 != 0) {
        field_0x8->setAlpha(field_0x2173);
        field_0x8->draw(-1.0f, -1.0f, 611.0f, 451.0f, false, false, false);
        lbl_8053AEC0->fn_804316EC();
    }
    if (field_0x2176 != 0) {
        lbl_8053AED8->fn_80440EE8();
    }
}

// FIXME(shotgun): 99.95 %, scheduling
void cls_804B0288::fn_803D5090() {
    if (field_0x2174 == 1) {
        lbl_8053AEC0->fn_80431174(field_0x2170);
    }
    u8 state = field_0x216c;
    if (state != 0 && state != 2 && state != 1 && state != 3) {
        u8 card = lbl_8051C1A0.field_0x1ff5;
        if (card == 0 || (card == 1 && !lbl_8053ACA8->fn_802B1DCC())) {
            if (field_0x216e == 0 && field_0x1b4 == 0) {
                field_0x216c = 5;
                field_0x2175 = 1;
            }
        }
    }
    if (field_0x216c < 0x18) {
        (lbl_8053ADB8->*lbl_804B0168[field_0x216c])();
    }
    fn_803D4DB0();
    lbl_8051C1A0.field_0x1ff5 = 2;
    if (field_0x2174 == 2) {
        if (field_0x2173 > 0x48) {
            field_0x2173 = 0x50;
        } else {
            field_0x2173 += 8;
        }
        if (lbl_8053AEC0->fn_804317AC()) {
            field_0x2173 = 0x50;
            field_0x2174 = 1;
        }
    } else if (field_0x2174 == 3) {
        if (field_0x2173 < 8) {
            field_0x2173 = 0;
        } else {
            field_0x2173 -= 8;
        }
        if (lbl_8053AEC0->fn_80431740()) {
            field_0x2173 = 0;
            field_0x2174 = 0;
        }
    }
}

// FIXME(shotgun): 98.44 %, regalloc-only
void cls_804B0288::fn_803D4770() {
    bool done1 = false;
    u8 done2 = 0;
    u8 done3 = 0;
    u8 done4 = 0;
    u8 done5 = 0;
    s16 t0 = lbl_8050DB0C.field_0x4c[(0xC8 - 0x4C) / 2];
    s16 t1 = lbl_8050DB0C.field_0x4c[(0xCA - 0x4C) / 2];
    s16 t2 = lbl_8050DB0C.field_0x4c[(0xCC - 0x4C) / 2];
    s16 t3 = lbl_8050DB0C.field_0x4c[(0xCE - 0x4C) / 2];
    s16 t4 = lbl_8050DB0C.field_0x4c[(0xD0 - 0x4C) / 2];
    s16 t5 = lbl_8050DB0C.field_0x4c[(0xD2 - 0x4C) / 2];
    s16 t6 = lbl_8050DB0C.field_0x4c[(0xE2 - 0x4C) / 2];
    s16 t7 = lbl_8050DB0C.field_0x4c[(0xD4 - 0x4C) / 2];
    s16 t8 = lbl_8050DB0C.field_0x4c[(0xD6 - 0x4C) / 2];
    s16 t9 = lbl_8050DB0C.field_0x4c[(0xD8 - 0x4C) / 2];
    s16 t10 = lbl_8050DB0C.field_0x4c[(0xDE - 0x4C) / 2];
    s16 t11 = lbl_8050DB0C.field_0x4c[(0xE0 - 0x4C) / 2];
    field_0x84++;
    if (field_0x84 > t0) {
        done1 = field_0x14->fn_80402628(t1, 0, field_0x14->field_0x28);
    }
    if (field_0x84 > t2) {
        done2 = field_0x24->fn_80402940(t3, 0, -250.0f, 0.0f);
        if (field_0x2176 != 0) {
            field_0x4c->fn_80402940(t3, 0, -200.0f, 0.0f);
        }
    }
    if (field_0x84 > t4 + t5) {
        done3 = field_0x18->fn_80402940(t6, 0, 26.0f, 0.0f);
    } else if (field_0x84 > t4) {
        field_0x18->fn_80402940(t5, 0, -480.0f, 26.0f);
    }
    for (s32 i = 0; i < 2; i++) {
        if (field_0x84 > t7 + t9 * i) {
            done4 = field_0x1c[i]->fn_80402940(t8, 0, 200.0f, 0.0f);
        }
    }
    if (field_0x84 > t10) {
        for (s32 i = 0; i < 8; i++) {
            done5 = field_0x2c[i]->fn_8040271C(t11, 2.0f, 1.0f);
            field_0x2c[i]->fn_80402628(t11, field_0x2c[i]->field_0x28, 0);
        }
        fn_803D49A4();
    }
    if (done1 && done2 && done3 && done4 && done5) {
        field_0x216f = 3;
        field_0x80 = 30;
        field_0x28->mPane->show();
        field_0x216c = field_0x216d;
    }
}

// FIXME(shotgun): 91.71 %, missing/extra (register allocation of the loop counters)
void cls_804B0288::fn_803D4484() {
    s16 fadeStart = lbl_8050DB0C.field_0x4c[(0xE4 - 0x4C) / 2];
    s16 fadeLength = lbl_8050DB0C.field_0x4c[(0xE6 - 0x4C) / 2];
    s16 start = lbl_8050DB0C.field_0x4c[(0xE8 - 0x4C) / 2];
    s16 length = lbl_8050DB0C.field_0x4c[(0xEA - 0x4C) / 2];
    s16 stagger = lbl_8050DB0C.field_0x4c[(0xEC - 0x4C) / 2];
    u8 flags[3];
    flags[2] = 0;
    flags[1] = 0;
    flags[0] = 0;
    s32 offset = 0;
    field_0x86++;
    if (field_0x216f == 1) {
        for (s32 i = 0; i < 2; i++) {
            if (field_0x86 > start + offset) {
                flags[i] = field_0x1c[i]->fn_80402940(length, 0, 200.0f, 0.0f);
            }
            offset += stagger;
        }
        if (flags[0] && flags[1]) {
            field_0x216f = 3;
            field_0x80 = 30;
            field_0x28->mPane->show();
        }
    } else if (field_0x216f == 2) {
        for (s32 i = 0; i < 2; i++) {
            if (field_0x86 > start + offset) {
                flags[i] = field_0x1c[i]->fn_80402940(length, 0, 0.0f, 200.0f);
            }
            offset += stagger;
        }
        if (field_0x86 > start + 1) {
            field_0x28->mPane->hide();
            if (flags[0] && flags[1]) {
                field_0x216f = 0;
            }
        }
    } else {
        flags[1] = 1;
        flags[0] = 1;
    }
    if (field_0x86 > fadeStart) {
        f32 ratio = (f32)(field_0x86 - fadeStart) / (f32)fadeLength;
        field_0xc->setAlpha((s32)(255.0f * (1.0f - ratio)));
        field_0x10->setAlpha((s32)(255.0f * ratio));
        if (field_0x86 >= fadeLength + fadeStart) {
            flags[2] = 1;
        }
    }
    if (flags[0] && flags[1] && flags[2]) {
        if (field_0x54 == 0x80) {
            gAudioMgr_8053AF80->fn_80458880(0x5B, -1);
        }
        strcpy(field_0xc->getStringPtr(), field_0x88);
        field_0xc->setAlpha(0xFF);
        field_0x10->setAlpha(0);
        field_0x216c = field_0x216d;
    }
}

// FIXME(shotgun): 94.63 %, scheduling+regalloc
void cls_804B0288::fn_803D4298() {
    if (lbl_8053AD30->fn_80309C4C(1, -1)) {
        if (field_0x2170 != 0) {
            field_0x2170--;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
        if (field_0x2170 < 1) {
            field_0x2170++;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x2175 = 0;
        field_0x2171 = field_0x2170;
        if (field_0x2170 == 0) {
            field_0x216c = 5;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x1b4 = 1;
            if (lbl_80529DEC.field_0x24d != 0) {
                field_0x18->field_0x22 = 0;
                field_0x1c[0]->field_0x22 = 0;
                field_0x1c[1]->field_0x22 = 0;
                field_0x24->field_0x22 = 0;
                if (field_0x2176 != 0) {
                    field_0x4c->field_0x22 = 0;
                }
                field_0x28->mPane->hide();
                field_0x84 = 0;
                field_0x216f = 2;
                field_0x2171 = field_0x2170;
                if (field_0x2174 != 0) {
                    lbl_8053AEC0->fn_804310BC();
                }
                field_0x2173 = 0;
                field_0x216d = 0x19;
                field_0x216c = 4;
            } else {
                field_0x2170 = 0;
                field_0x54 = 0x82;
                field_0x2177 = 0;
                field_0x216d = 3;
                gAudioMgr_8053AF80->fn_80458880(0xC, -1);
            }
            field_0x82 = 1;
        }
    }
}

// FIXME(shotgun): 99.96 %, scheduling
void cls_804B0288::fn_803D4068() {
    if (lbl_8053AD30->fn_80309C4C(1, -1)) {
        if (field_0x2170 != 0) {
            field_0x2170--;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
        if (field_0x2170 < 1) {
            field_0x2170++;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x18->field_0x22 = 0;
        field_0x1c[0]->field_0x22 = 0;
        field_0x1c[1]->field_0x22 = 0;
        field_0x14->field_0x26 = 0;
        for (s32 i = 0; i < 8; i++) {
            field_0x2c[i]->field_0x24 = 0;
            field_0x2c[i]->field_0x26 = 0;
        }
        field_0x24->field_0x22 = 0;
        if (field_0x2176 != 0) {
            field_0x4c->field_0x22 = 0;
        }
        field_0x28->mPane->hide();
        field_0x84 = 0;
        field_0x216f = 2;
        field_0x2171 = field_0x2170;
        if (field_0x2174 != 0) {
            lbl_8053AEC0->fn_804310BC();
        }
        field_0x2173 = 0;
        if (field_0x2170 == 0) {
            field_0x216d = 0x1A;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x216d = 0x19;
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
        field_0x216c = 4;
        field_0x82 = 1;
    }
}

// FIXME(shotgun): 98.56 %, scheduling+regalloc
void cls_804B0288::fn_803D3EC8() {
    u8 done1 = 0;
    u8 done2 = 0;
    u8 done3 = 0;
    s16 t0 = lbl_8050DB0C.field_0x4c[(0xEE - 0x4C) / 2];
    s16 t1 = lbl_8050DB0C.field_0x4c[(0xF0 - 0x4C) / 2];
    s16 t2 = lbl_8050DB0C.field_0x4c[(0xF2 - 0x4C) / 2];
    s16 t3 = lbl_8050DB0C.field_0x4c[(0xF4 - 0x4C) / 2];
    s16 t4 = lbl_8050DB0C.field_0x4c[(0xF6 - 0x4C) / 2];
    s16 t5 = lbl_8050DB0C.field_0x4c[(0xF8 - 0x4C) / 2];
    s16 t6 = lbl_8050DB0C.field_0x4c[(0xFA - 0x4C) / 2];
    field_0x84++;
    if (field_0x84 > t0) {
        done1 = field_0x24->fn_80402940(t1, 0, 0.0f, 480.0f);
        if (field_0x2176 != 0) {
            field_0x4c->fn_80402940(t1, 0, 0.0f, -200.0f);
            field_0x14->fn_80402628(t1, field_0x14->field_0x28, 0);
            for (s32 i = 0; i < 8; i++) {
                field_0x2c[i]->fn_8040271C(t1, 1.0f, 2.0f);
                field_0x2c[i]->fn_80402628(t1, field_0x2c[i]->field_0x28, 0);
            }
            fn_803D49A4();
        }
    }
    if (field_0x84 > t2) {
        done2 = field_0x18->fn_80402940(t3, 0, 0.0f, 480.0f);
    }
    for (s32 i = 0; i < 2; i++) {
        if (field_0x84 > t4 + t6 * i) {
            done3 = field_0x1c[i]->fn_80402940(t5, 0, 0.0f, 200.0f);
        }
    }
    if (done1 && done2 && done3) {
        field_0x216c = field_0x216d;
    }
}

// FIXME(shotgun): 99.84 %, reloc-name (the jump table)
void cls_804B0288::fn_803D3D50() {
    switch (lbl_8051C1A0.fn_803B9940(0)) {
    case 0:
        field_0x54 = 0x6B;
        field_0x2177 = 0;
        field_0x216d = 6;
        break;
    case 8:
        field_0x54 = 0x6C;
        field_0x2177 = 0;
        field_0x216d = 6;
        break;
    case 9:
        field_0x54 = 0x6D;
        field_0x2177 = 0;
        field_0x216d = 6;
        break;
    case 10:
        field_0x54 = 0x6E;
        field_0x2177 = 0;
        field_0x216d = 6;
        break;
    case 6:
    case 7:
        field_0x2170 = 1;
        field_0x54 = 0x6F;
        field_0x2177 = 0;
        field_0x216d = 7;
        break;
    case 11:
    case 12:
        field_0x54 = 0x70;
        field_0x2177 = 0;
        field_0x216d = 6;
        break;
    case 2:
        if (field_0x2175 != 0) {
            field_0x2170 = 0;
            field_0x54 = 0x7B;
            field_0x2177 = 0;
            field_0x216d = 2;
        } else {
            lbl_8051C1A0.fn_803B9B8C();
            field_0x216c = 0x16;
        }
        break;
    case 1:
        field_0x2170 = 1;
        field_0x54 = 0x76;
        field_0x2177 = 0;
        field_0x216d = 0xE;
        break;
    default:
        field_0x54 = 0;
        break;
    }
}

void cls_804B0288::fn_803D3C90() {
    if (lbl_8053AD30->fn_80309C4C(0x1300, -1)) {
        if (field_0x50 == 0x70) {
            field_0x54 = 0x85;
            field_0x2177 = 0;
            field_0x216d = field_0x216c;
        } else if (field_0x50 == 0x85) {
            field_0x2170 = 1;
            field_0x54 = 0x86;
            field_0x2177 = 0;
            field_0x216d = 0xB;
        } else {
            field_0x2170 = 0;
            field_0x54 = 0x7B;
            field_0x2177 = 0;
            field_0x216d = 2;
        }
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

void cls_804B0288::fn_803D3B38() {
    if (lbl_8053AD30->fn_80309C4C(1, -1)) {
        if (field_0x2170 != 0) {
            field_0x2170--;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
        if (field_0x2170 < 1) {
            field_0x2170++;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x2171 = field_0x2170;
        if (field_0x2170 == 0) {
            field_0x2170 = 1;
            field_0x54 = 0x72;
            field_0x2177 = 0;
            field_0x216d = 8;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x54 = 0x71;
            field_0x2177 = 0;
            field_0x216d = 6;
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
        field_0x82 = 1;
    }
}

void cls_804B0288::fn_803D39C8() {
    if (lbl_8053AD30->fn_80309C4C(1, -1)) {
        if (field_0x2170 != 0) {
            field_0x2170--;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
        if (field_0x2170 < 1) {
            field_0x2170++;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x2171 = field_0x2170;
        if (field_0x2170 == 0) {
            field_0x54 = 0x73;
            field_0x2177 = 0;
            field_0x216d = 9;
            lbl_8051C1A0.fn_803B9DDC();
            field_0x216e = 1;
            lbl_80529DEC.field_0x24c = 1;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x54 = 0x71;
            field_0x2177 = 0;
            field_0x216d = 6;
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
        field_0x82 = 1;
    }
}

void cls_804B0288::fn_803D3928() {
    s32 result = lbl_8051C1A0.fn_803B9C5C();
    if (result == 1) {
        field_0x54 = 0x74;
        field_0x2177 = 0;
        field_0x216d = 10;
        field_0x216e = 0;
        lbl_80529DEC.field_0x24c = 0;
        field_0x7c = 90;
    } else if (result == 2) {
        field_0x54 = 0x75;
        field_0x2177 = 0;
        field_0x216d = 0x17;
        field_0x216e = 0;
        lbl_80529DEC.field_0x24c = 0;
    }
}

void cls_804B0288::fn_803D3898() {
    if (field_0x7c > 0) {
        field_0x7c--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, -1) || field_0x7c == 0) {
        field_0x7c = -1;
        field_0x54 = 0;
        field_0x216c = 5;
        field_0x2175 = 1;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

void cls_804B0288::fn_803D373C() {
    if (lbl_8053AD30->fn_80309C4C(1, -1)) {
        if (field_0x2170 != 0) {
            field_0x2170--;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
        if (field_0x2170 < 1) {
            field_0x2170++;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x2171 = field_0x2170;
        if (field_0x2170 == 0) {
            field_0x2170 = 1;
            field_0x54 = 0x87;
            field_0x2177 = 0;
            field_0x216d = 0xC;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x2170 = 0;
            field_0x54 = 0x7B;
            field_0x2177 = 0;
            field_0x216d = 2;
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
        field_0x82 = 1;
    }
}

void cls_804B0288::fn_803D35F0() {
    if (lbl_8053AD30->fn_80309C4C(1, -1)) {
        if (field_0x2170 != 0) {
            field_0x2170--;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
        if (field_0x2170 < 1) {
            field_0x2170++;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x2171 = field_0x2170;
        if (field_0x2170 == 0) {
            field_0x54 = 0;
            field_0x216c = 0xD;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x2170 = 0;
            field_0x54 = 0x7B;
            field_0x2177 = 0;
            field_0x216d = 2;
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
        field_0x82 = 1;
    }
}

void cls_804B0288::fn_803D35C4() {
    OSResetSystem(1, 1, 1);
}

void cls_804B0288::fn_803D3460() {
    if (lbl_8053AD30->fn_80309C4C(1, -1)) {
        if (field_0x2170 != 0) {
            field_0x2170--;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
        if (field_0x2170 < 1) {
            field_0x2170++;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x2171 = field_0x2170;
        if (field_0x2170 == 0) {
            field_0x54 = 0x78;
            field_0x2177 = 0;
            field_0x216d = 0xF;
            field_0x216e = 1;
            lbl_80529DEC.field_0x24c = 1;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x54 = 0x77;
            field_0x2177 = 0;
            field_0x216d = 6;
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
        field_0x82 = 1;
    }
}

// FIXME(shotgun): 93.54 %, missing/extra (the three save-file index setups)
void cls_804B0288::fn_803D335C() {
    lbl_8050EC80.fn_803B93E8((SaveFile_803B950C*)field_0x1c0, (SaveFileBody_803B950C*)field_0x420, 0);
    fn_803BACF8((SaveFile_803B950C*)field_0x1c0, 0);
    lbl_8050EC80.fn_803B93E8((SaveFile_803B950C*)field_0x1c0, (SaveFileBody_803B950C*)field_0x420, 1);
    fn_803BACF8((SaveFile_803B950C*)field_0x1c0, 1);
    lbl_8050EC80.fn_803B93E8((SaveFile_803B950C*)field_0x1c0, (SaveFileBody_803B950C*)field_0x420, 2);
    fn_803BACF8((SaveFile_803B950C*)field_0x1c0, 2);
    lbl_8051C1A0.fn_803B9D54(field_0x1c0, 0x258, field_0x420, 0x1D4C, 0);
    for (s32 i = 0; i < 3; i++) {
        if (i != lbl_8050EC80.mCurrentFile) {
            memcpy(&lbl_8050EC80.mFiles[i], field_0x1c0 + i * 0xC8, 0xC8);
        }
    }
    field_0x216c = 0x10;
}

void cls_804B0288::fn_803D3180() {
    if (field_0x7c > 0) {
        field_0x7c--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, -1) || field_0x7c == 0) {
        field_0x7c = -1;
        field_0x2170 = 0;
        field_0x54 = 0x7B;
        field_0x2177 = 0;
        field_0x216d = 2;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

void cls_804B0288::fn_803D3108() {
    memcpy(field_0x1c0, (u8*)&lbl_8050EC80 + 0x20, 0x258);
    fn_803BACF8((SaveFile_803B950C*)field_0x1c0, lbl_8050EC80.mCurrentFile);
    lbl_8051C1A0.fn_803B9CCC(field_0x1c0, 0x258, 0);
    field_0x216c = 0x13;
}

void cls_804B0288::fn_803D3060() {
    s32 result = lbl_8051C1A0.fn_803B9BDC();
    if (result == 1) {
        field_0x54 = 0x80;
        field_0x2177 = 0;
        field_0x216d = 0x14;
        field_0x216e = 0;
        lbl_80529DEC.field_0x24c = 0;
        field_0x1b4 = 1;
        field_0x7c = 90;
    } else if (result == 2) {
        field_0x54 = 0x7F;
        field_0x2177 = 0;
        field_0x216d = 0x17;
        field_0x216e = 0;
        lbl_80529DEC.field_0x24c = 0;
    }
}

// FIXME(shotgun): 99.80 %, scheduling
void cls_804B0288::fn_803D2FFC() {
    if (field_0x7c > 0) {
        field_0x7c--;
        if (field_0x7c != 0) {
            return;
        }
    }
    if (lbl_80529DEC.field_0x24d != 0) {
        field_0x216d = 0x19;
        field_0x216c = 4;
    } else {
        field_0x54 = 0x82;
        field_0x2177 = 0;
        field_0x216d = 3;
    }
}

void cls_804B0288::fn_803D2C34() {
    if (lbl_8053AD30->fn_80309C4C(0x1300, -1)) {
        field_0x54 = 0;
        field_0x216c = 5;
        field_0x2175 = 1;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}
