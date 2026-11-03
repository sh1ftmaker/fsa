#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

// The players. *lbl_8053AC90 (PlayerManager_804A6F28, game/tu_8022AD64.cpp)
// owns the four Player_804A6F38 objects (code in game/tu_8023FBF0.cpp and
// the units after it up to 0x8027E5BC); tu_8022AD64 is mostly free
// functions that take a player index (0-3), check it and forward to that
// player. 325 game units call into it.
//
// "Player": the index is the one the rest of the game passes around as the
// player number (ActorBase_804A5770's skipPlayer/outPlayer arguments, its
// field_0x1ac[0..3] of player positions), there are exactly four (the
// manager constructor's loop), and 193 units read the position at 0x8 to
// chase or face them. Nothing names the classes (no RTTI, no strings), so
// they carry their vtable addresses (docs/PLAN.md, "Placeholder names").
//
// The per-player queries (about 270 in tu_8022AD64) share one shape, matched
// in fn_8022B7C4, fn_8022B860, fn_8023DE58 and fn_8023E724:
//
//     if (lbl_8053AC90 == NULL) return <fallback>;
//     if (isPlayerActive(player)) return lbl_8053AC90->mPlayers[player]-><query>;
//     return <fallback>;
//
// isPlayerActive (below) is fn_8023F264's body, inlined; its own test of
// lbl_8053AC90 is the dead `bne` right after the outer test in every one of
// them (e.g. 0x8022B7E4). MWCC only inlines functions defined before the
// caller, and the queries come before fn_8023F264, so it is a header inline.
//
// Owner: the shared-header owner (docs/COORDINATOR.md, "Shared game
// headers"). Game-file agents propose changes as SHARED-EDITS.

#include "types.h"
#include "JSystem/JGeometry.h"

// Vtable 0x804A6F38: only the destructor (0x802558E4). Constructor
// 0x80255B28 (this, index), called by PlayerManager_804A6F28's constructor
// for indices 0-3. Size 0x13BC: the manager's __nw__(0x13BC) (0x8023F904).
// Fields only where code outside the player's own units reads them inline;
// the player's getters (tu_8023FBF0) read the rest.
class Player_804A6F38 {
public:
    Player_804A6F38(s32 index);
    virtual ~Player_804A6F38();

    // Out-of-line copy at 0x8023F984, called where MWCC did not inline it
    // (fn_8022EA0C). That it is the weak copy of this class inline is
    // inferred from where it sits (after the unit's functions, before its
    // __sinit); dtk marks it global, as it does every game function.
    bool fn_8023F984() { return mActive; }

    // tu_8023FBF0 getters and setters the queries forward to.
    bool fn_8023FC08();          // byte at 0x42C
    void fn_8023FC10(s32 value); // 0xCA8 = max(0xCA8, value)

    /* 0x0000 */ // vtable
    /* 0x0004 */ u8 field_0x4[0x8 - 0x4];
    /* 0x0008 */ JGeometry::TVec3f mPos; // fn_8023E724 (193 units)
    /* 0x0014 */ u8 field_0x14[0x3B4 - 0x14];
    // The player's layer: compared with ActorBase_804A5770::field_0x4 by
    // fn_801F320C and fn_801F5790; fn_801F329C tests it against 8.
    /* 0x03B4 */ s32 field_0x3b4;
    /* 0x03B8 */ u8 field_0x3b8[0x3DC - 0x3B8];
    /* 0x03DC */ s32 field_0x3dc; // fn_80230CFC (2 without a player)
    /* 0x03E0 */ u8 field_0x3e0[0x3E8 - 0x3E0];
    /* 0x03E8 */ s32 field_0x3e8; // fn_80230B70 (2 without a player)
    /* 0x03EC */ u8 field_0x3ec[0xCAC - 0x3EC];
    // Tested by every per-player query before it touches the player.
    /* 0x0CAC */ bool mActive;
    /* 0x0CAD */ u8 field_0xcad[0x13BC - 0xCAD];
}; // Size: 0x13BC

// *lbl_8053AC90. Vtable 0x804A6F28: only the destructor (0x8023F7F8, which
// deletes the four players and clears lbl_8053AC90). Constructor 0x8023F8B0
// (sets lbl_8053AC90, creates the players, clears 0x68-0x73); tu_80139BE8
// creates it with __nw__(0x74) (0x8013A5A8).
class PlayerManager_804A6F28 {
public:
    PlayerManager_804A6F28();
    virtual ~PlayerManager_804A6F28();

    /* 0x00 */ // vtable
    /* 0x04 */ Player_804A6F38* mPlayers[4];
    // 0x14-0x67: per player, set together by fn_8023F1D4(player, pos, a, b, c).
    /* 0x14 */ JGeometry::TVec3f field_0x14[4];
    /* 0x44 */ s32 field_0x44[4];
    /* 0x54 */ s32 field_0x54[4];
    /* 0x64 */ u8 field_0x64[4];
    /* 0x68 */ s32 field_0x68;
    // Flags, each with a getter and a setter/clearer (fn_8022ADB0 ...
    // fn_8022B278); fn_8022ADFC tests 0x6C-0x72.
    /* 0x6C */ bool field_0x6c;
    /* 0x6D */ bool field_0x6d;
    /* 0x6E */ bool field_0x6e;
    /* 0x6F */ bool field_0x6f;
    /* 0x70 */ bool field_0x70;
    /* 0x71 */ bool field_0x71;
    /* 0x72 */ bool field_0x72;
    /* 0x73 */ bool field_0x73;
}; // Size: 0x74

extern PlayerManager_804A6F28* lbl_8053AC90;

// Whether `player` is a valid index of an existing, active player: the body
// of fn_8023F264, which every per-player query inlines.
inline bool isPlayerActive(s32 player) {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    if (player < 0) {
        return false;
    }
    if (player >= 4) {
        return false;
    }
    if (lbl_8053AC90->mPlayers[player] == NULL) {
        return false;
    }
    return lbl_8053AC90->mPlayers[player]->fn_8023F984();
}

// Functions of game/tu_8022AD64.cpp other units call. Each query returns
// its fallback (after "else") when there is no manager or the player is not
// active. Types come from the getter each one forwards to and from the
// callers (only 0/1 tests -> bool; lfs at 0/4/8 of the result -> TVec3f*).
// Not declared yet: the ~300 others, whose getters take arguments or do more.
//
// Defined in game/tu_8022AD64.cpp (C++ linkage, renamed in symbols.txt).
// The manager's flags (no player index).
bool fn_8022ADB0(void); // 0x73
void fn_8022ADCC(void); // 0x73 = false
void fn_8022ADE4(void); // 0x73 = true
bool fn_8022ADFC(void); // any of 0x6C-0x72
bool fn_8022AF10(void); // 0x72
void fn_8022AF2C(void); // 0x72 = false
void fn_8022AF44(void); // 0x72 = true
bool fn_8022AF5C(void); // 0x71
void fn_8022B130(void); // 0x70 = false
void fn_8022B148(void); // 0x70 = true
bool fn_8022B160(void); // 0x6F
void fn_8022B17C(void); // 0x6F = false
void fn_8022B194(void); // 0x6F = true
bool fn_8022B1AC(void); // 0x6E
void fn_8022B1C8(void); // 0x6E = false
void fn_8022B1E0(void); // 0x6E = true
bool fn_8022B1F8(void); // 0x6D
void fn_8022B214(void); // 0x6D = false
void fn_8022B22C(void); // 0x6D = true
bool fn_8022B244(void); // 0x6C
void fn_8022B260(void); // 0x6C = false

// isPlayerActive(player). 81 units.
bool fn_8023F264(s32 player);

// Per-player queries, by address. "N units": other units that call it;
// "->": the player's getter (tu_8023FBF0 unless said) or field it returns.
bool fn_8022B7C4(s32 player);            // 7 units. -> fn_8023FC08 (byte 0x42C); else false
void fn_8022B860(s32 player, s32 value); // 1 unit. -> fn_8023FC10(value)
s32 fn_8023DE58(s32 player);             // 134 units. -> field_0x3b4 (layer); else 0
JGeometry::TVec3f* fn_8023E724(s32 player); // 193 units. -> &mPos; else &lbl_804CF460

// Not written yet in game/tu_8022AD64.cpp: extern "C" keeps their dtk names
// until that unit defines them.
extern "C" {
// Per-player queries, by address (legend as above).
// 2 units / 7 calls (act_player_start_position, tu_802D39B8 x6). Calls
// fn_80252F80(mPlayers[player], pos, a, b, c), then stores pos in
// field_0x14[player], a in field_0x44, b in field_0x54, c (stb) in field_0x64.
void fn_8023F1D4(s32 player, const JGeometry::TVec3f* pos, s32 a, s32 b, u8 c);
s32 fn_8022BCD4(s32 player);             // 1 unit. -> fn_8023FC24 (word 0xBEC); else 0
bool fn_8022CCAC(s32 player);            // 2 units. -> fn_80249068 (byte 0x121F); else false
bool fn_8022D75C(s32 player);            // 29 units. -> fn_8024BBB0 (byte 0xCE8); else false
// 48 units. -> fn_80240644 (word 0xD60: another player's index, or -1);
// else -1.
s32 fn_8022F3DC(s32 player);
// 58 units. -> fn_80240808 (word 0xD60 >= 0: fn_8022F3DC names a player);
// else false.
bool fn_8022F514(s32 player);
bool fn_8022FA80(s32 player);            // 2 units. -> fn_8024C458 (byte 0xD4C); else false
bool fn_80230188(s32 player);            // 56 units. -> fn_802413E8 (byte 0x454); else false
// 1 unit (act_horse_race_end_area). Owner: tu_8022AD64 (0x80230050). -> fn_8024E03C(player
// slot) when the slot is present and its byte 0xCAC is set; else false.
bool fn_80230050(s32 player);
// 3+ units (act_warp, act_view_mode). Owner: tu_8022AD64 (0x802373EC, defined
// there; per-player query, 0 without a manager). Compared against 1 and 2.
s32 fn_802373EC(s32 player);
s32 fn_80230B70(s32 player);             // 1 unit. -> field_0x3e8; else 2
s32 fn_80230CFC(s32 player);             // 37 units. -> field_0x3dc; else 2
bool fn_80233D20(s32 player);            // 1 unit. -> byte 0x43F; else false
// 1 unit. -> fn_80240868 (word 0x4); 0 without a manager. An inactive
// player falls off the end with r3 still the index (the source has no final
// return); the caller uses the result (mr r26, r3 at 0x80306B44).
s32 fn_802340A0(s32 player);
bool fn_80234134(s32 player);            // 2 units. -> fn_80240870 (byte 0xCFE); else false
bool fn_802349EC(s32 player);            // 4 units. -> fn_80240AF0 (byte 0x1215); else false
bool fn_80234A88(s32 player);            // 1 unit. -> fn_80240AF8 (byte 0x1214); else false
bool fn_80235448(s32 player);            // 8 units. -> fn_802412CC (byte 0x460); else false
s32 fn_802357E0(s32 player);             // 2 units. -> fn_802702E4 (word 0xC74, tu_8026FC20); else 0
s32 fn_8023587C(s32 player);             // 1 unit. -> fn_80270310 (word 0xC70, tu_8026FC20); else 0
s32 fn_80235A30(s32 player);             // 1 unit. -> fn_80270350 (word 0xC64, tu_8026FC20); else 0
s32 fn_80235ACC(s32 player);             // 2 units. -> fn_80270358 (word 0xC50, tu_8026FC20); else 0
// 21 units. -> fn_80241920: fn_80236228(word 0xD60) when that names a
// player, else max(word 0xC04, 0); else 0.
s32 fn_80236228(s32 player);
s32 fn_8023697C(s32 player);             // 1 unit. -> fn_8024322C (word 0xD08); else -1
JGeometry::TVec3f* fn_80238430(s32 player); // 1 unit. -> 0x324; else &lbl_804CF460
// 5+ units (act_thwomp_switch, act_warp, ...). Same shape as fn_8023A51C below.
JGeometry::TVec3f* fn_8023A478(s32 player);
bool fn_8023A51C(s32 player);            // 5+ units. -> player slot present, else false
bool fn_8023A5B8(s32 player);            // 1 unit. -> fn_8026FC84 (byte 0xB9B, tu_8026FC20); else false
s32 fn_8023B22C(s32 player);             // 2 units. -> word 0xB18; else -1
bool fn_8023B400(s32 player);            // 1 unit. -> byte 0xB78; else false
bool fn_8023BE0C(s32 player);            // 3 units. -> fn_8023FF88 (byte 0x1200); else false
// Signed counts, not flags. Callers sign-extend the result before using it
// as an int and never clrlwi it: fn_8023CA88 extsb. + ble at 0x80306D7C and
// extsb r28 three times in act_bomb; fn_8023CB24 extsb + addi 1 at
// 0x8014B7C0, 0x8014BB74 and 0x8014BE84. The getters fn_8024EE14 and
// fn_8024EE0C are a bare lbz, which is what MWCC emits for an s8 field
// (checked with a scratch compile; a u8 return makes callers clrlwi). The
// other sites stb the result or add 1 and stb it, which fits either type.
s8 fn_8023CA88(s32 player);              // 20 units. -> fn_8024EE14 (byte 0x3D8); else 0
s8 fn_8023CB24(s32 player);              // 19 units. -> fn_8024EE0C (byte 0x3D9); else 0
u16 fn_8023CBC0(s32 player);             // 10 units. -> fn_8024EE1C (halfword 0x3C4); else 0x121
s32 fn_8023E10C(s32 player);             // 5 units. -> word 0xCE4; else 0
s32 fn_8023E190(s32 player);             // 1 unit. -> word 0xCE0; else 0
JGeometry::TVec3f* fn_8023E214(s32 player); // 3 units. -> 0xCD4; else &lbl_804CF460
JGeometry::TVec3f* fn_8023E2A0(s32 player); // 1 unit. -> 0xCC8; else &lbl_804CF460
JGeometry::TVec3f* fn_8023E32C(s32 player); // 5 units. -> 0xCB0; else &lbl_804CF460
JGeometry::TVec3f* fn_8023E578(s32 player); // 3 units. -> 0x14; else &lbl_804CF460
JGeometry::TVec3f* fn_8023E604(s32 player); // 3 units. -> 0x2C4; else &lbl_804CF460

// Called by ActorBase_804A5770::vfunc_0x18 with the actor's screen box,
// field_0x4 and mSerial when the box overlaps a player's.
void fn_802397B4(const JGeometry::TBox2f* box, s32 arg_0x4, s32 serial);

// Owner unit tu_8022AD64 (not written yet). 1 unit (tu_802242F4). Index of
// the first player on `layer` whose box test (fn_80273FB8(player, box,
// layer, out)) passes, or -1; *out (two floats, 0 and 4) is written by the
// test.
s32 fn_8023A7BC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
}

#endif
