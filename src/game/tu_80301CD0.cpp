#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "JSystem/JKernel/JKRArchive.h"
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

// The 4-byte object of tu_803FF2C4 (GameState field_0x60); named after its
// vtable. Its state lives in the file statics below.
class cls_804AA8B8 {
public:
    cls_804AA8B8();
    /* 0x00 */ virtual ~cls_804AA8B8();
    void fn_80308E90();
};

u32 lbl_804DBC18[4][0x2000];
Transfer_8030361C lbl_804FBC18[4][32];
s32 lbl_80504A18[4]; // queue head per player
s32 lbl_80504A28[4]; // queue tail per player
cls_803099FC lbl_80504A38[4][16];
cls_803099FC lbl_80505438[4][40];
u32 lbl_80506D38[4];
s32 lbl_80506D48[4][4];
s32 lbl_80506D88[4];
s32 lbl_80506D98[4];
u8 lbl_80506DA8[4][4];
u8 lbl_80506DB8[4][4];
u8 lbl_80506DC8[4][4];
s32 lbl_80506DD8[4];
cls_804AA858Sub11C lbl_80506DE8[4];
u8 lbl_80506F88[4][0x98];
s16 lbl_805071E8[4][0x80];

u8 lbl_8053ACD0;
u8 lbl_8053ACD4[4];
u8 lbl_8053ACD8[4];
u8 lbl_8053ACDC[4];
u8 lbl_8053ACE0[4];
u8 lbl_8053ACE4[4];
u8 lbl_8053ACE8[4];
u8 lbl_8053ACEC[4];
u8 lbl_8053ACF0[4];
s32 lbl_8053ACF4;
u8 lbl_8053ACF8[4];
u8 lbl_8053ACFC[4];
u8 lbl_8053AD00[4];
u8 lbl_8053AD04[4];
u8 lbl_8053AD08;
s16 lbl_8053AD0C[4];
s16 lbl_8053AD14[4];
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

// 0x80301D30
u8 fn_80301D30(s32 player) {
    if (lbl_8053ACEC[player] > 1) {
        return lbl_8053ACE8[player];
    }
    return 0;
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

// FIXME: 99.39. Regalloc only: the busy flag is built in r0 and copied to r3 by the clrlwi.
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

// 0x803037D4
void fn_803037D4(s32 layer, u32 value) {
    s32 player = fn_8022F340(layer);
    if (value > lbl_80506D38[player]) {
        lbl_80506D38[player] = value;
    }
}

// 0x8030381C
u8 fn_8030381C(s32 layer) {
    s32 player = fn_8022F340(layer);
    if (isLinked(player)) {
        return lbl_8053ACE0[player];
    }
    return 0;
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
        size = lbl_8053AAF8->field_0xa4->getResSize(lbl_8053AAF8->field_0xa4->getResource('????', name));
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

// FIXME: 98.47. Regalloc only: the original keeps the list bases in r6 / r7 and the element in
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
        if (!(lbl_8053AAF8->field_0x82 & (u8)(1 << p))) {
            continue;
        }
        if (player != p && (player >= 0 || layer != fn_8023DE58(slot))) {
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
        if (!(lbl_8053AAF8->field_0x82 & (u8)(1 << p))) {
            continue;
        }
        if (layer != fn_8023DE58(slot)) {
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

// FIXME: 95.36. Regalloc only: the same code as fn_80308D0C, but the original keeps the
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

// 0x80309014
cls_804AA8B8::~cls_804AA8B8() {}

// __sinit_tu_80301CD0_cpp (compiler-generated: the two __construct_array calls for the sprite
// lists and the JAudio pools of GameAudio.h). lbl_804DB798 (0x480, read by fn_80304FB0 here and
// fn_8030A258) is not part of this object: with it first in the .bss, the sinit offsets shift by
// 0x480, so this unit's .bss starts at lbl_804DBC18 and splits.txt gives the object to
// tu_803006E8 (the previous unit).
//
// Not written yet: fn_80301D54, fn_803020C8, fn_803022C4, fn_8030275C, fn_80302C34, fn_80302F9C,
// fn_80303870, fn_80303CE4, fn_80304FB0, fn_803064A8, fn_803066F0, fn_803068D0, fn_80306A94,
// fn_80307B58, fn_8030801C, fn_803085A4 and the constructor fn_8030905C (cls_804AA8B8).
