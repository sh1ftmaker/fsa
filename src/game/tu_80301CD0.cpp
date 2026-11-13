#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/Player.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "game/GbaClient.h"
#include "game/Message.h"
#include "game/PlayerBit.h"
#include "game/tu_80379F84.h"
#include <string.h>

// The GBA link of each player: a queue of files sent to the GBA (names such as
// "player_sob.bin", GBA addresses 0x0600xxxx / 0x05000001), the 0x68-byte
// records fn_802F53F4 receives, and the per-player sprite lists.

// One queued transfer (0x11C bytes; 32 per player in lbl_804FBC18). Named
// after the first function that walks the queue (fn_8030361C).
struct Transfer_8030361C {
    /* 0x000 */ char mName[0x100];
    /* 0x100 */ s32 mWords;   // (strlen + 3) / 4
    /* 0x104 */ s32 field_0x104;
    /* 0x108 */ s32 mIndex;   // its own slot in the queue
    /* 0x10C */ s32 field_0x10c;
    /* 0x110 */ s32 field_0x110;
    /* 0x114 */ u32 mAddress; // GBA address
    /* 0x118 */ u8 mUsed;
    /* 0x119 */ u8 field_0x119;
    /* 0x11A */ u8 field_0x11a;
}; // Size: 0x11C

// The element of the per-player sprite lists lbl_80504A38 / lbl_80505438,
// built by __construct_array with the empty constructor fn_803099FC.
class cls_803099FC {
public:
    cls_803099FC() {}
    void reset() {
        mActive = 0;
        mRemove = 0;
        mTimer = 0;
        mSerial = -1;
    }
    void resetAll() {
        reset();
        field_0xc = -1;
    }

    /* 0x00 */ s32 mTimer;
    /* 0x04 */ s32 mSerial;
    /* 0x08 */ s32 mLayer;
    /* 0x0C */ s32 field_0xc;
    /* 0x10 */ JGeometry::TVec3f mPos;
    /* 0x1C */ s32 mId;
    /* 0x20 */ u8 field_0x20;
    /* 0x21 */ bool field_0x21;
    /* 0x22 */ u8 field_0x22;
    /* 0x23 */ u8 field_0x23;
    /* 0x24 */ u8 mActive;
    /* 0x25 */ u8 mRemove;
}; // Size: 0x28

// cls_804AA8B8 (game/GameGlobals.h): its state lives in the file statics below.
u32 lbl_804DBC18[4][0x2000];
Transfer_8030361C lbl_804FBC18[4][32];
s32 lbl_80504A18[4]; // queue head per player
s32 lbl_80504A28[4]; // queue tail per player
cls_803099FC lbl_80504A38[4][16];
cls_803099FC lbl_80505438[4][40];
u32 lbl_80506D38[4];
s32 lbl_80506D48[12];
f32 lbl_80506D78[4];
s32 lbl_80506D88[4];
s32 lbl_80506D98[4];
u8 lbl_80506DA8[4][4];
u8 lbl_80506DB8[4][4];
u8 lbl_80506DC8[4][4];
s32 lbl_80506DD8[4];
cls_804AA858Sub11C lbl_80506DE8[4];
// Two halves of 19 sprite words per player (fn_803068D0 fills one half from
// a record's field_0x18 words and hides the rest).
GbaSprite_80506F88 lbl_80506F88[4][2][19];
u16 lbl_805071E8[4][0x80];

u8 lbl_8053ACD0;
u8 lbl_8053ACD4[4];
u8 lbl_8053ACD8[4];
u8 lbl_8053ACDC[4];
u8 lbl_8053ACE0[4];
u8 lbl_8053ACE4[4];
u8 lbl_8053ACE8[4];
u8 lbl_8053ACEC[4];
u8 lbl_8053ACF0[4];
u8 lbl_8053ACF4[4];
u8 lbl_8053ACF8[4];
u8 lbl_8053ACFC[4];
u8 lbl_8053AD00[4];
u8 lbl_8053AD04[4];
u8 lbl_8053AD08;
u16 lbl_8053AD0C[4];
u16 lbl_8053AD14[4];
u8 lbl_8053AD1C[4];
u8 lbl_8053AD20[4];
u8 lbl_8053AD24[4];
u8 lbl_8053AD28[4];
cls_804AA8C8* lbl_8053AD30;
u8 lbl_8053AD34[4];

// Whether the player's GBA link answers (getPlayerInputSlot_8030AAF4's input slot).
static inline u8 isLinked(s32 player) {
    return lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(player));
}

// Whether the player's transfer queue holds anything.
static inline bool isQueueBusy(s32 player) {
    if (lbl_80504A18[player] == lbl_80504A28[player]) {
        return false;
    }
    return true;
}

// __sinit_tu_80301CD0_cpp (compiler-generated: the two __construct_array calls for the sprite
// lists and the JAudio pools of GameAudio.h). lbl_804DB798 (0x380, read by fn_80304FB0 here and
// fn_8030A258) is not part of this object: with it (and tu_803006E8's sinit objects, 0x480 in all)
// first in the .bss, the sinit offsets shift by 0x480, so this unit's .bss starts at lbl_804DBC18 and splits.txt gives the object to
// tu_803006E8 (the previous unit).
//
// Not written yet: fn_80301D54, fn_803020C8, fn_803022C4, fn_8030275C, fn_80302C34, fn_80302F9C,
// fn_80303870, fn_803064A8, fn_803066F0, fn_803068D0,
// fn_80307B58, fn_8030801C and the constructor fn_8030905C (cls_804AA8B8).

// Forward declarations.
bool fn_80307830(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, u8 d, bool e, s32 f, u8 g);
bool fn_803075AC(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, u8 d, s32 e, s32 player, bool g, u8 h, u8 i);

// 0x8030905C
// FIXME: 84.65 %, missing/extra + reloc-name; behaviour drift.
cls_804AA8B8::cls_804AA8B8() {
    for (s32 p = 0; p < 4; p++) {
        lbl_80504A18[p] = 0;
        lbl_80504A28[p] = 0;
        for (s32 i = 0; i < 32; i++) {
            Transfer_8030361C& t = lbl_804FBC18[p][i];
            t.mUsed = 0;
            t.field_0x119 = 0;
            t.field_0x104 = 0;
            t.mWords = 0;
            t.mAddress = 0;
            t.mIndex = i;
            t.field_0x10c = 0;
            t.mName[0] = 0;
        }
        for (s32 i = 0; i < 0x2000; i++) {
            lbl_804DBC18[p][i] = 0xCDCDCDCD;
        }
        lbl_80506D48[p] = 0;
        lbl_8053AD00[p] = 0;
        lbl_80506D48[p + 4] = 5;
        fn_8030801C(p);
        lbl_8053ACD4[p] = 1;
        lbl_80506D38[p] = 0;
        lbl_8053ACD8[p] = 0;
        lbl_8053ACDC[p] = 0;
        lbl_8053ACE0[p] = 0;
        lbl_8053ACE4[p] = 0;
        lbl_80506D48[p + 8] = 0;
        for (s32 i = 0; i < 19; i++) {
            lbl_80506F88[p][0][i].f.field_0x3_0 = 1;
            lbl_80506F88[p][0][i].f.field_0x2 = 0x80;
            lbl_80506F88[p][1][i].f.field_0x3_0 = 1;
            lbl_80506F88[p][1][i].f.field_0x2 = 0x80;
        }
        for (s32 k = 0; k < 4; k++) {
            lbl_80506DA8[p][k] = 0;
            lbl_80506DB8[p][k] = 0;
            lbl_80506DC8[p][k] = 0;
        }
        lbl_80506D88[p] = 0;
        lbl_80506D98[p] = 0;
        lbl_8053ACE8[p] = 0;
        lbl_8053ACEC[p] = 0;
        lbl_8053ACF0[p] = 0;
        lbl_8053ACF8[p] = 0;
        lbl_8053ACFC[p] = 0;
        lbl_80506DD8[p] = 0;
        for (s32 i = 0; i < 0x80; i++) {
            lbl_805071E8[p][i] = 0;
        }
        lbl_8053AD0C[p] = 0;
        lbl_8053AD04[p] = 0;
        lbl_8053AD14[p] = 0;
        lbl_8053AD1C[p] = 0;
        lbl_8053AD20[p] = 0;
        lbl_8053AD24[p] = 0;
        lbl_8053AD28[p] = 0;
    }
    lbl_8053AD08 = 0;
}

// 0x80309014
cls_804AA8B8::~cls_804AA8B8() {}

// FIXME: 95.36 %, regalloc-only; behaviour clean.
// Regalloc only: the same code as fn_80308D0C, but the original keeps the
// player counter in r4, the list bases in r6 / r7 and the element in r5 (ours r7, r5 / r6, r4).
// 0x80308E90
void cls_804AA8B8::fn_80308E90() {
    for (int player = 0; player < 4; player++) {
        for (int i = 0; i < 16; i++) {
            lbl_80504A38[player][i].reset();
        }
        for (int j = 0; j < 40; j++) {
            lbl_80505438[player][j].resetAll();
        }
    }
}

// 0x80308D0C
void fn_80308D0C() {
    for (s32 player = 0; player < 4; player++) {
        for (int i = 0; i < 16; i++) {
            lbl_80504A38[player][i].reset();
        }
        for (int j = 0; j < 40; j++) {
            lbl_80505438[player][j].resetAll();
        }
    }
}

// Fills the header of the player's record: player, size, layer, mode and
// link flags, then the mode's status bits in bytes 4-7.
// 0x803085A4
// FIXME: 90.87 %, missing/extra; behaviour drift.
void cls_804AA8B8::fn_803085A4(s32 player) {
    s32 layer = fn_8022EA0C(player);
    cls_804AA858Sub11C* record = &lbl_80506DE8[player];
    record->field_0x3_0 = lbl_8053AD08;
    record->mPlayer = layer;
    if (getPlayerLayer_8023DE58(layer) < 8) {
        record->mValid = 0;
    } else {
        record->mValid = getPlayerLayer_8023DE58(layer) - 7;
    }
    record->mMode = lbl_80506D48[player];
    if (lbl_8053ACD0 && isPlayerBitSet(gGameRoot_8053AAF8, player)) {
        if (gGameRoot_8053AAF8->field_0x4c == 9 || gGameRoot_8053AAF8->field_0x80 != 0
            || (lbl_8053ACD8[player] && (fn_8037F1E4(player) < 1 || fn_8037F1E4(player) > 5))) {
            record->field_0x2_5 = 1;
        } else {
            record->field_0x2_5 = 0;
        }
        record->field_0x3_2 = lbl_8053ACEC[player] > 1 ? lbl_8053ACE8[player] : 0;
    } else {
        record->field_0x2_5 = 1;
        record->field_0x3_2 = 0;
    }
    record->mMask = 0;
    record->field_0x1_0 = 0;
    record->field_0x1_0 |= lbl_8053ACF8[0];
    record->field_0x1_0 |= lbl_8053ACF8[1] << 1;
    record->field_0x1_0 |= lbl_8053ACF8[2] << 2;
    record->field_0x1_0 |= lbl_8053ACF8[3] << 3;
    record->field_0x4.mWord = 0;
    if (lbl_80506D48[player] == 1) {
        record->field_0x4.mMode1.field_0x5_4 = 15;
    }
    if (lbl_8053AAF0->mState >= 4 && (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << player))) {
        record->field_0x4.mOdd.field_0x4_0 = lbl_80506D38[player];
        lbl_80506D38[player] = 0;
        if (lbl_8053ACD0 || lbl_8053AD68->fn_80379F84()) {
            s32 mode = lbl_80506D48[player];
            if ((mode & 1) == 0) {
                record->field_0x4.mEven.field_0x5_0 = lbl_80529DEC.field_0xd8[player];
                record->field_0x4.mEven.field_0x4_5 = (u8)(lbl_80529DEC.field_0xc8[player] / 4 - 1);
                record->field_0x4.mEven.field_0x6_0 = fn_8037EE8C(player);
                s32 v = lbl_80529DEC.field_0xe8[player] + 5000;
                if (v > 14999) {
                    v = 14999;
                } else if (v < 0) {
                    v = 0;
                }
                lbl_80506DE8[player].field_0x4.mEven.field_0x7 = (v >> 6) & 0xFF;
                record->field_0x4.mEven.field_0x6_2 = v & 0x3F;
            } else if (mode == 1) {
                bool timed = gGameRoot_8053AAF8->field_0x4c == 11;
                u8 count = gGameRoot_8053AAF8->field_0x88;
                bool busy;
                if (lbl_8053AEC8 != NULL) {
                    u8 state = lbl_8053AEC8->field_0x40;
                    if (state == 1 || (u8)(state - 3) <= 1 || state == 6) {
                        busy = true;
                    } else {
                        busy = false;
                    }
                } else {
                    busy = false;
                }
                record->field_0x4.mMode1.field_0x4_5 = count | (busy << 1) | timed;
                record->field_0x4.mMode1.field_0x5_4 = lbl_80529DEC.field_0xf8[player];
            }
        }
        if (lbl_8053ACD0) {
            s32 mode = lbl_80506D48[player];
            if (mode == 2) {
                record->field_0x4.mEven.field_0x5_7 = getGameView_802D6A9C()->fn_802D7804(getPlayerLayer_8023DE58(layer), 1);
            } else if ((mode & 1) == 1) {
                if (getPlayerLayer_8023DE58(layer) >= 8) {
                    u8 skip;
                    if (gGameRoot_8053AAF8->field_0x4c == 11) {
                        skip = 1;
                    } else {
                        skip = gGameRoot_8053AAF8->field_0x88 == 2;
                    }
                    if (!skip) {
                        s32 t;
                        if (fn_8023BC38(layer) > 0) {
                            t = (fn_8023BC38(layer) + 59) / 60;
                            if (t > 11) {
                                t = 11;
                            }
                            t += 20;
                        } else {
                            if (fn_80236768(layer) > 0) {
                                t = (fn_80236768(layer) + 59) / 60;
                            } else if (fn_80236630(layer) > 0) {
                                t = (fn_80236630(layer) + 59) / 60;
                            } else if (fn_802366CC(layer) > 0) {
                                t = (fn_802366CC(layer) + 59) / 60;
                            } else {
                                t = (getGameView_802D6A9C()->field_0xc4 + 59) / 60;
                                if (!getGameView_802D6A9C()->field_0x131f0 || t > 5) {
                                    t = 0;
                                }
                            }
                            if (t > 20) {
                                t = 20;
                            }
                        }
                        record->field_0x4.mOdd.field_0x4_7 = t;
                        record->field_0x4.mOdd.field_0x5_0 = (u8)(t >> 1);
                    }
                }
                if (lbl_80506D48[player] == 3) {
                    if (getGameView_802D6A9C()->fn_802D7804(getPlayerLayer_8023DE58(layer), 12)) {
                        record->field_0x4.mOdd.mOffsetMode =
                            (getGameView_802D6A9C()->field_0x131b8[getPlayerLayer_8023DE58(layer)] & 0xC) >> 2;
                    } else {
                        record->field_0x4.mOdd.mOffsetMode = fn_802351D8(layer) != 0;
                    }
                    record->field_0x4.mOdd.field_0x4_5 =
                        (getGameView_802D6A9C()->field_0x131b8[getPlayerLayer_8023DE58(layer)] & 0x3000) >> 12;
                    record->field_0x4.mOdd.field_0x5_6 = (u8)(fn_80237060(player) - 1);
                    if (!fn_802DEBF8() && !fn_802DEBA0()) {
                        record->field_0x4.mOdd.field_0x5_7 = 1;
                    }
                }
            }
        }
        if (lbl_8053AD68->fn_80379F84()) {
            s32 mode = lbl_80506D48[player];
            if (mode == 0) {
                record->field_0x4.mEven.field_0x5_7 = 1;
            } else if (mode == 3) {
                record->field_0x4.mOdd.field_0x5_7 = 1;
            }
        }
    }
    lbl_8053ACDC[player] = 0;
}

// Builds the player's first GBA record: the kind, the four marks and the hidden sprites,
// then the checksum.
// 0x8030801C
// FIXME: 64.80 %, missing/extra; behaviour drift.
void cls_804AA8B8::fn_8030801C(s32 player) {
    cls_804AA858Sub11C* record;
    if (lbl_8053AAF0->mState >= 4) {
        s32 mode = gGameRoot_8053AAF8->field_0x4c;
        if (mode == 7 || mode == 11 || mode == 12) {
            lbl_80506DE8[player].mKind = 9;
        } else {
            lbl_80506DE8[player].mKind = 13;
        }
    } else {
        lbl_80506DE8[player].mKind = 13;
    }
    fn_803085A4(player);
    record = &lbl_80506DE8[player];
    for (s32 i = 0; i < 4; i++) {
        record->mEntries[i].mXHigh = 1;
        record->mEntries[i].mXLow = 0x80;
        record->mEntries[i].mY = 0xE0;
        record->mEntries[i].field_0x3_1 = 0;
        record->mEntries[i].field_0x0 = 0;
    }
    record->mEntries[0].mScroll = 0;
    record->mEntries[0].mScrollBit = 0;
    record->mEntries[1].mScroll = 0;
    record->mEntries[2].mScroll = 0;
    record->mEntries[2].mScrollBit = 0;
    record->mEntries[3].mScroll = 0;
    s32 layer = fn_8022EA0C(player);
    if (lbl_8053AAF0->mState >= 4) {
        u8 near = 0;
        if (fn_8037F1E4(player) >= 1 && fn_8037F1E4(player) <= 5) {
            near = 1;
        }
        record->mEntries[1].mScrollBit = near ? true : false;
        if (lbl_8053ACD0 && (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << player))) {
            u8 flag = 0;
            if (fn_8022F5B0(layer)) {
                s32 l = getPlayerLayer_8023DE58(layer);
                if (!getGameView_802D6A9C()->fn_802D7804(l, 2)) {
                    flag = 1;
                }
            }
            record->mEntries[3].mScrollBit = flag ? true : false;
        } else {
            record->mEntries[3].mScrollBit = 0;
        }
    } else {
        record->mEntries[1].mScrollBit = 0;
        record->mEntries[3].mScrollBit = 0;
    }
    for (s32 i = 0; i < lbl_80541618[lbl_8053AD08] - 7; i++) {
        GbaSprite_80506F88& sprite = record->field_0x18[i];
        sprite.f.field_0x3_0 = 1;
        sprite.f.field_0x2 = 0x80;
        sprite.f.field_0x1 = 0xE0;
        sprite.f.field_0x3_1 = 0;
        sprite.f.field_0x0 = 0;
        sprite.f.mKind = 0;
        sprite.f.field_0x3_4 = 0;
    }
    u8 sum = 0;
    u8* bytes = (u8*)&record->mSprites[0];
    for (s32 k = 8; k < lbl_80541618[lbl_8053AD08] * 4; k++) {
        sum += *bytes++;
    }
    record->field_0x0 = sum;
}

static inline s32 spriteId(const GbaSprite_80506F88& sprite) {
    return sprite.f.field_0x0 | sprite.f.field_0x3_1 << 8;
}

// fn_803074A4's kind of a sprite word.
static inline s32 spriteKind(const GbaSprite_80506F88& sprite) {
    return fn_803074A4(sprite.f.mKind, spriteId(sprite));
}

// Byte-reverses a word.
static inline u32 swapBytes(u32 v) {
    union {
        u32 w;
        u8 b[4];
    } in, out;
    in.w = v;
    out.b[0] = in.b[3];
    out.b[1] = in.b[2];
    out.b[2] = in.b[1];
    out.b[3] = in.b[0];
    return out.w;
}

// Fills the player's record with the message words (mode 10 or 11), byte-swapped,
// and its checksum. Still differs: the original has the checksum loop twice (branch on the
// queue head entry's mUsed) and different register allocation.
// 0x80307B58
// FIXME: 71.06 %, missing/extra; behaviour drift.
void cls_804AA8B8::fn_80307B58(s32 player, bool flag) {
    if (flag) {
        lbl_80506DE8[player].mKind = 10;
    } else {
        lbl_80506DE8[player].mKind = 11;
    }
    fn_803085A4(player);
    s32 count = (lbl_80541618[lbl_8053AD08] - 2) * 2;
    s32 start = lbl_8053AD0C[player];
    s32 i;
    for (i = 0; i < count; i++) {
        lbl_805071E8[player][start + i] = fn_8037F0CC(player);
        if (fn_8037F1E4(player) != 3) {
            lbl_8053AD24[player] = 1;
            lbl_8053AD1C[player] = 0;
            lbl_80506DE8[player].mKind = 12;
            i++;
            break;
        }
    }
    lbl_8053AD0C[player] += i;
    for (; i < count; i++) {
        lbl_805071E8[player][start + i] = 0;
    }
    s32 n = lbl_80541618[lbl_8053AD08] - 2;
    for (s32 j = 0; j < n; j++) {
        u32 w = (u16)lbl_805071E8[player][start + j * 2] | (u32)(u16)lbl_805071E8[player][start + j * 2 + 1] << 16;
        lbl_80506DE8[player].mSprites[j].mWord = swapBytes(w);
    }
    u8 sum = 0;
    u8* bytes = (u8*)&lbl_80506DE8[player].mSprites[0];
    for (s32 k = 8; k < lbl_80541618[lbl_8053AD08] * 4; k++) {
        sum += *bytes++;
    }
    lbl_80506DE8[player].field_0x0 = sum;
}

// Sprite ids below 0x45 (after adding 0x200 when e is 2) go to the 16-entry
// lists, the others (plus 0x400) to the 40-entry ones.
// 0x80307AC8
void fn_80307AC8(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, s32 d, s32 e,
                 s32 f) {
    if (e == 2) {
        e = 0;
        id += 0x200;
    }
    if (id < 0x45) {
        fn_80307830(serial, layer, pos, id, d != 0, false, e != 0, 4);
    } else {
        fn_803075AC(serial, layer, pos, id + 0x400, d != 0, 2, f, 0, e != 0, 0);
    }
}

// The same for the 16-entry lists of the players on `layer` (8 or more).
// 0x80307830
bool fn_80307830(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, u8 d, bool e,
                 s32 f, u8 g) {
    if (layer < 8) {
        return false;
    }
    bool result = false;
    for (int p = 0; p < 4; p++) {
        s32 slot = fn_8022EA0C(p);
        if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << p))) {
            continue;
        }
        if (layer != getPlayerLayer_8023DE58(slot)) {
            continue;
        }
        cls_803099FC* list = lbl_80504A38[p];
        int k;
        for (k = 0; k < 16; k++) {
            if (list->mActive == 1 && list->mSerial == serial) {
                break;
            }
            list++;
        }
        if (k < 16) {
            list->mLayer = layer;
            list->mPos = *pos;
            list->mId = id;
            list->field_0x21 = d;
            list->field_0x22 = f;
            result = true;
        } else {
            list = lbl_80504A38[p];
            for (k = 0; k < 16; k++) {
                if (list->mActive != 1) {
                    list->mSerial = serial;
                    list->mLayer = layer;
                    list->mPos = *pos;
                    list->mId = id;
                    list->mActive = 1;
                    list->mTimer = 1;
                    if (e) {
                        if (g == 4) {
                            list->field_0x20 = 3;
                        } else {
                            list->field_0x20 = (g & 3) | 4;
                        }
                    } else {
                        list->field_0x20 = 2;
                    }
                    list->field_0x21 = d;
                    list->field_0x23 = 0;
                    list->field_0x22 = f;
                    result = true;
                    break;
                }
                list++;
            }
        }
    }
    return result;
}

// Shows the sprite of actor `serial` on the GBA screens of the players on
// `layer` (or of player `player` when it is 0 or more), in their 40-entry
// lists; returns whether any list took it.
// 0x803075AC
bool fn_803075AC(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, u8 d, s32 e,
                 s32 player, bool g, u8 h, u8 i) {
    if (layer < 8 && layer != -1) {
        return false;
    }
    bool result = false;
    for (int p = 0; p < 4; p++) {
        s32 slot = fn_8022EA0C(p);
        if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << p))) {
            continue;
        }
        if (player != p && (player >= 0 || layer != getPlayerLayer_8023DE58(slot))) {
            continue;
        }
        cls_803099FC* list = lbl_80505438[p];
        int k;
        for (k = 0; k < 40; k++) {
            if (list->mActive == 1 && list->mSerial == serial) {
                break;
            }
            list++;
        }
        if (k < 40) {
            list->mLayer = layer;
            list->mPos = *pos;
            list->mId = id;
            list->field_0x21 = d;
            list->field_0x22 = h;
            result = true;
        } else {
            list = lbl_80505438[p];
            for (k = 0; k < 40; k++) {
                if (list->mActive == 0) {
                    list->mTimer = e;
                    list->mSerial = serial;
                    list->mLayer = layer;
                    list->field_0xc = player;
                    list->mPos = *pos;
                    list->mId = id;
                    list->field_0x21 = d;
                    list->field_0x20 = g;
                    list->field_0x23 = i;
                    list->field_0x22 = h;
                    list->mActive = 1;
                    result = true;
                    break;
                }
                list++;
            }
        }
    }
    return result;
}

// The kind of sprite `id` of list `kind` (0-2).
// 0x803074A4
s32 fn_803074A4(s32 kind, s32 id) {
    if (kind == 2) {
        if (id >= 0x39 && id < 0x45) {
            return 4;
        }
        if (id >= 0 && id < 0x25) {
            return -1;
        }
        if (id >= 0x200) {
            return 1;
        }
        return 0;
    }
    if (kind == 0 || kind == 1) {
        if ((id >= 0xC0 && id < 0xD3) || (id >= 0x155 && id < 0x158)) {
            return 3;
        }
        if (id >= 0x37 && id < 0x41) {
            return 2;
        }
        if (id >= 0x3CD && id < 0x3CE) {
            return -1;
        }
        if (id >= 0x2A3 && id < 0x2AA) {
            return 4;
        }
        if (id >= 0x34C && id < 0x37C) {
            return 4;
        }
        if (kind == 1) {
            return 1;
        }
        return 0;
    }
    return 0;
}

// Builds the player's regular GBA record: the kind, the other players' marks
// (screen position and sprite), the mode's status bits, the scroll, the sprite
// words sorted by kind, the layer's mask word and the checksum.
// Still differs: the original has the checksum loop twice (branch on the queue head entry's mUsed).
// 0x80306A94
// FIXME: 85.19 %, missing/extra; behaviour drift.
void cls_804AA8B8::fn_80306A94(s32 player) {
    if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << player)) {
        lbl_80506DE8[player].mKind = 5;
    } else {
        lbl_80506DE8[player].mKind = 14;
    }
    fn_803085A4(player);
    s32 layer = fn_8022EA0C(player);
    cls_804AA858Sub11C* record = &lbl_80506DE8[player];
    for (s32 i = 0; i < 4; i++) {
        s32 idx = fn_802340A0(i);
        if (!fn_8023F264(i) || !fn_8023188C(i) || getPlayerLayer_8023DE58(layer) < 8 ||
            (!fn_8022BC38(layer, getPlayerLayer_8023DE58(i), getPlayerPos_8023E724(i)) &&
             (getPlayerLayer_8023DE58(layer) > 8 || getPlayerLayer_8023DE58(i) > 8))) {
            record->mEntries[idx].mXHigh = 1;
            record->mEntries[idx].mXLow = 0x80;
            record->mEntries[idx].mY = 0xE0;
            record->mEntries[idx].field_0x3_1 = 0;
            record->mEntries[idx].field_0x0 = 0;
        } else {
            if (getPlayerLayer_8023DE58(i) < 8) {
                record->mMask |= 1 << idx;
                record->mEntries[idx].mXHigh = fn_8023D21C(i, layer) >> 8;
                record->mEntries[idx].mXLow = fn_8023D21C(i, layer);
                record->mEntries[idx].mY = fn_8023CEAC(i, layer);
            } else {
                record->mEntries[idx].mXHigh = fn_8023DAF4(i, layer) >> 8;
                record->mEntries[idx].mXLow = fn_8023DAF4(i, layer);
                record->mEntries[idx].mY = fn_8023D644(i, layer);
            }
            record->mEntries[idx].field_0x3_1 = fn_8023CBC0(i) >> 8;
            record->mEntries[idx].field_0x0 = fn_8023CBC0(i);
        }
    }
    if ((lbl_80506D48[player] & 1) == 1) {
        for (s32 i = 0; i < 4; i++) {
            s32 idx = fn_802340A0(i);
            record->field_0x4.mOdd.field_0x7_4 |= fn_8022BECC(idx) << idx;
            bool skip;
            if (gGameRoot_8053AAF8->field_0x4c == 11) {
                skip = true;
            } else {
                skip = gGameRoot_8053AAF8->field_0x88 == 2;
            }
            u8 state;
            if (skip) {
                state = 3;
            } else if (fn_8023CA88(idx) > 0) {
                s32 l = getPlayerLayer_8023DE58(i);
                if (((getGameView_802D6A9C()->field_0x131b8[l] & 0x3000) >> 12) == 0) {
                    state = 3;
                } else {
                    state = 1;
                }
            } else if (fn_8023EA28(i)) {
                state = 1;
            } else {
                state = 0;
            }
            record->field_0x4.mOdd.field_0x6 |= state << (idx * 2);
            if (fn_8023BCD4(idx) < 0xA0) {
                record->field_0x4.mOdd.field_0x7_0 |= 1 << idx;
            }
        }
    }
    record->mEntries[0].mScroll = (fn_8023CE10(layer) >> 5) & 0xF;
    record->mEntries[0].mScrollBit = (fn_8023CE10(layer) >> 4) & 1;
    record->mEntries[1].mScroll = fn_8023CE10(layer);
    record->mEntries[2].mScroll = (fn_8023CD74(layer) >> 5) & 0xF;
    record->mEntries[2].mScrollBit = (fn_8023CD74(layer) >> 4) & 1;
    record->mEntries[3].mScroll = fn_8023CD74(layer);
    u8 near = 0;
    if (fn_8037F1E4(player) >= 1 && fn_8037F1E4(player) <= 5) {
        near = 1;
    }
    record->mEntries[1].mScrollBit = near ? true : false;
    u8 flag = 0;
    if (fn_8022F5B0(layer)) {
        s32 l = getPlayerLayer_8023DE58(layer);
        if (!getGameView_802D6A9C()->fn_802D7804(l, 2)) {
            flag = 1;
        }
    }
    record->mEntries[3].mScrollBit = flag ? true : false;
    GbaSprite_80506F88* sprite = record->field_0x18;
    s32 words = lbl_80541618[lbl_8053AD08] - 7;
    for (s32 i = 0; i < words; i++, sprite++) {
        sprite->f.field_0x3_0 = 1;
        sprite->f.field_0x2 = 0x80;
        sprite->f.field_0x1 = 0xE0;
        sprite->f.field_0x3_1 = 0;
        sprite->f.field_0x0 = 0;
        sprite->f.mKind = 0;
        sprite->f.field_0x3_4 = 0;
    }
    s32 count = 0;
    if (fn_8023F264(player)) {
        count = fn_803066F0((s32)this, player, lbl_80541618[lbl_8053AD08] - 7);
    }
    if (fn_8023F264(player)) {
        fn_803064A8((s32)this, player, lbl_80541618[lbl_8053AD08] - 7 - count, count);
    }
    for (s32 n = lbl_80541618[lbl_8053AD08] - 8; n > 0; n--) {
        for (s32 j = 0; j < n; j++) {
            s32 a = spriteKind(record->field_0x18[j]);
            s32 b = spriteKind(record->field_0x18[j + 1]);
            if (record->field_0x18[j + 1].f.field_0x1 != 0xE0 &&
                (a < b || (a == b && record->field_0x18[j].f.field_0x1 < record->field_0x18[j + 1].f.field_0x1))) {
                u32 tmp = record->field_0x18[j + 1].mWord;
                record->field_0x18[j + 1].mWord = record->field_0x18[j].mWord;
                record->field_0x18[j].mWord = tmp;
            }
        }
    }
    if ((lbl_80506D48[player] & 1) == 0) {
        if (getPlayerLayer_8023DE58(layer) < 8) {
            u64 mask = getGameView_802D6A9C()->fn_802D7AC4(8);
            record->field_0x18[lbl_80541618[lbl_8053AD08] - 7].mWord = swapBytes(mask & 0xFFFFFFFF);
        } else {
            u64 mask = getGameView_802D6A9C()->fn_802D7AC4(getPlayerLayer_8023DE58(layer));
            record->field_0x18[lbl_80541618[lbl_8053AD08] - 7].mWord = swapBytes(mask & 0xFFFFFFFF);
        }
    } else {
        if (getPlayerLayer_8023DE58(layer) < 8) {
            u64 mask = getGameView_802D6A9C()->fn_802D7AC4(8);
            record->field_0x18[lbl_80541618[lbl_8053AD08] - 7].mWord = swapBytes(mask >> 32);
        } else {
            u64 mask = getGameView_802D6A9C()->fn_802D7AC4(getPlayerLayer_8023DE58(layer));
            record->field_0x18[lbl_80541618[lbl_8053AD08] - 7].mWord = swapBytes(mask >> 32);
        }
    }
    u8 sum = 0;
    u8* bytes = (u8*)&record->mSprites[0];
    s32 size = lbl_80541618[lbl_8053AD08] * 4;
    for (s32 k = 8; k < size; k++) {
        sum += *bytes++;
    }
    record->field_0x0 = sum;
}

// Copies a record's sprite words into the half of the player's sprite table
// its mode selects, hides the rest of that half and the kind-2 sprites below
// 0x45 of the other half.
// FIXME(shotgun): 99.10 %, different; register allocation only.
void fn_803068D0(cls_804AA858Sub11C* record) {
    if (record->mValid == 0) {
        return;
    }
    u32 half = record->mMode & 1;
    GbaSprite_80506F88* sprites = lbl_80506F88[fn_8022F340(record->mPlayer)][0];
    GbaSprite_80506F88* dst = sprites + half * 19;
    s32 i;
    GbaSprite_80506F88* src = record->field_0x18;
    u32 index = record->field_0x3_0;
    for (i = 0; i < lbl_80541618[index] - 7; i++) {
        dst[i].mWord = src[i].mWord;
    }
    for (; i < 19; i++) {
        dst[i].f.field_0x3_0 = 1;
        dst[i].f.field_0x2 = 0x80;
    }
    GbaSprite_80506F88* other = sprites + (half ^ 1) * 19;
    for (i = 0; i < 19; i++) {
        if (other[i].f.mKind == 2 && (other[i].f.field_0x0 | other[i].f.field_0x3_1 << 8) < 0x45) {
            other[i].f.field_0x3_0 = 1;
            other[i].f.field_0x2 = 0x80;
        }
    }
}

// Fills the player's record sprite words from the player's active short-list
// sprites (lbl_80504A38), timer 5 down to 0, at most `max` of them and four of
// kind 4 or more; every sprite it looks at is marked for removal. Returns the
// count written.
// FIXME(shotgun): 77.53 %, missing/extra; behaviour drift.
// (the original tests mode 11 as a bool, cntlzw)
s32 fn_803066F0(s32 unused, s32 player, s32 max) {
    s32 count = 0;
    s32 big = 0;
    GbaSprite_80506F88* dst = lbl_80506DE8[player].field_0x18;
    s32 slot = fn_8022EA0C(player);
    for (s32 timer = 5; timer >= 0; timer--) {
        cls_803099FC* sprite = lbl_80504A38[player];
        for (s32 i = 0; i < 16; i++, sprite++) {
            if (sprite->mActive != 1 || sprite->mTimer != timer) {
                continue;
            }
            if (sprite->mLayer != getPlayerLayer_8023DE58(slot) ||
                ((gGameRoot_8053AAF8->field_0x4c == 11 ? true : gGameRoot_8053AAF8->field_0x88 != 0) &&
                 sprite->field_0x23 == 0)) {
                sprite->mRemove = 1;
                continue;
            }
            s32 x = fn_8023D9B0(&sprite->mPos, slot) & 0x1FF;
            u8 y = fn_8023D4F8(&sprite->mPos, slot);
            if (x == 0x180 || y == 0xE0) {
                sprite->mRemove = 1;
                continue;
            }
            if (sprite->field_0x20 >= 4) {
                if (big >= 4) {
                    sprite->mRemove = 1;
                    continue;
                }
                big++;
            }
            count++;
            dst->f.field_0x3_0 = x >> 8;
            s32 id = sprite->mId;
            dst->f.field_0x2 = x;
            dst->f.field_0x1 = y;
            sprite->mRemove = 1;
            dst->f.field_0x3_1 = id >> 8;
            dst->f.field_0x0 = id;
            dst->f.mKind = sprite->field_0x20;
            dst->f.field_0x3_4 = sprite->field_0x21;
            dst->f.field_0x3_3 = sprite->field_0x22;
            if (count >= max) {
                return count;
            }
            dst++;
        }
    }
    return count;
}

// Appends the player's long-list sprites (lbl_80505438), timer 5 down to 1
// (or any timer for the player's own), to the record's sprite words after
// fn_803066F0's `start` ones, until `max`; ids above 0x3FF are kind 2.
// FIXME(shotgun): 88.05 %, missing/extra; behaviour drift.
void fn_803064A8(s32 unused, s32 player, s32 max, s32 start) {
    s32 count = start;
    if (max == 0) {
        return;
    }
    s32 slot = fn_8022EA0C(player);
    GbaSprite_80506F88* dst = &lbl_80506DE8[player].field_0x18[start];
    for (s32 timer = 5; timer > 0; timer--) {
        cls_803099FC* sprite = lbl_80505438[player];
        for (s32 i = 0; i < 40; i++, sprite++) {
            if (sprite->mActive == 0) {
                continue;
            }
            if ((sprite->mLayer != getPlayerLayer_8023DE58(slot) && sprite->field_0xc != player) ||
                (gGameRoot_8053AAF8->field_0x4c == 11 && sprite->field_0x23 == 0)) {
                sprite->mTimer = 0;
                sprite->mRemove = 1;
                continue;
            }
            if (sprite->mTimer != timer && sprite->field_0xc != player) {
                continue;
            }
            if (sprite->field_0xc >= 0) {
                dst->f.field_0x3_0 = (s32)sprite->mPos.x >> 8;
                dst->f.field_0x2 = (s32)sprite->mPos.x;
                dst->f.field_0x1 = (s32)sprite->mPos.y;
            } else {
                s32 x = fn_8023D9B0(&sprite->mPos, slot) & 0x1FF;
                u8 y = fn_8023D4F8(&sprite->mPos, slot);
                if (x == 0x180 || y == 0xE0) {
                    sprite->mTimer = 0;
                    sprite->mRemove = 1;
                    continue;
                }
                dst->f.field_0x3_0 = x >> 8;
                dst->f.field_0x2 = x;
                dst->f.field_0x1 = y;
            }
            s32 id = sprite->mId;
            if (id > 0x3FF) {
                id -= 0x400;
                dst->f.field_0x3_1 = id >> 8;
                dst->f.field_0x0 = id;
                dst->f.mKind = 2;
            } else {
                dst->f.field_0x3_1 = id >> 8;
                dst->f.field_0x0 = id;
                dst->f.mKind = sprite->field_0x20;
            }
            count++;
            dst->f.field_0x3_4 = sprite->field_0x21;
            dst->f.field_0x3_3 = sprite->field_0x22;
            sprite->mRemove = 1;
            dst++;
            if (count >= max) {
                return;
            }
        }
    }
}

// The per-frame GBA link update: counts the linked GBAs (the transfer length
// index), then for every player either sends the regular record, feeds the
// transfer queue (file words, kinds 6-8) or the message words (kinds 10-12).
// Still differs: the original has each checksum loop twice (branch on the queue head entry's mUsed).
// 0x80304FB0
// FIXME: 76.04 %, missing/extra + reloc-name; behaviour drift.
void cls_804AA8B8::fn_80304FB0() {
    GbaClient_803007D0* client = lbl_804DB798;
    lbl_8053AD08 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (lbl_804DB798[i].field_0xd8 || lbl_804DB798[i].field_0xd9) {
            lbl_8053AD08++;
        }
    }
    if (lbl_8053AD08 != 0) {
        lbl_8053AD08 = (lbl_8053AD08 - 1) & 3;
    }
    for (s32 player = 0; player < 4; player++, client++) {
        s32 layer = fn_8022EA0C(player);
        cls_804AA858Sub11C* record = &lbl_80506DE8[player];
        if (lbl_8053AD30->fn_8030A364(getPlayerInputSlot_8030AAF4(player))) {
            if (lbl_8053ACD0 && isPlayerBitSet(gGameRoot_8053AAF8, player)) {
                fn_80306A94(player);
                gGameRoot2_8053AB10->field_0x20->fn_802F53F4(player, record);
                lbl_80506D78[player] += 0.7854;
                switch (lbl_8053ACEC[player]) {
                case 0:
                    if (lbl_8053ACE8[player]) {
                        lbl_8053ACF0[player] = 0;
                        lbl_8053ACEC[player]++;
                    }
                    break;
                case 1:
                    lbl_8053ACF0[player]++;
                    if ((lbl_8053ACE8[player] == 1 && fn_8022F64C(layer)) ||
                        (lbl_8053ACE8[player] != 1 && lbl_8053ACF0[player] >= lbl_8053ACF4[player])) {
                        lbl_8053ACF0[player] = 0;
                        lbl_8053ACEC[player]++;
                    }
                    break;
                case 2:
                    if (lbl_8053ACE8[player] == 1) {
                        if (lbl_8053ACF0[player] < 0x1F) {
                            lbl_8053ACF0[player]++;
                        }
                    } else {
                        if (lbl_8053ACF0[player] < 0x10) {
                            lbl_8053ACF0[player]++;
                        }
                    }
                    break;
                default:
                    if (lbl_8053ACF0[player] == 0) {
                        lbl_8053ACE8[player] = 0;
                        lbl_8053ACEC[player] = 0;
                        lbl_8053ACF4[player] = 0;
                    } else {
                        lbl_8053ACF0[player]--;
                    }
                    break;
                }
                if (lbl_8053ACF8[layer]) {
                    if (lbl_8053ACFC[layer] < 0x11) {
                        lbl_8053ACFC[layer]++;
                    }
                } else {
                    lbl_8053ACFC[layer] = 0;
                }
            } else {
                fn_8030801C(player);
            }
            lbl_80506D48[player] = (lbl_80506D48[player] + 1) & 3;
            continue;
        }
        if (!isLinked(player)) {
            Transfer_8030361C* entry = &lbl_804FBC18[player][lbl_80504A18[player]];
            if (entry->mUsed == 1) {
                entry->field_0x119 = 0;
                entry->field_0x104 = 0;
                entry->field_0x10c = 0;
            } else if (lbl_8053AD20[player] == 1 && lbl_8053AD24[player] != 1) {
                if (lbl_8053AD28[player] == 1) {
                    lbl_8053AD0C[player] = lbl_8053AD14[player];
                    lbl_8053AD24[player] = 1;
                    lbl_8053AD1C[player] = 60;
                } else {
                    do {
                        lbl_805071E8[player][lbl_8053AD0C[player]++] = fn_8037F0CC(player);
                    } while (fn_8037F1E4(player) == 3);
                    lbl_8053AD24[player] = 1;
                    lbl_8053AD1C[player] = 60;
                }
            }
            lbl_80506D48[player + 4] = 5;
            continue;
        }
        if (lbl_8053AD34[player] == 0) {
            if (isLinked(player) == 1 && lbl_8053ACD0 && isPlayerBitSet(gGameRoot_8053AAF8, player) &&
                getPlayerLayer_8023DE58(layer) >= 8) {
                gGameRoot2_8053AB10->field_0x20->field_0x12c[player] = 1;
            }
            continue;
        }
        if (lbl_8053AAF0->field_0x60) {
            fn_8030801C(player);
            continue;
        }
        if (client->field_0xdc) {
            fn_80303CE4(player);
            client->field_0xdc = 0;
        }
        lbl_80506D78[player] += 0.7854;
        switch (lbl_8053ACEC[player]) {
        case 0:
            if (lbl_8053ACE8[player]) {
                lbl_8053ACF0[player] = 0;
                lbl_8053ACEC[player]++;
            }
            break;
        case 1:
            lbl_8053ACF0[player]++;
            if ((lbl_8053ACE8[player] == 1 && fn_8022F64C(layer)) ||
                (lbl_8053ACE8[player] != 1 && lbl_8053ACF0[player] >= lbl_8053ACF4[player])) {
                lbl_8053ACF0[player] = 0;
                lbl_8053ACEC[player]++;
            }
            break;
        case 2:
            if (lbl_8053ACE8[player] == 1) {
                if (lbl_8053ACF0[player] < 0x1F) {
                    lbl_8053ACF0[player]++;
                } else {
                    lbl_8053ACEC[player]++;
                }
            } else {
                if (lbl_8053ACF0[player] < 0x10) {
                    lbl_8053ACF0[player]++;
                } else {
                    lbl_8053ACEC[player]++;
                }
            }
            break;
        case 3:
            if (lbl_8053ACF0[player] != 0) {
                lbl_8053ACF0[player]--;
            }
            break;
        default:
            lbl_8053ACE8[player] = 0;
            lbl_8053ACEC[player] = 0;
            lbl_8053ACF0[player] = 0;
            lbl_8053ACF4[player] = 0;
            break;
        }
        if (lbl_8053ACF8[layer]) {
            if (lbl_8053ACFC[layer] < 0x11) {
                lbl_8053ACFC[layer]++;
            }
        } else {
            lbl_8053ACFC[layer] = 0;
        }
        if (lbl_80506D48[player + 4] > 0) {
            fn_8030801C(player);
            lbl_80506D48[player] = (lbl_80506D48[player] + 1) & 3;
            lbl_80506D48[player + 4]--;
            continue;
        }
        Transfer_8030361C* entry = &lbl_804FBC18[player][lbl_80504A18[player]];
        s32 otherLayer;
        if (entry->mUsed == 1 && entry->field_0x104 == 0 &&
            (entry->mAddress == 0x0600C000 || entry->mAddress == 0x0600E000) &&
            entry->field_0x110 != getPlayerLayer_8023DE58(layer)) {
            otherLayer = 1;
        } else {
            otherLayer = 0;
        }
        if (entry->mUsed == 0 || otherLayer == 1) {
            if (lbl_8053AD20[player] == 1) {
                if (lbl_8053AD24[player] == 1) {
                    if (client->mKeys >> 24 == 3) {
                        lbl_8053AD20[player] = 0;
                    } else {
                        lbl_8053AD1C[player]++;
                        if (lbl_8053AD1C[player] > 40) {
                            lbl_8053AD14[player] = lbl_8053AD0C[player];
                            lbl_8053AD0C[player] = 0;
                            lbl_8053AD24[player] = 0;
                            lbl_8053AD28[player] = 1;
                            record->mKind = 10;
                            fn_803085A4(player);
                            s32 count = lbl_80541618[lbl_8053AD08] - 2;
                            s32 i;
                            for (i = 0; i < count; i++) {
                                u32 w = lbl_805071E8[player][lbl_8053AD0C[player]] |
                                        lbl_805071E8[player][lbl_8053AD0C[player] + 1] << 16;
                                lbl_8053AD0C[player] += 2;
                                record->mSprites[i].mWord = swapBytes(w);
                                if (lbl_8053AD0C[player] >= lbl_8053AD14[player]) {
                                    lbl_8053AD24[player] = 1;
                                    lbl_8053AD1C[player] = 0;
                                    record->mKind = 12;
                                    i++;
                                    break;
                                }
                            }
                            for (; i < count; i++) {
                                record->mSprites[i].mWord = 0;
                            }
                            u8 sum = 0;
                            u8* bytes = (u8*)&record->mSprites[0];
                            s32 size = lbl_80541618[lbl_8053AD08] * 4;
                            for (s32 k = 8; k < size; k++) {
                                sum += *bytes++;
                            }
                            record->field_0x0 = sum;
                        } else {
                            fn_8030801C(player);
                        }
                    }
                } else if (lbl_8053AD28[player] == 1) {
                    record->mKind = 11;
                    fn_803085A4(player);
                    s32 count = lbl_80541618[lbl_8053AD08] - 2;
                    s32 i;
                    for (i = 0; i < count; i++) {
                        u32 w = lbl_805071E8[player][lbl_8053AD0C[player]] |
                                lbl_805071E8[player][lbl_8053AD0C[player] + 1] << 16;
                        lbl_8053AD0C[player] += 2;
                        record->mSprites[i].mWord = swapBytes(w);
                        if (lbl_8053AD0C[player] >= lbl_8053AD14[player]) {
                            lbl_8053AD24[player] = 1;
                            lbl_8053AD1C[player] = 0;
                            record->mKind = 12;
                            i++;
                            break;
                        }
                    }
                    for (; i < count; i++) {
                        record->mSprites[i].mWord = 0;
                    }
                    u8 sum = 0;
                    u8* bytes = (u8*)&record->mSprites[0];
                    s32 size = lbl_80541618[lbl_8053AD08] * 4;
                    for (s32 k = 8; k < size; k++) {
                        sum += *bytes++;
                    }
                    record->field_0x0 = sum;
                } else {
                    fn_80307B58(player, false);
                }
            } else if (fn_8037F1E4(player) == 3 && isPlayerBitSet(gGameRoot_8053AAF8, player)) {
                lbl_8053AD20[player] = 1;
                lbl_8053AD24[player] = 0;
                lbl_8053AD28[player] = 0;
                lbl_8053AD0C[player] = 0;
                fn_80307B58(player, true);
                lbl_8053AD04[player] = 0;
            }
            if (lbl_8053AD20[player] == 0) {
                if (lbl_8053ACD0 && isPlayerBitSet(gGameRoot_8053AAF8, player)) {
                    fn_80306A94(player);
                } else {
                    fn_8030801C(player);
                }
            }
            if (isPlayerBitSet(gGameRoot_8053AAF8, player)) {
                gGameRoot2_8053AB10->field_0x20->fn_802F53F4(player, record);
            }
        } else if (entry->field_0x10c != 0) {
            fn_8030801C(player);
            if (client->mKeys >> 24 == 2) {
                if (entry->mAddress == 0xFFFFFFFF) {
                    lbl_8053ACD4[player] = 0;
                } else if (entry->mAddress == 0x02033000) {
                    lbl_8053ACE0[player] = 1;
                }
                entry->mUsed = 0;
                entry->field_0x119 = 0;
                entry->field_0x104 = 0;
                entry->mName[0] = 0;
                s32 tail = lbl_80504A28[player];
                do {
                    lbl_80504A18[player]++;
                    if (lbl_80504A18[player] >= 32) {
                        lbl_80504A18[player] = 0;
                    }
                } while (lbl_80504A18[player] != tail && lbl_804FBC18[player][lbl_80504A18[player]].mUsed == 0);
            } else {
                entry->field_0x10c--;
                if (entry->field_0x10c == 0) {
                    entry->field_0x119 = 0;
                    entry->field_0x104 = 0;
                }
            }
        } else if (entry->field_0x119 == 1) {
            record->mKind = 7;
            fn_803085A4(player);
            for (s32 i = 0; i < lbl_80541618[lbl_8053AD08] - 2; i++) {
                record->mSprites[i].mWord = lbl_804DBC18[player][entry->field_0x104++];
                if (entry->field_0x104 >= entry->mWords) {
                    record->mKind = 8;
                    entry->field_0x10c = 60;
                    break;
                }
            }
            u8 sum = 0;
            u8* bytes = (u8*)&record->mSprites[0];
            s32 size = lbl_80541618[lbl_8053AD08] * 4;
            for (s32 k = 8; k < size; k++) {
                sum += *bytes++;
            }
            record->field_0x0 = sum;
        } else {
            record->mKind = 6;
            fn_803085A4(player);
            u16* src;
            if (entry->mAddress == 0x0600C000 || entry->mAddress == 0x0600E000) {
                src = (u16*)fn_80403328(entry->mName);
            } else {
                src = (u16*)gGameRoot_8053AAF8->field_0xa4->getResource('????', entry->mName);
            }
            u32* dst = lbl_804DBC18[player];
            for (s32 i = 0; i < entry->mWords; i++) {
                *dst++ = src[0] << 16 | src[1];
                src += 2;
            }
            record->mSprites[0].mWord = swapBytes(entry->field_0x11a);
            record->mSprites[1].mWord = swapBytes(entry->mAddress);
            u8 sum = 0;
            u8* bytes = (u8*)&record->mSprites[0];
            s32 size = lbl_80541618[lbl_8053AD08] * 4;
            for (s32 k = 8; k < size; k++) {
                sum += *bytes++;
            }
            entry->field_0x104 = 0;
            record->field_0x0 = sum;
            entry->field_0x119 = 1;
        }
        lbl_80506D48[player] = (lbl_80506D48[player] + 1) & 3;
        if (lbl_8053AD20[player] == 0) {
            lbl_8053AD04[player] = lbl_8053AD30->fn_80309A38(getPlayerInputSlot_8030AAF4(player));
        }
    }
    fn_80304D64();
}

// FIXME: 98.47 %, regalloc-only; behaviour clean.
// Regalloc only: the original keeps the list bases in r6 / r7 and the element in
// r8 (ours r7 / r8 and r6).
// 0x80304D64
void fn_80304D64() {
    for (int player = 0; player < 4; player++) {
        for (int i = 0; i < 16; i++) {
            if (lbl_80504A38[player][i].mRemove == 1) {
                lbl_80504A38[player][i].mActive = 0;
                lbl_80504A38[player][i].mRemove = 0;
                lbl_80504A38[player][i].mSerial = -1;
            } else if (lbl_80504A38[player][i].mActive == 1 && lbl_80504A38[player][i].mTimer < 5) {
                lbl_80504A38[player][i].mTimer++;
            }
        }
        for (int i = 0; i < 40; i++) {
            if (lbl_80505438[player][i].mRemove == 1 && lbl_80505438[player][i].mTimer == 0) {
                lbl_80505438[player][i].mActive = 0;
                lbl_80505438[player][i].mRemove = 0;
                lbl_80505438[player][i].mSerial = -1;
                lbl_80505438[player][i].field_0xc = -1;
            } else if (lbl_80505438[player][i].mRemove == 1) {
                lbl_80505438[player][i].field_0xc = -1;
                lbl_80505438[player][i].mTimer = 0;
            } else if (lbl_80505438[player][i].mActive == 1 && lbl_80505438[player][i].mTimer < 5) {
                lbl_80505438[player][i].mTimer++;
            }
        }
    }
}

// Queues the file `name` for the player's GBA at `address`.
// 0x80304C10
bool fn_80304C10(s32 player, const char* name, u32 address, u8 arg3, s32 arg4) {
    if (name == NULL) {
        return false;
    }
    if (!isLinked(player)) {
        return false;
    }
    Transfer_8030361C* entry = &lbl_804FBC18[player][lbl_80504A28[player]];
    entry->field_0x104 = 0;
    s32 size;
    if (address == 0x0600C000 || address == 0x0600E000) {
        size = fn_80403284(name);
    } else {
        size = gGameRoot_8053AAF8->field_0xa4->getResSize(gGameRoot_8053AAF8->field_0xa4->getResource('????', name));
    }
    entry->mWords = (size + 3) / 4;
    entry->mAddress = address;
    entry->field_0x119 = 0;
    entry->field_0x11a = arg3;
    entry->field_0x110 = arg4;
    entry->mUsed = 1;
    entry->field_0x10c = 0;
    strcpy(entry->mName, name);
    lbl_80504A28[player]++;
    if (lbl_80504A28[player] >= 32) {
        lbl_80504A28[player] = 0;
    }
    return true;
}

// The same for fn_802D8228's name, at 0x0600E000.
// 0x80304B00
bool fn_80304B00(s32 layer, s32 index) {
    s32 player = fn_8022F340(layer);
    const char* name = getGameView_802D6A9C()->fn_802D8228(index);
    if (name == NULL) {
        return false;
    }
    if (!isLinked(player)) {
        return false;
    }
    Transfer_8030361C* entry = &lbl_804FBC18[player][lbl_80504A28[player]];
    entry->field_0x104 = 0;
    entry->mWords = (s32)(fn_80403284(name) + 3) / 4;
    entry->mAddress = 0x0600E000;
    entry->field_0x119 = 0;
    entry->field_0x11a = 1;
    entry->field_0x110 = index;
    entry->mUsed = 1;
    entry->field_0x10c = 0;
    strcpy(entry->mName, name);
    lbl_80504A28[player]++;
    if (lbl_80504A28[player] >= 32) {
        lbl_80504A28[player] = 0;
    }
    return true;
}

// Queues the stage file `layer`'s view names for index `index` (fn_802D8128)
// for the GBA at 0x0600C000.
// 0x803049F0
bool fn_803049F0(s32 layer, s32 index) {
    s32 player = fn_8022F340(layer);
    const char* name = getGameView_802D6A9C()->fn_802D8128(index);
    if (name == NULL) {
        return false;
    }
    if (!isLinked(player)) {
        return false;
    }
    Transfer_8030361C* entry = &lbl_804FBC18[player][lbl_80504A28[player]];
    entry->field_0x104 = 0;
    entry->mWords = (s32)(fn_80403284(name) + 3) / 4;
    entry->mAddress = 0x0600C000;
    entry->field_0x119 = 0;
    entry->field_0x11a = 1;
    entry->field_0x110 = index;
    entry->mUsed = 1;
    entry->field_0x10c = 0;
    strcpy(entry->mName, name);
    lbl_80504A28[player]++;
    if (lbl_80504A28[player] >= 32) {
        lbl_80504A28[player] = 0;
    }
    return true;
}

// Resets the player's GBA link state and transfer queue, then queues the
// files of the GBA program.
// Prologue only: the zero for the global resets gets its own register instead of the loop counter's.
// 0x80303CE4
// FIXME: 98.46 %, missing/extra + reloc-name; behaviour clean.
void fn_80303CE4(s32 player) {
    lbl_80506D38[player] = 0;
    lbl_8053ACE0[player] = 0;
    lbl_8053ACE4[player] = 0;
    lbl_8053ACD4[player] = 1;
    lbl_80506D88[player] = 0;
    lbl_80506D98[player] = 0;
    lbl_8053ACE8[player] = 0;
    lbl_8053ACEC[player] = 0;
    lbl_8053ACF0[player] = 0;
    lbl_8053ACF8[player] = 0;
    lbl_8053ACFC[player] = 0;
    lbl_80504A18[player] = 0;
    lbl_80504A28[player] = 0;
    Transfer_8030361C* queue = lbl_804FBC18[player];
    for (int i = 0; i < 32; i++) {
        Transfer_8030361C& t = queue[i];
        t.mUsed = 0;
        t.field_0x119 = 0;
        t.field_0x104 = 0;
        t.mWords = 0;
        t.mAddress = 0;
        t.mIndex = i;
        t.field_0x10c = 0;
        t.mName[0] = 0;
    }
    for (int i = 0; i < 0x2000; i++) {
        lbl_804DBC18[player][i] = 0xCDCDCDCD;
    }
    fn_80304C10(player, "end_code_scl.bin", 0xAAAAAAAA, 0, 0);
    fn_80304C10(player, "sound_data.szs", 0x02033000, 1, 0);
    fn_80304C10(player, "gb_ch_obj_base_sch.szs", 0x06010000, 1, 0);
    fn_80304C10(player, "gb_cl_obj_base_scl.szs", 0x05000201, 1, 0);
    fn_80304C10(player, "player_sch.szs", 0x02034000, 1, 0);
    fn_80304C10(player, "bg_a_spl.szs", 0x0203C000, 1, 0);
    fn_80304C10(player, "player_sob.szs", 0x02010000, 1, 0);
    fn_80304C10(player, "enemy_both_sob.szs", 0x02021000, 1, 0);
    fn_80304C10(player, "enemy_both2_sob.szs", 0x02018E00, 1, 0);
    if (fn_801365A8() == 2) {
        fn_80304C10(player, "moji_japan_test_sch.szs", 0x02038000, 1, 0);
    } else if (fn_801365A8() == 0 || fn_801365A8() == 1) {
        fn_80304C10(player, "moji_english_sch.szs", 0x02038000, 1, 0);
    }
    fn_80304C10(player, "end_code_scl.bin", 0xFFFFFFFF, 0, 0);
}

// 0x80303870
// FIXME: 98.91 %, regalloc-only; behaviour clean.
void fn_80303870(s32 player) {
    char name[32];
    char name2[32];
    const char* str = getGameView_802D6A9C()->fn_802D9C4C();
    if (str[0] != 'c') {
        sprintf(name, "bg_%s_sch.szs", str);
        fn_80304C10(player, name, 0x6000000, 1, 0);
    } else {
        fn_80304C10(player, "bg_c2_sch.szs", 0x6000000, 1, 0);
    }
    sprintf(name2, "bg_%s_gb_scl.szs", str);
    fn_80304C10(player, name2, 0x5000001, 1, 0);
    sprintf(name, "gb_ch_obj_tr_sch%s.szs", getGameView_802D6A9C()->fn_802D9C1C());
    fn_80304C10(player, name, 0x6017000, 1, 0);
    lbl_8053ACE4[player] = 1;
}

// 0x8030381C
u8 fn_8030381C(s32 layer) {
    s32 player = fn_8022F340(layer);
    if (isLinked(player)) {
        return lbl_8053ACE0[player];
    }
    return 0;
}

// 0x803037D4
void fn_803037D4(s32 layer, u32 value) {
    s32 player = fn_8022F340(layer);
    if (value > lbl_80506D38[player]) {
        lbl_80506D38[player] = value;
    }
}

// FIXME: 99.39 %, regalloc-only; behaviour clean.
// Regalloc only: the busy flag is built in r0 and copied to r3 by the clrlwi.
// the original builds it in r3 and tests it into r0.
// 0x80303750
u8 fn_80303750(s32 layer) {
    s32 player = fn_8022F340(layer);
    u8 result = isLinked(player);
    if (result) {
        result = isQueueBusy(player);
        if (!result) {
            result = lbl_8053ACD4[player];
        }
    }
    return result;
}

// 0x8030361C
void fn_8030361C() {
    for (s32 player = 0; player < 4; player++) {
        Transfer_8030361C* entry = &lbl_804FBC18[player][lbl_80504A18[player]];
        while (entry != &lbl_804FBC18[player][lbl_80504A28[player]]) {
            if (entry->mUsed == 1 && entry->field_0x104 == 0) {
                u32 address = entry->mAddress;
                if (address == 0x0600C000 || address == 0x0600E000 || address == 0x06000000 ||
                    address == 0x05000001 || address == 0x06017000)
                {
                    entry->mUsed = 0;
                    entry->field_0x119 = 0;
                    entry->field_0x104 = 0;
                    entry->mName[0] = 0;
                    if (entry->mIndex == lbl_80504A18[player]) {
                        do {
                            lbl_80504A18[player]++;
                            if (lbl_80504A18[player] >= 32) {
                                lbl_80504A18[player] = 0;
                            }
                        } while (lbl_80504A18[player] != lbl_80504A28[player] &&
                                 lbl_804FBC18[player][lbl_80504A18[player]].mUsed == 0);
                    }
                }
            }
            if (entry->mIndex < 31) {
                entry++;
            } else {
                entry = lbl_804FBC18[player];
            }
        }
        lbl_8053AD20[player] = 0;
    }
}

// Draws one GBA sprite word on the map in the resource its kind selects, with a
// scale (or rotation about y by `scale`) chosen by its flag bits.
// FIXME: 94.75 %, missing/extra + reloc-name; behaviour drift.
void fn_80302F9C(const GbaSprite_80506F88* sprite, f32 scale, u8 flag, bool set) {
    s32 x = sprite->f.field_0x3_0 << 8 | sprite->f.field_0x2;
    s32 y = sprite->f.field_0x1;
    if (x == 0x180 || y == 0xE0) {
        return;
    }
    if (x >= 0x180) {
        x |= -0x100;
    }
    if (y >= 0xE0) {
        y |= -0x100;
    }
    s32 id = sprite->f.field_0x0 | sprite->f.field_0x3_1 << 8;
    JGeometry::TVec3f pos((f32)x, (f32)y, 0.0f);
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x26 = 0;
    params.field_0x2c = 0;
    params.field_0x2d = 0;
    if (!flag) {
        params.field_0x34 = 2;
    }
    params.field_0x1f = 0;
    params.field_0x38 = 0;
    if (sprite->f.mKind == 2 && id >= 0x200) {
        id &= 0x1FF;
        if (sprite->f.field_0x3_4 == 1) {
            PSMTXScale(mtx, 2.0f, 2.0f, -2.0f);
        } else {
            PSMTXScale(mtx, 1.0f, 1.0f, -1.0f);
        }
    } else if (sprite->f.field_0x3_3 == 1) {
        PSMTXScale(mtx, 1.0f, 1.0f, 1.0f);
        JGeometry::TVec3f rot(0.0f, scale, 0.0f);
        if (sprite->f.field_0x3_4 == 1) {
            setRotScaleMtx(mtx, rot, 1.5f);
        } else {
            setRotScaleMtx(mtx, rot, 0.75f);
        }
        for (s32 i = 0; i < 16; i++) {
            params.field_0x0[i] = 7;
        }
    } else if (sprite->f.field_0x3_4 == 1 || (sprite->f.mKind == 2 && id >= 0x39 && id < 0x45)) {
        PSMTXScale(mtx, 2.0f, 2.0f, 2.0f);
    } else {
        PSMTXScale(mtx, 1.0f, 1.0f, 1.0f);
    }
    params.mMtx = mtx;
    if (set) {
        params.field_0x22 = 2;
    } else {
        params.field_0x22 = 1;
    }
    if (sprite->f.mKind < 2) {
        if ((id >= 0x37 && id < 0x41) || (id >= 0x3CD && id < 0x3CE)) {
            params.field_0x10.a = 0xA0;
        } else if (id == 0x2A8 || id == 0x2A9) {
            id -= 3;
        }
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
    } else if (sprite->f.mKind == 2) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
    } else {
        if (sprite->f.mKind & 4) {
            params.field_0x0[7] = (sprite->f.mKind & 3) + 12;
        }
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
    }
}

// Draws one GBA sprite word on the map: the "SOB " frame of its 10-bit id at
// its (sign-extended 9 / 8-bit) x, y, tinted per player and scaled by the
// player's lbl_8053ACFC count (or lbl_80506DB8's flag).
// FIXME(shotgun): 99.59 %, regalloc-only + reloc-name; behaviour clean.
// (control flow of the early return, the 16 tint stores)
void fn_80302C34(const GbaSprite_80506F88* sprite, s32 record, s32 player, u8 mask, u8 flag) {
    SpriteParams_801F5AE0 params;
    s32 x = sprite->f.field_0x2 | sprite->f.field_0x3_0 << 8;
    s32 y = sprite->f.field_0x1;
    if (x != 0x180 && y != 0xE0 && lbl_8053ACFC[player] < 0x11) {
        if (x >= 0x180) {
            x |= -0x100;
        }
        if (y >= 0xE0) {
            y |= -0x100;
        }
        s32 id = sprite->f.field_0x0 | sprite->f.field_0x3_1 << 8;
        if (lbl_80506DC8[record][player] != 0) {
            params.field_0x10.a = 0xA0;
        }
        if ((mask >> player) & 1) {
            for (int i = 0; i < 16; i++) {
                params.field_0x0[i] = 7;
            }
        } else {
            params.field_0x0[7] = player + 0xC;
        }
        if (fn_80237060(player) >= 2) {
            params.field_0x0[10] = 6;
        }
        JGeometry::TVec3f pos((f32)x, (f32)y, 0.0f);
        Mtx scale;
        if (lbl_8053ACFC[player] != 0) {
            PSMTXScale(scale, 1.0f - 0.75f * lbl_8053ACFC[player] * 0.0625f, 1.0f,
                       lbl_8053ACFC[player] * 0.0625f + 1.0f);
        } else if (lbl_80506DB8[record][player] == 1) {
            PSMTXScale(scale, 1.0f, 1.0f, 0.5f);
        } else {
            PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
        }
        params.field_0x2c = 0;
        params.field_0x26 = 0;
        params.field_0x2d = 0;
        if (!flag) {
            params.field_0x34 = 2;
        }
        params.field_0x1f = 0;
        params.field_0x38 = 0;
        params.mMtx = scale;
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
    }
}

// The draw order key of mark `i` of a record: its slot's sprite kind when it is set like the
// player's own mark (lbl_80506DD8), -1 otherwise.
static inline s32 markKey(const cls_804AA858Sub11C* record, s32 player, s32 i) {
    s32 own = lbl_80506DD8[player];
    if ((record->mMask >> i & 1) != (record->mMask >> own & 1)) {
        return -1;
    }
    return lbl_80506DA8[player][i];
}

static inline s32 markKey2(s32 mask, s32 player, s32 i) {
    if ((mask >> i & 1) != (mask >> lbl_80506DD8[player] & 1)) {
        return -1;
    }
    return lbl_80506DA8[player][i];
}

// A sprite word that is off screen (x = 0x180).
static inline bool isHiddenSprite(const GbaSprite_80506F88& sprite) {
    return sprite.f.field_0x3_0 == 1 && sprite.f.field_0x2 == 0x80;
}

// Variant of fn_803022C4.
// 0x8030275C
// FIXME: 81.15 %, missing/extra; behaviour drift.
void fn_8030275C(cls_804AA858Sub11C* record, s32 player, u8 flag, bool set) {
    s32 order[4];
    s32 mask = record->mMask;
    for (s32 i = 0; i < 4; i++) {
        order[i] = i;
    }
    for (s32 i = 3; i > 0; i--) {
        for (s32 j = 0; j < i; j++) {
            s32 a = order[j];
            s32 b = order[j + 1];
            if (markKey2(mask, player, a) < markKey2(mask, player, b) ||
                (markKey2(mask, player, a) == markKey2(mask, player, b) &&
                 record->mEntries[a].mY < record->mEntries[b].mY))
            {
                order[j] = b;
                order[j + 1] = a;
            }
        }
    }
        s32 i28 = 18;
    for (; i28 >= 0; i28--) {
        if (!(lbl_80506F88[player][0][i28].f.field_0x3_0 == 1 && lbl_80506F88[player][0][i28].f.field_0x2 == 0x80)) {
            break;
        }
    }
    s32 i27 = 18;
    for (; i27 >= 0; i27--) {
        if (!(lbl_80506F88[player][1][i27].f.field_0x3_0 == 1 && lbl_80506F88[player][1][i27].f.field_0x2 == 0x80)) {
            break;
        }
    }
    if (record->mValid <= 1) {
        set = true;
    }
    s32 n = 3;
    while (i28 >= 0 || i27 >= 0 || n >= 0) {
        s32 keyMark = n >= 0 ? markKey(record, player, order[n]) : 0xFF;
        s32 keyA = i28 >= 0 ? fn_803074A4(lbl_80506F88[player][0][i28].f.mKind, spriteId(lbl_80506F88[player][0][i28])) : 0xFF;
        s32 keyB = i27 >= 0 ? fn_803074A4(lbl_80506F88[player][1][i27].f.mKind, spriteId(lbl_80506F88[player][1][i27])) : 0xFF;
        if (i27 >= 0 && (n < 0 || keyB < keyMark ||
                         (keyB == keyMark && record->mEntries[order[n]].mY >= lbl_80506F88[player][1][i27].f.field_0x1)) &&
            (i28 < 0 || keyA <= keyB ||
             (keyA == keyB && lbl_80506F88[player][0][i28].f.field_0x1 >= lbl_80506F88[player][1][i27].f.field_0x1)))
        {
            if (keyB >= 2) {
                i27 = 0;
            } else {
                fn_80302F9C(&lbl_80506F88[player][1][i27], lbl_80506D78[player], flag, set);
            }
            i27--;
            while (i27 >= 0 && lbl_80506F88[player][1][i27].f.field_0x3_0 == 1 && lbl_80506F88[player][1][i27].f.field_0x2 == 0x80) {
                i27--;
            }
        } else if (i28 >= 0 && (n < 0 || keyA < keyMark ||
                                (keyA == keyMark && record->mEntries[order[n]].mY >= lbl_80506F88[player][0][i28].f.field_0x1)))
        {
            if (keyA >= 2) {
                i28 = 0;
            } else {
                fn_80302F9C(&lbl_80506F88[player][0][i28], lbl_80506D78[player], flag, set);
            }
            i28--;
            while (i28 >= 0 && lbl_80506F88[player][0][i28].f.field_0x3_0 == 1 && lbl_80506F88[player][0][i28].f.field_0x2 == 0x80) {
                i28--;
            }
        } else {
            if (markKey(record, player, order[n]) >= 2) {
                n = 0;
            } else {
                fn_80302C34(&record->mSprites[order[n]], player, order[n], record->mMask, flag);
            }
            n--;
        }
    }
}

// Draws a record's marks and the player's two sprite lists (the last visible sprite of each
// first) in order of their kinds (fn_803074A4), each list from its end towards its start.
// 0x803022C4
// FIXME: 68.41 %, missing/extra; behaviour drift.
void fn_803022C4(cls_804AA858Sub11C* record, s32 player, u8 flag, bool set) {
    s32 order[4];
    for (s32 i = 0; i < 4; i++) {
        order[i] = i;
    }
    for (s32 i = 3; i > 0; i--) {
        for (s32 j = 0; j < i; j++) {
            s32 a = order[j];
            s32 b = order[j + 1];
            if (markKey(record, player, a) < markKey(record, player, b) ||
                (markKey(record, player, a) == markKey(record, player, b) &&
                 record->mEntries[a].mY < record->mEntries[b].mY))
            {
                order[j] = b;
                order[j + 1] = a;
            }
        }
    }
    GbaSprite_80506F88* list = lbl_80506F88[player][0];
    s32 i28 = 18;
    while (i28 >= 0 && isHiddenSprite(list[i28])) {
        i28--;
    }
    s32 i27 = 18;
    while (i27 >= 0 && isHiddenSprite(list[19 + i27])) {
        i27--;
    }
    if (record->mValid <= 1) {
        set = true;
    }
    s32 n = 3;
    while (i28 >= 0 || i27 >= 0 || n >= 0) {
        s32 keyMark = n >= 0 ? markKey(record, player, order[n]) : 0xFF;
        s32 keyA = i28 >= 0 ? fn_803074A4(list[i28].f.mKind, spriteId(list[i28])) : 0xFF;
        s32 keyB = i27 >= 0 ? fn_803074A4(list[19 + i27].f.mKind, spriteId(list[19 + i27])) : 0xFF;
        if (i27 >= 0 && (n < 0 || keyB < keyMark ||
                         (keyB == keyMark && record->mEntries[order[n]].mY >= list[19 + i27].f.field_0x1)) &&
            (i28 < 0 || keyA <= keyB ||
             (keyA == keyB && list[i28].f.field_0x1 >= list[19 + i27].f.field_0x1)))
        {
            if (keyB >= 2) {
                fn_80302F9C(&list[19 + i27], lbl_80506D78[player], flag, set);
            }
            i27--;
            while (i27 >= 0 && isHiddenSprite(list[19 + i27])) {
                i27--;
            }
        } else if (i28 >= 0 && (n < 0 || keyA < keyMark ||
                                (keyA == keyMark && record->mEntries[order[n]].mY >= list[i28].f.field_0x1)))
        {
            if (keyA >= 2) {
                fn_80302F9C(&list[i28], lbl_80506D78[player], flag, set);
            }
            i28--;
            while (i28 >= 0 && isHiddenSprite(list[i28])) {
                i28--;
            }
        } else {
            if (markKey(record, player, order[n]) >= 2) {
                fn_80302C34(&record->mSprites[order[n]], player, order[n], record->mMask, flag);
            }
            n--;
        }
    }
}

// Works out the link state lbl_8053ACE8[slot] (1-3, 0 for none) of a
// connected player from the two layers a and b (8 counts as set, above 8
// asks the view's fn_802D7804), stores them and the timer of `mode`.
void fn_803020C8(s32 player, s32 a, s32 b, u8 mode) {
    s32 slot = fn_8022F340(player);
    if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << slot)) == 0) {
        return;
    }
    if (a < 8) {
        if (b < 8) {
            lbl_8053ACE8[slot] = 0;
        } else if (b == 8 || getGameView_802D6A9C()->fn_802D7804(b, 1)) {
            lbl_8053ACE8[slot] = 1;
        } else {
            lbl_8053ACE8[slot] = 3;
        }
    } else if (a == 8 || getGameView_802D6A9C()->fn_802D7804(a, 1)) {
        if (b < 8) {
            lbl_8053ACE8[slot] = 3;
            lbl_8053ACF8[player] = 1;
        } else if (b == 8 || getGameView_802D6A9C()->fn_802D7804(b, 1)) {
            lbl_8053ACE8[slot] = 2;
        } else {
            lbl_8053ACE8[slot] = 2;
            lbl_8053ACF8[player] = 1;
        }
    } else {
        if (b < 8) {
            lbl_8053ACE8[slot] = 3;
        } else if (b == 8 || getGameView_802D6A9C()->fn_802D7804(b, 1)) {
            lbl_8053ACE8[slot] = 1;
        } else {
            lbl_8053ACE8[slot] = 2;
        }
    }
    lbl_80506D88[slot] = a;
    lbl_80506D98[slot] = b;
    switch (mode) {
    case 0:
        lbl_8053ACF4[slot] = 0;
        break;
    case 2:
        lbl_8053ACF4[slot] = 0x5A;
        break;
    case 1:
    default:
        lbl_8053ACF4[slot] = 0x14;
        break;
    }
}

// Resets the player's link state (or marks it 0xFF while lbl_8053ACF0 is
// set), then does the same for the player fn_8023ECF0 names, while there is
// one that is its own fn_8022EA0C.
void fn_80301D54(s32 player) {
    s32 slot = fn_8022F340(player);
    if (lbl_8053ACF0[slot] == 0) {
        lbl_8053ACE8[slot] = 0;
        lbl_8053ACEC[slot] = 0;
        lbl_8053ACF4[slot] = 0;
    } else {
        lbl_8053ACEC[slot] = 0xFF;
    }
    lbl_8053ACF8[player] = 0;
    if (fn_8023ED8C(player)) {
        s32 other = fn_8023ECF0(player);
        if (other == fn_8022EA0C(other)) {
            fn_80301D54(other);
        }
    }
}

// 0x80301D30
u8 fn_80301D30(s32 player) {
    if (lbl_8053ACEC[player] > 1) {
        return lbl_8053ACE8[player];
    }
    return 0;
}

// 0x80301CD0
bool fn_80301CD0(s32 layer) {
    s32 player = fn_8022F340(layer);
    if (lbl_8053ACE8[player] != 0 &&
        (lbl_8053ACEC[player] < 2 || (lbl_8053ACEC[player] == 2 && lbl_8053ACF0[player] < 0x10)))
    {
        return true;
    }
    return false;
}
