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
//   0x3C4-byte objects are built by the constructor below). Only tu_80390818 calls
//   into tu_803944C8, so those classes are not declared here.
// * lbl_8050DB0C: the layout constants object (cls_804AF32C), 20 units.
//
// The variable lbl_8053AD78 is owned by game/tu_8037DF68.cpp (its only
// store); lbl_8053AD70, the object of game/tu_8037ACC4.cpp, is owned there.
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

namespace JMessage {
struct TControl;
}
class cls_804B20F8;
class J2DTextBox;
class JUTResFont;

// The 0x3C4-byte objects of game/tu_803944C8.cpp. Named after the vtable
// their constructor (0x803973E8) stores at 0x0, 0x804AF5A0. Only the fields
// tu_80390818 uses are declared.
class MessageWindow_804AF490;

class cls_804AF5A0 {
public:
    // Constructor, defined in game/tu_803944C8.cpp (not yet written; the
    // symbol was fn_803973E8). Called right after the operator new of the
    // 0x3C4 bytes, with the window, its index and the BMG name; the
    // constructor reading is a guess from that shape.
    cls_804AF5A0(MessageWindow_804AF490* window, s32 index, const char* bmgName);
    // Virtual: MessageWindow_804AF490's destructor deletes these through the
    // vtable (lwz 8(r12), deleting flag 1), so the dtor is in slot 2.
    /* 0x008 */ virtual ~cls_804AF5A0();

    /* 0x000 */ // vtable
    /* 0x004 */ u8 field_0x4[0x8 - 0x4];
    // fn_80396F94 updates it each frame (JMessage::TControl::update).
    /* 0x008 */ JMessage::TControl* field_0x8;
    /* 0x00C */ u8 field_0xc[0xB8 - 0xC];
    // The window box's width and height (fn_8037EB4C scales them).
    /* 0x0B8 */ f32 field_0xb8;
    /* 0x0BC */ f32 field_0xbc;
    /* 0x0C0 */ u8 field_0xc0[0x13C - 0xC0];
    // fn_803A30FC stores into it by index; the constructor sets all three to -1.
    /* 0x13C */ s32 field_0x13c[3];
    /* 0x148 */ s32 field_0x148; // fn_80390C38 returns it (-1 when the window is in neither state).
    /* 0x14C */ s32 field_0x14c;
    // fn_80390BDC counts it down to 0 and returns it.
    /* 0x150 */ u32 field_0x150;
    /* 0x154 */ u8 field_0x154[0x15C - 0x154];
    /* 0x15C */ s32 field_0x15c;
    /* 0x160 */ s32 field_0x160;
    /* 0x164 */ u8 field_0x164[0x168 - 0x164];
    /* 0x168 */ s32 field_0x168;
    /* 0x16C */ s32 field_0x16c;
    /* 0x170 */ u8 field_0x170[0x174 - 0x170];
    // fn_80390850 returns it (-1 when the window is in neither state).
    /* 0x174 */ s32 field_0x174;
    /* 0x178 */ s32 field_0x178;
    /* 0x17C */ s32 field_0x17c;
    /* 0x180 */ s32 field_0x180[4];
    /* 0x190 */ s16 field_0x190;
    /* 0x192 */ s16 field_0x192;
    /* 0x194 */ u8 field_0x194[0x19E - 0x194];
    /* 0x19E */ u8 field_0x19e;
    // fn_80390B64 writes 1 here (the byte after 0x19E, which it sets to 0xFF).
    /* 0x19F */ u8 field_0x19f;
    /* 0x1A0 */ u8 field_0x1a0;
    /* 0x1A1 */ u8 field_0x1a1;
    /* 0x1A2 */ u8 field_0x1a2[0x1A9 - 0x1A2];
    /* 0x1A9 */ u8 field_0x1a9;
    /* 0x1AA */ u8 field_0x1aa;
    /* 0x1AB */ u8 field_0x1ab[0x3AE - 0x1AB];
    /* 0x3AE */ u8 field_0x3ae;
    /* 0x3AF */ u8 field_0x3af;
    /* 0x3B0 */ u8 field_0x3b0;
    /* 0x3B1 */ u8 field_0x3b1;
    // fn_80390894 indexes it with the window's argument; the array runs to
    // the end of the object, its real length is not known.
    /* 0x3B2 */ u8 field_0x3b2[0x3BC - 0x3B2];
    /* 0x3BC */ cls_804B20F8* field_0x3bc; // new'd by the constructor
    /* 0x3C0 */ u8 field_0x3c0[0x3C4 - 0x3C0];

    // Defined in game/tu_803944C8.cpp; both take the processor as their
    // object (fn_80390E68 loads it from the window's 0x48).
    void fn_80394690();
    void fn_8039499C(bool flag);

    // game/tu_803944C8.cpp. The one-field accessors and setters the unit ends with.
    void fn_803944C8(s32 value, u8 a, u8 b);
    void fn_80396F94();
    void fn_80395BE8();
    void fn_80396884();
    s32 fn_803A3050();
    s32 fn_803A3058();
    s16 fn_803A3060();
    s16 fn_803A3068();
    void fn_803A3070(s32 value);
    void fn_803A3078();
    void fn_803A3084(s32 index, s32 value);
    s32 fn_803A3094();
    cls_804B20F8* fn_803A309C();
    void fn_803A30A4(u8 a, u32 b);
    void fn_803A30B0();
    s32 fn_803A30BC();
    void fn_803A30C4();
    void fn_803A30D0();
    void fn_803A30DC(s32 value);
    void fn_803A30E4();
    void fn_803A30F0();
    void fn_803A30FC(s32 index, s32 value);
    s32 fn_803A310C();
    void fn_803A3114();
    u8 fn_803A313C();
}; // Size: 0x3C4 (the fields game/tu_803944C8.cpp and tu_80390818 use are declared)

// "gc_four_swords_text_e.bmg" (a JMessage BMG file): a 0x20-byte header, the
// INF1 block (one 8-byte entry per message) and then the DAT1 text block. Only
// what the game code reads is declared.
struct MessageEntry_804AF070 {
    /* 0x0 */ u32 textOffset; // into the DAT1 text
    /* 0x4 */ u16 id;         // the message id fn_803846BC looks up
    /* 0x6 */ u8 field_0x6[2];
}; // Size: 0x8
struct MessageInf1_804AF070 {
    /* 0x00 */ u8 magic[4]; // 'INF1'
    /* 0x04 */ u32 size;    // of the INF1 block; the DAT1 block follows it
    /* 0x08 */ u16 count;   // entries
    /* 0x0A */ u16 entrySize;
    /* 0x0C */ u8 field_0xc[0x10 - 0xC];
    /* 0x10 */ MessageEntry_804AF070 entries[1]; // `count` of them
};
struct MessageFile_804AF070 {
    /* 0x00 */ u8 field_0x0[0x20];
    /* 0x20 */ MessageInf1_804AF070 inf1;
};

// One per player, field_0x114 of the manager (created there with
// __nw__(0x5C)). Code in game/tu_80390818.cpp. Named after its vtable
// (0x804AF490, stored first by the constructor).
class MessageWindow_804AF490 {
public:
    // fn_80391A1C: stores the vtable and index, builds field_0x48 and
    // field_0x4c (two 0x3C4-byte objects, cls_804AF5A0), field_0x44 = 0,
    // field_0x1c = 1.
    MessageWindow_804AF490(s32 index);
    /* 0x08 */ virtual ~MessageWindow_804AF490(); // fn_80391974

    /* 0x00 */ // vtable
    // fn_8038D870 stores the manager's mBmg here.
    /* 0x04 */ MessageFile_804AF070* field_0x4;
    /* 0x08 */ u8 field_0x8[0x18 - 0x8];
    /* 0x18 */ s32 mIndex;
    /* 0x1C */ s32 field_0x1c;
    // fn_80390E40 stores its word argument here while the state is 0 or the
    // flag is set.
    /* 0x20 */ s32 field_0x20;
    // Window state byte (fn_80384C58 compares it with 5 and 8; callers of
    // fn_8037F1E4, which returns it, compare with 1, 3 and 5).
    /* 0x24 */ u8 field_0x24;
    /* 0x25 */ u8 field_0x25[0x28 - 0x25];
    // Option flags, read by fn_80390E68: 0x2 gates the update, 0x40 picks
    // fn_80394690 over fn_8039499C (which gets 0x2000 as its flag).
    /* 0x28 */ u16 field_0x28;
    /* 0x2A */ u8 field_0x2a[0x2C - 0x2A];
    /* 0x2C */ s32 field_0x2c; // fn_8037EFB4 returns it
    /* 0x30 */ s32 field_0x30; // fn_80389F78 sets it to -1
    /* 0x34 */ s32 field_0x34; // fn_80389254 stores a value clamped to 99999
    /* 0x38 */ s32 field_0x38; // fn_8038911C: the same
    /* 0x3C */ s16 field_0x3c; // fn_803894BC: clamped to 9
    /* 0x3E */ s16 field_0x3e; // fn_8038938C: clamped to 9999
    /* 0x40 */ u8 field_0x40[0x44 - 0x40];
    /* 0x44 */ u8 field_0x44;
    /* 0x48 */ cls_804AF5A0* field_0x48;
    /* 0x4C */ cls_804AF5A0* field_0x4c;
    /* 0x50 */ JGeometry::TVec3f field_0x50; // fn_80389F78 stores the box position

    // fn_80390818: maps the window state to a 0-2 class (0: state 0 or 9+,
    // 1: 6-8, 2: 1-5). Placeholder name, no caller identified yet.
    u8 fn_80390818();

    // Placeholder names, bodies in game/tu_80390818.cpp (not written yet):
    // fn_80390C6C steps the window and returns its halfword at 0x26; fn_80391678
    // resets it.
    u16 fn_80390C6C();
    void fn_80391678();
    // Placeholder name: called by fn_8038D570 each frame for a player that
    // takes part (game/tu_80390818.cpp, not written yet).
    void fn_80390EBC();
    // Placeholder name: called by fn_8038470C (game/tu_80390818.cpp, not
    // written yet).
    void fn_803913E0();

    // fn_80390E40: stores value at 0x20 when the state is 0 or flag is set and
    // returns true; false otherwise.
    bool fn_80390E40(s32 value, bool flag);

    // fn_80390E68: with bit 0x2 of field_0x28 set, runs the processor of
    // field_0x48 through fn_80394690 (bit 0x40) or else fn_8039499C (its flag
    // is bit 0x2000).
    void fn_80390E68();

    // fn_80390850: the field at 0x174 of the processor of the window's state
    // (1-5: field_0x4c, 6-8: field_0x48), -1 in any other state.
    s32 fn_80390850();

    // fn_80390B64: sets the byte at 0x19E of the processor of the window's
    // state (5: field_0x4c, 8: field_0x48) to 0xFF and the one at 0x19F to 1.
    void fn_80390B64();

    // fn_80390BA8: the byte at 0x19E of the processor of the window's state
    // (5: field_0x4c, 8: field_0x48), 0xFF in any other state.
    u8 fn_80390BA8();

    // fn_80390BDC: decrements the field at 0x150 of the processor of the
    // window's state (5: field_0x4c, 8: field_0x48) unless it is 0, and
    // returns it; 0 in any other state.
    u32 fn_80390BDC();

    // fn_80390C38: the field at 0x148 of the processor of the window's
    // state (5: field_0x4c, 8: field_0x48), -1 in any other state.
    s32 fn_80390C38();

    // fn_80390894: the byte at index + 0x3B2 of the processor of the window's
    // state (5: field_0x4c, 8: field_0x48), 0 in any other state.
    u8 fn_80390894(s32 index);
}; // Size: 0x5C

// *lbl_8053AD78. Vtable 0x804AF070 (one slot, the destructor fn_8038D90C).
// tu_80134C04 allocates it with __nw__(0x158, heap, 0) (0x80139384) and
// constructs it with fn_8038DA00.
class MessageManager_804AF070 {
public:
    // fn_8038DA00: sets lbl_8053AD78, loads the font and the BMG from the
    // archive at gGameRoot_8053AAF8->0xA8 (getResource('????', name)), creates
    // the four windows and the text box, and zeroes the state below.
    MessageManager_804AF070();
    /* 0x08 */ virtual ~MessageManager_804AF070(); // fn_8038D90C

    // 2 uses (this unit's fn_8038AD24 / fn_8038BD08 / fn_8038C678). Whether every
    // player is ready for a message: players that do not take part, and
    // `exclude`, count as ready; the others need their window idle (state 0) and
    // fn_80230188 (flags 2 / 4 or msg 0x265) or fn_802300EC to hold for them and
    // for everyone fn_8022E438 pairs them with.
    bool fn_80389964(s32 exclude, u32 msg, u32 flags);
    // (this is the manager and is unused.)

    // 0x8038C60C (not written: the inlined fn_803846BC loop comes out in another
    // order, 82 %): fn_8038A984 with msg turned into its INF1 index.
    bool fn_8038C60C(s32 player, s32 actor, u32 msg, s32 serial, u32 flags);
    // 0x8038B8A8 (this unused; callers pass the manager): the variant without a
    // serial: the window goes to state 1 for flags with one of 0x91F, else when
    // the player presses 0x100 and fn_8037EC98 (actor) holds.
    bool fn_8038B8A8(s32 player, s32 actor, s32 value, u32 flags);
    // 0x8038A984 (this unused; callers pass the manager): like fn_8038A42C
    // without a position: the window goes to state 1 (an actor `serial`
    // speaks); needs fn_80303750 to be 0 and fn_8022C8EC to hold.
    bool fn_8038A984(s32 player, s32 actor, s32 value, s32 serial, u32 flags);
    // 0x8038A42C (this unused; callers pass the manager): the same for an actor
    // `serial`: `actor` is the player that talks (fn_8037EC98, fn_8022FCF0 /
    // fn_8022FC54 with serial), the window's field_0x30 gets serial.
    bool fn_8038A42C(s32 player, s32 actor, s32 value, s32 serial, JGeometry::TVec3f pos, u32 flags);
    // 0x80389F78 (this unused; callers pass the manager): opens the player's
    // window when it is idle and flags has one of 0x11F: option bits from flags,
    // fn_80390E40(value, false), position pos, state 6, and the window index
    // appended to field_0xfc; true then. arg is unused.
    bool fn_80389F78(s32 player, s32 arg, s32 value, JGeometry::TVec3f pos, u32 flags);
    // 0x803895EC (this unused; callers pass the manager): for every window in
    // state 9, optionally (a) fn_8022FB1C(player or its mapped index, arg) and
    // (b) fn_8023061C(window, 0); then the window goes back to state 0.
    void fn_803895EC(s32 player, s32 arg, bool a, bool b);

    /* 0x000 */ // vtable
    // new J2DTextBox("rock_24_20_4i_e.bfn", field_0x80, 0x64) (fn_8038DA00).
    /* 0x004 */ J2DTextBox* mTextBox;
    // The "gc_four_swords_text_e.bmg" resource; getMessageText_803832BC reads its INF1
    // entries (8 bytes each from +0x20) and the text at +0x24.
    /* 0x008 */ MessageFile_804AF070* mBmg;
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
    /* 0x0EF */ u8 field_0xef;
    /* 0x0F0 */ u16 field_0xf0[4]; // fn_8038D870 clears them
    /* 0x0F8 */ s32 field_0xf8;
    /* 0x0FC */ s32 field_0xfc[4]; // fn_8038D870 sets them to -1
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
// references, mostly one unit per halfword). the object is 0x310 bytes: the unit's
// __sinit puts the pools' destructor chain right after it. The constructor's last
// store is the byte at 0x30D.
class cls_804AF32C {
public:
    // The constructor (fn_8037DF68, constants only) is not written yet.
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
    /* 0x30E */ u8 field_0x30e[0x310 - 0x30E];
}; // Size: 0x310 (the unit's __sinit places its destructor chain at 0x310)

extern cls_804AF32C lbl_8050DB0C;

// Functions of game/tu_8037DF68.cpp with C++ linkage (written in that unit).
// "window" is the player mapped to a window index as described above.
// Whether any player's window is in state class 1 (fn_80390818).
bool fn_8037E4BC();
// 0 unless lbl_80529DEC's 0x118 is 8 or more and its 0x11C is 0 (the stage
// check), then the manager's field_0x4c[window]. player outside 0-3 gives 0.
s32 fn_8037E54C(s32 player);
// field_0x114[window]->field_0x44.
u8 fn_8037E694(s32 player);
// Whether the manager's byte field_0xe8 has bit `window` set.
bool fn_8037E7AC(s32 player);
// Sets that bit.
void fn_8037E8D4(s32 player);
// 1 unit (tu_80316E34). Maps the player to a window and returns
// fn_80390850(field_0x114[window]): -1 or field_0x174 of the window's object.
s32 fn_8037E9F4(s32 player);
// Copies four floats of the manager (0x5C, 0x64, 0x60, 0x68) and its byte at
// 0xE6 to the pointers.
void fn_8037EB0C(f32* a, f32* b, f32* c, f32* d, u8* e);
// Calls fn_8043DF8C at the four corners of each open window's box (its 0xB8 /
// 0xBC size, scaled by lbl_8050DB0C's field_0x30[2] when option 0x40 is set).
void fn_8037EB4C();
// Whether the players are ready for the message: true at once when flags has
// bit 2 or 4; with GameRoot's field_0x7e == 1 it is fn_802308E0(window), else
// every present player must pass fn_802300EC (then fn_80230AD4 is called on
// each of them).
bool fn_8037EC98(s32 player, u16 flags);
// 20 units. True when the player's window is idle (field_0x24 == 0);
// otherwise fn_803865B8(player) && fn_80385960(player).
bool fn_80388EEC(s32 player);
// The manager's per-frame update (0x8038D570): steps the windows (fn_80390EBC),
// the countdown field_0x7c (with field_0xe4 cleared when a player of
// field_0xe5 presses 0x100 or it runs out), the counter field_0x44 against
// field_0x48 and the view bit field_0xea, and the byte flags lbl_8053ACD8.
void fn_8038D570();
// 14 units. Returns 0 or 1. A window in state 5 goes to state 1, one in state
// 8 to state 6 (both after fn_80390E40(mapped msg, true)); true then, else false.
bool fn_8038470C(s32 player, u32 msg);
// Opens the window for message `value` when it is idle and the player's
// fn_80303750 is 0: state 1, option bits 0x1000 / 0x80 / 0x20 set; true then.
bool fn_80389BF0(s32 player, s32 value);
// The window's field_0x2c plus 1, wrapping to 0 after 3.
s32 fn_8037EE8C(s32 player);
// 16 units. field_0x114[window]->field_0x2c (callers cmpwi it).
s32 fn_8037EFB4(s32 player);
// The halfword at 0x4 of the manager's mBmg entry `index`: its message id.
u16 fn_803846A4(s32 index);
// Store a value into the window's field_0x38 / 0x34 (at most 99999), field_0x3e
// (at most 9999) and field_0x3c (at most 9).
void fn_8038911C(s32 player, s32 value);
void fn_80389254(s32 player, s32 value);
void fn_8038938C(s32 player, s32 value);
void fn_803894BC(s32 player, s32 value);
// The window's fn_80390C6C (its halfword at 0x26).
u16 fn_8037F0CC(s32 player);
// 4 units. Returns field_0x114[window]->field_0x24, the window's state byte
// (lbz with no bool conversion; tu_80301CD0 compares it with 3, 1 and 5).
u8 fn_8037F1E4(s32 player);
// The message text of entry `index` when it starts with the control code 0x1A
// (size 2) and its command byte (at +4) is one of twelve values (7, 0x10,
// 0x17, 0x18, 0x1D, 0x20, 0x21, 0x23, 0x2A, 0x2C, 0x30, 0x33): *out gets the
// command, else 0 and a NULL return.
const char* fn_80383E2C(u32 index, u8* out);
// The index of the message whose id is `msg` in the INF1 entries, 0x12B when
// there is none.
u32 fn_803846BC(u32 msg);

// 41 units. Returns 0 or 1.
bool fn_80388CAC(s32 player, s32 serial);
// Functions of game/tu_8037DF68.cpp other units call. Return types: callers
// clrlwi 24 / clrlwi. the bool ones.
// 1 unit (tu_80139BE8's fn_80139C5C). Draws the open windows in the order
// they were opened (field_0xfc), then the text box when field_0xe4 is set.
void fn_8038D4A8();
extern "C" {
// 16 units / 196 calls (the menu units). Copies the text of message msg
// (mBmg's INF1 entry msg) into out, or into out2 / out3 (when not NULL)
// as a counter that control codes advance reaches 1 / 2; one control code
// writes '0' + value (addi 0x30). Returns a width summed from mFont's
// vtable slot 0x2C per character (f1). value: s32 is a guess.
f32 getMessageText_803832BC(char* out, u32 msg, s32 value, char* out2, char* out3);
// Message msg mapped for window `window` (this unit, not written yet); fn_8038470C
// passes the result to the window's fn_80390E40.
u32 fn_80383EB0(s32 window, u32 msg);
// 1 unit (game/tu_80403D84.cpp) + 1 (tu_8043A0E8). Builds the text of message
// msg into out (lines wrapped to width / height, the two counters are
// getMessageText_803832BC-style arguments); returns a line count that starts at 1.
s32 fn_803837A4(char* out, u32 msg, s32 a, s32 b, f32 width, f32 height);
// 16 units (act_talking_npc calls it once fn_8038AD24 has succeeded).
// Returns 0 or 1.
bool fn_80384C58(s32 player, s32 serial);
// 31 units. Returns 0 or 1; fn_80388EEC calls it.
bool fn_803865B8(s32 player);
// Returns 0 or 1 (fn_80388EEC returns it; this unit, not written yet).
bool fn_80385960(s32 player);

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
