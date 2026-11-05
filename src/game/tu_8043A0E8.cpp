#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804B13D4.h"
#include "game/cls_804B146C.h"
#include "game/tu_80379F84.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"

void fn_80440928(f32 x, f32 y);

void fn_8043A0E8(bool force) {
    if (lbl_8053AAF8->field_0x7f != 0 && (lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x28 & 1) == 0 &&
        lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 0 && !force) {
        return;
    }
    if (lbl_80529DEC.field_0x118 == 10 && lbl_80529DEC.field_0x11c == 2) {
        return;
    }
    lbl_8053AED0->field_0xdd = 0xF;
}

void fn_8043A170(s32 player, bool force) {
    if (lbl_8053AAF8->field_0x7f != 0 && (lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x28 & 1) == 0 &&
        lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 0 && !force) {
        return;
    }
    if (lbl_80529DEC.field_0x118 == 10 && lbl_80529DEC.field_0x11c == 2) {
        return;
    }
    lbl_8053AED0->field_0xdd |= (u8)(1 << player);
}

void cls_804B1920::fn_8043A208(u8 alpha, f32 a, f32 b, f32 c, f32 d) {
    if (field_0x10 != NULL) {
        field_0x1c0 = a;
        field_0x1c4 = b;
        field_0x1c8 = c;
        field_0x1cc = d;
        field_0x10->setAlpha(alpha);
        field_0x254 = 1;
    }
}

static const char* lbl_804B1820[] = {
    "w01_01.bti", "w01_02.bti", "w01_03.bti", "w01_04.bti",
    "w02_01.bti", "w02_02.bti", "w02_03.bti", "w02_04.bti",
    "w03_01.bti", "w03_02.bti", "w03_03.bti", "w03_04.bti",
    "w04_01.bti", "w04_02.bti", "w04_03.bti", "w04_04.bti",
    "w05_01.bti", "w05_02.bti", "w05_03.bti", "w05_04.bti",
    "w06_01.bti", "w06_02.bti", "w06_03.bti", "w06_04.bti",
    "w07_01.bti", "w07_02.bti", "w07_03.bti", "w07_04.bti",
    "w08_01.bti", "w08_02.bti", "w08_03.bti", "w08_04.bti",
};

// Declared here, not at its use, to keep the .data order (0x804B1820, 0x804B18D0, 0x804B1900).
static void (cls_804B1920::*lbl_804B18D0[4])() = {0, 0, 0, 0};

// FIXME: 99.0. The test for stage 7 / sub 1 is `bne next; b exit` in the original (ours: beq exit), one instruction fewer.
void cls_804B1920::fn_8043A450(s32 stage, s32 sub) {
    if (stage == 0 && sub == 2) return;
    if (stage == 1 && sub == 2) return;
    if (stage == 2 && sub == 2) return;
    if (stage == 3 && sub == 2) return;
    if (stage == 4 && sub == 2) return;
    if (stage == 5 && sub == 2) return;
    if (stage == 6 && sub == 2) return;
    if (stage == 7 && sub == 1) return;
    if (lbl_8053AAF8->field_0x7f != 0 && stage < 8 && sub < 4) {
        s32 index = sub + stage * 4;
        void* res;
        if (index != 0) {
            res = fn_804032E8('TIMG', lbl_804B1820[index]);
        } else {
            res = fn_804030C4('TIMG', lbl_804B1820[index]);
        }
        static_cast<J2DPicture*>(field_0x8->search(0x0000507730313031ull))->changeTexture((const ResTIMG*)res, 0);
        field_0x8->search(0x0000507730313031ull)->show();
        field_0x8->search(0x0000005064663031ull)->hide();
        field_0x106 = 1;
        field_0x10d = 0;
    }
}

void cls_804B1920::fn_8043A608() {
    fn_8043A450(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c);
}

static const char* lbl_804B1900[] = {
    "dB01f.bti", "d01f.bti", "d02f.bti", "d03f.bti", "d04f.bti", "d05f.bti", "d06f.bti", "d07f.bti",
};

void cls_804B1920::fn_8043ABCC(s32 index) {
    index = index < 0 ? 0 : index;
    void* res;
    if (index != 1) {
        res = fn_804032E8('TIMG', lbl_804B1900[index]);
    } else {
        res = fn_804030C4('TIMG', lbl_804B1900[index]);
    }
    static_cast<J2DPicture*>(field_0x8->search(0x0000005064663031ull))->changeTexture((const ResTIMG*)res, 0);
    field_0x8->search(0x0000005064663031ull)->show();
    field_0x8->search(0x0000507730313031ull)->hide();
    field_0x106 = 1;
    field_0x10d = 0;
}

void cls_804B1920::fn_8043CEA0() {
    field_0x104--;
    if (lbl_8053AAF8->field_0x4c != 7) {
        field_0x104 = 0;
    }
    if (field_0x104 > 0) {
        return;
    }
    for (int i = 0; i < 12; i++) {
        field_0x40[i]->field_0x22 = 0;
    }
    field_0x10b = 3;
}

// FIXME: 98.1. Registers differ: the original keeps the bounds value in f1 (the 0.0 default is dead there).
f32 cls_804B1920::fn_8043D514() {
    f32 result = 0.0f;
    if (field_0x10b != 0 && field_0x10c == 1) {
        result = -66.0f * (-87.0f - (field_0x40[9]->mPane->getBounds()->i.y - 13.0f)) / 100.0f;
    }
    return result;
}

// FIXME: 97.8. Registers differ: the original keeps the bounds value in f2 and the 100.0 literal in f1.
f32 cls_804B1920::fn_8043D57C() {
    f32 result = 0.0f;
    if (field_0x10b != 0 && field_0x10c == 0) {
        result = -66.0f * (100.0f - (field_0x40[0]->mPane->getBounds()->i.y - field_0x40[0]->field_0xc)) / 100.0f;
    }
    return result;
}

void cls_804B1920::fn_8043D5F0() {
    lbl_8053AB10->fn_8013B490();
    field_0x4->setScissor(true);
    field_0x4->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
}

void cls_804B1920::fn_8043D644() {
    (this->*lbl_804B18D0[field_0x10b])();
}

void cls_804B1920::fn_8043D834() {
    if (lbl_8053AAF8->field_0x7f != 0) {
        field_0x290->fn_8041E58C();
    } else {
        field_0x294->fn_80425128();
    }
}

void cls_804B1920::fn_8043D874(s32 id, s32 arg1, u32 arg2) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        field_0x290->fn_8041E708(id, arg1, arg2);
    } else {
        field_0x294->fn_80425234(id, arg1, arg2);
    }
}

// FIXME: 96.0. Same instructions, float registers allocated differently (f0/f7/f8).
bool cls_804B1920::fn_8043DF24(f32 x, f32 y, f32 px, f32 py, f32 margin) {
    JGeometry::TBox2f box(x - 94.0f, y - 61.5f, 94.0f + x, 61.5f + y);
    box.i.x -= margin;
    box.i.y -= margin;
    box.f.x = margin + box.f.x;
    box.f.y = margin + box.f.y;
    if (px >= box.i.x && px <= box.f.x && py >= box.i.y && py <= box.f.y) {
        return false;
    }
    return true;
}

void cls_804B1920::fn_8043E7CC(JGeometry::TVec3f pos) {
    Vec screen;
    lbl_8053AB10->field_0x24->fn_8030C5F0(&pos, false, false, &screen);
    fn_80440928(screen.x, screen.y);
}

void cls_804B1920::fn_8043EDE8(s32 arg) {
    field_0xe0 = arg;
    field_0xe4 = 30;
}

void fn_80440928(f32 x, f32 y) {
    lbl_8053AED0->fn_8043DF8C(x, y);
}
