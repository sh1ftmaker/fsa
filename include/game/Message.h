#ifndef GAME_MESSAGE_H
#define GAME_MESSAGE_H

// The game's message (dialogue text) system:
//
// * game/tu_8037DF68.cpp: the message manager *lbl_8053AD78 and the free
//   functions actors call to open, advance and query message boxes (97
//   consumer units). It loads "gc_four_swords_text_e.bmg" and the font
//   "rock_24_20_4i_e.bfn" (fn_8038DA00), hence "message".
// * game/tu_80390818.cpp: the per-player message windows
//   (MessageWindow_804AF490), which own the JMessage processors of
//   game/tu_803944C8.cpp (its four vtables derive from JMessage's
//   TProcessor / TRenderingProcessor / TSequenceProcessor / TReference; the
//   0x3C4-byte objects are built by fn_803973E8). Only tu_80390818 calls
//   into tu_803944C8, so those classes are not declared here.
// * lbl_8050DB0C: the layout constants object (cls_804AF32C), 20 units.
//
// The variable lbl_8053AD78 (and lbl_8053AD70, the object of
// game/tu_8037ACC4.cpp) sits in the .sbss dtk gives game/tu_8036FB60.cpp,
// but only tu_8037DF68 stores lbl_8053AD78.
//
// fn_8037F2FC (sets up a message box at a position) is declared in
// game/GameGlobals.h, where it was first needed.
//
// Conventions: "player" is a player index 0-3; most functions first map it
// to a window index through fn_8022F340 in some game modes (GameRoot
// 0x7F/0x4C, the same inline block at the start of each) and then use
// field_0x114[index]. "msg" is a BMG message id (compared unsigned, cmplw /
// cmplwi). "serial" is the calling actor's mSerial (ActorBase 0x198).
// Positions are passed by value (callers psq_l/psq_st mPos into a stack
// copy and pass its address). Free functions are extern "C" (dtk's names
// stay the link names); the types beyond the register class are the
// narrowest the asm shows, guesses are said. "N units" counts the other game
// units calling the function.
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function showing it.

#include "types.h"
#include "JSystem/JGeometry.h"

class J2DTextBox;
class JUTResFont;

// One per player, field_0x114 of the manager (created there with
// __nw__(0x5C)). Code in game/tu_80390818.cpp. Named after its vtable
// (0x804AF490, stored first by the constructor).
class MessageWindow_804AF490 {
public:
    // fn_80391A1C: stores the vtable and index, builds field_0x48 and
    // field_0x4c (two 0x3C4-byte objects, fn_803973E8), field_0x44 = 0,
    // field_0x1c = 1.
    MessageWindow_804AF490(s32 index);
    /* 0x08 */ virtual ~MessageWindow_804AF490(); // fn_80391974

    /* 0x00 */ // vtable
    /* 0x04 */ u8 field_0x4[0x18 - 0x4];
    /* 0x18 */ s32 mIndex;
    /* 0x1C */ s32 field_0x1c;
    /* 0x20 */ u8 field_0x20[0x24 - 0x20];
    // Window state byte (fn_80384C58 compares it with 5 and 8; callers of
    // fn_8037F1E4, which returns it, compare with 1, 3 and 5).
    /* 0x24 */ u8 field_0x24;
    /* 0x25 */ u8 field_0x25[0x2C - 0x25];
    /* 0x2C */ s32 field_0x2c; // fn_8037EFB4 returns it
    /* 0x30 */ u8 field_0x30[0x44 - 0x30];
    /* 0x44 */ u8 field_0x44;
    /* 0x48 */ void* field_0x48; // 0x3C4-byte object, game/tu_803944C8.cpp
    /* 0x4C */ void* field_0x4c; // the same
    /* 0x50 */ u8 field_0x50[0x5C - 0x50];
}; // Size: 0x5C

// *lbl_8053AD78. Vtable 0x804AF070 (one slot, the destructor fn_8038D90C).
// tu_80134C04 allocates it with __nw__(0x158, heap, 0) (0x80139384) and
// constructs it with fn_8038DA00.
class MessageManager_804AF070 {
public:
    // fn_8038DA00: sets lbl_8053AD78, loads the font and the BMG from the
    // archive at lbl_8053AAF8->0xA8 (getResource('????', name)), creates
    // the four windows and the text box, and zeroes the state below.
    MessageManager_804AF070();
    /* 0x08 */ virtual ~MessageManager_804AF070(); // fn_8038D90C

    /* 0x000 */ // vtable
    // new J2DTextBox("rock_24_20_4i_e.bfn", field_0x80, 0x64) (fn_8038DA00).
    /* 0x004 */ J2DTextBox* mTextBox;
    // The "gc_four_swords_text_e.bmg" resource; fn_803832BC reads its INF1
    // entries (8 bytes each from +0x20) and the text at +0x24.
    /* 0x008 */ void* mBmg;
    /* 0x00C */ JGeometry::TVec3f field_0xc[4];
    // The message id the box was set up for, -1 when none; fn_8037F2FC
    // compares msg with it (cmplw).
    /* 0x03C */ s32 field_0x3c;
    /* 0x040 */ s32 field_0x40; // -1 after construction
    /* 0x044 */ s32 field_0x44; // 4 units (tu_80390818, tu_803944C8)
    /* 0x048 */ s32 field_0x48;
    /* 0x04C */ s32 field_0x4c[4];
    /* 0x05C */ f32 field_0x5c[4];
    // fn_8037F2FC's x, y, z and fourth float.
    /* 0x06C */ f32 field_0x6c;
    /* 0x070 */ f32 field_0x70;
    /* 0x074 */ f32 field_0x74;
    /* 0x078 */ f32 field_0x78;
    /* 0x07C */ s32 field_0x7c; // fn_8037F2FC's param
    /* 0x080 */ char field_0x80[0x64]; // the text box's string buffer
    /* 0x0E4 */ u8 field_0xe4; // act_battle_royale_end_check reads it
    /* 0x0E5 */ u8 field_0xe5; // fn_8037F2FC's param2
    /* 0x0E6 */ u8 field_0xe6;
    /* 0x0E7 */ u8 field_0xe7;
    /* 0x0E8 */ u8 field_0xe8;
    /* 0x0E9 */ u8 field_0xe9; // 4 units
    /* 0x0EA */ u8 field_0xea;
    /* 0x0EB */ u8 field_0xeb;
    /* 0x0EC */ u8 field_0xec;
    /* 0x0ED */ u8 field_0xed;
    /* 0x0EE */ u8 field_0xee;
    /* 0x0EF */ u8 field_0xef[0xF8 - 0xEF];
    /* 0x0F8 */ s32 field_0xf8;
    /* 0x0FC */ u8 field_0xfc[0x10C - 0xFC];
    /* 0x10C */ s32 field_0x10c; // act_bandit_herding_npc reads it
    /* 0x110 */ s32 field_0x110;
    /* 0x114 */ MessageWindow_804AF490* mWindows[4];
    /* 0x124 */ JGeometry::TVec3f field_0x124[4];
    // The message font (new JUTResFont(res, NULL)); read by 20 units.
    /* 0x154 */ JUTResFont* mFont;
}; // Size: 0x158

extern MessageManager_804AF070* lbl_8053AD78;

// lbl_8050DB0C: a static object of tu_8037DF68 (constructed in place by
// fn_8037DF68, destructor fn_8038DBF0 registered by the unit's __sinit),
// named after its vtable 0x804AF32C. The constructor only stores constants:
// bytes, floats and from 0x4C on 0x156 halfwords (screen positions, sizes
// and frame counts the message and menu code reads with lha; 20 units, 240
// references, mostly one unit per halfword). dtk's object is 0x40C bytes;
// the constructor's last store is the byte at 0x30D.
class cls_804AF32C {
public:
    cls_804AF32C(); // fn_8037DF68
    /* 0x08 */ virtual ~cls_804AF32C(); // fn_8038DBF0

    /* 0x000 */ // vtable
    /* 0x004 */ u8 field_0x4;
    /* 0x005 */ u8 field_0x5;
    /* 0x006 */ s16 field_0x6;
    /* 0x008 */ f32 field_0x8;
    /* 0x00C */ u8 field_0xc[0x30 - 0xC];
    /* 0x030 */ f32 field_0x30[7];
    /* 0x04C */ s16 field_0x4c[(0x2F8 - 0x4C) / 2];
    /* 0x2F8 */ s16 field_0x2f8;
    /* 0x2FA */ u8 field_0x2fa;
    /* 0x2FB */ u8 field_0x2fb;
    /* 0x2FC */ u8 field_0x2fc[0x304 - 0x2FC];
    /* 0x304 */ u8 field_0x304[10];
    /* 0x30E */ u8 field_0x30e[0x40C - 0x30E];
}; // Size: 0x40C (dtk's object; see above)

extern cls_804AF32C lbl_8050DB0C;

// Functions of game/tu_8037DF68.cpp other units call. Return types: callers
// clrlwi 24 / clrlwi. the bool ones.
extern "C" {
// 4 units. Returns field_0x114[window]->field_0x24, the window's state byte
// (lbz with no bool conversion; tu_80301CD0 compares it with 3, 1 and 5).
u8 fn_8037F1E4(s32 player);
// 16 units. field_0x114[window]->field_0x2c (callers cmpwi it).
s32 fn_8037EFB4(s32 player);
// 16 units / 196 calls (the menu units). Copies the text of message msg
// (mBmg's INF1 entry msg) into out, or into out2 / out3 (when not NULL)
// as a counter that control codes advance reaches 1 / 2; one control code
// writes '0' + value (addi 0x30). Returns a width summed from mFont's
// vtable slot 0x2C per character (f1). value: s32 is a guess.
f32 fn_803832BC(char* out, u32 msg, s32 value, char* out2, char* out3);
// 14 units. Returns 0 or 1.
bool fn_8038470C(s32 player, u32 msg);
// 16 units (act_talking_npc calls it once fn_8038AD24 has succeeded).
// Returns 0 or 1.
bool fn_80384C58(s32 player, s32 serial);
// 31 units. Returns 0 or 1; fn_80388EEC calls it.
bool fn_803865B8(s32 player);
// 41 units. Returns 0 or 1.
bool fn_80388CAC(s32 player, s32 serial);
// 20 units. True when the player's window is idle (field_0x24 == 0);
// otherwise fn_803865B8(player) && fn_80385960(player).
bool fn_80388EEC(s32 player);
// 36 units. Opens message msg (mapped by fn_80383EB0(window, msg)) for the
// player, from the actor `serial` on `layer` at pos, through the manager's
// fn_8038A42C; flags bits 0x40 / 0x2000 select variants (rlwinm tests; the
// low 16 bits are passed on). Returns 0 or 1. act_talking_npc passes the
// top 4 bits of its spawn parameter as player and the low 28 as msg.
// flags: u32 is a guess.
bool fn_8038AD24(s32 player, u32 msg, s32 serial, s32 layer, JGeometry::TVec3f pos, u32 flags);
// 22 units. The same without an actor (callers pass an id, layer, pos and
// small flags such as 0x21).
bool fn_8038BD08(s32 player, u32 msg, s32 layer, JGeometry::TVec3f pos, u32 flags);
// 10 units. fn_8038AD24's argument shape, another variant (flags 0x10 in
// tu_80441A1C).
bool fn_8038C678(s32 player, u32 msg, s32 serial, s32 layer, JGeometry::TVec3f pos, u32 flags);
}

#endif
