#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80379F84.h"
#include "game/cls_804B16A8.h"
#include "game/cls_804B20F8.h"
#include "game/act/tu_80440BF4.h"
#include "game/tu_803E1668.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DAnimation.h"
#include "JSystem/J2D/J2DAnmLoader.h"
#include "game/Message.h"
#include "game/Map.h"
#include "game/Effect.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "dolphin/os.h"

// Entered when the state changes (index: the new state). The three tables
// are filled at startup by the unit's static initialiser, as in
// game/tu_803F0A54.cpp.
static cls_804B0B00::StateFunc lbl_804B0754[15] = {
    &cls_804B0B00::fn_803E4610, &cls_804B0B00::fn_803E45FC, &cls_804B0B00::fn_803E4344,
    &cls_804B0B00::fn_803E4330, &cls_804B0B00::fn_803E4308, &cls_804B0B00::fn_803E3CB0,
    &cls_804B0B00::fn_803E3C94, &cls_804B0B00::fn_803E3C80, &cls_804B0B00::fn_803E3C00,
    &cls_804B0B00::fn_803E3564, &cls_804B0B00::fn_803E3548, &cls_804B0B00::fn_803E3320,
    &cls_804B0B00::fn_803E2A20, &cls_804B0B00::fn_803E2A0C, &cls_804B0B00::fn_803E29C8,
};

// Run each frame (index: the state).
static cls_804B0B00::StateFunc lbl_804B08BC[15] = {
    &cls_804B0B00::fn_803ED604, &cls_804B0B00::fn_803ED5B4, &cls_804B0B00::fn_803ECC64,
    &cls_804B0B00::fn_803EC804, &cls_804B0B00::fn_803EBAA8, &cls_804B0B00::fn_803EAEC8,
    &cls_804B0B00::fn_803EA3F8, &cls_804B0B00::fn_803E9EE8, &cls_804B0B00::fn_803E9A20,
    &cls_804B0B00::fn_803E926C, &cls_804B0B00::fn_803E9034, &cls_804B0B00::fn_803E831C,
    &cls_804B0B00::fn_803E8210, &cls_804B0B00::fn_803E6AB0, &cls_804B0B00::fn_803E6A2C,
};

// Drawn each frame (index: the state).
static cls_804B0B00::StateFunc lbl_804B0A24[15] = {
    &cls_804B0B00::fn_803E4940, &cls_804B0B00::fn_803E6A28, &cls_804B0B00::fn_803E64A0,
    &cls_804B0B00::fn_803E64A0, &cls_804B0B00::fn_803E64A0, &cls_804B0B00::fn_803E51C4,
    &cls_804B0B00::fn_803E51C4, &cls_804B0B00::fn_803E51C4, &cls_804B0B00::fn_803E51C4,
    &cls_804B0B00::fn_803E4940, &cls_804B0B00::fn_803E4940, &cls_804B0B00::fn_803E4940,
    &cls_804B0B00::fn_803E4650, &cls_804B0B00::fn_803E4650, &cls_804B0B00::fn_803E462C,
};

// Cursor y positions by field_0x1c0[i] (.sdata2, lhax).
static const s16 lbl_80543648[4] = {-25, 25, 77, 77};

// .rodata: the unit's tables in address order (the functions address them
// off one base); u64 tables are J2DPane::search tags.
static const f32 lbl_80477970[16] = {
    50.0f, 206.0f, 50.0f, 206.0f, 60.0f, 60.0f, 132.0f, 132.0f, 87.0f, 464.0f, 87.0f, 464.0f, 159.0f, 159.0f, 327.0f, 327.0f,
};
static const s32 lbl_804779B0[30] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6,
};
static const u64 lbl_80477A28[17] = {
    'Pmadbb07', 'Pmadbb06', 'Pmadbb05', 'Pmadbb04', 'Pmadbb03', 'Pmadbb02', 'Pmadbb01', 'Pmadb05',
    'Pmadb04', 'Pmadb03', 'Pmadb02', 'Pmadb01', 'Pmad02', 'Pmad01', 'Phy03', 'Phy02', 'Phy01',
};
static const u64 lbl_80477AB0[4][6] = {
    {'Pnop01', 'Pnop02', 'Pnopb01', 'Pnopb02', 'Pnopb03', 'Pnopb04'},
    {'Pnop03', 'Pnop04', 'Pnopb05', 'Pnopb06', 'Pnopb07', 'Pnopb08'},
    {'Pnop05', 'Pnop06', 'Pnopb09', 'Pnopb10', 'Pnopb11', 'Pnopb12'},
    {'Pnop07', 'Pnop08', 'Pnopb13', 'Pnopb14', 'Pnopb15', 'Pnopb16'},
};
static const u64 lbl_80477B70[29] = {
    'Plibl01', 'Plibl02', 'Plibl03', 'Plibl04', 'Plw01', 'Plw02', 'Plw03', 'Plw04', 'Plw05',
    'Plw06', 'Plw07', 'Plw08', 'Plw09', 'Plw10', 'Plw11', 'Plw12', 'Plili01', 'Plili02', 'Plili03',
    'Plili04', 'Plib01', 'Plib02', 'Plib03', 'Plib04', 'Plib05', 'Plibk01', 'Plibk02', 'Plibk02',
    'Plibk03',
};
static const u64 lbl_80477C58[22] = {
    'P1pli01', 'P1pli02', 'P1pli03', 'P1pli04', 'P2pli01', 'P2pli02', 'P2pli03', 'P2pli04',
    'P3pli01', 'P3pli02', 'P3pli03', 'P3pli04', 'P3pli05', 'P3pli06', 'P4pli01', 'P4pli02',
    'P4pli03', 'P4pli04', 'P4pli05', 'P4pli06', 'P4pli07', 'P4pli08',
};
static const u64 lbl_80477D08[11] = {
    'Ttkg01', 'Ttkg02', 'Ptkgb01', 'Ptkgbk01', 'Ptkgb02', 'Ptkgbk02', 'Ptkgb03', 'Ptkgb04',
    'Ptkgb05', 'Ptkgbk03', 'Ptkgbk04',
};
static const u64 lbl_80477D60[6] = {
    'Tten01', 'Tten02', 'Ptgba01', 'Ptgba02', 'Ptn01', 'Ptn02',
};
static const u64 lbl_80477D90[34] = {
    'Pcgps01', 'Pcgp01', 'Pcgba01', 'Pcgbak01', 'Pcgps02', 'Pcgp02', 'Pcgba02', 'Pcgbak02',
    'Pcgps03', 'Pcgp03', 'Pcgba03', 'Pcgbak03', 'Pcgps04', 'Pcgp04', 'Pcgba04', 'Pcgbak04', 'Pcc01',
    'Pccaa01', 'Pcc02', 'Pccaa02', 'Pcc03', 'Pccaa03', 'Pcc04', 'Pccaa04', 'Pcca01', 'Pcca02',
    'Pcca03', 'Pcca04', 'Pcca05', 'Pcca06', 'Pcca07', 'Pcca08', 'Pccu01', 'Pccuk01',
};
static const u64 lbl_80477EA0[4][9] = {
    {'Ppln01', 'Pplli01', 'Ppllb01', 'Ppllb02', 'Ppllb03', 'Ppllb04', 'Ppllb05', 'Ppllbk01', 'Ppllbk02'},
    {'Ppln02', 'Pplli02', 'Ppllb11', 'Ppllb12', 'Ppllb13', 'Ppllb14', 'Ppllb15', 'Ppllbk11', 'Ppllbk12'},
    {'Ppln03', 'Pplli03', 'Ppllb21', 'Ppllb22', 'Ppllb23', 'Ppllb24', 'Ppllb25', 'Ppllbk21', 'Ppllbk22'},
    {'Ppln04', 'Pplli04', 'Ppllb31', 'Ppllb32', 'Ppllb33', 'Ppllb34', 'Ppllb35', 'Ppllbk31', 'Ppllbk32'},
};
static const u64 lbl_80477FC0[17] = {
    'Pmadbb07', 'Pmadbb06', 'Pmadbb05', 'Pmadbb04', 'Pmadbb03', 'Pmadbb02', 'Pmadbb01', 'Pmadb05',
    'Pmadb04', 'Pmadb03', 'Pmadb02', 'Pmadb01', 'Pmad02', 'Pmad01', 'Phy03', 'Phy02', 'Phy01',
};
static const u64 lbl_80478048[3][5] = {
    {'Tsta01', 'Pstab01', 'Pstab02', 'Pstab03', 'Pstab04'},
    {'Than01', 'Phanb01', 'Phanb02', 'Phanb03', 'Phanb04'},
    {'Trule01', 'Prub01', 'Prub02', 'Prub03', 'Prub04'},
};
static const u64 lbl_804780C0[26] = {
    'Pcon01', 'Pcon02', 'Pgba01', 'Pgba02', 'Pnop01', 'Pnop02', 'Plwa01', 'Plwa02', 'Plwa03',
    'Plwa04', 'Plwa05', 'Plwa06', 'Plwa07', 'Plwa08', 'Plwa09', 'Plwa10', 'Plibk01', 'Plibk02',
    'Plibk03', 'Plibk04', 'Plibk11', 'Plibk12', 'Plibk05', 'Plibkk01', 'Plibkk02', 'Plibkk03',
};
static const u64 lbl_80478190[4] = {
    'Pplli01', 'Pplli02', 'Pplli03', 'Pplli04',
};
static const u64 lbl_804781B0[4] = {
    'Pcgbak01', 'Pcgbak02', 'Pcgbak03', 'Pcgbak04',
};
static const u64 lbl_804781D0[4] = {
    'Pcgba01', 'Pcgba02', 'Pcgba03', 'Pcgba04',
};
static const u64 lbl_804781F0[4] = {
    'Pcgp01', 'Pcgp02', 'Pcgp03', 'Pcgp04',
};
static const u64 lbl_80478210[4] = {
    'Pcgps01', 'Pcgps02', 'Pcgps03', 'Pcgps04',
};
static const u64 lbl_80478230[4] = {
    'Ppln01', 'Ppln02', 'Ppln03', 'Ppln04',
};
static const u64 lbl_80478250[4][4] = {
    {'Pcc01', 'Pccaa01', 'Pcca01', 'Pcca02'},
    {'Pcc02', 'Pccaa02', 'Pcca03', 'Pcca04'},
    {'Pcc03', 'Pccaa03', 'Pcca05', 'Pcca06'},
    {'Pcc04', 'Pccaa04', 'Pcca07', 'Pcca08'},
};
static const u64 lbl_804782D0[4] = {
    'Par01', 'Par02', 'Par03', 'Par04',
};
static const u64 lbl_804782F0[4] = {
    'Ndatay01', 'Ndatay02', 'Ndatay03', 'Ndatay04',
};
static const u64 lbl_80478310[4] = {
    'Pbcur01', 'Pbcur02', 'Pbcur03', 'Pbcur04',
};
static const u64 lbl_80478330[4] = {
    'Pscur01', 'Pscur02', 'Pscur03', 'Pscur04',
};
static const u64 lbl_80478350[4] = {
    'Paccur02', 'Paccur04', 'Paccur06', 'Paccur08',
};
static const u64 lbl_80478370[4] = {
    'Paccur01', 'Paccur03', 'Paccur05', 'Paccur07',
};
static const u64 lbl_80478390[4] = {
    'P1s03', 'P2s03', 'P3s03', 'P4s03',
};
static const u64 lbl_804783B0[4] = {
    'P1sk03', 'P2sk03', 'P3sk03', 'P4sk03',
};
static const u64 lbl_804783D0[4] = {
    'P1s02', 'P2s02', 'P3s02', 'P4s02',
};
static const u64 lbl_804783F0[4] = {
    'P1sk02', 'P2sk02', 'P3sk02', 'P4sk02',
};
static const u64 lbl_80478410[4] = {
    'P1s01', 'P2s01', 'P3s01', 'P4s01',
};
static const u64 lbl_80478430[4] = {
    'P1sk01', 'P2sk01', 'P3sk01', 'P4sk01',
};
// The rupee digit textures (the strings are emitted here, between the tag
// tables).
static const char* lbl_804B0AD8[10] = {
    "kk_rupyNum_00.bti", "kk_rupyNum_01.bti", "kk_rupyNum_02.bti", "kk_rupyNum_03.bti",
    "kk_rupyNum_04.bti", "kk_rupyNum_05.bti", "kk_rupyNum_06.bti", "kk_rupyNum_07.bti",
    "kk_rupyNum_08.bti", "kk_rupyNum_09.bti",
};
static const u64 lbl_80478518[4][6] = {
    {'Pp1lf01', 'Pp1lf02', 'Pp1lf03', 'Pp1lf04', 'Pp1lf05', 'Pp1lf06'},
    {'Pp2lf01', 'Pp2lf02', 'Pp2lf03', 'Pp2lf04', 'Pp2lf05', 'Pp2lf06'},
    {'Pp3lf01', 'Pp3lf02', 'Pp3lf03', 'Pp3lf04', 'Pp3lf05', 'Pp3lf06'},
    {'Pp4lf01', 'Pp4lf02', 'Pp4lf03', 'Pp4lf04', 'Pp4lf05', 'Pp4lf06'},
};
static const u64 lbl_804785D8[4][6] = {
    {'Pp1lfk01', 'Pp1lfk02', 'Pp1lfk03', 'Pp1lfk04', 'Pp1lfk05', 'Pp1lfk06'},
    {'Pp2lfk01', 'Pp2lfk02', 'Pp2lfk03', 'Pp2lfk04', 'Pp2lfk05', 'Pp2lfk06'},
    {'Pp3lfk01', 'Pp3lfk02', 'Pp3lfk03', 'Pp3lfk04', 'Pp3lfk05', 'Pp3lfk06'},
    {'Pp4lfk01', 'Pp4lfk02', 'Pp4lfk03', 'Pp4lfk04', 'Pp4lfk05', 'Pp4lfk06'},
};
static const u64 lbl_80478698[4][6] = {
    {'Pp1lf09', 'Pp1lf10', 'Pp1lf11', 'Pp1lf12', 'Pp1lf13', 'Pp1lf14'},
    {'Pp2lf09', 'Pp2lf10', 'Pp2lf11', 'Pp2lf12', 'Pp2lf13', 'Pp2lf14'},
    {'Pp3lf09', 'Pp3lf10', 'Pp3lf11', 'Pp3lf12', 'Pp3lf13', 'Pp3lf14'},
    {'Pp4lf09', 'Pp4lf10', 'Pp4lf11', 'Pp4lf12', 'Pp4lf13', 'Pp4lf14'},
};
static const u64 lbl_80478758[4][6] = {
    {'Pp1lfk09', 'Pp1lfk10', 'Pp1lfk11', 'Pp1lfk12', 'Pp1lfk13', 'Pp1lfk14'},
    {'Pp2lfk09', 'Pp2lfk10', 'Pp2lfk11', 'Pp2lfk12', 'Pp2lfk13', 'Pp2lfk14'},
    {'Pp3lfk09', 'Pp3lfk10', 'Pp3lfk11', 'Pp3lfk12', 'Pp3lfk13', 'Pp3lfk14'},
    {'Pp4lfk09', 'Pp4lfk10', 'Pp4lfk11', 'Pp4lfk12', 'Pp4lfk13', 'Pp4lfk14'},
};
static const u64 lbl_80478818[4] = {
    'Pnopb04', 'Pnopb08', 'Pnopb12', 'Pnopb16',
};
static const u64 lbl_80478838[4] = {
    'Plibl01', 'Plibl02', 'Plibl03', 'Plibl04',
};
static const u64 lbl_80478858[4] = {
    'Plili01', 'Plili02', 'Plili03', 'Plili04',
};
static const u64 lbl_80478878[3] = {
    'Pblk01', 'Pblk02', 'Pblk03',
};
static const u64 lbl_80478890[4][4] = {
    {'P1pli01', 0, 0, 0},
    {'P2pli01', 'P2pli02', 0, 0},
    {'P3pli01', 'P3pli02', 'P3pli03', 0},
    {'P4pli01', 'P4pli02', 'P4pli03', 'P4pli04'},
};
// A pane's initial alpha scaled by rate. The constructor fades every pane
// out with rate 0.0f: the original still multiplies (the constants reloaded
// for each loop), which a literal `alpha * 0.0f` in the loop would fold.
static inline u8 fadeAlpha(u8 alpha, f32 rate) {
    return alpha * rate;
}

// Sets the 2-bit field `player` of root->field_0x83 to `kind` (as in
// game/tu_803F0A54.cpp).
static inline void setPlayerKind(GameRoot_8049EF20* root, s32 player, u8 kind) {
    root->field_0x83 &= ~(u8)(3 << (2 * player));
    root->field_0x83 |= (u8)(kind << (2 * player));
}

static inline s16 nextUnconnected(s16 start, s32 count) {
    s16 cur = start;
    for (s32 i = 0; i < count; i++) {
        cur++;
        if (cur >= count) {
            cur = 0;
        }
        if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (cur + 4)))) {
            return cur;
        }
        if (cur == start) {
            return -1;
        }
    }
    return -1;
}

// Sets bit `i` of gGameRoot_8053AAF8->mPlayerMask and the 2-bit field `i` of
// field_0x83 to 1 for the field_0x1bc + 1 players that take part, and clears
// both for the others.
static inline void setEntries(cls_804B0B00* self) {
    for (s32 i = 0; i < 4; i++) {
        if (i < self->field_0x1bc + 1) {
            gGameRoot_8053AAF8->mPlayerMask |= (u8)(1 << i);
            setPlayerKind(gGameRoot_8053AAF8, i, 1);
        } else {
            gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << i);
            gGameRoot_8053AAF8->field_0x83 &= ~(u8)(3 << (2 * i));
        }
    }
}

// One player: sets bit 4 (connected) and bit 0 (taking part) of
// gGameRoot_8053AAF8->mPlayerMask for player 0 only, and player 0's kind to 1 with
// a controller in slot 0, else 2. fn_803E3320 (not written: written here
// it gets inlined into fn_803E9034 / fn_803ED604, which call it out of line
// in the original) has the same block, then setEntries for more players.
static inline void setSingleEntry() {
    gGameRoot_8053AAF8->mPlayerMask |= (u8)(1 << 4);
    for (s32 i = 1; i < 4; i++) {
        gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << (i + 4));
    }
    gGameRoot_8053AAF8->mPlayerMask |= (u8)(1 << 0);
    for (s32 i = 1; i < 4; i++) {
        gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << i);
    }
    if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(0))) {
        setPlayerKind(gGameRoot_8053AAF8, 0, 1);
    } else {
        setPlayerKind(gGameRoot_8053AAF8, 0, 2);
    }
    for (s32 i = 1; i < 4; i++) {
        gGameRoot_8053AAF8->field_0x83 &= ~(u8)(3 << (2 * i));
    }
}

// Steps the cursor's blink counter (field_0x254[0]) and the frame counter (field_0x220[0]).
// Shared by fn_803E9034 and fn_803ED604 (the two menu update functions).
static inline void stepBlink(cls_804B0B00* self) {
    if (self->field_0x254[0] != 0) {
        self->field_0x254[0]++;
        if (self->field_0x254[0] > 10) {
            self->field_0x254[0] = 0;
        }
    }
    self->field_0x220[0]++;
    if (self->field_0x220[0] >= 30) {
        self->field_0x220[0] = 0;
    }
}

static inline void setBounce(cls_804B0B00* self, s32 i) {
    if (i <= self->field_0x1bc) {
        self->field_0x254[i] = 1;
    }
}

static inline void setBounceIfReady(cls_804B0B00* self, s32 i) {
    if (i <= self->field_0x1bc && self->field_0x1c0[i] == 4) {
        self->field_0x254[i] = 1;
    }
}

static inline void updateHandicapCursor(cls_804B0B00* self, s32 i) {
    if (i <= self->field_0x1bc) {
        if (self->field_0x1c0[i] < 3) {
            self->field_0x10->search(lbl_80478310[i])->hide();
            self->field_0x10->search(lbl_80478330[i])->show();
            self->field_0x10->search(lbl_80478350[i])->hide();
        } else if (self->field_0x1c0[i] == 3) {
            self->field_0x10->search(lbl_80478310[i])->hide();
            self->field_0x10->search(lbl_80478330[i])->show();
            self->field_0x10->search(lbl_80478350[i])->show();
        } else {
            self->field_0x10->search(lbl_80478310[i])->show();
            self->field_0x10->search(lbl_80478330[i])->hide();
            self->field_0x10->search(lbl_80478350[i])->hide();
        }
    } else {
        self->field_0x10->search(lbl_80478310[i])->hide();
        self->field_0x10->search(lbl_80478330[i])->hide();
        self->field_0x10->search(lbl_80478350[i])->hide();
    }
    s32 state = self->field_0x1c0[i];
    if (state <= 3) {
        f32 x = self->field_0x10->search(lbl_80478330[i])->getBounds()->i.x;
        self->field_0x10->search(lbl_80478330[i])->move(x, lbl_80543648[state]);
    }
}

// The centre of a pane of field_0xc. Macros: the original evaluates the tag
// expression (table index, field_0x1bc and all) again for each search, and
// reads the size before the second search (as in game/tu_803F0A54.cpp).
#define PANE_CENTRE_X(dst, tag)                                                    \
    do {                                                                           \
        f32 size = field_0xc->search(tag)->getWidth();                             \
        dst = size / 2 + field_0xc->search(tag)->getGlobalBounds().i.x;         \
    } while (0)
#define PANE_CENTRE_Y(dst, tag)                                                    \
    do {                                                                           \
        f32 size = field_0xc->search(tag)->getHeight();                            \
        dst = size / 2 + field_0xc->search(tag)->getGlobalBounds().i.y;         \
    } while (0)

// Screen coordinates (640 x 480 layout scaled to 280 x 210) of a point on
// the current render mode.
static inline f32 toSpriteX(f32 x) {
    return 280.0f * x / (s32)JUTVideo::getManager()->getRenderMode()->fbWidth;
}

static inline f32 toSpriteY(f32 y) {
    return 210.0f * (8.0f + y) / (s32)JUTVideo::getManager()->getRenderMode()->efbHeight;
}

// The same point scaled to 224 x 168 (the effect layer).
static inline f32 toEffectX(f32 x) {
    return 224.0f * x / (s32)JUTVideo::getManager()->getRenderMode()->fbWidth;
}

static inline f32 toEffectY(f32 y) {
    return 168.0f * (8.0f + y) / (s32)JUTVideo::getManager()->getRenderMode()->efbHeight;
}

// FIXME: 99.55. Registers only in the pane-tag loops (e.g. the first loop's
// array pointers are r24 / r26 in the original, r26 / r27 here).
cls_804B0B00::cls_804B0B00() {
    field_0x1b4 = 0;
    field_0x1bc = lbl_80529DEC.field_0x23c;
    if (field_0x1bc == 0) {
        field_0x1b8 = 1;
    } else {
        field_0x1b8 = 0;
    }
    field_0x1d0 = 0;
    field_0x264 = 0;
    field_0x26b = 0;
    field_0x26c = 0;
    field_0x1d8 = 2;
    field_0x1d4 = 2;
    field_0x1dc = 0;
    for (int i = 0; i < 4; i++) {
        field_0x1e0[i] = 0;
        field_0x1f0[i] = fn_80403414(i);
        field_0x200[i] = fn_804033D4(i);
        field_0x210[i] = fn_8040336C(i);
        field_0x220[i] = 0;
        field_0x1c0[i] = 0;
        field_0x254[i] = 0;
    }
    field_0x25e = 0;
    field_0x25c = 0;
    field_0x26e = 0;
    field_0x26d = 0;
    field_0x260 = 0;
    field_0x360 = 4;

    mScreen = new J2DScreen;
    mScreen->setPriority("adv_n_player.blo", 0x1100000, lbl_80529DEC.field_0x28);
    field_0x8 = new J2DScreen;
    field_0x8->setPriority("adv_entry.blo", 0x1100000, lbl_80529DEC.field_0x28);
    field_0xc = new J2DScreen;
    field_0xc->setPriority("adv_entry_con.blo", 0x1100000, lbl_80529DEC.field_0x28);
    field_0x10 = new J2DScreen;
    field_0x10->setPriority("handicap.blo", 0x1100000, lbl_80529DEC.field_0x28);
    field_0x14 = new J2DScreen;
    field_0x14->setPriority("adv_back.blo", 0x1100000, lbl_80529DEC.field_0x28);
    for (int i = 0; i < 4; i++) {
        field_0x18[i] = new J2DScreen;
        field_0x18[i]->setPriority("entry_arrow_anim.blo", 0x1100000, lbl_80529DEC.field_0x28);
        field_0x28[i] = new J2DScreen;
        field_0x28[i]->setPriority("entry_connect_anim.blo", 0x1100000, lbl_80529DEC.field_0x28);
    }
    OSInitFastCast();
    void* arrowAnm = fn_804031C4('SCRN', "entry_arrow_anim.bck");
    void* connectAnm = fn_804031C4('SCRN', "entry_connect_anim.bck");
    for (int i = 0; i < 4; i++) {
        field_0x38[i] = static_cast<J2DAnmTransform*>(J2DAnmLoaderDataBase::load(arrowAnm));
        field_0x48[i] = static_cast<J2DAnmTransform*>(J2DAnmLoaderDataBase::load(connectAnm));
        field_0x18[i]->search('NULL_001')->setAnimation(field_0x38[i]);
        field_0x28[i]->search('Ndatay00')->setAnimation(field_0x48[i]);
        field_0x8->search(lbl_804782D0[i])->hide();
        field_0x230[i] = 0;
        field_0x240[i] = 0;
    }
    field_0x250 = -1;
    field_0x252 = -1;

    // mScreen: the initial alphas, then every pane faded out.
    for (int i = 0; i < 17; i++) {
        field_0x26f[i] = mScreen->search(lbl_80477A28[i])->getAlpha();
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            field_0x280[i][j] = mScreen->search(lbl_80477AB0[i][j])->getAlpha();
        }
    }
    for (int i = 0; i < 29; i++) {
        field_0x298[i] = mScreen->search(lbl_80477B70[i])->getAlpha();
    }
    for (int i = 0; i < 22; i++) {
        field_0x2b5[i] = mScreen->search(lbl_80477C58[i])->getAlpha();
    }
    for (int i = 0; i < 17; i++) {
        mScreen->search(lbl_80477A28[i])->setAlpha(fadeAlpha(field_0x26f[i], 0.0f));
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            mScreen->search(lbl_80477AB0[i][j])->setAlpha(fadeAlpha(field_0x280[i][j], 0.0f));
        }
    }
    for (int i = 0; i < 29; i++) {
        mScreen->search(lbl_80477B70[i])->setAlpha(fadeAlpha(field_0x298[i], 0.0f));
    }
    for (int i = 0; i < 22; i++) {
        mScreen->search(lbl_80477C58[i])->setAlpha(fadeAlpha(field_0x2b5[i], 0.0f));
    }
    field_0x268 = 156;
    field_0x19c = mScreen->search('Pscuro02')->getBounds()->i.x;
    field_0x1a0 = mScreen->search('Pscuro02')->getBounds()->i.y;
    field_0xf0 = mScreen->search('Pmadbb07')->getBounds()->i.x;
    field_0xf4 = mScreen->search('Pmadbb07')->getBounds()->i.y;
    field_0xf8 = 0.0f;
    field_0xfc = 0.0f;
    field_0x100[0] = mScreen->search('Pnopb04')->getBounds()->i.x;
    field_0x110[0] = mScreen->search('Pnopb04')->getBounds()->i.y;
    field_0x120[0] = 0.0f;
    field_0x100[1] = mScreen->search('Pnopb08')->getBounds()->i.x;
    field_0x110[1] = mScreen->search('Pnopb08')->getBounds()->i.y;
    field_0x120[1] = 0.0f;
    field_0x100[2] = mScreen->search('Pnopb12')->getBounds()->i.x;
    field_0x110[2] = mScreen->search('Pnopb12')->getBounds()->i.y;
    field_0x120[2] = 0.0f;
    field_0x100[3] = mScreen->search('Pnopb16')->getBounds()->i.x;
    field_0x110[3] = mScreen->search('Pnopb16')->getBounds()->i.y;
    field_0x120[3] = 0.0f;
    field_0x130 = mScreen->search('Ndata02')->getBounds()->i.x;
    field_0x134 = mScreen->search('Ndata02')->getBounds()->i.y;
    field_0x138 = 0.0f;
    field_0x13c = mScreen->search('Ndata05')->getBounds()->i.x;
    field_0x140 = mScreen->search('Ndata05')->getBounds()->i.y;
    field_0x144 = mScreen->search('Ndata07')->getBounds()->i.x;
    field_0x148 = mScreen->search('Ndata07')->getBounds()->i.y;
    field_0x14c = 0.0f;

    // field_0x8 (the entry screen).
    for (int i = 0; i < 4; i++) {
        field_0x8->search(lbl_80478190[i])->hide();
        for (int j = 0; j < 2; j++) {
            field_0x8->search(lbl_80478250[i][j])->hide();
        }
    }
    for (int i = 0; i < 11; i++) {
        field_0x2cb[i] = field_0x8->search(lbl_80477D08[i])->getAlpha();
    }
    for (int i = 0; i < 6; i++) {
        field_0x2d6[i] = field_0x8->search(lbl_80477D60[i])->getAlpha();
    }
    for (int i = 0; i < 34; i++) {
        field_0x2dc[i] = field_0x8->search(lbl_80477D90[i])->getAlpha();
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 9; j++) {
            field_0x2fe[i][j] = field_0x8->search(lbl_80477EA0[i][j])->getAlpha();
        }
    }
    field_0x265 = field_0x8->search(lbl_804781B0[0])->getAlpha();
    field_0x266 = field_0x8->search(lbl_804781D0[0])->getAlpha();
    field_0x267 = field_0x8->search(lbl_804781F0[0])->getAlpha();
    for (int i = 0; i < 11; i++) {
        field_0x8->search(lbl_80477D08[i])->setAlpha(fadeAlpha(field_0x2cb[i], 0.0f));
    }
    for (int i = 0; i < 6; i++) {
        field_0x8->search(lbl_80477D60[i])->setAlpha(fadeAlpha(field_0x2d6[i], 0.0f));
    }
    for (int i = 0; i < 34; i++) {
        field_0x8->search(lbl_80477D90[i])->setAlpha(fadeAlpha(field_0x2dc[i], 0.0f));
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 9; j++) {
            field_0x8->search(lbl_80477EA0[i][j])->setAlpha(fadeAlpha(field_0x2fe[i][j], 0.0f));
        }
    }
    JUTResFont* font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x8->search('Tten02'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x8->search('Tten01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x8->search('Ttkg02'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x8->search('Ttkg01'))->setFont(font);
    getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x8->search('Tten02'))->getStringPtr(), 0xA6, 0, NULL, NULL);
    getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x8->search('Tten01'))->getStringPtr(), 0xA6, 0, NULL, NULL);
    field_0x98 = field_0x8->search('Ndata03')->getBounds()->i.x;
    field_0x9c = field_0x8->search('Ndata03')->getBounds()->i.y;
    field_0xa0 = 0.0f;
    field_0xa4 = field_0x8->search('Ndata05')->getBounds()->i.x;
    field_0xa8 = field_0x8->search('Ndata05')->getBounds()->i.y;
    field_0xac = 0.0f;
    field_0xb0 = field_0x8->search('Ndata06')->getBounds()->i.x;
    field_0xb4 = field_0x8->search('Ndata06')->getBounds()->i.y;
    field_0xb8 = 0.0f;
    field_0xbc = 0.0f;
    field_0xc0[0] = field_0x8->search('Ppllbk02')->getBounds()->i.x;
    field_0xd0[0] = field_0x8->search('Ppllbk02')->getBounds()->i.y;
    field_0xe0[0] = 0.0f;
    field_0xc0[1] = field_0x8->search('Ppllbk12')->getBounds()->i.x;
    field_0xd0[1] = field_0x8->search('Ppllbk12')->getBounds()->i.y;
    field_0xe0[1] = 0.0f;
    field_0xc0[2] = field_0x8->search('Ppllbk22')->getBounds()->i.x;
    field_0xd0[2] = field_0x8->search('Ppllbk22')->getBounds()->i.y;
    field_0xe0[2] = 0.0f;
    field_0xc0[3] = field_0x8->search('Ppllbk32')->getBounds()->i.x;
    field_0xd0[3] = field_0x8->search('Ppllbk32')->getBounds()->i.y;
    field_0xe0[3] = 0.0f;

    // field_0xc (the connect screen).
    for (int i = 0; i < 17; i++) {
        field_0x322[i] = field_0xc->search(lbl_80477FC0[i])->getAlpha();
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            field_0x333[i][j] = field_0xc->search(lbl_80478048[i][j])->getAlpha();
        }
    }
    for (int i = 0; i < 26; i++) {
        field_0x342[i] = field_0xc->search(lbl_804780C0[i])->getAlpha();
    }
    for (int i = 0; i < 17; i++) {
        field_0xc->search(lbl_80477FC0[i])->setAlpha(fadeAlpha(field_0x322[i], 0.0f));
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            field_0xc->search(lbl_80478048[i][j])->setAlpha(fadeAlpha(field_0x333[i][j], 0.0f));
        }
    }
    for (int i = 0; i < 26; i++) {
        field_0xc->search(lbl_804780C0[i])->setAlpha(fadeAlpha(field_0x342[i], 0.0f));
    }
    field_0x15c = field_0xc->search('Pmadbb07')->getBounds()->i.x;
    field_0x160 = field_0xc->search('Pmadbb07')->getBounds()->i.y;
    field_0x164 = 0.0f;
    field_0xbc = 0.0f;
    field_0x16c[0] = field_0xc->search('Pstab04')->getBounds()->i.x;
    field_0x178[0] = field_0xc->search('Pstab04')->getBounds()->i.y;
    field_0x184[0] = 0.0f;
    field_0x16c[1] = field_0xc->search('Phanb04')->getBounds()->i.x;
    field_0x178[1] = field_0xc->search('Phanb04')->getBounds()->i.y;
    field_0x184[1] = 0.0f;
    field_0x16c[2] = field_0xc->search('Prub04')->getBounds()->i.x;
    field_0x178[2] = field_0xc->search('Prub04')->getBounds()->i.y;
    field_0x184[2] = 0.0f;
    field_0x190 = field_0xc->search('Plibkk03')->getBounds()->i.x;
    field_0x194 = field_0xc->search('Plibkk03')->getBounds()->i.y;
    field_0x198 = 0.0f;
    field_0x269 = 156;
    field_0x1a4 = field_0xc->search('Pscuro02')->getBounds()->i.x;
    field_0x1a8 = field_0xc->search('Pscuro02')->getBounds()->i.y;
    field_0x150[0] = field_0xc->search('Pstab04')->getBounds()->i.y;
    field_0x150[1] = field_0xc->search('Phanb04')->getBounds()->i.y;
    field_0x150[2] = field_0xc->search('Prub04')->getBounds()->i.y;
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0xc->search('Trule01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0xc->search('Than01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0xc->search('Tsta01'))->setFont(font);
    field_0xc->search('P1pli01')->hide();
    field_0xc->search('P2pli01')->hide();
    field_0xc->search('P2pli02')->hide();
    field_0xc->search('P3pli01')->hide();
    field_0xc->search('P3pli02')->hide();
    field_0xc->search('P3pli03')->hide();
    field_0xc->search('P4pli01')->hide();
    field_0xc->search('P4pli02')->hide();
    field_0xc->search('P4pli03')->hide();
    field_0xc->search('P4pli04')->hide();

    // field_0x10 (the handicap screen).
    field_0x10->search('Ndatab01')->hide();
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x10->search('Tstr01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x10->search('Tstr02'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x10->search('Tstr03'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x10->search('Tstr04'))->setFont(font);
    getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x10->search('Tstr01'))->getStringPtr(), 0xF5, 0, NULL, NULL);
    getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x10->search('Tstr02'))->getStringPtr(), 0xF5, 0, NULL, NULL);
    getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x10->search('Tstr03'))->getStringPtr(), 0xF5, 0, NULL, NULL);
    getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x10->search('Tstr04'))->getStringPtr(), 0xF5, 0, NULL, NULL);
    field_0x26a = 156;
    field_0x1ac = field_0x10->search('Paccur02')->getBounds()->i.x;
    field_0x1b0 = field_0x10->search('Paccur02')->getBounds()->i.y;
    field_0x64 = new PaneCtrl_804AF86C;
    field_0x64->setPane_80402F04(field_0x10->search('Pthabb07'));
    field_0x68[0] = new PaneCtrl_804AF86C;
    field_0x68[0]->setPane_80402F04(field_0x10->search('Pstrb14'));
    field_0x68[1] = new PaneCtrl_804AF86C;
    field_0x68[1]->setPane_80402F04(field_0x10->search('Pstrb24'));
    field_0x68[2] = new PaneCtrl_804AF86C;
    field_0x68[2]->setPane_80402F04(field_0x10->search('Pstrb34'));
    field_0x68[3] = new PaneCtrl_804AF86C;
    field_0x68[3]->setPane_80402F04(field_0x10->search('Pstrb44'));
    field_0x78 = lbl_8053AED8->field_0x8;
    const ResTIMG* tex = (const ResTIMG*)fn_804030C4('TIMG', "kk_kettei_01.bti");
    lbl_8053AED8->fn_80440D78(tex);
    lbl_8053AED8->fn_80440CE4((const ResTIMG*)fn_804030C4('TIMG', "kk_modoru_01.bti"));
    field_0x5c = new cls_804B16A8;
    tex = (const ResTIMG*)fn_804030C4('TIMG', "kk_black.bti");
    field_0x60 = new J2DPicture(tex);
    field_0x60->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));
    for (int i = 0; i < 4; i++) {
        field_0x7c[i] = new cls_804B20F8(i, NULL, NULL, 8);
    }
}

cls_804B0B00::~cls_804B0B00() {
    delete mScreen;
    mScreen = NULL;
    delete field_0x8;
    field_0x8 = NULL;
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
    delete field_0x64;
    field_0x64 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x68[i];
        field_0x68[i] = NULL;
    }
    delete field_0x5c;
    field_0x5c = NULL;
    delete field_0x60;
    field_0x60 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x7c[i];
        field_0x7c[i] = NULL;
    }
}

// cls_804AF878::fn_803A86A4: `mode` 1 / 2 go straight to state 10 / 9;
// otherwise field_0x1bc counts the connected controllers and the menu
// starts in state 2.
void cls_804B0B00::fn_803EDBF0(u8 mode) {
    lbl_8053AEC0->fn_804310CC();
    switch (mode) {
    case 1:
        field_0x1d4 = 10;
        fn_803E3564();
        break;
    case 2:
        field_0x1d4 = 9;
        fn_803E3564();
        break;
    default:
        if (lbl_8053AD30->fn_8030A388(getPlayerInputSlot_8030AAF4(0))) {
            field_0x1bc = 0;
        } else if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(0))) {
            field_0x1bc = 0;
            for (s32 i = 1; i < 4; i++) {
                if (!lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(i))) {
                    break;
                }
                field_0x1bc++;
            }
        }
        field_0x1d4 = 2;
        fn_803E4344();
        break;
    }
    field_0x360 = 4;
}

// cls_804AF878::fn_803A8664: draws field_0x14, the state, then the fade
// (field_0x60). When the fade-out into state 9 ends, the decide / back
// button textures of lbl_8053AED8 are set.
void cls_804B0B00::fn_803EDA2C() {
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x14->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x1d4 != 15) {
        (this->*lbl_804B0A24[field_0x1d4])();
    } else {
        (this->*lbl_804B0A24[field_0x1d8])();
    }
    drawFade();
}

// cls_804AF878::fn_803A8600 each frame: counts the connected controllers
// into field_0x1d0 and bits 4-7 of gGameRoot_8053AAF8->mPlayerMask (one player:
// slot 0 through fn_8030A3B0), then runs the state's update.
void cls_804B0B00::fn_803ED83C() {
    field_0x1d0 = 0;
    if (field_0x1bc == 0) {
        if (lbl_8053AD30->fn_8030A3B0(getPlayerInputSlot_8030AAF4(0))) {
            gGameRoot_8053AAF8->mPlayerMask |= (u8)(1 << 4);
            field_0x1d0++;
        } else {
            gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << 4);
        }
        for (s32 i = 1; i < 4; i++) {
            gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << (i + 4));
        }
    } else {
        for (s32 i = 0; i < 4; i++) {
            if (i <= field_0x1bc) {
                if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(i))) {
                    gGameRoot_8053AAF8->mPlayerMask |= (u8)(1 << (i + 4));
                    field_0x1d0++;
                } else {
                    gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << (i + 4));
                }
            } else {
                gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << (i + 4));
            }
        }
    }
    if (field_0x26e == 0 || field_0x1d4 == 6) {
        if (field_0x1d4 != 15) {
            (this->*lbl_804B08BC[field_0x1d4])();
        } else {
            (this->*lbl_804B08BC[field_0x1d8])();
        }
        for (s32 i = 0; i < 4; i++) {
            field_0x7c[i]->fn_80456154();
        }
    }
}

// FIXME: 99.8. Registers only: the original loads field_0x1bc into r4 and field_0x1d0 into r3, ours the other way round.
void cls_804B0B00::fn_803ED604() {
    stepBlink(this);
    if (field_0x1d0 != field_0x1bc + 1 && field_0x1bc > 0) {
        field_0x26c = 255;
        (this->*lbl_804B0754[5])();
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1b8 < 3) {
            field_0x1dc = 15;
            fn_803E3320();
        }
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23c + 1)) {
        field_0x1dc = 2;
        (this->*lbl_804B0754[11])();
        gAudioMgr_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1b8 == 1) {
            field_0x1b8 = 2;
        } else {
            field_0x1b8 = 1;
        }
        field_0x254[0] = 1;
        field_0x1e0[0] = 30;
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    } else if (lbl_8053AD30->fn_80309A9C(8, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1b8 == 1) {
            field_0x1b8 = 2;
        } else {
            field_0x1b8 = 1;
        }
        field_0x254[0] = 1;
        field_0x1e0[0] = 30;
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    }
    field_0x1e0[0]++;
    if (field_0x1e0[0] >= 60) {
        field_0x1e0[0] = 0;
    }
}

void cls_804B0B00::fn_803ED5B4() {
    if (field_0x26e == 0) {
        field_0x1dc = 9;
        field_0x26e = 1;
    }
    field_0x1b8 = 1;
    gAudioMgr_8053AF80->fn_80458880(11, -1);
}

// FIXME: 96.95. 2 fewer instrs (594 vs 596): extsh. vs cmpwi r0, 0 on
// field_0x254[0] > 0, and one merged i * 5 induction variable (r30) where the
// original keeps two (r30 and r28).
void cls_804B0B00::fn_803ECC64() {
    field_0x254[0]++;
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 25) {
            field_0xf8 = -150.0f * ((f32)((25 - field_0x254[0]) * (25 - field_0x254[0])) / 625.0f);
            f32 alpha = (f32)field_0x254[0] / 25.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 17; i++) {
                mScreen->search(lbl_80477A28[i])->setAlpha(fadeAlpha(field_0x26f[i], alpha));
            }
        } else {
            field_0xf8 = 0.0f;
            for (int i = 0; i < 17; i++) {
                mScreen->search(lbl_80477A28[i])->setAlpha(fadeAlpha(field_0x26f[i], 1.0f));
            }
        }
    }
    if (field_0x254[0] > 25) {
        if (field_0x254[0] < 45) {
            field_0xfc = -200.0f * ((f32)((20 - (field_0x254[0] - 25)) * (20 - (field_0x254[0] - 25))) / 400.0f);
        } else {
            field_0xfc = 0.0f;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (field_0x254[0] > i * 5) {
            if (field_0x254[0] < i * 5 + 10) {
                field_0x120[i] = -350.0f * ((f32)((10 - (field_0x254[0] - i * 5)) * (10 - (field_0x254[0] - i * 5))) / 100.0f);
                f32 alpha = (f32)(field_0x254[0] - i * 5) / 10.0f;
                if (alpha < 0.0f) {
                    alpha = 0.0f;
                } else if (alpha > 1.0f) {
                    alpha = 1.0f;
                }
                for (int j = 0; j < 6; j++) {
                    mScreen->search(lbl_80477AB0[i][j])->setAlpha(fadeAlpha(field_0x280[i][j], alpha));
                }
            } else {
                field_0x120[i] = 0.0f;
                for (int j = 0; j < 6; j++) {
                    mScreen->search(lbl_80477AB0[i][j])->setAlpha(fadeAlpha(field_0x280[i][j], 1.0f));
                }
            }
        }
    }
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 15) {
            field_0x138 = field_0x14c = 400.0f * ((f32)((15 - field_0x254[0]) * (15 - field_0x254[0])) / 225.0f);
            f32 alpha = (f32)field_0x254[0] / 15.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 29; i++) {
                mScreen->search(lbl_80477B70[i])->setAlpha(fadeAlpha(field_0x298[i], alpha));
            }
            for (int i = 0; i < 22; i++) {
                mScreen->search(lbl_80477C58[i])->setAlpha(fadeAlpha(field_0x2b5[i], alpha));
            }
        } else {
            field_0x138 = field_0x14c = 0.0f;
            for (int i = 0; i < 29; i++) {
                mScreen->search(lbl_80477B70[i])->setAlpha(fadeAlpha(field_0x298[i], 1.0f));
            }
            for (int i = 0; i < 22; i++) {
                mScreen->search(lbl_80477C58[i])->setAlpha(fadeAlpha(field_0x2b5[i], 1.0f));
            }
        }
    }
    if (field_0x254[0] >= 75) {
        field_0x254[0] = 0;
        fn_803E4330();
    }
    mScreen->search('Pmadbb07')->move(field_0xf0, field_0xf4 + field_0xf8);
    field_0x78->moveFromCentre_80402E08(0.0f, field_0xfc);
    mScreen->search('Pnopb04')->move(field_0x100[0] + field_0x120[0], field_0x110[0]);
    mScreen->search('Pnopb08')->move(field_0x100[1] + field_0x120[1], field_0x110[1]);
    mScreen->search('Pnopb12')->move(field_0x100[2] + field_0x120[2], field_0x110[2]);
    mScreen->search('Pnopb16')->move(field_0x100[3] + field_0x120[3], field_0x110[3]);
    mScreen->search('Ndata02')->move(field_0x130 + field_0x138, field_0x134);
    mScreen->search('Ndata05')->move(field_0x13c + field_0x14c, field_0x140);
    mScreen->search('Ndata07')->move(field_0x144 + field_0x14c, field_0x148);
    f32 y = mScreen->search('Pscuro02')->getBounds()->i.y;
    mScreen->search('Pscuro02')->move(field_0x19c, y);
}

// FIXME: 99.61. One register: in setEntries' loop the original gives i a
// fresh li r4, 0 and keeps 2 * i in r3 (the zero stored to field_0x1b8);
// ours copies that zero into i (mr r4, r3). Same as the twin's
// ConnectScreen_804B0F10::fn_803F2580.
// Update state 3 (the player-count menu, field_0x1bc = players - 1): decide
// with one player takes slot 0 alone (kind 1 with a controller there, else
// 2); with more, the entries are set when every controller is connected,
// and a changed count resets the players' tables (fn_80403680). Back
// (0x200) leaves the screen (field_0x360 = 2); right / left (4 / 8) cycle
// the count.
void cls_804B0B00::fn_803EC804() {
    stepBlink(this);
    if (lbl_8053AD30->fn_80309C4C(0x1100, 1)) {
        if (field_0x1bc == 0) {
            field_0x1b8 = 1;
            gGameRoot_8053AAF8->field_0x7e = 0;
            setSingleEntry();
            field_0x1dc = 15;
            fn_803E4308();
        } else {
            field_0x1b8 = 0;
            gGameRoot_8053AAF8->field_0x7e = 1;
            if (field_0x1d0 == field_0x1bc + 1) {
                setEntries(this);
                field_0x1dc = 15;
            }
            fn_803E4308();
            if (field_0x1bc != lbl_80529DEC.field_0x23c) {
                for (s32 i = 0; i < 4; i++) {
                    lbl_80529DEC.field_0x130[i] = 0;
                }
                lbl_80529DEC.field_0x64 = -1;
            }
        }
        if (field_0x1bc != lbl_80529DEC.field_0x23c) {
            lbl_80529DEC.fn_80403680();
        }
        lbl_80529DEC.field_0x23c = field_0x1bc;
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1)) {
        field_0x360 = 2;
        gAudioMgr_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, 1)) {
        field_0x1bc++;
        if (field_0x1bc >= 4) {
            field_0x1bc = 0;
        }
        field_0x254[0] = 1;
        field_0x1e0[0] = 30;
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    } else if (lbl_8053AD30->fn_80309A9C(8, 1)) {
        field_0x1bc--;
        if (field_0x1bc < 0) {
            field_0x1bc = 3;
        }
        field_0x254[0] = 1;
        field_0x1e0[0] = 30;
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    }
    field_0x1e0[0]++;
    if (field_0x1e0[0] >= 60) {
        field_0x1e0[0] = 0;
    }
}

// FIXME: 96.93. 2 fewer instrs (853 vs 855): extsh. vs cmpwi r0, 0 on
// field_0x254[0] > 0, .rodata base load scheduled before others[], and one
// merged i * 5 induction variable where the original keeps two (r29 and r27).
void cls_804B0B00::fn_803EBAA8() {
    s32 others[3];
    s32* dst = others;
    for (s32 i = 0; i < 4; i++) {
        if (i != field_0x1bc) {
            *dst++ = i;
        }
    }
    field_0x254[0]++;
    if (field_0x254[0] > 20) {
        if (field_0x254[0] < 28) {
            field_0xf8 = -150.0f * ((f32)((field_0x254[0] - 20) * (field_0x254[0] - 20)) / 64.0f);
            f32 alpha = (f32)(8 - (field_0x254[0] - 20)) / 8.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 17; i++) {
                mScreen->search(lbl_80477A28[i])->setAlpha(fadeAlpha(field_0x26f[i], alpha));
            }
        } else {
            field_0xf8 = -150.0f;
            for (int i = 0; i < 17; i++) {
                mScreen->search(lbl_80477A28[i])->setAlpha(fadeAlpha(field_0x26f[i], 0.0f));
            }
        }
    }
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 8) {
            field_0xfc = -200.0f * ((f32)(field_0x254[0] * field_0x254[0]) / 64.0f);
        } else {
            field_0xfc = -200.0f;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (field_0x254[0] > i * 5) {
            if (field_0x254[0] < i * 5 + 8) {
                s32 idx = others[i];
                field_0x120[idx] = -350.0f * ((f32)((field_0x254[0] - i * 5) * (field_0x254[0] - i * 5)) / 64.0f);
                f32 alpha = (f32)(8 - (field_0x254[0] - i * 5)) / 8.0f;
                if (alpha < 0.0f) {
                    alpha = 0.0f;
                } else if (alpha > 1.0f) {
                    alpha = 1.0f;
                }
                for (int j = 0; j < 6; j++) {
                    mScreen->search(lbl_80477AB0[idx][j])->setAlpha(fadeAlpha(field_0x280[idx][j], alpha));
                }
            } else {
                s32 idx = others[i];
                field_0x120[idx] = -350.0f;
                for (int j = 0; j < 6; j++) {
                    mScreen->search(lbl_80477AB0[idx][j])->setAlpha(fadeAlpha(field_0x280[idx][j], 0.0f));
                }
            }
        }
    }
    if (field_0x254[0] > 15) {
        if (field_0x254[0] < 23) {
            field_0x120[field_0x1bc] = -350.0f * ((f32)((field_0x254[0] - 15) * (field_0x254[0] - 15)) / 64.0f);
            s32 row = field_0x1bc;
            f32 alpha = (f32)(8 - (field_0x254[0] - 15)) / 8.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int j = 0; j < 6; j++) {
                mScreen->search(lbl_80477AB0[row][j])->setAlpha(fadeAlpha(field_0x280[row][j], alpha));
            }
        } else {
            field_0x120[field_0x1bc] = -350.0f;
            s32 row = field_0x1bc;
            for (int j = 0; j < 6; j++) {
                mScreen->search(lbl_80477AB0[row][j])->setAlpha(fadeAlpha(field_0x280[row][j], 0.0f));
            }
        }
    }
    if (field_0x254[0] > 15) {
        if (field_0x254[0] < 23) {
            field_0x138 = field_0x14c = 400.0f * ((f32)((field_0x254[0] - 15) * (field_0x254[0] - 15)) / 64.0f);
            f32 alpha = (f32)(8 - (field_0x254[0] - 15)) / 8.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 29; i++) {
                mScreen->search(lbl_80477B70[i])->setAlpha(fadeAlpha(field_0x298[i], alpha));
            }
            for (int i = 0; i < 22; i++) {
                mScreen->search(lbl_80477C58[i])->setAlpha(fadeAlpha(field_0x2b5[i], alpha));
            }
        } else {
            field_0x138 = field_0x14c = 400.0f;
            for (int i = 0; i < 29; i++) {
                mScreen->search(lbl_80477B70[i])->setAlpha(fadeAlpha(field_0x298[i], 0.0f));
            }
            for (int i = 0; i < 22; i++) {
                mScreen->search(lbl_80477C58[i])->setAlpha(fadeAlpha(field_0x2b5[i], 0.0f));
            }
        }
    }
    if (field_0x254[0] >= 28) {
        if (field_0x1dc == 15) {
            if (field_0x1d0 == field_0x1bc + 1 || field_0x1bc == 0) {
                enterState15();
            } else {
                fn_803E3CB0();
            }
        } else if (field_0x1bc == 0) {
            (this->*lbl_804B0754[9])();
        } else {
            field_0x26c = 0;
            (this->*lbl_804B0754[5])();
        }
    }
    if (field_0x254[0] >= 53) {
        field_0x254[0] = 0;
    }
    field_0x1e0[0]++;
    if (field_0x1e0[0] >= 30) {
        field_0x1e0[0] = 0;
    }
    mScreen->search('Pmadbb07')->move(field_0xf0, field_0xf4 + field_0xf8);
    field_0x78->moveFromCentre_80402E08(0.0f, field_0xfc);
    mScreen->search('Pnopb04')->move(field_0x100[0] + field_0x120[0], field_0x110[0]);
    mScreen->search('Pnopb08')->move(field_0x100[1] + field_0x120[1], field_0x110[1]);
    mScreen->search('Pnopb12')->move(field_0x100[2] + field_0x120[2], field_0x110[2]);
    mScreen->search('Pnopb16')->move(field_0x100[3] + field_0x120[3], field_0x110[3]);
    mScreen->search('Ndata02')->move(field_0x130 + field_0x138, field_0x134);
    mScreen->search('Ndata05')->move(field_0x13c + field_0x14c, field_0x140);
    mScreen->search('Ndata07')->move(field_0x144 + field_0x14c, field_0x148);
    f32 y = mScreen->search('Pscuro02')->getBounds()->i.y;
    mScreen->search('Pscuro02')->move(field_0x19c + field_0x120[field_0x1bc], y);
}

// FIXME: 97.46. 2 fewer instrs (758 vs 760): extsh. vs cmpwi r0, 0 on
// field_0x254[0] > 0, and one merged i * 5 induction variable where the
// original keeps two (r30 and r28).
void cls_804B0B00::fn_803EAEC8() {
    field_0x254[0]++;
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 25) {
            field_0xa0 = -300.0f * ((f32)((25 - field_0x254[0]) * (25 - field_0x254[0])) / 625.0f);
            field_0xac = -100.0f * ((f32)((25 - field_0x254[0]) * (25 - field_0x254[0])) / 625.0f);
            f32 alpha = (f32)field_0x254[0] / 25.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 34; i++) {
                field_0x8->search(lbl_80477D90[i])->setAlpha(fadeAlpha(field_0x2dc[i], alpha));
            }
            alpha = (f32)field_0x254[0] / 25.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 11; i++) {
                field_0x8->search(lbl_80477D08[i])->setAlpha(fadeAlpha(field_0x2cb[i], alpha));
            }
        } else {
            field_0xa0 = field_0xac = 0.0f;
            for (int i = 0; i < 34; i++) {
                field_0x8->search(lbl_80477D90[i])->setAlpha(fadeAlpha(field_0x2dc[i], 1.0f));
            }
            for (int i = 0; i < 11; i++) {
                field_0x8->search(lbl_80477D08[i])->setAlpha(fadeAlpha(field_0x2cb[i], 1.0f));
            }
        }
    }
    if (field_0x254[0] > 25) {
        if (field_0x254[0] < 45) {
            field_0xb8 = -200.0f * ((f32)((20 - (field_0x254[0] - 25)) * (20 - (field_0x254[0] - 25))) / 400.0f);
            field_0xbc = -200.0f * ((f32)((20 - (field_0x254[0] - 25)) * (20 - (field_0x254[0] - 25))) / 400.0f);
            f32 alpha = (f32)(field_0x254[0] - 25) / 20.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 6; i++) {
                field_0x8->search(lbl_80477D60[i])->setAlpha(fadeAlpha(field_0x2d6[i], alpha));
            }
        } else {
            field_0xb8 = field_0xbc = 0.0f;
            for (int i = 0; i < 6; i++) {
                field_0x8->search(lbl_80477D60[i])->setAlpha(fadeAlpha(field_0x2d6[i], 1.0f));
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        if (field_0x254[0] > i * 5) {
            if (field_0x254[0] < i * 5 + 15) {
                field_0xe0[i] = 200.0f * ((f32)((15 - (field_0x254[0] - i * 5)) * (15 - (field_0x254[0] - i * 5))) / 225.0f);
                f32 alpha = (f32)(field_0x254[0] - i * 5) / 15.0f;
                if (alpha < 0.0f) {
                    alpha = 0.0f;
                } else if (alpha > 1.0f) {
                    alpha = 1.0f;
                }
                for (int j = 0; j < 9; j++) {
                    field_0x8->search(lbl_80477EA0[i][j])->setAlpha(fadeAlpha(field_0x2fe[i][j], alpha));
                }
            } else {
                field_0xe0[i] = 0.0f;
                for (int j = 0; j < 9; j++) {
                    field_0x8->search(lbl_80477EA0[i][j])->setAlpha(fadeAlpha(field_0x2fe[i][j], 1.0f));
                }
            }
        }
    }
    field_0x25e++;
    if (field_0x25e >= 100) {
        field_0x25e = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (i + 4))) && field_0xe0[i] == 0.0f) {
            field_0x220[i]++;
            if (field_0x220[i] >= 130) {
                field_0x220[i] = 100;
            }
            if (field_0x220[i] == 11) {
                field_0x7c[i]->field_0x44 = 0;
                field_0x7c[i]->field_0x45 = 0;
                field_0x7c[i]->startSound(0x10001);
            } else if (field_0x220[i] == 36) {
                f32 size = field_0x8->search(lbl_80478190[i])->getWidth();
                field_0x8c = toEffectX(size / 2 + field_0x8->search(lbl_80478190[i])->getGlobalBounds().i.x);
                size = field_0x8->search(lbl_80478190[i])->getHeight();
                field_0x90 = toEffectY(size / 2 + field_0x8->search(lbl_80478190[i])->getGlobalBounds().i.y);
                field_0x94 = 0.0f;
                field_0x7c[i]->field_0x44 = 0;
                field_0x7c[i]->field_0x45 = 0;
                field_0x7c[i]->startSound(0x2000E);
                JGeometry::TVec3f pos(field_0x8c + 7.0f, field_0x90 - 27.0f, field_0x94);
                spawnEmitter_8013CC50(0, &pos, 0xCB, 1, 0, NULL, NULL, true, 1.0f);
                JGeometry::TVec3f pos2(field_0x8c + 7.0f, field_0x90 - 27.0f, field_0x94);
                spawnEmitter_8013CC50(0, &pos2, 0xCC, 1, 0, NULL, NULL, true, 1.0f);
            }
        } else {
            field_0x220[i] = 0;
        }
    }
    if (field_0x254[0] >= 75) {
        field_0x254[0] = 0;
        fn_803E3C80();
    }
    field_0x8->search('Ndata03')->move(field_0x98, field_0x9c + field_0xa0);
    field_0x8->search('Ndata05')->move(field_0xa4, field_0xa8 + field_0xac);
    field_0x8->search('Ndata06')->move(field_0xb0, field_0xb4 + field_0xb8);
    field_0x78->moveFromCentre_80402E08(0.0f, field_0xbc);
    field_0x8->search('Ppllbk02')->move(field_0xc0[0], field_0xd0[0] + field_0xe0[0]);
    field_0x8->search('Ppllbk12')->move(field_0xc0[1], field_0xd0[1] + field_0xe0[1]);
    field_0x8->search('Ppllbk22')->move(field_0xc0[2], field_0xd0[2] + field_0xe0[2]);
    field_0x8->search('Ppllbk32')->move(field_0xc0[3], field_0xd0[3] + field_0xe0[3]);
}

// FIXME: 97.63. 2 fewer instrs (690 vs 692): extsh. vs cmpwi r0, 0 on
// field_0x254[0] > 0, and one merged i * 5 induction variable where the
// original keeps two (r29 and r27).
void cls_804B0B00::fn_803EA3F8() {
    field_0x254[0]++;
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 8) {
            field_0xa0 = -300.0f * ((f32)(field_0x254[0] * field_0x254[0]) / 64.0f);
            field_0xac = -100.0f * ((f32)(field_0x254[0] * field_0x254[0]) / 64.0f);
            f32 alpha = (f32)(8 - field_0x254[0]) / 8.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 34; i++) {
                field_0x8->search(lbl_80477D90[i])->setAlpha(fadeAlpha(field_0x2dc[i], alpha));
            }
            alpha = (f32)(8 - field_0x254[0]) / 8.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 11; i++) {
                field_0x8->search(lbl_80477D08[i])->setAlpha(fadeAlpha(field_0x2cb[i], alpha));
            }
        } else {
            field_0xa0 = -300.0f;
            field_0xac = -100.0f;
            for (int i = 0; i < 34; i++) {
                field_0x8->search(lbl_80477D90[i])->setAlpha(fadeAlpha(field_0x2dc[i], 0.0f));
            }
            for (int i = 0; i < 11; i++) {
                field_0x8->search(lbl_80477D08[i])->setAlpha(fadeAlpha(field_0x2cb[i], 0.0f));
            }
        }
    }
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 8) {
            field_0xb8 = -200.0f * ((f32)(field_0x254[0] * field_0x254[0]) / 64.0f);
            field_0xbc = -200.0f * ((f32)(field_0x254[0] * field_0x254[0]) / 64.0f);
            f32 alpha = (f32)(8 - field_0x254[0]) / 8.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 6; i++) {
                field_0x8->search(lbl_80477D60[i])->setAlpha(fadeAlpha(field_0x2d6[i], alpha));
            }
        } else {
            field_0xb8 = -200.0f;
            field_0xbc = -200.0f;
            for (int i = 0; i < 6; i++) {
                field_0x8->search(lbl_80477D60[i])->setAlpha(fadeAlpha(field_0x2d6[i], 0.0f));
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        if (field_0x254[0] > i * 5) {
            if (field_0x254[0] < i * 5 + 8) {
                s32 idx = 3 - i;
                field_0xe0[idx] = 200.0f * ((f32)((field_0x254[0] - i * 5) * (field_0x254[0] - i * 5)) / 64.0f);
                f32 alpha = (f32)(8 - (field_0x254[0] - i * 5)) / 8.0f;
                if (alpha < 0.0f) {
                    alpha = 0.0f;
                } else if (alpha > 1.0f) {
                    alpha = 1.0f;
                }
                for (int j = 0; j < 9; j++) {
                    field_0x8->search(lbl_80477EA0[idx][j])->setAlpha(fadeAlpha(field_0x2fe[idx][j], alpha));
                }
            } else {
                s32 idx = 3 - i;
                field_0xe0[idx] = 200.0f;
                for (int j = 0; j < 9; j++) {
                    field_0x8->search(lbl_80477EA0[idx][j])->setAlpha(fadeAlpha(field_0x2fe[idx][j], 0.0f));
                }
            }
        }
    }
    if (field_0x254[0] >= 23) {
        setEntries(this);
        if (field_0x1d0 == field_0x1bc + 1) {
            enterState15();
        } else {
            fn_803E3CB0();
        }
        field_0x254[0] = 0;
    }
    field_0x8->search('Ndata03')->move(field_0x98, field_0x9c + field_0xa0);
    field_0x8->search('Ndata05')->move(field_0xa4, field_0xa8 + field_0xac);
    field_0x8->search('Ndata06')->move(field_0xb0, field_0xb4 + field_0xb8);
    field_0x78->moveFromCentre_80402E08(0.0f, field_0xbc);
    field_0x8->search('Ppllbk02')->move(field_0xc0[0], field_0xd0[0] + field_0xe0[0]);
    field_0x8->search('Ppllbk12')->move(field_0xc0[1], field_0xd0[1] + field_0xe0[1]);
    field_0x8->search('Ppllbk22')->move(field_0xc0[2], field_0xd0[2] + field_0xe0[2]);
    field_0x8->search('Ppllbk32')->move(field_0xc0[3], field_0xd0[3] + field_0xe0[3]);
}

// Update state 7 (the GBA connection prompt): field_0x26c 255 or decide
// (0x1100) without every controller connected enters state 8 (fn_803E3C00);
// decide with all connected reads the players' results and enters state 6
// (fn_803E3C94); back (0x200) enters state 2.
void cls_804B0B00::fn_803E9EE8() {
    if (field_0x26c == 255) {
        fn_803E3C00();
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1d0 == field_0x1bc + 1) {
            field_0x1b8 = 0;
            for (s32 i = 0; i < 4; i++) {
                field_0x1f0[i] = fn_80403414(i);
                field_0x200[i] = fn_804033D4(i);
                field_0x210[i] = fn_8040336C(i);
                field_0x1c0[i] = 0;
            }
            fn_803E3C94();
        } else {
            fn_803E3C00();
        }
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23c + 1)) {
        (this->*lbl_804B0754[2])();
        gAudioMgr_8053AF80->fn_80458880(12, -1);
    }
    field_0x1e0[0]++;
    if (field_0x1e0[0] >= 60) {
        field_0x1e0[0] = 0;
    }
    field_0x25e++;
    if (field_0x25e >= 100) {
        field_0x25e = 0;
    }
    field_0x25c++;
    if (field_0x25c >= 100) {
        field_0x25c = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (i + 4))) {
            field_0x220[i]++;
            if (field_0x220[i] >= 130) {
                field_0x220[i] = 100;
            }
            if (field_0x220[i] == 11) {
                field_0x7c[i]->field_0x44 = 0;
                field_0x7c[i]->field_0x45 = 0;
                field_0x7c[i]->startSound(0x10001);
            } else if (field_0x220[i] == 36) {
                f32 size = field_0x8->search(lbl_80478190[i])->getWidth();
                field_0x8c = toEffectX(size / 2 + field_0x8->search(lbl_80478190[i])->getGlobalBounds().i.x);
                size = field_0x8->search(lbl_80478190[i])->getHeight();
                field_0x90 = toEffectY(size / 2 + field_0x8->search(lbl_80478190[i])->getGlobalBounds().i.y);
                field_0x94 = 0.0f;
                field_0x7c[i]->field_0x44 = 0;
                field_0x7c[i]->field_0x45 = 0;
                field_0x7c[i]->startSound(0x2000E);
                JGeometry::TVec3f pos(field_0x8c + 7.0f, field_0x90 - 27.0f, field_0x94);
                spawnEmitter_8013CC50(0, &pos, 0xCB, 1, 0, NULL, NULL, true, 1.0f);
                JGeometry::TVec3f pos2(field_0x8c + 7.0f, field_0x90 - 27.0f, field_0x94);
                spawnEmitter_8013CC50(0, &pos2, 0xCC, 1, 0, NULL, NULL, true, 1.0f);
            }
        } else {
            field_0x220[i] = 0;
        }
    }
}

// Update state 8 (waiting for the GBA link through lbl_8053AEC0;
// field_0x26c: 0 starting, 1 running, 2 ending): decide (0x1100 / 0x1300) or
// back (0x200) ends it; then each connected player's pane animation
// (field_0x220[i], 100-129 looping) plays sound 0x10001 at frame 11 and at
// frame 36 sound 0x2000E and effects 0xCB / 0xCC at the 'Pplli' pane.
void cls_804B0B00::fn_803E9A20() {
    switch (field_0x26c) {
    case 0:
        if (lbl_8053AEC0->fn_804317AC()) {
            field_0x26c = 1;
        }
        break;
    case 2:
        if (lbl_8053AEC0->fn_80431740()) {
            field_0x26c = 0;
            field_0x1d8 = field_0x1d4;
            field_0x1d4 = 7;
        }
        break;
    case 1:
    default:
        if (field_0x1d0 == field_0x1bc + 1) {
            field_0x26c = 2;
            gAudioMgr_8053AF80->fn_80458880(11, -1);
        } else if ((gGameRoot_8053AAF8->mPlayerMask & 0x10) && lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(0))) {
            if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23c + 1)) {
                field_0x26c = 2;
                gAudioMgr_8053AF80->fn_80458880(11, -1);
            } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23c + 1)) {
                field_0x26c = 2;
                gAudioMgr_8053AF80->fn_80458880(12, -1);
            }
        } else if (lbl_8053AD30->fn_80309C4C(0x1300, lbl_80529DEC.field_0x23c + 1)) {
            field_0x26c = 2;
            gAudioMgr_8053AF80->fn_80458880(11, -1);
        }
        break;
    }
    field_0x25e++;
    if (field_0x25e >= 100) {
        field_0x25e = 0;
    }
    field_0x25c++;
    if (field_0x25c >= 100) {
        field_0x25c = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (i + 4))) {
            field_0x220[i]++;
            if (field_0x220[i] >= 130) {
                field_0x220[i] = 100;
            }
            if (field_0x220[i] == 11) {
                field_0x7c[i]->field_0x44 = 0;
                field_0x7c[i]->field_0x45 = 0;
                field_0x7c[i]->startSound(0x10001);
            } else if (field_0x220[i] == 36) {
                f32 size = field_0x8->search(lbl_80478190[i])->getWidth();
                field_0x8c = toEffectX(size / 2 + field_0x8->search(lbl_80478190[i])->getGlobalBounds().i.x);
                size = field_0x8->search(lbl_80478190[i])->getHeight();
                field_0x90 = toEffectY(size / 2 + field_0x8->search(lbl_80478190[i])->getGlobalBounds().i.y);
                field_0x94 = 0.0f;
                field_0x7c[i]->field_0x44 = 0;
                field_0x7c[i]->field_0x45 = 0;
                field_0x7c[i]->startSound(0x2000E);
                JGeometry::TVec3f pos(field_0x8c + 7.0f, field_0x90 - 27.0f, field_0x94);
                spawnEmitter_8013CC50(0, &pos, 0xCB, 1, 0, NULL, NULL, true, 1.0f);
                JGeometry::TVec3f pos2(field_0x8c + 7.0f, field_0x90 - 27.0f, field_0x94);
                spawnEmitter_8013CC50(0, &pos2, 0xCC, 1, 0, NULL, NULL, true, 1.0f);
            }
        } else {
            field_0x220[i] = 0;
        }
    }
}

// FIXME: 96.67. 2 fewer instrs (491 vs 493): extsh. vs cmpwi r0, 0 on
// field_0x254[0] > 0, and one merged i * 10 induction variable where the
// original keeps two (r30 and r28).
void cls_804B0B00::fn_803E926C() {
    field_0x254[0]++;
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 25) {
            s32 d = 25 - field_0x254[0];
            field_0x164 = -200.0f * ((f32)(d * d) / 625.0f);
            f32 alpha = (f32)field_0x254[0] / 25.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 17; i++) {
                field_0xc->search(lbl_80477FC0[i])->setAlpha(fadeAlpha(field_0x322[i], alpha));
            }
        } else {
            field_0x164 = 0.0f;
            for (int i = 0; i < 17; i++) {
                field_0xc->search(lbl_80477FC0[i])->setAlpha(fadeAlpha(field_0x322[i], 1.0f));
            }
        }
    }
    if (field_0x254[0] > 25) {
        if (field_0x254[0] < 45) {
            s32 d = 20 - (field_0x254[0] - 25);
            field_0x168 = -200.0f * ((f32)(d * d) / 400.0f);
        } else {
            field_0x168 = 0.0f;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (field_0x254[0] > i * 10) {
            if (field_0x254[0] < i * 10 + 15) {
                s32 d = 15 - (field_0x254[0] - i * 10);
                field_0x184[i] = 300.0f * ((f32)(d * d) / 225.0f);
                f32 alpha = (f32)(field_0x254[0] - i * 10) / 15.0f;
                if (alpha < 0.0f) {
                    alpha = 0.0f;
                } else if (alpha > 1.0f) {
                    alpha = 1.0f;
                }
                for (int j = 0; j < 5; j++) {
                    field_0xc->search(lbl_80478048[i][j])->setAlpha(fadeAlpha(field_0x333[i][j], alpha));
                }
            } else {
                field_0x184[i] = 0.0f;
                for (int j = 0; j < 5; j++) {
                    field_0xc->search(lbl_80478048[i][j])->setAlpha(fadeAlpha(field_0x333[i][j], 1.0f));
                }
            }
        }
    }
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 15) {
            s32 d = 15 - field_0x254[0];
            field_0x198 = -450.0f * ((f32)(d * d) / 225.0f);
            f32 alpha = (f32)field_0x254[0] / 15.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 26; i++) {
                field_0xc->search(lbl_804780C0[i])->setAlpha(fadeAlpha(field_0x342[i], alpha));
            }
        } else {
            field_0x198 = 0.0f;
            for (int i = 0; i < 26; i++) {
                field_0xc->search(lbl_804780C0[i])->setAlpha(fadeAlpha(field_0x342[i], 1.0f));
            }
        }
    }
    if (field_0x254[0] >= 75) {
        field_0x254[0] = 0;
        if (field_0x1bc == 0) {
            fn_803E4610();
        } else {
            fn_803E3548();
        }
    }
    field_0xc->search('Pmadbb07')->move(field_0x15c, field_0x160 + field_0x164);
    field_0x78->moveFromCentre_80402E08(0.0f, field_0x168);
    field_0xc->search('Pstab04')->move(field_0x16c[0] + field_0x184[0], field_0x178[0]);
    field_0xc->search('Phanb04')->move(field_0x16c[1] + field_0x184[1], field_0x178[1]);
    field_0xc->search('Prub04')->move(field_0x16c[2] + field_0x184[2], field_0x178[2]);
    field_0xc->search('Plibkk03')->move(field_0x190 + field_0x198, field_0x194);
    f32 y = field_0xc->search('Pscuro02')->getBounds()->i.y;
    field_0xc->search('Pscuro02')->move(field_0x1a4, y);
}

// FIXME: 99.8. Registers only: the original loads field_0x1bc into r4 and field_0x1d0 into r3, ours the other way round.
void cls_804B0B00::fn_803E9034() {
    stepBlink(this);
    if (field_0x1d0 != field_0x1bc + 1 && field_0x1bc > 0) {
        field_0x26c = 255;
        (this->*lbl_804B0754[5])();
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1b8 < 3) {
            field_0x1dc = 15;
            fn_803E3320();
        }
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23c + 1)) {
        field_0x1dc = 2;
        (this->*lbl_804B0754[11])();
        gAudioMgr_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, lbl_80529DEC.field_0x23c + 1)) {
        field_0x1b8++;
        if (field_0x1b8 >= 3) {
            field_0x1b8 = 0;
        }
        field_0x254[0] = 1;
        field_0x1e0[0] = 30;
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    } else if (lbl_8053AD30->fn_80309A9C(8, lbl_80529DEC.field_0x23c + 1)) {
        field_0x1b8--;
        if (field_0x1b8 < 0) {
            field_0x1b8 = 2;
        }
        field_0x254[0] = 1;
        field_0x1e0[0] = 30;
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    }
    field_0x1e0[0]++;
    if (field_0x1e0[0] >= 60) {
        field_0x1e0[0] = 0;
    }
}

// FIXME: 97.07. 4 fewer instrs (834 vs 838): extsh. vs cmpwi r0, 0 on
// field_0x254[0] > 0, and merged i * 10 induction variables in the two
// field_0x184 loops where the original keeps two per loop.
void cls_804B0B00::fn_803E831C() {
    if (field_0x1bc != 3) {
        if (field_0x220[0] < 100) {
            field_0x220[0] = 0;
        } else if (field_0x220[0] < 112) {
            field_0x220[0]++;
        } else if (field_0x220[0] < 212) {
            field_0x220[0] = 112;
        } else {
            field_0x220[0]++;
            if (field_0x220[0] >= 224) {
                field_0x220[0] = 0;
            }
        }
    }
    field_0x254[0]++;
    if (field_0x254[0] > 20) {
        if (field_0x254[0] < 28) {
            field_0x164 = -200.0f * ((f32)((field_0x254[0] - 20) * (field_0x254[0] - 20)) / 64.0f);
            f32 alpha = (f32)(8 - (field_0x254[0] - 20)) / 8.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 17; i++) {
                field_0xc->search(lbl_80477FC0[i])->setAlpha(fadeAlpha(field_0x322[i], alpha));
            }
        } else {
            field_0x164 = -200.0f;
            for (int i = 0; i < 17; i++) {
                field_0xc->search(lbl_80477FC0[i])->setAlpha(fadeAlpha(field_0x322[i], 0.0f));
            }
        }
    }
    if (field_0x254[0] > 0) {
        if (field_0x254[0] < 10) {
            field_0x168 = -200.0f * ((f32)(field_0x254[0] * field_0x254[0]) / 100.0f);
        } else {
            field_0x168 = -200.0f;
        }
    }
    s16 others[2];
    s16* dst = others;
    for (s32 i = 0; i < 3; i++) {
        if (i != field_0x1b8) {
            *dst++ = i;
        }
    }
    if (field_0x1dc == 2) {
        for (int i = 0; i < 3; i++) {
            if (field_0x254[0] > i * 10) {
                if (field_0x254[0] < i * 10 + 10) {
                    field_0x184[i] = 300.0f * ((f32)((field_0x254[0] - i * 10) * (field_0x254[0] - i * 10)) / 100.0f);
                    f32 alpha = (f32)(10 - (field_0x254[0] - i * 10)) / 10.0f;
                    if (alpha < 0.0f) {
                        alpha = 0.0f;
                    } else if (alpha > 1.0f) {
                        alpha = 1.0f;
                    }
                    for (int j = 0; j < 5; j++) {
                        field_0xc->search(lbl_80478048[i][j])->setAlpha(fadeAlpha(field_0x333[i][j], alpha));
                    }
                } else {
                    field_0x184[i] = 300.0f;
                    for (int j = 0; j < 5; j++) {
                        field_0xc->search(lbl_80478048[i][j])->setAlpha(fadeAlpha(field_0x333[i][j], 0.0f));
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < 2; i++) {
            if (field_0x254[0] > i * 10) {
                if (field_0x254[0] < i * 10 + 10) {
                    s16 idx = others[i];
                    field_0x184[idx] = 300.0f * ((f32)((field_0x254[0] - i * 10) * (field_0x254[0] - i * 10)) / 100.0f);
                    f32 alpha = (f32)(10 - (field_0x254[0] - i * 10)) / 10.0f;
                    if (alpha < 0.0f) {
                        alpha = 0.0f;
                    } else if (alpha > 1.0f) {
                        alpha = 1.0f;
                    }
                    for (int j = 0; j < 5; j++) {
                        field_0xc->search(lbl_80478048[idx][j])->setAlpha(fadeAlpha(field_0x333[idx][j], alpha));
                    }
                } else {
                    s16 idx = others[i];
                    field_0x184[idx] = 300.0f;
                    for (int j = 0; j < 5; j++) {
                        field_0xc->search(lbl_80478048[idx][j])->setAlpha(fadeAlpha(field_0x333[idx][j], 0.0f));
                    }
                }
            }
        }
        if (field_0x254[0] > 15) {
            if (field_0x254[0] < 23) {
                field_0x184[field_0x1b8] = 300.0f * ((f32)((field_0x254[0] - 15) * (field_0x254[0] - 15)) / 64.0f);
                s32 row = field_0x1b8;
                f32 alpha = (f32)(8 - (field_0x254[0] - 15)) / 8.0f;
                if (alpha < 0.0f) {
                    alpha = 0.0f;
                } else if (alpha > 1.0f) {
                    alpha = 1.0f;
                }
                for (int j = 0; j < 5; j++) {
                    field_0xc->search(lbl_80478048[row][j])->setAlpha(fadeAlpha(field_0x333[row][j], alpha));
                }
            } else {
                field_0x184[field_0x1b8] = 300.0f;
                s32 row = field_0x1b8;
                for (int j = 0; j < 5; j++) {
                    field_0xc->search(lbl_80478048[row][j])->setAlpha(fadeAlpha(field_0x333[row][j], 0.0f));
                }
            }
        }
    }
    if (field_0x254[0] > 15) {
        if (field_0x254[0] < 23) {
            field_0x198 = -450.0f * ((f32)((field_0x254[0] - 15) * (field_0x254[0] - 15)) / 64.0f);
            f32 alpha = (f32)(8 - (field_0x254[0] - 15)) / 8.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            } else if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            for (int i = 0; i < 26; i++) {
                field_0xc->search(lbl_804780C0[i])->setAlpha(fadeAlpha(field_0x342[i], alpha));
            }
        } else {
            field_0x198 = -450.0f;
            for (int i = 0; i < 26; i++) {
                field_0xc->search(lbl_804780C0[i])->setAlpha(fadeAlpha(field_0x342[i], 0.0f));
            }
        }
    }
    if (field_0x254[0] >= 48) {
        if (field_0x1dc == 2) {
            field_0x360 = 6;
        } else if (field_0x1bc == 0) {
            // In 1P mode row 0 ('Pstab04') is hidden and row 1 is Start, so the original's
            // 0 / 1 / else chain became 1 / 1 / else (`cmpwi r0, 1; bne; ... bne` in the asm).
            if (field_0x1b8 == 1) {
                field_0x360 = 6;
            } else if (field_0x1b8 == 1) {
                if (field_0x26e == 0) {
                    field_0x1dc = 1;
                    field_0x26e = 1;
                }
            } else {
                if (field_0x26e == 0) {
                    field_0x1dc = 14;
                    field_0x26e = 1;
                }
            }
        } else {
            if (field_0x1b8 == 0) {
                field_0x360 = 6;
            } else if (field_0x1b8 == 1) {
                if (field_0x26e == 0) {
                    field_0x1dc = 12;
                    field_0x26e = 1;
                }
            } else {
                if (field_0x26e == 0) {
                    field_0x1dc = 14;
                    field_0x26e = 1;
                }
            }
        }
        field_0x254[0] = 0;
    }
    field_0x1e0[0]++;
    if (field_0x1e0[0] >= 30) {
        field_0x1e0[0] = 0;
    }
    field_0xc->search('Pmadbb07')->move(field_0x15c, field_0x160 + field_0x164);
    field_0x78->moveFromCentre_80402E08(0.0f, field_0x168);
    field_0xc->search('Pstab04')->move(field_0x16c[0] + field_0x184[0], field_0x178[0]);
    field_0xc->search('Phanb04')->move(field_0x16c[1] + field_0x184[1], field_0x178[1]);
    field_0xc->search('Prub04')->move(field_0x16c[2] + field_0x184[2], field_0x178[2]);
    field_0xc->search('Plibkk03')->move(field_0x190 + field_0x198, field_0x194);
    f32 y = field_0xc->search('Pscuro02')->getBounds()->i.y;
    field_0xc->search('Pscuro02')->move(field_0x1a4 + field_0x184[field_0x1b8], y);
}

// Update state 12: slides field_0x64, field_0x78 and the field_0x1bc + 1
// player panes field_0x68[] in, then enters state 13.
void cls_804B0B00::fn_803E8210() {
    u8 done0 = false;
    u8 done1 = false;
    u8 done2 = false;
    field_0x260++;
    if (field_0x260 > 0) {
        done0 = field_0x64->fn_80402940(22, 0, -200.0f, 0.0f);
    }
    if (field_0x260 > 16) {
        done1 = field_0x78->fn_80402940(24, 0, -200.0f, 0.0f);
    }
    for (s32 i = 0; i < 4; i++) {
        if (i <= field_0x1bc && field_0x260 > i * 5 + 4) {
            done2 = field_0x68[i]->fn_80402940(18, 0, 150.0f, 0.0f);
        }
    }
    if (done0 && done1 && done2) {
        field_0x1d8 = field_0x1d4;
        field_0x1d4 = 13;
    }
}

// FIXME: 95.41. 4 fewer instrs (1492 vs 1496): register allocation and loop
// induction pointers across the per-player button and marker loops.
void cls_804B0B00::fn_803E6AB0() {
    for (s32 i = 0; i < 4; i++) {
        if (field_0x254[i] != 0) {
            field_0x254[i]++;
            if (field_0x254[i] >= 10) {
                field_0x254[i] = 0;
            }
        }
        if (i <= field_0x1bc) {
            if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(i), 0x1000)) {
                if (field_0x1c0[0] == 4 && field_0x1c0[1] == 4 && field_0x1c0[2] == 4 && field_0x1c0[3] == 4) {
                    field_0x1b8 = 0;
                    setBounce(this, 0);
                    setBounce(this, 1);
                    setBounce(this, 2);
                    setBounce(this, 3);
                    if (field_0x26e == 0) {
                        field_0x1dc = 9;
                        field_0x26e = 1;
                    }
                } else {
                    for (s32 j = 0; j < 4; j++) {
                        if (field_0x1c0[j] != 4) {
                            fn_804034D4(j, field_0x1f0[j]);
                            fn_80403494(j, field_0x200[j]);
                            fn_80403454(j, field_0x210[j]);
                            field_0x1c0[j] = 4;
                            field_0x254[j] = 1;
                            field_0x1e0[j] = 30;
                            updateHandicapCursor(this, j);
                        }
                    }
                }
                gAudioMgr_8053AF80->fn_80458880(11, -1);
                return;
            } else if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(i), 0x100)) {
                if (field_0x1c0[0] == 4 && field_0x1c0[1] == 4 && field_0x1c0[2] == 4 && field_0x1c0[3] == 4) {
                    field_0x1b8 = 0;
                    setBounce(this, 0);
                    setBounce(this, 1);
                    setBounce(this, 2);
                    setBounce(this, 3);
                    if (field_0x26e == 0) {
                        field_0x1dc = 9;
                        field_0x26e = 1;
                    }
                    gAudioMgr_8053AF80->fn_80458880(11, -1);
                } else if (field_0x1c0[i] == 3) {
                    fn_804034D4(i, field_0x1f0[i]);
                    fn_80403494(i, field_0x200[i]);
                    fn_80403454(i, field_0x210[i]);
                    field_0x1c0[i] = 4;
                    field_0x254[i] = 1;
                    field_0x1e0[i] = 30;
                    updateHandicapCursor(this, i);
                    gAudioMgr_8053AF80->fn_80458880(11, -1);
                    if (field_0x1c0[0] == 4 && field_0x1c0[1] == 4 && field_0x1c0[2] == 4 && field_0x1c0[3] == 4) {
                        field_0x1b8 = 0;
                        setBounce(this, 0);
                        setBounce(this, 1);
                        setBounce(this, 2);
                        setBounce(this, 3);
                        if (field_0x26e == 0) {
                            field_0x1dc = 9;
                            field_0x26e = 1;
                        }
                        return;
                    }
                } else {
                    switch (field_0x1c0[i]) {
                    case 0: {
                        field_0x1f0[i]++;
                        if (field_0x1f0[i] > 3) {
                            field_0x1f0[i] = 0;
                        }
                        s32 rupees = fn_80403070(field_0x1f0[i]);
                        const ResTIMG* tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0AD8[rupees / 100]);
                        static_cast<J2DPicture*>(field_0x10->search(lbl_80478410[i]))->changeTexture(tex, 0);
                        static_cast<J2DPicture*>(field_0x10->search(lbl_80478430[i]))->changeTexture(tex, 0);
                        tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0AD8[rupees % 100 / 10]);
                        static_cast<J2DPicture*>(field_0x10->search(lbl_804783D0[i]))->changeTexture(tex, 0);
                        static_cast<J2DPicture*>(field_0x10->search(lbl_804783F0[i]))->changeTexture(tex, 0);
                        tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0AD8[rupees % 10]);
                        static_cast<J2DPicture*>(field_0x10->search(lbl_80478390[i]))->changeTexture(tex, 0);
                        static_cast<J2DPicture*>(field_0x10->search(lbl_804783B0[i]))->changeTexture(tex, 0);
                        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
                        break;
                    }
                    case 1: {
                        field_0x200[i]++;
                        if (field_0x200[i] > 2) {
                            field_0x200[i] = 0;
                        }
                        s32 count = fn_8040305C(field_0x200[i]) - 1;
                        for (s32 j = 0; j < 6; j++) {
                            if (j < count) {
                                field_0x10->search(lbl_80478518[i][j])->show();
                                field_0x10->search(lbl_804785D8[i][j])->show();
                                field_0x10->search(lbl_80478698[i][j])->hide();
                                field_0x10->search(lbl_80478758[i][j])->hide();
                            } else if (j == count) {
                                field_0x10->search(lbl_80478518[i][j])->hide();
                                field_0x10->search(lbl_804785D8[i][j])->hide();
                                field_0x10->search(lbl_80478698[i][j])->show();
                                field_0x10->search(lbl_80478758[i][j])->show();
                            } else {
                                field_0x10->search(lbl_80478518[i][j])->hide();
                                field_0x10->search(lbl_804785D8[i][j])->hide();
                                field_0x10->search(lbl_80478698[i][j])->hide();
                                field_0x10->search(lbl_80478758[i][j])->hide();
                            }
                        }
                        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
                        break;
                    }
                    case 2:
                        field_0x210[i]++;
                        if (field_0x210[i] > 2) {
                            field_0x210[i] = 0;
                        }
                        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
                        break;
                    }
                }
            } else if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(i), 0x200)) {
                gAudioMgr_8053AF80->fn_80458880(12, -1);
                if (field_0x1c0[i] == 4) {
                    field_0x1c0[i] = 3;
                    field_0x1e0[i] = 30;
                    updateHandicapCursor(this, i);
                } else {
                    setBounceIfReady(this, 0);
                    setBounceIfReady(this, 1);
                    setBounceIfReady(this, 2);
                    setBounceIfReady(this, 3);
                    if (field_0x26e == 0) {
                        field_0x1dc = 9;
                        field_0x26e = 1;
                    }
                    return;
                }
            } else if (lbl_8053AD30->fn_80309D48(getPlayerInputSlot_8030AAF4(i), 8)) {
                if (field_0x1c0[i] > 0 && field_0x1c0[i] <= 3) {
                    field_0x1c0[i]--;
                    if (field_0x1c0[i] == 2) {
                        field_0x1c0[i]--;
                    }
                    field_0x1e0[i] = 30;
                    updateHandicapCursor(this, i);
                    gAudioMgr_8053AF80->fn_80458880(0x26, -1);
                }
            } else if (lbl_8053AD30->fn_80309D48(getPlayerInputSlot_8030AAF4(i), 4)) {
                if (field_0x1c0[i] >= 0 && field_0x1c0[i] < 3) {
                    field_0x1c0[i]++;
                    if (field_0x1c0[i] == 2) {
                        field_0x1c0[i]++;
                    }
                    field_0x1e0[i] = 30;
                    updateHandicapCursor(this, i);
                    if (field_0x1c0[i] == 3) {
                        field_0x254[i] = 1;
                    }
                    gAudioMgr_8053AF80->fn_80458880(0x26, -1);
                }
            }
        }
        field_0x1e0[i]++;
        if (field_0x1e0[i] >= 60) {
            field_0x1e0[i] = 0;
        }
    }
}

void cls_804B0B00::fn_803E6A2C() {
    if (field_0x5c->fn_804366B8()) {
        if (field_0x26e == 0) {
            field_0x1dc = 9;
            field_0x26e = 1;
        }
        if (field_0x1bc == 0) {
            field_0x1b8 = 1;
        } else {
            field_0x1b8 = 0;
        }
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    }
}

void cls_804B0B00::fn_803E6A28() {}

// FIXME: 99.17. Stack slots and registers only: the original converts every
// int / u8 to float in the one 0x8 slot (ours uses 0x8-0x18), and the loop's
// table pointers / the lit alpha sit in other registers.
// Draws states 2-4 (the player-count menu, mScreen): the cursor
// 'Pscuro01' / 'Pscuro02' fades with field_0x1e0[0] (in state 4 it fades to
// 230 over 15 frames, then by (n - 15)^2 / 225), the chosen count's pane
// (field_0x1bc) bounces with field_0x254[0] and shows its lit version at
// 80 * the fade; the others show the plain one.
void cls_804B0B00::fn_803E64A0() {
    f32 alpha;
    f32 t;
    if (field_0x1d4 != 4) {
        if (field_0x1e0[0] < 30) {
            t = (f32)(field_0x1e0[0] * field_0x1e0[0]) / 900.0f;
        } else {
            t = 1.0f - (f32)((field_0x1e0[0] - 30) * (field_0x1e0[0] - 30)) / 900.0f;
        }
        alpha = t * field_0x268;
    } else {
        if (field_0x1e0[0] < 15) {
            t = (f32)(field_0x1e0[0] * field_0x1e0[0]) / 225.0f;
        } else {
            t = (f32)((field_0x1e0[0] - 15) * (field_0x1e0[0] - 15)) / 225.0f;
        }
        alpha = 230.0f * t;
    }
    s32 bounce;
    if (field_0x1d4 == 3 || (field_0x1d4 == 4 && field_0x254[0] <= 10)) {
        if (field_0x254[0] < 5) {
            bounce = field_0x254[0];
        } else {
            bounce = 10 - field_0x254[0];
        }
    } else {
        bounce = 0;
    }
    f32 lit = 80.0f * t;
    f32 offset = 5.0f * (f32)(bounce * bounce) / 25.0f;
    for (s32 i = 0; i < 4; i++) {
        if (i == field_0x1bc) {
            f32 x = mScreen->search(lbl_80478818[i])->getBounds()->i.x;
            mScreen->search(lbl_80478818[i])->move(x, field_0x110[i] + offset);
            if (i == 0) {
                x = mScreen->search('Pscuro02')->getBounds()->i.x;
                mScreen->search('Pscuro02')->move(x, field_0x1a0 + offset);
            } else {
                f32 top = mScreen->search(lbl_80478818[0])->getBounds()->i.y;
                f32 y = field_0x1a0 + mScreen->search(lbl_80478818[i])->getBounds()->i.y - top;
                x = mScreen->search('Pscuro02')->getBounds()->i.x;
                mScreen->search('Pscuro02')->move(x, y);
            }
            mScreen->search(lbl_80478838[i])->hide();
            mScreen->search(lbl_80478858[i])->show();
            mScreen->search(lbl_80478858[i])->setAlpha(lit);
        } else {
            f32 x = mScreen->search(lbl_80478818[i])->getBounds()->i.x;
            mScreen->search(lbl_80478818[i])->move(x, field_0x110[i]);
            mScreen->search(lbl_80478838[i])->show();
            mScreen->search(lbl_80478858[i])->hide();
        }
    }
    mScreen->search('Pscuro01')->setAlpha(alpha);
    mScreen->search('Pscuro02')->setAlpha(alpha);
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
}

// FIXME: 97.03. 4 fewer instrs (1179 vs 1183): stack slot and induction
// variable register allocation across the three per-player loops.
void cls_804B0B00::fn_803E51C4() {
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (i + 4))) {
            u8 alpha;
            if (field_0x220[i] == 0) {
                field_0x26b &= ~(1 << i);
                alpha = 0;
            } else if (field_0x220[i] < 36) {
                alpha = 150.0f * ((f32)(field_0x220[i] * field_0x220[i]) / 1296.0f);
            } else {
                if (field_0x25e == 0) {
                    field_0x26b |= (1 << i);
                }
                if (field_0x26b & (1 << i)) {
                    if (field_0x25e < 50) {
                        alpha = 105.0f * ((f32)(field_0x25e * field_0x25e) / 2500.0f) + 150.0f;
                    } else {
                        alpha = 105.0f * ((f32)((50 - (field_0x25e - 50)) * (50 - (field_0x25e - 50))) / 2500.0f) + 150.0f;
                    }
                } else {
                    alpha = 150;
                }
            }
            field_0x8->search(lbl_80478210[i])->setAlpha(alpha);
            if (field_0x26b & (1 << (i + 4))) {
                field_0x26b &= ~(1 << (i + 4));
                field_0x8->search(lbl_804781B0[i])->setAlpha(field_0x265);
                field_0x8->search(lbl_804781D0[i])->setAlpha(field_0x266);
                field_0x8->search(lbl_804781F0[i])->setAlpha(field_0x267);
            }
        } else {
            field_0x8->search(lbl_80478210[i])->setAlpha(0);
            if (lbl_8053AD30->fn_8030A388(getPlayerInputSlot_8030AAF4(i))) {
                if (!(field_0x26b & (1 << (i + 4))) && field_0x25c == 50) {
                    field_0x26b |= (1 << (i + 4));
                }
                if (field_0x26b & (1 << (i + 4))) {
                    f32 t;
                    if (field_0x25c < 50) {
                        t = (f32)(field_0x25c * field_0x25c) / 2500.0f;
                    } else {
                        t = (f32)((50 - (field_0x25c - 50)) * (50 - (field_0x25c - 50))) / 2500.0f;
                    }
                    field_0x8->search(lbl_804781B0[i])->setAlpha(t * field_0x265);
                    field_0x8->search(lbl_804781D0[i])->setAlpha(t * field_0x266);
                    field_0x8->search(lbl_804781F0[i])->setAlpha(t * field_0x267);
                }
            } else if (field_0x26b & (1 << (i + 4))) {
                field_0x26b &= ~(1 << (i + 4));
                field_0x8->search(lbl_804781B0[i])->setAlpha(field_0x265);
                field_0x8->search(lbl_804781D0[i])->setAlpha(field_0x266);
                field_0x8->search(lbl_804781F0[i])->setAlpha(field_0x267);
            }
        }
    }
    field_0x8->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x1d4 != 5) {
        if (field_0x230[0] == 0 && field_0x230[1] == 0 && field_0x230[2] == 0 && field_0x230[3] == 0) {
            field_0x250 = nextUnconnected(field_0x250, field_0x1bc + 1);
            if (field_0x250 != -1) {
                field_0x230[field_0x250]++;
            }
        }
        if (field_0x240[0] == 0 && field_0x240[1] == 0 && field_0x240[2] == 0 && field_0x240[3] == 0) {
            field_0x252 = nextUnconnected(field_0x252, field_0x1bc + 1);
            if (field_0x252 != -1) {
                field_0x240[field_0x252]++;
            }
        }
    }
    for (s32 i = 0; i < field_0x1bc + 1; i++) {
        if (field_0x230[i] != 0) {
            field_0x230[i]++;
            if (field_0x230[i] >= field_0x38[i]->getFrameMax()) {
                field_0x230[i] = 0;
            }
            field_0x38[i]->setFrame(field_0x230[i]);
            field_0x18[i]->search('NULL_001')->animationTransform();
            if (i == field_0x250 && field_0x230[i] == field_0x38[i]->getFrameMax() - 10) {
                s16 next = nextUnconnected(field_0x250, field_0x1bc + 1);
                if (next != -1 && field_0x250 != next) {
                    field_0x250 = next;
                    field_0x230[next]++;
                    field_0x38[i]->setFrame(field_0x230[next]);
                    field_0x18[next]->search('NULL_001')->animationTransform();
                }
            }
        }
        if (field_0x240[i] != 0) {
            field_0x240[i]++;
            if (field_0x240[i] >= field_0x48[i]->getFrameMax()) {
                field_0x240[i] = 0;
            }
            field_0x48[i]->setFrame(field_0x240[i]);
            field_0x28[i]->search('Ndatay00')->animationTransform();
            if (i == field_0x252 && field_0x240[i] == field_0x48[i]->getFrameMax() - 10) {
                s16 next = nextUnconnected(field_0x252, field_0x1bc + 1);
                if (next != -1 && field_0x252 != next) {
                    field_0x252 = next;
                    field_0x240[next]++;
                    field_0x48[i]->setFrame(field_0x240[next]);
                    field_0x28[next]->search('Ndatay00')->animationTransform();
                }
            }
        }
        f32 w1 = field_0x18[i]->search('NULL_001')->getWidth();
        f32 x = field_0x8->search('NULL_001')->getBounds()->i.x +
                field_0x8->search(lbl_804782D0[i])->getBounds()->i.x +
                (field_0x8->search(lbl_804782D0[i])->getWidth() - w1) / 2;
        f32 h1 = field_0x18[i]->search('NULL_001')->getHeight();
        f32 y = field_0x8->search('NULL_001')->getBounds()->i.y +
                field_0x8->search(lbl_804782D0[i])->getBounds()->i.y +
                (field_0x8->search(lbl_804782D0[i])->getHeight() - h1) / 2;
        field_0x18[i]->search('NULL_001')->move(x, y);
        f32 x2 = field_0x8->search('Ndata03')->getBounds()->i.x +
                 field_0x8->search(lbl_804782F0[i])->getBounds()->i.x;
        f32 y0 = field_0x28[i]->search('Pccaa00')->getBounds()->i.y;
        f32 y2 = field_0x8->search(lbl_80478250[i][1])->getBounds()->i.y +
                 (field_0x8->search('Ndata03')->getBounds()->i.y +
                  field_0x8->search(lbl_804782F0[i])->getBounds()->i.y) -
                 y0;
        field_0x28[i]->search('Ndatay00')->move(x2, y2);
        field_0x28[i]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
        if (field_0x1d4 != 5 && !(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (i + 4)))) {
            field_0x18[i]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
        }
    }
    JGeometry::TBox2f box(0.0f, 0.0f, 224.0f, 168.0f);
    gGameRoot2_8053AB10->fn_8013B414(&box, true);
    for (s32 i = 0; i < 4; i++) {
        SpriteParams_801F5AE0 params;
        params.field_0x26 = 0;
        JGeometry::TVec3f pos;
        f32 size = field_0x8->search(lbl_80478190[i])->getWidth();
        pos.x = toEffectX(size / 2 + field_0x8->search(lbl_80478190[i])->getGlobalBounds().i.x);
        size = field_0x8->search(lbl_80478190[i])->getHeight();
        pos.y = toEffectY(size / 2 + field_0x8->search(lbl_80478190[i])->getGlobalBounds().i.y);
        pos.z = 0.0f;
        params.field_0x0[7] = i + 12;
        f32 scale = (f32)field_0x8->search(lbl_80477EA0[i][0])->getAlpha() / (f32)field_0x2fe[i][0];
        if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (i + 4))) || field_0x220[i] == 0) {
            if (i > field_0x1bc) {
                params.field_0x10.r = 128;
                params.field_0x10.g = 128;
                params.field_0x10.b = 128;
                params.field_0x10.a = 128.0f * scale;
            } else {
                params.field_0x10.a = 255.0f * scale;
            }
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x10, &params, 1.0f, 1.0f);
        } else {
            params.field_0x10.a = 255.0f * scale;
            if (field_0x220[i] <= 10) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0xDB, &params, 1.0f, 1.0f);
            } else if (field_0x220[i] <= 36) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, (field_0x220[i] - 10) / 2 + 0xDB, &params, 1.0f, 1.0f);
            } else if (field_0x220[i] < 100) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0xE8, &params, 1.0f, 1.0f);
            } else {
                s32 frame = lbl_804779B0[field_0x220[i] - 100] + 0x10;
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
            }
        }
    }
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0x1d4 == 8) {
        lbl_8053AEC0->fn_804316EC();
    }
    lbl_8053AED8->fn_80440EE8();
}

// FIXME: 99.67. Stack layout and float registers only: the original keeps
// every int-to-float conversion in one 8-byte slot (0x18; frame 0x60), ours
// uses two (frame 0x70), and the bounce offset / pane x swap f29 and f30.
// Draws states 7-9 (the game-select menu, field_0xc): the cursor
// 'Pscuro01' / 'Pscuro02' fades in and out (field_0x1e0[0] over 60 frames;
// in state 9 it fades to 230 over 30, or is hidden while field_0x1cc is 0)
// and the chosen entry ('Pstab04' / 'Phanb04' / 'Prub04', field_0x1a8)
// bounces with field_0x254[0]; then the screen, the player sprites
// (fn_803F0A54) and lbl_8053AED8's buttons.
void cls_804B0B00::fn_803E4940() {
    f32 alpha;
    if (field_0x1d4 != 11) {
        f32 t;
        if (field_0x1e0[0] < 30) {
            t = (f32)(field_0x1e0[0] * field_0x1e0[0]) / 900.0f;
        } else {
            t = 1.0f - (f32)((field_0x1e0[0] - 30) * (field_0x1e0[0] - 30)) / 900.0f;
        }
        alpha = t * field_0x269;
    } else if (field_0x1dc == 2) {
        alpha = 0.0f;
    } else {
        f32 t;
        if (field_0x1e0[0] < 15) {
            t = (f32)(field_0x1e0[0] * field_0x1e0[0]) / 225.0f;
        } else {
            t = 1.0f - (f32)((field_0x1e0[0] - 15) * (field_0x1e0[0] - 15)) / 225.0f;
        }
        alpha = 230.0f * t;
    }
    s32 bounce;
    if (field_0x1d4 == 0 || field_0x1d4 == 10 ||
        (field_0x1d4 == 11 && field_0x1dc != 2 && field_0x254[0] <= 10)) {
        if (field_0x254[0] < 5) {
            bounce = field_0x254[0];
        } else {
            bounce = 10 - field_0x254[0];
        }
    } else {
        bounce = 0;
    }
    f32 offset = 5.0f * (f32)(bounce * bounce) / 25.0f;
    if (field_0x1b8 == 0) {
        f32 x = field_0xc->search('Pstab04')->getBounds()->i.x;
        field_0xc->search('Pstab04')->move(x, field_0x150[0] + offset);
        x = field_0xc->search('Phanb04')->getBounds()->i.x;
        field_0xc->search('Phanb04')->move(x, field_0x150[1]);
        x = field_0xc->search('Prub04')->getBounds()->i.x;
        field_0xc->search('Prub04')->move(x, field_0x150[2]);
        x = field_0xc->search('Pscuro02')->getBounds()->i.x;
        field_0xc->search('Pscuro02')->move(x, field_0x1a8 + offset);
    } else if (field_0x1b8 == 1) {
        f32 x = field_0xc->search('Pstab04')->getBounds()->i.x;
        field_0xc->search('Pstab04')->move(x, field_0x150[0]);
        x = field_0xc->search('Phanb04')->getBounds()->i.x;
        field_0xc->search('Phanb04')->move(x, field_0x150[1] + offset);
        x = field_0xc->search('Prub04')->getBounds()->i.x;
        field_0xc->search('Prub04')->move(x, field_0x150[2]);
        f32 top = field_0xc->search('Pstab04')->getBounds()->i.y;
        f32 y = field_0x1a8 + field_0xc->search('Phanb04')->getBounds()->i.y - top;
        x = field_0xc->search('Pscuro02')->getBounds()->i.x;
        field_0xc->search('Pscuro02')->move(x, y);
    } else {
        f32 x = field_0xc->search('Pstab04')->getBounds()->i.x;
        field_0xc->search('Pstab04')->move(x, field_0x150[0]);
        x = field_0xc->search('Phanb04')->getBounds()->i.x;
        field_0xc->search('Phanb04')->move(x, field_0x150[1]);
        x = field_0xc->search('Prub04')->getBounds()->i.x;
        field_0xc->search('Prub04')->move(x, field_0x150[2] + offset);
        f32 top = field_0xc->search('Pstab04')->getBounds()->i.y;
        f32 y = field_0x1a8 + field_0xc->search('Prub04')->getBounds()->i.y - top;
        x = field_0xc->search('Pscuro02')->getBounds()->i.x;
        field_0xc->search('Pscuro02')->move(x, y);
    }
    field_0xc->search('Pscuro01')->setAlpha(alpha);
    field_0xc->search('Pscuro02')->setAlpha(alpha);
    field_0xc->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    JGeometry::TBox2f box(0.0f, 0.0f, 280.0f, 210.0f);
    gGameRoot2_8053AB10->fn_8013B414(&box, true);
    fn_803E1668();
    lbl_8053AED8->fn_80440EE8();
}

// FIXME: 99.81. Stack slots and float registers only: the original converts
// both blink branches' ints in the 0x8 slot, ours the first branch's in 0x10
// (and f30 / f31 swap for the bounce offset and pane x).
// Draws states 12 and 13 (the results screen field_0x10): each taking
// player's cursor 'Pscur' fades with field_0x1e0[i] (60 frames), the
// joined ones' 'Paccur' panes with it; outside state 12 the 'Paccur02'
// pane and field_0x68[i] bounce with field_0x254[i]. Then lbl_8053AED8's
// buttons.
void cls_804B0B00::fn_803E4650() {
    for (s32 i = 0; i < 4; i++) {
        f32 t;
        if (field_0x1e0[i] < 30) {
            t = (f32)(field_0x1e0[i] * field_0x1e0[i]) / 900.0f;
        } else {
            t = 1.0f - (f32)((field_0x1e0[i] - 30) * (field_0x1e0[i] - 30)) / 900.0f;
        }
        if (i <= field_0x1bc) {
            if (field_0x1c0[i] < 4) {
                field_0x10->search(lbl_80478330[i])->setAlpha(160.0f * t);
            }
            if (field_0x1c0[i] == 3) {
                field_0x10->search(lbl_80478350[i])->setAlpha(t * field_0x26a);
                field_0x10->search(lbl_80478370[i])->setAlpha(t * field_0x26a);
            }
        }
        if (field_0x1d4 != 12) {
            s32 bounce = field_0x254[i] < 5 ? field_0x254[i] : 10 - field_0x254[i];
            f32 offset = 5.0f * (f32)(bounce * bounce) / 25.0f;
            f32 x = field_0x10->search(lbl_80478350[i])->getBounds()->i.x;
            field_0x10->search(lbl_80478350[i])->move(x, field_0x1b0 + offset);
            if (field_0x1c0[i] >= 3) {
                field_0x68[i]->moveFromCentre_80402E08(0.0f, offset);
            }
        }
    }
    field_0x10->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
}

void cls_804B0B00::fn_803E462C() {
    field_0x5c->fn_80436668();
}

void cls_804B0B00::fn_803E4610() {
    field_0x1e0[0] = 0;
    field_0x254[0] = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 0;
}

void cls_804B0B00::fn_803E45FC() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 1;
}

// Enter state 2: slides the menu panes off screen (offsets cleared by the
// update functions as they slide back in). fn_803EDBF0 inlines it.
void cls_804B0B00::fn_803E4344() {
    field_0xf8 = -150.0f;
    mScreen->search('Pmadbb07')->move(field_0xf0, field_0xf4 + field_0xf8);
    field_0xfc = -200.0f;
    field_0x78->moveFromCentre_80402E08(0.0f, field_0xfc);
    field_0x120[0] = -350.0f;
    mScreen->search('Pnopb04')->move(field_0x100[0] + field_0x120[0], field_0x110[0]);
    field_0x120[1] = -350.0f;
    mScreen->search('Pnopb08')->move(field_0x100[1] + field_0x120[1], field_0x110[1]);
    field_0x120[2] = -350.0f;
    mScreen->search('Pnopb12')->move(field_0x100[2] + field_0x120[2], field_0x110[2]);
    field_0x120[3] = -350.0f;
    mScreen->search('Pnopb16')->move(field_0x100[3] + field_0x120[3], field_0x110[3]);
    field_0x138 = 400.0f;
    mScreen->search('Ndata02')->move(field_0x130 + field_0x138, field_0x134);
    field_0x14c = 400.0f;
    mScreen->search('Ndata05')->move(field_0x13c + field_0x14c, field_0x140);
    mScreen->search('Ndata07')->move(field_0x144 + field_0x14c, field_0x148);
    field_0x220[0] = 0;
    field_0x1e0[0] = 0;
    field_0x254[0] = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 2;
}

void cls_804B0B00::fn_803E4330() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 3;
}

void cls_804B0B00::fn_803E4308() {
    field_0x220[0] = 0;
    field_0x254[0] = 0;
    field_0x1e0[0] = 5;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 4;
}

// Enter state 5 (the GBA connection screen field_0x8): the player-count
// textures ('Ptn01' / 'Ptn02', 2-4 players), message 0x3E in 'Tkg01' /
// 'Tkg02', the panes moved off screen to slide in, and per player the
// 'Pcgbak' / 'Pcca' panes shown (taking part, 'Ppln' opaque, the
// animation lengths in field_0x230 / field_0x240) or hidden ('Ppln' at 128).
void cls_804B0B00::fn_803E3CB0() {
    if (field_0x1bc == 1) {
        const ResTIMG* tex = (const ResTIMG*)fn_804031C4('TIMG', "kk_nPlayerP2_01.bti");
        static_cast<J2DPicture*>(field_0x8->search('Ptn02'))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x8->search('Ptn01'))->changeTexture(tex, 0);
    } else if (field_0x1bc == 2) {
        const ResTIMG* tex = (const ResTIMG*)fn_804031C4('TIMG', "kk_nPlayerP3_01.bti");
        static_cast<J2DPicture*>(field_0x8->search('Ptn02'))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x8->search('Ptn01'))->changeTexture(tex, 0);
    } else {
        const ResTIMG* tex = (const ResTIMG*)fn_804031C4('TIMG', "kk_nPlayerP4_01.bti");
        static_cast<J2DPicture*>(field_0x8->search('Ptn02'))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x8->search('Ptn01'))->changeTexture(tex, 0);
    }
    char* text1 = static_cast<J2DTextBox*>(field_0x8->search('Ttkg01'))->getStringPtr();
    char* text2 = static_cast<J2DTextBox*>(field_0x8->search('Ttkg02'))->getStringPtr();
    getMessageText_803832BC(text1, 0x3E, 0, NULL, NULL);
    getMessageText_803832BC(text2, 0x3E, 0, NULL, NULL);
    field_0xa0 = -300.0f;
    field_0x8->search('Ndata03')->move(field_0x98, field_0x9c + field_0xa0);
    field_0xac = -100.0f;
    field_0x8->search('Ndata05')->move(field_0xa4, field_0xa8 + field_0xac);
    field_0xb8 = -200.0f;
    field_0x8->search('Ndata06')->move(field_0xb0, field_0xb4 + field_0xb8);
    field_0xbc = -200.0f;
    field_0x78->moveFromCentre_80402E08(0.0f, field_0xbc);
    field_0xe0[0] = 200.0f;
    field_0x8->search('Ppllbk02')->move(field_0xc0[0], field_0xd0[0] + field_0xe0[0]);
    field_0xe0[1] = 200.0f;
    field_0x8->search('Ppllbk12')->move(field_0xc0[1], field_0xd0[1] + field_0xe0[1]);
    field_0xe0[2] = 200.0f;
    field_0x8->search('Ppllbk22')->move(field_0xc0[2], field_0xd0[2] + field_0xe0[2]);
    field_0xe0[3] = 200.0f;
    field_0x8->search('Ppllbk32')->move(field_0xc0[3], field_0xd0[3] + field_0xe0[3]);
    for (s32 i = 0; i < 4; i++) {
        if (i <= field_0x1bc) {
            field_0x8->search(lbl_804781B0[i])->show();
            for (s32 j = 2; j < 4; j++) {
                field_0x8->search(lbl_80478250[i][j])->show();
            }
            field_0x8->search(lbl_80478230[i])->setAlpha(255);
            field_0x230[i] = field_0x38[i]->getFrameMax();
            field_0x240[i] = field_0x48[i]->getFrameMax();
        } else {
            field_0x8->search(lbl_804781B0[i])->hide();
            for (s32 j = 2; j < 4; j++) {
                field_0x8->search(lbl_80478250[i][j])->hide();
            }
            field_0x8->search(lbl_80478230[i])->setAlpha(128);
            field_0x230[i] = 0;
            field_0x240[i] = 0;
        }
    }
    field_0x254[0] = 0;
    field_0x25e = 0;
    field_0x25c = 0;
    field_0x26b = 0;
    field_0x250 = -1;
    field_0x252 = -1;
    field_0x262 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x220[i] = 0;
    }
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 5;
}

void cls_804B0B00::fn_803E3C94() {
    field_0x254[0] = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 6;
}

void cls_804B0B00::fn_803E3C80() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 7;
}

void cls_804B0B00::fn_803E3C00() {
    if (field_0x1bc == 0) {
        lbl_8053AEC0->fn_80431A9C(0xA9, field_0x1bc + 1, NULL);
    } else {
        lbl_8053AEC0->fn_80431A9C(0xC4, field_0x1bc + 1, NULL);
    }
    field_0x26c = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 8;
}

// Enter state 9 (the game-select screen field_0xc): sets the messages of
// 'Tsta01' / 'Than01' / 'Trule01', toggles the 1P vs 2-4P panes, sets the
// player-count texture in 'Pnop02' / 'Pnop01' (1-4 players), and moves the
// panes off screen to slide in.
void cls_804B0B00::fn_803E3564() {
    char* han = static_cast<J2DTextBox*>(field_0xc->search('Than01'))->getStringPtr();
    char* sta = static_cast<J2DTextBox*>(field_0xc->search('Tsta01'))->getStringPtr();
    char* rule = static_cast<J2DTextBox*>(field_0xc->search('Trule01'))->getStringPtr();
    if (field_0x1bc == 0) {
        if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(0))) {
            field_0xc->search('Pgba02')->hide();
            field_0xc->search('Pcon02')->show();
        } else {
            field_0xc->search('Pgba02')->show();
            field_0xc->search('Pcon02')->hide();
        }
        getMessageText_803832BC(sta, 0x11B, 0, NULL, NULL);
        getMessageText_803832BC(han, 0x11B, 0, NULL, NULL);
        getMessageText_803832BC(rule, 0xF7, 0, NULL, NULL);
        const ResTIMG* tex = (const ResTIMG*)fn_804031C4('TIMG', "kk_nPlayerP1_01.bti");
        static_cast<J2DPicture*>(field_0xc->search('Pnop02'))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0xc->search('Pnop01'))->changeTexture(tex, 0);
        field_0xc->search('Pstab04')->hide();
    } else {
        getMessageText_803832BC(sta, 0x11B, 0, NULL, NULL);
        getMessageText_803832BC(han, 0xAC, 0, NULL, NULL);
        getMessageText_803832BC(rule, 0xF7, 0, NULL, NULL);
        field_0xc->search('Pgba02')->hide();
        field_0xc->search('Pcon02')->show();
        if (field_0x1bc == 1) {
            const ResTIMG* tex = (const ResTIMG*)fn_804031C4('TIMG', "kk_nPlayerP2_01.bti");
            static_cast<J2DPicture*>(field_0xc->search('Pnop02'))->changeTexture(tex, 0);
            static_cast<J2DPicture*>(field_0xc->search('Pnop01'))->changeTexture(tex, 0);
        } else if (field_0x1bc == 2) {
            const ResTIMG* tex = (const ResTIMG*)fn_804031C4('TIMG', "kk_nPlayerP3_01.bti");
            static_cast<J2DPicture*>(field_0xc->search('Pnop02'))->changeTexture(tex, 0);
            static_cast<J2DPicture*>(field_0xc->search('Pnop01'))->changeTexture(tex, 0);
        } else {
            const ResTIMG* tex = (const ResTIMG*)fn_804031C4('TIMG', "kk_nPlayerP4_01.bti");
            static_cast<J2DPicture*>(field_0xc->search('Pnop02'))->changeTexture(tex, 0);
            static_cast<J2DPicture*>(field_0xc->search('Pnop01'))->changeTexture(tex, 0);
        }
        field_0xc->search('Pstab04')->show();
    }
    field_0x164 = -200.0f;
    field_0x168 = -200.0f;
    field_0x184[0] = 300.0f;
    field_0x184[1] = 300.0f;
    field_0x184[2] = 300.0f;
    field_0x198 = -450.0f;
    field_0xc->search('Pmadbb07')->move(field_0x15c, field_0x160 + field_0x164);
    field_0x78->moveFromCentre_80402E08(0.0f, field_0x168);
    field_0xc->search('Pstab04')->move(field_0x16c[0] + field_0x184[0], field_0x178[0]);
    field_0xc->search('Phanb04')->move(field_0x16c[1] + field_0x184[1], field_0x178[1]);
    field_0xc->search('Prub04')->move(field_0x16c[2] + field_0x184[2], field_0x178[2]);
    field_0xc->search('Plibkk03')->move(field_0x190 + field_0x198, field_0x194);
    field_0x220[0] = 0;
    field_0x1e0[0] = 0;
    field_0x254[0] = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 9;
}

void cls_804B0B00::fn_803E3548() {
    field_0x254[0] = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 10;
}

// FIXME: 99.20. One register in setEntries' loop (li r4, 0 vs mr r4, r3, as in fn_803EC804).
// Not auto-inlined: fn_803E9034 and fn_803ED604 call fn_803E3320 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void cls_804B0B00::fn_803E3320() {
    field_0x254[0] = 0;
    field_0x1e0[0] = 5;
    if (field_0x1bc == 0) {
        if (field_0x1b8 == 1) {
            setSingleEntry();
        }
    } else if (field_0x1b8 == 0) {
        setEntries(this);
    }
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 11;
}
#pragma auto_inline reset

// FIXME: 98.58. Registers: the loop's table pointers and counter sit in other
// registers (i in r18 here, r16 in the original), and the first digit
// texture is copied through r0 into r17 (one extra mr) where the original
// keeps it in r27.
// Enter state 12 (the results / entry screen field_0x10): per player, the
// 'Pblk' cover over the players beyond field_0x1bc + 1, the rupee count
// (fn_80403070) as three digit textures, the fn_8040305C - 1 markers of the
// four marker tables, the cursor panes by field_0x1c0[i] (3: joined,
// 4: not); then the panes start their slide-in.
void cls_804B0B00::fn_803E2A20() {
    for (s32 i = 0; i < 4; i++) {
        if (i >= 1) {
            if (i > field_0x1bc) {
                field_0x10->search(lbl_80478878[i - 1])->show();
            } else {
                field_0x10->search(lbl_80478878[i - 1])->hide();
            }
        }
        field_0x1f0[i] = fn_80403414(i);
        field_0x200[i] = fn_804033D4(i);
        field_0x210[i] = fn_8040336C(i);
        s32 rupees = fn_80403070(field_0x1f0[i]);
        const ResTIMG* tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0AD8[rupees / 100]);
        static_cast<J2DPicture*>(field_0x10->search(lbl_80478410[i]))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x10->search(lbl_80478430[i]))->changeTexture(tex, 0);
        tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0AD8[rupees % 100 / 10]);
        static_cast<J2DPicture*>(field_0x10->search(lbl_804783D0[i]))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x10->search(lbl_804783F0[i]))->changeTexture(tex, 0);
        tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0AD8[rupees % 10]);
        static_cast<J2DPicture*>(field_0x10->search(lbl_80478390[i]))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x10->search(lbl_804783B0[i]))->changeTexture(tex, 0);
        s32 count = fn_8040305C(field_0x200[i]) - 1;
        for (s32 j = 0; j < 6; j++) {
            if (j < count) {
                field_0x10->search(lbl_80478518[i][j])->show();
                field_0x10->search(lbl_804785D8[i][j])->show();
                field_0x10->search(lbl_80478698[i][j])->hide();
                field_0x10->search(lbl_80478758[i][j])->hide();
            } else if (j == count) {
                field_0x10->search(lbl_80478518[i][j])->hide();
                field_0x10->search(lbl_804785D8[i][j])->hide();
                field_0x10->search(lbl_80478698[i][j])->hide();
                field_0x10->search(lbl_80478758[i][j])->show();
            } else {
                field_0x10->search(lbl_80478518[i][j])->hide();
                field_0x10->search(lbl_804785D8[i][j])->hide();
                field_0x10->search(lbl_80478698[i][j])->hide();
                field_0x10->search(lbl_80478758[i][j])->hide();
            }
        }
        field_0x10->search(lbl_80478350[i])->hide();
    }
    field_0x64->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x64->field_0x22 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i <= field_0x1bc) {
            field_0x68[i]->moveFromCentre_80402E08(0.0f, 150.0f);
            field_0x68[i]->field_0x22 = 0;
        }
        if (i <= field_0x1bc) {
            field_0x1c0[i] = 3;
        } else {
            field_0x1c0[i] = 4;
        }
        field_0x1e0[i] = 30;
        if (i <= field_0x1bc) {
            if (field_0x1c0[i] < 3) {
                field_0x10->search(lbl_80478310[i])->hide();
                field_0x10->search(lbl_80478330[i])->show();
                field_0x10->search(lbl_80478350[i])->hide();
            } else if (field_0x1c0[i] == 3) {
                field_0x10->search(lbl_80478310[i])->hide();
                field_0x10->search(lbl_80478330[i])->show();
                field_0x10->search(lbl_80478350[i])->show();
            } else {
                field_0x10->search(lbl_80478310[i])->show();
                field_0x10->search(lbl_80478330[i])->hide();
                field_0x10->search(lbl_80478350[i])->hide();
            }
        } else {
            field_0x10->search(lbl_80478310[i])->hide();
            field_0x10->search(lbl_80478330[i])->hide();
            field_0x10->search(lbl_80478350[i])->hide();
        }
        s32 state = field_0x1c0[i];
        if (state <= 3) {
            f32 x = field_0x10->search(lbl_80478330[i])->getBounds()->i.x;
            field_0x10->search(lbl_80478330[i])->move(x, lbl_80543648[state]);
        }
        field_0x254[i] = 0;
        field_0x1e0[i] = 0;
    }
    field_0x78->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x78->field_0x22 = 0;
    field_0x260 = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 12;
}

void cls_804B0B00::fn_803E2A0C() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 13;
}

void cls_804B0B00::fn_803E29C8() {
    field_0x5c->fn_8043670C(0);
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 14;
}

// Updates and draws the fade overlay field_0x60. Dead-stripped: fn_803EDA2C
// inlines it; its own copy created the unit's early .rodata ("kk_kettei_01.bti",
// "kk_modoru_01.bti") and .sdata2 (800.0f, 640.0f) pool entries before fn_803E2A20.
void cls_804B0B00::drawFade() {
    if (field_0x26e != 0) {
        switch (field_0x26e) {
        case 1:
            if (field_0x26d >= 245) {
                if (field_0x1dc == 9) {
                    lbl_8053AED8->fn_80440D78((const ResTIMG*)fn_804030C4('TIMG', "kk_kettei_01.bti"));
                    lbl_8053AED8->fn_80440CE4((const ResTIMG*)fn_804030C4('TIMG', "kk_modoru_01.bti"));
                }
                field_0x26d = 255;
                field_0x26e = 2;
                (this->*lbl_804B0754[field_0x1dc])();
            } else {
                field_0x26d += 10;
            }
            break;
        case 2:
            if (field_0x26d <= 10) {
                field_0x26d = 0;
                field_0x26e = 0;
            } else {
                field_0x26d -= 10;
            }
            break;
        }
        gGameRoot2_8053AB10->fn_8013B490();
        field_0x60->setAlpha(field_0x26d);
        field_0x60->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    }
}

// Enter state 15 (the terminal state after a slide-out completes). Dead-stripped:
// fn_803EA3F8, fn_803EBAA8 and fn_803E6AB0 inline it (lbl_804B0754[15] is NULL);
// its own copy created the 3.0f .sdata2 literal right after fn_803E1668.
void cls_804B0B00::enterState15() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 15;
    if (field_0x1bc == 2 && lbl_80529DEC.field_0x64 == -1) {
        s32 r = 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        if (r >= 3) {
            r = 2;
        }
        lbl_80529DEC.field_0x64 = r;
    }
    if (lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x82[3] & 1) {
        lbl_80529DEC.field_0x237 = 5;
        field_0x360 = 6;
    } else {
        lbl_80529DEC.field_0x118 = 8;
        lbl_80529DEC.field_0x11c = 3;
        field_0x360 = 7;
        lbl_80529DEC.field_0x242 = 0;
    }
    lbl_80529DEC.field_0x23e[0] = 0;
}

// FIXME: 98.92. Same length. Registers and scheduling of the table index
// arithmetic (i / 2 + (i % 2) * 2 and the [field_0x1bc] row address are
// computed in another order).
// Draws the four player sprites ("player_sob.bin") over field_0xc's player
// panes, sliding them between the field_0x1bc + 1 player layout and the
// 4-player one as field_0x220[0] counts 100-112 and 212-224 (the
// ConnectScreen_804B0F10::fn_803F0A54 twin, with a 1-player row).
void cls_804B0B00::fn_803E1668() {
    f32 ratio = (f32)field_0xc->search(lbl_804780C0[0])->getAlpha() / (f32)field_0x342[0];
    JGeometry::TVec3f pos;
    if (field_0x220[0] < 100) {
        for (s32 i = 0; i < 4; i++) {
            if (lbl_80478890[field_0x1bc][i] != 0) {
                SpriteParams_801F5AE0 params;
                params.field_0x26 = 0;
                params.field_0x10.a = 255.0f * ratio;
                f32 x;
                PANE_CENTRE_X(x, lbl_80478890[field_0x1bc][i]);
                pos.x = toSpriteX(x);
                f32 y;
                PANE_CENTRE_Y(y, lbl_80478890[field_0x1bc][i]);
                pos.y = toSpriteY(y);
                pos.z = 0.0f;
                params.field_0x0[7] = i + 12;
                s32 frame = lbl_804779B0[field_0x220[0] % 30] + 0x10;
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
            }
        }
    } else if (field_0x220[0] < 112) {
        // Slide from the field_0x1bc + 1 player panes to the 4-player ones.
        for (s32 i = 3; i >= 0; i--) {
            SpriteParams_801F5AE0 params;
            params.field_0x26 = 0;
            params.field_0x10.a = 255.0f * ratio;
            f32 from;
            f32 to;
            switch (field_0x1bc) {
            case 0:
                PANE_CENTRE_X(from, lbl_80478890[field_0x1bc][0]);
                PANE_CENTRE_X(to, lbl_80478890[3][i]);
                break;
            case 1:
                PANE_CENTRE_X(from, lbl_80478890[field_0x1bc][i % 2]);
                PANE_CENTRE_X(to, lbl_80478890[3][i / 2 + (i % 2) * 2]);
                break;
            case 2:
                if (i != 3) {
                    PANE_CENTRE_X(from, lbl_80478890[field_0x1bc][i]);
                    PANE_CENTRE_X(to, lbl_80478890[3][i]);
                } else {
                    PANE_CENTRE_X(from, lbl_80478890[field_0x1bc][2]);
                    PANE_CENTRE_X(to, lbl_80478890[3][i]);
                }
                break;
            }
            pos.x = toSpriteX((to - from) * ((f32)(field_0x220[0] - 100) / 12.0f) + from);
            f32 y;
            PANE_CENTRE_Y(y, lbl_80478890[3][i]);
            pos.y = toSpriteY(y);
            pos.z = 0.0f;
            params.field_0x0[7] = i + 12;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x10, &params, 1.0f, 1.0f);
        }
    } else if (field_0x220[0] < 212) {
        for (s32 i = 0; i < 4; i++) {
            SpriteParams_801F5AE0 params;
            params.field_0x26 = 0;
            params.field_0x10.a = 255.0f * ratio;
            f32 x;
            PANE_CENTRE_X(x, lbl_80478890[3][i]);
            pos.x = toSpriteX(x);
            f32 y;
            PANE_CENTRE_Y(y, lbl_80478890[3][i]);
            pos.y = toSpriteY(y);
            pos.z = 0.0f;
            if (field_0x1bc == 1) {
                params.field_0x0[7] = i / 2 + (i % 2) * 2 + 12;
            } else {
                params.field_0x0[7] = i + 12;
            }
            s32 frame = lbl_804779B0[field_0x220[0] % 30] + 0x10;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
        }
    } else {
        // Slide back to the field_0x1bc + 1 player panes.
        for (s32 i = 3; i >= 0; i--) {
            SpriteParams_801F5AE0 params;
            params.field_0x26 = 0;
            params.field_0x10.a = 255.0f * ratio;
            f32 from;
            f32 to;
            switch (field_0x1bc) {
            case 0:
                PANE_CENTRE_X(from, lbl_80478890[3][i]);
                PANE_CENTRE_X(to, lbl_80478890[field_0x1bc][0]);
                break;
            case 1:
                PANE_CENTRE_X(from, lbl_80478890[3][i / 2 + (i % 2) * 2]);
                PANE_CENTRE_X(to, lbl_80478890[field_0x1bc][i % 2]);
                break;
            case 2:
                if (i != 3) {
                    PANE_CENTRE_X(from, lbl_80478890[3][i]);
                    PANE_CENTRE_X(to, lbl_80478890[field_0x1bc][i]);
                } else {
                    PANE_CENTRE_X(from, lbl_80478890[3][i]);
                    PANE_CENTRE_X(to, lbl_80478890[field_0x1bc][2]);
                }
                break;
            }
            pos.x = toSpriteX((to - from) * ((f32)(field_0x220[0] - 212) / 12.0f) + from);
            f32 y;
            PANE_CENTRE_Y(y, lbl_80478890[3][i]);
            pos.y = toSpriteY(y);
            pos.z = 0.0f;
            params.field_0x0[7] = i + 12;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x10, &params, 1.0f, 1.0f);
        }
    }
}
