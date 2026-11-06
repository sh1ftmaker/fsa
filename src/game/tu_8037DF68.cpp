#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameMode.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/JUtility/JUTFont.h"
#include "JSystem/JUtility/JUTVideo.h"

// The window index of a player: the one fn_8022F340 maps it to in some game
// modes, else the player itself.
static inline s32 windowIndex(s32 player) {
    if (isGameModeSet()) {
        s32 mapped = fn_8022F340(player);
        if (mapped >= 0 && mapped < 4) {
            return mapped;
        }
    }
    return player;
}

MessageManager_804AF070* lbl_8053AD78;

// Clears the manager's field_0xe4 when a player of field_0xe5 presses 0x100.
static inline void clearOnPress() {
    for (s32 i = 0; i < 4; i++) {
        if (lbl_8053AD78->field_0xe5 & (1 << i)) {
            if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(i), 0x100) != 0) {
                lbl_8053AD78->field_0xe4 = 0;
            }
        }
    }
}

// Forward declarations.
f32 fn_8037F580(u8 command, s32 value, char* out, s32* len, f32 scale);
u8 fn_8037F1E4(s32 player);
bool fn_8037EC98(s32 player, u16 flags);

cls_804AF32C::~cls_804AF32C() {}

// FIXME: 98.8. One instruction more: the font resource goes through r0 into a saved
// register (mr r0, r3; mr r28, r0) where the original keeps it in r30 directly
// (act_giant_bomb has the same two-mr getResource shape).
MessageManager_804AF070::MessageManager_804AF070() {
    lbl_8053AD78 = this;
    const ResFONT* font = (const ResFONT*)gGameRoot_8053AAF8->field_0xa8->getResource('????', "rock_24_20_4i_e.bfn");
    mFont = new JUTResFont(font, NULL);
    mBmg = (MessageFile_804AF070*)gGameRoot_8053AAF8->field_0xa8->getResource('????', "gc_four_swords_text_e.bmg");
    for (s32 i = 0; i < 4; i++) {
        mWindows[i] = new MessageWindow_804AF490(i);
        field_0xc[i].x = 0.0f;
        field_0xc[i].y = 0.0f;
        field_0xc[i].z = 0.0f;
        field_0x4c[i] = 0;
    }
    field_0xec = 0;
    field_0x80[0] = '\0';
    field_0x40 = -1;
    field_0x3c = -1;
    field_0x7c = 0;
    field_0xe5 = 0;
    field_0x78 = 0.0f;
    field_0x74 = 0.0f;
    field_0x70 = 0.0f;
    field_0x6c = 0.0f;
    field_0xe4 = 0;
    field_0x5c[3] = 0.0f;
    field_0x5c[2] = 0.0f;
    field_0x5c[1] = 0.0f;
    field_0x5c[0] = 0.0f;
    field_0xe6 = 0;
    field_0xe7 = 0;
    field_0xe8 = 0;
    field_0x44 = 0;
    field_0xe9 = 0;
    field_0x48 = 0;
    field_0xea = 0;
    field_0xeb = 0;
    field_0x10c = 0;
    field_0xed = 0;
    field_0x110 = 0;
    field_0xee = 0;
    mTextBox = new J2DTextBox("rock_24_20_4i_e.bfn", field_0x80, 0x64);
}

MessageManager_804AF070::~MessageManager_804AF070() {
    if (lbl_8053AD78 != NULL) {
        for (s32 i = 0; i < 4; i++) {
            if (mWindows[i] != NULL) {
                delete mWindows[i];
                mWindows[i] = NULL;
            }
        }
        delete mFont;
        mFont = NULL;
        delete mTextBox;
        mTextBox = NULL;
        lbl_8053AD78 = NULL;
    }
}

void fn_8038D870() {
    for (s32 i = 0; i < 4; i++) {
        lbl_8053AD78->mWindows[i]->fn_80391678();
        lbl_8053AD78->mWindows[i]->field_0x4 = lbl_8053AD78->mBmg;
        lbl_8053AD78->field_0xfc[i] = -1;
        lbl_8053AD78->field_0xf0[i] = 0;
    }
    lbl_8053AD78->field_0xf8 = 0;
    lbl_8053AD78->field_0xe4 = 0;
}

// FIXME: 98.8. Two opcodes: the original loads the constant 0 stored into
// field_0xe4 by the second press loop with li (r30); ours copies the loop
// index register (mr r30, r29).
void fn_8038D570() {
    lbl_8053AD78->field_0xe7 = 0;
    for (s32 i = 0; i < 4; i++) {
        bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i);
        if (takesPart == true) {
            lbl_8053AD78->mWindows[i]->mIndex = i;
            lbl_8053AD78->mWindows[i]->fn_80390EBC();
        }
    }
    if (lbl_8053AD78->field_0xe4 != 0) {
        if (lbl_8053AD78->field_0x7c == 0) {
            clearOnPress();
        } else {
            lbl_8053AD78->field_0x7c--;
            lbl_8053AD78->field_0x70 = lbl_8053AD78->field_0x70 - lbl_8053AD78->field_0x78;
            if (lbl_8053AD78->field_0x7c == 0) {
                lbl_8053AD78->field_0xe4 = 0;
            }
            clearOnPress();
        }
    }
    MessageManager_804AF070* manager = lbl_8053AD78;
    if (manager->field_0xe9 != 0) {
        if (manager->field_0xe9 == 1 && manager->field_0x44 < 0x57E40u) {
            manager->field_0x44++;
        }
        manager = lbl_8053AD78;
        s32 minutes = manager->field_0x44 / 3600;
        s32 seconds = manager->field_0x44 / 60 % 60;
        if (seconds == 0) {
            seconds = 1;
        }
        u32 elapsed = seconds + minutes * 60;
        if (elapsed > manager->field_0x48) {
            if (manager->field_0xeb != 0) {
                if (!getGameView_802D6A9C()->fn_802D800C(manager->field_0xea)) {
                    u8 bit = lbl_8053AD78->field_0xea;
                    getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
                }
            } else if (getGameView_802D6A9C()->fn_802D800C(manager->field_0xea) == true) {
                u8 bit = lbl_8053AD78->field_0xea;
                getGameView_802D6A9C()->fn_802D8050(bit, false, 0xFF);
            }
        }
    }
    if (lbl_8053AD78->field_0xed != 0) {
        for (s32 i = 0; i < gGameRoot_8053AAF8->fn_80138A30(); i++) {
            lbl_8053ACD8[i] = 1;
        }
    }
    lbl_8053AD78->field_0xe8 = 0;
}

// FIXME: 99.3. Regalloc only: the manager pointer and the window index sit in r5 / r4
// in the original, r4 / r5 here.
// Draws the open windows in the order they were opened (field_0xfc), then the
// text box when field_0xe4 is set.
void fn_8038D4A8() {
    for (s32 i = 0; i < 4; i++) {
        s32 index = lbl_8053AD78->field_0xfc[i];
        if (index != -1) {
            bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << index);
            if (takesPart == true) {
                lbl_8053AD78->mWindows[index]->fn_80390E68();
            }
        }
    }
    if (lbl_8053AD78->field_0xe4 != 0) {
        lbl_8053AD78->mTextBox->draw(lbl_8053AD78->field_0x70, lbl_8053AD78->field_0x74);
    }
}

void fn_8038D454() {
    for (s32 i = 0; i < 4; i++) {
        lbl_8053AD78->mWindows[i]->fn_80391678();
    }
}

// FIXME: 99.98. Regalloc only: the second fn_8037EC98 call passes the flags register
// (r29) in the original; ours reuses the option-bits copy (r30).
bool MessageManager_804AF070::fn_8038B8A8(s32 player, s32 actor, s32 value, u32 flags) {
    if (fn_8037F1E4(player) == 0 && !fn_80303750(player) && fn_8022C8EC(player)) {
        u16 bits = flags; // only to match: a halfword copy of the flags parameter
        if (bits & 0x91F) {
            lbl_8053AD78->mWindows[player]->fn_80390E40(value, false);
            lbl_8053AD78->mWindows[windowIndex(player)]->field_0x24 = 1;
            if (bits & 2) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x80;
            } else {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x1;
            }
            if (bits & 8) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x10;
            } else if (bits & 0x10) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x20;
            }
            if (bits & 0x100) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x100;
            }
            return true;
        }
        if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 0x100) != 0 && fn_8037EC98(actor, flags)) {
            lbl_8053AD78->mWindows[player]->fn_80390E40(value, false);
            lbl_8053AD78->mWindows[windowIndex(player)]->field_0x24 = 1;
            return true;
        }
    }
    return false;
}

// FIXME: 99.7. Regalloc only: the flags parameter is r30 and the option bits r31 in the
// original, the other way round here.
bool MessageManager_804AF070::fn_8038A984(s32 player, s32 actor, s32 value, s32 serial, u32 flags) {
    if (fn_8037F1E4(player) == 0 && !fn_80303750(player) && fn_8022C8EC(player)) {
        if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 0x100) != 0 || (u16)flags & 0x19F) {
            u16 bits = flags; // only to match: a halfword copy of the flags parameter
            bool ready = false;
            if (flags & 0x1000) {
                if (fn_8037EC98(actor, flags)) {
                    ready = true;
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x800;
                }
            } else if (fn_8022FCF0(actor, serial) && fn_8037EC98(actor, flags)) {
                fn_8022FC54(actor, serial);
                ready = true;
            }
            if (ready) {
                lbl_8053AD78->mWindows[player]->fn_80390E40(value, false);
                lbl_8053AD78->mWindows[windowIndex(player)]->field_0x24 = 1;
                if (bits & 2) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x80;
                }
                if (bits & 0x80) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x4;
                }
                if (bits & 4) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x8;
                }
                if (bits & 0x100) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x100;
                }
                return true;
            }
        }
    }
    return false;
}

// FIXME: 99.7. Regalloc only: the actor parameter, the option bits and the flags sit in
// r29 / r30 / r28 in the original, r30 / r29 / r29 here.
bool MessageManager_804AF070::fn_8038A42C(s32 player, s32 actor, s32 value, s32 serial, JGeometry::TVec3f pos, u32 flags) {
    if (fn_8037F1E4(player) == 0) {
        if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 0x100) != 0 || (u16)flags & 0x19F) {
            bool ready = false;
            u16 bits = flags; // only to match: a halfword copy of the flags parameter
            if (flags & 0x1000) {
                if (fn_8037EC98(actor, flags)) {
                    ready = true;
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x800;
                }
            } else if (fn_8022FCF0(actor, serial) && fn_8037EC98(actor, flags)) {
                fn_8022FC54(actor, serial);
                ready = true;
            }
            if (ready) {
                s32 plain = bits & 0x40;
                if (plain) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x40;
                } else if (bits & 0x200) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x200;
                } else if (bits & 0x400) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x400;
                }
                lbl_8053AD78->mWindows[player]->fn_80390E40(value, false);
                JGeometry::TVec3f boxPos = pos; // only to match: a local copy of the by-value position
                MessageWindow_804AF490* window = lbl_8053AD78->mWindows[player];
                window->field_0x50 = boxPos;
                window->field_0x30 = serial;
                lbl_8053AD78->mWindows[windowIndex(player)]->field_0x24 = 6;
                lbl_8053AD78->field_0xfc[lbl_8053AD78->field_0xf8] = windowIndex(player);
                lbl_8053AD78->field_0xf8++;
                if (bits & 2) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x80;
                }
                if (bits & 0x80) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x4;
                }
                if (bits & 4) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x8;
                }
                if (bits & 0x100) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x100;
                }
                if (bits & 0x4000) {
                    lbl_8053AD78->mWindows[player]->field_0x28 |= 0x2000;
                }
                if (plain == 0) {
                    gAudioMgr_8053AF80->fn_80458880(0x28, -1);
                }
                return true;
            }
        }
    }
    return false;
}

bool MessageManager_804AF070::fn_80389F78(s32 player, s32 arg, s32 value, JGeometry::TVec3f pos, u32 flags) {
    if (fn_8037F1E4(player) == 0) {
        u16 bits = flags; // only to match: a halfword copy of the flags parameter
        if (bits & 0x11F) {
            s32 plain = bits & 0x40;
            if (plain) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x40;
            } else if (bits & 0x200) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x200;
            } else if (bits & 0x400) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x400;
            }
            lbl_8053AD78->mWindows[player]->fn_80390E40(value, false);
            JGeometry::TVec3f boxPos = pos; // only to match: a local copy of the by-value position
            MessageWindow_804AF490* window = lbl_8053AD78->mWindows[player];
            window->field_0x50 = boxPos;
            window->field_0x30 = -1;
            lbl_8053AD78->mWindows[windowIndex(player)]->field_0x24 = 6;
            lbl_8053AD78->field_0xfc[lbl_8053AD78->field_0xf8] = windowIndex(player);
            lbl_8053AD78->field_0xf8++;
            if (bits & 2) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x80;
            } else {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x1;
            }
            if (bits & 8) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x10;
            } else if (bits & 0x10) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x20;
            }
            if (bits & 0x100) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x100;
            }
            if (bits & 0x4000) {
                lbl_8053AD78->mWindows[player]->field_0x28 |= 0x2000;
            }
            if (plain == 0) {
                gAudioMgr_8053AF80->fn_80458880(0x28, -1);
            }
            return true;
        }
    }
    return false;
}

bool fn_80389BF0(s32 player, s32 value) {
    s32 index = windowIndex(player);
    if (fn_8037F1E4(index) == 0 && !fn_80303750(index)) {
        lbl_8053AD78->mWindows[index]->fn_80390E40(value, false);
        lbl_8053AD78->mWindows[windowIndex(index)]->field_0x24 = 1;
        lbl_8053AD78->mWindows[index]->field_0x28 |= 0x1000;
        lbl_8053AD78->mWindows[index]->field_0x28 |= 0x80;
        lbl_8053AD78->mWindows[index]->field_0x28 |= 0x20;
        return true;
    }
    return false;
}

// FIXME: 97.7. One instruction more than the original (an mr copying the
// loop index before the fn_8022FA80 call) and the registers shifted by one.
bool MessageManager_804AF070::fn_80389964(s32 exclude, u32 msg, u32 flags) {
    u8 ready[4];
    for (int i = 0; i < 4; i++) {
        ready[i] = 0;
        if (i != exclude && (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i))) {
            if (!fn_8022FA80(i) && fn_8037F1E4(i) == 0) {
                if ((flags & 4) || (flags & 2) || msg == 0x265) {
                    if (fn_80230188(i)) {
                        ready[i] = 1;
                        for (int j = 0; j < 4; j++) {
                            if (fn_8022E438(i, j) && !fn_80230188(j)) {
                                ready[i] = 0;
                            }
                        }
                    }
                } else if (fn_802300EC(i)) {
                    ready[i] = 1;
                    for (int j = 0; j < 4; j++) {
                        if (fn_8022E438(i, j) && !fn_802300EC(j)) {
                            ready[i] = 0;
                        }
                    }
                }
            }
        } else {
            ready[i] = 2;
        }
    }
    if (ready[0] && ready[1] && ready[2] && ready[3]) {
        return true;
    }
    return false;
}

// FIXME: 99.3. One instruction fewer: the original copies player into r31 (mr r31, r26)
// before the loop reuses r26 as the index; ours keeps player in place.
void MessageManager_804AF070::fn_803895EC(s32 player, s32 arg, bool a, bool b) {
    s32 mapped = windowIndex(player);
    for (s32 i = 0; i < 4; i++) {
        if (fn_8037F1E4(i) == 9) {
            if (a) {
                if (i == mapped) {
                    fn_8022FB1C(player, arg);
                } else if (fn_8022CAF8(i) == 4) {
                    fn_8022FB1C(fn_8022EA0C(i), arg);
                } else {
                    fn_8022FB1C(i, arg);
                }
            }
            if (b) {
                fn_8023061C(i, false);
            }
            lbl_8053AD78->mWindows[windowIndex(i)]->field_0x24 = 0;
        }
    }
}

void fn_803894BC(s32 player, s32 value) {
    s32 index = windowIndex(player);
    if (value > 9) {
        value = 9;
    }
    lbl_8053AD78->mWindows[index]->field_0x3c = value;
}

void fn_8038938C(s32 player, s32 value) {
    s32 index = windowIndex(player);
    if (value > 9999) {
        value = 9999;
    }
    lbl_8053AD78->mWindows[index]->field_0x3e = value;
}

void fn_80389254(s32 player, s32 value) {
    s32 index = windowIndex(player);
    if (value > 99999) {
        value = 99999;
    }
    lbl_8053AD78->mWindows[index]->field_0x34 = value;
}

void fn_8038911C(s32 player, s32 value) {
    s32 index = windowIndex(player);
    if (value > 99999) {
        value = 99999;
    }
    lbl_8053AD78->mWindows[index]->field_0x38 = value;
}

bool fn_80388EEC(s32 player) {
    s32 index = windowIndex(player);
    if (fn_8037F1E4(index) == 0) {
        return true;
    }
    if (fn_803865B8(player)) {
        return fn_80385960(player);
    }
    return false;
}

bool fn_80388CAC(s32 player, s32 serial) {
    s32 index = windowIndex(player);
    if (fn_8037F1E4(index) == 0) {
        return true;
    }
    if (fn_803865B8(player)) {
        return fn_80384C58(player, serial);
    }
    return false;
}

bool fn_8038470C(s32 player, u32 msg) {
    s32 index = windowIndex(player);
    lbl_8053AD78->mWindows[index]->fn_803913E0();
    u32 mapped = fn_80383EB0(index, msg);
    if (fn_8037F1E4(index) == 5) {
        lbl_8053AD78->mWindows[index]->fn_80390E40(mapped, true);
        lbl_8053AD78->mWindows[windowIndex(index)]->field_0x24 = 1;
        return true;
    }
    if (fn_8037F1E4(index) == 8) {
        lbl_8053AD78->mWindows[index]->fn_80390E40(mapped, true);
        lbl_8053AD78->mWindows[windowIndex(index)]->field_0x24 = 6;
        return true;
    }
    return false;
}

u32 fn_803846BC(u32 msg) {
    MessageInf1_804AF070* inf1 = &lbl_8053AD78->mBmg->inf1;
    u32 i = 0;
    for (; i < inf1->count; i++) {
        if (inf1->entries[i].id == msg) {
            msg = i;
            break;
        }
    }
    if (i >= inf1->count) {
        msg = 0x12B;
    }
    return msg;
}

u16 fn_803846A4(s32 index) {
    return lbl_8053AD78->mBmg->inf1.entries[index].id;
}

// FIXME: 99.1. Regalloc only (r3 / r5 / r6 rotated); the jump table's name waits
// on the unit's .data (jumptable_804AF0CC is the third of six).
const char* fn_80383E2C(u32 index, u8* out) {
    MessageFile_804AF070* bmg = lbl_8053AD78->mBmg;
    MessageInf1_804AF070* inf1 = &bmg->inf1;
    const char* text = &((const char*)inf1 + 8)[inf1->size + inf1->entries[index].textOffset];
    *out = 0;
    if (text[0] == 0x1A && text[2] == 2) {
        switch (text[4]) {
        case 7:
        case 0x10:
        case 0x17:
        case 0x18:
        case 0x1D:
        case 0x20:
        case 0x21:
        case 0x23:
        case 0x2A:
        case 0x2C:
        case 0x30:
        case 0x33:
            *out = text[4];
            return text;
        }
    }
    return NULL;
}

// Stores c at out[len], or in out2 / out3 while the text is in its second /
// third part (when those buffers are given). A macro: the original evaluates c
// in each branch and stores it unconverted.
#define PUT_CHAR(c)                          \
    if (part == 1 && out2 != NULL) {         \
        out2[len] = (c);                     \
    } else if (part == 2 && out3 != NULL) {  \
        out3[len] = (c);                     \
    } else {                                 \
        out[len] = (c);                      \
    }

// FIXME: 95.1. Structurally close (one instruction fewer, registers shifted): the
// text address is summed as (offset + inf1) + 8 + size where the original adds
// size + offset, then 8, then inf1 (as in fn_80383E2C); the loop's first
// character test reuses the condition's extsb. where the original sign-extends
// again; the lead-byte code is loaded into r4 and copied to r31 (the original
// loads it into r31).
// Copies the text of message msg into out (into out2 / out3 for the parts after
// the first two part-break control codes, when given) and returns its width in
// mFont units. Control code 0x1A size 2 ends a part (the width and line count so
// far go to field_0x5c[part] / field_0x5c[part + 2]); 0x1A size 3 writes the digit
// value and the expansion fn_8037F580 makes of its command byte.
f32 getMessageText_803832BC(char* out, u32 msg, s32 value, char* out2, char* out3) {
    MessageFile_804AF070* bmg = lbl_8053AD78->mBmg;
    MessageInf1_804AF070* inf1 = &bmg->inf1;
    s32 len = 0;
    f32 width = 0.0f;
    const char* text = &((const char*)inf1 + 8)[inf1->size + inf1->entries[msg].textOffset];
    s32 pos = 0;
    s32 lines = 1;
    u8 part = 0;
    char c;
    while ((c = text[pos]) != 0) {
        const char* p = &text[pos];
        if (c == '\n') {
            PUT_CHAR(c);
            len++;
            lines++;
            pos++;
        } else if (c == 0x1A) {
            if (p[2] == 2) {
                if (p[4] == 0 || p[4] == 10 || p[4] == 0x12 || p[4] == 0x19) {
                    if (part < 2) {
                        lbl_8053AD78->field_0x5c[part] = width;
                        lbl_8053AD78->field_0x5c[part + 2] = lines - 1;
                        if (part == 0) {
                            if (out2 != NULL) {
                                out[len] = 0;
                                len = 0;
                            }
                        } else if (part == 1) {
                            if (out3 != NULL) {
                                out2[len] = 0;
                                len = 0;
                            }
                        }
                        part++;
                    }
                } else if (p[4] == 1 || p[4] == 9 || p[4] == 0x11 || p[4] == 0x1A) {
                    if (part < 2) {
                        lbl_8053AD78->field_0x5c[part] = width;
                        lbl_8053AD78->field_0x5c[part + 2] = lines - 1;
                        lbl_8053AD78->field_0xe6 = part;
                        if (part == 0) {
                            if (out2 != NULL) {
                                out[len] = 0;
                                len = 0;
                            }
                        } else if (part == 1) {
                            if (out3 != NULL) {
                                out2[len] = 0;
                                len = 0;
                            }
                        }
                        part++;
                    }
                }
            } else if (p[2] == 3) {
                s32 digit = value + '0';
                PUT_CHAR(digit);
                len++;
                width += lbl_8053AD78->mFont->JUTFont::getWidth(digit);
                width += fn_8037F580(p[5], value, out, &len, 1.0f);
            }
            pos += p[1];
        } else {
            PUT_CHAR(c);
            len++;
            u32 code = (u8)*p;
            pos++;
            if (lbl_8053AD78->mFont->isLeadByte(code)) {
                code <<= 8;
                PUT_CHAR(text[pos]);
                code |= text[pos] & 0xFF;
                pos++;
                len++;
            }
            width += lbl_8053AD78->mFont->JUTFont::getWidth(code);
        }
    }
    PUT_CHAR(c);
    return width;
}

#undef PUT_CHAR

// Shows message msg in the manager's text box at (x, y) with a font of
// width x height and the colour color, or, when frames is not 0, scrolling in
// from the right edge of the screen over that many frames (fn_8038D570 moves it).
// skipMask: the players whose 0x100 press closes it (field_0xe5). False while
// a text box is already shown.
bool fn_8037F2FC(u32 msg, s32 frames, u8 skipMask, f32 x, f32 y, f32 width, f32 height,
                 JUtility::TColor color) {
    if (lbl_8053AD78->field_0xe4 == 0) {
        if (lbl_8053AD78->field_0x3c != msg) {
            lbl_8053AD78->field_0x3c = msg;
            char* text = lbl_8053AD78->mTextBox->getStringPtr();
            lbl_8053AD78->field_0x6c = getMessageText_803832BC(text, lbl_8053AD78->field_0x3c, 0, NULL, NULL);
        }
        f32 scale = width / 20.0f;
        f32 textWidth = lbl_8053AD78->field_0x6c * scale;
        f32 distance = textWidth + (s32)JUTVideo::getManager()->getRenderMode()->fbWidth;
        J2DTextBox::TFontSize size;
        size.x = width;
        size.y = height;
        lbl_8053AD78->mTextBox->setFontSize(size);
        lbl_8053AD78->mTextBox->setFontColor(color, color);
        lbl_8053AD78->field_0x7c = frames;
        if (lbl_8053AD78->field_0x7c == 0) {
            lbl_8053AD78->field_0x70 = x - lbl_8053AD78->field_0x6c * scale / 2;
            lbl_8053AD78->field_0x78 = 0.0f;
        } else {
            lbl_8053AD78->field_0x70 = (s32)JUTVideo::getManager()->getRenderMode()->fbWidth;
            lbl_8053AD78->field_0x78 = distance / lbl_8053AD78->field_0x7c;
        }
        lbl_8053AD78->field_0x74 = y - height / 2;
        lbl_8053AD78->field_0xe5 = skipMask;
        lbl_8053AD78->field_0xe4 = 1;
        return true;
    }
    return false;
}

u8 fn_8037F1E4(s32 player) {
    return lbl_8053AD78->mWindows[windowIndex(player)]->field_0x24;
}

u16 fn_8037F0CC(s32 player) {
    return lbl_8053AD78->mWindows[windowIndex(player)]->fn_80390C6C();
}

s32 fn_8037EFB4(s32 player) {
    return lbl_8053AD78->mWindows[windowIndex(player)]->field_0x2c;
}

s32 fn_8037EE8C(s32 player) {
    s32 next = lbl_8053AD78->mWindows[windowIndex(player)]->field_0x2c + 1;
    if (next > 3) {
        next = 0;
    }
    return next;
}

bool fn_8037EC98(s32 player, u16 flags) {
    s32 index = windowIndex(player);
    if ((flags & 4) || (flags & 2)) {
        return true;
    }
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        return fn_802308E0(index);
    }
    s32 players[4];
    s32 count = 0;
    s32 ready = 0;
    for (s32 i = 0; i < 4; i++) {
        players[i] = -1;
        if (fn_8023F264(i) == true) {
            players[i] = i;
            count++;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (players[i] >= 0 && fn_802300EC(players[i]) == true) {
            ready++;
        }
    }
    if (count != ready) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (players[i] >= 0) {
            fn_80230AD4(players[i]);
        }
    }
    return true;
}

// FIXME: 97.4. Regalloc only: the loop's index registers and the two half-size floats are swapped (r28 / r29, f30 / f31).
void fn_8037EB4C() {
    for (s32 i = 0; i < 4; i++) {
        MessageWindow_804AF490* window = lbl_8053AD78->mWindows[i];
        f32 scale = 1.0f;
        if ((window->field_0x28 >> 6) & 1) {
            scale = lbl_8050DB0C.field_0x30[2];
        }
        f32 width = scale * window->field_0x48->field_0xb8;
        f32 height = scale * window->field_0x48->field_0xbc;
        if (window->field_0x24 != 0) {
            f32 halfW = 0.5f * width;
            f32 halfH = 0.5f * height;
            lbl_8053AED0->fn_8043DF8C(lbl_8053AD78->field_0x124[i].x - halfW, lbl_8053AD78->field_0x124[i].y - halfH);
            lbl_8053AED0->fn_8043DF8C(lbl_8053AD78->field_0x124[i].x + halfW, lbl_8053AD78->field_0x124[i].y - halfH);
            lbl_8053AED0->fn_8043DF8C(lbl_8053AD78->field_0x124[i].x - halfW, lbl_8053AD78->field_0x124[i].y + halfH);
            lbl_8053AED0->fn_8043DF8C(lbl_8053AD78->field_0x124[i].x + halfW, lbl_8053AD78->field_0x124[i].y + halfH);
        }
    }
}

void fn_8037EB0C(f32* a, f32* b, f32* c, f32* d, u8* e) {
    *a = lbl_8053AD78->field_0x5c[0];
    *b = lbl_8053AD78->field_0x5c[2];
    *c = lbl_8053AD78->field_0x5c[1];
    *d = lbl_8053AD78->field_0x5c[3];
    *e = lbl_8053AD78->field_0xe6;
}

s32 fn_8037E9F4(s32 player) {
    return lbl_8053AD78->mWindows[windowIndex(player)]->fn_80390850();
}

void fn_8037E8D4(s32 player) {
    lbl_8053AD78->field_0xe8 |= (u8)(1 << windowIndex(player));
}

// FIXME: 97.2. Scheduling only: the original restores r31 after the result is
// computed (lwz r31 / lwz r0 after the srwi); ours restores r31 before it.
bool fn_8037E7AC(s32 player) {
    return lbl_8053AD78->field_0xe8 & (u8)(1 << windowIndex(player));
}

u8 fn_8037E694(s32 player) {
    return lbl_8053AD78->mWindows[windowIndex(player)]->field_0x44;
}

s32 fn_8037E54C(s32 player) {
    if (player >= 0 && player < 4) {
        s32 index = windowIndex(player);
        if (lbl_80529DEC.field_0x118 >= 8 && lbl_80529DEC.field_0x11c == 0) {
            return lbl_8053AD78->field_0x4c[index];
        }
    }
    return 0;
}

bool fn_8037E4BC() {
    for (s32 i = 0; i < 4; i++) {
        bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i);
        if (takesPart && lbl_8053AD78->mWindows[i]->fn_80390818() == 1) {
            return true;
        }
    }
    return false;
}
