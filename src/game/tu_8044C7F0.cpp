#include "game/GameAudio.h"
#include "game/cls_804B1D58.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/EmitterDelete.h"
#include "game/Unk_8045A080.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"

void* fn_80403084(u32 type, const char* name);

// The pane tag table of the unit (u64 tags, .rodata 0x80490FC8; 0x320 bytes): not written, only
// declared, as it is by the other functions' relocations.
extern const u64 lbl_80490FC8[];

// Effect ids of the second list's emitters (.sdata2, before the unit's float literals).
static const u16 lbl_80543DD8[4] = {0xD8, 0xD9, 0xDB, 0xDA};

void fn_80453884(J2DWindow* window, const JUtility::TColor* colors) {
    window->setContentsColor(colors[0], colors[1], colors[2], colors[3]);
}

void fn_80453874(JGeometry::TVec3f* vec, f32 x, f32 y, f32 z) {
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

bool cls_804B1D58::fn_80453848() {
    bool result = false;
    if (field_0xf4 & 8) {
        if (field_0xd0 + (s32)field_0xd8[1] == 0) {
            result = true;
        }
    }
    return result;
}

cls_80453830::cls_80453830() {}

// FIXME(shotgun): 99 %, reloc-name
cls_804B1D58::~cls_804B1D58() {
    gAudioMgr_8053AF80->fn_80459274();
    fn_8044C7F0();
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0xd8[i];
        field_0xd8[i] = NULL;
    }
    delete field_0xec;
    field_0xec = NULL;
    delete mPane_0xf4;
    mPane_0xf4 = NULL;
    delete mPane_0xd0;
    mPane_0xd0 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    for (s32 i = 0; i < 15; i++) {
        delete field_0xc[i];
        field_0xc[i] = NULL;
    }
    for (s32 i = 0; i < 3; i++) {
        delete field_0x48[i];
        field_0x48[i] = NULL;
        delete field_0x54[i];
        field_0x54[i] = NULL;
        delete field_0x60[i];
        field_0x60[i] = NULL;
    }
    lbl_8053AF40 = NULL;
}

// FIXME(shotgun): 88.89 %, missing/extra + reloc-name (the zero shared with the loop counter, lbl_80529DEC reloaded per test)
void cls_804B1D58::fn_80452350() {
    lbl_8053AEC0->fn_804310CC();
    s32 i = 0;
    field_0xf8 = i;
    field_0xfe = i;
    for (; i < 3; i++) {
        field_0x48[i]->moveFromCentre_80402E08(300.0f, 0.0f);
        field_0x60[i]->moveFromCentre_80402E08(300.0f, 0.0f);
        field_0x48[i]->field_0x20 = 0;
        field_0x60[i]->field_0x20 = 0;
        field_0x48[i]->mPane->setAlpha(0);
        field_0x48[i]->field_0x26 = 0;
        field_0x54[i]->mPane->setAlpha(0);
        field_0x54[i]->field_0x26 = 0;
        field_0x60[i]->mPane->setAlpha(0);
        field_0x60[i]->field_0x26 = 0;
    }
    mScreen->search('Pcul01')->move(300.0f + field_0x114, field_0x118 + field_0x108[field_0xf8]);
    mScreen->search('Pcul01')->setAlpha(0);
    field_0x132 = 0;
    field_0xfe = 0;
    field_0xfa = 2;
    field_0xfc = 0;
    field_0x8->mPane->setAlpha(0);
    field_0x8->field_0x26 = 0;
    for (s32 i = 0; i < 15; i++) {
        field_0xc[i]->moveFromCentre_80402E08(0.0f, -480.0f);
        field_0xc[i]->field_0x22 = 0;
    }
    const ResTIMG* image;
    bool useBack = false;
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        s32 mode = lbl_80529DEC.field_0x118;
        if ((mode == 0 && lbl_80529DEC.field_0x11c == 2) || (mode == 1 && lbl_80529DEC.field_0x11c == 2) ||
            (mode == 2 && lbl_80529DEC.field_0x11c == 2) || (mode == 3 && lbl_80529DEC.field_0x11c == 2) ||
            (mode == 4 && lbl_80529DEC.field_0x11c == 2) || (mode == 5 && lbl_80529DEC.field_0x11c == 2) ||
            (mode == 6 && lbl_80529DEC.field_0x11c == 2) || (mode == 7 && lbl_80529DEC.field_0x11c == 1)) {
            useBack = true;
        }
    }
    if (useBack) {
        image = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_pYback_01.bti"));
    } else {
        image = static_cast<const ResTIMG*>(fn_80403084('TIMG', "kk_pCselect_01.bti"));
    }
    static_cast<J2DPicture*>(mScreen->search('Psc01'))->changeTexture(image, 0);
    fn_8044D894();
    field_0x184 = 9;
}

// FIXME(shotgun): 99.72 %, reloc-name
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

// Selects row `arg` (0-3) of the second list; like fn_8044CB08, with one emitter (field_0xb0).
// FIXME(shotgun): 67 %, missing/extra + reloc-name
void cls_804B1D58::fn_8044D298(int arg) {
    if (arg >= 0 && arg <= 3) {
        gGameRoot_8053AAF8->fn_80134D20(false);
        field_0x140 = 1;
        field_0x137 = 1;
        field_0x134 = 0;
        field_0x138 = arg;
        for (s32 i = 0; i < 3; i++) {
            JUtility::TColor black(100, 100, 100, field_0x15c[0].a);
            JUtility::TColor white(0, 0, 0, field_0x154[0].a);
            static_cast<J2DPicture*>(mScreen->search(lbl_80490FC8[i]))->setBlackWhite(black, white);
            JUtility::TColor black2(80, 80, 80, field_0x14c[0].a);
            JUtility::TColor white2(80, 80, 80, field_0x144[0].a);
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FC8[3 + i]))->setBlackWhite(black2, white2);
            JUtility::TColor c3(150, 150, 150, field_0x164[0].field_0xc.a);
            JUtility::TColor c2(150, 150, 150, field_0x164[0].field_0x8.a);
            JUtility::TColor c1(150, 150, 150, field_0x164[0].field_0x4.a);
            JUtility::TColor c0(150, 150, 150, field_0x164[0].field_0x0.a);
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FC8[3 + i]))->setContentsColor(c0, c1, c2, c3);
            mScreen->search(lbl_80490FC8[3 + i])->setAlpha(200);
        }
        mScreen->search(lbl_80490FC8[73 + field_0x138])->mIsVisible = true;
        mScreen->search(lbl_80490FC8[77 + field_0x138])->setAlpha(0);
        mScreen->search(lbl_80490FC8[81 + field_0x138])->setAlpha(0);
        if (field_0xb0[field_0x138] == NULL) {
            f32 h = mScreen->search(lbl_80490FC8[77 + field_0x138])->mBounds.f.y -
                    mScreen->search(lbl_80490FC8[77 + field_0x138])->mBounds.i.y;
            f32 y0 = mScreen->search(lbl_80490FC8[73 + field_0x138])->getBounds()->i.y;
            f32 y1 = mScreen->search('Ndataj01')->getBounds()->i.y;
            f32 w = mScreen->search(lbl_80490FC8[77 + field_0x138])->mBounds.f.x -
                    mScreen->search(lbl_80490FC8[77 + field_0x138])->mBounds.i.x;
            f32 x0 = mScreen->search(lbl_80490FC8[73 + field_0x138])->getBounds()->i.x;
            f32 x1 = mScreen->search('Ndataj01')->getBounds()->i.x;
            JGeometry::TVec3f pos(0.5f * w + (x1 + x0), 0.5f * h + (y1 + y0), 0.0f);
            field_0xb0[field_0x138] = spawnEmitter_8013CC50(0, &pos, lbl_80543DD8[field_0x138], 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0xb0[field_0x138]->setStatus(1);
        field_0xb0[field_0x138]->setStatus(2);
        field_0xb0[field_0x138]->setStatus(4);
        mScreen->search('Pcul01')->mIsVisible = false;
    }
}

u8 cls_804B1D58::fn_8044D290() {
    return field_0x140;
}

// Selects row `arg` (0-6): recolours the cursor panes, hides the row's panes and starts its two
// emitters (stopped at once: the status bits 1, 2 and 4).
// FIXME(shotgun): 70 %, missing/extra + reloc-name
void cls_804B1D58::fn_8044CB08(int arg) {
    static const u16 ids0[8] = {0xDC, 0xE6, 0xE0, 0xDE, 0xE4, 0xE2, 0xE8, 0};
    static const u16 ids1[8] = {0xDD, 0xE7, 0xE1, 0xDF, 0xE5, 0xE3, 0xE9, 0};
    if (arg >= 0 && arg <= 6) {
        gGameRoot_8053AAF8->fn_80134D20(false);
        field_0x140 = 1;
        field_0x137 = 2;
        field_0x134 = 0;
        field_0x138 = arg;
        for (s32 i = 0; i < 3; i++) {
            JUtility::TColor black(100, 100, 100, field_0x15c[0].a);
            JUtility::TColor white(0, 0, 0, field_0x154[0].a);
            static_cast<J2DPicture*>(mScreen->search(lbl_80490FC8[i]))->setBlackWhite(black, white);
            JUtility::TColor black2(80, 80, 80, field_0x14c[0].a);
            JUtility::TColor white2(80, 80, 80, field_0x144[0].a);
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FC8[3 + i]))->setBlackWhite(black2, white2);
            JUtility::TColor c3(150, 150, 150, field_0x164[0].field_0xc.a);
            JUtility::TColor c2(150, 150, 150, field_0x164[0].field_0x8.a);
            JUtility::TColor c1(150, 150, 150, field_0x164[0].field_0x4.a);
            JUtility::TColor c0(150, 150, 150, field_0x164[0].field_0x0.a);
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FC8[3 + i]))->setContentsColor(c0, c1, c2, c3);
            mScreen->search(lbl_80490FC8[3 + i])->setAlpha(200);
        }
        mScreen->search(lbl_80490FC8[45 + field_0x138])->mIsVisible = false;
        mScreen->search(lbl_80490FC8[66 + field_0x138])->mIsVisible = false;
        mScreen->search(lbl_80490FC8[52 + field_0x138])->setAlpha(0);
        mScreen->search(lbl_80490FC8[59 + field_0x138])->setAlpha(0);
        if (field_0x6c[field_0x138] == NULL) {
            f32 h = mScreen->search(lbl_80490FC8[66 + field_0x138])->mBounds.f.y -
                    mScreen->search(lbl_80490FC8[66 + field_0x138])->mBounds.i.y;
            f32 y0 = mScreen->search(lbl_80490FC8[66 + field_0x138])->getBounds()->i.y;
            f32 y1 = mScreen->search('Ndata06')->getBounds()->i.y;
            f32 w = mScreen->search(lbl_80490FC8[66 + field_0x138])->mBounds.f.x -
                    mScreen->search(lbl_80490FC8[66 + field_0x138])->mBounds.i.x;
            f32 x0 = mScreen->search(lbl_80490FC8[66 + field_0x138])->getBounds()->i.x;
            f32 x1 = mScreen->search('Ndata06')->getBounds()->i.x;
            JGeometry::TVec3f pos(0.5f * w + (x1 + x0), 0.5f * h + (y1 + y0), 0.0f);
            field_0x6c[field_0x138] = spawnEmitter_8013CC50(0, &pos, ids0[field_0x138], 0, 0, NULL, NULL, true, 1.0f);
        }
        if (field_0x88[field_0x138] == NULL) {
            f32 h = mScreen->search(lbl_80490FC8[66 + field_0x138])->mBounds.f.y -
                    mScreen->search(lbl_80490FC8[66 + field_0x138])->mBounds.i.y;
            f32 y0 = mScreen->search(lbl_80490FC8[66 + field_0x138])->getBounds()->i.y;
            f32 y1 = mScreen->search('Ndata06')->getBounds()->i.y;
            f32 w = mScreen->search(lbl_80490FC8[66 + field_0x138])->mBounds.f.x -
                    mScreen->search(lbl_80490FC8[66 + field_0x138])->mBounds.i.x;
            f32 x0 = mScreen->search(lbl_80490FC8[66 + field_0x138])->getBounds()->i.x;
            f32 x1 = mScreen->search('Ndata06')->getBounds()->i.x;
            JGeometry::TVec3f pos(0.5f * w + (x1 + x0), 0.5f * h + (y1 + y0), 0.0f);
            field_0x88[field_0x138] = spawnEmitter_8013CC50(0, &pos, ids1[field_0x138], 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x6c[field_0x138]->setStatus(1);
        field_0x6c[field_0x138]->setStatus(2);
        field_0x6c[field_0x138]->setStatus(4);
        field_0x88[field_0x138]->setStatus(1);
        field_0x88[field_0x138]->setStatus(2);
        field_0x88[field_0x138]->setStatus(4);
        mScreen->search('Pcul01')->mIsVisible = false;
    }
}

u8 cls_804B1D58::fn_8044CB00() {
    return field_0x140;
}

// FIXME(shotgun): 99.75 %, reloc-name
void cls_804B1D58::fn_8044C968() {
    for (s32 i = 0; i < 3; i++) {
        field_0x48[i]->moveFromCentre_80402E08(300.0f, 0.0f);
        field_0x60[i]->moveFromCentre_80402E08(300.0f, 0.0f);
        field_0x48[i]->field_0x20 = 0;
        field_0x60[i]->field_0x20 = 0;
        field_0x48[i]->mPane->setAlpha(0);
        field_0x48[i]->field_0x26 = 0;
        field_0x54[i]->mPane->setAlpha(0);
        field_0x54[i]->field_0x26 = 0;
        field_0x60[i]->mPane->setAlpha(0);
        field_0x60[i]->field_0x26 = 0;
    }
    mScreen->search('Pcul01')->move(300.0f + field_0x114, field_0x118 + field_0x108[field_0xf8]);
    mScreen->search('Pcul01')->setAlpha(0);
    field_0x132 = 0;
    field_0xfe = 0;
    field_0xfa = 2;
}

// Stops the emitters of the three arrays and deletes the two others (the destructor inlines it).
// FIXME(shotgun): 100 %, matches
void cls_804B1D58::fn_8044C7F0() {
    for (s32 i = 0; i < 7; i++) {
        if (field_0x6c[i] != NULL) {
            field_0x6c[i]->becomeInvalidEmitter();
            field_0x6c[i] = NULL;
        }
        if (field_0x88[i] != NULL) {
            field_0x88[i]->becomeInvalidEmitter();
            field_0x88[i] = NULL;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (field_0xb0[i] != NULL) {
            field_0xb0[i]->becomeInvalidEmitter();
            field_0xb0[i] = NULL;
        }
    }
    for (s32 i = 0; i < 3; i++) {
        deleteEmitter(&field_0xa4[i]);
        deleteEmitter(&field_0xc0[i]);
    }
}
