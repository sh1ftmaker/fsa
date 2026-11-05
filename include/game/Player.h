#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

// The players. *gPlayerManager_8053AC90 (PlayerManager_804A6F28, game/tu_8022AD64.cpp)
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
// in fn_8022B7C4, fn_8022B860, getPlayerLayer_8023DE58 and getPlayerPos_8023E724:
//
//     if (gPlayerManager_8053AC90 == NULL) return <fallback>;
//     if (isPlayerActive(player)) return gPlayerManager_8053AC90->mPlayers[player]-><query>;
//     return <fallback>;
//
// isPlayerActive (below) is fn_8023F264's body, inlined; its own test of
// gPlayerManager_8053AC90 is the dead `bne` right after the outer test in every one of
// them (e.g. 0x8022B7E4). MWCC only inlines functions defined before the
// caller, and the queries come before fn_8023F264, so it is a header inline.
//
// Owner: the shared-header owner (docs/COORDINATOR.md, "Shared game
// headers"). Game-file agents propose changes as SHARED-EDITS.

#include "types.h"
#include "JSystem/JGeometry.h"
#include "game/cls_804A64A8.h"
#include "game/cls_8049F140.h"

// Vtable 0x804A6F38: only the destructor (0x802558E4). Constructor
// 0x80255B28 (this, index), called by PlayerManager_804A6F28's constructor
// for indices 0-3. Size 0x13BC: the manager's __nw__(0x13BC) (0x8023F904).
// Fields only where code outside the player's own units reads them inline;
// the player's getters (tu_8023FBF0) read the rest.
class cls_804A71A8;
class cls_804AA8D8;
class cls_804B20F8;
class JPABaseEmitter;

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
    void fn_80254660(); // tu_8023FBF0; the player's reset, run per slot by fn_8023F6C4
    void fn_80252FBC(); // tu_8023FBF0; empty, called per player slot from tu_8022AD64 (0x8023F504)
    bool fn_8023FC08();          // byte at 0x42C
    void fn_8023FC10(s32 value); // 0xCA8 = max(0xCA8, value)
    // Getters and setters the generated queries forward to (tools/gen_game.py).
    bool fn_80272224(const JGeometry::TBox2f* box, s32 layer, s32 type, bool a, bool b); // via fn_80239B64
    // Getters and setters the generated queries forward to (tools/gen_game.py).
    void fn_8023FBF0(); // via fn_80231928
    cls_804AA8D8* fn_8023FC24(); // via fn_8022BCD4
    s32 fn_8023FC2C(); // via fn_8023BC38
    u8 fn_8023FC5C() const; // via fn_8023BCD4
    bool fn_8023FF50(); // via fn_8023BD70
    bool fn_8023FF88(); // via fn_8023BE0C
    s32 fn_8023FFAC(); // via fn_8022BECC
    s32 fn_8023FFC4(); // via fn_80236630
    s32 fn_8024018C(); // via fn_802366CC
    s32 fn_80240354(); // via fn_80236768
    void fn_8024051C(); // via fn_802384BC
    s32 fn_80240644(); // via fn_8022F3DC
    bool fn_80240670() const; // via fn_8022F478
    bool fn_80240808(); // via fn_8022F514
    bool fn_80240818(); // via fn_8022DCD8
    bool fn_80240840(s32 arg); // via fn_8022E39C
    bool fn_80240860(); // byte at 0x436
    s32 fn_80240868(); // word 0x4 (the player index)
    bool fn_80240870(); // via fn_80234134
    void fn_80240878(); // via fn_80233F78
    s32 fn_80240888() const; // via fn_80234004
    bool fn_802408C0(); // byte at 0x1217
    void fn_802408C8(s32 a, const JGeometry::TVec3f* b); // via fn_802348C4
    bool fn_80240AD4(); // via fn_80234950
    bool fn_80240AF0(); // via fn_802349EC
    bool fn_80240AF8(); // via fn_80234A88
    bool fn_80240B00(); // via fn_80234B24
    bool fn_80240B20(); // via fn_80234BC0
    bool fn_80240B5C(); // via fn_80234C5C
    bool fn_80240B70(); // via fn_80234CF8
    bool fn_80240B84() const; // via fn_80233EDC
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
    u8 fn_802412C4();   // byte at 0x458
    void fn_802635C8(); // byte at 0x458 = 0 (stb r0)
    void fn_802412D4(); // via fn_802356B8
    bool fn_80241380(); // via fn_8023561C
    bool fn_802413C8(); // via fn_80235744
    bool fn_802413E8(); // via fn_80230188
    bool fn_802413F0(); // via fn_8023188C
    void fn_80241454(u8 value); // 0xC80 = value (stb r4)
    // Called by tu_8022AD64 (0x80237214, tests r3 with clrlwi.).
    bool fn_8024157C(s32 arg);
    // Whether pos is inside the view box for `layer`: the camera's
    // (fn_8030C2A8) on layers below 8, else this player's box (0xBEC).
    bool fn_80241DD0(s32 layer, const JGeometry::TVec3f* pos); // via fn_8022BA58
    bool fn_80241BA0(s32 layer, const JGeometry::TBox2f* box, bool flag); // via fn_8022B978
    bool fn_80241E28(s32 a, const JGeometry::TVec3f* b); // via fn_8022BC38
    void fn_80241E64(); // via fn_8022BD70
    void fn_80242560(); // via fn_80231424
    // Called by tu_8022AD64's fn_802314D8 with a TVec3f copied to its stack.
    bool fn_80242624(JGeometry::TVec3f pos);
    bool fn_80242704(s32 arg); // via fn_8023162C
    bool fn_802466B4(); // via fn_8022C3AC
    u32 fn_80246B10(u32 arg); // via fn_8022C52C; tested with cmplwi by fn_802870C8
    u32 fn_80246D1C(u32 arg); // via fn_8022C5EC
    u8 fn_80247534(); // via fn_8022C8EC
    bool fn_80247564(); // via fn_8022C9AC
    void fn_80248DAC(); // via fn_80234810
    bool fn_8024B108(s32 arg); // via fn_8022DE10
    bool fn_8024C4C4(s32 arg); // via fn_8022FBB8
    bool fn_8024C828(s32 arg); // via fn_8022FCF0
    void fn_8024CD20(const JGeometry::TBox2f* box, s32 layer, s32 serial, bool a, bool b); // via fn_8022FEB8
    // via fn_8022FE80; a is stored with stb at 0xD4E, b tested with clrlwi 24.
    void fn_8024CAC8(s32 layer, s32 serial, bool a, bool b);
    JGeometry::TVec3f* fn_8024E018(); // &field_0x2b8; via fn_8023D7B0's callers (tu_8022AD64)
    void fn_8024E020(const JGeometry::TVec3f* arg); // via fn_8022FFC4
    void fn_8024F54C(s32 arg); // via fn_80230F44
    bool fn_802570BC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // via fn_8023AB94
    bool fn_8025AA8C(s32 arg); // via fn_8022E23C
    void fn_8025EE0C(s32 a, const JGeometry::TVec3f* b); // via fn_8022F820
    bool fn_8025EE48(s32 arg); // via fn_8022F948
    bool fn_8025EFFC(s32 a, bool b, bool c); // via fn_8022F9E4
    bool fn_80260048(s32 arg); // via fn_80231230
    bool fn_80272768(const JGeometry::TBox2f* box, s32 layer, s32 serial); // via fn_80239558
    bool fn_802728AC(const JGeometry::TBox2f* box, s32 layer, s32 serial); // via fn_80239470
    bool fn_80272C20(const JGeometry::TBox2f* box, s32 arg_0x4, s32 serial); // via fn_802397B4
    bool fn_80273D44(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag); // via fn_8023BB50
    bool fn_80273FB8(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // via fn_8023A7BC
    u8 fn_80274238(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* pos, bool flag); // via fn_8023B8E4
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
    bool fn_80244FFC(); // tu_8023FBF0 (0x80244FFC): pad trigger 0x100 or 0x20 (gated)
    bool fn_80245238(); // tu_8023FBF0 (0x80245238): pad hold 0x100 / 0x20, or 0x200 on a flagged layer (gated)
    bool fn_802454C0(); // via fn_8022BF68
    bool fn_802456FC(); // via fn_8022C004
    bool fn_80245938(); // via fn_8022C0A0
    bool fn_80245B74(); // via fn_8022C13C
    bool fn_80245DB0(); // via fn_8022C1D8
    bool fn_80245FEC(); // via fn_8022C274
    bool fn_80246228(); // via fn_8022C310
    void fn_80248004(s32 a, bool b); // via fn_8022CA6C
    s32 fn_802486A0() const; // via fn_8022CAF8
    s32 fn_80248778() const; // via fn_8022E8D4
    s32 fn_80248970() const; // via fn_8022E970
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
    bool fn_8024D058(u8 reset); // via fn_80230224 (s32 flag -> clrlwi 24 at 0x802302A0) and two more in tu_8022AD64
    // via fn_80230498 (tu_8022AD64); act_battle_royale_end_check passes
    // frame, 0xFF, -0x80, -0x80, 1.
    bool fn_8024DC00(const JGeometry::TVec3f* pos, s32 frame, u8 a, s8 b, s8 c, bool check);
    s32 fn_8024DCD0(); // via fn_80230AD4
    bool fn_8024E03C(); // via fn_80230050
    bool fn_8024E060(); // via fn_802300EC
    void fn_8024E500(); // via fn_80235FE8
    s8 fn_8024EE0C(); // via fn_8023CB24
    s8 fn_8024EE14(); // via fn_8023CA88
    u16 fn_8024EE1C(); // via fn_8023CBC0
    bool fn_8024EFA0(bool arg); // via fn_80230E1C; called by fn_80292454 (0x3B4 >= 8 && 0xCE4 < 8)
    void fn_8024EFF8(u32 id); // via fn_80230EB8 (cmplwi on the id)
    void fn_8024F0E8(s32 value); // via fn_80230FD0
    bool fn_8024F5E4(); // via fn_8023E854
    bool fn_8024F630(); // via fn_8022DBA0
    s32 fn_8024FF54(); // via fn_80238EDC
    s32 fn_8024FF6C(); // via fn_8022D7F8
    s32 fn_8024FF84(); // via fn_80238CB4
    bool fn_80250090(); // via fn_8022D9CC
    // tu_802684A4 (0x802684A4): true when pos (x, y) is inside one of the
    // 0x50 boxes field_0x5fc[i] whose field_0xdb0[i] is set (>= 0) and whose
    // field_0x1030[i] is `layer`; it then copies field_0xdb0[i] into
    // field_0xef0[i]. cls_804A71A8::fn_80268BC4 has it inlined twice.
    bool fn_802684A4(const JGeometry::TVec3f* pos, s32 layer);
    // tu_8023FBF0 (0x80247330 / 0x8024712C): the pad axes fn_80309FCC /
    // fn_80309F20 of the player's slot behind the input gate, else 0; f32 results
    // that cls_804A71A8::fn_8026E1E0 compares with 0.0f for facings 0 / 1
    // (0x80247330) and 2 / 3 (0x8024712C).
    f32 fn_80247330();
    f32 fn_8024712C();
    void fn_802685D4(); // tu_802684A4 (0x568); reads the player's fields directly
    void fn_80268B3C(); // tu_802684A4; holds mPos.z at 0 across fn_802685D4
    bool fn_80250ACC(); // via fn_8022DA68
    bool fn_80250D30(); // via fn_8022DB04
    u8 fn_802517E4(); // via fn_8022D894
    bool fn_80251BFC(s32 arg); // via fn_802319B4
    bool fn_80251AEC(s32 arg); // tu_8023FBF0 (0x80251AEC); called by fn_8026E5F8
    bool fn_8026E5F8(const JGeometry::TVec2f* pos); // tu_802684A4 (0x8026E5F8)
    void fn_80252F14(const JGeometry::TVec3f* offset); // via fn_8023DFF4
    void fn_80252F40(s32 arg); // via fn_8023E080
    void fn_80252F50(s32 arg); // via fn_8023CC5C
    // tu_8026FC20 (0x8027A698) passes field_0x3dc and tests r3 (clrlwi.).
    u8 fn_8025196C(s32 index);
    void fn_80252F5C(s32 arg); // via fn_8023CCE8
    s32 fn_80252F68(); // via fn_8023CD74
    s32 fn_80252F74(); // via fn_8023CE10
    bool fn_802577E0(); // via fn_8023B484
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
    void fn_802635D4(); // empty; the dispatch in fn_802870C8 calls it
    void fn_802636D0(); // tu_8025EB18; empty, called by the same dispatch
    // Called by the same dispatch (tu_80285E58 fn_802870C8) with the player in r3.
    void fn_802632A4(); // empty
    void fn_80262234(); // forwards to fn_80258BBC
    void fn_80262B6C(); // forwards to fn_80259660
    void fn_80262B8C(); // forwards to fn_80283D2C
    void fn_8026173C(); // clears field_0x121c, field_0xd84 = 20
    void fn_80261468(); // counts field_0xd74 down to 0
    void fn_80259660(); // tu_80256C74
    void fn_80283D2C(); // tu_8027E5BC
    bool fn_8025F484() const; // field_0xd68 is a living actor (field_0x1a4 > 0)
    void fn_80277100(); // tu_8026FC20; empty, called by the same dispatch
    void fn_802791AC(); // tu_8026FC20; empty, called by the same dispatch
    void fn_80264410(); // byte at 0x458 = 0 (stb r0)
    void fn_80264768(); // byte at 0x458 = 0 (stb r0)
    void fn_802636C4(); // byte at 0x458 = 0 (stb r0)
    void fn_80263848(); // byte at 0x458 = 0 (stb r0)
    void fn_80263854(); // byte at 0x459 = 0 (stb r0)
    void fn_80293760(); // byte at 0x124D = 1 (stb r4), byte at 0x458 = 0 (stb r0)
    void fn_80293B1C(); // byte at 0x124D = 1 (stb r4), byte at 0x458 = 0 (stb r0)
    void fn_802954B8(); // 0x124D = 1 (stb), 0x458 = 0 (stb); TVec3f 0x344 zeroed
    void fn_80295240(); // 0x124D = 1, 0x458 = 0 (stb); TVec3f 0x344 set (0x348 from lbl_80540644)
    void fn_80292E90(); // 0x124D = 1, 0x458 = 0, 0x3FD = 1 (stb); then fn_8024F0E8(0x2000C)
    void fn_802938B8(); // 0x124D = 1, 0x458 = 0, 0x3FD = 1 (stb); then fn_8024F0E8(0x2000C) unless 0xCF4 == 0x1D
    void fn_802922D0(); // bytes 0x124D = 1, 0x458 = 0, 0x3FB = 1, 0x3F9 = 0 (stb)
    void fn_80292454(); // 0x124D = 1, 0x458 = 0 (stb); 0x3DC = 3, 0x3E8 = 3 (stw); fn_8024EFA0(0) if 0x3B4 >= 8 && 0xCE4 < 8
    void fn_802925C4(); // 0x124D = 1, 0x458 = 0 (stb); 0x3DC = 2, 0x3E8 = 2 (stw)
    void fn_80292124(); // 0x124D = 1, 0x458 = 0 (stb); 0x3DC = 3, 0x3E8 = 3 (stw); fn_8024EFA0(0) if !fn_80240670() && 0x3B4 >= 8 && 0xCE4 < 8
    // Copies 0xCB0 (TVec3f), lowers its y by 16 unless 0xCF2 is set, then calls
    // fn_80290070(0, &mPos, &copy, 5); on success clears 0xBDC and returns true.
    bool fn_8028FBA0();
    // Not written yet (tu_8028FBA0); this in r3, &mPos passed as the second position.
    bool fn_80290070(s32 a, const JGeometry::TVec3f* b, const JGeometry::TVec3f* c, s32 d);
    // Not written yet (tu_8028FBA0's fn_80290E90 is 0x90 bytes): fn_80301D54(field_0x4) unless
    // fn_80240670(), clears 0x400, then fn_8045815C on gAudioMgr_8053AF80 (0 below 8, 1 at 8) by 0xCE4.
    void fn_80290E90();
    void fn_80295584(); // 0x124D = 1, 0x3FD = 1 (stb), 0x458 = 0 (stb); 0x3DC = 2, 0x3E8 = 2 (stw)
    s32 fn_8026FC20(); // via fn_8023A1A4
    bool fn_8026FC84(); // via fn_8023A5B8
    bool fn_8026FCB0(); // via fn_8023A3DC
    bool fn_8026FCE0(); // via fn_8023A51C
    s32 fn_802702E4(); // via fn_802357E0
    void fn_802702EC(); // via fn_80235918
    s32 fn_80270310(); // via fn_8023587C
    s32 fn_80270350(); // via fn_80235A30
    s32 fn_80270358(); // via fn_80235ACC
    s32 fn_8026FC68(); // called by fn_8023A2C8 (tu_8022AD64)
    void fn_8026FC8C(); // called by fn_8022B030 and fn_8022B300 (tu_8022AD64)
    JGeometry::TVec3f* fn_8026FCC4(); // called by fn_8023A4F4 (tu_8022AD64)
    void fn_8027023C(); // called by fn_80255250 (tu_8023FBF0)
    void fn_802702A4(); // called by tu_8025EB18
    void fn_802702C4(); // called by tu_8023FBF0
    void fn_80270D34(s32 amount); // called by tu_8022AD64; subtracted from field_0xbfc
    // Damage / hit from player `attacker` at `pos` with hit kind `kind` (0x1B, 0xD-0xF, 0x16,
    // 0x18 ...): knockback (field_0x350 / field_0x35c), facing, state request. Called by
    // fn_80238548 (tu_8022AD64) for each player.
    bool fn_802710C8(s32 attacker, s32 damage, const JGeometry::TVec2f* pos, s32 kind);
    void fn_80277104(); // called by tu_80285E58
    void fn_802771BC(); // field_0x458 = 0; called by tu_80285E58
    // tu_80256C74
    void fn_80257FF4(); // drops field_0xb30 serials with no actor
    void fn_80258070(); // drops field_0xb2c if its actor is gone or not a live Somaria block
    void fn_8025819C(); // tracks the actor field_0xb3c: its layer and box (+ position) into 0xb70 / 0x5a0
    void fn_80258384(); // the same for each of field_0xb1c[3]: field_0xb5c[] / field_0x550[]
    void fn_802585F4(); // the same for each of field_0xb00[3]: field_0xb40[] / field_0x4e0[]
    void fn_80258470(); // same for field_0xb18: 0xb58 / 0x540
    void fn_8025851C(); // the same for each of field_0xb0c[3]: field_0xb4c[] / field_0x510[]
    void fn_802586CC(); // empty
    void fn_80258880(); // field_0x458 = 0
    void fn_8025888C(); // empty
    void fn_80258BA8(); // clears 0x458, 0xb9f, 0xba0
    void fn_80258BBC(); // clears 0xb79 and 0x1204; mPos.z = 0 unless 0x1244 is 5 or 7
    void fn_80259778(); // field_0x34c = 5.4f
    void fn_80259784(); // ends the emitters at 0x12c4 and 0x12c8, kills the actor field_0xb28 (fn_802BAA00)
    void fn_8025A0B0(); // same body as fn_80259784
    void fn_8025A04C(); // clears 0x458; field_0xb84 = fn_80241794() > 0; 0xb88, 0xb8c = 0, 0xb28 = -1
    void fn_8025A9C0(); // same body as fn_8025A04C
    void fn_8025AAF4(); // field_0x11f8 = -1
    void fn_8025AEB0(); // field_0x124d = 1, field_0x458 = 0
    void fn_8025AEC4(); // field_0x11f8 = -1
    void fn_8025B218(); // field_0x124d = 1, field_0x458 = 0
    void fn_8025B35C(); // field_0x458 = 0
    void fn_8025B368(); // clears 0xb99-0xb9b, ends the emitters at 0x1274 and 0x128c, clears 0x40c
    void fn_8025BDD8(); // ends the emitters at 0x12cc and 0x12e4, kills the actor field_0x12c0, clears 0x40c
    void fn_8025BDC8(); // clears 0x458 and 0x40c
    void fn_8025C79C(); // clears 0x458, field_0x344 and 0x40c
    void fn_8025C7BC(); // same body as fn_8025BDD8
    void fn_8025D090(); // clears 0x458, field_0x344 and 0x40c
    void fn_8025D6A0(); // field_0xb9c = 0
    // tu_8027E5BC (dispatch targets of tu_80285E58); fn_80248604, fn_8024D134 and fn_8024E3D8 are tu_8023FBF0 (r3 = Player)
    void fn_8027EE84();
    void fn_8026152C(bool arg); // tu_8025EB18; field_0x2f4 from mPos (carry), called by fn_802948C0
    void fn_8027F16C();
    void fn_80281E84();
    void fn_8028211C();
    void fn_8028286C();
    void fn_80282AA8();
    void fn_80282FC8();
    void fn_80283944();
    void fn_80283CEC();
    void fn_80283D00();
    void fn_80283D20();
    void fn_80284290();
    void fn_80284328();
    void fn_802848F0();
    void fn_8028496C();
    void fn_80284DD0();
    void fn_80284DE4();
    void fn_80285068();
    void fn_80285074();
    void fn_8028537C();
    void fn_80285388();
    void fn_80285668();
    void fn_802857C8();
    void fn_802859B8();
    void fn_80285C28();
    void fn_8027EB04();
    void fn_80284294();
    void fn_80282A24();
    // tu_80285E58: the update (fn_80285E58, from fn_80253E6C), the state tick
    // fn_802870C8 and the helpers it calls.
    void fn_80285E58();
    void fn_80285F44();
    void fn_80286688();
    void fn_8028676C();
    void fn_80286B74();
    void fn_80286E10();
    void fn_802870C8();
    // State handlers (update / enter / exit) the three switches of fn_802870C8
    // call, in address order; tables in docs/PLAYER.md. Not written yet.
    void fn_802586D0(); void fn_80258890(); void fn_80258BE8(); void fn_80259830();
    void fn_8025A15C(); void fn_8025AB00(); void fn_8025AED0(); void fn_8025B22C();
    void fn_8025B3F0(); void fn_8025BEB0(); void fn_8025C894(); void fn_8025D6AC();
    void fn_8025D7E4(); void fn_8025D9FC(); void fn_8025DACC(); void fn_8025E79C();
    void fn_8025D0B0(); // tu_80256C74; the step of state 0x39's update fn_8025DACC (pad-driven slide)
    // tu_80256C74 inline (fn_80259660, three times in fn_80258BE8): in mode 5 (fn_802486A0)
    // and not following (fn_80240808), every other present player to mPos (fn_8023E690), then fn_8024BAEC.
    void gatherOthers();
    void fn_80261750(); void fn_80262188(); void fn_80262254(); void fn_80262BAC();
    void fn_80263078(); void fn_802632A8(); void fn_802635D8(); void fn_802636D4();
    void fn_80263860(); void fn_80263BA0(); void fn_80263C64(); void fn_8026441C();
    void fn_802771C8(); void fn_80277220(); void fn_802775C8(); void fn_8027776C();
    void fn_80277EBC(); void fn_80277F98(); void fn_802791B0(); void fn_802796B8();
    void fn_80279824(); void fn_80279CFC(); void fn_80279FE8(); void fn_8027A0D4();
    void fn_8027A178(); void fn_8027A1E0(); void fn_8027A908(); void fn_8027AAD0();
    void fn_8027B304(); void fn_8027B648(); void fn_8027BA2C(); void fn_8027BC90();
    void fn_8027BE74(); void fn_8027C4FC(); void fn_8027C6C4(); void fn_8027CBB0();
    void fn_8027CC90(); void fn_8027D320(); void fn_8027D5F0(); void fn_8027DEA0();
    void fn_80280DD8(); void fn_80280E44(); void fn_802811C0(); void fn_8028126C();
    void fn_80281760(); void fn_80281A18(); void fn_80281A90(); void fn_80281D50();
    void fn_80281E98(); void fn_80282094(); void fn_80282178(); void fn_80282ACC();
    void fn_80282E6C(); void fn_80282FE0(); void fn_80283948(); void fn_80283E70();
    void fn_802844F8(); void fn_80284978(); void fn_80284E04(); void fn_80285098();
    void fn_8028538C(); void fn_80285690(); void fn_802857F0(); void fn_802859C4();
    void fn_80290D58(); void fn_80290F20(); void fn_80290FC0(); void fn_8029185C();
    void fn_80291910(); void fn_80291B64(); void fn_80291B8C(); void fn_80291CB8();
    void fn_80291D28(); void fn_80291E44(); void fn_80291F10(); void fn_80292198();
    void fn_802922EC(); void fn_802924AC(); void fn_802925E4(); void fn_802927B0();
    void fn_80292820(); void fn_80292944(); void fn_802929FC(); void fn_80292ECC();
    void fn_802932BC(); void fn_80293398(); void fn_80293534(); void fn_802935A4();
    void fn_80293774(); void fn_80293900(); void fn_80293B30(); void fn_80293E8C();
    void fn_80293FE4(); void fn_80294374(); void fn_802948C0(); void fn_80294ADC();
    void fn_8029513C(); void fn_80295268(); void fn_80295334(); void fn_802953A0();
    void fn_802954DC();
    // tu_8028FBA0 inline (the enters of 0x23 and 0x25): carried actor to mPos + 16 up.
    void raiseHeldActor();
    // tu_8028FBA0 inline (the updates of 0x2A and 0x2B): facing turns with the counter 0x3FC.
    void spinFacing();
    // tu_8028FBA0 inlines: the 0x3F8/0x3F9 walk cycle; field_0x2f4 = mPos raised 16.
    void stepWalkCycle();
    void setHeldPos();
    void stepTransitionAnim(); // tu_8028FBA0 inline: walk / push / carry animation step
    void spawnEffect17D(); // tu_8028FBA0 inline: effect 0x17D above the player
    void followCamera();  // tu_8028FBA0 inline: camera on mPos - (0, 150) on the TV
    bool requestArrival(); // tu_8028FBA0 inline: 0x33 / 0x3E / 0x12 / 0 after an arrival
    void spawnEffect15B(); // tu_8028FBA0 inline: effects 0x15B, 0x15C at the player
    s32 life(); // tu_8028FBA0 inline: fn_80237774(fn_80240644()) if fn_80240808(), else fn_8024189C()
    void fallStep(); // tu_8028FBA0 inline: gravity on 0x344, then mPos += 0x344
    // tu_8026FC20 inline, defined at the top of that unit: fn_80270A8C's body (its out-of-line
    // copy, which the other units call). fn_80270948 and fn_8027057C, before fn_80270A8C, have
    // it inlined while the unit's .sdata2 order rules out deferred inlining, so it is an inline.
    // State 0x33 (requested when the life 0xBFC reaches 0) requested or current, or 0x34.
    bool isDown();
    void riseToTarget(s32 state); // tu_8028FBA0 inline: rise above 128, then request `state`
    // Called by fn_802870C8 after the state update (r3 = this; results unused
    // unless typed).
    void fn_80242940();
    void fn_80242BC0();
    void fn_80242CB0();
    bool fn_80246464(); // clears field_0xcad when false
    void fn_8024762C(); // state-request helper (stores 0x1244 = 0/1/2/0xB)
    void fn_8024BBB8();
    void fn_8024EE24();
    void fn_8025250C();
    void fn_8025785C();
    void fn_802580E0();
    void fn_8025825C();
    void fn_80265AE4();
    void fn_80265D68();
    void fn_80265D8C();
    void fn_80265DB0();
    void fn_80280B30();
    void fn_80265E88(); // tu_80264998 (skeleton / animation), called by fn_80285E58
    // tu_80264998: the frame functions fn_80265E88 calls per state (r3 = this).
    void fn_80267018();
    void fn_802670CC();
    void fn_802671C8();
    void fn_80267384();
    void fn_80267470();
    void fn_80267598();
    void fn_80267684();
    void fn_80267748();
    void fn_80267818();
    void fn_802678CC();
    void fn_80267990();
    void fn_80267A54();
    void fn_80267B18();
    void fn_80267BA8();
    void fn_80267C6C();
    void fn_80267D30();
    void fn_80267E18();
    void fn_80267ECC();
    void fn_80268194();
    JGeometry::TVec3f* fn_80252F48(); // returns &field_0x11b4 (tu_8023FBF0)
    bool fn_80275D9C(); // tu_8026FC20 (li r3, 1 / 0); true ends the state updates of tu_8027E5BC early
    void fn_8027FA08(JGeometry::TVec3f* out); // tu_8027E5BC; called by update B and fn_8028676C
    void fn_802808E8(f32 speed); // tu_8027E5BC: the stick walk step (states 2 / 3)
    bool fn_8025197C(bool arg); // tu_8023FBF0
    void fn_80284430();
    void fn_8028447C();
    void fn_80247F58();
    void fn_80248604();
    void fn_80248DD8();
    void fn_8024B2E0(); // tu_8023FBF0 (0x708 bytes, unwritten); r3 = Player
    void fn_8024D134();
    void fn_8028414C();
    void fn_8027E7E0();
    void fn_8027E5BC(f32 speed);
    void fn_8027EDDC();
    void fn_8027EEB4(); // steps the walk-animation counter field_0x3f8 (frame field_0x3f9)
    void fn_8027F1C8(); // places or spawns the effect in field_0x1388 (ids 0x35E-0x361)
    void fn_8027F854(JGeometry::TVec3f* out, s32 player); // player's position plus an offset by fn_802486A0
    s32 fn_802487FC() const; // tu_8023FBF0 (0x802487FC); a count of players (r3 only)
    void fn_80283848();
    void fn_8028434C();
    void fn_8024E344();
    void fn_8024CA20();
    void fn_8024E3D8();
    void fn_8024E46C();
    void fn_802810E4();
    void fn_802828A8();
    void fn_80282958();
    void fn_8027769C(); // called by tu_80285E58
    void fn_80277E18(); // called by tu_80285E58
    void fn_8027AA10(); // called by tu_80285E58
    bool fn_80272B4C(const JGeometry::TVec3f* pos); // called by tu_8025EB18
    void fn_802790C0(); // called by tu_80285E58
    void fn_8027D4F4(); // called by tu_80285E58
    bool fn_80270ABC(); // Not written yet (0x1CC bytes). bool: its body only returns li 1 / li 0 and a neg/andc/srwi test; fn_80270C88 returns it unchanged as its bool
    void fn_802709C8(); // BgRewrite fn_802A20F0 on field_0x30c; called by tu_80256C74
    void fn_80270D08(); // counts field_0x464 and field_0x468 down; called by tu_8023FBF0 and tu_80285E58
    bool fn_80272160(s32 damage, const JGeometry::TVec2f* pos, s32 arg); // fn_80238548 on this player; called with (0, &mPos, 0x12)
    void fn_8027221C(s32 value); // field_0xbf4 = value
    // tu_8026FC20, not written yet; called by the reset fn_80254744 and update A.
    void fn_80276538(); // sets the box field_0x4c0 around mPos
    void fn_80276078();
    // The per-frame calls of the player's update A (fn_80253E6C), all on
    // this player and not written yet: tu_8023FBF0 fn_802490A8 (input and
    // actions), fn_80251D50, fn_8024BDBC, fn_8024E0A0; tu_8026FC20
    // fn_8026FD14; tu_80264998 fn_80265E88 and tu_80285E58 fn_80285E58 /
    // fn_80286688 (declared above).
    void fn_80253E6C();
    void fn_802490A8();
    void fn_80251D50();
    void fn_8024BDBC();
    void fn_8024E0A0();
    void fn_8026FD14();
    void fn_80270088(); // called by fn_8027DEA0 (enter state 4)
    void fn_80270360(s32 serial); // called by tu_8022AD64 (fn_80235B68)
    void fn_80274728(); // called by fn_8027AAD0 (update of state 0x0A)
    void fn_80274988(); // called by fn_8027B648 (update of state 0x09)
    void fn_8027439C(); // called by fn_8027A1E0 (update of state 0x0B): pose 0x3FA / 0x3FB, boxes 0x4A0 / 0x4B0
    void fn_802765C0();
    bool fn_802738F0(const JGeometry::TBox2f* box, JGeometry::TVec2f* out, bool flag, bool center);
    u8 fn_80273E7C(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag); // called by tu_8022AD64
    // The manager's other per-player calls (fn_8023F390 / fn_8023F2BC):
    // update B (tu_8023FBF0, not written yet) and the draw (tu_80264998).
    void fn_80252FC0();
    void fn_80264998();
    void fn_80277580(); // called by tu_80285E58
    void fn_8027761C(); // called by tu_80285E58
    void fn_8027774C(); // called by tu_80285E58
    void fn_8027966C(); // called by tu_80285E58
    void fn_80279F94(); // called by tu_80285E58
    void fn_8027B5F0(); // called by tu_80285E58
    void fn_8027C6B8(); // called by tu_80285E58
    void fn_8027CAA8(); // called by tu_80285E58
    void fn_8027CB24(); // called by tu_80285E58
    void fn_8027CB44(); // called by tu_80285E58
    void fn_8027CB9C(); // called by tu_80285E58
    void fn_802708D0(); // via fn_80236350
    void fn_802708E8(); // via fn_802363DC
    bool fn_80270A04(); // via fn_80236A18
    bool fn_80270A58(); // via fn_80236AB4
    bool fn_80270A8C(); // via fn_80236B50
    bool fn_80270C88(); // via fn_80236BEC
    void fn_80270F00(s32 arg); // via fn_80236804
    // via fn_80236554 (act_recovery_heart heals 4): adds `amount` to the life 0xBFC (or takes
    // the group's, fn_802376D8), capped at 0xBF8; once per 0xD35, which is set to `flag`.
    void fn_80270DDC(s32 amount, bool flag);
    void fn_802422D0(); // via fn_80231368 (tu_8022AD64)
    const JGeometry::TVec2f* fn_802422C4(); // &field_0xbf0->field_0x14; via fn_8022DFD4 (tu_8022AD64)
    void fn_80248E94(bool flag); // via fn_8022AF78 (tu_8022AD64); clrlwi 24, compared with 1
    void fn_80270948(const JGeometry::TVec3f* pos); // called by tu_8023FBF0 fn_802422D0; 0xD10 = pos (mPos if NULL) once while down
    void fn_8027057C(); // called by fn_80277F98 (update of state 0x33): revive; not written (inlines fn_80270DDC)
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
    /* 0x0008 */ JGeometry::TVec3f mPos; // getPlayerPos_8023E724 (193 units)
    // Address returned by fn_8023E578.
    /* 0x0014 */ JGeometry::TVec3f field_0x14;
    /* 0x0020 */ JGeometry::TVec3f field_0x20; // fn_80254744: copy of field_0x14
    // fn_802685D4's step is mPos - field_0x2c; it copies mPos here at the end.
    /* 0x002C */ JGeometry::TVec3f field_0x2c;
    // 0x31 positions (fn_80252CF0 shifts them up one, last store at 0x278); read by fn_8023E4E4.
    /* 0x0038 */ JGeometry::TVec3f field_0x38[0x31];
    // 0x31 bytes (fn_80252CF0 shifts them, lbz 0x283 -> stb 0x284); read by fn_8023E45C.
    /* 0x0284 */ u8 field_0x284[0x31];
    /* 0x02B5 */ u8 field_0x2b5[0x2B8 - 0x2B5];
    // fn_8024E020 stores a TVec3 here; fn_8024E018 returns its address.
    /* 0x02B8 */ JGeometry::TVec3f field_0x2b8;
    // Address returned by fn_8023E604.
    /* 0x02C4 */ JGeometry::TVec3f field_0x2c4;
    /* 0x02D0 */ JGeometry::TVec3f field_0x2d0;
    /* 0x02DC */ JGeometry::TVec3f field_0x2dc;
    /* 0x02E8 */ JGeometry::TVec3f field_0x2e8;
    /* 0x02F4 */ JGeometry::TVec3f field_0x2f4;
    /* 0x0300 */ JGeometry::TVec3f field_0x300; // fn_8025EE0C stores its argument
    /* 0x030C */ JGeometry::TVec3f field_0x30c;
    /* 0x0318 */ JGeometry::TVec3f field_0x318;
    // The x and y that fn_8026E5F8 stores (its z is left alone).
    /* 0x0324 */ JGeometry::TVec2f field_0x324;
    /* 0x032C */ JGeometry::TVec3f field_0x32c;
    // fn_80254744: mPos, or (battle mode) mPos moved 16 along the facing field_0x3dc.
    /* 0x0338 */ JGeometry::TVec3f field_0x338;
    // Zeroed by fn_802954B8 (three stfs of 0.0f).
    /* 0x0344 */ JGeometry::TVec3f field_0x344;
    /* 0x0350 */ JGeometry::TVec3f field_0x350; // psq_l by tu_8026FC20 (0x80279844)
    /* 0x035C */ JGeometry::TVec3f field_0x35c;
    /* 0x0368 */ JGeometry::TVec3f field_0x368;
    /* 0x0374 */ JGeometry::TVec3f field_0x374;
    /* 0x0380 */ JGeometry::TVec3f field_0x380;
    /* 0x038C */ f32 field_0x38c;
    /* 0x0390 */ f32 field_0x390;
    /* 0x0394 */ s32 field_0x394;
    /* 0x0398 */ JGeometry::TVec3f field_0x398;
    /* 0x03A4 */ JGeometry::TVec3f field_0x3a4;
    /* 0x03B0 */ s32 field_0x3b0;
    // The player's layer: compared with ActorBase_804A5770::field_0x4 by
    // fn_801F320C and fn_801F5790; fn_801F329C tests it against 8.
    /* 0x03B4 */ s32 field_0x3b4;
    /* 0x03B8 */ s32 field_0x3b8;
    // Copy of field_0x3b4 that fn_802685D4 stores each frame.
    /* 0x03BC */ s32 field_0x3bc;
    // fn_802685D4 sets field_0x3c0 (li 1, stb); it clears field_0x3c1 and 0x3c2.
    /* 0x03C0 */ bool field_0x3c0;
    /* 0x03C1 */ bool field_0x3c1;
    /* 0x03C2 */ bool field_0x3c2;
    /* 0x03C3 */ u8 field_0x3c3[0x3C4 - 0x3C3];
    /* 0x03C4 */ u16 field_0x3c4;
    /* 0x03C6 */ u16 field_0x3c6; // fn_80254744: copy of field_0x3c4
    /* 0x03C8 */ u16 field_0x3c8; // lhz in tu_80264998
    /* 0x03CA */ u16 field_0x3ca;
    /* 0x03CC */ s32 field_0x3cc;
    /* 0x03D0 */ s32 field_0x3d0;
    /* 0x03D4 */ s32 field_0x3d4;
    // Signed count read by fn_8024EE14 (tu_8023FBF0) and fn_8023CA88.
    /* 0x03D8 */ s8 field_0x3d8;
    // Signed count read by fn_8024EE0C (tu_8023FBF0) and fn_8023CB24.
    /* 0x03D9 */ s8 field_0x3d9;
    /* 0x03DA */ u8 field_0x3da[0x3DC - 0x3DA];
    /* 0x03DC */ s32 field_0x3dc; // fn_80230CFC (2 without a player)
    /* 0x03E0 */ s32 field_0x3e0;
    /* 0x03E4 */ s32 field_0x3e4;
    /* 0x03E8 */ s32 field_0x3e8; // fn_80230B70 (2 without a player)
    /* 0x03EC */ s32 field_0x3ec;
    /* 0x03F0 */ s32 field_0x3f0;
    /* 0x03F4 */ s32 field_0x3f4;
    /* 0x03F8 */ u8 field_0x3f8;
    // Cleared by fn_802922D0 (stb r0); 0x3FB set by it (stb r4).
    /* 0x03F9 */ u8 field_0x3f9;
    /* 0x03FA */ u8 field_0x3fa;
    /* 0x03FB */ u8 field_0x3fb;
    /* 0x03FC */ u8 field_0x3fc;
    /* 0x03FD */ u8 field_0x3fd; // set to 1 by fn_80295584 (stb)
    /* 0x03FE */ u8 field_0x3fe;
    /* 0x03FF */ u8 field_0x3ff;
    // Cleared (stb 0) by fn_80290E90 before it checks gGameRoot_8053AAF8->field_0x7e.
    /* 0x0400 */ u8 field_0x400;
    /* 0x0401 */ u8 field_0x401;
    /* 0x0402 */ u8 field_0x402;
    /* 0x0403 */ u8 field_0x403;
    /* 0x0404 */ u8 field_0x404;
    /* 0x0405 */ u8 field_0x405[0x408 - 0x405];
    /* 0x0408 */ s32 field_0x408;
    /* 0x040C */ s32 field_0x40c;
    /* 0x0410 */ f32 field_0x410;
    /* 0x0414 */ JGeometry::TVec3f field_0x414;
    // Offset added to mPos at the end of fn_802685D4 (ps_add). Zeroed while
    // mPos.z > 0; otherwise its x and y decay under the tile code.
    /* 0x0420 */ JGeometry::TVec3f field_0x420;
    // Read by fn_8023FC08 (tu_8023FBF0); fn_8023FBF0 sets the byte after it.
    /* 0x042C */ bool field_0x42c;
    /* 0x042D */ u8 field_0x42d;
    /* 0x042E */ u8 field_0x42e[0x430 - 0x42E];
    /* 0x0430 */ s32 field_0x430;
    /* 0x0434 */ u8 field_0x434;
    /* 0x0435 */ u8 field_0x435;
    // Returned by fn_80240860 (lbz; blr).
    /* 0x0436 */ bool field_0x436;
    /* 0x0437 */ u8 field_0x437;
    /* 0x0438 */ u8 field_0x438;
    /* 0x0439 */ u8 field_0x439;
    /* 0x043A */ u8 field_0x43a;
    /* 0x043B */ u8 field_0x43b;
    /* 0x043C */ u8 field_0x43c;
    /* 0x043D */ u8 field_0x43d;
    /* 0x043E */ u8 field_0x43e;
    // Returned by fn_80233D20 (lbz; blr).
    /* 0x043F */ bool field_0x43f;
    /* 0x0440 */ bool field_0x440;
    /* 0x0441 */ bool field_0x441;
    /* 0x0442 */ u8 field_0x442[0x444 - 0x442];
    /* 0x0444 */ f32 field_0x444;
    /* 0x0448 */ s32 field_0x448;
    /* 0x044C */ s32 field_0x44c;
    /* 0x0450 */ u16 field_0x450; // lhz at 0x8024DA2C and in tu_80264998
    /* 0x0452 */ u16 field_0x452;
    // Returned by fn_802413E8 (lbz; blr), read through fn_80230188.
    /* 0x0454 */ bool field_0x454;
    /* 0x0455 */ u8 field_0x455;
    /* 0x0456 */ u8 field_0x456;
    /* 0x0457 */ u8 field_0x457;
    // Returned by fn_802412C4 (a byte, read by its caller in tu_80256C74).
    /* 0x0458 */ u8 field_0x458;
    // Cleared by fn_80263854 (stb r0).
    /* 0x0459 */ u8 field_0x459;
    /* 0x045A */ u8 field_0x45a[0x45C - 0x45A];
    /* 0x045C */ s32 field_0x45c;
    // Returned by fn_802412CC (lbz; blr), read through fn_80235448.
    /* 0x0460 */ bool field_0x460;
    /* 0x0461 */ u8 field_0x461[0x464 - 0x461];
    /* 0x0464 */ s32 field_0x464;
    /* 0x0468 */ s32 field_0x468;
    /* 0x046C */ s32 field_0x46c;
    /* 0x0470 */ s32 field_0x470;
    /* 0x0474 */ JGeometry::TVec3f field_0x474;
    /* 0x0480 */ s32 field_0x480;
    /* 0x0484 */ u8 field_0x484;
    /* 0x0485 */ u8 field_0x485[0x488 - 0x485];
    /* 0x0488 */ s32 field_0x488;
    /* 0x048C */ s32 field_0x48c;
    /* 0x0490 */ u8 field_0x490;
    /* 0x0491 */ u8 field_0x491;
    /* 0x0492 */ u8 field_0x492;
    /* 0x0493 */ u8 field_0x493[0x494 - 0x493];
    /* 0x0494 */ s32 field_0x494;
    /* 0x0498 */ u8 field_0x498;
    /* 0x0499 */ u8 field_0x499;
    /* 0x049A */ u8 field_0x49a[0x49C - 0x49A];
    /* 0x049C */ s32 field_0x49c;
    // Two boxes tested by fn_802738F0 (view-relative, against its box argument).
    /* 0x04A0 */ JGeometry::TBox2f field_0x4a0;
    /* 0x04B0 */ JGeometry::TBox2f field_0x4b0;
    /* 0x04C0 */ JGeometry::TBox2f field_0x4c0;
    /* 0x04D0 */ JGeometry::TBox2f field_0x4d0;
    /* 0x04E0 */ JGeometry::TBox2f field_0x4e0[3];
    /* 0x0510 */ JGeometry::TBox2f field_0x510[3];
    /* 0x0540 */ JGeometry::TBox2f field_0x540;
    /* 0x0550 */ JGeometry::TBox2f field_0x550[3];
    // The item box of states 0x44 / 0x45 (fn_8025A15C / fn_80259830 store a per-facing box built on
    // the stack: i at 0x580, f at 0x588); fn_802570BC tests points against it (lfs 0x580 / 0x588 vs x,
    // 0x584 / 0x58C vs y).
    /* 0x0580 */ JGeometry::TBox2f field_0x580;
    /* 0x0590 */ JGeometry::TBox2f field_0x590;
    /* 0x05A0 */ JGeometry::TBox2f field_0x5a0;
    /* 0x05B0 */ JGeometry::TBox2f field_0x5b0;
    /* 0x05C0 */ JGeometry::TVec3f field_0x5c0;
    /* 0x05CC */ JGeometry::TBox2f field_0x5cc;
    /* 0x05DC */ JGeometry::TBox2f field_0x5dc; // set by tu_8026FC20 fn_80277F98, read by tu_80256C74
    // The 8x16 box in front of the player for direction field_0x3ec (fn_80276078).
    /* 0x05EC */ JGeometry::TBox2f field_0x5ec;
    /* 0x05FC */ JGeometry::TBox2f field_0x5fc[0x50];
    /* 0x0AFC */ u8 field_0xafc;
    /* 0x0AFD */ u8 field_0xafd[0xB00 - 0xAFD];
    /* 0x0B00 */ s32 field_0xb00[3];
    /* 0x0B0C */ s32 field_0xb0c[3];
    // Returned by fn_8023B22C (-1 without a player).
    /* 0x0B18 */ s32 field_0xb18;
    /* 0x0B1C */ s32 field_0xb1c[3];
    /* 0x0B28 */ s32 field_0xb28;
    /* 0x0B2C */ s32 field_0xb2c;
    /* 0x0B30 */ s32 field_0xb30[2];
    /* 0x0B38 */ s32 field_0xb38;
    /* 0x0B3C */ s32 field_0xb3c;
    /* 0x0B40 */ s32 field_0xb40[3];
    /* 0x0B4C */ s32 field_0xb4c[3];
    /* 0x0B58 */ s32 field_0xb58;
    /* 0x0B5C */ s32 field_0xb5c[3];
    /* 0x0B68 */ u8 field_0xb68;
    /* 0x0B69 */ u8 field_0xb69[0xB6C - 0xB69];
    /* 0x0B6C */ s32 field_0xb6c;
    /* 0x0B70 */ s32 field_0xb70;
    /* 0x0B74 */ s32 field_0xb74;
    // Returned by fn_8023B400 (lbz; blr).
    /* 0x0B78 */ bool field_0xb78;
    /* 0x0B79 */ u8 field_0xb79;
    /* 0x0B7A */ bool field_0xb7a;
    /* 0x0B7B */ bool field_0xb7b;
    /* 0x0B7C */ s32 field_0xb7c;
    /* 0x0B80 */ s32 field_0xb80;
    /* 0x0B84 */ bool field_0xb84;
    /* 0x0B85 */ u8 field_0xb85[0xB88 - 0xB85];
    /* 0x0B88 */ s32 field_0xb88;
    /* 0x0B8C */ s32 field_0xb8c;
    /* 0x0B90 */ f32 field_0xb90; // fn_80254660: 0.0f, or 256/3 unless gGameRoot_8053AAF8->field_0x7f
    /* 0x0B94 */ s32 field_0xb94;
    /* 0x0B98 */ u8 field_0xb98;
    /* 0x0B99 */ bool field_0xb99;
    /* 0x0B9A */ bool field_0xb9a;
    // Returned by fn_8026FC84 (lbz; blr), read through fn_8023A5B8.
    /* 0x0B9B */ bool field_0xb9b;
    /* 0x0B9C */ bool field_0xb9c;
    /* 0x0B9D */ bool field_0xb9d;
    /* 0x0B9E */ bool field_0xb9e;
    // Set by fn_8023A654.
    /* 0x0B9F */ bool field_0xb9f;
    /* 0x0BA0 */ s32 field_0xba0;
    /* 0x0BA4 */ u8 field_0xba4;
    /* 0x0BA5 */ u8 field_0xba5[0xBA8 - 0xBA5];
    /* 0x0BA8 */ s32 field_0xba8;
    /* 0x0BAC */ s32 field_0xbac;
    /* 0x0BB0 */ u8 field_0xbb0[0xBB4 - 0xBB0];
    /* 0x0BB4 */ s32 field_0xbb4;
    /* 0x0BB8 */ u8 field_0xbb8;
    /* 0x0BB9 */ u8 field_0xbb9;
    /* 0x0BBA */ u8 field_0xbba;
    /* 0x0BBB */ u8 field_0xbbb;
    /* 0x0BBC */ f32 field_0xbbc; // fn_80254744: 3.0f
    /* 0x0BC0 */ s32 field_0xbc0;
    /* 0x0BC4 */ u8 field_0xbc4[0xBD4 - 0xBC4];
    /* 0x0BD4 */ u8 field_0xbd4;
    /* 0x0BD5 */ u8 field_0xbd5[0xBD8 - 0xBD5];
    /* 0x0BD8 */ s32 field_0xbd8;
    // Cleared (stb 0) by fn_8028FBA0 when fn_80290070 succeeds.
    /* 0x0BDC */ u8 field_0xbdc;
    /* 0x0BDD */ u8 field_0xbdd[0xBE0 - 0xBDD];
    /* 0x0BE0 */ s32 field_0xbe0;
    /* 0x0BE4 */ s32 field_0xbe4;
    /* 0x0BE8 */ u8 field_0xbe8;
    /* 0x0BE9 */ u8 field_0xbe9[0xBEC - 0xBE9];
    // Allocated by the constructor (new, 0x2C0 bytes), deleted by the
    // destructor; returned by fn_8023FC24 (via fn_8022BCD4).
    /* 0x0BEC */ cls_804AA8D8* field_0xbec;
    // The player's cls_804A71A8 (allocated by the constructor); fn_802685D4
    // calls its fn_80268BC4.
    /* 0x0BF0 */ cls_804A71A8* field_0xbf0;
    /* 0x0BF4 */ s32 field_0xbf4;
    /* 0x0BF8 */ s32 field_0xbf8;
    /* 0x0BFC */ s32 field_0xbfc;
    /* 0x0C00 */ u8 field_0xc00;
    /* 0x0C01 */ u8 field_0xc01[0xC04 - 0xC01];
    /* 0x0C04 */ s32 field_0xc04;
    /* 0x0C08 */ s32 field_0xc08;
    /* 0x0C0C */ s32 field_0xc0c; // fn_80254660 sets it and field_0xc08 to 0x180
    /* 0x0C10 */ s32 field_0xc10;
    /* 0x0C14 */ s32 field_0xc14;
    /* 0x0C18 */ s32 field_0xc18;
    /* 0x0C1C */ s32 field_0xc1c[13]; // zeroed in a loop by fn_802544B0; indexed by tu_8026FC20
    /* 0x0C50 */ s32 field_0xc50;
    /* 0x0C54 */ s32 field_0xc54;
    /* 0x0C58 */ s32 field_0xc58;
    /* 0x0C5C */ s32 field_0xc5c;
    /* 0x0C60 */ s32 field_0xc60;
    // Word returned by fn_80270350 (tu_8026FC20).
    /* 0x0C64 */ s32 field_0xc64;
    /* 0x0C68 */ s32 field_0xc68;
    /* 0x0C6C */ s32 field_0xc6c;
    // Words returned by fn_80270310 and fn_802702E4 (tu_8026FC20).
    /* 0x0C70 */ s32 field_0xc70;
    /* 0x0C74 */ s32 field_0xc74;
    /* 0x0C78 */ s32 field_0xc78;
    /* 0x0C7C */ s32 field_0xc7c;
    // 0xC80, 0xCA0, 0xCE9, 0xCEE and 0x1215: the bytes fn_802413F0 tests
    // (game/PlayerFlags.h).
    /* 0x0C80 */ u8 field_0xc80;
    /* 0x0C81 */ u8 field_0xc81[0xC84 - 0xC81];
    // Set to (1, 1, 1) by fn_8026FC8C; fn_8023FBF0's code ramps z up to 1.0f.
    /* 0x0C84 */ JGeometry::TVec3f field_0xc84;
    /* 0x0C90 */ JGeometry::TVec3f field_0xc90;
    /* 0x0C9C */ s32 field_0xc9c;
    /* 0x0CA0 */ u8 field_0xca0;
    /* 0x0CA1 */ u8 field_0xca1;
    /* 0x0CA2 */ u8 field_0xca2;
    /* 0x0CA3 */ s8 field_0xca3; // fn_80254744 stores -0x80
    /* 0x0CA4 */ s8 field_0xca4;
    /* 0x0CA5 */ u8 field_0xca5[0xCA8 - 0xCA5];
    /* 0x0CA8 */ s32 field_0xca8;
    // Tested by every per-player query before it touches the player.
    /* 0x0CAC */ bool mActive;
    /* 0x0CAD */ bool field_0xcad;
    /* 0x0CAE */ u8 field_0xcae;
    /* 0x0CAF */ u8 field_0xcaf;
    // Read by fn_8023E32C (5 units); copied by fn_8028FBA0 and passed to fn_80290070.
    /* 0x0CB0 */ JGeometry::TVec3f field_0xcb0;
    /* 0x0CBC */ JGeometry::TVec3f field_0xcbc;
    // Address returned by fn_8023E2A0.
    /* 0x0CC8 */ JGeometry::TVec3f field_0xcc8;
    // Address returned by fn_8023E214.
    /* 0x0CD4 */ JGeometry::TVec3f field_0xcd4;
    // Returned by fn_8023E190.
    /* 0x0CE0 */ s32 field_0xce0;
    // Compared with 8 (lwz; cmpwi) by fn_80292454 before it calls fn_8024EFA0.
    /* 0x0CE4 */ s32 field_0xce4;
    // A bool: fn_8024BBB0 returns it as loaded (lbz; blr).
    /* 0x0CE8 */ bool field_0xce8;
    // A u8: fn_802413F0 converts it to its bool result (neg / or / srwi).
    /* 0x0CE9 */ u8 field_0xce9;
    /* 0x0CEA */ u8 field_0xcea;
    /* 0x0CEB */ u8 field_0xceb;
    /* 0x0CEC */ u8 field_0xcec;
    /* 0x0CED */ u8 field_0xced;
    /* 0x0CEE */ u8 field_0xcee;
    /* 0x0CEF */ u8 field_0xcef;
    /* 0x0CF0 */ u8 field_0xcf0;
    /* 0x0CF1 */ u8 field_0xcf1;
    // Tested against zero (lbz; cmplwi) by fn_8028FBA0 before it lowers 0xCB4.
    /* 0x0CF2 */ u8 field_0xcf2;
    /* 0x0CF3 */ u8 field_0xcf3;
    // Compared with 0x1D (lwz; cmpwi) by fn_802938B8 before it calls fn_8024F0E8.
    /* 0x0CF4 */ s32 field_0xcf4;
    /* 0x0CF8 */ s32 field_0xcf8;
    /* 0x0CFC */ u8 field_0xcfc;
    /* 0x0CFD */ u8 field_0xcfd;
    // Returned by fn_80240870 (via fn_80234134).
    /* 0x0CFE */ bool field_0xcfe;
    /* 0x0CFF */ u8 field_0xcff[0xD00 - 0xCFF];
    /* 0x0D00 */ s32 field_0xd00;
    /* 0x0D04 */ u8 field_0xd04;
    /* 0x0D05 */ u8 field_0xd05[0xD08 - 0xD05];
    // Returned by fn_8024322C (via fn_8023697C, which returns -1 without a player).
    /* 0x0D08 */ s32 field_0xd08;
    /* 0x0D0C */ s32 field_0xd0c;
    // Where the player comes back after state 0x33 (fn_80270948 stores it, fn_8027057C restores mPos).
    /* 0x0D10 */ JGeometry::TVec3f field_0xd10;
    /* 0x0D1C */ JGeometry::TVec3f field_0xd1c;
    /* 0x0D28 */ s32 field_0xd28;
    /* 0x0D2C */ u8 field_0xd2c;
    /* 0x0D2D */ u8 field_0xd2d;
    /* 0x0D2E */ u8 field_0xd2e[0xD30 - 0xD2E];
    /* 0x0D30 */ s32 field_0xd30;
    /* 0x0D34 */ bool field_0xd34;
    /* 0x0D35 */ u8 field_0xd35;
    /* 0x0D36 */ u8 field_0xd36[0xD38 - 0xD36];
    /* 0x0D38 */ s32 field_0xd38;
    /* 0x0D3C */ s32 field_0xd3c;
    /* 0x0D40 */ s32 field_0xd40;
    /* 0x0D44 */ s32 field_0xd44;
    /* 0x0D48 */ s32 field_0xd48;
    // A bool: fn_8024C458 returns it as loaded (lbz; blr).
    /* 0x0D4C */ bool field_0xd4c;
    /* 0x0D4D */ u8 field_0xd4d;
    /* 0x0D4E */ bool field_0xd4e;
    /* 0x0D4F */ u8 field_0xd4f[0xD50 - 0xD4F];
    /* 0x0D50 */ s32 field_0xd50;
    /* 0x0D54 */ s32 field_0xd54;
    /* 0x0D58 */ s32 field_0xd58;
    /* 0x0D5C */ u8 field_0xd5c;
    /* 0x0D5D */ u8 field_0xd5d[0xD60 - 0xD5D];
    // Another player's index, or negative for none: fn_80246F28 asks the
    // per-player queries about it (fn_8022CAF8, fn_80234004) and reads its
    // own field_0x120c / field_0x1224 instead when it is negative.
    /* 0x0D60 */ s32 field_0xd60;
    /* 0x0D64 */ u8 field_0xd64;
    /* 0x0D65 */ u8 field_0xd65[0xD68 - 0xD65];
    /* 0x0D68 */ s32 field_0xd68; // fn_8025EDD4 (-1 until set); fn_8025EE34 compares it
    /* 0x0D6C */ s32 field_0xd6c; // fn_8025F648 returns it when field_0xd64 == 1, else 'NULL'
    // Passed to fn_8022D75C (lwz r3) by fn_802685D4.
    /* 0x0D70 */ s32 field_0xd70;
    /* 0x0D74 */ s32 field_0xd74;
    /* 0x0D78 */ JGeometry::TVec3f field_0xd78; // fn_8025EDD4 stores its position
    /* 0x0D84 */ s32 field_0xd84; // fn_8026173C sets it to 20
    /* 0x0D88 */ s32 field_0xd88; // fn_8025EF68: the serial it was given, when accepted
    /* 0x0D8C */ s32 field_0xd8c;
    /* 0x0D90 */ u8 field_0xd90;
    /* 0x0D91 */ u8 field_0xd91[0xD94 - 0xD91];
    /* 0x0D94 */ s32 field_0xd94;
    /* 0x0D98 */ s32 field_0xd98;
    /* 0x0D9C */ s32 field_0xd9c;
    /* 0x0DA0 */ s32 field_0xda0;
    /* 0x0DA4 */ s32 field_0xda4;
    /* 0x0DA8 */ s32 field_0xda8;
    /* 0x0DAC */ s32 field_0xdac;
    /* 0x0DB0 */ s32 field_0xdb0[0x50];
    // fn_80254744 sets all 0x50 entries of 0xDB0, 0xEF0 and 0x1030 to -1 in one loop.
    /* 0x0EF0 */ s32 field_0xef0[0x50];
    /* 0x1030 */ s32 field_0x1030[0x50];
    /* 0x1170 */ s32 field_0x1170;
    /* 0x1174 */ u8 field_0x1174;
    /* 0x1175 */ u8 field_0x1175[0x1178 - 0x1175];
    /* 0x1178 */ s32 field_0x1178; // stw in tu_8023FBF0 (0x8024B1A0); 0-2 pick the frames of state 0x49 (fn_80265E88)
    /* 0x117C */ s32 field_0x117c[8]; // zeroed by fn_8027023C
    /* 0x119C */ u8 field_0x119c[4][4]; // zeroed by fn_8027023C
    /* 0x11AC */ s32 field_0x11ac;
    /* 0x11B0 */ s32 field_0x11b0;
    // Copied as the effect position by fn_8027F1C8 when fn_80241284 is true.
    /* 0x11B4 */ JGeometry::TVec3f field_0x11b4;
    /* 0x11C0 */ s16 field_0x11c0;
    /* 0x11C2 */ u8 field_0x11c2[0x11C4 - 0x11C2];
    /* 0x11C4 */ s32 field_0x11c4;
    /* 0x11C8 */ s32 field_0x11c8;
    /* 0x11CC */ s32 field_0x11cc;
    /* 0x11D0 */ s32 field_0x11d0;
    /* 0x11D4 */ s32 field_0x11d4;
    /* 0x11D8 */ u8 field_0x11d8;
    /* 0x11D9 */ u8 field_0x11d9[0x11DC - 0x11D9];
    /* 0x11DC */ s32 field_0x11dc;
    /* 0x11E0 */ u8 field_0x11e0;
    /* 0x11E1 */ u8 field_0x11e1[0x11E4 - 0x11E1];
    /* 0x11E4 */ s32 field_0x11e4;
    /* 0x11E8 */ u8 field_0x11e8;
    /* 0x11E9 */ u8 field_0x11e9;
    /* 0x11EA */ u8 field_0x11ea;
    /* 0x11EB */ u8 field_0x11eb;
    /* 0x11EC */ s32 field_0x11ec; // a player index for getPlayerInputSlot_8030AAF4
    /* 0x11F0 */ s32 field_0x11f0;
    /* 0x11F4 */ s32 field_0x11f4;
    /* 0x11F8 */ s32 field_0x11f8;
    /* 0x11FC */ s32 field_0x11fc;
    // A bool: fn_8023FF88 returns it as loaded (lbz; blr).
    /* 0x1200 */ bool field_0x1200;
    /* 0x1201 */ u8 field_0x1201;
    /* 0x1202 */ bool field_0x1202;
    /* 0x1203 */ u8 field_0x1203;
    /* 0x1204 */ u8 field_0x1204;
    /* 0x1205 */ u8 field_0x1205[0x1208 - 0x1205];
    /* 0x1208 */ s32 field_0x1208;
    /* 0x120C */ s32 field_0x120c;
    /* 0x1210 */ s32 field_0x1210;
    // A bool: fn_80240AF8 returns it as loaded (lbz; blr).
    /* 0x1214 */ bool field_0x1214;
    // A bool: fn_80240AF0 returns it as loaded (lbz; blr).
    /* 0x1215 */ bool field_0x1215;
    /* 0x1216 */ bool field_0x1216;
    // A bool: fn_802408C0 returns it as loaded (lbz; blr).
    /* 0x1217 */ bool field_0x1217;
    /* 0x1218 */ s32 field_0x1218;
    /* 0x121C */ u8 field_0x121c; // fn_8026173C clears it
    /* 0x121D */ u8 field_0x121d;
    /* 0x121E */ u8 field_0x121e;
    // A bool: fn_80249068 returns it as loaded (lbz; blr).
    /* 0x121F */ bool field_0x121f;
    /* 0x1220 */ s32 field_0x1220;
    /* 0x1224 */ s32 field_0x1224;
    // Word compared with 0 (lwz; cmpwi) by fn_802685D4.
    /* 0x1228 */ s32 field_0x1228;
    /* 0x122C */ s32 field_0x122c;
    /* 0x1230 */ s32 field_0x1230;
    /* 0x1234 */ s32 field_0x1234;
    /* 0x1238 */ s32 field_0x1238;
    /* 0x123C */ s32 field_0x123c;
    /* 0x1240 */ s32 field_0x1240;
    /* 0x1244 */ s32 field_0x1244;
    /* 0x1248 */ s32 field_0x1248;
    /* 0x124C */ bool field_0x124c; // fn_80248DD8 sets it (stb 1)
    // Set to 1 by fn_80293760 (stb r4); stored to by other Player functions too (stb r0 / stb r3).
    /* 0x124D */ u8 field_0x124d;
    /* 0x124E */ u8 field_0x124e[0x1250 - 0x124E];
    // The player's two sound objects (constructor: new cls_804B20F8(index,
    // pos, NULL, 8)); 0x1250 plays at field_0x1258, 0x1254 has no position.
    /* 0x1250 */ cls_804B20F8* field_0x1250;
    /* 0x1254 */ cls_804B20F8* field_0x1254;
    /* 0x1258 */ JGeometry::TVec3f field_0x1258; // zeroed by the constructor
    /* 0x1264 */ s32 field_0x1264;
    /* 0x1268 */ JGeometry::TVec3f field_0x1268; // fn_8024C378 / fn_8024E0A0: mPos plus a view offset, returned as TVec3f*
    /* 0x1274 */ JPABaseEmitter* field_0x1274;
    // Six particle callbacks, constructed and destroyed with the player.
    /* 0x1278 */ cls_804A64A8 field_0x1278;
    /* 0x128C */ JPABaseEmitter* field_0x128c;
    /* 0x1290 */ cls_804A64A8 field_0x1290;
    /* 0x12A4 */ u8 field_0x12a4[0x12B0 - 0x12A4];
    // 0x1274-0x1394: emitters, deleted in address order by fn_80241E64.
    /* 0x12B0 */ JPABaseEmitter* field_0x12b0;
    /* 0x12B4 */ JPABaseEmitter* field_0x12b4;
    /* 0x12B8 */ JPABaseEmitter* field_0x12b8;
    /* 0x12BC */ JPABaseEmitter* field_0x12bc; // deleted with deleteEmitter() (fn_8027B5F0)
    /* 0x12C0 */ s32 field_0x12c0;
    /* 0x12C4 */ JPABaseEmitter* field_0x12c4;
    /* 0x12C8 */ JPABaseEmitter* field_0x12c8;
    /* 0x12CC */ JPABaseEmitter* field_0x12cc;
    /* 0x12D0 */ cls_804A64A8 field_0x12d0;
    /* 0x12E4 */ JPABaseEmitter* field_0x12e4;
    /* 0x12E8 */ cls_804A64A8 field_0x12e8;
    /* 0x12FC */ u8 field_0x12fc[0x1308 - 0x12FC];
    /* 0x1308 */ JPABaseEmitter* field_0x1308; // deleted with deleteEmitter() (fn_802708E8)
    /* 0x130C */ cls_804A64A8 field_0x130c;
    /* 0x1320 */ u8 field_0x1320[0x132C - 0x1320];
    /* 0x132C */ JPABaseEmitter* field_0x132c;
    /* 0x1330 */ cls_804A64A8 field_0x1330;
    /* 0x1344 */ JGeometry::TVec3f field_0x1344;
    /* 0x1350 */ JPABaseEmitter* field_0x1350;
    /* 0x1354 */ JPABaseEmitter* field_0x1354;
    /* 0x1358 */ JPABaseEmitter* field_0x1358;
    /* 0x135C */ JPABaseEmitter* field_0x135c;
    /* 0x1360 */ JPABaseEmitter* field_0x1360;
    /* 0x1364 */ JPABaseEmitter* field_0x1364;
    /* 0x1368 */ JPABaseEmitter* field_0x1368;
    /* 0x136C */ JPABaseEmitter* field_0x136c;
    /* 0x1370 */ JPABaseEmitter* field_0x1370;
    /* 0x1374 */ JPABaseEmitter* field_0x1374;
    /* 0x1378 */ JPABaseEmitter* field_0x1378;
    /* 0x137C */ JPABaseEmitter* field_0x137c;
    /* 0x1380 */ JPABaseEmitter* field_0x1380;
    /* 0x1384 */ JPABaseEmitter* field_0x1384;
    // fn_8027F1C8's effect (ids 0x35E-0x361), moved with setEmitterPos_8013CB44.
    /* 0x1388 */ JPABaseEmitter* field_0x1388;
    /* 0x138C */ JPABaseEmitter* field_0x138c;
    /* 0x1390 */ JPABaseEmitter* field_0x1390;
    /* 0x1394 */ JPABaseEmitter* field_0x1394;
    // Two 'PEFF' spawner callbacks (game/cls_8049F140.h).
    /* 0x1398 */ cls_8049F140 field_0x1398;
    /* 0x13A8 */ cls_8049F140 field_0x13a8;
    /* 0x13B8 */ u8 field_0x13b8;
    /* 0x13B9 */ u8 field_0x13b9[0x13BC - 0x13B9];
    void fn_80241B44(JGeometry::TVec2f* out); // via fn_8022B8EC
    s32 fn_80246904(u32 arg); // via fn_8022C46C
    bool fn_8028FC2C(); // via fn_8022DC3C
    void fn_8024B298(s32 index, JGeometry::TVec3f* out); // via fn_8022E088
    void fn_8024B2BC(JGeometry::TVec3f* out); // via fn_8022E114
    bool fn_8025AA24(s32 arg); // via fn_8022E1A0
    bool fn_8024C5A0(s32 arg); // via fn_8022FC54
    bool fn_8025FDD0(const JGeometry::TVec3f* pos); // via fn_80230D80
    bool fn_8025FEDC(); // via fn_8023105C
    bool fn_8025FF70(); // via fn_802310F8
    bool fn_8026000C(); // via fn_80231194
    bool fn_802600A0(s32 arg); // via fn_802312CC
    bool fn_802426E0(s32 serial); // via fn_80231590
    void fn_80270318(s32 value); // via fn_802359A4; the body never reads r4, but the forwarder keeps value live (mgr in r5)
    void fn_8024E694(s32 value, s32 arg); // via fn_80236074
    void fn_8024ECCC(s32 value); // via fn_8023619C
    void fn_8024ED4C(s32 value); // via fn_802362C4
    bool fn_80247594(s32 arg); // via fn_80237488
    bool fn_80271038(const JGeometry::TVec2f* pos); // via fn_802378AC
    bool fn_80271068(const JGeometry::TVec2f* pos); // via fn_80237948
    bool fn_80256C74(const JGeometry::TBox2f* box, s32 layer); // tu_80256C74; box test on field_0x5dc, called by fn_8023A6CC
    // tu_8023FBF0 (0x80241618): field_0xc10 = item, field_0xc14 = level (kept at 2 or more
    // unless level >= 2), sets the player's bit in lbl_8053AED0 0x255; returns true. Result unused.
    bool fn_80241618(s32 item, s32 level);
    bool fn_80257DB4(); // tu_80256C74; false if any of 21 state / flag tests holds, else field_0xd3c < 0; called by tu_8022AD64
    bool fn_80256DCC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // via fn_8023A97C
    u8 fn_80256F44(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // via fn_8023AAF8
    bool fn_80273518(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // via fn_8023B848
    void fn_80252BD0(const JGeometry::TVec3f* offset); // via fn_8023DEDC
    s32 fn_8025F534(); // via fn_8023EC54
    s32 fn_8025F55C(); // via fn_8023ECF0
    s32 fn_80240628(); // via fn_8022F340
    bool fn_8025EDD4(s32 a, const JGeometry::TVec3f* pos); // via fn_8022F784
    // tu_8025EB18 (round 6): put-down check (callers: the carry state updates),
    // the request filter fn_80290070 asks before a screen change, and the
    // pick-up checks run by fn_80261480 (from the states 0-3 updates).
    bool fn_8025EB18();
    bool fn_8025FCC8();
    void fn_8026022C();
    bool fn_802604CC();
    bool fn_80260C08();
    void fn_80261480();
    bool fn_8024DA64(s32 a, bool b); // via fn_802302C4
    bool fn_8024D99C(const JGeometry::TVec3f* pos, bool flag); // via fn_802303FC
    bool fn_80272E98(const JGeometry::TBox2f* box, s32 layer); // via fn_80239C00
    bool fn_802724E0(const JGeometry::TBox2f* box, s32 layer); // via fn_80239C9C
    bool fn_802574CC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // via fn_8023AC74
    bool fn_802575B8(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // field_0xb0c[3]; stores the hit index in 0xb80 (-1 if none)
    bool fn_802576CC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // field_0xb00[3]; hit index in 0xb7c
    bool fn_802573E0(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out); // the single-slot box test over field_0xb38 / 0xb6c / 0x590
    bool fn_80273C30(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag); // via fn_8023BA68
    void fn_80252CF0(const JGeometry::TVec3f* pos); // via fn_8023DF68
    JGeometry::TVec3f* fn_8024C378(); // via fn_8023E3B8
    JGeometry::TVec3f* fn_8024145C(); // via fn_8023E7B0
    void fn_8025F698(bool arg); // via fn_80230C6C
    void fn_802544B0(); // via fn_8023F728
    void fn_80254744(); // via fn_8023F78C
    // tu_8023FBF0 members without a generated query (SHARED-EDIT: added with the small-function pass).
    void fn_8023FF90(const JGeometry::TVec3f* arg);
    s32 fn_8024064C() const;
    bool fn_80240B34();
    bool fn_80240D8C();
    bool fn_802411D8();
    bool fn_802412E0();
    bool fn_80241338();
    bool fn_80241514(s32 value);
    bool fn_802431B8();
    bool fn_80243194(s32 other);
    s32 fn_80247B9C();
    s32 fn_80247BD4();
    void fn_80247F0C();
    void fn_80249070();
    void fn_8024909C();
    void fn_8024BB88();
    bool fn_8024C448();
    bool fn_8024CA0C(s32 arg);
    bool fn_8024FE78();
    bool fn_8024FEBC();
    bool fn_8024FEFC();
    void fn_8024FF9C(s32 arg);
    void fn_8024FFB4(s32 arg);
    bool fn_8024FFC8(s32 arg);
    bool fn_80250030(s32 arg);
    void fn_80252BAC(const JGeometry::TVec3f* offset);
    bool fn_802517F8(bool a);
    bool fn_802518AC(bool a);
    bool fn_8025190C(bool a);
    void fn_80252F80(const JGeometry::TVec3f* pos, s32 a, s32 b, u8 c);
    bool fn_8024F63C();
    bool fn_80243234(); // tu_8023FBF0 (0x80243234)
    bool fn_80243C64();
    bool fn_80244630(); // tu_8023FBF0 (0x80244630)
    s32 fn_8027218C();
}; // Size: 0x13BC

// *gPlayerManager_8053AC90. Vtable 0x804A6F28: only the destructor (0x8023F7F8, which
// deletes the four players and clears gPlayerManager_8053AC90). Constructor 0x8023F8B0
// (sets gPlayerManager_8053AC90, creates the players, clears 0x68-0x73); tu_80139BE8
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

extern PlayerManager_804A6F28* gPlayerManager_8053AC90;

// Whether `player` is a valid index of an existing, active player: the body
// of fn_8023F264, which every per-player query inlines.
inline bool isPlayerActive(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (player < 0) {
        return false;
    }
    if (player >= 4) {
        return false;
    }
    if (gPlayerManager_8053AC90->mPlayers[player] == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_8023F984();
}

// Functions of game/tu_8022AD64.cpp other units call. Each query returns
// its fallback (after "else") when there is no manager or the player is not
// active. Types come from the getter each one forwards to and from the
// callers (only 0/1 tests -> bool; lfs at 0/4/8 of the result -> TVec3f*).
// Not declared yet: the ~300 others, whose getters take arguments or do more.
//
// Defined in game/tu_8022AD64.cpp (C++ linkage, renamed in symbols.txt).
void fn_8022AD64(void); // gGameRoot_8053AAF8->fn_80138B7C()
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
f32 fn_8022C6AC(s32 player);             // -> fn_80246F28; else lbl_80540008
s32 getPlayerLayer_8023DE58(s32 player);             // 134 units. -> field_0x3b4 (layer); else 0
// tu_8022AD64 (0x8023EAC4): whether a present player of `player`'s group (fn_8022E438) has
// fn_8025F484; false while the manager is missing. cls_804A71A8::fn_8026E1E0
// passes its owner's index and tests r3 with clrlwi 24.
bool fn_8023EAC4(s32 player);
JGeometry::TVec3f* getPlayerPos_8023E724(s32 player); // 193 units. -> &mPos; else &lbl_804CF460

// Generated by tools/gen_game.py (family fwd), by address. The return type
// comes from the callers (only clrlwi 24 / byte stores of the result -> bool;
// word uses -> s32; a caller that returns it passes on its callers' uses),
// each extra argument's from the getter's first use of it (followed into a
// callee that receives it in the same register: its mangled parameter type,
// else its own first use; float loads of exactly +0/+4/+8 through it ->
// const JGeometry::TVec3f*, stores -> JGeometry::TVec3f*).
s32 fn_80239A74(const JGeometry::TBox2f* a, s32 b, s32 c, bool d, bool e); // -> the first active player whose fn_80272224 is true; else -1
s32 fn_8023A6CC(const JGeometry::TBox2f* a, s32 b); // -> the first active player whose fn_80256C74 is true; else -1
s32 fn_8023B004(const JGeometry::TBox2f* a, s32 b); // -> the first active player whose fn_802732C4 is true; else -1

// Owner: tu_8022AD64 (0x8022E2D8, defined there). 1 unit (act_four_switch_monitor's
// update, always with 0xF); the return is unused.
void fn_8022E2D8(s32 index);

// Owner: tu_8022AD64 (0x80239B64, defined there). 1 unit (tu_8021693C). The
// per-player box test of fn_8022FF8C's family: if the player is present
// (byte 0xCAC set) it forwards (player object, box, layer, serial, a, b) to
// fn_80272224 and returns its 0/1 result (clrlwi 24 / test in the caller).
// tu_8021693C passes its hit box, the actor's field_0x4, a type word, 1, 1.
bool fn_80239B64(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 type, bool a, bool b);

// game/tu_8022AD64.cpp (0x8022BB48): whether any active player's
// fn_80241E28(layer, pos) holds (false while gPlayerManager_8053AC90 is NULL). Called by
// ActorBase_804A5770::startSound_801F0E34 with (field_0x4, &mPos).
bool fn_8022BB48(s32 layer, const JGeometry::TVec3f* pos);

// Generated by tools/gen_game.py (family fwd), by address. The return type
// comes from the callers (only clrlwi 24 / byte stores of the result -> bool;
// word uses -> s32; a caller that returns it passes on its callers' uses),
// each extra argument's from the getter's first use of it (followed into a
// callee that receives it in the same register: its mangled parameter type,
// else its own first use; float loads of exactly +0/+4/+8 through it ->
// const JGeometry::TVec3f*, stores -> JGeometry::TVec3f*).
bool fn_8022BC38(s32 player, s32 a, const JGeometry::TVec3f* b); // -> fn_80241E28; else false
cls_804AA8D8* fn_8022BCD4(s32 player); // 1 unit. -> fn_8023FC24 (0xBEC); else NULL
void fn_8022BD70(); // -> fn_80241E64 on every active player
s32 fn_8022BECC(s32 player); // -> fn_8023FFAC; else 0
bool fn_8022BF68(s32 player); // -> fn_802454C0; else false
bool fn_8022C004(s32 player); // -> fn_802456FC; else false
bool fn_8022C0A0(s32 player); // -> fn_80245938; else false
bool fn_8022C13C(s32 player); // -> fn_80245B74; else false
bool fn_8022C1D8(s32 player); // -> fn_80245DB0; else false
bool fn_8022C274(s32 player); // -> fn_80245FEC; else false
bool fn_8022C310(s32 player); // -> fn_80246228; else false
// tu_8022AD64 (0x8022B334, 0x490, unwritten): composite per-player query, tested
// with clrlwi by fn_8025785C before the item states.
bool fn_8022B334(s32 player);
bool fn_8022C3AC(s32 player); // -> fn_802466B4; else false (also for a player outside 0-3)
u32 fn_8022C52C(s32 player, u32 arg); // -> fn_80246B10; else 0 (also for a player outside 0-3)
u32 fn_8022C5EC(s32 player, u32 arg); // -> fn_80246D1C; else 0 (also for a player outside 0-3)
u8 fn_8022C8EC(s32 player); // -> fn_80247534; else false (also for a player outside 0-3)
bool fn_8022C9AC(s32 player); // -> fn_80247564; else false (also for a player outside 0-3)
void fn_8022CA6C(s32 player, s32 a, bool b); // 11 units. act_post_warp_pillar_handling passes (i, 0, 1)
s32 fn_8022CAF8(s32 player); // -> fn_802486A0; else 0
void fn_8022CB94(s32 player); // -> fn_8024B9E8
void fn_8022CC20(s32 player); // -> fn_8024BAEC
void fn_8022CD48(s32 player); // -> fn_80248FF0
// Owner: game/tu_8022AD64.cpp (0x8022CDD4 not written yet; 0x8022CF08 defined there).
// Called with a player index by UnknownFORM_804A7BE8 (game/tu_8029C584.cpp): the
// destructor calls fn_8022CDD4, init fn_8022CF08.
extern "C" void fn_8022CDD4(s32 player);
void fn_8022CF08(s32 player);
// Owner: game/tu_8022AD64.cpp (0x8022CFE8, not written yet): a float per player index (lbl_80540008
// without the manager); compared with 0.0f by fn_802859C4 (tu_8027E5BC).
extern "C" f32 fn_8022CFE8(s32 player);
// Owner: game/tu_8022AD64.cpp (0x8022D534, not written yet): a per-player test (false
// without the manager); ActorBase_804A5770::fn_801EE558 skips players for which it holds
// (clrlwi. of r3).
extern "C" bool fn_8022D534(s32 player);
s32 fn_8022D498(s32 player); // -> fn_8024BB94; else 0
s32 fn_8022D7F8(s32 player); // -> fn_8024FF6C; else 0
u8 fn_8022D894(s32 player); // -> fn_802517E4; else false
bool fn_8022D930(s32 player); // -> fn_80241478; else false
bool fn_8022D9CC(s32 player); // -> fn_80250090; else false
// -> fn_802422C4 (&field_0xbf0->field_0x14); else the address of a zeroed
// stack TVec2f (8 bytes, stfs 0 at 0x10 / 0x14).
const JGeometry::TVec2f* fn_8022DFD4(s32 player);
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
// Owner: tu_8022AD64's area (0x80231590, not written yet). 0x80231590 is
// unmangled, so it is C linkage. act_cannon's update calls it with
// (player, serial).
bool fn_80231590(s32 player, s32 serial);
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

// Owner unit tu_8022AD64 (0x80236074, 0x8C bytes). act_terrorpin's vfunc_0x68
// calls it once per bit of a hit mask with (player, 0x64, 0). Not known yet
// what the arguments mean.

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
bool fn_802341D0(s32 player);            // 0x17C bytes: loops over the players (true / false)
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
bool fn_8023B484(s32 player); // -> fn_802577E0; else false
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
// Owner unit: not written yet (0x80230C6C, 0x90 bytes). Called on a player for
// its result-free state change in fn_801E5754 (game/tu_801E4D54.cpp).
void fn_80230C6C(s32 player);

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

// 1 unit (tu_801CB270 fn_801CD1D0). Owner: tu_80301CD0 (0x80303750), a layer
// query (fn_8022F340) returning a byte: 0 without a GBA link, 1 while its transfer
// queue is busy, else a per-player flag.
u8 fn_80303750(s32 layer);
// Not written yet (tu_80301CD0, 0x80301D54): called with a player index (field_0x4) by
// fn_80290E90 and tu_8028FBA0's fn_802901DC path.
extern "C" void fn_80301D54(s32 player);

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

// Owner: tu_8022AD64 (0x80236890). No arguments; true without a manager, else whether
// fn_80270A04 holds for every present player whose bit is set in gGameRoot_8053AAF8->field_0x82. fn_80251BFC refuses a state
// change when it holds while gGameRoot_8053AAF8->field_0x7e is set.
bool fn_80236890();
// Owner: tu_8022AD64 (not written yet). The manager update's cross-player
// passes: fn_80232A0C every frame, fn_80231DE4 only with gGameRoot_8053AAF8->field_0x7f.
void fn_80232A0C();
void fn_80231DE4();
void fn_80230EB8(s32 player, s32 arg);
// Called by act_recovery_heart's update() with (player, 4, 0) once the heart reaches a
// player: fn_80270DDC(amount, flag) on it and on every player of its group (fn_8022E438).
void fn_80236554(s32 player, s32 amount, bool flag);
// Owner not written yet. Called by act_recovery_heart's update() when fn_8023B8E4 finds no
// player on the box; the result is unused.
s32 fn_8023B768(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
// Not written yet (0x8022E088, its own unit): the point of player `player`'s
// sample `index` (x, y, z) that act_cloud's fn_801053E8 tests against a box.
void fn_8022E088(s32 player, s32 index, JGeometry::TVec3f* out);
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

// Owner unit: not yet written (game/tu_8022AD64.cpp area, after getPlayerLayer_8023DE58).
// 1 unit (act_lift update) so far, called with the actor's index and a vector
// (mPos - mPrevPos); its result is unused there.
void fn_8023DF68(s32 player, const JGeometry::TVec3f* pos);

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
// Owner: tu_8022AD64 (0x8023434C, defined there). tu_8026FC20 fn_80274728 tests it
// (clrlwi.) for this player while fn_802486A0() == 3.
bool fn_8023434C(s32 player);
// Owner: tu_8022AD64 (0x80230E1C, defined there). 1 unit (act_four_switch_monitor's
// update: fn_80230E1C(player, 0) for each player whose bit is set in field_0x82).
// Owner: tu_8022AD64 (0x8022E114, defined there). 2 uses (tu_801D11FC). Forwards
// (player object, out) to the player's fn_8024B2BC when the player is present;
// *out (3 floats, read as a position) is written by it.
// Owner: tu_8022AD64 (0x8023E7B0, defined there). 1 unit (act_rock's update: the carried
// rock's position follows it). Like getPlayerPos_8023E724 (the player's mPos, else &lbl_804CF460) but
// through fn_8024145C; takes a player index (act_rock passes field_0xc8).
bool fn_802308E0(s32 player);
// Owner: tu_8022AD64 (0x8022EA0C, defined there). 1 unit (act_blindfold's update): maps a
// player index to the index whose position getPlayerPos_8023E724 is then asked for (r3 in, r3 out).
s32 fn_8022EA0C(s32 player);
// Owner: tu_8022AD64 (0x8022FF8C, defined there). 1 unit (act_merchant's update, state 1):
// fn_8022FEB8's box test for a given player (player, box, layer, serial, a, b), called with
// the actor's box moved to its position, its field_0x4 and mSerial, li 1 / li 0.
void fn_8022FF8C(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 serial, bool a, bool b);
bool fn_8023061C(s32 player, bool flag);
// Owner: tu_8022AD64 (0x8022F340, defined there). act_gambling_chest. Maps a
// player index to a window index (-1 without a manager; see Message.h).
// Owner: tu_8022AD64 (0x80231A50, defined there). tu_80441318. No arguments
// (r3 is not loaded); returns a player index, or a negative value for none
// (the caller maps a non-negative one through fn_8022F340).
s32 fn_80231A50();
// Owner: tu_8022AD64 (0x802362C4, defined there). act_gambling_chest (also
// act_cucco, act_force_gem_2). (player, value): tail-calls fn_8024ED4C(player
// object, value) when the player exists and its byte 0xCAC is set.
// game/tu_8022AD64.cpp (not written yet; plain C symbols). fn_802382D0:
// act_link_ending, two li 0 arguments (types not known yet).
extern "C" {
void fn_802382D0(s32 a, s32 b);
// Owner: tu_8022AD64 (0x80237948, defined there). 6 units (act_eyegore, act_blade_trap, ...).
// Tests the player object's byte 0xCAC for player index `player` (0-3) and returns
// fn_80271068(object) when it is set; the TVec2f pointer the callers pass is not read.
}
// 1 unit (act_attack_hitbox_when_player_is_thrown; others call it too). Player pair test.
bool fn_8022E438(s32 player, s32 other);
// Owner: tu_8022AD64 (0x8023077C, defined there). 2 units (tu_802F4AEC, act_zoom).
// No arguments; returns a bool (clrlwi 24 by the caller); false without a
// manager.
bool fn_8023077C();
JGeometry::TVec3f* fn_80238430(s32 player); // 1 unit. -> 0x324; else &lbl_804CF460
// Hits the player with `damage` at `pos` (fn_801F2B7C: fn_80238548(player, damage, &pos, arg)).
bool fn_80238548(s32 player, s32 damage, const JGeometry::TVec2f* pos, s32 arg); // reads pos+0/+4 only
// Called by act_fairy_invincible.cpp's update() on the tracked player in state 1; the owning unit is not written yet, result unused.
void fn_8023C830(s32 player);

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_invisible_npc_wall).
// Loops over the players and calls fn_80272DCC, which copies the 4 box floats
// and stores r5/r6 (field_0x4, serial) as words.

// Owner unit not written yet (C linkage, 0x802378AC, 1 unit: act_bumper).
// Called with a player index and a box centre after fn_80134DE4; the result
// is not used.

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_eyeball_switch). Index (0-3)
// of the first present player whose box test (fn_80273C30(player, box, layer,
// out, flag), a bool OR of the player's tile probes) passes, or -1; -1
// without a player manager. The caller ignores the result.

// Owner unit tu_8022AD64 (not written yet). 1 unit (act_burning_block). Counts
// (compared with 1 by the caller) the active players for which fn_80250030(player
// object, serial) passes.
s32 fn_80239108(s32 serial);

}
// Defined in game/tu_8022AD64.cpp, with C++ names (moved out of the extern "C" block above).
// 1 unit (act_dummy_conversation). Owner: tu_8022AD64. fn_8022FE80 forwards
// (layer, serial, a, b) to the player's fn_8024CAC8 (it compares serial with
// the player's word 0xD44); fn_8022FD8C returns the player index whose
// fn_8024CA0C accepts serial, or -1; fn_8023061C(player, 0) is called when a
// conversation test starts or is cancelled (result unused).
void fn_8022FE80(s32 player, s32 layer, s32 serial, bool a, bool b);
// 61 units; returns fn_8024DC00's bool (false without the player). (player, pos, id, a, b, c, flag): r6 an unsigned byte (clrlwi 24),
// r7/r8 li -0x80, r9 li 1 in act_post_warp_pillar_handling.
bool fn_80230498(s32 player, const JGeometry::TVec3f* pos, s32 id, u8 a, s8 b, s8 c, bool flag);
// 1 unit. -> Player_804A6F38::fn_80240868 (word 0x4); 0 without a manager. An inactive
// player falls off the end with r3 still the index (the source has no final
// return); the caller uses the result (mr r26, r3 at 0x80306B44).
s32 fn_802340A0(s32 player);
// -> fn_80290070; false without the player. 3 units (act_warp, act_magic_circle, tu_802957CC; clrlwi 24).
bool fn_802317F0(s32 player, s32 a, const JGeometry::TVec3f* b, const JGeometry::TVec3f* c, s32 d);
// Whether two boxes overlap (a copy of a clipped to b is not empty). Called by fn_80231DE4 and fn_80232A0C.
bool fn_80233C9C(const JGeometry::TBox2f* a, const JGeometry::TBox2f* b);
// 8, or 16 when the player's byte 0xB9B is set; 0 without the player. 1 unit (tu_801A3C50, subf.).
s32 fn_8023B520(s32 player);
// -> fn_80274238; 0 without the player. 5 units (act_force_gem*, tu_801EE24C).
u8 fn_8023B9CC(s32 player, const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* pos, bool flag);
// 1 unit (act_seesaw_lift). -> fn_8023FF90 with a stack copy of *pos.
void fn_8022BE24(s32 player, const JGeometry::TVec3f* pos);
// 2 units (tu_801CB270, tu_801A3C50; mr. / blt). The serial the player's field_0xb00 /
// field_0xb0c holds at index field_0xb7c / field_0xb80, or -1 (no player, index outside 0-2).
s32 fn_8023B2B0(s32 player);
s32 fn_8023B358(s32 player);
// 2 units (act_zora_lotus, act_seesaw_lift; they build the vector in the argument slot).
// -> fn_80242624(pos); false without the player.
bool fn_802314D8(s32 player, JGeometry::TVec3f pos);
// 2 units (tu_800F46C0, act_block). fn_8024FF9C(arg) on every present player.
void fn_80238D50(s32 arg);
// 1 unit (tu_800FF060). Sets the manager's flag 0x6C, then fn_8026FC8C on every present player.
void fn_8022B278();
// 1 unit (act_horse_race_man). Sets the manager's flag 0x71, then fn_8024D98C and fn_80248DD8 on every present player.
void fn_8022B064();
// The index of the first present player whose fn_8024FF6C / fn_8024FF84 equals serial, else -1.
// tu_80285E58 compares the result with field_0x4; act_block / act_somaria_block pass their 0x198.
s32 fn_80238F78(s32 serial);
s32 fn_80239040(s32 serial);
// 1 unit (act_force_gem; compared with 2). The first present player's non-negative fn_8026FC68, else -1.
s32 fn_8023A240();
// 3 units (act_thwomp_launch_npc, act_thwomp_launch_npc_2, tu_802957CC). Whether fn_8026FCE0 is true for any present player.
bool fn_8023A30C();
// 2 units (act_horse_race_man, tu_8026FC20). Clears the manager's flag 0x71, then fn_8024D98C,
// fn_8024BAEC, fn_80248E94(false) and fn_8026FC8C on every present player.
void fn_8022AF78();
// act_shop_item's update() calls it after paying for kinds 4-7, with 9, 4, 3 or 6 as the
// second argument by kind and 1 as the third (result unused): whether fn_80241618(item, level)
// accepts for every present player of `player`'s group (fn_8022E438); false without a manager.
bool fn_802372F8(s32 player, s32 item, s32 level);
// Owner: tu_8022AD64. The player index for actor `serial` (act_talking_sign stores it and passes it
// to getPlayerPos_8023E724): the first present player that fn_8037F1E4 does not hold and whose
// fn_8024CA0C accepts serial; -1 when none.
s32 fn_8022FD8C(s32 serial);
// Any-player version of fn_8023B0F4: loops players 0-3 that are present and returns the index
// of the first whose fn_802730D8 test (box, layer, flag, margin) hits, or -1 (also without a
// manager). act_zoom passes (&box, field_0x4, false, 0.0f); tested against 0 with cmpwi.
s32 fn_8023AF14(const JGeometry::TBox2f* box, s32 layer, bool flag, f32 margin);
// Owner: tu_8022AD64 (0x8022C76C / 0x8022C82C). Forwarders like fn_8022C6AC:
// the player's fn_8024712C / fn_80247330, lbl_80540008 without a player. tu_8027E5BC fn_80281A90
// subtracts the first from mPos.y and adds the second to mPos.x.
f32 fn_8022C76C(s32 player);
f32 fn_8022C82C(s32 player);
// 24 units / 37 calls (li 0). -> fn_8024D058((u8)flag); false without the player.
bool fn_80230224(s32 player, s32 flag);
// 5+ units (act_thwomp_switch, act_warp, ...). Same shape as fn_8023A51C below.
JGeometry::TVec3f* fn_8023A478(s32 player);
// 2 units / 7 calls (act_player_start_position, tu_802D39B8 x6). Calls
// fn_80252F80(mPlayers[player], pos, a, b, c), then stores pos in
// field_0x14[player], a in field_0x44, b in field_0x54, c (stb) in field_0x64.
void fn_8023F1D4(s32 player, const JGeometry::TVec3f* pos, s32 a, s32 b, u8 c);
void fn_80231368();                      // 4 units.
// 1 unit (tu_8021827C). Loop over the four players: fn_8022BA58 calls
// fn_80241DD0(player, layer, pos) and fn_8022B978 fn_80241BA0(player, layer,
// box, flag); each returns 1 as soon as one player passes, else 0 (callers
// test r3 with clrlwi.). Whether *pos / *box is visible for a player on `layer`.
bool fn_8022BA58(s32 layer, const JGeometry::TVec3f* pos);
bool fn_8022B978(s32 layer, const JGeometry::TBox2f* box, bool flag);
// Owner unit tu_8022AD64. 1 unit (act_cactus). Whether
// fn_802729F0(player object, serial) passes for any active player.
bool fn_80239638(s32 serial);
// Owner unit tu_8022AD64. 1 unit (act_burning_block). Calls
// fn_8024FFB4(player object, serial) on every active player for which
// fn_80250030 passes.
void fn_80238E08(s32 serial);
// Owner unit tu_8022AD64. 1 unit (act_pull_switch). Index (0-3)
// of the first present player whose box test (fn_802728AC(player, box, layer,
// serial)) passes, or -1; 0 without a player manager.
s32 fn_80239470(const JGeometry::TBox2f* box, s32 layer, s32 serial);
// Owner unit tu_8022AD64. 1 unit (tu_80206B30 vfunc_0x58).
// Four sibling probes over the players: each calls a per-player box test
// (fn_802576CC / fn_80256F44 / fn_802574CC / fn_802575B8: player object,
// box, layer, out) and the test stores two floats through `out` (callers
// read it as a TVec2f). fn_8023AA18 and fn_8023AC74 return the index of the
// first player whose test passes, fn_8023AE34 and fn_8023AD54 the last one;
// all return -1 when none does (also without a player manager).
s32 fn_8023AE34(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
s32 fn_8023AD54(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
// Owner: tu_8022AD64 (0x80236468). (player, amount): fn_80270D34(amount) on every present
// player of `player`'s group (fn_8022E438). Called by tu_8026FC20 fn_802710C8.
void fn_80236468(s32 player, s32 amount);
// 34+ units. Takes a flag (clrlwi 24), not a player: loops all four players;
// every caller passes 0. Result tested against 1.
bool fn_80230534(bool flag);
// Owner unit tu_8022AD64. 1 unit (act_burning_block). Index
// (0-3, read from the player object's 0x3DC) of the first active player for which
// fn_80250030(player object, serial) passes, or -1 (fn_80239108 not positive).
s32 fn_80238BC4(s32 serial);

void fn_8022B8EC(s32 player, JGeometry::TVec2f* out);
s32 fn_8022C46C(s32 player, u32 arg);
bool fn_8022DC3C(s32 player);
bool fn_8022E1A0(s32 player, s32 arg);
bool fn_8022FC54(s32 player, s32 arg);
bool fn_80230D80(s32 player, const JGeometry::TVec3f* pos);
bool fn_8023105C(s32 player);
bool fn_802310F8(s32 player);
bool fn_80231194(s32 player);
bool fn_802312CC(s32 player, s32 arg);
void fn_802359A4(s32 player, s32 value);
void fn_8023619C(s32 player, s32 value);
bool fn_80237488(s32 player, s32 arg);
bool fn_8023A97C(s32 player, const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
u8 fn_8023AAF8(s32 player, const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
bool fn_8023B848(s32 player, const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
void fn_8023DEDC(s32 player, const JGeometry::TVec3f* offset);
s32 fn_8023EC54(s32 player);
s32 fn_8023ECF0(s32 player);
bool fn_80237948(s32 player, const JGeometry::TVec2f* pos);
void fn_8022E114(s32 player, JGeometry::TVec3f* out);
s32 fn_8022F340(s32 player);
bool fn_80230E1C(s32 player, bool flag);
void fn_80236074(s32 player, s32 value, s32 arg);
void fn_802362C4(s32 player, s32 value);
bool fn_802378AC(s32 player, const JGeometry::TVec2f* pos);
bool fn_8022F784(s32 player, s32 a, const JGeometry::TVec3f* pos);
bool fn_802302C4(s32 player, s32 a, bool b);
bool fn_802303FC(s32 player, const JGeometry::TVec3f* pos, bool flag);
bool fn_80239878(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 serial);
bool fn_80239C00(s32 player, const JGeometry::TBox2f* box, s32 layer);
bool fn_80239C9C(s32 player, const JGeometry::TBox2f* box, s32 layer);
s32 fn_8023A89C(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
void fn_80239914(const JGeometry::TBox2f* box, s32 layer, s32 serial);
s32 fn_8023AA18(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
s32 fn_8023AC74(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out);
s32 fn_8023BA68(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag);
s32 fn_80230B70(s32 player);
s32 fn_80230CFC(s32 player);
bool fn_80233D20(s32 player);
s32 fn_8023B22C(s32 player);
bool fn_8023B400(s32 player);
s32 fn_8023E10C(s32 player);
s32 fn_8023E190(s32 player);
JGeometry::TVec3f* fn_8023E214(s32 player);
JGeometry::TVec3f* fn_8023E2A0(s32 player);
JGeometry::TVec3f* fn_8023E32C(s32 player);
JGeometry::TVec3f* fn_8023E578(s32 player);
JGeometry::TVec3f* fn_8023E604(s32 player);
JGeometry::TVec3f* fn_8023E3B8(s32 player);
u8 fn_8023E45C(s32 player, s32 index);
JGeometry::TVec3f* fn_8023E4E4(s32 player, s32 index);
JGeometry::TVec3f* fn_8023E7B0(s32 player);
void fn_8023A654(s32 player);
s32 fn_8023B5B4(s32 player);
void fn_8023E690(s32 player, const JGeometry::TVec3f* pos);
void fn_80230BF4(s32 player, s32 mode);  // 18 units. act_post_warp_pillar_handling passes 2

// Owner: tu_8022AD64's area (not written yet). The per-player queries the
// small getters of game/tu_8023FBF0.cpp forward to when a team index is set.
bool fn_80230E1C(s32 player, bool arg); // -> fn_8024EFA0
s32 fn_8023BEA8(s32 player);            // -> fn_80247B9C
s32 fn_8023C36C(s32 player);            // -> fn_80247BD4

#endif
