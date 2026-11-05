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

// Sprite frame offsets for field_0x210[0] % 30 (the player sprites' idle
// animation).
static const s32 lbl_80478AA0[30] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6,
};

// FIXME: .rodata 0x80478A60-0x80479AC0 is not split yet; these tables are
// only the parts fn_803F0A54 reads, at their addresses.
static const u64 lbl_80479168[2] = {
    0x000050636F6E3031ull, // "Pcon01"
    0x000050636F6E3032ull, // "Pcon02"
};

// field_0x8's player panes (the effect positions of fn_803F8FBC); the
// first table of the unsplit .rodata object lbl_80479238.
static const u64 lbl_80479238[4] = {
    'Pplli01', 'Pplli02', 'Pplli03', 'Pplli04',
};

// field_0x10's cursor panes, parts of the unsplit .rodata object
// lbl_80479238 at their offsets there (fn_803F35B0).
static const u64 lbl_804793D8[4] = {
    'Pscur01', 'Pscur02', 'Pscur03', 'Pscur04',
};
static const u64 lbl_804793F8[4] = {
    'Paccur02', 'Paccur04', 'Paccur06', 'Paccur08',
};
static const u64 lbl_80479418[4] = {
    'Paccur01', 'Paccur03', 'Paccur05', 'Paccur07',
};

// mScreen's player-count panes (2-4 players), parts of the unsplit .rodata
// object lbl_804795AC at their offsets there (fn_803F5410).
static const u64 lbl_804798C0[3] = {
    'Pnopb08', 'Pnopb12', 'Pnopb16',
};
static const u64 lbl_804798D8[3] = {
    'Plibl02', 'Plibl03', 'Plibl04',
};
static const u64 lbl_804798F0[3] = {
    'Plili02', 'Plili03', 'Plili04',
};

// field_0xc's player panes for 2, 3 and 4 players (field_0x1ac + 2).
static const u64 lbl_80479920[3][4] = {
    {0x005032706C693031ull /* "P2pli01" */, 0x005032706C693032ull /* "P2pli02" */, 0, 0},
    {0x005033706C693031ull /* "P3pli01" */, 0x005033706C693032ull /* "P3pli02" */,
     0x005033706C693033ull /* "P3pli03" */, 0},
    {0x005034706C693031ull /* "P4pli01" */, 0x005034706C693032ull /* "P4pli02" */,
     0x005034706C693033ull /* "P4pli03" */, 0x005034706C693034ull /* "P4pli04" */},
};

// Sets the 2-bit field `player` of root->field_0x83 to `kind`.
static inline void setPlayerKind(GameRoot_8049EF20* root, s32 player, u8 kind) {
    root->field_0x83 &= ~(u8)(3 << (2 * player));
    root->field_0x83 |= (u8)(kind << (2 * player));
}

// Sets bit `i` of gGameRoot_8053AAF8->field_0x82 and the 2-bit field `i` of
// field_0x83 to 1 for the field_0x1ac + 2 players that take part, and clears
// both for the others.
static inline void setEntries(ConnectScreen_804B0F10* self) {
    for (s32 i = 0; i < 4; i++) {
        if (i < self->field_0x1ac + 2) {
            gGameRoot_8053AAF8->field_0x82 |= (u8)(1 << i);
            setPlayerKind(gGameRoot_8053AAF8, i, 1);
        } else {
            gGameRoot_8053AAF8->field_0x82 &= ~(u8)(1 << i);
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
    f32 ratio = (f32)field_0xc->search(lbl_80479168[0])->getAlpha() / (f32)field_0x335;
    JGeometry::TVec3f pos;
    if (field_0x210[0] < 100) {
        for (s32 i = 0; i < 4; i++) {
            if (lbl_80479920[field_0x1ac][i] != 0) {
                SpriteParams_801F5AE0 params;
                params.field_0x26 = 0;
                params.field_0x10.mColor.a = 255.0f * ratio;
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
            params.field_0x10.mColor.a = 255.0f * ratio;
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
            params.field_0x10.mColor.a = 255.0f * ratio;
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
            params.field_0x10.mColor.a = 255.0f * ratio;
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
        if (gGameRoot_8053AAF8->field_0x82 & (u8)(1 << (i + 4))) {
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
        if (gGameRoot_8053AAF8->field_0x82 & (u8)(1 << (i + 4))) {
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
                gGameRoot_8053AAF8->field_0x82 |= (u8)(1 << (i + 4));
                field_0x1c0++;
            } else {
                gGameRoot_8053AAF8->field_0x82 &= ~(u8)(1 << (i + 4));
            }
        } else {
            gGameRoot_8053AAF8->field_0x82 &= ~(u8)(1 << (i + 4));
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
