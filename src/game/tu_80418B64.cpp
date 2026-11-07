#include "game/GameAudio.h"
#include "game/cls_804B13D4.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "game/act/tu_80441318.h"
#include "game/GameGlobals.h"
#include "game/tu_803B8C74.h"
#include "game/Camera.h"
#include "game/Message.h"

// J2DPane::search tags, as u64 (the names are padded with leading zeros).
// The tables are defined in address order.

// The heart panes of player 0 on the main screen: "Php1lf01".., "Php1lk01".., "Php1lf09".., "Php1lk09".. (fn_8041DD24).
// FIXME(shotgun): table not written out (more literals than the gate allows without a
// blobs.txt array line); it sits inside dtk's merged .rodata object lbl_8048D454.
extern const u64 lbl_8048D468[4][8];
extern const u64 lbl_8048D568[12];
extern const u64 lbl_8048D5C8[12];

static const u64 lbl_8048D628[3] = {
    0x5068317069696231ull, /* "Ph1piib1" */
    0x5068317069696232ull, /* "Ph1piib2" */
    0x5068317069696233ull, /* "Ph1piib3" */
};

static const u64 lbl_8048D640[3] = {
    0x5068316C65323031ull, /* "Ph1le201" */
    0x5068316C65323032ull, /* "Ph1le202" */
    0x5068316C65323033ull, /* "Ph1le203" */
};

// "Ph1ps01" .. "Ph1ps05" and "Ph1psk01" .. "Ph1psk05": the Force Gem digits (fn_8041CC04).
static const u64 lbl_8048D658[5] = {
    0x0050683170733031ull,
    0x0050683170733032ull,
    0x0050683170733033ull,
    0x0050683170733034ull,
    0x0050683170733035ull,
};
static const u64 lbl_8048D680[5] = {
    0x50683170736B3031ull,
    0x50683170736B3032ull,
    0x50683170736B3033ull,
    0x50683170736B3034ull,
    0x50683170736B3035ull,
};

static const u64 lbl_8048D6A8[2] = {
    0x0050683170723031ull, /* "Ph1pr01" */
    0x0050683170723032ull, /* "Ph1pr02" */
};

// "Phbrak01", "Phbra01", "Phbrak02", "Phbra02" (fn_8041FC2C).
static const u64 lbl_8048D6B8[4] = {
    0x50686272616B3031ull,
    0x0050686272613031ull,
    0x50686272616B3032ull,
    0x0050686272613032ull,
};

// "P1s01" .. "P4r06" and "P1sk01" .. "P4rk06": the Force Gem digits per player (fn_8041C0C0;
// player 4 has the r tags).
// FIXME(shotgun): table not written out (more literals than the gate allows without a
// blobs.txt array line); it sits inside dtk's merged .rodata object lbl_8048D454.
extern const u64 lbl_8048D6D8[4][5];
// FIXME(shotgun): table not written out (more literals than the gate allows without a
// blobs.txt array line); it sits inside dtk's merged .rodata object lbl_8048D454.
extern const u64 lbl_8048D778[4][5];
extern const u64 lbl_8048D818[4][2];
extern const u64 lbl_8048D858[4][12];
extern const u64 lbl_8048D9D8[4][12];
extern const u64 lbl_8048DB58[4][3];
extern const u64 lbl_8048DBB8[4][3];
extern const u64 lbl_8048DC18[4][6];

static const u64 lbl_8048DCD8[4] = {
    0x5070316E6F693032ull, /* "Pp1noi02" */
    0x5070316E6F693033ull, /* "Pp1noi03" */
    0x5070336E6F693032ull, /* "Pp3noi02" */
    0x5070346E6F693032ull, /* "Pp4noi02" */
};

// FIXME(shotgun): the tags "P1brak01", "P1bra01", "P1brak02", "P1bra02" .. "P4bra02" inside
// dtk's merged .rodata object lbl_8048D454 (at 0x8048DCF8); not split out yet.
extern const u64 lbl_8048DCF8[4][4];

static const u64 lbl_8048DD78[3] = {
    0x506870316D623033ull, /* "Php1mb03" */
    0x506870316D623031ull, /* "Php1mb01" */
    0x506870316D623031ull, /* "Php1mb01" */
};

static const u64 lbl_8048DD90[4] = {
    0x00506870316D3034ull, /* "Php1m04" */
    0x00506870316D3033ull, /* "Php1m03" */
    0x00506870316D3032ull, /* "Php1m02" */
    0x00506870316D3031ull, /* "Php1m01" */
};

static const u64 lbl_8048DDB0[4] = {
    0x506870316E6D3034ull, /* "Php1nm04" */
    0x506870316E6D3033ull, /* "Php1nm03" */
    0x506870316E6D3032ull, /* "Php1nm02" */
    0x506870316E6D3031ull, /* "Php1nm01" */
};

static const u64 lbl_8048DDD0[6] = {
    0x00000050746B3031ull, /* "Ptk01" */
    0x0000000050743031ull, /* "Pt01" */
    0x00000050746B3032ull, /* "Ptk02" */
    0x0000000050743032ull, /* "Pt02" */
    0x00005074696D3031ull, /* "Ptim01" */
    0x00005074696D3032ull, /* "Ptim02" */
};

// "P11st02" .. "P41st02".
static const u64 lbl_8048DE00[4] = {
    0x0050313173743032ull,
    0x0050323173743032ull,
    0x0050333173743032ull,
    0x0050343173743032ull,
};

// FIXME(shotgun): pane-tag tables of this unit's .rodata (0x8048DE20, 0x398 bytes) not
// written out yet; fn_80418D08 reads four 8-entry tables at [0x24], [0x2C], [0x34], [0x3C].
extern const u64 lbl_8048DE20[0x398 / 8];
extern const f32 lbl_8048E020[4][5];
extern const u8 lbl_8048E070[4][10];
extern const s16 lbl_8048E0B8[2][5];
extern const s16 lbl_8048E0CC[2][5];
extern const s16 lbl_8048E0E0[2][5];
extern const s16 lbl_8048E0F4[2][5];
extern const s16 lbl_8048E108[2][5];
extern const s16 lbl_8048E11C[2][5];
extern const s16 lbl_8048E130[2][5];
extern const s16 lbl_8048E144[2][5];

const char* lbl_804B1390[3] = {
    "kk_heart_01.bti",
    "kk_heart_02.bti",
    "kk_heart_03.bti",
};

// The digit textures fn_8041918C passes to fn_804030C4 ('TIMG').
const char* lbl_804B139C[10] = {
    "kk_rupyNum_00.bti",
    "kk_rupyNum_01.bti",
    "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti",
    "kk_rupyNum_04.bti",
    "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti",
    "kk_rupyNum_07.bti",
    "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};

// The rank pictures; fn_8041C67C only uses the first.
const char* lbl_804B13C4[4] = {
    "kk_mainRank_01.bti",
    "kk_mainRank_02.bti",
    "kk_mainRank_03.bti",
    "kk_mainRank_04.bti",
};

// FIXME(shotgun): 87.83 %, missing/extra + reloc-name
cls_804B13D4::cls_804B13D4() : field_0x244(0xFFFFFFFF), field_0x248(0xFFFFFFFF) {
    if (lbl_80529DEC.field_0x118 != 0 || lbl_80529DEC.field_0x11c != 0) {
        lbl_8053AED0->field_0x10e |= 1;
    }
    if (lbl_80529DEC.field_0x118 == 6 && lbl_80529DEC.field_0x11c == 2) {
        field_0x291 = 1;
    } else {
        field_0x291 = 1;
    }
    field_0x292 = 0;
    field_0x28f = 0;
    field_0x290 = 0;
    field_0x293 = 0;
    field_0x294[0] = 0;
    field_0x294[1] = 0;
    field_0x294[2] = 0;
    field_0x294[3] = 0;
    field_0x218 = 1;
    field_0x298 = 0;
    if (gGameRoot_8053AAF8->fn_80138A30() >= 2) {
        field_0x50 = new cls_804B19E0();
    } else {
        field_0x50 = NULL;
    }
    field_0x4c = new J2DOrthoGraph(0.0f, 0.0f, (f32)JUTVideo::getManager()->getFbWidth(),
                                   (f32)JUTVideo::getManager()->getEfbHeight(), -10000.0f, 10000.0f);
    field_0x4 = new J2DScreen();
    field_0x4->setPriority("main_adv.blo", 0x1100000, lbl_80529DEC.field_0x4c);
    for (s32 i = 0; i < 4; i++) {
        field_0x8[i] = new J2DScreen();
        field_0x8[i]->setPriority("main_hubuki.blo", 0x1100000, lbl_80529DEC.field_0x4c);
        field_0x2c[i] = new J2DScreen();
        field_0x2c[i]->setPriority("main_diving.blo", 0x1100000, lbl_80529DEC.field_0x20);
    }
    field_0x18 = new J2DScreen();
    field_0x18->setPriority("main_adv_ranking.blo", 0x1100000, lbl_80529DEC.field_0x4c);
    field_0x4->search('Pp1li01')->hide();
    field_0x4->search('Pp2li01')->hide();
    field_0x4->search('Pp3li01')->hide();
    field_0x4->search('Pp4li01')->hide();
    field_0x24c[0] = field_0x4->search(lbl_8048D6A8[0])->getAlpha();
    field_0x24c[1] = field_0x4->search(lbl_8048D6A8[1])->getAlpha();
    field_0x24c[2] = field_0x4->search(lbl_8048D658[0])->getAlpha();
    field_0x24c[3] = field_0x4->search(lbl_8048D680[0])->getAlpha();
    field_0x25e = field_0x4->search(lbl_8048D568[0])->getAlpha();
    field_0x25f = field_0x4->search(lbl_8048D5C8[0])->getAlpha();
    for (s32 i = 0; i < 3; i++) {
        field_0x258[i] = field_0x4->search(lbl_8048D628[i])->getAlpha();
        field_0x25b[i] = field_0x4->search(lbl_8048D640[i])->getAlpha();
    }
    field_0x260[0] = field_0x4->search(lbl_8048DC18[0][0])->getAlpha();
    field_0x260[1] = field_0x4->search(lbl_8048DC18[0][1])->getAlpha();
    field_0x260[2] = field_0x4->search(lbl_8048DC18[0][2])->getAlpha();
    field_0x260[3] = field_0x4->search(lbl_8048DC18[0][3])->getAlpha();
    field_0x260[4] = field_0x4->search(lbl_8048DC18[0][4])->getAlpha();
    field_0x260[5] = field_0x4->search(lbl_8048DC18[0][5])->getAlpha();
    field_0x266 = field_0x4->search(lbl_8048DE20[0x8])->getAlpha();
    field_0x267 = field_0x18->search(lbl_8048DE20[0x4])->getAlpha();
    field_0x250[0] = field_0x4->search(lbl_8048D468[0][0])->getAlpha();
    field_0x250[1] = field_0x4->search(lbl_8048D468[1][0])->getAlpha();
    field_0x250[2] = field_0x4->search(lbl_8048D468[2][0])->getAlpha();
    field_0x250[3] = field_0x4->search(lbl_8048D468[3][0])->getAlpha();
    field_0x26a = field_0x4->search('Php1ni02')->getAlpha();
    field_0x26b = field_0x4->search('Php1ni01')->getAlpha();
    field_0x26c[0] = field_0x4->search(lbl_8048D6B8[0])->getAlpha();
    field_0x26c[1] = field_0x4->search(lbl_8048D6B8[1])->getAlpha();
    field_0x26c[2] = field_0x4->search(lbl_8048D6B8[2])->getAlpha();
    field_0x26c[3] = field_0x4->search(lbl_8048D6B8[3])->getAlpha();
    field_0x268 = field_0x18->search(lbl_8048DE00[0])->getAlpha();
    field_0x269 = field_0x18->search(lbl_8048DE20[0x0])->getAlpha();
    for (s32 i = 0; i < 6; i++) {
        field_0x27b[i] = field_0x2c[0]->search(lbl_8048DDD0[i])->getAlpha();
    }
    field_0x248 = static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[0]))->getBlack();
    field_0x244 = static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[0]))->getWhite();
    field_0x54 = new PaneCtrl_804AF86C();
    field_0x54->setPane_80402F04(field_0x4->search('Ndata13'));
    field_0x58 = new PaneCtrl_804AF86C();
    field_0x58->setPane_80402F04(field_0x4->search('Php1ni02'));
    field_0x5c = new PaneCtrl_804AF86C();
    field_0x5c->setPane_80402F04(field_0x4->search('Ph1piib3'));
    field_0x6c = new PaneCtrl_804AF86C();
    field_0x6c->setPane_80402F04(field_0x4->search('Ndata16'));
    field_0x70 = new PaneCtrl_804AF86C();
    field_0x70->setPane_80402F04(field_0x4->search('Ndata02'));
    for (s32 i = 0; i < 4; i++) {
        field_0x74[i] = new PaneCtrl_804AF86C();
        field_0x74[i]->setPane_80402F04(field_0x18->search(lbl_8048DE00[i]));
        field_0x84[i] = new PaneCtrl_804AF86C();
        field_0x84[i]->setPane_80402F04(field_0x18->search(lbl_8048DE20[0x4 + i]));
        field_0x94[i] = new PaneCtrl_804AF86C();
        field_0x94[i]->setPane_80402F04(field_0x4->search(lbl_8048DE20[0x8 + i]));
        field_0xa4[i] = new PaneCtrl_804AF86C();
        field_0xa4[i]->setPane_80402F04(field_0x4->search(lbl_8048DE20[0xC + i]));
        field_0xb4[i] = new PaneCtrl_804AF86C();
        field_0xb4[i]->setPane_80402F04(field_0x4->search(lbl_8048DE20[0x10 + i]));
        field_0xc4[i] = new PaneCtrl_804AF86C();
        field_0xc4[i]->setPane_80402F04(field_0x4->search(lbl_8048DE20[0x18 + i]));
        field_0xd4[i] = new PaneCtrl_804AF86C();
        field_0xd4[i]->setPane_80402F04(field_0x4->search(lbl_8048DE20[0x1C + i]));
        field_0xe4[i] = new PaneCtrl_804AF86C();
        field_0xe4[i]->setPane_80402F04(field_0x4->search(lbl_8048DE20[0x20 + i]));
    }
    if (gGameRoot_8053AAF8->field_0x7e == 0) {
        for (s32 i = 0; i < 8; i++) {
            for (s32 j = 0; j < 3; j++) {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1390[j]));
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[0][i]))->append(res, 1.0f);
                static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[1][i]))->append(res, 1.0f);
            }
        }
    } else {
        for (s32 i = 0; i < 4; i++) {
            for (s32 j = 1; j < 4; j++) {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1390[j]));
                static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE00[i]))->append(res, 1.0f);
                static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))->append(res, 1.0f);
            }
        }
    }
    if (field_0x291 != 0) {
        for (s32 p = 0; p < 4; p++) {
            for (s32 i = 0; i < 8; i++) {
                for (s32 j = 0; j < 3; j++) {
                    const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1390[j]));
                    static_cast<J2DPicture*>(field_0x8[p]->search(lbl_8048DE20[0x24 + i]))->append(res, 1.0f);
                    static_cast<J2DPicture*>(field_0x8[p]->search(lbl_8048DE20[0x34 + i]))->append(res, 1.0f);
                }
            }
        }
    }
}

cls_804B13D4::~cls_804B13D4() {
    if (field_0x50 != NULL) {
        delete field_0x50;
        field_0x50 = NULL;
    }
    delete field_0x4c;
    field_0x4c = NULL;
    delete field_0x4;
    field_0x4 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x8[i];
        field_0x8[i] = NULL;
        delete field_0x2c[i];
        field_0x2c[i] = NULL;
    }
    delete field_0x18;
    field_0x18 = NULL;
    delete field_0x54;
    field_0x54 = NULL;
    delete field_0x58;
    field_0x58 = NULL;
    delete field_0x5c;
    field_0x5c = NULL;
    delete field_0x6c;
    field_0x6c = NULL;
    delete field_0x70;
    field_0x70 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x74[i];
        field_0x74[i] = NULL;
        delete field_0x84[i];
        field_0x84[i] = NULL;
        delete field_0x94[i];
        field_0x94[i] = NULL;
        delete field_0xa4[i];
        field_0xa4[i] = NULL;
        delete field_0xb4[i];
        field_0xb4[i] = NULL;
        delete field_0xc4[i];
        field_0xc4[i] = NULL;
        delete field_0xd4[i];
        field_0xd4[i] = NULL;
        delete field_0xe4[i];
        field_0xe4[i] = NULL;
    }
}

// FIXME(shotgun): 90.19 %, missing/extra + reloc-name
void cls_804B13D4::fn_8041FC2C() {
    for (s32 i = 0; i < 4; i++) {
        fn_8041E3B4(i);
        lbl_80529DEC.field_0xd8[i] = fn_80237774(i);
        lbl_80529DEC.field_0xe8[i] = fn_80236100(i);
        lbl_80529DEC.field_0x108[i] = fn_802375B0(i);
        field_0x188[i] = 0x180;
        field_0x198[i] = 0;
        field_0x1a8[i] = 0;
        field_0x1b8[i] = 0;
        field_0xf8[i] = 0.0f;
        field_0x108[i] = 0.0f;
        field_0x118[i] = 0.0f;
        field_0x128[i] = 0.0f;
        field_0x138[i] = 0.0f;
        field_0x148[i] = 0.0f;
        field_0x158[i] = 0.0f;
        field_0x168[i] = 0.0f;
        field_0x285[i] = i;
        field_0x281[i] = 0xFF;
        field_0x23c[i] = 0;
        field_0x1c8[i] = 0;
        field_0x1d8[i] = 0;
        field_0x1e8[i] = 0;
        field_0x1f8[i] = 0;
        field_0x21a[i] = 0;
        field_0x224[i] = 0;
        field_0x22c[i] = 0;
        field_0x208[i] = 0;
        field_0x234[i] = 0;
    }
    field_0x222 = -1;
    if (gGameRoot_8053AAF8->field_0x7e == 0) {
        field_0x4->search('Ndata13')->show();
        field_0x4->search('Php1ni02')->hide();
        field_0x4->search('Ph1piib3')->show();
        field_0x4->search(lbl_8048D6B8[0])->hide();
        field_0x4->search(lbl_8048D6B8[1])->hide();
        field_0x4->search(lbl_8048D6B8[2])->hide();
        field_0x4->search(lbl_8048D6B8[3])->hide();
        if ((gGameRoot_8053AAF8->field_0x83 & 3) == 2) {
            field_0x4->search('Ndata16')->show();
        } else {
            field_0x4->search('Ndata16')->hide();
        }
        for (s32 i = 0; i < 4; i++) {
            field_0x18->search(lbl_8048DE00[i])->hide();
            field_0x18->search(lbl_8048DE20[0x4 + i])->hide();
            field_0x4->search(lbl_8048DE20[0x8 + i])->hide();
            field_0x4->search(lbl_8048DE20[0xC + i])->hide();
            field_0x4->search(lbl_8048DE20[0x10 + i])->hide();
            field_0x4->search(lbl_8048DE20[0x18 + i])->hide();
            field_0x4->search(lbl_8048DE20[0x1C + i])->hide();
            field_0x4->search(lbl_8048DE20[0x20 + i])->hide();
        }
        fn_8041A868(true);
    } else {
        fn_8041C67C();
    }
    if (field_0x291 != 0) {
        for (s32 i = 0; i < 4; i++) {
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                fn_80418D08(i, true);
            }
        }
    }
    field_0x28a = 0xF;
    field_0x28b = 0xF;
    field_0x289 = 0xF;
    field_0x28c = 0;
    field_0x28d = 0;
    field_0x28e = 0;
    field_0x178[0] = -2;
    field_0x178[1] = -2;
    field_0x178[2] = -2;
    field_0x178[3] = -2;
    s32 mode = lbl_80529DEC.field_0x118;
    if (mode < 8) {
        s32 sub = lbl_80529DEC.field_0x11c;
        if (sub < 4 && !(mode == 0 && sub == 0 && !(lbl_8050EC80.getCurrentFile().mFlags & 1)) &&
            !(mode == 0 && sub == 1) && !(mode == 1 && sub == 3) && !(mode == 2 && sub == 3) &&
            !(mode == 4 && sub == 3) && !(mode == 5 && sub == 1) && !(mode == 5 && sub == 3) &&
            !(mode == 6 && sub == 1) && !(mode == 6 && sub == 3) && !(mode == 7 && sub == 3)) {
            lbl_8053AED0->fn_8043A608();
        }
    }
    if (field_0x50 != NULL) {
        field_0x50->fn_8044172C();
    }
}

// The game is paused (GameRoot field_0x4c 11 or field_0x88 2), as in fn_80134CC8.
static inline bool isPaused() {
    if (gGameRoot_8053AAF8->field_0x4c == 11) {
        return true;
    }
    return gGameRoot_8053AAF8->field_0x88 == 2;
}

// FIXME(shotgun): 79.80 %, missing/extra + reloc-name
void cls_804B13D4::fn_8041F720() {
    s32 mode = lbl_80529DEC.field_0x118;
    if ((mode == 9 && lbl_80529DEC.field_0x11c == 3) || (mode == 0 && lbl_80529DEC.field_0x11c == 2) ||
        (mode == 1 && lbl_80529DEC.field_0x11c == 2) || (mode == 2 && lbl_80529DEC.field_0x11c == 2) ||
        (mode == 4 && lbl_80529DEC.field_0x11c == 2) || (mode == 5 && lbl_80529DEC.field_0x11c == 2) ||
        (mode == 10 && lbl_80529DEC.field_0x11c == 2)) {
        return;
    }
    if (field_0x291 != 0) {
        f32 sx = lbl_8050DB0C.field_0x30[4] *
                 ((f32)JUTVideo::getManager()->getFbWidth() / gGameRoot2_8053AB10->field_0x24->field_0x858);
        f32 sy = lbl_8050DB0C.field_0x30[4] *
                 ((f32)JUTVideo::getManager()->getEfbHeight() / gGameRoot2_8053AB10->field_0x24->field_0x858);
        f32 w = 0.0f + sx;
        f32 h = 0.0f + sy;
        for (s32 i = 0; i < 4; i++) {
            if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) || getPlayerLayer_8023DE58(fn_8022EA0C(i)) >= 8) {
                continue;
            }
            if (field_0x21a[i] > 0 && lbl_8050EC80.getCurrentFile().field_0x68 != 0) {
                Vec screen;
                gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(getPlayerPos_8023E724(fn_8022EA0C(i)), false, false, &screen);
                f32 x = sx * (screen.x / JUTVideo::getManager()->getFbWidth()) - JUTVideo::getManager()->getFbWidth() * 0.5f;
                f32 y = lbl_8050DB0C.field_0x4c[1] +
                        (sy * (screen.y / JUTVideo::getManager()->getEfbHeight()) - JUTVideo::getManager()->getEfbHeight() * 0.5f);
                JGeometry::TBox2f box(0.0f, 0.0f, w, h);
                field_0x4c->setOrtho(box, -10000.0f, 10000.0f);
                field_0x4c->setPort();
                field_0x8[i]->draw(x, y, field_0x4c);
                field_0x234[i] = 0;
            } else if (!isPaused() && fn_802366CC(fn_8022EA0C(i)) > 0 && fn_802366CC(fn_8022EA0C(i)) < 300) {
                Vec screen;
                gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(getPlayerPos_8023E724(fn_8022EA0C(i)), false, false, &screen);
                f32 x = sx * (screen.x / JUTVideo::getManager()->getFbWidth()) - JUTVideo::getManager()->getFbWidth() * 0.5f;
                f32 y = lbl_8050DB0C.field_0x4c[1] +
                        (sy * (screen.y / JUTVideo::getManager()->getEfbHeight()) - JUTVideo::getManager()->getEfbHeight() * 0.5f);
                JGeometry::TBox2f box(0.0f, 0.0f, w, h);
                field_0x4c->setOrtho(box, -10000.0f, 10000.0f);
                field_0x4c->setPort();
                field_0x2c[i]->draw(x, y, field_0x4c);
            }
        }
    }
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

// FIXME(shotgun): 99.93 %, reloc-name
void cls_804B13D4::fn_8041F5FC() {
    s32 mode = lbl_80529DEC.field_0x118;
    if ((mode == 9 && lbl_80529DEC.field_0x11c == 3) || (mode == 0 && lbl_80529DEC.field_0x11c == 2) ||
        (mode == 1 && lbl_80529DEC.field_0x11c == 2) || (mode == 2 && lbl_80529DEC.field_0x11c == 2) ||
        (mode == 4 && lbl_80529DEC.field_0x11c == 2) || (mode == 5 && lbl_80529DEC.field_0x11c == 2) ||
        (mode == 10 && lbl_80529DEC.field_0x11c == 2)) {
        return;
    }
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x18->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x50 != NULL) {
        field_0x50->fn_8044148C();
    }
}

// FIXME(shotgun): 94.34 %, missing/extra + reloc-name
void cls_804B13D4::fn_8041EDB0() {
    if (gGameRoot_8053AAF8->field_0x7e == 0) {
        fn_8041E3B4(0);
        fn_8041E8B4(0);
        if ((gGameRoot_8053AAF8->field_0x83 & 3) == 2) {
            fn_8041DD24();
        }
        fn_8041D18C(0);
        fn_8041CC04();
        fn_8041A868(false);
        if (gGameRoot2_8053AB10->field_0x20->field_0x14c[0] != 0) {
            gGameRoot2_8053AB10->field_0x20->field_0x217[0] = 1;
        }
        if (gGameRoot_8053AAF8->field_0x4c != 11 && fn_80236B50(0) && fn_80237774(0) == 0) {
            if (field_0x292 >= 2) {
                if (lbl_8053AEC8->field_0x5d == 0) {
                    lbl_8053AEC8->field_0x5d = 1;
                }
            } else {
                field_0x292++;
            }
        } else {
            field_0x292 = 0;
        }
    } else {
        for (s32 i = 0; i < 4; i++) {
            fn_8041E3B4(i);
            u8 bit = 1 << i;
            if (gGameRoot_8053AAF8->mPlayerMask & bit) {
                fn_8041E8B4(i);
                if (gGameRoot2_8053AB10->field_0x20->field_0x14c[i] != 0) {
                    if (!(field_0x28c & bit)) {
                        field_0x28c |= bit;
                        field_0x18->search(lbl_8048DE20[0x4 + i])->show();
                        field_0x4->search(lbl_8048DE20[0x8 + i])->show();
                        field_0x4->search(lbl_8048DE20[0xC + i])->hide();
                    }
                } else if (field_0x28c & bit) {
                    field_0x28c &= ~bit;
                    field_0x18->search(lbl_8048DE20[0x4 + i])->hide();
                    field_0x4->search(lbl_8048DE20[0x8 + i])->hide();
                    field_0x4->search(lbl_8048DE20[0xC + i])->show();
                }
                fn_8041D18C(i);
                fn_8041C0C0(i);
            }
        }
        fn_8041B69C();
        for (s32 i = 0; i < 4; i++) {
            if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i))) {
                continue;
            }
            fn_80419C38(i, false);
            TileMap_804AA858* map = gGameRoot2_8053AB10->field_0x20;
            if (map->field_0x214 != 0 && i == map->field_0x230) {
                map->field_0x217[i] = 1;
            }
            if (gGameRoot2_8053AB10->field_0x20->field_0x14c[i] != 0) {
                s32 x = field_0x4->search(lbl_8048DE20[0x8 + i])->getBounds()->i.x;
                s32 y = field_0x4->search(lbl_8048DE20[0x8 + i])->getBounds()->i.y;
                f32 w = field_0x4->search(lbl_8048DE20[0x8 + i])->getWidth();
                f32 h = field_0x4->search(lbl_8048DE20[0x8 + i])->getHeight();
                cls_803004C0& rect = gGameRoot2_8053AB10->field_0x20->field_0x1d0[i];
                rect.field_0x0 = x;
                rect.field_0x4 = y;
                rect.field_0x8 = w;
                rect.field_0xc = h;
                if (i == 0 || i == 2) {
                    if (field_0xf8[i] < 12.0f) {
                        field_0xf8[i] += 3.0f;
                    } else {
                        field_0xf8[i] = 15.0f;
                    }
                } else if (field_0xf8[i] > -12.0f) {
                    field_0xf8[i] -= 3.0f;
                } else {
                    field_0xf8[i] = -15.0f;
                }
            } else if (i == 0 || i == 2) {
                if (field_0xf8[i] < 3.0f) {
                    field_0xf8[i] = 0.0f;
                } else {
                    field_0xf8[i] -= 3.0f;
                }
            } else if (field_0xf8[i] > -3.0f) {
                field_0xf8[i] = 0.0f;
            } else {
                field_0xf8[i] += 3.0f;
            }
        }
    }
    if (field_0x291 != 0) {
        for (s32 i = 0; i < 4; i++) {
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                fn_80419570(i);
                fn_8041918C(i);
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                fn_80418D08(i, false);
            }
        }
    }
    field_0x28a = 0;
    field_0x28b = 0;
    field_0x289 = 0;
    if (field_0x50 != NULL) {
        field_0x50->fn_80441490();
    }
    if (gGameRoot_8053AAF8->field_0x4c != 11 && field_0x298 == 0 && fn_80236890()) {
        gGameRoot_8053AAF8->fn_801367E0(11);
    }
    if (gGameRoot_8053AAF8->field_0x4c == 11) {
        if (field_0x298 == 0) {
            field_0x298 = 1;
        }
    } else if (field_0x298 == 1) {
        field_0x298 = 0;
    }
    s32 mode = lbl_80529DEC.field_0x118;
    if (mode == 9 && lbl_80529DEC.field_0x11c == 3) {
        for (s32 i = 0; i < 4; i++) {
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                gGameRoot2_8053AB10->field_0x20->field_0x217[i] = 1;
            }
        }
    } else if ((mode == 0 && lbl_80529DEC.field_0x11c == 2) || (mode == 1 && lbl_80529DEC.field_0x11c == 2) ||
               (mode == 2 && lbl_80529DEC.field_0x11c == 2) || (mode == 4 && lbl_80529DEC.field_0x11c == 2) ||
               (mode == 5 && lbl_80529DEC.field_0x11c == 2) || (mode == 10 && lbl_80529DEC.field_0x11c == 2)) {
        for (s32 i = 0; i < 4; i++) {
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                gGameRoot2_8053AB10->field_0x20->field_0x217[i] = 1;
            }
        }
    }
}

// FIXME(shotgun): 85.64 %, missing/extra
void cls_804B13D4::fn_8041E8B4(s32 player) {
    u8 bit = 1 << player;
    if (lbl_80529DEC.field_0x239 & bit) {
        lbl_80529DEC.field_0xd8[player] = fn_80237774(player);
        for (s32 i = 0; i < 4; i++) {
            if (i != player && fn_8022F514(i) && player == fn_8022F3DC(i)) {
                lbl_80529DEC.field_0xd8[i] = lbl_80529DEC.field_0xd8[player];
            }
        }
        field_0x28a |= bit;
        field_0x198[player] = 0;
        field_0x21a[player] = lbl_8050DB0C.field_0x4c[20] * lbl_8050DB0C.field_0x4c[21] + 30;
        field_0x1e8[player] = field_0x1d8[player];
        field_0x1d8[player] = lbl_80529DEC.field_0xd8[player];
        field_0x1f8[player] = fn_80237810(player);
        lbl_80529DEC.field_0x239 &= ~bit;
    } else if (lbl_80529DEC.field_0xd8[player] < fn_80237774(player)) {
        field_0x198[player]++;
        if (field_0x198[player] >= 10) {
            lbl_80529DEC.field_0xd8[player]++;
            for (s32 i = 0; i < 4; i++) {
                if (i != player && fn_8022F514(i) && player == fn_8022F3DC(i)) {
                    lbl_80529DEC.field_0xd8[i] = lbl_80529DEC.field_0xd8[player];
                }
            }
            field_0x28a |= bit;
            field_0x198[player] = 0;
            fn_80230EB8(player, 9);
        }
        if (field_0x291 != 0) {
            field_0x21a[player] = lbl_8050DB0C.field_0x4c[20] * lbl_8050DB0C.field_0x4c[21] + 30;
            field_0x1e8[player] = field_0x1d8[player];
            field_0x1d8[player] = lbl_80529DEC.field_0xd8[player];
            field_0x1f8[player] = fn_80237810(player);
        }
    } else if (lbl_80529DEC.field_0xd8[player] > fn_80237774(player)) {
        lbl_80529DEC.field_0x24a |= bit;
        field_0x198[player]++;
        if (field_0x198[player] >= 10) {
            lbl_80529DEC.field_0xd8[player]--;
            for (s32 i = 0; i < 4; i++) {
                if (i != player && fn_8022F514(i) && player == fn_8022F3DC(i)) {
                    lbl_80529DEC.field_0xd8[i] = lbl_80529DEC.field_0xd8[player];
                }
            }
            field_0x28a |= bit;
            field_0x198[player] = 0;
        }
        if (field_0x291 != 0) {
            field_0x21a[player] = lbl_8050DB0C.field_0x4c[20] * lbl_8050DB0C.field_0x4c[21] + 30;
            field_0x1e8[player] = field_0x1d8[player];
            field_0x1d8[player] = lbl_80529DEC.field_0xd8[player];
            field_0x1f8[player] = fn_80237810(player);
        }
    } else {
        field_0x198[player] = 0;
    }
    if (lbl_80529DEC.field_0xe8[player] < fn_80236100(player)) {
        field_0x1a8[player]++;
        if (field_0x1a8[player] >= 2) {
            lbl_80529DEC.field_0xe8[player] += 2;
            if (lbl_80529DEC.field_0xe8[player] >= fn_80236100(player)) {
                lbl_80529DEC.field_0xe8[player] = fn_80236100(player);
                fn_80230EB8(player, 0x51);
            } else {
                fn_80230EB8(player, 4);
            }
            field_0x28b |= bit;
            field_0x1a8[player] = 0;
        }
    } else if (lbl_80529DEC.field_0xe8[player] > fn_80236100(player)) {
        field_0x1a8[player]++;
        if (field_0x1a8[player] >= 2) {
            lbl_80529DEC.field_0xe8[player] -= 2;
            if (lbl_80529DEC.field_0xe8[player] <= fn_80236100(player)) {
                lbl_80529DEC.field_0xe8[player] = fn_80236100(player);
                fn_80230EB8(player, 0x51);
            } else {
                fn_80230EB8(player, 0x4E);
            }
            field_0x28b |= bit;
            field_0x1a8[player] = 0;
        }
    } else {
        field_0x1a8[player] = 0;
    }
    if (field_0x224[player] > 0) {
        if (field_0x22c[player] == 0) {
            field_0x224[player]--;
            fn_80230FD0(player, 0x20034);
            if (field_0x224[player] != 0) {
                field_0x22c[player] = 12;
            }
        } else {
            field_0x22c[player]--;
        }
    }
}

// FIXME(shotgun): 99.58 %, regalloc-only
void cls_804B13D4::fn_8041E708(s32 id, s32 arg1, u32 arg2) {
    s32 target;
    if (fn_8022F514(id)) {
        target = fn_8022F3DC(id);
    } else {
        target = id;
    }
    if ((u8)arg2) {
        s32 sum = arg1 + fn_80236100(target);
        if (sum > 9999) {
            arg1 = 9999 - fn_80236228(target);
        } else if (sum < -5000) {
            arg1 = -5000 - fn_80236228(target);
        }
    } else {
        s32 sum = arg1 + fn_80236228(target);
        if (sum > 9999) {
            arg1 = 9999 - fn_80236228(target);
        } else if (sum < 0) {
            if (fn_80236228(target) <= 0) {
                arg1 = 0;
            } else {
                arg1 = -fn_80236228(target);
            }
        }
    }
    if (arg1 >= 1000 || arg1 <= -1000) {
        field_0x224[id] = __abs(arg1) / 100;
        field_0x22c[id] = 0;
    } else {
        fn_80230FD0(id, 0x20034);
    }
    if ((u8)arg2) {
        fn_8023619C(target, arg1);
    } else {
        fn_802362C4(target, arg1);
    }
    lbl_80529DEC.field_0xe8[target] = fn_80236100(target);
    field_0x1a8[target] = 0;
    field_0x28b |= (u8)(1 << target);
}

// FIXME(shotgun): 98.84 %, missing/extra (the takesPart bool test is one cmplwi longer)
void cls_804B13D4::fn_8041E58C() {
    for (s32 i = 0; i < 4; i++) {
        bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i);
        if (takesPart == true) {
            lbl_80529DEC.field_0xd8[i] = fn_80237774(i);
            for (s32 j = 0; j < 4; j++) {
                if (j != i && fn_8022F514(j) && i == fn_8022F3DC(j)) {
                    lbl_80529DEC.field_0xd8[j] = lbl_80529DEC.field_0xd8[i];
                }
            }
            field_0x28a |= (u8)(1 << i);
            field_0x198[i] = 0;
            lbl_80529DEC.field_0xe8[i] = fn_80236100(i);
            field_0x28b |= (u8)(1 << i);
            field_0x1a8[i] = 0;
            lbl_80529DEC.field_0x108[i] = fn_802375B0(i);
            for (s32 j = 0; j < 4; j++) {
                if (j != i && fn_8022F514(j) && i == fn_8022F3DC(j)) {
                    lbl_80529DEC.field_0x108[j] = lbl_80529DEC.field_0x108[i];
                }
            }
            field_0x289 |= (u8)(1 << i);
            field_0x1b8[i] = 0;
        }
    }
}

// FIXME(shotgun): 94.11 %, missing/extra (bool tests of the masks are two instructions shorter)
void cls_804B13D4::fn_8041E3B4(s32 player) {
    s32 mask = 1 << player;
    u8 bit = mask;
    bool takesPart = gGameRoot_8053AAF8->mPlayerMask & bit;
    if (takesPart == false) {
        field_0x28f &= ~bit;
        return;
    }
    bool flagged = lbl_8053AED0->field_0xdd & bit;
    if (flagged == true && !(field_0x290 & bit)) {
        field_0x290 |= bit;
        field_0x28f |= bit;
        return;
    }
    if (lbl_8053AED0->field_0xdc & 0x10) {
        field_0x28f &= ~bit;
        return;
    }
    if (getGameView_802D6A9C()->field_0x131ef != 0 || fn_8022AF5C() || fn_8022B1AC() || fn_8022B160()) {
        field_0x28f &= ~bit;
        field_0x293 |= (u8)mask;
        return;
    }
    if (lbl_8053AEC8->field_0x5e == 1) {
        for (s32 i = 0; i < gGameRoot_8053AAF8->fn_80138A30(); i++) {
            field_0x28f |= (u8)(1 << i);
        }
        lbl_8053AEC8->field_0x5e = 0;
        return;
    }
    if (gGameRoot_8053AAF8->field_0x4c == 11) {
        field_0x28f &= ~bit;
        return;
    }
    if (!fn_8022B1AC() && !fn_8022B160() && (field_0x293 & bit)) {
        field_0x28f |= (u8)mask;
        field_0x293 &= ~bit;
    }
}

// FIXME(shotgun): 94.92 %, missing/extra
void cls_804B13D4::fn_8041DD24() {
    if (!(field_0x28a & 1)) {
        return;
    }
    s32 max = fn_80237810(0);
    s32 life = lbl_80529DEC.field_0xd8[0];
    if (max > 32) {
        max = 32;
    }
    if (max < 0) {
        max = 0;
    }
    if (max < life) {
        life = max;
    }
    if (life < 0) {
        life = 0;
    }
    for (s32 i = 0; i < 8; i++) {
        if (life >= 4) {
            field_0x4->search(lbl_8048D468[0][i])->show();
            field_0x4->search(lbl_8048D468[1][i])->show();
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[0][i]))->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[1][i]))->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x4->search(lbl_8048D468[2][i])->hide();
            field_0x4->search(lbl_8048D468[3][i])->hide();
            max -= 4;
            life -= 4;
            continue;
        }
        if (life == 0) {
            field_0x4->search(lbl_8048D468[0][i])->hide();
            field_0x4->search(lbl_8048D468[1][i])->hide();
            if (max > 0) {
                field_0x4->search(lbl_8048D468[2][i])->show();
                field_0x4->search(lbl_8048D468[3][i])->show();
                max -= 4;
            } else {
                field_0x4->search(lbl_8048D468[2][i])->hide();
                field_0x4->search(lbl_8048D468[3][i])->hide();
            }
        } else if (life == 1) {
            field_0x4->search(lbl_8048D468[0][i])->show();
            field_0x4->search(lbl_8048D468[1][i])->show();
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[0][i]))->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[1][i]))->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x4->search(lbl_8048D468[2][i])->hide();
            field_0x4->search(lbl_8048D468[3][i])->hide();
            max -= 4;
        } else if (life == 2) {
            field_0x4->search(lbl_8048D468[0][i])->show();
            field_0x4->search(lbl_8048D468[1][i])->show();
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[0][i]))->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[1][i]))->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x4->search(lbl_8048D468[2][i])->hide();
            field_0x4->search(lbl_8048D468[3][i])->hide();
            max -= 4;
        } else if (life == 3) {
            field_0x4->search(lbl_8048D468[0][i])->show();
            field_0x4->search(lbl_8048D468[1][i])->show();
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[0][i]))->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D468[1][i]))->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x4->search(lbl_8048D468[2][i])->hide();
            field_0x4->search(lbl_8048D468[3][i])->hide();
            max -= 4;
        }
        life = 0;
    }
}

// FIXME(shotgun): 90.11 %, missing/extra
void cls_804B13D4::fn_8041D18C(s32 player) {
    s32 idx = -1;
    if (gGameRoot_8053AAF8->field_0x7e == 0) {
        u8 bit = 1 << player;
        if (!(field_0x28d & bit)) {
            if (!fn_80230188(player) && fn_8023BE0C(fn_8022EA0C(player)) && !(lbl_8053AED0->field_0x255 & bit)) {
                field_0x28d |= bit;
                field_0x4->search('Php1ni02')->show();
            }
        } else {
            if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(player), 0x100) == 0 ||
                (lbl_8053AED0->field_0x255 & bit) || fn_80230188(player)) {
                field_0x28d &= ~bit;
                field_0x4->search('Php1ni02')->hide();
            }
        }
        if (fn_80236D14(player)) {
            if (!field_0x4->search(lbl_8048D6B8[0])->isVisible()) {
                field_0x4->search(lbl_8048D6B8[0])->show();
                field_0x4->search(lbl_8048D6B8[1])->show();
            }
        } else {
            if (field_0x4->search(lbl_8048D6B8[0])->isVisible() == true) {
                field_0x4->search(lbl_8048D6B8[0])->hide();
                field_0x4->search(lbl_8048D6B8[1])->hide();
            }
        }
        if (fn_80236E3C(player)) {
            if (!field_0x4->search(lbl_8048D6B8[2])->isVisible()) {
                field_0x4->search(lbl_8048D6B8[2])->show();
                field_0x4->search(lbl_8048D6B8[3])->show();
            }
        } else {
            if (field_0x4->search(lbl_8048D6B8[2])->isVisible() == true) {
                field_0x4->search(lbl_8048D6B8[2])->hide();
                field_0x4->search(lbl_8048D6B8[3])->hide();
            }
        }
    } else {
        u8 bit = 1 << player;
        if (!(field_0x28d & bit)) {
            if (!fn_80230188(player) && fn_8023BE0C(fn_8022EA0C(player)) && !(lbl_8053AED0->field_0x255 & bit)) {
                field_0x28d |= bit;
                field_0x4->search(lbl_8048DCD8[player])->show();
            }
        } else {
            if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(player), 0x100) == 0 ||
                (lbl_8053AED0->field_0x255 & bit) || fn_80230188(player)) {
                field_0x28d &= ~bit;
                field_0x4->search(lbl_8048DCD8[player])->hide();
            }
        }
        if (fn_80236D14(player)) {
            if (!field_0x4->search(lbl_8048DCF8[player][0])->isVisible()) {
                field_0x4->search(lbl_8048DCF8[player][0])->show();
                field_0x4->search(lbl_8048DCF8[player][1])->show();
            }
        } else {
            if (field_0x4->search(lbl_8048DCF8[player][0])->isVisible() == true) {
                field_0x4->search(lbl_8048DCF8[player][0])->hide();
                field_0x4->search(lbl_8048DCF8[player][1])->hide();
            }
        }
        if (fn_80236E3C(player)) {
            if (!field_0x4->search(lbl_8048DCF8[player][2])->isVisible()) {
                field_0x4->search(lbl_8048DCF8[player][2])->show();
                field_0x4->search(lbl_8048DCF8[player][3])->show();
            }
        } else {
            if (field_0x4->search(lbl_8048DCF8[player][2])->isVisible() == true) {
                field_0x4->search(lbl_8048DCF8[player][2])->hide();
                field_0x4->search(lbl_8048DCF8[player][3])->hide();
            }
        }
    }
    switch (fn_802373EC(player)) {
    case 1:
        idx = 5;
        break;
    case 2:
        idx = 11;
        break;
    case 3:
        idx = 2;
        break;
    case 4:
        idx = 3;
        break;
    case 5:
        idx = 6;
        break;
    case 6:
        idx = 7;
        break;
    case 7:
        idx = 4;
        break;
    case 8:
        idx = 8;
        break;
    case 9:
        idx = 10;
        break;
    case 10:
        idx = 1;
        break;
    case 11:
    case 12:
    case 13:
        idx = 0;
        break;
    case 0:
    default:
        break;
    }
    if (idx != field_0x178[player] || (lbl_8053AED0->field_0x255 & (u8)(1 << player))) {
        field_0x178[player] = idx;
        for (s32 i = 0; i < 12; i++) {
            if (gGameRoot_8053AAF8->field_0x7e == 0) {
                if (i == idx) {
                    field_0x4->search(lbl_8048D568[i])->show();
                    field_0x4->search(lbl_8048D5C8[i])->show();
                } else {
                    field_0x4->search(lbl_8048D568[i])->hide();
                    field_0x4->search(lbl_8048D5C8[i])->hide();
                }
            } else {
                if (i == idx) {
                    field_0x4->search(lbl_8048D858[player][i])->show();
                    field_0x4->search(lbl_8048D9D8[player][i])->show();
                } else {
                    field_0x4->search(lbl_8048D858[player][i])->hide();
                    field_0x4->search(lbl_8048D9D8[player][i])->hide();
                }
            }
        }
        if (gGameRoot_8053AAF8->field_0x7e == 0) {
            for (s32 i = 0; i < 12; i++) {
                if (i == idx) {
                    field_0x4->search(lbl_8048D568[i])->show();
                    field_0x4->search(lbl_8048D5C8[i])->show();
                } else {
                    field_0x4->search(lbl_8048D568[i])->hide();
                    field_0x4->search(lbl_8048D5C8[i])->hide();
                }
            }
            if (fn_8023725C(player) == 2) {
                for (s32 i = 0; i < 3; i++) {
                    field_0x4->search(lbl_8048D640[i])->show();
                }
            } else {
                for (s32 i = 0; i < 3; i++) {
                    field_0x4->search(lbl_8048D640[i])->hide();
                }
            }
        } else {
            for (s32 i = 0; i < 12; i++) {
                if (i == idx) {
                    field_0x4->search(lbl_8048D858[player][i])->show();
                    field_0x4->search(lbl_8048D9D8[player][i])->show();
                } else {
                    field_0x4->search(lbl_8048D858[player][i])->hide();
                    field_0x4->search(lbl_8048D9D8[player][i])->hide();
                }
            }
            if (fn_8023725C(player) == 2) {
                for (s32 i = 0; i < 3; i++) {
                    field_0x4->search(lbl_8048DBB8[player][i])->show();
                }
            } else {
                for (s32 i = 0; i < 3; i++) {
                    field_0x4->search(lbl_8048DBB8[player][i])->hide();
                }
            }
        }
    }
}

// FIXME(shotgun): 83.30 %, missing/extra
void cls_804B13D4::fn_8041CC04() {
    if (!(field_0x28b & 1)) {
        return;
    }
    s32 digit = 0;
    s32 n = lbl_80529DEC.field_0xe8[0];
    if (n > 9999) {
        n = 9999;
    }
    if (n < 0) {
        for (s32 i = 0; i < 5; i++) {
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[i]))
                ->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xFF, 0x55, 0x41, 0xFF));
        }
        if (n < -5000) {
            n = -5000;
        }
        n *= -1;
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_rupyNumMinus.bti"));
        static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[0]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(field_0x4->search(lbl_8048D680[0]))->changeTexture(res, 0);
        field_0x4->search(lbl_8048D658[4])->show();
        field_0x4->search(lbl_8048D680[4])->show();
        digit = 1;
    } else {
        for (s32 i = 0; i < 5; i++) {
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[i]))->setBlackWhite(field_0x248, field_0x244);
        }
        field_0x4->search(lbl_8048D658[4])->hide();
        field_0x4->search(lbl_8048D680[4])->hide();
    }
    const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[n / 1000]));
    n -= (n / 1000) * 1000;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[digit]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D680[digit]))->changeTexture(res, 0);
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[n / 100]));
    n -= (n / 100) * 100;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[digit + 1]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D680[digit + 1]))->changeTexture(res, 0);
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[n / 10]));
    n -= (n / 10) * 10;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[digit + 2]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D680[digit + 2]))->changeTexture(res, 0);
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[n]));
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D658[digit + 3]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D680[digit + 3]))->changeTexture(res, 0);
}

// FIXME(shotgun): 90.34 %, missing/extra + reloc-name
void cls_804B13D4::fn_8041C67C() {
    field_0x4->search('Ndata13')->hide();
    field_0x4->search('Php1ni02')->hide();
    field_0x4->search('Ph1piib3')->hide();
    field_0x4->search('Ndata16')->hide();
    field_0x4->search('Ndata02')->hide();
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            field_0x18->search(lbl_8048DE00[i])->hide();
            field_0x18->search(lbl_8048DE20[0x4 + i])->hide();
            field_0x4->search(lbl_8048DE20[0x8 + i])->hide();
            field_0x4->search(lbl_8048DE20[0xC + i])->show();
            field_0x4->search(lbl_8048DE20[0x10 + i])->hide();
            field_0x4->search(lbl_8048DE20[0x18 + i])->show();
            if (lbl_8050EC80.getCurrentFile().field_0x68 != 0) {
                field_0x4->search(lbl_8048DE20[0x1C + i])->show();
            } else {
                field_0x4->search(lbl_8048DE20[0x1C + i])->hide();
            }
            field_0x4->search(lbl_8048DE20[0x20 + i])->show();
            field_0x4->search(lbl_8048DCF8[i][0])->hide();
            field_0x4->search(lbl_8048DCF8[i][1])->hide();
            field_0x4->search(lbl_8048DCF8[i][2])->hide();
            field_0x4->search(lbl_8048DCF8[i][3])->hide();
        } else {
            field_0x18->search(lbl_8048DE00[i])->hide();
            field_0x18->search(lbl_8048DE20[0x4 + i])->hide();
            field_0x4->search(lbl_8048DE20[0x8 + i])->hide();
            field_0x4->search(lbl_8048DE20[0xC + i])->hide();
            field_0x4->search(lbl_8048DE20[0x10 + i])->hide();
            field_0x4->search(lbl_8048DE20[0x18 + i])->hide();
            field_0x4->search(lbl_8048DE20[0x1C + i])->hide();
            field_0x4->search(lbl_8048DE20[0x20 + i])->hide();
        }
    }
    for (s32 i = 0; i < 4; i++) {
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B13C4[0]));
        static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE00[i]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))->changeTexture(res, 0);
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            fn_80419C38(i, true);
        }
    }
}

// FIXME(shotgun): 68.65 %, missing/extra + reloc-name
void cls_804B13D4::fn_8041C0C0(s32 player) {
    if (!(field_0x28b & (u8)(1 << player))) {
        return;
    }
    s32 digit = 0;
    s32 n = lbl_80529DEC.field_0xe8[player];
    if (n > 9999) {
        n = 9999;
    }
    if (n < 0) {
        for (s32 i = 0; i < 5; i++) {
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D6D8[player][i]))
                ->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xFF, 0x55, 0x41, 0xFF));
        }
        if (n < -5000) {
            n = -5000;
        }
        n *= -1;
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_rupyNumMinus.bti"));
        static_cast<J2DPicture*>(field_0x4->search(lbl_8048D6D8[player][0]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(field_0x4->search(lbl_8048D778[player][0]))->changeTexture(res, 0);
        field_0x4->search(lbl_8048D6D8[player][4])->show();
        field_0x4->search(lbl_8048D778[player][4])->show();
        digit = 1;
    } else {
        for (s32 i = 0; i < 5; i++) {
            static_cast<J2DPicture*>(field_0x4->search(lbl_8048D6D8[player][i]))->setBlackWhite(field_0x248, field_0x244);
        }
        field_0x4->search(lbl_8048D6D8[player][4])->hide();
        field_0x4->search(lbl_8048D778[player][4])->hide();
    }
    const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[n / 1000]));
    n %= 1000;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D6D8[player][digit]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D778[player][digit]))->changeTexture(res, 0);
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[n / 100]));
    n %= 100;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D6D8[player][digit + 1]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D778[player][digit + 1]))->changeTexture(res, 0);
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[n / 10]));
    n %= 10;
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D6D8[player][digit + 2]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D778[player][digit + 2]))->changeTexture(res, 0);
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[n]));
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D6D8[player][digit + 3]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(field_0x4->search(lbl_8048D778[player][digit + 3]))->changeTexture(res, 0);
}

// FIXME(shotgun): 99.78 %, missing/extra
void cls_804B13D4::fn_8041B69C() {
    s32 count = 0;
    s32 assigned = 0;
    if (!(field_0x28b & 1) && !(field_0x28b & 2) && !(field_0x28b & 4) && !(field_0x28b & 8)) {
        return;
    }
    if (field_0x28e == 0) {
        field_0x28e = 1;
        return;
    }
    s32 rank[4];
    s32 done[4];
    for (s32 i = 0; i < 4; i++) {
        rank[i] = 0;
        done[i] = 0;
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            if (lbl_8050EC80.getCurrentFile().field_0x68 != 0) {
                field_0x18->search(lbl_8048DE00[i])->show();
            } else {
                field_0x18->search(lbl_8048DE00[i])->hide();
            }
            count++;
        }
    }
    while (assigned < count) {
        s32 maxScore = -5001;
        s32 tieCount = 0;
        for (s32 i = 0; i < count; i++) {
            if (done[i] == 0 && maxScore < fn_80236100(i)) {
                maxScore = fn_80236100(i);
            }
        }
        for (s32 i = 0; i < count; i++) {
            if (done[i] == 0 && maxScore == fn_80236100(i)) {
                rank[i] = assigned;
                tieCount++;
                done[i] = 1;
            }
        }
        assigned += tieCount;
    }
    for (s32 i = 0; i < count; i++) {
        switch (rank[i]) {
        case 0:
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setCornerColor(JUtility::TColor(0xB9, 0x12, 0x12, 0xFF),
                                 JUtility::TColor(0xD8, 0x3A, 0x16, 0xFF),
                                 JUtility::TColor(0xB9, 0x54, 0x12, 0xFF),
                                 JUtility::TColor(0xFF, 0x9D, 0x10, 0xFF));
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE00[i]))
                ->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 1:
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setCornerColor(JUtility::TColor(0xF2, 0x48, 0x09, 0xFF),
                                 JUtility::TColor(0xF2, 0x48, 0x09, 0xFF),
                                 JUtility::TColor(0xF2, 0xAB, 0x24, 0xFF),
                                 JUtility::TColor(0xFC, 0xFF, 0x21, 0xFF));
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE00[i]))
                ->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 2:
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setWhite(JUtility::TColor(0xFF, 0xFF, 0x77, 0xFF));
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                 JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                 JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                 JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE00[i]))
                ->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 3:
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setWhite(JUtility::TColor(0xD8, 0xFF, 0xA1, 0xFF));
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                 JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                 JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
                                 JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE00[i]))
                ->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x18->search(lbl_8048DE20[i]))
                ->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            break;
        }
    }
}

// FIXME(shotgun): 87.66 %, missing/extra + reloc-name
void cls_804B13D4::fn_8041A868(bool arg) {
    f32 prev0 = field_0x118[0];
    f32 prev4 = field_0x158[0];
    s32 row = 0;
    if ((gGameRoot_8053AAF8->field_0x83 & 3) == 2) {
        row = 1;
    }
    s32 level = fn_80418B64();
    bool fadingOut = false;
    if (field_0x222 != (s16)level) {
        if (field_0x118[0] > 0.0f) {
            fadingOut = true;
        } else {
            field_0x222 = level;
        }
    }
    s16 dx = lbl_8050DB0C.field_0x4c[0xF3] + lbl_8048E0E0[row][field_0x222];
    s16 dy = lbl_8050DB0C.field_0x4c[0xF4] + lbl_8048E0F4[row][field_0x222];
    if (lbl_8048E0CC[row][field_0x222] == 0) {
        dy = (s32)((f32)dy + lbl_8053AED0->fn_8043D514());
    } else {
        dy = (s32)((f32)dy + lbl_8053AED0->fn_8043D57C());
    }
    field_0x54->moveFromCentre_80402E08(
        (f32)(dx + (lbl_8048E0B8[row][field_0x222] + lbl_8050DB0C.field_0x4c[0x143] + lbl_8048E108[row][field_0x222])),
        (f32)dy + (field_0x108[0] + (f32)lbl_8048E0CC[row][field_0x222]) + (f32)lbl_8050DB0C.field_0x4c[0x144]);
    field_0x58->moveFromCentre_80402E08(
        (f32)dx + (field_0xf8[0] + (f32)lbl_8048E0B8[row][field_0x222]) + (f32)lbl_8048E130[row][field_0x222],
        (f32)dy + (field_0x108[0] + (f32)lbl_8048E0CC[row][field_0x222]) + (f32)lbl_8050DB0C.field_0x4c[0x145] + (f32)lbl_8050DB0C.field_0x4c[0x146]);
    field_0x5c->moveFromCentre_80402E08(
        (f32)dx + (field_0xf8[0] + (f32)lbl_8048E0B8[row][field_0x222]) + (f32)lbl_8048E130[row][field_0x222],
        (f32)dy + (field_0x108[0] + (f32)lbl_8048E0CC[row][field_0x222]) + (f32)lbl_8050DB0C.field_0x4c[0x145] + (f32)lbl_8050DB0C.field_0x4c[0x146]);
    field_0x6c->moveFromCentre_80402E08(
        (f32)dx + (field_0xf8[0] + (f32)lbl_8048E0B8[row][field_0x222]) + (f32)lbl_8048E11C[row][field_0x222],
        (f32)dy + (field_0x108[0] + (f32)lbl_8048E0CC[row][field_0x222]) + (f32)lbl_8050DB0C.field_0x4c[0x147] + (f32)lbl_8050DB0C.field_0x4c[0x148]);
    field_0x70->moveFromCentre_80402E08(
        (f32)dx + (field_0xf8[0] + (f32)lbl_8048E0B8[row][field_0x222]) + (f32)lbl_8048E144[row][field_0x222],
        (f32)dy + (field_0x108[0] + (f32)lbl_8048E0CC[row][field_0x222]) + (f32)lbl_8050DB0C.field_0x4c[0x149] + (f32)lbl_8050DB0C.field_0x4c[0x14A]);
    fn_8037EB4C();
    if (!(field_0x28f & 1) || gGameRoot2_8053AB10->field_0x30->field_0x284 != 0 ||
        gGameRoot_8053AAF8->field_0x85 != 0 || field_0x222 == 4 || fadingOut) {
        if (field_0x118[0] > 0.1f) {
            field_0x118[0] -= 0.1f;
        } else {
            field_0x118[0] = 0.0f;
        }
        if (field_0x158[0] > 0.1f) {
            field_0x158[0] -= 0.1f;
        } else {
            field_0x158[0] = 0.0f;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x217[0] = 1;
    } else if (lbl_80529DEC.field_0xd8[0] == 0 && lbl_8053AEC8->field_0x5e == 0) {
        if (field_0x118[0] > 0.6f) {
            field_0x118[0] -= 0.1f;
            if (field_0x118[0] <= 0.6f) {
                field_0x118[0] = 0.6f;
            }
        } else if (field_0x118[0] < 0.6f) {
            field_0x118[0] += 0.1f;
            if (field_0x118[0] >= 0.6f) {
                field_0x118[0] = 0.6f;
            }
        }
        if (lbl_8053AED0->field_0x10e & 1) {
            if (field_0x158[0] > 0.6f) {
                field_0x158[0] -= 0.1f;
                if (field_0x158[0] <= 0.6f) {
                    field_0x158[0] = 0.6f;
                }
            } else if (field_0x158[0] < 0.6f) {
                field_0x158[0] += 0.1f;
                if (field_0x158[0] >= 0.6f) {
                    field_0x158[0] = 0.6f;
                }
            }
        } else if (field_0x158[0] > 0.1f) {
            field_0x158[0] -= 0.1f;
        } else {
            field_0x158[0] = 0.0f;
        }
    } else {
        if (field_0x118[0] < 0.9f) {
            field_0x118[0] += 0.1f;
        } else {
            field_0x118[0] = 1.0f;
        }
        if (lbl_8053AED0->field_0x10e & 1) {
            if (field_0x158[0] < 0.9f) {
                field_0x158[0] += 0.1f;
            } else {
                field_0x158[0] = 1.0f;
            }
        } else if (field_0x158[0] > 0.1f) {
            field_0x158[0] -= 0.1f;
        } else {
            field_0x158[0] = 0.0f;
        }
    }
    if (prev0 == field_0x118[0] && prev4 == field_0x158[0] && !arg) {
        return;
    }
    field_0x4->search(lbl_8048D6A8[0])->setAlpha(field_0x24c[0] * field_0x118[0]);
    field_0x4->search(lbl_8048D6A8[1])->setAlpha(field_0x24c[1] * field_0x118[0]);
    for (s32 i = 0; i < 5; i++) {
        field_0x4->search(lbl_8048D658[i])->setAlpha(field_0x24c[2] * field_0x118[0]);
        field_0x4->search(lbl_8048D680[i])->setAlpha(field_0x24c[3] * field_0x118[0]);
    }
    for (s32 i = 0; i < 3; i++) {
        field_0x4->search(lbl_8048D628[i])->setAlpha(field_0x258[i] * field_0x118[0]);
        field_0x4->search(lbl_8048D640[i])->setAlpha(field_0x25b[i] * field_0x118[0]);
    }
    for (s32 i = 0; i < 12; i++) {
        field_0x4->search(lbl_8048D568[i])->setAlpha(field_0x25e * field_0x118[0]);
        field_0x4->search(lbl_8048D5C8[i])->setAlpha(field_0x25f * field_0x118[0]);
    }
    for (s32 i = 0; i < 8; i++) {
        field_0x4->search(lbl_8048D468[0][i])->setAlpha(field_0x250[0] * field_0x118[0]);
        field_0x4->search(lbl_8048D468[1][i])->setAlpha(field_0x250[1] * field_0x118[0]);
        field_0x4->search(lbl_8048D468[2][i])->setAlpha(field_0x250[2] * field_0x118[0]);
        field_0x4->search(lbl_8048D468[3][i])->setAlpha(field_0x250[3] * field_0x118[0]);
    }
    field_0x4->search('Php1ni02')->setAlpha(field_0x26a * field_0x118[0]);
    field_0x4->search('Php1ni01')->setAlpha(field_0x26b * field_0x118[0]);
    for (s32 i = 0; i < 4; i++) {
        field_0x4->search(lbl_8048D6B8[i])->setAlpha(field_0x26c[i] * field_0x118[0]);
    }
}

// FIXME(shotgun): 56.39 %, missing/extra + reloc-name
void cls_804B13D4::fn_80419C38(s32 player, bool arg) {
    u8 bit = 1 << player;
    f32 prev0 = field_0x108[player];
    f32 prev1 = field_0x118[player];
    f32 prev2 = field_0x138[player];
    f32 prev3 = field_0x148[player];
    f32 prev4 = field_0x158[player];
    JGeometry::TVec2f screenPos[4];
    for (s32 i = 0; i < 4; i++) {
        if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) && getPlayerLayer_8023DE58(fn_8022EA0C(i)) < 8) {
            Vec screen;
            gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(getPlayerPos_8023E724(fn_8022EA0C(i)), false, false, &screen);
            screenPos[i].x = screen.x;
            screenPos[i].y = screen.y;
        } else {
            screenPos[i].x = 0.0f;
            screenPos[i].y = 0.0f;
        }
    }
    bool overlap = false;
    for (s32 i = 0; i < 4; i++) {
        if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) &&
            screenPos[i].x > lbl_8048E020[player][0] && screenPos[i].x < lbl_8048E020[player][1] &&
            screenPos[i].y > lbl_8048E020[player][2] && screenPos[i].y < lbl_8048E020[player][3]) {
            overlap = true;
            break;
        }
    }
    if (overlap || !(field_0x28f & bit)) {
        if (prev0 > 0.0f) {
            field_0x108[player] -= 0.1f;
        } else {
            field_0x108[player] = 0.0f;
        }
        if (prev1 > 0.0f) {
            field_0x118[player] -= 0.1f;
        } else {
            field_0x118[player] = 0.0f;
        }
        if (prev2 > 0.0f) {
            field_0x138[player] -= 0.1f;
        } else {
            field_0x138[player] = 0.0f;
        }
        if (prev3 > 0.0f) {
            field_0x148[player] -= 0.1f;
        } else {
            field_0x148[player] = 0.0f;
        }
        if (!fn_8022B160()) {
            if (field_0x158[player] > 0.0f) {
                field_0x158[player] -= 0.1f;
            } else {
                field_0x158[player] = 0.0f;
            }
        }
        gGameRoot2_8053AB10->field_0x20->field_0x217[player] = 1;
    } else if (lbl_80529DEC.field_0xd8[player] == 0 && lbl_8053AEC8->field_0x5e == 0) {
        if (prev0 > 0.6f) {
            field_0x108[player] -= 0.1f;
            if (field_0x108[player] <= 0.6f) {
                field_0x108[player] = 0.6f;
            }
        } else if (prev0 < 0.6f) {
            field_0x108[player] += 0.1f;
            if (field_0x108[player] >= 0.6f) {
                field_0x108[player] = 0.6f;
            }
        }
        if (prev1 > 0.6f) {
            field_0x118[player] -= 0.1f;
            if (field_0x118[player] <= 0.6f) {
                field_0x118[player] = 0.6f;
            }
        } else if (prev1 < 0.6f) {
            field_0x118[player] += 0.1f;
            if (field_0x118[player] >= 0.6f) {
                field_0x118[player] = 0.6f;
            }
        }
        if (prev2 > 0.6f) {
            field_0x138[player] -= 0.1f;
            if (field_0x138[player] <= 0.6f) {
                field_0x138[player] = 0.6f;
            }
        } else if (prev2 < 0.6f) {
            field_0x138[player] += 0.1f;
            if (field_0x138[player] >= 0.6f) {
                field_0x138[player] = 0.6f;
            }
        }
        if (prev3 > 0.6f) {
            field_0x148[player] -= 0.1f;
            if (field_0x148[player] <= 0.6f) {
                field_0x148[player] = 0.6f;
            }
        } else if (prev3 < 0.6f) {
            field_0x148[player] += 0.1f;
            if (field_0x148[player] >= 0.6f) {
                field_0x148[player] = 0.6f;
            }
        }
        if (prev4 > 0.6f) {
            field_0x158[player] -= 0.1f;
            if (field_0x158[player] <= 0.6f) {
                field_0x158[player] = 0.6f;
            }
        } else if (prev4 < 0.6f) {
            field_0x158[player] += 0.1f;
            if (field_0x158[player] >= 0.6f) {
                field_0x158[player] = 0.6f;
            }
        }
    } else {
        if (prev0 < 1.0f) {
            field_0x108[player] += 0.1f;
        } else {
            field_0x108[player] = 1.0f;
        }
        if (prev1 < 1.0f) {
            field_0x118[player] += 0.1f;
        } else {
            field_0x118[player] = 1.0f;
        }
        if (prev2 < 1.0f) {
            field_0x138[player] += 0.1f;
        } else {
            field_0x138[player] = 1.0f;
        }
        if (prev3 < 1.0f) {
            field_0x148[player] += 0.1f;
        } else {
            field_0x148[player] = 1.0f;
        }
        if (prev4 < 1.0f) {
            field_0x158[player] += 0.1f;
        } else {
            field_0x158[player] = 1.0f;
        }
    }
    if (prev0 == field_0x108[player] && prev1 == field_0x118[player] && prev2 == field_0x138[player] &&
        prev3 == field_0x148[player] && prev4 == field_0x158[player] && !arg) {
        return;
    }
    field_0x4->search(lbl_8048D818[player][0])->setAlpha(field_0x24c[0] * field_0x138[player]);
    field_0x4->search(lbl_8048D818[player][1])->setAlpha(field_0x24c[1] * field_0x138[player]);
    for (s32 i = 0; i < 5; i++) {
        field_0x4->search(lbl_8048D6D8[player][i])->setAlpha(field_0x24c[2] * field_0x158[player]);
        field_0x4->search(lbl_8048D778[player][i])->setAlpha(field_0x24c[3] * field_0x158[player]);
    }
    for (s32 i = 0; i < 12; i++) {
        field_0x4->search(lbl_8048D858[player][i])->setAlpha(field_0x25e * field_0x148[player]);
        field_0x4->search(lbl_8048D9D8[player][i])->setAlpha(field_0x25f * field_0x148[player]);
    }
    for (s32 i = 0; i < 3; i++) {
        field_0x4->search(lbl_8048DB58[player][i])->setAlpha(field_0x258[i] * field_0x148[player]);
        field_0x4->search(lbl_8048DBB8[player][i])->setAlpha(field_0x25b[i] * field_0x148[player]);
    }
    for (s32 i = 0; i < 6; i++) {
        field_0x4->search(lbl_8048DC18[player][i])->setAlpha(field_0x260[i] * field_0x108[player]);
    }
    field_0x4->search(lbl_8048DE20[0x8 + player])->setAlpha(field_0x266 * field_0x108[player]);
    field_0x18->search(lbl_8048DE20[0x4 + player])->setAlpha(field_0x267 * field_0x108[player]);
    field_0x18->search(lbl_8048DE00[player])->setAlpha(field_0x268 * field_0x138[player]);
    field_0x18->search(lbl_8048DE20[player])->setAlpha(field_0x269 * field_0x138[player]);
    field_0x4->search(lbl_8048DE20[0x14 + player])->setAlpha(field_0x26b * field_0x148[player]);
    field_0x4->search(lbl_8048DCD8[player])->setAlpha(field_0x26a * field_0x148[player]);
    for (s32 i = 0; i < 4; i++) {
        field_0x4->search(lbl_8048DCF8[player][i])->setAlpha(field_0x26c[i] * field_0x148[player]);
    }
}

// FIXME(shotgun): 81.76 %, missing/extra + reloc-name
void cls_804B13D4::fn_80419570(s32 player) {
    if (field_0x21a[player] == 0) {
        field_0x1d8[player] = fn_80237774(player);
        field_0x1f8[player] = fn_80237810(player);
        return;
    }
    s32 max = field_0x1f8[player];
    s32 life = field_0x1d8[player];
    if (max > 32) {
        max = 32;
    }
    if (max < 0) {
        max = 0;
    }
    if (max < life) {
        life = max;
    }
    if (life < 0) {
        life = 0;
    }
    for (s32 i = 0; i < 8; i++) {
        if (life >= 4) {
            field_0x8[player]->search(lbl_8048DE20[0x24 + i])->show();
            field_0x8[player]->search(lbl_8048DE20[0x34 + i])->show();
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048DE20[0x24 + i]))->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048DE20[0x34 + i]))->setBlendRatio(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x8[player]->search(lbl_8048DE20[0x2C + i])->hide();
            field_0x8[player]->search(lbl_8048DE20[0x3C + i])->hide();
            max -= 4;
            life -= 4;
            continue;
        }
        if (life == 0) {
            field_0x8[player]->search(lbl_8048DE20[0x24 + i])->hide();
            field_0x8[player]->search(lbl_8048DE20[0x34 + i])->hide();
            if (max > 0) {
                field_0x8[player]->search(lbl_8048DE20[0x2C + i])->show();
                field_0x8[player]->search(lbl_8048DE20[0x3C + i])->show();
                max -= 4;
            } else {
                field_0x8[player]->search(lbl_8048DE20[0x2C + i])->hide();
                field_0x8[player]->search(lbl_8048DE20[0x3C + i])->hide();
            }
        } else if (life == 1) {
            field_0x8[player]->search(lbl_8048DE20[0x24 + i])->show();
            field_0x8[player]->search(lbl_8048DE20[0x34 + i])->show();
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048DE20[0x24 + i]))->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048DE20[0x34 + i]))->setBlendRatio(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x8[player]->search(lbl_8048DE20[0x2C + i])->hide();
            field_0x8[player]->search(lbl_8048DE20[0x3C + i])->hide();
            max -= 4;
        } else if (life == 2) {
            field_0x8[player]->search(lbl_8048DE20[0x24 + i])->show();
            field_0x8[player]->search(lbl_8048DE20[0x34 + i])->show();
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048DE20[0x24 + i]))->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048DE20[0x34 + i]))->setBlendRatio(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x8[player]->search(lbl_8048DE20[0x2C + i])->hide();
            field_0x8[player]->search(lbl_8048DE20[0x3C + i])->hide();
            max -= 4;
        } else if (life == 3) {
            field_0x8[player]->search(lbl_8048DE20[0x24 + i])->show();
            field_0x8[player]->search(lbl_8048DE20[0x34 + i])->show();
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048DE20[0x24 + i]))->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            static_cast<J2DPicture*>(field_0x8[player]->search(lbl_8048DE20[0x34 + i]))->setBlendRatio(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            field_0x8[player]->search(lbl_8048DE20[0x2C + i])->hide();
            field_0x8[player]->search(lbl_8048DE20[0x3C + i])->hide();
            max -= 4;
        }
        life = 0;
    }
    field_0x21a[player]--;
}

void cls_804B13D4::fn_8041918C(s32 player) {
    if (fn_802366CC(fn_8022EA0C(player)) > 0 && fn_802366CC(fn_8022EA0C(player)) < 300) {
        s32 level = fn_802366CC(fn_8022EA0C(player)) / 60 + 1;
        if (level != field_0x294[player]) {
            if (level > 99) {
                level = 99;
            }
            if (level < 0) {
                level = 0;
            }
            if (level < 10) {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[level]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[0]))->changeTexture(res, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[1]))->changeTexture(res, 0);
                if (field_0x2c[player]->search(lbl_8048DDD0[2])->isVisible() == true) {
                    field_0x2c[player]->search(lbl_8048DDD0[2])->hide();
                    field_0x2c[player]->search(lbl_8048DDD0[3])->hide();
                }
            } else {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[level / 10]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[0]))->changeTexture(res, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[1]))->changeTexture(res, 0);
                res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[level % 10]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[2]))->changeTexture(res, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[3]))->changeTexture(res, 0);
                if (!field_0x2c[player]->search(lbl_8048DDD0[2])->isVisible()) {
                    field_0x2c[player]->search(lbl_8048DDD0[2])->show();
                    field_0x2c[player]->search(lbl_8048DDD0[3])->show();
                }
            }
            field_0x294[player] = level;
        }
    } else {
        field_0x294[player] = 0;
    }
    if (field_0x234[player] > 0) {
        field_0x234[player]--;
    }
}

// FIXME(shotgun): 91.26 %, missing/extra
void cls_804B13D4::fn_80418D08(s32 player, bool force) {
    u8 bit = 1 << player;
    f32 prevA = field_0x128[player];
    f32 prevB = field_0x168[player];
    if (!(field_0x28f & bit)) {
        if (prevA > 0.0f) {
            field_0x128[player] = prevA - 0.1f;
        } else {
            field_0x128[player] = 0.0f;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x217[player] = 1;
    } else if (lbl_80529DEC.field_0xd8[player] == 0 && lbl_8053AEC8->field_0x5e == 0) {
        if (prevA > 0.6f) {
            field_0x128[player] = prevA - 0.1f;
            if (field_0x128[player] <= 0.6f) {
                field_0x128[player] = 0.6f;
            }
        } else if (prevA < 0.6f) {
            field_0x128[player] = prevA + 0.1f;
            if (field_0x128[player] >= 0.6f) {
                field_0x128[player] = 0.6f;
            }
        }
    } else if (prevA < 1.0f) {
        field_0x128[player] += 0.1f;
    } else {
        field_0x128[player] = 1.0f;
    }
    if (prevA == field_0x128[player] && !force) {
        return;
    }
    if (!(field_0x28f & bit) && !fn_8022B1AC()) {
        if (field_0x168[player] > 0.0f) {
            field_0x168[player] -= 0.1f;
        } else {
            field_0x168[player] = 0.0f;
        }
    } else if (lbl_80529DEC.field_0xd8[player] == 0 && lbl_8053AEC8->field_0x5e == 0) {
        if (field_0x168[player] > 0.6f) {
            field_0x168[player] -= 0.1f;
            if (field_0x168[player] <= 0.6f) {
                field_0x168[player] = 0.6f;
            }
        } else if (field_0x168[player] < 0.6f) {
            field_0x168[player] += 0.1f;
            if (field_0x168[player] >= 0.6f) {
                field_0x168[player] = 0.6f;
            }
        }
    } else if (field_0x168[player] < 1.0f) {
        field_0x168[player] += 0.1f;
    } else {
        field_0x168[player] = 1.0f;
    }
    if (prevA == field_0x128[player] && prevB == field_0x168[player] && !force) {
        return;
    }
    for (s32 i = 0; i < 8; i++) {
        field_0x8[player]->search(lbl_8048DE20[0x24 + i])->setAlpha(field_0x250[0] * field_0x128[player]);
        field_0x8[player]->search(lbl_8048DE20[0x34 + i])->setAlpha(field_0x250[1] * field_0x128[player]);
        field_0x8[player]->search(lbl_8048DE20[0x2C + i])->setAlpha(field_0x250[2] * field_0x128[player]);
        field_0x8[player]->search(lbl_8048DE20[0x3C + i])->setAlpha(field_0x250[3] * field_0x128[player]);
    }
    for (s32 i = 0; i < 6; i++) {
        field_0x2c[player]->search(lbl_8048DDD0[i])->setAlpha(field_0x27b[i] * field_0x168[player]);
    }
}

s32 cls_804B13D4::fn_80418B64() {
    Vec screen;
    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(getPlayerPos_8023E724(fn_8022EA0C(0)), false, false, &screen);
    if (getPlayerLayer_8023DE58(0) >= 8) {
        field_0x218 = 1;
    } else {
        s32 height = JUTVideo::getManager()->getRenderMode()->efbHeight;
        if (screen.y < 0.33333334f * (f32)height) {
            field_0x218 = 0;
        } else if (screen.y > 0.6666667f * (f32)height) {
            field_0x218 = 1;
        }
    }
    s32 level;
    if (field_0x218 != 0) {
        u8 flags = lbl_8053AED0->field_0xdc;
        if (((flags >> 0) & 1) == 1) {
            if (((flags >> 1) & 1) == 1) {
                if (((flags >> 2) & 1) == 1) {
                    if (((flags >> 3) & 1) == 1) {
                        level = 4;
                    } else {
                        level = 3;
                    }
                } else {
                    level = 1;
                }
            } else {
                level = 2;
            }
        } else {
            level = 0;
        }
    } else {
        u8 flags = lbl_8053AED0->field_0xdc;
        if (((flags >> 2) & 1) == 1) {
            if (((flags >> 3) & 1) == 1) {
                if (((flags >> 0) & 1) == 1) {
                    if (((flags >> 1) & 1) == 1) {
                        level = 4;
                    } else {
                        level = 2;
                    }
                } else {
                    level = 0;
                }
            } else {
                level = 3;
            }
        } else {
            level = 1;
        }
    }
    return level;
}
