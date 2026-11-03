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
    // Getters and setters the generated queries forward to (tools/gen_game.py).
    void fn_8023FBF0(); // via fn_80231928
    s32 fn_8023FC24(); // via fn_8022BCD4
    s32 fn_8023FC2C(); // via fn_8023BC38
    u8 fn_8023FC5C(); // via fn_8023BCD4
    bool fn_8023FF50(); // via fn_8023BD70
    bool fn_8023FF88(); // via fn_8023BE0C
    s32 fn_8023FFAC(); // via fn_8022BECC
    s32 fn_8023FFC4(); // via fn_80236630
    s32 fn_8024018C(); // via fn_802366CC
    s32 fn_80240354(); // via fn_80236768
    void fn_8024051C(); // via fn_802384BC
    s32 fn_80240644(); // via fn_8022F3DC
    bool fn_80240670(); // via fn_8022F478
    bool fn_80240808(); // via fn_8022F514
    bool fn_80240818(); // via fn_8022DCD8
    bool fn_80240840(s32 arg); // via fn_8022E39C
    bool fn_80240870(); // via fn_80234134
    void fn_80240878(); // via fn_80233F78
    s32 fn_80240888(); // via fn_80234004
    void fn_802408C8(s32 a, const JGeometry::TVec3f* b); // via fn_802348C4
    bool fn_80240AD4(); // via fn_80234950
    bool fn_80240AF0(); // via fn_802349EC
    bool fn_80240AF8(); // via fn_80234A88
    bool fn_80240B00(); // via fn_80234B24
    bool fn_80240B20(); // via fn_80234BC0
    bool fn_80240B5C(); // via fn_80234C5C
    bool fn_80240B70(); // via fn_80234CF8
    bool fn_80240B84(); // via fn_80233EDC
    bool fn_80240D50(); // via fn_80234D94
    bool fn_80240D64(); // via fn_80234E30
    bool fn_80240DAC(); // via fn_80234ECC
    bool fn_80240DCC(); // via fn_8022F64C
    bool fn_80240ED4(); // via fn_80234F68
    bool fn_80240EF4(); // via fn_80235004
    bool fn_80240F14(); // via fn_802350A0
    bool fn_80240F28(); // via fn_8023513C
    bool fn_8024119C(); // via fn_802351D8
    bool fn_802411C4(); // via fn_80235274
    bool fn_802411EC(); // via fn_802354E4
    bool fn_80241200(); // via fn_80235580
    bool fn_80241230(); // via fn_80233DA4
    bool fn_80241258(); // via fn_80235310
    bool fn_80241284(); // via fn_802353AC
    bool fn_802412A4(); // via fn_80233E40
    bool fn_802412CC(); // via fn_80235448
    void fn_802412D4(); // via fn_802356B8
    bool fn_80241380(); // via fn_8023561C
    bool fn_802413C8(); // via fn_80235744
    bool fn_802413E8(); // via fn_80230188
    bool fn_802413F0(); // via fn_8023188C
    bool fn_80241E28(s32 a, const JGeometry::TVec3f* b); // via fn_8022BC38
    void fn_80241E64(); // via fn_8022BD70
    void fn_80242560(); // via fn_80231424
    bool fn_80242704(s32 arg); // via fn_8023162C
    bool fn_802466B4(); // via fn_8022C3AC
    s32 fn_80246B10(u32 arg); // via fn_8022C52C
    s32 fn_80246D1C(u32 arg); // via fn_8022C5EC
    bool fn_80247534(); // via fn_8022C8EC
    bool fn_80247564(); // via fn_8022C9AC
    void fn_80248DAC(); // via fn_80234810
    bool fn_8024B108(s32 arg); // via fn_8022DE10
    bool fn_8024C4C4(s32 arg); // via fn_8022FBB8
    bool fn_8024C828(s32 arg); // via fn_8022FCF0
    void fn_8024CD20(const JGeometry::TBox2f* box, s32 layer, s32 serial, bool a, bool b); // via fn_8022FEB8
    void fn_8024E020(const JGeometry::TVec3f* arg); // via fn_8022FFC4
    void fn_8024F54C(s32 arg); // via fn_80230F44
    bool fn_802570BC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // via fn_8023AB94
    bool fn_8025AA8C(s32 arg); // via fn_8022E23C
    void fn_8025EE0C(s32 a, const JGeometry::TVec3f* b); // via fn_8022F820
    bool fn_8025EE48(s32 arg); // via fn_8022F948
    bool fn_8025EFFC(s32 a, bool b, bool c); // via fn_8022F9E4
    bool fn_80260048(s32 arg); // via fn_80231230
    bool fn_80272768(const JGeometry::TBox2f* box, s32 layer, s32 serial); // via fn_80239558
    void fn_80272C20(const JGeometry::TBox2f* box, s32 arg_0x4, s32 serial); // via fn_802397B4
    bool fn_80273D44(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag); // via fn_8023BB50
    bool fn_80273FB8(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // via fn_8023A7BC
    bool fn_80274238(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* pos, bool flag); // via fn_8023B8E4
    // tu_80256C74 / tu_8025EB18 call it with the player in r3; 0.0f unless
    // playerFlags_802413F0 and the team checks pass (game/tu_8023FBF0.cpp).
    f32 fn_80246F28();
    bool fn_80241478(); // via fn_8022D930
    void fn_8024148C(); // via fn_80236C88
    bool fn_80241498(); // via fn_80236D14
    void fn_802414D0(); // via fn_80236DB0
    bool fn_802414DC(); // via fn_80236E3C
    s32 fn_80241544(); // via fn_80237060
    s32 fn_802415CC(); // via fn_8023725C
    s32 fn_802416C4(); // via fn_802373EC
    void fn_802416FC(s32 arg); // via fn_80237524
    s32 fn_80241794(); // via fn_802375B0
    s32 fn_802417CC(); // via fn_80237810
    void fn_80241804(s32 value); // via fn_8023764C
    s32 fn_80241864(); // via fn_802376D8
    s32 fn_8024189C(); // via fn_80237774
    s32 fn_802418E8(); // via fn_80236100
    s32 fn_80241920(); // via fn_80236228
    s32 fn_8024322C(); // via fn_8023697C
    bool fn_802454C0(); // via fn_8022BF68
    bool fn_802456FC(); // via fn_8022C004
    bool fn_80245938(); // via fn_8022C0A0
    bool fn_80245B74(); // via fn_8022C13C
    bool fn_80245DB0(); // via fn_8022C1D8
    bool fn_80245FEC(); // via fn_8022C274
    bool fn_80246228(); // via fn_8022C310
    void fn_80248004(s32 a, bool b); // via fn_8022CA6C
    s32 fn_802486A0(); // via fn_8022CAF8
    s32 fn_80248778(); // via fn_8022E8D4
    s32 fn_80248970(); // via fn_8022E970
    void fn_80248DB8(); // via fn_802381B8
    void fn_80248DCC(); // via fn_80238244
    void fn_80248FF0(); // via fn_8022CD48
    bool fn_80249068(); // via fn_8022CCAC
    bool fn_8024AFF8(); // via fn_8022DD74
    bool fn_8024B24C(); // via fn_8022DEAC
    void fn_8024B274(); // via fn_8022DF48
    void fn_8024B9E8(); // via fn_8022CB94
    void fn_8024BAEC(); // via fn_8022CC20
    s32 fn_8024BB94(); // via fn_8022D498
    bool fn_8024BBB0(); // via fn_8022D75C
    bool fn_8024C35C(); // via fn_8022F5B0
    bool fn_8024C458(); // via fn_8022FA80
    bool fn_8024C460(s32 arg); // via fn_8022FB1C
    bool fn_8024D98C(); // via fn_80230360
    s32 fn_8024DCD0(); // via fn_80230AD4
    bool fn_8024E03C(); // via fn_80230050
    bool fn_8024E060(); // via fn_802300EC
    void fn_8024E500(); // via fn_80235FE8
    s8 fn_8024EE0C(); // via fn_8023CB24
    s8 fn_8024EE14(); // via fn_8023CA88
    u16 fn_8024EE1C(); // via fn_8023CBC0
    void fn_8024EFF8(s32 arg); // via fn_80230EB8
    void fn_8024F0E8(s32 value); // via fn_80230FD0
    bool fn_8024F5E4(); // via fn_8023E854
    bool fn_8024F630(); // via fn_8022DBA0
    s32 fn_8024FF54(); // via fn_80238EDC
    s32 fn_8024FF6C(); // via fn_8022D7F8
    s32 fn_8024FF84(); // via fn_80238CB4
    bool fn_80250090(); // via fn_8022D9CC
    bool fn_80250ACC(); // via fn_8022DA68
    bool fn_80250D30(); // via fn_8022DB04
    bool fn_802517E4(); // via fn_8022D894
    bool fn_80251BFC(s32 arg); // via fn_802319B4
    void fn_80252F14(const JGeometry::TVec3f* offset); // via fn_8023DFF4
    void fn_80252F40(s32 arg); // via fn_8023E080
    void fn_80252F50(s32 arg); // via fn_8023CC5C
    void fn_80252F5C(s32 arg); // via fn_8023CCE8
    s32 fn_80252F68(); // via fn_8023CD74
    s32 fn_80252F74(); // via fn_8023CE10
    s32 fn_802577E0(); // via fn_8023B484
    bool fn_8025EE34(s32 arg); // via fn_8022F8AC
    bool fn_8025EF68(s32 arg); // via fn_8022F6E8
    bool fn_8025F2CC(); // via fn_8023E8F0
    bool fn_8025F418(); // via fn_8023E98C
    bool fn_8025F448(); // via fn_8023EA28
    bool fn_8025F500(); // via fn_8023EBB8
    bool fn_8025F590(); // via fn_8023ED8C
    void fn_8025F5CC(); // via fn_8023EE28
    s32 fn_8025F648(); // via fn_8023EEB4
    s32 fn_8025F668(); // via fn_8023EF50
    bool fn_8025F684(); // via fn_8023EFEC
    s32 fn_8026FC20(); // via fn_8023A1A4
    bool fn_8026FC84(); // via fn_8023A5B8
    bool fn_8026FCB0(); // via fn_8023A3DC
    bool fn_8026FCE0(); // via fn_8023A51C
    s32 fn_802702E4(); // via fn_802357E0
    void fn_802702EC(); // via fn_80235918
    s32 fn_80270310(); // via fn_8023587C
    s32 fn_80270350(); // via fn_80235A30
    s32 fn_80270358(); // via fn_80235ACC
    void fn_802708D0(); // via fn_80236350
    void fn_802708E8(); // via fn_802363DC
    bool fn_80270A04(); // via fn_80236A18
    bool fn_80270A58(); // via fn_80236AB4
    bool fn_80270A8C(); // via fn_80236B50
    bool fn_80270C88(); // via fn_80236BEC
    void fn_80270F00(s32 arg); // via fn_80236804
    bool fn_80271098(const JGeometry::TVec3f* pos); // via fn_802379E4
    s32 fn_80272184(); // via fn_8023B630
    s32 fn_802721A8(); // via fn_8023B6CC
    bool fn_802729F0(s32 serial); // via fn_80239718
    bool fn_80272DCC(const JGeometry::TBox2f* box, s32 layer, s32 serial); // via fn_802399D8
    bool fn_802730D8(const JGeometry::TBox2f* box, s32 layer, bool flag, f32 margin); // via fn_8023B0F4
    bool fn_802732C4(const JGeometry::TBox2f* box, s32 layer); // via fn_8023B190

    /* 0x0000 */ // vtable
    // The player's index (0-3): the constructor's argument; fn_80246F28
    // counts the players 0..field_0x4.
    /* 0x0004 */ s32 field_0x4;
    /* 0x0008 */ JGeometry::TVec3f mPos; // fn_8023E724 (193 units)
    /* 0x0014 */ u8 field_0x14[0x3B4 - 0x14];
    // The player's layer: compared with ActorBase_804A5770::field_0x4 by
    // fn_801F320C and fn_801F5790; fn_801F329C tests it against 8.
    /* 0x03B4 */ s32 field_0x3b4;
    /* 0x03B8 */ u8 field_0x3b8[0x3DC - 0x3B8];
    /* 0x03DC */ s32 field_0x3dc; // fn_80230CFC (2 without a player)
    /* 0x03E0 */ u8 field_0x3e0[0x3E8 - 0x3E0];
    /* 0x03E8 */ s32 field_0x3e8; // fn_80230B70 (2 without a player)
    /* 0x03EC */ u8 field_0x3ec[0x42C - 0x3EC];
    // Read by fn_8023FC08 (tu_8023FBF0); fn_8023FBF0 sets the byte after it.
    /* 0x042C */ bool field_0x42c;
    /* 0x042D */ u8 field_0x42d;
    /* 0x042E */ u8 field_0x42e[0xBEC - 0x42E];
    // Returned by fn_8023FC24 (via fn_8022BCD4).
    /* 0x0BEC */ s32 field_0xbec;
    /* 0x0BF0 */ u8 field_0xbf0[0xC80 - 0xBF0];
    // 0xC80, 0xCA0, 0xCE9, 0xCEE and 0x1215: the bytes fn_802413F0 tests
    // (game/PlayerFlags.h).
    /* 0x0C80 */ u8 field_0xc80;
    /* 0x0C81 */ u8 field_0xc81[0xCA0 - 0xC81];
    /* 0x0CA0 */ u8 field_0xca0;
    /* 0x0CA1 */ u8 field_0xca1[0xCAC - 0xCA1];
    // Tested by every per-player query before it touches the player.
    /* 0x0CAC */ bool mActive;
    /* 0x0CAD */ u8 field_0xcad[0xCE9 - 0xCAD];
    // A u8: fn_802413F0 converts it to its bool result (neg / or / srwi).
    /* 0x0CE9 */ u8 field_0xce9;
    /* 0x0CEA */ u8 field_0xcea[0xCEE - 0xCEA];
    /* 0x0CEE */ u8 field_0xcee;
    /* 0x0CEF */ u8 field_0xcef[0xCFE - 0xCEF];
    // Returned by fn_80240870 (via fn_80234134).
    /* 0x0CFE */ bool field_0xcfe;
    /* 0x0CFF */ u8 field_0xcff[0xD60 - 0xCFF];
    // Another player's index, or negative for none: fn_80246F28 asks the
    // per-player queries about it (fn_8022CAF8, fn_80234004) and reads its
    // own field_0x120c / field_0x1224 instead when it is negative.
    /* 0x0D60 */ s32 field_0xd60;
    /* 0x0D64 */ u8 field_0xd64[0x11EA - 0xD64];
    /* 0x11EA */ u8 field_0x11ea;
    /* 0x11EB */ u8 field_0x11eb;
    /* 0x11EC */ s32 field_0x11ec; // a player index for fn_8030AAF4
    /* 0x11F0 */ u8 field_0x11f0[0x1200 - 0x11F0];
    // A bool: fn_8023FF88 returns it as loaded (lbz; blr).
    /* 0x1200 */ bool field_0x1200;
    /* 0x1201 */ u8 field_0x1201[0x120C - 0x1201];
    /* 0x120C */ s32 field_0x120c;
    /* 0x1210 */ u8 field_0x1210[0x1214 - 0x1210];
    // A bool: fn_80240AF8 returns it as loaded (lbz; blr).
    /* 0x1214 */ bool field_0x1214;
    // A bool: fn_80240AF0 returns it as loaded (lbz; blr).
    /* 0x1215 */ bool field_0x1215;
    /* 0x1216 */ u8 field_0x1216[0x1224 - 0x1216];
    /* 0x1224 */ s32 field_0x1224;
    /* 0x1228 */ u8 field_0x1228[0x13BC - 0x1228];
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

// Generated by tools/gen_game.py (family fwd), by address. The return type
// comes from the callers (only clrlwi 24 / byte stores of the result -> bool;
// word uses -> s32; a caller that returns it passes on its callers' uses),
// each extra argument's from the getter's first use of it (followed into a
// callee that receives it in the same register: its mangled parameter type,
// else its own first use; float loads of exactly +0/+4/+8 through it ->
// const JGeometry::TVec3f*, stores -> JGeometry::TVec3f*).
bool fn_8022BC38(s32 player, s32 a, const JGeometry::TVec3f* b); // -> fn_80241E28; else false
s32 fn_8022BCD4(s32 player);             // 1 unit. -> fn_8023FC24 (word 0xBEC); else 0
void fn_8022BD70(); // -> fn_80241E64 on every active player
s32 fn_8022BECC(s32 player); // -> fn_8023FFAC; else 0
bool fn_8022BF68(s32 player); // -> fn_802454C0; else false
bool fn_8022C004(s32 player); // -> fn_802456FC; else false
bool fn_8022C0A0(s32 player); // -> fn_80245938; else false
bool fn_8022C13C(s32 player); // -> fn_80245B74; else false
bool fn_8022C1D8(s32 player); // -> fn_80245DB0; else false
bool fn_8022C274(s32 player); // -> fn_80245FEC; else false
bool fn_8022C310(s32 player); // -> fn_80246228; else false
bool fn_8022C3AC(s32 player); // -> fn_802466B4; else false (also for a player outside 0-3)
s32 fn_8022C52C(s32 player, u32 arg); // -> fn_80246B10; else 0 (also for a player outside 0-3)
s32 fn_8022C5EC(s32 player, u32 arg); // -> fn_80246D1C; else 0 (also for a player outside 0-3)
bool fn_8022C8EC(s32 player); // -> fn_80247534; else false (also for a player outside 0-3)
bool fn_8022C9AC(s32 player); // -> fn_80247564; else false (also for a player outside 0-3)
void fn_8022CA6C(s32 player, s32 a, bool b); // 11 units. act_post_warp_pillar_handling passes (i, 0, 1)
s32 fn_8022CAF8(s32 player); // -> fn_802486A0; else 0
void fn_8022CB94(s32 player); // -> fn_8024B9E8
void fn_8022CC20(s32 player); // -> fn_8024BAEC
void fn_8022CD48(s32 player); // -> fn_80248FF0
s32 fn_8022D498(s32 player); // -> fn_8024BB94; else 0
s32 fn_8022D7F8(s32 player); // -> fn_8024FF6C; else 0
bool fn_8022D894(s32 player); // -> fn_802517E4; else false
bool fn_8022D930(s32 player); // -> fn_80241478; else false
bool fn_8022D9CC(s32 player); // -> fn_80250090; else false
bool fn_8022DA68(s32 player); // -> fn_80250ACC; else false
bool fn_8022DBA0(s32 player); // -> fn_8024F630; else false
bool fn_8022DE10(s32 player, s32 arg); // -> fn_8024B108; else false
bool fn_8022E23C(s32 player, s32 arg); // -> fn_8025AA8C; else false
void fn_8022F820(s32 player, s32 a, const JGeometry::TVec3f* b); // -> fn_8025EE0C
bool fn_8022F948(s32 player, s32 arg); // -> fn_8025EE48; else false
bool fn_8022F9E4(s32 player, s32 a, bool b, bool c); // -> fn_8025EFFC; else false
bool fn_8022FBB8(s32 player, s32 arg); // -> fn_8024C4C4; else false
bool fn_8022FCF0(s32 player, s32 arg); // -> fn_8024C828; else false
void fn_8022FFC4(s32 player, const JGeometry::TVec3f* arg); // -> fn_8024E020
void fn_80230F44(s32 player, s32 arg); // -> fn_8024F54C
bool fn_80231230(s32 player, s32 arg); // -> fn_80260048; else false
void fn_80231424(); // -> fn_80242560 on every active player
bool fn_8023162C(s32 player, s32 arg); // -> fn_80242704; else false
void fn_80234810(); // -> fn_80248DAC on every active player
void fn_802348C4(s32 player, s32 a, const JGeometry::TVec3f* b); // -> fn_802408C8

// Owner: tu_8022AD64 (0x8022FEB8, defined there). 34 units (act_old_man, act_talking_sign, ...).
// (box, layer, serial, a, b): called with the actor's box moved to its position, its field_0x4
// and mSerial; a, b are li 0/1.
void fn_8022FEB8(const JGeometry::TBox2f* box, s32 layer, s32 serial, bool a, bool b);

// act_magic_jar's pickup test (tu_8022AD64 forwards to fn_80274238 for each present player):
// the first player whose box overlaps `box` on `layer`, or -1. `pos` is forwarded to fn_80274238, which adds the view offset into it in place
// (0x80274344-0x80274360) when a player hits; `flag` is only passed on.
s32 fn_8023B8E4(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* pos, bool flag);

// Called by ActorBase_804A5770::vfunc_0x18 with the actor's screen box,
// field_0x4 and mSerial when the box overlaps a player's.
void fn_802397B4(const JGeometry::TBox2f* box, s32 arg_0x4, s32 serial);

// Owner unit tu_8022AD64. 5 units (act_armos, act_colour_block,
// act_somaria_block, act_gravestone, ...). Index (0-3) of the first present
// player whose box test (fn_80272768(player, box, layer, serial)) passes, or
// -1; 0 without a player manager.
s32 fn_80239558(const JGeometry::TBox2f* box, s32 layer, s32 serial);

// Owner unit tu_8022AD64. 5+ units (act_candlestick_switch,
// act_icicle, act_snowball, act_ice_wall, act_force_gem_wooden). Index (0-3) of
// the first present player whose box test (fn_802570BC(player, box, layer,
// out)) passes, or -1 (also without a player manager). fn_802570BC stores two
// floats (+0, +4) through `out`, which the callers read as a position.
s32 fn_8023AB94(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);

// Owner unit tu_8022AD64. 1 unit (tu_802242F4). Index of
// the first player on `layer` whose box test (fn_80273FB8(player, box,
// layer, out)) passes, or -1; *out (two floats, 0 and 4) is written by the
// test.
s32 fn_8023A7BC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);

// Owner unit tu_8022AD64. 1 unit (act_cactus). Index of the
// first active player for which fn_80273D44(player object, box, layer, out,
// flag) passes, or -1 (also without a manager); *out is two floats.
s32 fn_8023BB50(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag);

// 1 unit (act_sand_crab's update: clrlwi 24 on the result, like fn_80230188 beside it).
bool fn_8022DCD8(s32 player); // -> fn_80240818; else false
bool fn_8022DD74(s32 player); // -> fn_8024AFF8; else false
bool fn_8022DEAC(s32 player); // -> fn_8024B24C; else false
void fn_8022DF48(s32 player); // -> fn_8024B274
bool fn_8022E39C(s32 player, s32 arg); // -> fn_80240840; else false
s32 fn_8022E8D4(s32 player); // -> fn_80248778; else 0
s32 fn_8022E970(s32 player); // -> fn_80248970; else 0
bool fn_8022F5B0(s32 player); // -> fn_8024C35C; else false
bool fn_8022F64C(s32 player); // -> fn_80240DCC; else false
bool fn_8022F6E8(s32 player, s32 arg); // -> fn_8025EF68; else false
bool fn_8022F8AC(s32 player, s32 arg); // -> fn_8025EE34; else false
bool fn_8022FB1C(s32 player, s32 arg); // -> fn_8024C460; else false
bool fn_80230360(s32 player); // -> fn_8024D98C; else false
bool fn_8023188C(s32 player); // -> fn_802413F0; else false
void fn_80231928(s32 player); // -> fn_8023FBF0
bool fn_802319B4(s32 player, s32 arg); // -> fn_80251BFC; else false
bool fn_80233DA4(s32 player); // -> fn_80241230; else false
bool fn_80233E40(s32 player); // -> fn_802412A4; else false
bool fn_80233EDC(s32 player); // -> fn_80240B84; else false
void fn_80233F78(s32 player); // -> fn_80240878
s32 fn_80234004(s32 player); // -> fn_80240888; else 0
bool fn_80234134(s32 player);            // 2 units. -> fn_80240870 (byte 0xCFE); else false
bool fn_80234950(s32 player); // -> fn_80240AD4; else false
bool fn_802349EC(s32 player);            // 4 units. -> fn_80240AF0 (byte 0x1215); else false
bool fn_80234A88(s32 player);            // 1 unit. -> fn_80240AF8 (byte 0x1214); else false
bool fn_80234B24(s32 player); // -> fn_80240B00; else false
bool fn_80234BC0(s32 player); // -> fn_80240B20; else false
bool fn_80234C5C(s32 player); // -> fn_80240B5C; else false
bool fn_80234CF8(s32 player); // -> fn_80240B70; else false
bool fn_80234D94(s32 player); // -> fn_80240D50; else false
bool fn_80234E30(s32 player); // -> fn_80240D64; else false
bool fn_80234ECC(s32 player); // -> fn_80240DAC; else false
bool fn_80234F68(s32 player); // -> fn_80240ED4; else false
bool fn_80235004(s32 player); // -> fn_80240EF4; else false
bool fn_802350A0(s32 player); // -> fn_80240F14; else false
bool fn_802351D8(s32 player); // -> fn_8024119C; else false
bool fn_80235274(s32 player); // -> fn_802411C4; else false
bool fn_80235310(s32 player); // -> fn_80241258; else false
bool fn_802353AC(s32 player); // -> fn_80241284; else false
bool fn_80235448(s32 player);            // 8 units. -> fn_802412CC (byte 0x460); else false
bool fn_802354E4(s32 player); // -> fn_802411EC; else false
bool fn_80235580(s32 player); // -> fn_80241200; else false
void fn_802356B8(s32 player); // -> fn_802412D4
bool fn_80235744(s32 player); // -> fn_802413C8; else false
s32 fn_802357E0(s32 player);             // 2 units. -> fn_802702E4 (word 0xC74, tu_8026FC20); else 0
s32 fn_8023587C(s32 player);             // 1 unit. -> fn_80270310 (word 0xC70, tu_8026FC20); else 0
void fn_80235918(s32 player); // -> fn_802702EC
s32 fn_80235A30(s32 player);             // 1 unit. -> fn_80270350 (word 0xC64, tu_8026FC20); else 0
s32 fn_80235ACC(s32 player);             // 2 units. -> fn_80270358 (word 0xC50, tu_8026FC20); else 0
void fn_80235FE8(s32 player); // -> fn_8024E500
s32 fn_80236100(s32 player); // -> fn_802418E8; else 0
void fn_80236350(s32 player); // -> fn_802708D0
void fn_802363DC(s32 player); // -> fn_802708E8
s32 fn_80236630(s32 player); // -> fn_8023FFC4; else 0
s32 fn_802366CC(s32 player); // -> fn_8024018C; else 0
s32 fn_80236768(s32 player); // -> fn_80240354; else 0
void fn_80236804(s32 player, s32 arg); // -> fn_80270F00
bool fn_80236AB4(s32 player); // -> fn_80270A58; else true
void fn_80236C88(s32 player); // -> fn_8024148C
void fn_80236DB0(s32 player); // -> fn_802414D0
bool fn_80236E3C(s32 player); // -> fn_802414DC; else true
s32 fn_80237060(s32 player); // -> fn_80241544; else 0
s32 fn_8023725C(s32 player); // -> fn_802415CC; else 0
s32 fn_802376D8(s32 player); // -> fn_80241864; else 0
s32 fn_80237774(s32 player); // -> fn_8024189C; else 0
void fn_802381B8(s32 player); // -> fn_80248DB8
void fn_80238244(s32 player); // -> fn_80248DCC
void fn_802384BC(s32 player); // -> fn_8024051C
s32 fn_80238CB4(s32 player); // -> fn_8024FF84; else -1
s32 fn_80238EDC(s32 player); // -> fn_8024FF54; else -1
s32 fn_8023A1A4(s32 player); // -> fn_8026FC20; else 0
bool fn_8023A3DC(s32 player); // -> fn_8026FCB0; else false
bool fn_8023A51C(s32 player);            // 5+ units. -> player slot present, else false
bool fn_8023A5B8(s32 player);            // 1 unit. -> fn_8026FC84 (byte 0xB9B, tu_8026FC20); else false
s32 fn_8023B484(s32 player); // -> fn_802577E0; else 0
s32 fn_8023B630(s32 player); // -> fn_80272184; else 0
s32 fn_8023B6CC(s32 player); // -> fn_802721A8; else 0
s32 fn_8023BC38(s32 player); // -> fn_8023FC2C; else 0
bool fn_8023BD70(s32 player); // -> fn_8023FF50; else false
void fn_8023CC5C(s32 player, s32 arg); // -> fn_80252F50
void fn_8023CCE8(s32 player, s32 arg); // -> fn_80252F5C
s32 fn_8023CD74(s32 player); // -> fn_80252F68; else 0
s32 fn_8023CE10(s32 player); // -> fn_80252F74; else 0
void fn_8023E080(s32 player, s32 arg); // -> fn_80252F40
bool fn_8023E854(s32 player); // -> fn_8024F5E4; else false
bool fn_8023E8F0(s32 player); // -> fn_8025F2CC; else false
bool fn_8023E98C(s32 player); // -> fn_8025F418; else false
void fn_8023EE28(s32 player); // -> fn_8025F5CC
s32 fn_8023EEB4(s32 player); // -> fn_8025F648; else 0
s32 fn_8023EF50(s32 player); // -> fn_8025F668; else 0
bool fn_8023EFEC(s32 player); // -> fn_8025F684; else false

// Per-player queries, by address (legend as above).
// act_heart_container's update: results tested with clrlwi 24 (bool); fn_80230224's
// second argument is clrlwi 24 (a bool) and its result is unused.
bool fn_8023ED8C(s32 player);
bool fn_8023EBB8(s32 player);

// Owner: tu_8022AD64 (0x802300EC, defined there). 1 unit (act_merchant's update), also called
// by ActorManager_804A5800::fn_801F6884's loop. Per-player query on the index fn_8022FD8C
// returned; result tested with clrlwi 24.
bool fn_802300EC(s32 player);

// Owner: tu_8022AD64 (0x80230AD4, defined there). 1 unit (act_pull_switch, result
// unused); also called by ActorManager_804A5800::fn_801F6884. Returns fn_8024DCD0's
// result on the player, 0 without one.
s32 fn_80230AD4(s32 player);

// Per-player queries and setters of the 0x9C-byte family, owner tu_8022AD64
// (cls_804AC388::update, game/tu_8033F448.cpp). Results are tested with
// clrlwi 24 (bool); fn_80237810 is compared as a word and the value
// minus 4 goes to fn_8023764C.
bool fn_8022F478(s32 player);
bool fn_80236A18(s32 player);
bool fn_80236D14(s32 player);
s32 fn_80237810(s32 player);
void fn_8023764C(s32 player, s32 value);
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

// Per-player queries tested with clrlwi 24 (bool) by game/tu_8012EA00.cpp
// (cls_8049E8F8::playersInside); owner tu_8022AD64.
bool fn_8023EA28(s32 player);
bool fn_8023561C(s32 player);

// 1 unit (act_horse_race_end_area). Owner: tu_8022AD64 (0x80230050). -> fn_8024E03C(player
// slot) when the slot is present and its byte 0xCAC is set; else false.
bool fn_80230050(s32 player);

// Owner: tu_8022AD64 (0x80230FD0, defined there). 26 call sites.
// (player, value): the body passes the player's slot and r4 on to
// fn_8024F0E8 (compared with 0x20024 / 0x2002B) when its byte 0xCAC is set.
void fn_80230FD0(s32 player, s32 value);

// 3+ units (act_warp, act_view_mode). Owner: tu_8022AD64 (0x802373EC, defined
// there; per-player query, 0 without a manager). Compared against 1 and 2.
s32 fn_802373EC(s32 player);

// Owner: tu_8022AD64 (0x8022DB04, defined there). 2 units (act_push_switch,
// act_somaria_block). Same family as fn_80230050: tests the player slot's byte 0xCAC;
// else false. Result tested with clrlwi 24.
bool fn_8022DB04(s32 player);

// 21 units. -> fn_80241920: fn_80236228(word 0xD60) when that names a
// player, else max(word 0xC04, 0); else 0.
s32 fn_80236228(s32 player);

// 1 unit (act_attack_hitbox_when_player_is_thrown). Per-player query.
bool fn_80236BEC(s32 player);
s32 fn_8023697C(s32 player);             // 1 unit. -> fn_8024322C (word 0xD08); else -1

// Per-player forwarders of tu_8022AD64 (act_magic_jar's pickup): fn_80236B50 -> fn_80270A8C (a
// bool, true without a manager or player), fn_802375B0 -> fn_80241794 (an int compared with 0x180),
// fn_80230EB8 -> fn_8024EFF8 and fn_80237524 -> fn_802416FC (void, one int argument).
bool fn_80236B50(s32 player);
s32 fn_802375B0(s32 player);
void fn_80230EB8(s32 player, s32 arg);
void fn_80237524(s32 player, s32 arg);

// Box test: `box` on `layer`; `flag` (bool: fn_802730D8 tests r6 with clrlwi 24) and f1 = 0.0f
// are passed by act_attack_hitbox_when_player_is_thrown.
bool fn_8023B0F4(s32 player, const JGeometry::TBox2f* box, s32 layer, bool flag, f32 margin);

// 15 units. -> fn_8023FC5C (a byte); 0xFF without a player manager, 0xFF for an inactive player.
u8 fn_8023BCD4(s32 player);
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

// Owner unit tu_8022AD64. 5 units (act_quicksand, act_switch,
// act_warp, act_gravestone, tu_8023FBF0). Bool-tested (clrlwi 24) result of
// fn_80240F28 on the player; false without a player.
bool fn_8023513C(s32 player);

// Owner unit tu_8022AD64. 1 unit (act_pull_switch). Adds the
// vector to the player's position-like vector at 0x2D0 (fn_80252F14, which sets
// byte 0x434) when the player is present.
void fn_8023DFF4(s32 player, const JGeometry::TVec3f* offset);

// Owner unit tu_8022AD64. 3 units (act_cactus ignores the
// result, act_ice_wall and act_gohma test it with clrlwi 24). If `player` is
// active, fn_80271098(player object, pos), which is fn_80238548(player, 0,
// pos, 13): the hit position (act_cactus passes its mPos; the others a stack
// vector whose x and y they store).
bool fn_802379E4(s32 player, const JGeometry::TVec3f* pos);

// Owner unit tu_8022AD64. 1 unit (act_thwomp). If `player`
// is active (slot present, byte 0xCAC set), fn_802732C4(player object, box,
// layer): a box test, bool-tested (clrlwi 24) by the caller; else false.
bool fn_8023B190(s32 player, const JGeometry::TBox2f* box, s32 layer);

// Owner unit tu_8022AD64. 1 unit (act_thwomp). Same guard,
// then fn_80272DCC(player object, box, layer, serial) (the per-player form of
// fn_80239914); the caller ignores the result (0/1 from fn_80272DCC, 0 on the guard path).
bool fn_802399D8(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 serial);

// Owner unit tu_8022AD64. 1 unit (act_thwomp). Same guard,
// then fn_802729F0(player object, serial) (the per-player form of
// fn_80239638); the caller stores the result in a byte.
bool fn_80239718(s32 player, s32 serial);

// Not written yet in game/tu_8022AD64.cpp: extern "C" keeps their dtk names
// until that unit defines them.
extern "C" {
// Owner: tu_8022AD64 (0x8022E114, defined there). 2 uses (tu_801D11FC). Forwards
// (player object, out) to the player's fn_8024B2BC when the player is present;
// *out (3 floats, read as a position) is written by it.
void fn_8022E114(s32 player, JGeometry::TVec3f* out);
// Owner: tu_8022AD64 (0x8023E7B0, defined there). 1 unit (act_rock's update: the carried
// rock's position follows it). Like fn_8023E724 (the player's mPos, else &lbl_804CF460) but
// through fn_8024145C; takes a player index (act_rock passes field_0xc8).
JGeometry::TVec3f* fn_8023E7B0(s32 player);
bool fn_802308E0(s32 player);
// 1 unit (act_dummy_conversation). Owner: tu_8022AD64. fn_8022FE80 forwards
// (layer, serial, a, b) to the player's fn_8024CAC8 (it compares serial with
// the player's word 0xD44); fn_8022FD8C returns the player index whose
// fn_8024CA0C accepts serial, or -1; fn_8023061C(player, 0) is called when a
// conversation test starts or is cancelled (result unused).
void fn_8022FE80(s32 player, s32 layer, s32 serial, s32 a, s32 b);
s32 fn_8022FD8C(s32 serial);
// Owner: tu_8022AD64 (0x8022EA0C, defined there). 1 unit (act_blindfold's update): maps a
// player index to the index whose position fn_8023E724 is then asked for (r3 in, r3 out).
s32 fn_8022EA0C(s32 player);
// Owner: tu_8022AD64 (0x8022FF8C, defined there). 1 unit (act_merchant's update, state 1):
// fn_8022FEB8's box test for a given player (player, box, layer, serial, a, b), called with
// the actor's box moved to its position, its field_0x4 and mSerial, li 1 / li 0.
void fn_8022FF8C(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 serial, bool a, bool b);
bool fn_8023061C(s32 player, bool flag);
// Owner: tu_8022AD64 (0x80239B64, defined there). 1 unit (tu_8021693C). The
// per-player box test of fn_8022FF8C's family: if the player is present
// (byte 0xCAC set) it forwards (player object, box, layer, serial, a, b) to
// fn_80272224 and returns its 0/1 result (clrlwi 24 / test in the caller).
// tu_8021693C passes its hit box, the actor's field_0x4, a type word, 1, 1.
bool fn_80239B64(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 type, bool a, bool b);
// Owner: tu_8022AD64 (0x8022F340, defined there). act_gambling_chest. Maps a
// player index to a window index (-1 without a manager; see Message.h).
s32 fn_8022F340(s32 player);
// Owner: tu_8022AD64 (0x80231A50, defined there). tu_80441318. No arguments
// (r3 is not loaded); returns a player index, or a negative value for none
// (the caller maps a non-negative one through fn_8022F340).
s32 fn_80231A50();
// Owner: tu_8022AD64 (0x802362C4, defined there). act_gambling_chest (also
// act_cucco, act_force_gem_2). (player, value): tail-calls fn_8024ED4C(player
// object, value) when the player exists and its byte 0xCAC is set.
void fn_802362C4(s32 player, s32 value);
bool fn_80230224(s32 player, bool flag);
// 1 unit (tu_8021827C). Loop over the four players: fn_8022BA58 calls
// fn_80241DD0(player, layer, pos) and fn_8022B978 fn_80241BA0(player, layer,
// box, flag); each returns 1 as soon as one player passes, else 0 (callers
// test r3 with clrlwi.). Whether *pos / *box is visible for a player on `layer`.
bool fn_8022BA58(s32 layer, const JGeometry::TVec3f* pos);
bool fn_8022B978(s32 layer, const JGeometry::TBox2f* box, bool flag);
// 2 units / 7 calls (act_player_start_position, tu_802D39B8 x6). Calls
// fn_80252F80(mPlayers[player], pos, a, b, c), then stores pos in
// field_0x14[player], a in field_0x44, b in field_0x54, c (stb) in field_0x64.
void fn_8023F1D4(s32 player, const JGeometry::TVec3f* pos, s32 a, s32 b, u8 c);
// 1 unit (act_attack_hitbox_when_player_is_thrown; others call it too). Player pair test.
bool fn_8022E438(s32 player, s32 other);
// 61 units. (player, pos, id, a, b, c, flag): r6 an unsigned byte (clrlwi 24),
// r7/r8 li -0x80, r9 li 1 in act_post_warp_pillar_handling.
void fn_80230498(s32 player, const JGeometry::TVec3f* pos, s32 id, u8 a, s8 b, s8 c, bool flag);
// 34+ units. Takes a flag (clrlwi 24), not a player: loops all four players;
// every caller passes 0. Result tested against 1.
bool fn_80230534(bool flag);
// Owner: tu_8022AD64 (0x8023077C, defined there). 2 units (tu_802F4AEC, act_zoom).
// No arguments; returns a bool (clrlwi 24 by the caller); false without a
// manager.
bool fn_8023077C();
s32 fn_80230B70(s32 player);             // 1 unit. -> field_0x3e8; else 2
void fn_80230BF4(s32 player, s32 mode);  // 18 units. act_post_warp_pillar_handling passes 2
s32 fn_80230CFC(s32 player);             // 37 units. -> field_0x3dc; else 2
// Owner: tu_8022AD64 (0x8022FD8C, defined there). The player index for actor `serial` (act_talking_sign
// stores it and passes it to fn_8023E724); negative when none.
s32 fn_8022FD8C(s32 serial);
void fn_80231368();                      // 4 units.
bool fn_80233D20(s32 player);            // 1 unit. -> byte 0x43F; else false
// 1 unit. -> fn_80240868 (word 0x4); 0 without a manager. An inactive
// player falls off the end with r3 still the index (the source has no final
// return); the caller uses the result (mr r26, r3 at 0x80306B44).
s32 fn_802340A0(s32 player);
JGeometry::TVec3f* fn_80238430(s32 player); // 1 unit. -> 0x324; else &lbl_804CF460
// Hits the player with `damage` at `pos` (fn_801F2B7C: fn_80238548(player, damage, &pos, arg)).
bool fn_80238548(s32 player, s32 damage, const JGeometry::TVec2f* pos, s32 arg); // reads pos+0/+4 only
// 5+ units (act_thwomp_switch, act_warp, ...). Same shape as fn_8023A51C below.
JGeometry::TVec3f* fn_8023A478(s32 player);
// Any-player version of fn_8023B0F4: loops players 0-3 that are present and returns the index
// of the first whose fn_802730D8 test (box, layer, flag, margin) hits, or -1 (also without a
// manager). act_zoom passes (&box, field_0x4, false, 0.0f); tested against 0 with cmpwi.
s32 fn_8023AF14(const JGeometry::TBox2f* box, s32 layer, bool flag, f32 margin);
s32 fn_8023B22C(s32 player);             // 2 units. -> word 0xB18; else -1
bool fn_8023B400(s32 player);            // 1 unit. -> byte 0xB78; else false
s32 fn_8023E10C(s32 player);             // 5 units. -> word 0xCE4; else 0
s32 fn_8023E190(s32 player);             // 1 unit. -> word 0xCE0; else 0
JGeometry::TVec3f* fn_8023E214(s32 player); // 3 units. -> 0xCD4; else &lbl_804CF460
JGeometry::TVec3f* fn_8023E2A0(s32 player); // 1 unit. -> 0xCC8; else &lbl_804CF460
JGeometry::TVec3f* fn_8023E32C(s32 player); // 5 units. -> 0xCB0; else &lbl_804CF460
JGeometry::TVec3f* fn_8023E578(s32 player); // 3 units. -> 0x14; else &lbl_804CF460
JGeometry::TVec3f* fn_8023E604(s32 player); // 3 units. -> 0x2C4; else &lbl_804CF460

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_invisible_npc_wall).
// Loops over the players and calls fn_80272DCC, which copies the 4 box floats
// and stores r5/r6 (field_0x4, serial) as words.
void fn_80239914(const JGeometry::TBox2f* box, s32 arg_0x4, s32 serial);

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_pull_switch). Index (0-3)
// of the first present player whose box test (fn_802728AC(player, box, layer,
// serial)) passes, or -1; 0 without a player manager.
s32 fn_80239470(const JGeometry::TBox2f* box, s32 layer, s32 serial);

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_eyeball_switch). Index (0-3)
// of the first present player whose box test (fn_80273C30(player, box, layer,
// out, flag), a bool OR of the player's tile probes) passes, or -1; -1
// without a player manager. The caller ignores the result.
s32 fn_8023BA68(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2<f32>* out, bool flag);

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_cactus). Whether
// fn_802729F0(player object, serial) passes for any active player.
bool fn_80239638(s32 serial);

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_burning_block). Counts
// (compared with 1 by the caller) the active players for which fn_80250030(player
// object, serial) passes.
s32 fn_80239108(s32 serial);

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_burning_block). Index
// (0-3, read from the player object's 0x3DC) of the first active player for which
// fn_80250030(player object, serial) passes, or -1 (fn_80239108 not positive).
s32 fn_80238BC4(s32 serial);

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_burning_block). Calls
// fn_8024FFB4(player object, serial) on every active player for which
// fn_80250030 passes.
void fn_80238E08(s32 serial);

// Owner unit tu_8022AD64 (not written yet). 1 unit (tu_80206B30 vfunc_0x58).
// Four sibling probes over the players: each calls a per-player box test
// (fn_802576CC / fn_80256F44 / fn_802574CC / fn_802575B8: player object,
// box, layer, out) and the test stores two floats through `out` (callers
// read it as a TVec2f). fn_8023AA18 and fn_8023AC74 return the index of the
// first player whose test passes, fn_8023AE34 and fn_8023AD54 the last one;
// all return -1 when none does (also without a player manager).
s32 fn_8023AE34(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
s32 fn_8023AA18(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
s32 fn_8023AC74(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
s32 fn_8023AD54(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
}

#endif
