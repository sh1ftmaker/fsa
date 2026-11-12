#include "game/GameAudio.h"
#include "game/cls_804B1D58.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/EmitterDelete.h"
#include "game/Unk_8045A080.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DAnmLoader.h"
#include "dolphin/os/OSFastCast.h"

void* fn_80403084(u32 type, const char* name);

// The pane tag tables of the unit (.rodata 0x80490FC8..0x804912E8, back to back).
static const u64 lbl_80490FC8[3] = {'Pcntn01', 'Psc01', 'Psg01'};
static const u64 lbl_80490FE0[3] = {'Wcntn01', 'Wsc01', 'Wsg01'};
static const u64 lbl_80490FF8[3] = {'Pcntnk01', 'Psck01', 'Psgk01'};
static const u64 lbl_80491010[6] = {'Ps01', 'Ps02', 'Ps03', 'Ps04', 'Ps05', 'Ps06'};
static const u64 lbl_80491040[6] = {'Psk01', 'Psk02', 'Psk03', 'Psk04', 'Psk05', 'Psk06'};
static const u64 lbl_80491070[6] = {'Prs01', 'Prs02', 'Prs03', 'Prs04', 'Prs05', 'Prs06'};
static const u64 lbl_804910A0[6] = {'Prsk01', 'Prsk02', 'Prsk03', 'Prsk04', 'Prsk05', 'Prsk06'};
static const u64 lbl_804910D0[6] = {'Pts01', 'Pts02', 'Pts03', 'Pts04', 'Pts05', 'Pts06'};
static const u64 lbl_80491100[6] = {'Ptsk01', 'Ptsk02', 'Ptsk03', 'Ptsk04', 'Ptsk05', 'Ptsk06'};
static const u64 lbl_80491130[7] = {'Padhu12', 'Padhu22', 'Padhu32', 'Padhu42', 'Padhu52', 'Padhu62', 'Padhu72'};
static const u64 lbl_80491168[7] = {'Padvw11', 'Padvw21', 'Padvw31', 'Padvw41', 'Padvw51', 'Padvw61', 'Padvw71'};
static const u64 lbl_804911A0[7] = {'Padvw12', 'Padvw22', 'Padvw32', 'Padvw42', 'Padvw52', 'Padvw62', 'Padvw72'};
static const u64 lbl_804911D8[7] = {'Phu01', 'Phu02', 'Phu03', 'Phu04', 'Phu05', 'Phu06', 'Phu07'};
static const u64 lbl_80491210[4] = {'Pjb02', 'Pjg02', 'Pjr02', 'Pjp02'};
static const u64 lbl_80491230[4] = {'Pjb01', 'Pjg01', 'Pjr01', 'Pjp01'};
static const u64 lbl_80491250[4] = {'Pjb03', 'Pjg03', 'Pjr03', 'Pjp03'};
static const u64 lbl_80491270[15] = {'Ndata01', 'Pswb01', 'Ndata09', 'Ndata07', 'Ndata04', 'Ndata06', 'Ndatal01', 'Psw02', 'Pfab01', 'Ndata08', 'Ndata03', 'Ndataj02', 'Ndataj01', 'Ndatat01', 'Ndata02'};

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

cls_804B1D58::cls_804B1D58() {
    lbl_8053AF40 = this;
    mScreen = new J2DScreen;
    mScreen->setPriority("collect.blo", 0x1100000, lbl_80529DEC.field_0x4c);
    OSInitFastCast();
    for (s32 i = 0; i < 4; i++) {
        field_0xd4 = fn_80403084('SCRN', "collect.btk");
        field_0xd8[i] = static_cast<J2DAnmTextureSRTKey*>(J2DAnmLoaderDataBase::load(field_0xd4));
        field_0xd8[i]->searchUpdateMaterialID(mScreen);
        mScreen->search(lbl_80491250[i])->setAnimation(field_0xd8[i]);
    }
    field_0x124 = 0;
    field_0xe8 = fn_80403084('SCRN', "collect_02.btk");
    field_0xec = static_cast<J2DAnmTextureSRTKey*>(J2DAnmLoaderDataBase::load(field_0xe8));
    field_0xec->searchUpdateMaterialID(mScreen);
    mScreen->search('Ppo01')->setAnimation(field_0xec);
    field_0x128 = 0;
    field_0xf0 = fn_80403084('SCRN', "collect_03.btk");
    mAnm_0xf4 = static_cast<J2DAnmTextureSRTKey*>(J2DAnmLoaderDataBase::load(field_0xf0));
    mAnm_0xf4->searchUpdateMaterialID(mScreen);
    mScreen->search('Ppo02')->setAnimation(mAnm_0xf4);
    field_0x12c = 0;
    field_0x13e = 0;
    field_0xcc = fn_80403084('SCRN', "collect.brk");
    mAnm_0xd0 = static_cast<J2DAnmTevRegKey*>(J2DAnmLoaderDataBase::load(field_0xcc));
    mScreen->setAnimation(mAnm_0xd0);
    field_0x130 = 0;
    field_0x132 = 0;
    field_0x114 = mScreen->search('Pcul01')->getBounds()->i.x;
    field_0x118 = mScreen->search('Pcul01')->getBounds()->i.y;
    for (s32 i = 0; i < 3; i++) {
        field_0x108[i] = mScreen->search(lbl_80490FC8[i])->getBounds()->i.y -
                         mScreen->search(lbl_80490FC8[0])->getBounds()->i.y;
    }
    field_0x13b = mScreen->search(lbl_80491210[0])->getAlpha();
    field_0x13c = mScreen->search(lbl_80491230[0])->getAlpha();
    field_0x13d = mScreen->search(lbl_80491250[0])->getAlpha();
    field_0x139 = mScreen->search(lbl_80491168[0])->getAlpha();
    field_0x13a = mScreen->search(lbl_804911A0[0])->getAlpha();
    for (s32 i = 0; i < 7; i++) {
        mScreen->search(lbl_80491168[i])->hide();
        mScreen->search(lbl_804911A0[i])->hide();
    }
    for (s32 i = 0; i < 2; i++) {
        field_0x154[i] = static_cast<J2DPicture*>(mScreen->search(lbl_80490FC8[i]))->getBlack();
        field_0x15c[i] = static_cast<J2DPicture*>(mScreen->search(lbl_80490FC8[i]))->getWhite();
        field_0x144[i] = static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]))->getBlack();
        field_0x14c[i] = static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]))->getWhite();
        J2DWindow* window = static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]));
        field_0x164[i].field_0x0 = window->mContentsColorA;
        field_0x164[i].field_0x4 = window->mContentsColorB;
        field_0x164[i].field_0x8 = window->mContentsColorC;
        field_0x164[i].field_0xc = window->mContentsColorD;
        field_0x15c[i].a = 255;
        field_0x154[i].a = 0;
        field_0x14c[i].a = 255;
        field_0x144[i].a = 255;
    }
    field_0x164[0].field_0xc.a = 255;
    field_0x164[0].field_0x8.a = 255;
    field_0x164[0].field_0x4.a = 255;
    field_0x164[0].field_0x0.a = 255;
    field_0x164[1].field_0xc.a = 200;
    field_0x164[1].field_0x8.a = 200;
    field_0x164[1].field_0x4.a = 200;
    field_0x164[1].field_0x0.a = 200;
    field_0x15c[0].r = 0xA7;
    field_0x15c[0].g = 0x4C;
    field_0x15c[0].b = 0x2A;
    field_0x154[0].r = 0x27;
    field_0x154[0].g = 0x10;
    field_0x154[0].b = 0;
    field_0x15c[1].r = 0xFF;
    field_0x15c[1].g = 0xED;
    field_0x15c[1].b = 0xA0;
    field_0x154[1].r = 0;
    field_0x154[1].g = 0;
    field_0x154[1].b = 0;
    field_0x14c[0].r = 0x7F;
    field_0x14c[0].g = 0x51;
    field_0x14c[0].b = 0x18;
    field_0x144[0].r = 0x7F;
    field_0x144[0].g = 0x51;
    field_0x144[0].b = 0x18;
    field_0x14c[1].r = 0x7F;
    field_0x14c[1].g = 0x51;
    field_0x14c[1].b = 0x18;
    field_0x144[1].r = 0x7F;
    field_0x144[1].g = 0x51;
    field_0x144[1].b = 0x18;
    field_0x164[0].field_0x0.r = 0xC0;
    field_0x164[0].field_0x0.g = 0x91;
    field_0x164[0].field_0x0.b = 0x4C;
    field_0x164[0].field_0x4.r = 0xC0;
    field_0x164[0].field_0x4.g = 0x91;
    field_0x164[0].field_0x4.b = 0x4C;
    field_0x164[0].field_0x8.r = 0xE6;
    field_0x164[0].field_0x8.g = 0xE1;
    field_0x164[0].field_0x8.b = 0x87;
    field_0x164[0].field_0xc.set(0xE6, 0xE1, 0x87, field_0x164[0].field_0xc.a);
    field_0x164[1].field_0x0.r = 0xA9;
    field_0x164[1].field_0x0.g = 0x80;
    field_0x164[1].field_0x0.b = 0x46;
    field_0x164[1].field_0x4.r = 0xA9;
    field_0x164[1].field_0x4.g = 0x80;
    field_0x164[1].field_0x4.b = 0x46;
    field_0x164[1].field_0x8.r = 0xA9;
    field_0x164[1].field_0x8.g = 0x80;
    field_0x164[1].field_0x8.b = 0x46;
    field_0x164[1].field_0xc.r = 0xA9;
    field_0x164[1].field_0xc.g = 0x80;
    field_0x164[1].field_0xc.b = 0x46;
    mScreen->search(lbl_80490FE0[1])->setAlpha(255);
    field_0x8 = new PaneCtrl_804AF86C;
    field_0x8->setPane_80402F04(mScreen->search('Pblack'));
    for (s32 i = 0; i < 15; i++) {
        field_0xc[i] = new PaneCtrl_804AF86C;
        field_0xc[i]->setPane_80402F04(mScreen->search(lbl_80491270[i]));
    }
    for (s32 i = 0; i < 3; i++) {
        field_0x48[i] = new PaneCtrl_804AF86C;
        field_0x48[i]->setPane_80402F04(mScreen->search(lbl_80490FC8[i]));
        field_0x54[i] = new PaneCtrl_804AF86C;
        field_0x54[i]->setPane_80402F04(mScreen->search(lbl_80490FE0[i]));
        field_0x60[i] = new PaneCtrl_804AF86C;
        field_0x60[i]->setPane_80402F04(mScreen->search(lbl_80490FF8[i]));
    }
    field_0xf8 = 0;
    field_0xfe = 0;
    field_0xfa = 2;
    field_0xfc = 0;
    field_0x8->mPane->setAlpha(0);
    field_0x8->field_0x26 = 0;
    for (s32 i = 0; i < 15; i++) {
        field_0xc[i]->moveFromCentre_80402E08(0.0f, -480.0f);
        field_0xc[i]->field_0x22 = 0;
    }
    for (s32 i = 0; i < 3; i++) {
        if (i == field_0xf8) {
            static_cast<J2DPicture*>(mScreen->search(lbl_80490FC8[i]))->setBlackWhite(field_0x154[0], field_0x15c[0]);
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]))->setBlackWhite(field_0x144[0], field_0x14c[0]);
            cls_80453830 colors = field_0x164[0];
            fn_80453884(static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i])), &colors.field_0x0);
            mScreen->search(lbl_80490FE0[i])->setAlpha(255);
        } else {
            static_cast<J2DPicture*>(mScreen->search(lbl_80490FC8[i]))->setBlackWhite(field_0x154[1], field_0x15c[1]);
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]))->setBlackWhite(field_0x144[1], field_0x14c[1]);
            cls_80453830 colors = field_0x164[1];
            fn_80453884(static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i])), &colors.field_0x0);
            mScreen->search(lbl_80490FE0[i])->setAlpha(200);
        }
    }
    field_0x136 = 0;
    field_0x13f = 1;
    field_0x140 = 0;
    field_0x120 = 0;
    field_0x137 = 0;
    field_0x134 = 0;
    field_0x138 = 0;
    for (s32 i = 0; i < 7; i++) {
        field_0x6c[i] = NULL;
        field_0x88[i] = NULL;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0xb0[i] = NULL;
    }
    for (s32 i = 0; i < 3; i++) {
        field_0xa4[i] = NULL;
        field_0xc0[i] = NULL;
    }
}

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
    delete mAnm_0xf4;
    mAnm_0xf4 = NULL;
    delete mAnm_0xd0;
    mAnm_0xd0 = NULL;
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
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]))->setBlackWhite(black2, white2);
            JUtility::TColor c3(150, 150, 150, field_0x164[0].field_0xc.a);
            JUtility::TColor c2(150, 150, 150, field_0x164[0].field_0x8.a);
            JUtility::TColor c1(150, 150, 150, field_0x164[0].field_0x4.a);
            JUtility::TColor c0(150, 150, 150, field_0x164[0].field_0x0.a);
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]))->setContentsColor(c0, c1, c2, c3);
            mScreen->search(lbl_80490FE0[i])->setAlpha(200);
        }
        mScreen->search(lbl_80491210[field_0x138])->mIsVisible = true;
        mScreen->search(lbl_80491230[field_0x138])->setAlpha(0);
        mScreen->search(lbl_80491250[field_0x138])->setAlpha(0);
        if (field_0xb0[field_0x138] == NULL) {
            f32 h = mScreen->search(lbl_80491230[field_0x138])->mBounds.f.y -
                    mScreen->search(lbl_80491230[field_0x138])->mBounds.i.y;
            f32 y0 = mScreen->search(lbl_80491210[field_0x138])->getBounds()->i.y;
            f32 y1 = mScreen->search('Ndataj01')->getBounds()->i.y;
            f32 w = mScreen->search(lbl_80491230[field_0x138])->mBounds.f.x -
                    mScreen->search(lbl_80491230[field_0x138])->mBounds.i.x;
            f32 x0 = mScreen->search(lbl_80491210[field_0x138])->getBounds()->i.x;
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
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]))->setBlackWhite(black2, white2);
            JUtility::TColor c3(150, 150, 150, field_0x164[0].field_0xc.a);
            JUtility::TColor c2(150, 150, 150, field_0x164[0].field_0x8.a);
            JUtility::TColor c1(150, 150, 150, field_0x164[0].field_0x4.a);
            JUtility::TColor c0(150, 150, 150, field_0x164[0].field_0x0.a);
            static_cast<J2DWindow*>(mScreen->search(lbl_80490FE0[i]))->setContentsColor(c0, c1, c2, c3);
            mScreen->search(lbl_80490FE0[i])->setAlpha(200);
        }
        mScreen->search(lbl_80491130[field_0x138])->mIsVisible = false;
        mScreen->search(lbl_804911D8[field_0x138])->mIsVisible = false;
        mScreen->search(lbl_80491168[field_0x138])->setAlpha(0);
        mScreen->search(lbl_804911A0[field_0x138])->setAlpha(0);
        if (field_0x6c[field_0x138] == NULL) {
            f32 h = mScreen->search(lbl_804911D8[field_0x138])->mBounds.f.y -
                    mScreen->search(lbl_804911D8[field_0x138])->mBounds.i.y;
            f32 y0 = mScreen->search(lbl_804911D8[field_0x138])->getBounds()->i.y;
            f32 y1 = mScreen->search('Ndata06')->getBounds()->i.y;
            f32 w = mScreen->search(lbl_804911D8[field_0x138])->mBounds.f.x -
                    mScreen->search(lbl_804911D8[field_0x138])->mBounds.i.x;
            f32 x0 = mScreen->search(lbl_804911D8[field_0x138])->getBounds()->i.x;
            f32 x1 = mScreen->search('Ndata06')->getBounds()->i.x;
            JGeometry::TVec3f pos(0.5f * w + (x1 + x0), 0.5f * h + (y1 + y0), 0.0f);
            field_0x6c[field_0x138] = spawnEmitter_8013CC50(0, &pos, ids0[field_0x138], 0, 0, NULL, NULL, true, 1.0f);
        }
        if (field_0x88[field_0x138] == NULL) {
            f32 h = mScreen->search(lbl_804911D8[field_0x138])->mBounds.f.y -
                    mScreen->search(lbl_804911D8[field_0x138])->mBounds.i.y;
            f32 y0 = mScreen->search(lbl_804911D8[field_0x138])->getBounds()->i.y;
            f32 y1 = mScreen->search('Ndata06')->getBounds()->i.y;
            f32 w = mScreen->search(lbl_804911D8[field_0x138])->mBounds.f.x -
                    mScreen->search(lbl_804911D8[field_0x138])->mBounds.i.x;
            f32 x0 = mScreen->search(lbl_804911D8[field_0x138])->getBounds()->i.x;
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
