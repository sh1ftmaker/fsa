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

// J2DPane::search tags, as u64 (the names are padded with leading zeros).
static const u64 lbl_8048DDD0[6] = {
    0x00000050746B3031ull, /* "Ptk01" */
    0x0000000050743031ull, /* "Pt01" */
    0x00000050746B3032ull, /* "Ptk02" */
    0x0000000050743032ull, /* "Pt02" */
    0x00005074696D3031ull, /* "Ptim01" */
    0x00005074696D3032ull, /* "Ptim02" */
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
    reinterpret_cast<J2DScreen*>(field_0x18)->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x50 != NULL) {
        reinterpret_cast<cls_804B19E0*>(field_0x50)->fn_8044148C();
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
