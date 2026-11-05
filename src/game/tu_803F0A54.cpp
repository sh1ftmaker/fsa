#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Effect.h"
#include "game/ActorBase.h"
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
#include "JSystem/J2D/J2DAnmLoader.h"
#include "dolphin/os.h"
#include "JSystem/JUtility/JUTVideo.h"

// Entered when the state changes (index: the new state).
static ConnectScreen_804B0F10::StateFunc lbl_804B0BDC[13] = {
    &ConnectScreen_804B0F10::fn_803F3324, &ConnectScreen_804B0F10::fn_803F3310, &ConnectScreen_804B0F10::fn_803F32E8,
    &ConnectScreen_804B0F10::fn_803F2B78, &ConnectScreen_804B0F10::fn_803F2B5C, &ConnectScreen_804B0F10::fn_803F2B48,
    &ConnectScreen_804B0F10::fn_803F2AEC, &ConnectScreen_804B0F10::fn_803F267C, &ConnectScreen_804B0F10::fn_803F2660,
    &ConnectScreen_804B0F10::fn_803F2580, &ConnectScreen_804B0F10::fn_803F1C70, &ConnectScreen_804B0F10::fn_803F1C5C,
    &ConnectScreen_804B0F10::fn_803F1C18,
};

// Run each frame by fn_803FC304 (index: the state).
static ConnectScreen_804B0F10::StateFunc lbl_804B0D14[13] = {
    &ConnectScreen_804B0F10::fn_803FB9E4, &ConnectScreen_804B0F10::fn_803FB6F4, &ConnectScreen_804B0F10::fn_803FAA88,
    &ConnectScreen_804B0F10::fn_803F9EA8, &ConnectScreen_804B0F10::fn_803F943C, &ConnectScreen_804B0F10::fn_803F8AE4,
    &ConnectScreen_804B0F10::fn_803F8FBC, &ConnectScreen_804B0F10::fn_803F8354, &ConnectScreen_804B0F10::fn_803F8050,
    &ConnectScreen_804B0F10::fn_803F726C, &ConnectScreen_804B0F10::fn_803F715C, &ConnectScreen_804B0F10::fn_803F5A0C,
    &ConnectScreen_804B0F10::fn_803F59A0,
};

// Run each frame by fn_803FC46C (index: the state).
static ConnectScreen_804B0F10::StateFunc lbl_804B0E4C[13] = {
    &ConnectScreen_804B0F10::fn_803F5410, &ConnectScreen_804B0F10::fn_803F5410, &ConnectScreen_804B0F10::fn_803F5410,
    &ConnectScreen_804B0F10::fn_803F4120, &ConnectScreen_804B0F10::fn_803F4120, &ConnectScreen_804B0F10::fn_803F4120,
    &ConnectScreen_804B0F10::fn_803F4120, &ConnectScreen_804B0F10::fn_803F38A4, &ConnectScreen_804B0F10::fn_803F38A4,
    &ConnectScreen_804B0F10::fn_803F38A4, &ConnectScreen_804B0F10::fn_803F35B0, &ConnectScreen_804B0F10::fn_803F35B0,
    &ConnectScreen_804B0F10::fn_803F358C,
};

// Pane x offsets (lhax, index 0-3) read by fn_803F1C70 and fn_803F5A0C
// (not written yet).
static const s16 lbl_80543710[4] = {-25, 25, 77, 77};

// .rodata: the unit's tables in address order (the functions address them
// off one base); u64 tables are J2DPane::search tags. Same table set and
// layout as the twin game/tu_803E1668.cpp (cls_804B0B00), here for 2-4
// players.
static const f32 lbl_80478A60[16] = {
    50.0f, 206.0f, 50.0f, 206.0f, 60.0f, 60.0f, 132.0f, 132.0f, 87.0f, 464.0f, 87.0f, 464.0f, 159.0f, 159.0f, 327.0f, 327.0f,
};
// Sprite frame offsets for field_0x210[0] % 30 (the player sprites' idle
// animation).
static const s32 lbl_80478AA0[30] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6,
};
static const u64 lbl_80478B18[20] = {
    'Pmbabb07', 'Pmbabb06', 'Pmbabb05', 'Pmbabb04', 'Pmbabb03', 'Pmbabb02', 'Pmbabb01', 'Pmbab05', 'Pmbab04',
    'Pmbab03', 'Pmbab02', 'Pmbab01', 'Pmbam02', 'Pmbam01', 'Pmbalk02', 'Pmbal02', 'Pmbalk01', 'Pmbal01', 'Pmba02',
    'Pmba01',
};
static const u64 lbl_80478BB8[3][6] = {
    {'Pnop03', 'Pnop04', 'Pnopb05', 'Pnopb06', 'Pnopb07', 'Pnopb08'},
    {'Pnop05', 'Pnop06', 'Pnopb09', 'Pnopb10', 'Pnopb11', 'Pnopb12'},
    {'Pnop07', 'Pnop08', 'Pnopb13', 'Pnopb14', 'Pnopb15', 'Pnopb16'},
};
static const u64 lbl_80478C48[24] = {
    'Plibl02', 'Plibl03', 'Plibl04', 'Plw01', 'Plw02', 'Plw03', 'Plw04', 'Plw05', 'Plw06', 'Plw07', 'Plw08', 'Plw09',
    'Plw10', 'Plili02', 'Plili03', 'Plili04', 'Plib01', 'Plib02', 'Plib03', 'Plib04', 'Plib05', 'Plibk01', 'Plibk02',
    'Plibk03',
};
static const u64 lbl_80478D08[18] = {
    'P2pli01', 'P2pli02', 'P2pli03', 'P2pli04', 'P3pli01', 'P3pli02', 'P3pli03', 'P3pli04', 'P3pli05', 'P3pli06',
    'P4pli01', 'P4pli02', 'P4pli03', 'P4pli04', 'P4pli05', 'P4pli06', 'P4pli07', 'P4pli08',
};
static const u64 lbl_80478D98[11] = {
    'Ttkg01', 'Ttkg02', 'Ptkgb01', 'Ptkgbk01', 'Ptkgb02', 'Ptkgbk02', 'Ptkgb03', 'Ptkgb04', 'Ptkgb05', 'Ptkgbk03',
    'Ptkgbk04',
};
static const u64 lbl_80478DF0[6] = {
    'Tten01', 'Tten02', 'Ptgba01', 'Ptgba02', 'Ptn01', 'Ptn02',
};
static const u64 lbl_80478E20[34] = {
    'Pcgps01', 'Pcgp01', 'Pcgba01', 'Pcgbak01', 'Pcgps02', 'Pcgp02', 'Pcgba02', 'Pcgbak02', 'Pcgps03', 'Pcgp03',
    'Pcgba03', 'Pcgbak03', 'Pcgps04', 'Pcgp04', 'Pcgba04', 'Pcgbak04', 'Pcc01', 'Pccaa01', 'Pcc02', 'Pccaa02',
    'Pcc03', 'Pccaa03', 'Pcc04', 'Pccaa04', 'Pcca01', 'Pcca02', 'Pcca03', 'Pcca04', 'Pcca05', 'Pcca06', 'Pcca07',
    'Pcca08', 'Pccu01', 'Pccuk01',
};
static const u64 lbl_80478F30[4][9] = {
    {'Ppln01', 'Pplli01', 'Ppllb01', 'Ppllb02', 'Ppllb03', 'Ppllb04', 'Ppllb05', 'Ppllbk01', 'Ppllbk02'},
    {'Ppln02', 'Pplli02', 'Ppllb11', 'Ppllb12', 'Ppllb13', 'Ppllb14', 'Ppllb15', 'Ppllbk11', 'Ppllbk12'},
    {'Ppln03', 'Pplli03', 'Ppllb21', 'Ppllb22', 'Ppllb23', 'Ppllb24', 'Ppllb25', 'Ppllbk21', 'Ppllbk22'},
    {'Ppln04', 'Pplli04', 'Ppllb31', 'Ppllb32', 'Ppllb33', 'Ppllb34', 'Ppllb35', 'Ppllbk31', 'Ppllbk32'},
};
static const u64 lbl_80479050[20] = {
    'Pmbabb07', 'Pmbabb06', 'Pmbabb05', 'Pmbabb04', 'Pmbabb03', 'Pmbabb02', 'Pmbabb01', 'Pmbab05', 'Pmbab04',
    'Pmbab03', 'Pmbab02', 'Pmbab01', 'Pmbam02', 'Pmbam01', 'Pmbalk02', 'Pmbal02', 'Pmbalk01', 'Pmbal01', 'Pmba02',
    'Pmba01',
};
static const u64 lbl_804790F0[3][5] = {
    {'Tsta01', 'Pstab01', 'Pstab02', 'Pstab03', 'Pstab04'},
    {'Than01', 'Phanb01', 'Phanb02', 'Phanb03', 'Phanb04'},
    {'Trule01', 'Prub01', 'Prub02', 'Prub03', 'Prub04'},
};
static const u64 lbl_80479168[26] = {
    'Pcon01', 'Pcon02', 'Pgba01', 'Pgba02', 'Pnop01', 'Pnop02', 'Plwa01', 'Plwa02', 'Plwa03', 'Plwa04', 'Plwa05',
    'Plwa06', 'Plwa07', 'Plwa08', 'Plwa09', 'Plwa10', 'Plibk01', 'Plibk02', 'Plibk03', 'Plibk04', 'Plibk11',
    'Plibk12', 'Plibk05', 'Plibkk01', 'Plibkk02', 'Plibkk03',
};
static const u64 lbl_80479238[4] = {
    'Pplli01', 'Pplli02', 'Pplli03', 'Pplli04',
};
static const u64 lbl_80479258[4] = {
    'Pcgbak01', 'Pcgbak02', 'Pcgbak03', 'Pcgbak04',
};
static const u64 lbl_80479278[4] = {
    'Pcgba01', 'Pcgba02', 'Pcgba03', 'Pcgba04',
};
static const u64 lbl_80479298[4] = {
    'Pcgp01', 'Pcgp02', 'Pcgp03', 'Pcgp04',
};
static const u64 lbl_804792B8[4] = {
    'Pcgps01', 'Pcgps02', 'Pcgps03', 'Pcgps04',
};
static const u64 lbl_804792D8[4] = {
    'Ppln01', 'Ppln02', 'Ppln03', 'Ppln04',
};
static const u64 lbl_804792F8[4][4] = {
    {'Pcc01', 'Pccaa01', 'Pcca01', 'Pcca02'},
    {'Pcc02', 'Pccaa02', 'Pcca03', 'Pcca04'},
    {'Pcc03', 'Pccaa03', 'Pcca05', 'Pcca06'},
    {'Pcc04', 'Pccaa04', 'Pcca07', 'Pcca08'},
};
static const u64 lbl_80479378[4] = {
    'Par01', 'Par02', 'Par03', 'Par04',
};
static const u64 lbl_80479398[4] = {
    'Ndatay01', 'Ndatay02', 'Ndatay03', 'Ndatay04',
};
static const u64 lbl_804793B8[4] = {
    'Pbcur01', 'Pbcur02', 'Pbcur03', 'Pbcur04',
};
static const u64 lbl_804793D8[4] = {
    'Pscur01', 'Pscur02', 'Pscur03', 'Pscur04',
};
static const u64 lbl_804793F8[4] = {
    'Paccur02', 'Paccur04', 'Paccur06', 'Paccur08',
};
static const u64 lbl_80479418[4] = {
    'Paccur01', 'Paccur03', 'Paccur05', 'Paccur07',
};
static const u64 lbl_80479438[4] = {
    'P1s03', 'P2s03', 'P3s03', 'P4s03',
};
static const u64 lbl_80479458[4] = {
    'P1sk03', 'P2sk03', 'P3sk03', 'P4sk03',
};
static const u64 lbl_80479478[4] = {
    'P1s02', 'P2s02', 'P3s02', 'P4s02',
};
static const u64 lbl_80479498[4] = {
    'P1sk02', 'P2sk02', 'P3sk02', 'P4sk02',
};
static const u64 lbl_804794B8[4] = {
    'P1s01', 'P2s01', 'P3s01', 'P4s01',
};
static const u64 lbl_804794D8[4] = {
    'P1sk01', 'P2sk01', 'P3sk01', 'P4sk01',
};
// The rupee digit textures (the strings are emitted here, between the tag
// tables, as in game/tu_803E1668.cpp).
static const char* lbl_804B0EE8[10] = {
    "kk_rupyNum_00.bti", "kk_rupyNum_01.bti", "kk_rupyNum_02.bti", "kk_rupyNum_03.bti",
    "kk_rupyNum_04.bti", "kk_rupyNum_05.bti", "kk_rupyNum_06.bti", "kk_rupyNum_07.bti",
    "kk_rupyNum_08.bti", "kk_rupyNum_09.bti",
};
static const u64 lbl_804795C0[4][6] = {
    {'Pp1lf01', 'Pp1lf02', 'Pp1lf03', 'Pp1lf04', 'Pp1lf05', 'Pp1lf06'},
    {'Pp2lf01', 'Pp2lf02', 'Pp2lf03', 'Pp2lf04', 'Pp2lf05', 'Pp2lf06'},
    {'Pp3lf01', 'Pp3lf02', 'Pp3lf03', 'Pp3lf04', 'Pp3lf05', 'Pp3lf06'},
    {'Pp4lf01', 'Pp4lf02', 'Pp4lf03', 'Pp4lf04', 'Pp4lf05', 'Pp4lf06'},
};
static const u64 lbl_80479680[4][6] = {
    {'Pp1lfk01', 'Pp1lfk02', 'Pp1lfk03', 'Pp1lfk04', 'Pp1lfk05', 'Pp1lfk06'},
    {'Pp2lfk01', 'Pp2lfk02', 'Pp2lfk03', 'Pp2lfk04', 'Pp2lfk05', 'Pp2lfk06'},
    {'Pp3lfk01', 'Pp3lfk02', 'Pp3lfk03', 'Pp3lfk04', 'Pp3lfk05', 'Pp3lfk06'},
    {'Pp4lfk01', 'Pp4lfk02', 'Pp4lfk03', 'Pp4lfk04', 'Pp4lfk05', 'Pp4lfk06'},
};
static const u64 lbl_80479740[4][6] = {
    {'Pp1lf09', 'Pp1lf10', 'Pp1lf11', 'Pp1lf12', 'Pp1lf13', 'Pp1lf14'},
    {'Pp2lf09', 'Pp2lf10', 'Pp2lf11', 'Pp2lf12', 'Pp2lf13', 'Pp2lf14'},
    {'Pp3lf09', 'Pp3lf10', 'Pp3lf11', 'Pp3lf12', 'Pp3lf13', 'Pp3lf14'},
    {'Pp4lf09', 'Pp4lf10', 'Pp4lf11', 'Pp4lf12', 'Pp4lf13', 'Pp4lf14'},
};
static const u64 lbl_80479800[4][6] = {
    {'Pp1lfk09', 'Pp1lfk10', 'Pp1lfk11', 'Pp1lfk12', 'Pp1lfk13', 'Pp1lfk14'},
    {'Pp2lfk09', 'Pp2lfk10', 'Pp2lfk11', 'Pp2lfk12', 'Pp2lfk13', 'Pp2lfk14'},
    {'Pp3lfk09', 'Pp3lfk10', 'Pp3lfk11', 'Pp3lfk12', 'Pp3lfk13', 'Pp3lfk14'},
    {'Pp4lfk09', 'Pp4lfk10', 'Pp4lfk11', 'Pp4lfk12', 'Pp4lfk13', 'Pp4lfk14'},
};
static const u64 lbl_804798C0[3] = {
    'Pnopb08', 'Pnopb12', 'Pnopb16',
};
static const u64 lbl_804798D8[3] = {
    'Plibl02', 'Plibl03', 'Plibl04',
};
static const u64 lbl_804798F0[3] = {
    'Plili02', 'Plili03', 'Plili04',
};
static const u64 lbl_80479908[3] = {
    'Pblk01', 'Pblk02', 'Pblk03',
};
static const u64 lbl_80479920[3][4] = {
    {'P2pli01', 'P2pli02', 0, 0},
    {'P3pli01', 'P3pli02', 'P3pli03', 0},
    {'P4pli01', 'P4pli02', 'P4pli03', 'P4pli04'},
};

// A pane's initial alpha scaled by rate. The constructor fades every pane
// out with rate 0.0f: the original still multiplies (the constants reloaded
// for each loop), as in the twin game/tu_803E1668.cpp.
static inline u8 fadeAlpha(u8 alpha, f32 rate) {
    return alpha * rate;
}

// Sets the 2-bit field `player` of root->field_0x83 to `kind`.
static inline void setPlayerKind(GameRoot_8049EF20* root, s32 player, u8 kind) {
    root->field_0x83 &= ~(u8)(3 << (2 * player));
    root->field_0x83 |= (u8)(kind << (2 * player));
}

// Sets bit `i` of gGameRoot_8053AAF8->mPlayerMask and the 2-bit field `i` of
// field_0x83 to 1 for the field_0x1ac + 2 players that take part, and clears
// both for the others.
static inline void setEntries(ConnectScreen_804B0F10* self) {
    for (s32 i = 0; i < 4; i++) {
        if (i < self->field_0x1ac + 2) {
            gGameRoot_8053AAF8->mPlayerMask |= (u8)(1 << i);
            setPlayerKind(gGameRoot_8053AAF8, i, 1);
        } else {
            gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << i);
            setPlayerKind(gGameRoot_8053AAF8, i, 0);
        }
    }
}

// The menu cursor's blink counters, stepped at the start of a menu state.
static inline void stepBlink(ConnectScreen_804B0F10* self) {
    if (self->field_0x240[0] != 0) {
        self->field_0x240[0]++;
        if (self->field_0x240[0] > 10) {
            self->field_0x240[0] = 0;
        }
    }
    self->field_0x210[0]++;
    if (self->field_0x210[0] >= 30) {
        self->field_0x210[0] = 0;
    }
}

// The cursor moved: restart its blink and play sound 0x26.
static inline void cursorMoved(ConnectScreen_804B0F10* self) {
    self->field_0x240[0] = 1;
    self->field_0x1d0[0] = 30;
    gAudioMgr_8053AF80->fn_80458880(0x26, -1);
}

static inline void stepCursorTimer(ConnectScreen_804B0F10* self) {
    self->field_0x1d0[0]++;
    if (self->field_0x1d0[0] >= 60) {
        self->field_0x1d0[0] = 0;
    }
}

// The centre of a pane of field_0xc. Macros: the original evaluates the tag
// expression (table index, field_0x1ac and all) again for each search, and
// reads the size before the second search.
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

// FIXME: 98.24 %. Same length. The rodata base offsets differ (.rodata not
// split yet: the tables above sit at other offsets from the file's base), and
// in the 112-212 and 212+ phases the i / 2 + (i % 2) * 2 index is scheduled
// differently (the original computes i % 2 first, then i / 2). The twin
// cls_804B0B00::fn_803E1668 (rodata split) shows the same index scheduling.
void ConnectScreen_804B0F10::fn_803F0A54() {
    f32 ratio = (f32)field_0xc->search(lbl_80479168[0])->getAlpha() / (f32)field_0x335[0];
    JGeometry::TVec3f pos;
    if (field_0x210[0] < 100) {
        for (s32 i = 0; i < 4; i++) {
            if (lbl_80479920[field_0x1ac][i] != 0) {
                SpriteParams_801F5AE0 params;
                params.field_0x26 = 0;
                params.field_0x10.a = 255.0f * ratio;
                f32 x;
                PANE_CENTRE_X(x, lbl_80479920[field_0x1ac][i]);
                pos.x = toSpriteX(x);
                f32 y;
                PANE_CENTRE_Y(y, lbl_80479920[field_0x1ac][i]);
                pos.y = toSpriteY(y);
                pos.z = 0.0f;
                params.field_0x0[7] = i + 12;
                s32 frame = lbl_80478AA0[field_0x210[0] % 30] + 0x10;
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
            }
        }
    } else if (field_0x210[0] < 112) {
        // Slide from the 2/3-player panes to the 4-player ones.
        for (s32 i = 3; i >= 0; i--) {
            SpriteParams_801F5AE0 params;
            params.field_0x26 = 0;
            params.field_0x10.a = 255.0f * ratio;
            f32 from;
            f32 to;
            switch (field_0x1ac) {
            case 0:
                PANE_CENTRE_X(from, lbl_80479920[field_0x1ac][i % 2]);
                PANE_CENTRE_X(to, lbl_80479920[2][i / 2 + (i % 2) * 2]);
                break;
            case 1:
                if (i != 3) {
                    PANE_CENTRE_X(from, lbl_80479920[field_0x1ac][i]);
                    PANE_CENTRE_X(to, lbl_80479920[2][i]);
                } else {
                    PANE_CENTRE_X(from, lbl_80479920[field_0x1ac][2]);
                    PANE_CENTRE_X(to, lbl_80479920[2][i]);
                }
                break;
            }
            pos.x = toSpriteX((to - from) * ((f32)(field_0x210[0] - 100) / 12.0f) + from);
            f32 y;
            PANE_CENTRE_Y(y, lbl_80479920[2][i]);
                pos.y = toSpriteY(y);
            pos.z = 0.0f;
            params.field_0x0[7] = i + 12;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x10, &params, 1.0f, 1.0f);
        }
    } else if (field_0x210[0] < 212) {
        for (s32 i = 0; i < 4; i++) {
            SpriteParams_801F5AE0 params;
            params.field_0x26 = 0;
            params.field_0x10.a = 255.0f * ratio;
            f32 x;
            PANE_CENTRE_X(x, lbl_80479920[2][i]);
                pos.x = toSpriteX(x);
            f32 y;
            PANE_CENTRE_Y(y, lbl_80479920[2][i]);
                pos.y = toSpriteY(y);
            pos.z = 0.0f;
            if (field_0x1ac == 0) {
                params.field_0x0[7] = i / 2 + (i % 2) * 2 + 12;
            } else {
                params.field_0x0[7] = i + 12;
            }
            s32 frame = lbl_80478AA0[field_0x210[0] % 30] + 0x10;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
        }
    } else {
        // Slide back to the 2/3-player panes.
        for (s32 i = 3; i >= 0; i--) {
            SpriteParams_801F5AE0 params;
            params.field_0x26 = 0;
            params.field_0x10.a = 255.0f * ratio;
            f32 from;
            f32 to;
            switch (field_0x1ac) {
            case 0:
                PANE_CENTRE_X(from, lbl_80479920[2][i / 2 + (i % 2) * 2]);
                PANE_CENTRE_X(to, lbl_80479920[field_0x1ac][i % 2]);
                break;
            case 1:
                if (i != 3) {
                    PANE_CENTRE_X(from, lbl_80479920[2][i]);
                    PANE_CENTRE_X(to, lbl_80479920[field_0x1ac][i]);
                } else {
                    PANE_CENTRE_X(from, lbl_80479920[2][i]);
                    PANE_CENTRE_X(to, lbl_80479920[field_0x1ac][2]);
                }
                break;
            }
            pos.x = toSpriteX((to - from) * ((f32)(field_0x210[0] - 212) / 12.0f) + from);
            f32 y;
            PANE_CENTRE_Y(y, lbl_80479920[2][i]);
                pos.y = toSpriteY(y);
            pos.z = 0.0f;
            params.field_0x0[7] = i + 12;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x10, &params, 1.0f, 1.0f);
        }
    }
}

void ConnectScreen_804B0F10::fn_803F1C18() {
    field_0x58->fn_8043670C(1);
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 12;
}

void ConnectScreen_804B0F10::fn_803F1C5C() {
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 11;
}

// FIXME: 98.18. Registers: the loop's table pointers and counter sit in other
// registers, and the first digit texture is copied through r0 into r17 (one
// extra mr) where the original keeps it in r27 (as in the twin
// cls_804B0B00::fn_803E2A20, 98.17); .sdata2 @NNN names.
// Enter state 10 (the results / entry screen field_0x10): per player, the
// 'Pblk' cover over the players beyond field_0x1ac + 2, the rupee count
// (fn_80403070) as three digit textures, the fn_8040305C - 1 markers of the
// four marker tables, the cursor panes by field_0x1b0[i] (3: joined,
// 4: not); then the panes start their slide-in (twin of
// cls_804B0B00::fn_803E2A20).
void ConnectScreen_804B0F10::fn_803F1C70() {
    for (s32 i = 0; i < 4; i++) {
        if (i >= 1) {
            if (i > field_0x1ac + 1) {
                field_0x10->search(lbl_80479908[i - 1])->show();
            } else {
                field_0x10->search(lbl_80479908[i - 1])->hide();
            }
        }
        field_0x1e0[i] = fn_80403414(i);
        field_0x1f0[i] = fn_804033D4(i);
        field_0x200[i] = fn_8040336C(i);
        s32 rupees = fn_80403070(field_0x1e0[i]);
        const ResTIMG* tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0EE8[rupees / 100]);
        static_cast<J2DPicture*>(field_0x10->search(lbl_804794B8[i]))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x10->search(lbl_804794D8[i]))->changeTexture(tex, 0);
        tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0EE8[rupees % 100 / 10]);
        static_cast<J2DPicture*>(field_0x10->search(lbl_80479478[i]))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x10->search(lbl_80479498[i]))->changeTexture(tex, 0);
        tex = (const ResTIMG*)fn_804030C4('TIMG', lbl_804B0EE8[rupees % 10]);
        static_cast<J2DPicture*>(field_0x10->search(lbl_80479438[i]))->changeTexture(tex, 0);
        static_cast<J2DPicture*>(field_0x10->search(lbl_80479458[i]))->changeTexture(tex, 0);
        s32 count = fn_8040305C(field_0x1f0[i]) - 1;
        for (s32 j = 0; j < 6; j++) {
            if (j < count) {
                field_0x10->search(lbl_804795C0[i][j])->show();
                field_0x10->search(lbl_80479680[i][j])->show();
                field_0x10->search(lbl_80479740[i][j])->hide();
                field_0x10->search(lbl_80479800[i][j])->hide();
            } else if (j == count) {
                field_0x10->search(lbl_804795C0[i][j])->hide();
                field_0x10->search(lbl_80479680[i][j])->hide();
                field_0x10->search(lbl_80479740[i][j])->hide();
                field_0x10->search(lbl_80479800[i][j])->show();
            } else {
                field_0x10->search(lbl_804795C0[i][j])->hide();
                field_0x10->search(lbl_80479680[i][j])->hide();
                field_0x10->search(lbl_80479740[i][j])->hide();
                field_0x10->search(lbl_80479800[i][j])->hide();
            }
        }
        field_0x10->search(lbl_804793F8[i])->hide();
    }
    field_0x60->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x60->field_0x22 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i <= field_0x1ac + 1) {
            field_0x64[i]->moveFromCentre_80402E08(0.0f, 150.0f);
            field_0x64[i]->field_0x22 = 0;
        }
        if (i <= field_0x1ac + 1) {
            field_0x1b0[i] = 3;
        } else {
            field_0x1b0[i] = 4;
        }
        field_0x1d0[i] = 30;
        if (i <= field_0x1ac + 1) {
            if (field_0x1b0[i] < 3) {
                field_0x10->search(lbl_804793B8[i])->hide();
                field_0x10->search(lbl_804793D8[i])->show();
                field_0x10->search(lbl_804793F8[i])->hide();
            } else if (field_0x1b0[i] == 3) {
                field_0x10->search(lbl_804793B8[i])->hide();
                field_0x10->search(lbl_804793D8[i])->show();
                field_0x10->search(lbl_804793F8[i])->show();
            } else {
                field_0x10->search(lbl_804793B8[i])->show();
                field_0x10->search(lbl_804793D8[i])->hide();
                field_0x10->search(lbl_804793F8[i])->hide();
            }
        } else {
            field_0x10->search(lbl_804793B8[i])->hide();
            field_0x10->search(lbl_804793D8[i])->hide();
            field_0x10->search(lbl_804793F8[i])->hide();
        }
        s32 state = field_0x1b0[i];
        if (state <= 3) {
            f32 x = field_0x10->search(lbl_804793D8[i])->getBounds()->i.x;
            field_0x10->search(lbl_804793D8[i])->move(x, lbl_80543710[state]);
        }
        field_0x240[i] = 0;
        field_0x1d0[i] = 0;
    }
    field_0x74->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x74->field_0x22 = 0;
    field_0x24c = 0;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 12;
}

// FIXME: 98.04 %. Register swap in setEntries' loop: the original keeps the
// 2 * i shift in r4 (the zero stored to field_0x240) and i in a fresh r5 (li);
// ours keeps i in r4 and copies it (mr r5, r4) for the shift. fn_803F8050
// inlines this function and matches.
void ConnectScreen_804B0F10::fn_803F2580() {
    field_0x240[0] = 0;
    field_0x1d0[0] = 5;
    if (field_0x1a8 == 0) {
        setEntries(this);
    }
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 9;
}

void ConnectScreen_804B0F10::fn_803F2660() {
    field_0x240[0] = 0;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 8;
}

void ConnectScreen_804B0F10::fn_803F2AEC() {
    lbl_8053AEC0->fn_80431A9C(0xC4, field_0x1ac + 2, NULL);
    field_0x25c = 0;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 6;
}

void ConnectScreen_804B0F10::fn_803F2B48() {
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 5;
}

void ConnectScreen_804B0F10::fn_803F2B5C() {
    field_0x240[0] = 0;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 4;
}

void ConnectScreen_804B0F10::fn_803F32E8() {
    field_0x210[0] = 0;
    field_0x240[0] = 0;
    field_0x1d0[0] = 5;
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 2;
}

void ConnectScreen_804B0F10::fn_803F3310() {
    field_0x1c8 = field_0x1c4;
    field_0x1c4 = 1;
}

// FIXME: 99.77 %. Instructions identical; only the .sdata2 literals differ (see
// fn_803F715C). fn_803FC5E4 inlines this function.
void ConnectScreen_804B0F10::fn_803F3324() {
    field_0x110 = -150.0f;
    mScreen->search(0x506D626162623037ull /* "Pmbabb07" */)->move(field_0x108, field_0x10c + field_0x110);
    field_0x114 = -100.0f;
    field_0x74->moveFromCentre_80402E08(0.0f, field_0x114);
    field_0x130[0] = -350.0f;
    mScreen->search(0x00506E6F70623038ull /* "Pnopb08" */)->move(field_0x118[0] + field_0x130[0], field_0x124[0]);
    field_0x130[1] = -350.0f;
    mScreen->search(0x00506E6F70623132ull /* "Pnopb12" */)->move(field_0x118[1] + field_0x130[1], field_0x124[1]);
    field_0x130[2] = -350.0f;
    mScreen->search(0x00506E6F70623136ull /* "Pnopb16" */)->move(field_0x118[2] + field_0x130[2], field_0x124[2]);
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

void ConnectScreen_804B0F10::fn_803F358C() {
    field_0x58->fn_80436668();
}

// FIXME: 99.26 (twin cls_804B0B00::fn_803E4650): stack slots only, the
// blink's else-branch int conversion sits at 0x10 instead of 0x8.
// Draws states 10 and 11 (the results screen field_0x10): each taking
// player's cursor 'Pscur' fades with field_0x1d0[i] (60 frames), the
// joined ones' 'Paccur' panes with it; outside state 10 the 'Paccur02'
// pane and field_0x64[i] bounce with field_0x240[i]. Then lbl_8053AED8's
// buttons.
void ConnectScreen_804B0F10::fn_803F35B0() {
    for (s32 i = 0; i < 4; i++) {
        f32 t;
        if (field_0x1d0[i] < 30) {
            t = (f32)(field_0x1d0[i] * field_0x1d0[i]) / 900.0f;
        } else {
            t = 1.0f - (f32)((field_0x1d0[i] - 30) * (field_0x1d0[i] - 30)) / 900.0f;
        }
        if (i <= field_0x1ac + 1) {
            if (field_0x1b0[i] < 4) {
                field_0x10->search(lbl_804793D8[i])->setAlpha(160.0f * t);
            }
            if (field_0x1b0[i] == 3) {
                field_0x10->search(lbl_804793F8[i])->setAlpha(t * field_0x25a);
                field_0x10->search(lbl_80479418[i])->setAlpha(t * field_0x25a);
            }
        }
        if (field_0x1c4 != 10) {
            s32 bounce = field_0x240[i] < 5 ? field_0x240[i] : 10 - field_0x240[i];
            f32 offset = 5.0f * (f32)(bounce * bounce) / 25.0f;
            f32 x = field_0x10->search(lbl_804793F8[i])->getBounds()->i.x;
            field_0x10->search(lbl_804793F8[i])->move(x, field_0x1a0 + offset);
            if (field_0x1b0[i] >= 3) {
                field_0x64[i]->moveFromCentre_80402E08(0.0f, offset);
            }
        }
    }
    field_0x10->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
}

// FIXME: 99.60. Stack layout and float registers only: the original keeps
// every int-to-float conversion in one 8-byte slot (0x18; frame 0x60), ours
// uses two (frame 0x70), and the bounce offset / pane x swap f29 and f30.
// Draws states 7-9 (the game-select menu, field_0xc): the cursor
// 'Pscuro01' / 'Pscuro02' fades in and out (field_0x1d0[0] over 60 frames;
// in state 9 it fades to 230 over 30, or is hidden while field_0x1cc is 0)
// and the chosen entry ('Pstab04' / 'Phanb04' / 'Prub04', field_0x1a8)
// bounces with field_0x240[0]; then the screen, the player sprites
// (fn_803F0A54) and lbl_8053AED8's buttons.
void ConnectScreen_804B0F10::fn_803F38A4() {
    f32 alpha;
    if (field_0x1c4 != 9) {
        f32 t;
        if (field_0x1d0[0] < 30) {
            t = (f32)(field_0x1d0[0] * field_0x1d0[0]) / 900.0f;
        } else {
            t = 1.0f - (f32)((field_0x1d0[0] - 30) * (field_0x1d0[0] - 30)) / 900.0f;
        }
        alpha = t * field_0x259;
    } else if (field_0x1cc == 0) {
        alpha = 0.0f;
    } else {
        f32 t;
        if (field_0x1d0[0] < 15) {
            t = (f32)(field_0x1d0[0] * field_0x1d0[0]) / 225.0f;
        } else {
            t = 1.0f - (f32)((field_0x1d0[0] - 15) * (field_0x1d0[0] - 15)) / 225.0f;
        }
        alpha = 230.0f * t;
    }
    s32 bounce;
    if (field_0x1c4 == 8 || (field_0x1c4 == 9 && field_0x1cc != 0 && field_0x240[0] <= 10)) {
        if (field_0x240[0] < 5) {
            bounce = field_0x240[0];
        } else {
            bounce = 10 - field_0x240[0];
        }
    } else {
        bounce = 0;
    }
    f32 offset = 5.0f * (f32)(bounce * bounce) / 25.0f;
    if (field_0x1a8 == 0) {
        f32 x = field_0xc->search('Pstab04')->getBounds()->i.x;
        field_0xc->search('Pstab04')->move(x, field_0xfc[0] + offset);
        x = field_0xc->search('Phanb04')->getBounds()->i.x;
        field_0xc->search('Phanb04')->move(x, field_0xfc[1]);
        x = field_0xc->search('Prub04')->getBounds()->i.x;
        field_0xc->search('Prub04')->move(x, field_0xfc[2]);
        x = field_0xc->search('Pscuro02')->getBounds()->i.x;
        field_0xc->search('Pscuro02')->move(x, field_0xf8 + offset);
    } else if (field_0x1a8 == 1) {
        f32 x = field_0xc->search('Pstab04')->getBounds()->i.x;
        field_0xc->search('Pstab04')->move(x, field_0xfc[0]);
        x = field_0xc->search('Phanb04')->getBounds()->i.x;
        field_0xc->search('Phanb04')->move(x, field_0xfc[1] + offset);
        x = field_0xc->search('Prub04')->getBounds()->i.x;
        field_0xc->search('Prub04')->move(x, field_0xfc[2]);
        f32 top = field_0xc->search('Pstab04')->getBounds()->i.y;
        f32 y = field_0xf8 + field_0xc->search('Phanb04')->getBounds()->i.y - top;
        x = field_0xc->search('Pscuro02')->getBounds()->i.x;
        field_0xc->search('Pscuro02')->move(x, y);
    } else {
        f32 x = field_0xc->search('Pstab04')->getBounds()->i.x;
        field_0xc->search('Pstab04')->move(x, field_0xfc[0]);
        x = field_0xc->search('Phanb04')->getBounds()->i.x;
        field_0xc->search('Phanb04')->move(x, field_0xfc[1]);
        x = field_0xc->search('Prub04')->getBounds()->i.x;
        field_0xc->search('Prub04')->move(x, field_0xfc[2] + offset);
        f32 top = field_0xc->search('Pstab04')->getBounds()->i.y;
        f32 y = field_0xf8 + field_0xc->search('Prub04')->getBounds()->i.y - top;
        x = field_0xc->search('Pscuro02')->getBounds()->i.x;
        field_0xc->search('Pscuro02')->move(x, y);
    }
    field_0xc->search('Pscuro01')->setAlpha(alpha);
    field_0xc->search('Pscuro02')->setAlpha(alpha);
    field_0xc->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    JGeometry::TBox2f box(0.0f, 0.0f, 280.0f, 210.0f);
    gGameRoot2_8053AB10->fn_8013B414(&box, true);
    fn_803F0A54();
    lbl_8053AED8->fn_80440EE8();
}

// FIXME: 98.68. Stack slots and registers only: the original converts every
// int / u8 to float in the one 0x8 slot (ours uses 0x8-0x18), and the loop's
// table pointers / the lit alpha sit in other registers.
// Draws states 2-4 (the player-count menu, mScreen): the cursor
// 'Pscuro01' / 'Pscuro02' fades with field_0x1d0[0] (in state 4 it fades to
// 230 over 15 frames, then by (n - 15)^2 / 225), the chosen count's pane
// (field_0x1ac) bounces with field_0x240[0] and shows its lit version at
// 80 * the fade; the others show the plain one.
void ConnectScreen_804B0F10::fn_803F5410() {
    f32 alpha;
    f32 t;
    if (field_0x1c4 != 2) {
        if (field_0x1d0[0] < 30) {
            t = (f32)(field_0x1d0[0] * field_0x1d0[0]) / 900.0f;
        } else {
            t = 1.0f - (f32)((field_0x1d0[0] - 30) * (field_0x1d0[0] - 30)) / 900.0f;
        }
        alpha = t * field_0x258;
    } else {
        if (field_0x1d0[0] < 15) {
            t = (f32)(field_0x1d0[0] * field_0x1d0[0]) / 225.0f;
        } else {
            t = 1.0f - (f32)((field_0x1d0[0] - 15) * (field_0x1d0[0] - 15)) / 225.0f;
        }
        alpha = 230.0f * t;
    }
    s32 bounce;
    if (field_0x1c4 == 1 || (field_0x1c4 == 2 && field_0x240[0] <= 10)) {
        if (field_0x240[0] < 5) {
            bounce = field_0x240[0];
        } else {
            bounce = 10 - field_0x240[0];
        }
    } else {
        bounce = 0;
    }
    f32 lit = 80.0f * t;
    f32 offset = 5.0f * (f32)(bounce * bounce) / 25.0f;
    for (s32 i = 0; i < 3; i++) {
        if (i == field_0x1ac) {
            f32 x = mScreen->search(lbl_804798C0[i])->getBounds()->i.x;
            mScreen->search(lbl_804798C0[i])->move(x, field_0x124[i] + offset);
            if (i == 0) {
                x = mScreen->search('Pscuro02')->getBounds()->i.x;
                mScreen->search('Pscuro02')->move(x, field_0xf0 + offset);
            } else {
                f32 top = mScreen->search(lbl_804798C0[0])->getBounds()->i.y;
                f32 y = field_0xf0 + mScreen->search(lbl_804798C0[i])->getBounds()->i.y - top;
                x = mScreen->search('Pscuro02')->getBounds()->i.x;
                mScreen->search('Pscuro02')->move(x, y);
            }
            mScreen->search(lbl_804798D8[i])->hide();
            mScreen->search(lbl_804798F0[i])->show();
            mScreen->search(lbl_804798F0[i])->setAlpha(lit);
        } else {
            f32 x = mScreen->search(lbl_804798C0[i])->getBounds()->i.x;
            mScreen->search(lbl_804798C0[i])->move(x, field_0x124[i]);
            mScreen->search(lbl_804798D8[i])->show();
            mScreen->search(lbl_804798F0[i])->hide();
        }
    }
    mScreen->search('Pscuro01')->setAlpha(alpha);
    mScreen->search('Pscuro02')->setAlpha(alpha);
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
}

void ConnectScreen_804B0F10::fn_803F59A0() {
    if (field_0x58->fn_804366B8() != 0) {
        if (field_0x25e == 0) {
            field_0x1cc = 7;
            field_0x25e = 1;
        }
        field_0x1a8 = 0;
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    }
}

// FIXME: 99.56 %. Instructions identical; only the .sdata2 literals differ (@NNN vs
// lbl_805437xx): the literal pool follows first use, and the unwritten
// functions before this one (fn_803F1C70 on) use the earlier literals.
void ConnectScreen_804B0F10::fn_803F715C() {
    u8 done0 = false;
    u8 done1 = false;
    u8 done2 = false;
    field_0x24c++;
    if (field_0x24c > 0) {
        done0 = field_0x60->fn_80402940(22, 0, -200.0f, 0.0f);
    }
    if (field_0x24c > 16) {
        done1 = field_0x74->fn_80402940(24, 0, -200.0f, 0.0f);
    }
    for (s32 i = 0; i < 4; i++) {
        if (i <= field_0x1ac + 1 && field_0x24c > i * 5 + 4) {
            done2 = field_0x64[i]->fn_80402940(18, 0, 150.0f, 0.0f);
        }
    }
    if (done0 && done1 && done2) {
        field_0x1c8 = field_0x1c4;
        field_0x1c4 = 11;
    }
}

void ConnectScreen_804B0F10::fn_803F8050() {
    stepBlink(this);
    if (field_0x1c0 != field_0x1ac + 2) {
        field_0x25c = 255;
        (this->*lbl_804B0BDC[3])();
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23d + 2)) {
        if (field_0x1a8 < 3) {
            field_0x1cc = 13;
            fn_803F2580();
        }
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23d + 2)) {
        field_0x1cc = 0;
        (this->*lbl_804B0BDC[9])();
        gAudioMgr_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, lbl_80529DEC.field_0x23d + 2)) {
        field_0x1a8++;
        if (field_0x1a8 >= 3) {
            field_0x1a8 = 0;
        }
        cursorMoved(this);
    } else if (lbl_8053AD30->fn_80309A9C(8, lbl_80529DEC.field_0x23d + 2)) {
        field_0x1a8--;
        if (field_0x1a8 < 0) {
            field_0x1a8 = 2;
        }
        cursorMoved(this);
    }
    stepCursorTimer(this);
}

// FIXME: 99.9. Instructions identical; only the .sdata2 literals differ (@NNN vs lbl_80543xxx: the pool
// follows first use and the unwritten functions before this unit's first one use the earlier literals).
// Update state 5 (the GBA connection prompt): field_0x25c 255, or decide
// (0x1100) without every controller connected, enters state 6
// (fn_803F2AEC); decide with all connected reads the players' results and
// enters state 4 (fn_803F2B5C); back (0x200) enters state 0.
void ConnectScreen_804B0F10::fn_803F8AE4() {
    if (field_0x25c == 255) {
        fn_803F2AEC();
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23d + 2)) {
        if (field_0x1c0 == field_0x1ac + 2) {
            field_0x1a8 = 0;
            for (s32 i = 0; i < 4; i++) {
                field_0x1e0[i] = fn_80403414(i);
                field_0x1f0[i] = fn_804033D4(i);
                field_0x200[i] = fn_8040336C(i);
            }
            for (s32 i = 0; i < 4; i++) {
                field_0x1b0[i] = 1;
            }
            fn_803F2B5C();
            gAudioMgr_8053AF80->fn_80458880(11, -1);
        } else {
            fn_803F2AEC();
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23d + 2)) {
        (this->*lbl_804B0BDC[0])();
        gAudioMgr_8053AF80->fn_80458880(12, -1);
    }
    field_0x1d0[0]++;
    if (field_0x1d0[0] >= 60) {
        field_0x1d0[0] = 0;
    }
    field_0x248++;
    if (field_0x248 >= 100) {
        field_0x248 = 0;
    }
    field_0x24a++;
    if (field_0x24a >= 100) {
        field_0x24a = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (i + 4))) {
            field_0x210[i]++;
            if (field_0x210[i] >= 130) {
                field_0x210[i] = 100;
            }
            if (field_0x210[i] == 11) {
                field_0x78[i]->field_0x44 = 0;
                field_0x78[i]->field_0x45 = 0;
                field_0x78[i]->startSound(0x10001);
            } else if (field_0x210[i] == 36) {
                f32 size = field_0x8->search(lbl_80479238[i])->getWidth();
                field_0x88 = toEffectX(size / 2 + field_0x8->search(lbl_80479238[i])->getGlobalBounds().i.x);
                size = field_0x8->search(lbl_80479238[i])->getHeight();
                field_0x8c = toEffectY(size / 2 + field_0x8->search(lbl_80479238[i])->getGlobalBounds().i.y);
                field_0x90 = 0.0f;
                field_0x78[i]->field_0x44 = 0;
                field_0x78[i]->field_0x45 = 0;
                field_0x78[i]->startSound(0x2000E);
                JGeometry::TVec3f pos(field_0x88 + 7.0f, field_0x8c - 27.0f, field_0x90);
                spawnEmitter_8013CC50(0, &pos, 0xCB, 1, 0, NULL, NULL, true, 1.0f);
                JGeometry::TVec3f pos2(field_0x88 + 7.0f, field_0x8c - 27.0f, field_0x90);
                spawnEmitter_8013CC50(0, &pos2, 0xCC, 1, 0, NULL, NULL, true, 1.0f);
            }
        } else {
            field_0x210[i] = 0;
        }
    }
}

// FIXME: 99.9. Instructions identical; only the .sdata2 literals differ (@NNN vs lbl_80543xxx: the pool
// follows first use and the unwritten functions before this unit's first one use the earlier literals).
// Update state 6 (waiting for the GBA link through lbl_8053AEC0;
// field_0x25c: 0 starting, 1 running, 2 ending into state 5, 3 ending with
// field_0x350 = 2): all players connected, decide (0x1100) or back (0x200)
// ends it; then each connected player's pane animation (field_0x210[i],
// 100-129 looping) plays sound 0x10001 at frame 11 and at frame 36 sound
// 0x2000E and effects 0xCB / 0xCC at the 'Pplli' pane.
void ConnectScreen_804B0F10::fn_803F8FBC() {
    switch (field_0x25c) {
    case 0:
        if (lbl_8053AEC0->fn_804317AC()) {
            field_0x25c = 1;
        }
        break;
    case 2:
        if (lbl_8053AEC0->fn_80431740()) {
            field_0x25c = 0;
            field_0x1c8 = field_0x1c4;
            field_0x1c4 = 5;
        }
        break;
    case 3:
        if (lbl_8053AEC0->fn_80431740()) {
            field_0x25c = 0;
            field_0x350 = 2;
        }
        break;
    default:
        if (field_0x1c0 == field_0x1ac + 2) {
            field_0x25c = 2;
            gAudioMgr_8053AF80->fn_80458880(11, -1);
        } else if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23d + 2)) {
            field_0x25c = 2;
            gAudioMgr_8053AF80->fn_80458880(11, -1);
        } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23d + 2)) {
            field_0x25c = 2;
            gAudioMgr_8053AF80->fn_80458880(12, -1);
        }
        break;
    }
    field_0x248++;
    if (field_0x248 >= 100) {
        field_0x248 = 0;
    }
    field_0x24a++;
    if (field_0x24a >= 100) {
        field_0x24a = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << (i + 4))) {
            field_0x210[i]++;
            if (field_0x210[i] >= 130) {
                field_0x210[i] = 100;
            }
            if (field_0x210[i] == 11) {
                field_0x78[i]->field_0x44 = 0;
                field_0x78[i]->field_0x45 = 0;
                field_0x78[i]->startSound(0x10001);
            } else if (field_0x210[i] == 36) {
                f32 size = field_0x8->search(lbl_80479238[i])->getWidth();
                field_0x88 = toEffectX(size / 2 + field_0x8->search(lbl_80479238[i])->getGlobalBounds().i.x);
                size = field_0x8->search(lbl_80479238[i])->getHeight();
                field_0x8c = toEffectY(size / 2 + field_0x8->search(lbl_80479238[i])->getGlobalBounds().i.y);
                field_0x90 = 0.0f;
                field_0x78[i]->field_0x44 = 0;
                field_0x78[i]->field_0x45 = 0;
                field_0x78[i]->startSound(0x2000E);
                JGeometry::TVec3f pos(field_0x88 + 7.0f, field_0x8c - 27.0f, field_0x90);
                spawnEmitter_8013CC50(0, &pos, 0xCB, 1, 0, NULL, NULL, true, 1.0f);
                JGeometry::TVec3f pos2(field_0x88 + 7.0f, field_0x8c - 27.0f, field_0x90);
                spawnEmitter_8013CC50(0, &pos2, 0xCC, 1, 0, NULL, NULL, true, 1.0f);
            }
        } else {
            field_0x210[i] = 0;
        }
    }
}

// FIXME: 99.41 %. Same register swap as fn_803F2580 in the inlined
// setEntries loop (li r4, 0 for i in the original, mr r4, r3 here).
void ConnectScreen_804B0F10::fn_803FB6F4() {
    stepBlink(this);
    if (lbl_8053AD30->fn_80309C4C(0x1100, 1)) {
        field_0x1a8 = 0;
        gGameRoot_8053AAF8->field_0x7e = 1;
        if (field_0x1c0 == field_0x1ac + 2) {
            setEntries(this);
            field_0x1cc = 13;
        }
        fn_803F32E8();
        if (field_0x1ac != lbl_80529DEC.field_0x23d) {
            for (s32 i = 0; i < 4; i++) {
                lbl_80529DEC.field_0x140[i] = 0;
            }
            lbl_80529DEC.field_0x64 = -1;
            lbl_80529DEC.fn_80403680();
        }
        lbl_80529DEC.field_0x23d = field_0x1ac;
        gAudioMgr_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1)) {
        field_0x350 = 2;
        gAudioMgr_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, 1)) {
        field_0x1ac++;
        if (field_0x1ac >= 3) {
            field_0x1ac = 0;
        }
        cursorMoved(this);
    } else if (lbl_8053AD30->fn_80309A9C(8, 1)) {
        field_0x1ac--;
        if (field_0x1ac < 0) {
            field_0x1ac = 2;
        }
        cursorMoved(this);
    }
    stepCursorTimer(this);
}

void ConnectScreen_804B0F10::fn_803FC304() {
    field_0x1c0 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i <= field_0x1ac + 1) {
            if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(i))) {
                gGameRoot_8053AAF8->mPlayerMask |= (u8)(1 << (i + 4));
                field_0x1c0++;
            } else {
                gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << (i + 4));
            }
        } else {
            gGameRoot_8053AAF8->mPlayerMask &= ~(u8)(1 << (i + 4));
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

// FIXME: 99.79 %. Instructions identical; only the .sdata2 literals differ (@NNN vs
// lbl_805437xx): the literal pool follows first use, and the unwritten
// functions before this one (fn_803F1C70 on) use the earlier literals.
void ConnectScreen_804B0F10::fn_803FC46C() {
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x14->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
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
        gGameRoot2_8053AB10->fn_8013B490();
        field_0x5c->setAlpha(field_0x25d);
        field_0x5c->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    }
}

// FIXME: 99.80 %. Instructions identical; only the .sdata2 literals differ (@NNN vs
// lbl_805437xx): the literal pool follows first use, and the unwritten
// functions before this one (fn_803F1C70 on) use the earlier literals.
void ConnectScreen_804B0F10::fn_803FC5E4(u8 flag) {
    lbl_8053AEC0->fn_804310CC();
    if (flag) {
        field_0x1c4 = 7;
        fn_803F267C();
    } else {
        if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(0)) && lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(1))) {
            field_0x1ac = 0;
            for (s32 i = 2; i < 4; i++) {
                if (!lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(i))) {
                    break;
                }
                field_0x1ac++;
            }
        }
        field_0x1c4 = 0;
        fn_803F3324();
    }
    field_0x350 = 4;
}

ConnectScreen_804B0F10::~ConnectScreen_804B0F10() {
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

// FIXME: 99.23. (1) The string literals sit 0x3C bytes lower in our pool:
// the original's has "kk_nPlayerP2_01.bti".."P4" (3 x 0x14 bytes, used by
// the unwritten fn_803F267C) before "bat_n_player.blo", so the addi offsets
// off the .rodata base differ. (2) Registers only in most loops (e.g. the
// first loop's array pointers are r24 / r27 in the original, r27 / r28 here).
// Builds the seven screens and their panes' start state (structure as the
// twin cls_804B0B00::cls_804B0B00, game/tu_803E1668.cpp).
ConnectScreen_804B0F10::ConnectScreen_804B0F10() {
    field_0x1a4 = 0;
    field_0x1a8 = 0;
    field_0x1ac = lbl_80529DEC.field_0x23d;
    field_0x1c0 = 0;
    field_0x252 = 0;
    field_0x25b = 0;
    field_0x25c = 0;
    field_0x1c8 = 0;
    field_0x1c4 = 0;
    field_0x1cc = 0;
    for (int i = 0; i < 4; i++) {
        field_0x1d0[i] = 0;
        field_0x1e0[i] = fn_80403414(i);
        field_0x1f0[i] = fn_804033D4(i);
        field_0x200[i] = fn_8040336C(i);
        field_0x210[i] = 0;
        field_0x1b0[i] = 1;
        field_0x240[i] = 0;
    }
    field_0x248 = 0;
    field_0x24a = 0;
    field_0x25e = 0;
    field_0x25d = 0;
    field_0x24c = 0;
    field_0x350 = 4;

    mScreen = new J2DScreen;
    mScreen->setPriority("bat_n_player.blo", 0x1100000, lbl_80529DEC.field_0x28);
    field_0x8 = new J2DScreen;
    field_0x8->setPriority("bat_entry.blo", 0x1100000, lbl_80529DEC.field_0x28);
    field_0xc = new J2DScreen;
    field_0xc->setPriority("bat_entry_con.blo", 0x1100000, lbl_80529DEC.field_0x28);
    field_0x10 = new J2DScreen;
    field_0x10->setPriority("handicap.blo", 0x1100000, lbl_80529DEC.field_0x28);
    field_0x14 = new J2DScreen;
    field_0x14->setPriority("bat_back.blo", 0x1100000, lbl_80529DEC.field_0x28);
    for (int i = 0; i < 4; i++) {
        field_0x18[i] = new J2DScreen;
        field_0x18[i]->setPriority("entry_arrow_anim.blo", 0x1100000, lbl_80529DEC.field_0x28);
        field_0x28[i] = new J2DScreen;
        field_0x28[i]->setPriority("entry_connect_anim.blo", 0x1100000, lbl_80529DEC.field_0x28);
        static_cast<J2DPicture*>(field_0x28[i]->search('Pccaa00'))
            ->setBlackWhite(JUtility::TColor(0x82, 0x82, 0xA0, 0xFF), JUtility::TColor(0x82, 0x82, 0xA0, 0x00));
    }
    OSInitFastCast();
    void* arrowAnm = fn_804031C4('SCRN', "entry_arrow_anim.bck");
    void* connectAnm = fn_804031C4('SCRN', "entry_connect_anim.bck");
    for (int i = 0; i < 4; i++) {
        field_0x38[i] = static_cast<J2DAnmTransform*>(J2DAnmLoaderDataBase::load(arrowAnm));
        field_0x48[i] = static_cast<J2DAnmTransform*>(J2DAnmLoaderDataBase::load(connectAnm));
        field_0x18[i]->search('NULL_001')->setAnimation(field_0x38[i]);
        field_0x28[i]->search('Ndatay00')->setAnimation(field_0x48[i]);
        field_0x220[i] = 0;
        field_0x230[i] = 0;
        field_0x8->search(lbl_80479378[i])->hide();
    }
    field_0x24e = -1;
    field_0x250 = -1;

    // mScreen: the initial alphas, then every pane faded out.
    for (int i = 0; i < 20; i++) {
        field_0x25f[i] = mScreen->search(lbl_80478B18[i])->getAlpha();
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            field_0x273[i][j] = mScreen->search(lbl_80478BB8[i][j])->getAlpha();
        }
    }
    for (int i = 0; i < 24; i++) {
        field_0x285[i] = mScreen->search(lbl_80478C48[i])->getAlpha();
    }
    for (int i = 0; i < 18; i++) {
        field_0x29d[i] = mScreen->search(lbl_80478D08[i])->getAlpha();
    }
    for (int i = 0; i < 20; i++) {
        mScreen->search(lbl_80478B18[i])->setAlpha(fadeAlpha(field_0x25f[i], 0.0f));
    }
    // Four rows over the three-row table (the twin's 4 x 6 loop): the
    // original reads row 3 from the next table and field_0x285.
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            mScreen->search(lbl_80478BB8[i][j])->setAlpha(fadeAlpha(field_0x273[i][j], 0.0f));
        }
    }
    for (int i = 0; i < 24; i++) {
        mScreen->search(lbl_80478C48[i])->setAlpha(fadeAlpha(field_0x285[i], 0.0f));
    }
    for (int i = 0; i < 18; i++) {
        mScreen->search(lbl_80478D08[i])->setAlpha(fadeAlpha(field_0x29d[i], 0.0f));
    }
    field_0x258 = 156;
    field_0xec = mScreen->search('Pscuro02')->getBounds()->i.x;
    field_0xf0 = mScreen->search('Pscuro02')->getBounds()->i.y;
    field_0x108 = mScreen->search('Pmbabb07')->getBounds()->i.x;
    field_0x10c = mScreen->search('Pmbabb07')->getBounds()->i.y;
    field_0x110 = 0.0f;
    field_0x114 = 0.0f;
    field_0x118[0] = mScreen->search('Pnopb08')->getBounds()->i.x;
    field_0x124[0] = mScreen->search('Pnopb08')->getBounds()->i.y;
    field_0x130[0] = 0.0f;
    field_0x118[1] = mScreen->search('Pnopb12')->getBounds()->i.x;
    field_0x124[1] = mScreen->search('Pnopb12')->getBounds()->i.y;
    field_0x130[1] = 0.0f;
    field_0x118[2] = mScreen->search('Pnopb16')->getBounds()->i.x;
    field_0x124[2] = mScreen->search('Pnopb16')->getBounds()->i.y;
    field_0x130[2] = 0.0f;
    field_0x13c = mScreen->search('Ndata02')->getBounds()->i.x;
    field_0x140 = mScreen->search('Ndata02')->getBounds()->i.y;
    field_0x144 = 0.0f;
    field_0x148 = mScreen->search('Ndata05')->getBounds()->i.x;
    field_0x14c = mScreen->search('Ndata05')->getBounds()->i.y;
    field_0x150 = mScreen->search('Ndata07')->getBounds()->i.x;
    field_0x154 = mScreen->search('Ndata07')->getBounds()->i.y;
    field_0x158 = 0.0f;

    // field_0x8 (the entry screen).
    for (int i = 0; i < 4; i++) {
        field_0x8->search(lbl_80479238[i])->hide();
        for (int j = 0; j < 2; j++) {
            field_0x8->search(lbl_804792F8[i][j])->hide();
        }
    }
    for (int i = 0; i < 11; i++) {
        field_0x2af[i] = field_0x8->search(lbl_80478D98[i])->getAlpha();
    }
    for (int i = 0; i < 6; i++) {
        field_0x2ba[i] = field_0x8->search(lbl_80478DF0[i])->getAlpha();
    }
    for (int i = 0; i < 34; i++) {
        field_0x2c4[i] = field_0x8->search(lbl_80478E20[i])->getAlpha();
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 9; j++) {
            field_0x2ee[i][j] = field_0x8->search(lbl_80478F30[i][j])->getAlpha();
        }
    }
    field_0x253 = field_0x8->search(lbl_80479258[0])->getAlpha();
    field_0x254 = field_0x8->search(lbl_80479278[0])->getAlpha();
    field_0x255 = field_0x8->search(lbl_80479298[0])->getAlpha();
    for (int i = 0; i < 11; i++) {
        field_0x8->search(lbl_80478D98[i])->setAlpha(fadeAlpha(field_0x2af[i], 0.0f));
    }
    for (int i = 0; i < 6; i++) {
        field_0x8->search(lbl_80478DF0[i])->setAlpha(fadeAlpha(field_0x2ba[i], 0.0f));
    }
    for (int i = 0; i < 34; i++) {
        field_0x8->search(lbl_80478E20[i])->setAlpha(fadeAlpha(field_0x2c4[i], 0.0f));
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 9; j++) {
            field_0x8->search(lbl_80478F30[i][j])->setAlpha(fadeAlpha(field_0x2ee[i][j], 0.0f));
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
    field_0x94 = field_0x8->search('Ndata03')->getBounds()->i.x;
    field_0x98 = field_0x8->search('Ndata03')->getBounds()->i.y;
    field_0x9c = 0.0f;
    field_0xa0 = field_0x8->search('Ndata05')->getBounds()->i.x;
    field_0xa4 = field_0x8->search('Ndata05')->getBounds()->i.y;
    field_0xa8 = 0.0f;
    field_0xac = field_0x8->search('Ndata06')->getBounds()->i.x;
    field_0xb0 = field_0x8->search('Ndata06')->getBounds()->i.y;
    field_0xb4 = 0.0f;
    field_0xb8 = 0.0f;
    field_0xbc[0] = field_0x8->search('Ppllbk02')->getBounds()->i.x;
    field_0xcc[0] = field_0x8->search('Ppllbk02')->getBounds()->i.y;
    field_0xdc[0] = 0.0f;
    field_0xbc[1] = field_0x8->search('Ppllbk12')->getBounds()->i.x;
    field_0xcc[1] = field_0x8->search('Ppllbk12')->getBounds()->i.y;
    field_0xdc[1] = 0.0f;
    field_0xbc[2] = field_0x8->search('Ppllbk22')->getBounds()->i.x;
    field_0xcc[2] = field_0x8->search('Ppllbk22')->getBounds()->i.y;
    field_0xdc[2] = 0.0f;
    field_0xbc[3] = field_0x8->search('Ppllbk32')->getBounds()->i.x;
    field_0xcc[3] = field_0x8->search('Ppllbk32')->getBounds()->i.y;
    field_0xdc[3] = 0.0f;

    // field_0xc (the connect screen).
    for (int i = 0; i < 20; i++) {
        field_0x312[i] = field_0xc->search(lbl_80479050[i])->getAlpha();
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            field_0x326[i][j] = field_0xc->search(lbl_804790F0[i][j])->getAlpha();
        }
    }
    for (int i = 0; i < 26; i++) {
        field_0x335[i] = field_0xc->search(lbl_80479168[i])->getAlpha();
    }
    for (int i = 0; i < 20; i++) {
        field_0xc->search(lbl_80479050[i])->setAlpha(fadeAlpha(field_0x312[i], 0.0f));
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            field_0xc->search(lbl_804790F0[i][j])->setAlpha(fadeAlpha(field_0x326[i][j], 0.0f));
        }
    }
    for (int i = 0; i < 26; i++) {
        field_0xc->search(lbl_80479168[i])->setAlpha(fadeAlpha(field_0x335[i], 0.0f));
    }
    field_0x15c = field_0xc->search('Pmbabb07')->getBounds()->i.x;
    field_0x160 = field_0xc->search('Pmbabb07')->getBounds()->i.y;
    field_0x164 = 0.0f;
    field_0x168 = 0.0f;
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
    field_0x259 = 156;
    field_0xf4 = field_0xc->search('Pscuro02')->getBounds()->i.x;
    field_0xf8 = field_0xc->search('Pscuro02')->getBounds()->i.y;
    field_0xfc[0] = field_0xc->search('Pstab04')->getBounds()->i.y;
    field_0xfc[1] = field_0xc->search('Phanb04')->getBounds()->i.y;
    field_0xfc[2] = field_0xc->search('Prub04')->getBounds()->i.y;
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
    field_0xc->search('Pgba02')->hide();
    field_0xc->search('Pcon02')->show();

    // field_0x10 (the handicap screen).
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
    field_0x25a = 156;
    field_0x19c = field_0x10->search('Paccur02')->getBounds()->i.x;
    field_0x1a0 = field_0x10->search('Paccur02')->getBounds()->i.y;
    field_0x60 = new PaneCtrl_804AF86C;
    field_0x60->setPane_80402F04(field_0x10->search('Pthabb07'));
    field_0x64[0] = new PaneCtrl_804AF86C;
    field_0x64[0]->setPane_80402F04(field_0x10->search('Pstrb14'));
    field_0x64[1] = new PaneCtrl_804AF86C;
    field_0x64[1]->setPane_80402F04(field_0x10->search('Pstrb24'));
    field_0x64[2] = new PaneCtrl_804AF86C;
    field_0x64[2]->setPane_80402F04(field_0x10->search('Pstrb34'));
    field_0x64[3] = new PaneCtrl_804AF86C;
    field_0x64[3]->setPane_80402F04(field_0x10->search('Pstrb44'));
    field_0x74 = lbl_8053AED8->field_0x8;
    const ResTIMG* tex = (const ResTIMG*)fn_804030C4('TIMG', "kk_kettei_01.bti");
    lbl_8053AED8->fn_80440D78(tex);
    lbl_8053AED8->fn_80440CE4((const ResTIMG*)fn_804030C4('TIMG', "kk_modoru_01.bti"));
    field_0x58 = new cls_804B16A8;
    tex = (const ResTIMG*)fn_804030C4('TIMG', "kk_black.bti");
    field_0x5c = new J2DPicture(tex);
    field_0x5c->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));
    field_0x88 = 0.0f;
    field_0x8c = 0.0f;
    field_0x90 = 0.0f;
    for (int i = 0; i < 4; i++) {
        field_0x78[i] = new cls_804B20F8(i, NULL, NULL, 8);
    }
}
