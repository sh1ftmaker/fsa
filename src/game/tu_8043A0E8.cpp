#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804B13D4.h"
#include "game/cls_804B146C.h"
#include "game/cls_804B1C58.h"
#include "game/cls_804B1D00.h"
#include "game/act/tu_804432DC.h"
#include "game/tu_804455E0.h"
#include "game/tu_804465B8.h"
#include "game/tu_80447618.h"
#include "game/tu_80448940.h"
#include "game/tu_8044998C.h"
#include "game/act/tu_8044C028.h"
#include "game/cls_804B1E1C.h"
#include "game/tu_803B8C74.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/Message.h"
#include "game/Player.h"
#include <string.h>
#include <math.h>

// Auto-inlining is off for the whole file (docs/INLINING.md, pragma rule 2): the
// original inlines no same-unit plain function. Checked: built with inlining off,
// every written function's calls to this unit's functions equal the original's
// `bl`s. The callers that need it: fn_8043E7CC calls fn_80440928 out of line.
#pragma auto_inline off

// The panel offsets and the box test the panel checks inline (fn_8043D514 / fn_8043D57C /
// fn_8043DF24 as written out in fn_8043E398).
static inline f32 topOffset(cls_804B1920* m) {
    f32 result = 0.0f;
    if (m->field_0x10b != 0 && m->field_0x10c == 1) {
        result = -66.0f * (-87.0f - (m->field_0x40[9]->mPane->getBounds()->i.y - 13.0f)) / 100.0f;
    }
    return result;
}

static inline f32 bottomOffset(cls_804B1920* m) {
    f32 result = 0.0f;
    if (m->field_0x10b != 0 && m->field_0x10c == 0) {
        result = -66.0f * (100.0f - (m->field_0x40[0]->mPane->getBounds()->i.y - m->field_0x40[0]->field_0xc)) / 100.0f;
    }
    return result;
}

static inline bool insideBox(f32 x, f32 y, f32 px, f32 py, f32 margin) {
    JGeometry::TBox2f box(x - 94.0f, y - 61.5f, 94.0f + x, 61.5f + y);
    box.i.x -= margin;
    box.i.y -= margin;
    box.f.x = margin + box.f.x;
    box.f.y = margin + box.f.y;
    if (px >= box.i.x && px <= box.f.x && py >= box.i.y && py <= box.f.y) {
        return true;
    }
    return false;
}


void fn_804408F0(JGeometry::TVec3f* pos);
void fn_80440928(f32 x, f32 y);

// The rank pictures (fn_8043F504).
static const char* lbl_804B1810[] = {
    "kk_mainRank_01.bti", "kk_mainRank_02.bti", "kk_mainRank_03.bti", "kk_mainRank_04.bti",
};

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

// fn_8043D644's steps, indexed by field_0x10b; built at static init time.
// Declared here, not at its use, to keep the .data order (0x804B1820, 0x804B18D0, 0x804B1900).
static void (cls_804B1920::*lbl_804B18D0[4])() = {
    &cls_804B1920::fn_8043D034,
    &cls_804B1920::fn_8043CF40,
    &cls_804B1920::fn_8043CEA0,
    &cls_804B1920::fn_8043CDA0,
};

static const char* lbl_804B1900[] = {
    "dB01f.bti", "d01f.bti", "d02f.bti", "d03f.bti", "d04f.bti", "d05f.bti", "d06f.bti", "d07f.bti",
};

void fn_80440928(f32 x, f32 y) {
    lbl_8053AED0->fn_8043DF8C(x, y);
}

// FIXME(shotgun): 57.1 %, scheduling
void fn_804408F0(JGeometry::TVec3f* pos) {
    lbl_8053AED0->fn_8043E7CC(*pos);
}

cls_804B1920::~cls_804B1920() {
    if (field_0x290 != NULL) {
        delete field_0x290;
        field_0x290 = NULL;
    }
    if (field_0x294 != NULL) {
        delete field_0x294;
        field_0x294 = NULL;
    }
    if (field_0x298 != NULL) {
        delete field_0x298;
        field_0x298 = NULL;
    }
    if (field_0x2b8 != NULL) {
        delete field_0x2b8;
        field_0x2b8 = NULL;
    }
    if (field_0x2b0 != NULL) {
        delete field_0x2b0;
        field_0x2b0 = NULL;
    }
    if (field_0x29c != NULL) {
        delete field_0x29c;
        field_0x29c = NULL;
    }
    if (field_0x2a8 != NULL) {
        delete field_0x2a8;
        field_0x2a8 = NULL;
    }
    if (field_0x2b4 != NULL) {
        delete field_0x2b4;
        field_0x2b4 = NULL;
    }
    if (field_0x2ac != NULL) {
        delete field_0x2ac;
        field_0x2ac = NULL;
    }
    if (field_0x2a4 != NULL) {
        delete field_0x2a4;
        field_0x2a4 = NULL;
    }
    if (field_0x2a0 != NULL) {
        delete field_0x2a0;
        field_0x2a0 = NULL;
    }
    if (field_0x2bc != NULL) {
        delete field_0x2bc;
        field_0x2bc = NULL;
    }
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    for (s32 i = 0; i < 12; i++) {
        delete field_0x40[i];
        field_0x40[i] = NULL;
    }
    delete field_0x70[0];
    field_0x70[0] = NULL;
    delete field_0x70[1];
    field_0x70[1] = NULL;
    delete field_0x258[0];
    field_0x258[0] = NULL;
    delete field_0x258[1];
    field_0x258[1] = NULL;
    delete field_0x258[2];
    field_0x258[2] = NULL;
    delete field_0x258[3];
    field_0x258[3] = NULL;
    delete field_0x258[4];
    field_0x258[4] = NULL;
    delete field_0x258[5];
    field_0x258[5] = NULL;
    delete field_0x258[6];
    field_0x258[6] = NULL;
    delete field_0x258[7];
    field_0x258[7] = NULL;
    delete field_0x258[8];
    field_0x258[8] = NULL;
    delete field_0x258[9];
    field_0x258[9] = NULL;
    delete field_0x280[0];
    field_0x280[0] = NULL;
    delete field_0x280[1];
    field_0x280[1] = NULL;
    delete field_0x280[2];
    field_0x280[2] = NULL;
    delete field_0x280[3];
    field_0x280[3] = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x14[i];
        field_0x14[i] = NULL;
    }
    delete field_0x14[4];
    field_0x14[4] = NULL;
    if (field_0x10 != NULL) {
        delete field_0x10;
        field_0x10 = NULL;
    }
    lbl_8053AED0 = NULL;
}

void cls_804B1920::fn_8043EDF8() {
    field_0xdd = 0;
    field_0xdc = 0;
    field_0x255 = 0;
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        field_0x290->fn_8041FC2C();
        if (field_0x2b0 != NULL) {
            field_0x2b0->fn_8044A06C();
        }
        if (field_0x29c != NULL) {
            field_0x29c->fn_80445D24();
        }
        if (field_0x2a8 != NULL) {
            field_0x2a8->fn_80448334();
        }
        if (field_0x2b4 != NULL) {
            field_0x2b4->fn_8044AFD8();
        }
        if (field_0x2ac != NULL) {
            field_0x2ac->fn_804490B8();
        }
        if (field_0x2a4 != NULL) {
            field_0x2a4->fn_804479A0();
        }
        if (field_0x2a0 != NULL) {
            field_0x2a0->fn_80446DB0();
        }
    } else {
        field_0x294->fn_804263A8();
    }
    if (field_0x298 != NULL) {
        lbl_8053AEF0->fn_80443688();
    }
}

void cls_804B1920::fn_8043EDE8(s32 arg) {
    field_0xe0 = arg;
    field_0xe4 = 30;
}

// FIXME(shotgun): 99.07 %, regalloc-only
void cls_804B1920::fn_8043E80C(u32 digit, u8 alpha, u32 top, u32 bottom, f32 x, f32 y, f32 scale) {
    f32 sx;
    f32 sy;
    if ((99.0f == scale) | ((90.0f == scale) | (91.0f == scale))) {
        f32 size = 96.0f;
        sx = size;
        sy = size;
    } else if (scale > 1.0f) {
        f32 size = 24.0f * scale;
        sx = size;
        sy = size;
    } else if (1.0f == scale) {
        f32 size = 19.2f;
        sx = size;
        sy = size;
    } else {
        f32 size = 9.6f;
        sx = size;
        sy = size;
    }
    f32 px = (12.0f + x) - sx * 0.5f;
    f32 py = (12.0f + y) - sy * 0.5f;
    switch (digit) {
    case 0:
        field_0x258[0]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[0]->setAlpha(alpha);
        field_0x258[0]->draw(px, py, sx, sy, false, false, false);
        break;
    case 1:
        field_0x258[1]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[1]->setAlpha(alpha);
        field_0x258[1]->draw(px, py, sx, sy, false, false, false);
        break;
    case 2:
        field_0x258[2]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[2]->setAlpha(alpha);
        field_0x258[2]->draw(px, py, sx, sy, false, false, false);
        break;
    case 3:
        field_0x258[3]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[3]->setAlpha(alpha);
        field_0x258[3]->draw(px, py, sx, sy, false, false, false);
        break;
    case 4:
        field_0x258[4]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[4]->setAlpha(alpha);
        field_0x258[4]->draw(px, py, sx, sy, false, false, false);
        break;
    case 5:
        field_0x258[5]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[5]->setAlpha(alpha);
        field_0x258[5]->draw(px, py, sx, sy, false, false, false);
        break;
    case 6:
        field_0x258[6]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[6]->setAlpha(alpha);
        field_0x258[6]->draw(px, py, sx, sy, false, false, false);
        break;
    case 7:
        field_0x258[7]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[7]->setAlpha(alpha);
        field_0x258[7]->draw(px, py, sx, sy, false, false, false);
        break;
    case 8:
        field_0x258[8]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[8]->setAlpha(alpha);
        field_0x258[8]->draw(px, py, sx, sy, false, false, false);
        break;
    case 9:
        field_0x258[9]->setBlackWhite(JUtility::TColor(top), JUtility::TColor(bottom));
        field_0x258[9]->setAlpha(alpha);
        field_0x258[9]->draw(px, py, sx, sy, false, false, false);
        break;
    }
}

void cls_804B1920::fn_8043E7CC(JGeometry::TVec3f pos) {
    Vec screen;
    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&pos, false, false, &screen);
    fn_80440928(screen.x, screen.y);
}

// FIXME(shotgun): 88.13 %, missing/extra (the cx / cy and box bounds live in fewer float registers)
void cls_804B1920::fn_8043E398(const Vec* a, const Vec* b) {
    Vec screenA;
    Vec screenB;
    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(a, false, false, &screenA);
    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(b, false, false, &screenB);
    f32 dy = fabs(screenB.y - screenA.y);
    f32 margin = fabs(screenB.x - screenA.x);
    if (!(margin >= dy)) {
        margin = dy;
    }
    f32 cx = (screenA.x + screenB.x) * 0.5f;
    f32 cy = (screenA.y + screenB.y) * 0.5f;
    if (insideBox(93.0f, 61.5f + topOffset(lbl_8053AED0), cx, cy, margin)) {
        lbl_8053AED0->field_0xdc |= 1;
    }
    if (insideBox(514.0f, 61.5f + topOffset(lbl_8053AED0), cx, cy, margin)) {
        lbl_8053AED0->field_0xdc |= 2;
    }
    if (insideBox(93.0f, 407.5f + bottomOffset(lbl_8053AED0), cx, cy, margin)) {
        lbl_8053AED0->field_0xdc |= 4;
    }
    if (insideBox(514.0f, 407.5f + bottomOffset(lbl_8053AED0), cx, cy, margin)) {
        lbl_8053AED0->field_0xdc |= 8;
    }
}

// FIXME(shotgun): 88.8 %, missing/extra + reloc-name
void cls_804B1920::fn_8043DF8C(f32 x, f32 y) {
    Vec origin;
    origin.x = origin.y = origin.z = 0.0f;
    Vec screenOrigin;
    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&origin, false, false, &screenOrigin);
    Vec unit;
    unit.x = 16.0f;
    unit.y = unit.z = 0.0f;
    Vec screenUnit;
    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&unit, false, false, &screenUnit);
    f32 margin = screenUnit.x - screenOrigin.x;
    if (insideBox(93.0f, 61.5f + topOffset(lbl_8053AED0), x, y, margin)) {
        lbl_8053AED0->field_0xdc |= 1;
    }
    if (insideBox(514.0f, 61.5f + topOffset(lbl_8053AED0), x, y, margin)) {
        lbl_8053AED0->field_0xdc |= 2;
    }
    if (insideBox(93.0f, 407.5f + bottomOffset(lbl_8053AED0), x, y, margin)) {
        lbl_8053AED0->field_0xdc |= 4;
    }
    if (insideBox(514.0f, 407.5f + bottomOffset(lbl_8053AED0), x, y, margin)) {
        lbl_8053AED0->field_0xdc |= 8;
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

void cls_804B1920::fn_8043DC98() {
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        field_0x290->fn_8041F720();
        if (field_0x2b0 != NULL) {
            field_0x2b0->fn_8044A024();
        }
        if (field_0x29c != NULL) {
            field_0x29c->fn_80445CDC();
        }
        if (field_0x2a8 != NULL) {
            field_0x2a8->fn_804482EC();
        }
        if (field_0x2b4 != NULL) {
            field_0x2b4->fn_8044AF90();
        }
        if (field_0x2ac != NULL) {
            field_0x2ac->fn_80449070();
        }
        if (field_0x2a4 != NULL) {
            field_0x2a4->fn_80447958();
        }
        if (field_0x2a0 != NULL) {
            field_0x2a0->fn_80446D68();
        }
        if (field_0x2b8 != NULL) {
            field_0x2b8->fn_8044C14C();
        }
    } else {
        field_0x294->fn_80425F6C();
    }
    if (field_0x2bc != NULL) {
        field_0x2bc->fn_804556A4();
    }
    // The original clears the four player bits twice (two unrolled runs).
    for (int i = 0; i < 4; i++) {
        lbl_8053AED0->field_0xdc &= ~(1 << i);
    }
    for (int i = 0; i < 4; i++) {
        lbl_8053AED0->field_0xdc &= ~(1 << i);
    }
    gGameRoot2_8053AB10->fn_8013B490();
    fn_8043BB84();
    fn_8043ACE4();
    fn_8043A638();
    if (field_0x10 != NULL && field_0x254 != 0) {
        field_0x10->draw(field_0x1c0, field_0x1c4, field_0x1c8, field_0x1cc, false, false, false);
        field_0x254 = 0;
    }
    if (field_0x298 != NULL) {
        lbl_8053AEF0->fn_804435E4();
    }
    if (field_0x257 != 0) {
        gGameRoot2_8053AB10->fn_8013B490();
        field_0xc->search('Ndata05')->move(field_0x110 + lbl_8050DB0C.field_0x4c[(0x2A2 - 0x4C) / 2],
                                           field_0x114 + lbl_8050DB0C.field_0x4c[(0x2A4 - 0x4C) / 2]);
        field_0xc->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}

// FIXME(shotgun): 99.38 %, reloc-name
void cls_804B1920::fn_8043DB58() {
    gGameRoot2_8053AB10->fn_8013B490();
    u8 mode = field_0x256;
    if (mode != 0xFF) {
        switch (mode) {
        case 0:
            field_0x14[0]->draw(179.5f, 173.0f, false, false, false);
            break;
        case 1:
            field_0x14[1]->draw(179.5f, 173.0f, false, false, false);
            break;
        case 2:
            field_0x14[2]->draw(179.5f, 173.0f, false, false, false);
            break;
        case 3:
            field_0x14[3]->draw(179.5f, 173.0f, false, false, false);
            break;
        case 4:
            field_0x14[4]->draw(179.5f, 173.0f, false, false, false);
            break;
        }
    }
}

// FIXME: 99.7. Registers only: the second loop counts in r28 (ours r29, the first loop's).
void cls_804B1920::fn_8043D8B4() {
    int i;
    if (gGameRoot_8053AAF8->field_0x7e == 0) {
        for (i = 0; i < 4; i++) {
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                s32 player = fn_8022EA0C(i);
                if (getPlayerLayer_8023DE58(player) < 8) {
                    JGeometry::TVec3f pos = *getPlayerPos_8023E724(player);
                    fn_804408F0(&pos);
                }
                lbl_80529DEC.field_0xc8[i] = fn_80237810(i);
                lbl_80529DEC.field_0xf8[i] = fn_802373EC(i);
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (fn_8023F264(i) && getPlayerLayer_8023DE58(i) < 8) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                fn_804408F0(&pos);
            }
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                lbl_80529DEC.field_0xc8[i] = fn_80237810(i);
                lbl_80529DEC.field_0xf8[i] = fn_802373EC(i);
            }
        }
    }
    if (gGameRoot_8053AAF8->field_0x7e != 0 && fn_8037E4BC()) {
        lbl_8053AED0->field_0xdc |= 4;
        lbl_8053AED0->field_0xdc |= 8;
    }
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        field_0x290->fn_8041EDB0();
        if (field_0x2b0 != NULL) {
            field_0x2b0->fn_8044998C();
        }
        if (field_0x29c != NULL) {
            field_0x29c->fn_804455E0();
        }
        if (field_0x2a8 != NULL) {
            field_0x2a8->fn_80447FAC();
        }
        if (field_0x2b4 != NULL) {
            field_0x2b4->fn_8044A828();
        }
        if (field_0x2ac != NULL) {
            field_0x2ac->fn_80448940();
        }
        if (field_0x2a4 != NULL) {
            field_0x2a4->fn_80447618();
        }
        if (field_0x2a0 != NULL) {
            field_0x2a0->fn_804465B8();
        }
        if (field_0x2b8 != NULL) {
            field_0x2b8->fn_8044C038();
        }
    } else {
        field_0x294->fn_804257E4();
    }
    if (field_0x2bc != NULL) {
        field_0x2bc->fn_8045560C();
    }
    field_0x255 = 0;
    if (field_0x298 != NULL) {
        lbl_8053AEF0->fn_80443638();
    }
    if (gGameRoot_8053AAF8->field_0x85 != 0) {
        field_0x257 = 1;
    } else {
        field_0x257 = 0;
    }
}

void cls_804B1920::fn_8043D874(s32 id, s32 arg1, u32 arg2) {
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        field_0x290->fn_8041E708(id, arg1, arg2);
    } else {
        field_0x294->fn_80425234(id, arg1, arg2);
    }
}

void cls_804B1920::fn_8043D834() {
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        field_0x290->fn_8041E58C();
    } else {
        field_0x294->fn_80425128();
    }
}

// FIXME: 95.4. The original tests 0x199 / 0x1B5 unsigned (cmplwi) but switches signed (cmpwi on id
// itself), compares the message type byte signed (cmpwi 0x2A), and turns field_0xde & bit into a
// bool first (neg / or / srwi.; ours: and.).
bool cls_804B1920::fn_8043D67C(s32 id, u32 arg1, u32 arg2) {
    if (field_0x10a != 0) {
        return false;
    }
    if (id == 0x199) {
        if (getGameView_802D6A9C()->fn_802D46D8(0, id, false) == true) {
            return false;
        }
        getGameView_802D6A9C()->fn_802D46D8(0, id, true);
    } else if (id == 0x1B5) {
        u8 bit = 1 << arg1;
        if (field_0xde & bit) {
            return false;
        }
        field_0xde |= bit;
    }
    u8 type;
    fn_80383E2C(id, &type);
    if (type == 0x2A) {
        if (getGameView_802D6A9C()->fn_802D46D8(0, id, false) == true) {
            return false;
        }
        getGameView_802D6A9C()->fn_802D46D8(0, id, true);
    }
    field_0x10a = 1;
    field_0xfc = arg1 + 1;
    field_0xe8 = id;
    field_0x100 = arg2;
    switch (id) {
    case 0x18F:
    case 0x190:
    case 0x195:
    case 0x196:
    case 0x197:
    case 0x198:
    case 0x1A8:
        gAudioMgr_8053AF80->fn_80458880(0x40, -1);
        break;
    case 0x23F:
        break;
    default:
        gAudioMgr_8053AF80->fn_80458880(0x39, -1);
        break;
    }
    return true;
}

void cls_804B1920::fn_8043D644() {
    (this->*lbl_804B18D0[field_0x10b])();
}

void cls_804B1920::fn_8043D5F0() {
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x4->setScissor(true);
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

// FIXME: 97.8. Registers differ: the original keeps the bounds value in f2 and the 100.0 literal in f1.
f32 cls_804B1920::fn_8043D57C() {
    f32 result = 0.0f;
    if (field_0x10b != 0 && field_0x10c == 0) {
        result = -66.0f * (100.0f - (field_0x40[0]->mPane->getBounds()->i.y - field_0x40[0]->field_0xc)) / 100.0f;
    }
    return result;
}

// FIXME: 98.1. Registers differ: the original keeps the bounds value in f1 (the 0.0 default is dead there).
f32 cls_804B1920::fn_8043D514() {
    f32 result = 0.0f;
    if (field_0x10b != 0 && field_0x10c == 1) {
        result = -66.0f * (-87.0f - (field_0x40[9]->mPane->getBounds()->i.y - 13.0f)) / 100.0f;
    }
    return result;
}

// FIXME(shotgun): 99.36 %, reloc-name
void cls_804B1920::fn_8043D39C() {
    if (gGameRoot_8053AAF8->field_0x7f) {
        field_0x290->fn_8041F5FC();
    } else {
        field_0x294->fn_80425F24();
    }
    if (gGameRoot_8053AAF8->field_0xfe == 1 && field_0xe4 != 0) {
        gGameRoot2_8053AB10->fn_8013B490();
        field_0xe4--;
        u8 tens = field_0xe0 / 10;
        u8 ones = field_0xe0 % 10;
        if (tens != 0) {
            fn_8043E80C(tens, 0xFF, 0x0A280A00, 0x28FF28FF, 240.0f, 200.0f, 90.0f);
            fn_8043E80C(ones, 0xFF, 0x0A280A00, 0x28FF28FF, 340.0f, 200.0f, 90.0f);
        } else if (ones != 0) {
            fn_8043E80C(ones, 0xFF, 0x50500000, 0xFFFF00FF, 300.0f, 200.0f, 91.0f);
        } else {
            fn_8043E80C(ones, 0xFF, 0x50282800, 0xFF5050FF, 300.0f, 200.0f, 99.0f);
        }
    }
}

// FIXME: 98.9. The original loads field_0x70[0]->field_0x18 (f1) before the 0xFC / 0x100 words for
// fn_803837A4 (ours after them, in r7); the 1.0 / 16.0 / 100.0 / 13.0 literals sit early in our .sdata2 pool because fn_8043A638..fn_8043CB24 (the earlier first uses) are unwritten.
void cls_804B1920::fn_8043D034() {
    if (lbl_8053AED0->field_0x10a) {
        Vec originScreen;
        Vec topScreen;
        Vec screen;
        for (int i = 0; i < 12; i++) {
            field_0x40[i]->field_0x22 = 0;
        }
        fn_803837A4(field_0x78, lbl_8053AED0->field_0xe8, lbl_8053AED0->field_0xfc, lbl_8053AED0->field_0x100,
                    field_0x70[0]->field_0x18, 1.0f);
        for (int i = 0; i < 2; i++) {
            strcpy(((J2DTextBox*)field_0x70[i]->mPane)->getStringPtr(), field_0x78);
        }
        // The screen height of 16 units at the origin.
        JGeometry::TVec3f origin(0.0f, 0.0f, 0.0f);
        gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&origin, false, false, &originScreen);
        JGeometry::TVec3f top(0.0f, 16.0f, 0.0f);
        gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&top, false, false, &topScreen);
        f32 margin = (topScreen.y - originScreen.y) +
                     ((s32)JUTVideo::getManager()->getEfbHeight() - field_0x40[9]->field_0xc);
        u8 side[4];
        field_0x10c = 0;
        for (int i = 0; i < 4; i++) {
            gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(getPlayerPos_8023E724(i), false, false, &screen);
            side[i] = 0;
            if (screen.x > 0.0f && screen.x < (s32)JUTVideo::getManager()->getFbWidth() && screen.y > 0.0f &&
                screen.y < (s32)JUTVideo::getManager()->getEfbHeight()) {
                if (screen.y > (s32)JUTVideo::getManager()->getEfbHeight() - margin) {
                    side[i] = 1;
                    field_0x10c = 1;
                } else if (screen.y < margin) {
                    side[i] = 2;
                }
            }
        }
        if (field_0x10c == 1) {
            for (int i = 0; i < 4; i++) {
                if (side[i] == 2) {
                    field_0x10c = 0;
                }
            }
        }
        f32 y;
        if (field_0x10c == 0) {
            y = 100.0f;
        } else {
            y = 13.0f - field_0x40[9]->field_0xc - 100.0f;
        }
        for (int i = 0; i < 12; i++) {
            field_0x40[i]->moveFromCentre_80402E08(0.0f, y);
        }
        field_0x10b = 1;
    }
}

// FIXME: 99.8. Code identical; the 100.0 / 13.0 literals sit early in our .sdata2 pool because fn_8043A638..fn_8043CB24 (the earlier first uses) are unwritten.
void cls_804B1920::fn_8043CF40() {
    f32 x;
    f32 y;
    if (field_0x10c == 0) {
        x = 100.0f;
        y = 0.0f;
    } else {
        y = 13.0f - field_0x40[9]->field_0xc;
        x = y - 100.0f;
    }
    u8 done = field_0x40[0]->fn_80402940(19, 0, x, y);
    for (int i = 1; i < 12; i++) {
        field_0x40[i]->fn_80402940(19, 0, x, y);
    }
    if (done) {
        field_0x104 = 160;
        field_0x10b = 2;
    }
}

void cls_804B1920::fn_8043CEA0() {
    field_0x104--;
    if (gGameRoot_8053AAF8->field_0x4c != 7) {
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

// FIXME: 99.8. Code identical; the 100.0 / 13.0 literals sit early in our .sdata2 pool because fn_8043A638..fn_8043CB24 (the earlier first uses) are unwritten.
void cls_804B1920::fn_8043CDA0() {
    f32 x;
    f32 y;
    if (field_0x10c == 0) {
        x = 0.0f;
        y = 100.0f;
    } else {
        x = 13.0f - field_0x40[9]->field_0xc;
        y = x - 100.0f;
    }
    u8 done = field_0x40[0]->fn_80402940(11, 0, x, y);
    for (int i = 1; i < 12; i++) {
        field_0x40[i]->fn_80402940(11, 0, x, y);
    }
    if (done) {
        cls_804B1920* screen = lbl_8053AED0;
        screen->field_0x10a = 0;
        screen->field_0xfc = 0;
        screen->field_0xe8 = 0;
        screen->field_0x100 = 0;
        field_0x10b = 0;
    }
}

// FIXME(shotgun): 79.2 %, missing/extra
bool cls_804B1920::fn_8043CB24(s32 value, u8 arg2, u8 arg3, u8 digits, bool flag, f32 x, f32 y, f32 scale) {
    if (field_0x252 >= 10) {
        return false;
    }
    field_0x118[field_0x252] = x - 0.5f * (19.0f * (digits % 2) * scale) - 16.0f * (digits >> 1) * scale;
    field_0x140[field_0x252] = y - 0.5f * (19.0f * scale);
    field_0x168[field_0x252] = scale;
    field_0x244[field_0x252] = arg2;
    field_0x226[field_0x252] = digits;
    field_0x1f4[0][field_0x252] = 0xFF;
    field_0x1f4[1][field_0x252] = 0xFF;
    field_0x1f4[2][field_0x252] = 0xFF;
    field_0x1f4[3][field_0x252] = 0xFF;
    field_0x1f4[4][field_0x252] = 0xFF;
    bool seen = flag;
    s32 div = 1;
    for (s32 i = 0; i < digits; i++) {
        div *= 10;
    }
    if (value > div - 1) {
        value = div - 1;
    }
    if (value < 0) {
        value = 0;
    }
    div /= 10;
    for (s32 i = 0; i < digits - 1; i++) {
        s32 digit = value / div;
        value -= digit * div;
        if (!seen && digit == 0) {
            field_0x1f4[i][field_0x252] = 0xFF;
        } else {
            if (!seen) {
                seen = true;
            }
            field_0x1f4[i][field_0x252] = digit;
        }
        div /= 10;
    }
    field_0x1f4[digits - 1][field_0x252] = value;
    if (arg3 > 5) {
        arg3 = 5;
    }
    field_0x23a[field_0x252] = arg3;
    field_0x252++;
    return true;
}

void cls_804B1920::fn_8043ACE4() {
    // Built once, never read.
    static JUtility::TColor lbl_8052C5F0[4] = {
        JUtility::TColor(0xF2, 0x49, 0x12, 0xFF),
        JUtility::TColor(0xF2, 0x49, 0x12, 0xFF),
        JUtility::TColor(0xF2, 0x49, 0x12, 0xFF),
        JUtility::TColor(0xD8, 0xFF, 0xA1, 0xFF),
    };
    if (field_0x253 != 0) {
        for (s32 i = 0; i < field_0x253; i++) {
            f32 alpha = field_0x24e[i] / 255.0f;
            gGameRoot2_8053AB10->fn_8013B490();
            switch (field_0x1d0[i]) {
            case 1:
                field_0x280[0]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xFF));
                field_0x280[0]->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                               JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
                field_0x280[0]->setAlpha(200.0f * alpha);
                field_0x280[0]->draw(3.0f + field_0x190[i], 3.0f + field_0x1a0[i], 48.0f * field_0x1b0[i],
                                     48.0f * field_0x1b0[i], false, false, false);
                field_0x280[0]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
                field_0x280[0]->setCornerColor(JUtility::TColor(0xB9, 0x12, 0x12, 0xFF), JUtility::TColor(0xD8, 0x3A, 0x16, 0xFF),
                                               JUtility::TColor(0xB9, 0x54, 0x12, 0xFF), JUtility::TColor(0xFF, 0x9D, 0x10, 0xFF));
                field_0x280[0]->setAlpha(field_0x24e[i]);
                field_0x280[0]->draw(field_0x190[i], field_0x1a0[i], 48.0f * field_0x1b0[i], 48.0f * field_0x1b0[i],
                                     false, false, false);
                break;
            case 2:
                field_0x280[1]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xFF));
                field_0x280[1]->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                               JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
                field_0x280[1]->setAlpha(200.0f * alpha);
                field_0x280[1]->draw(3.0f + field_0x190[i], 3.0f + field_0x1a0[i], 48.0f * field_0x1b0[i],
                                     48.0f * field_0x1b0[i], false, false, false);
                field_0x280[1]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
                field_0x280[1]->setCornerColor(JUtility::TColor(0xF2, 0x48, 0x09, 0xFF), JUtility::TColor(0xF2, 0x48, 0x09, 0xFF),
                                               JUtility::TColor(0xF2, 0xAB, 0x24, 0xFF), JUtility::TColor(0xFC, 0xFF, 0x21, 0xFF));
                field_0x280[1]->setAlpha(field_0x24e[i]);
                field_0x280[1]->draw(field_0x190[i], field_0x1a0[i], 48.0f * field_0x1b0[i], 48.0f * field_0x1b0[i],
                                     false, false, false);
                break;
            case 3:
                field_0x280[2]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xFF));
                field_0x280[2]->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                               JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
                field_0x280[2]->setAlpha(200.0f * alpha);
                field_0x280[2]->draw(3.0f + field_0x190[i], 3.0f + field_0x1a0[i], 48.0f * field_0x1b0[i],
                                     48.0f * field_0x1b0[i], false, false, false);
                field_0x280[2]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xFF, 0xFF, 0x77, 0xFF));
                field_0x280[2]->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                               JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
                field_0x280[2]->setAlpha(field_0x24e[i]);
                field_0x280[2]->draw(field_0x190[i], field_0x1a0[i], 48.0f * field_0x1b0[i], 48.0f * field_0x1b0[i],
                                     false, false, false);
                break;
            case 4:
                field_0x280[3]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xFF));
                field_0x280[3]->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                               JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
                field_0x280[3]->setAlpha(200.0f * alpha);
                field_0x280[3]->draw(3.0f + field_0x190[i], 3.0f + field_0x1a0[i], 48.0f * field_0x1b0[i],
                                     48.0f * field_0x1b0[i], false, false, false);
                field_0x280[3]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xD8, 0xFF, 0xA1, 0xFF));
                field_0x280[3]->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                               JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF), JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
                field_0x280[3]->setAlpha(field_0x24e[i]);
                field_0x280[3]->draw(field_0x190[i], field_0x1a0[i], 48.0f * field_0x1b0[i], 48.0f * field_0x1b0[i],
                                     false, false, false);
                break;
            }
        }
        field_0x253 = 0;
    }
}

void cls_804B1920::fn_8043ABCC(s32 index, s32 unused) {
    index = index < 0 ? 0 : index;
    void* res;
    if (index != 1) {
        res = fn_804032E8('TIMG', lbl_804B1900[index]);
    } else {
        res = fn_804030C4('TIMG', lbl_804B1900[index]);
    }
    static_cast<J2DPicture*>(field_0x8->search('Pdf01'))->changeTexture((const ResTIMG*)res, 0);
    field_0x8->search('Pdf01')->show();
    field_0x8->search('Pw0101')->hide();
    field_0x106 = 1;
    field_0x10d = 0;
}

// FIXME: 99.5. Registers only: the original keeps delay and wait (both branches) in r31 and the
// three timings in r30/r29/r28 (ours: r28, r31, r30, r29); one shared s16 for delay and wait gives 99.87.
void cls_804B1920::fn_8043A638() {
    f32 dx = 0.0f;
    f32 dy = dx;
    s16 fadeIn = lbl_8050DB0C.field_0x4c[(0x22A - 0x4C) / 2];
    s16 hold = lbl_8050DB0C.field_0x4c[(0x22C - 0x4C) / 2];
    s16 fadeOut = lbl_8050DB0C.field_0x4c[(0x22E - 0x4C) / 2];
    if (field_0x106 == 0) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        lbl_8053AED0->field_0xdc |= (u8)(1 << i);
    }
    if (field_0x10d == 0) {
        s16 delay = lbl_8050DB0C.field_0x4c[(0x2E6 - 0x4C) / 2] + 80;
        s16 t = field_0x106;
        if (t < delay) {
            field_0x8->search('Pdf01')->setAlpha(0);
        } else if (t <= delay + fadeIn) {
            field_0x8->search('Pdf01')->setAlpha(255.0f * ((f32)(t - delay) / (f32)fadeIn));
        } else if (t > delay + fadeIn + hold) {
            field_0x8->search('Pdf01')->setAlpha(255.0f * ((f32)(delay + fadeIn + hold + fadeOut - t) / (f32)fadeOut));
        }
        field_0x8->search('Pw0101')->setAlpha(field_0x8->search('Pdf01')->getAlpha());
        if (field_0x106 < delay + fadeIn + hold + fadeOut) {
            field_0x106++;
        } else {
            field_0x106 = 0;
        }
        if (lbl_80529DEC.field_0x118 == 8 && lbl_80529DEC.field_0x11c == 3) {
            dy = 8.0f;
        }
        dx += lbl_8050DB0C.field_0x4c[(0x2E2 - 0x4C) / 2];
        dy += lbl_8050DB0C.field_0x4c[(0x2E4 - 0x4C) / 2];
        field_0x8->search('Pw0101')->move(field_0xec + dx, field_0xf0 + dy);
    } else {
        s16 t = field_0x106;
        s16 wait = field_0x108;
        if (t < 20) {
            field_0x8->search('Pdf01')->setAlpha(0);
        } else if (t <= fadeIn + 20) {
            field_0x8->search('Pdf01')->setAlpha(255.0f * ((f32)(t - 20) / (f32)fadeIn));
        } else if (t > wait + (fadeIn + 20)) {
            field_0x8->search('Pdf01')->setAlpha(255.0f * ((f32)(fadeOut + (wait + (fadeIn + 20)) - t) / (f32)fadeOut));
        }
        field_0x8->search('Pw0101')->setAlpha(field_0x8->search('Pdf01')->getAlpha());
        if (field_0x106 < 20 + fadeIn + wait + fadeOut) {
            field_0x106++;
        } else {
            field_0x106 = 0;
        }
        field_0x8->search('Pw0101')->move(field_0xf4, field_0xf8);
    }
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x8->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

void cls_804B1920::fn_8043A608() {
    fn_8043A450(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c);
}

void cls_804B1920::fn_8043A450(s32 stage, s32 sub) {
    if ((stage == 0 && sub == 2) || (stage == 1 && sub == 2) || (stage == 2 && sub == 2) ||
        (stage == 3 && sub == 2) || (stage == 4 && sub == 2) || (stage == 5 && sub == 2) ||
        (stage == 6 && sub == 2) || (stage == 7 && sub == 1)) {
        return;
    }
    if (gGameRoot_8053AAF8->field_0x7f != 0 && stage < 8 && sub < 4) {
        s32 index = sub + stage * 4;
        void* res;
        if (index != 0) {
            res = fn_804032E8('TIMG', lbl_804B1820[index]);
        } else {
            res = fn_804030C4('TIMG', lbl_804B1820[index]);
        }
        static_cast<J2DPicture*>(field_0x8->search('Pw0101'))->changeTexture((const ResTIMG*)res, 0);
        field_0x8->search('Pw0101')->show();
        field_0x8->search('Pdf01')->hide();
        field_0x106 = 1;
        field_0x10d = 0;
    }
}

void cls_804B1920::fn_8043A268(s32 stage, s32 sub, s16 value, f32 x, f32 y) {
    if ((stage == 0 && sub == 2) || (stage == 1 && sub == 2) || (stage == 2 && sub == 2) ||
        (stage == 3 && sub == 2) || (stage == 4 && sub == 2) || (stage == 5 && sub == 2) ||
        (stage == 6 && sub == 2) || (stage == 7 && sub == 1)) {
        return;
    }
    if (gGameRoot_8053AAF8->field_0x7f != 0 && stage < 8 && sub < 4) {
        s32 index = sub + stage * 4;
        void* res;
        if (index != 0) {
            res = fn_804032E8('TIMG', lbl_804B1820[index]);
        } else {
            res = fn_804030C4('TIMG', lbl_804B1820[index]);
        }
        static_cast<J2DPicture*>(field_0x8->search('Pw0101'))->changeTexture((const ResTIMG*)res, 0);
        field_0x8->search('Pw0101')->show();
        field_0x8->search('Pdf01')->hide();
        field_0x106 = 1;
        field_0x10d = 1;
        field_0xf4 = x;
        field_0xf8 = y;
        field_0x108 = value;
    }
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

void fn_8043A170(s32 player, bool force) {
    if (gGameRoot_8053AAF8->field_0x7f != 0 && (lbl_8050EC80.getCurrentFile().mFlags & 1) == 0 &&
        lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 0 && !force) {
        return;
    }
    if (lbl_80529DEC.field_0x118 == 10 && lbl_80529DEC.field_0x11c == 2) {
        return;
    }
    lbl_8053AED0->field_0xdd |= (u8)(1 << player);
}

void fn_8043A0E8(bool force) {
    if (gGameRoot_8053AAF8->field_0x7f != 0 && (lbl_8050EC80.getCurrentFile().mFlags & 1) == 0 &&
        lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 0 && !force) {
        return;
    }
    if (lbl_80529DEC.field_0x118 == 10 && lbl_80529DEC.field_0x11c == 2) {
        return;
    }
    lbl_8053AED0->field_0xdd = 0xF;
}
