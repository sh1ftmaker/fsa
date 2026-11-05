#ifndef GAME_GAMEGLOBALS_H
#define GAME_GAMEGLOBALS_H

// Globals most game units touch. Names are dtk's placeholders (they are the
// link names, so keep them until a unit that defines them names them);
// types are placeholders too. Field types come from the loads (lwz -> a
// 32-bit value or pointer, lbz -> u8); pointers whose target is unknown
// are void*.
//
// Class names: provisional descriptive names carry an address (docs/PLAN.md,
// "Placeholder names"): the vtable when known, else the constructor, else
// the first function seen reaching the object (said at each). Vectors:
// JGeometry::TVec3f for game data, Vec only where a mangled name or a
// PSMTX/PSVEC call proves it (Camera_804AA9E4::fn_8030C5F0's mangled name
// says Vec).
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function that shows
// it.

#include "types.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/TColor.h"
#include "game/Player.h"
// The view (fn_802D6A9C) and the effect system (fn_8013C824) moved to
// their own headers; included here so existing users keep compiling.
#include "game/Effect.h"
#include "game/GameView.h"
// The map, its collision queries (fn_80226850, ...) and the camera.
#include "game/Camera.h"
#include "game/Map.h"

class ActorManager_804A5800;
// The object lbl_8053AD68 holds (game/tu_80379F84.cpp); defined in tu_80379F84.h.
struct cls_804AE9A0;
class JAISeMgr;
class JPAEmitterManager;
// JSystem/J2D/J2DGrafContext.h (mkdd's J2D, which FSA's J2DGraph units use).
class J2DOrthoGraph;

// The object at offset 0x4 of *lbl_8053AAF8 (type unknown; named after
// fn_801F856C, the first function seen reading it).
class cls_804B1D10;
class BgRewrite_804A7C98;
class cls_804AE9B0;
class cls_804B1920;
class cls_804B17A0;
class PlayerManager_804A6F28;

struct GameRootSub04_801F856C {
    // vtable at 0x0 (0x8049F080); its one slot is the destructor below.
    virtual ~GameRootSub04_801F856C();

    /* 0x4 */ PlayerManager_804A6F28* field_0x4; // constructor fn_8013A538 (tu_80139BE8)
    /* 0x8 */ ActorManager_804A5800* mActorManager; // fn_801F856C; allocated in
                                           // game/tu_80139BE8.cpp
    /* 0xC */ GameView_804A9950* mView;             // fn_802D6A9C
    // fn_802A3D80 is called on it by fn_8013A1A4 (game/tu_80139BE8.cpp).
    /* 0x10 */ BgRewrite_804A7C98* field_0x10;
    // fn_8043EDF8 is called with it by fn_8013A1A4 (game/tu_80139BE8.cpp); the
    // object fn_8043F504 builds (cls_804B1920).
    /* 0x14 */ cls_804B1920* field_0x14;
    // fn_80439AB0 is called with it by fn_8013A1A4 (game/tu_80139BE8.cpp); the
    // object fn_80439C78 builds (cls_804B17A0).
    /* 0x18 */ cls_804B17A0* field_0x18;
    // fn_8037AFC0 is called on it by fn_8013A1A4 (game/tu_80139BE8.cpp).
    /* 0x1C */ cls_804AE9B0* field_0x1c;
    // Passed to fn_801902D0 by fn_80139CC4 (game/tu_80139BE8.cpp); its type
    // is not known yet.
    /* 0x20 */ void* field_0x20;
    // fn_8016A3AC returns it (game/tu_8016A2B4.cpp).
    /* 0x24 */ struct GameRootSub24_8016A3AC* field_0x24;
    /* 0x28 */ u8 field_0x28[0x2C - 0x28];
    // The staff-roll page loader (game/act/tu_8044B80C.h); fn_80139C84 draws it.
    /* 0x2C */ cls_804B1D10* field_0x2c;
    // A heap: game/tu_8044B80C.cpp makes it current, frees its J2DScreens
    // and calls freeAll on it (becomeCurrentHeap / freeAll calls on r3).
    /* 0x30 */ JKRHeap* field_0x30;
    // Cleared (stw 0) by fn_8013A1A4 (game/tu_80139BE8.cpp); type unknown.
    /* 0x34 */ s32 field_0x34;

    // 0x8013A1A4 (game/tu_80139BE8.cpp, not written yet in this file's copy).
    // Calls fn_8016EDB0 on field_0x24, fn_801F8E4C on mActorManager and
    // vfunc_0xC on mView; fn_8023F088 when lbl_8053AAF8->field_0x7e is 1; then
    // fn_8023F78C and fn_8023F728, sets lbl_8053ACD0, runs the calls on
    // field_0x10, field_0x14, field_0x18, field_0x1c and field_0x20, fn_8038D870,
    // and clears field_0x34 and lbl_80529DEC's 0x240 and 0x1F4 words.
    void fn_8013A1A4();

    // 0x80139BE8 (game/tu_80139BE8.cpp, not written yet). Calls fn_801F857C on
    // mActorManager, clears lbl_8053ACD0 and the four bytes at 0x235C of
    // lbl_8053AB10->0x20, and runs fn_801184F0 on fn_8011921C()'s object.
    void fn_80139BE8();

    // 0x80139C84 (game/tu_80139BE8.cpp, not written yet). Calls
    // fn_8043DC98 on lbl_8053AED0, then draws field_0x2c when it is set.
    void fn_80139C84();

    // 0x80139CC4 (game/tu_80139BE8.cpp, not written yet). Calls fn_8013B438 on
    // lbl_8053AB10 twice around the vfunc_0x14 of mView, fn_8023F2BC, fn_801F8658
    // on mActorManager and fn_801902D0 on field_0x20; then fn_8016DA6C on
    // field_0x24 unless lbl_8053AB10->field_0x39 is set.
    void fn_80139CC4();

    // 0x80139D3C (game/tu_80139BE8.cpp, not written yet). Calls fn_8016DB60 on
    // field_0x24 unless lbl_8053AB10->field_0x39 is set.
    void fn_80139D3C();
};

// *lbl_8053AAF8: the most-used game singleton (309 of 529 units read it).
// Set in game/tu_801344C0.cpp (0x80134998) and game/tu_80134C04.cpp
// (0x80139340). Named after its vtable, 0x8049EF20 (constructor
// fn_80139300, allocated with __nw__(0x11C)); its real layout is unknown.
struct GameRoot_8049EF20 {
    /* 0x00 */ u8 field_0x0[0x4];
    /* 0x04 */ GameRootSub04_801F856C* field_0x4; // -> 0x8: the actor manager
    /* 0x08 */ u8 field_0x8[0x4C - 0x8];
    /* 0x4C */ s32 field_0x4c; // act_sandstorm compares it with 11
    /* 0x50 */ u8 field_0x50[0x54 - 0x50];
    // Stored by fn_8019CDC4 (game/tu_80197070.cpp, 0x8019CDC4, stw of r3).
    /* 0x54 */ s32 field_0x54;
    // The fade state tested by cls_804A8390::fn_802AC8B4 (tu_802AC8AC): 1 and
    // 5 step the black picture up, 4 steps it down, 6 counts field_0x14 down.
    /* 0x58 */ s32 field_0x58;
    /* 0x5C */ u8 field_0x5c[0x74 - 0x5C];
    // Both cleared by fn_8019CDC4 (game/tu_80197070.cpp, stw of 0).
    /* 0x74 */ s32 field_0x74;
    /* 0x78 */ s32 field_0x78;
    /* 0x7C */ u8 field_0x7c[0x7E - 0x7C];
    /* 0x7E */ u8 field_0x7e;
    /* 0x7F */ u8 field_0x7f;
    /* 0x80 */ u8 field_0x80[0x2];
    /* 0x82 */ u8 field_0x82; // bit i set: player i takes part (fn_801F6068)
    /* 0x83 */ u8 field_0x83;
    /* 0x84 */ u8 field_0x84;
    // Set to 1 by fn_8019CDC4 (game/tu_80197070.cpp, stb of 1).
    /* 0x85 */ u8 field_0x85;
    // Toggled between 0 and 1 by fn_80134DC0 (game/tu_80134C04.cpp, 0x80134DC0,
    // stb of r3 at 0x86 only).
    /* 0x86 */ u8 field_0x86;
    /* 0x87 */ u8 field_0x87;
    // Stored by fn_80134D14 (game/tu_80134C04.cpp, 0x80134D14, stb of r4).
    /* 0x88 */ u8 field_0x88;
    /* 0x89 */ u8 field_0x89[0x94 - 0x89];
    // Stored by fn_80134FC4 (game/tu_80134C04.cpp, 0x80134FC4, stb of r4), which
    // sets field_0x95 to 1 unless the value is 1.
    /* 0x94 */ u8 field_0x94;
    // Set to 1 by fn_80134FC4 (game/tu_80134C04.cpp) when field_0x94 is not 1.
    /* 0x95 */ u8 field_0x95;
    // Stored by fn_80134FBC (game/tu_80134C04.cpp, 0x80134FBC, stb of r4).
    /* 0x96 */ u8 field_0x96;
    /* 0x97 */ u8 field_0x97[0xA4 - 0x97];
    // Copied from lbl_80529DEC+0x14 by the constructor (0x80139360). Game
    // code calls it through vtable slot 0x18 = getResource(u32 type,
    // const char* name) with a 4CC and a name ('ENM ' in fn_801F62AC, 'TIMG'
    // in the constructor at 0x801395D8), and through slot 0x30 =
    // getResSize(const void*) (24 sites, e.g. in tu_801F6058). JKRFileLoader declares
    // both slots too, so the code is the same either way; the game's loaders
    // build JKRAramArchive/JKRMemArchive (fn_80401AC8, fn_80401D20).
    /* 0xA4 */ JKRArchive* field_0xa4;
    /* 0xA8 */ u8 field_0xa8[0xB4 - 0xA8];
    // Constructed with __ct__Q25JMath13TRandom_fast_FUl at this+0xB4 by the
    // constructor fn_80139300 (0x8013932C-0x80139338). 132 units inline
    // TRandom_fast_::get()/get_ufloat_1() on it (lwz 0xB4, mullw 0x19660D,
    // + 0x3C6EF35F, stw, srwi 9, oris 0x3F80, - 1.0f), e.g. 0x803D6578.
    /* 0xB4 */ JMath::TRandom_fast_ mRandom;
    /* 0xB8 */ u8 field_0xb8[0xF8 - 0xB8];
    // Cleared to 0 by fn_801365C8 (game/tu_80134C04.cpp, 0x801365C8), together
    // with field_0xfc, when it is stored a 1 (stb of r4 at 0xFE first).
    /* 0xF8 */ s32 field_0xf8;
    /* 0xFC */ u8 field_0xfc;
    /* 0xFD */ u8 field_0xfd[0xFE - 0xFD];
    // Stored by fn_801365C8 (game/tu_80134C04.cpp, 0x801365C8, stb of r4).
    /* 0xFE */ u8 field_0xfe;
    // SHARED-EDIT (tu_803BC6F0): the file select screen sets it to 1 before
    // it clears the save rows (fn_803D0264, fn_803CDEE8) or rebuilds them.
    /* 0xFF */ u8 field_0xff;
    /* 0x100 */ u8 field_0x100[0x108 - 0x100];
    // Stored by fn_80134CC0 (game/tu_80134C04.cpp, 0x80134CC0), which
    // tu_803A60E4 calls on lbl_8053AAF8 with 0, 1 and 0x1E; what the value means
    // is not known yet.
    /* 0x108 */ s32 field_0x108;
    /* 0x10C */ u8 field_0x10c[0x11C - 0x10C]; // main allocates 0x11C bytes
    // Owner: game/tu_80134C04.cpp (0x80134CC0; defined there). Stores r4 at
    // this+0x108 (stw).
    void fn_80134CC0(s32 value);

    // 15 units / 17 calls. Stores at this+0x50, 0x58 and 0x84 (per review);
    // act_ending_map_skipper calls it with (15, 0).
    void fn_80136798(s32 a, s32 b);
    // ~28 units. In tu_80134C04 with GameRoot's constructor; uses this->0xA0.
    // r3 only.
    void fn_80138B7C();
    // Flips this+0x86 between 0 and 1 (lbz, then stb of 1 or 0). Defined in
    // game/tu_80134C04.cpp (0x80134DC0); no callers found yet.
    void fn_80134DC0();
    // Stores r4 at lbl_8053AAF8+0x88 (stb). Defined in game/tu_80134C04.cpp;
    // the body reads the global itself, r3 (this) is lbl_8053AAF8 at every call.
    void fn_80134D14(u8 value);
    // Stores r4 at this+0x96 (stb). Defined in game/tu_80134C04.cpp; called
    // by act_stop_npc (0x802ED1B4, 0).
    void fn_80134FBC(u8 value);
    // Stores r4 at this+0x94 (stb), then sets this+0x95 to 1 unless the value
    // is 1. Defined in game/tu_80134C04.cpp (0x80134FC4); no callers found yet.
    void fn_80134FC4(u8 value);
    // Owner: game/tu_80134C04.cpp (0x80134FE0, not written yet). Stores r4 at
    // this+0x94 (stb), reads it back, and calls fn_80458658 on lbl_8053AF80 when
    // it is 1, else sets this+0x95 to 1. Callers: game/tu_8029C584.cpp
    // (cls_804A7BE8's destructor, with 0), game/tu_8023FBF0.cpp and game/tu_802D39B8.cpp.
    void fn_80134FE0(u8 value);
    // Stores r4 at this+0xFE (stb). When it is 1, clears this+0xFC (stb) and
    // this+0xF8 (stw). Defined in game/tu_80134C04.cpp (0x801365C8); no callers found yet.
    void fn_801365C8(u8 value);
    // Owner: game/tu_80134C04.cpp (next to fn_80134FBC; not written yet).
    // 1 unit. act_wait_switch calls it with (10, count) when its switch is
    // seen; r3 is lbl_8053AAF8 (the only object this unit's calls pass).
    void fn_80134DE4(s32 a, s32 b);
    // game/tu_80134C04.cpp, called by main (tu_801344C0) only. The
    // constructor allocated with __nw__(0x11C); no arguments.
    GameRoot_8049EF20();
    // game/tu_80134C04.cpp (0x80138C9C), called once by main right after the
    // constructor. No further arguments.
    void fn_80138C9C();
    // game/tu_80134C04.cpp (0x80136828 and 0x80137300): main calls them each
    // frame once the loader is done, between beginRender / endRender and
    // endRender / endFrame; the first only while lbl_80529DEC's byte 0x251 is 0.
    void fn_80136828();
    void fn_80137300();
    // Owner: game/tu_80134C04.cpp (0x80138A30; not written yet). 107 units.
    // The number of players that take part: the set bits of field_0x82 (bits
    // 0-3). Its body reads lbl_8053AAF8 itself; callers load it into r3.
    s32 fn_80138A30();
    // game/tu_8022AD64.cpp (0x8023F98C): whether bit `player` of field_0x82 is set.
    bool fn_8023F98C(s32 player);
    // Owner: not written yet (0x8013670C, 0x8C bytes). Called by
    // fn_8019CD9C (game/tu_80197070.cpp) with its s32 argument; r3 is
    // lbl_8053AAF8 (the only object that unit's call passes).
    void fn_8013670C(s32 value);
}; // Size: 0x11C

extern GameRoot_8049EF20* lbl_8053AAF8;

// Owner: game/tu_801E667C.cpp (.sdata). Read by tu_80441318 and act_horse_race_man.
extern s32 lbl_8053A160;
// .sdata word in act_navi's data (0x8053A188-0x8053A198): fn_8023F78C stores -1 to it.
extern s32 lbl_8053A190;

// The object fn_801902C0 returns (lbl_8053AAF8->field_0x4->field_0x20; named
// after that getter). Owner: game/tu_801902C0.cpp (not written yet).
// act_special_bg_draw_object: three 0x20-byte requests at 0x2E0, 0x300 and
// 0x320; an update sets a position and a value and raises the flag, once the
// pointer at 0xC is non-NULL and the flag is clear.
struct GameRootSub20_801902C0 {
    // Two smaller requests at 0x2A8 and 0x2C4 (act_environment_control_2's
    // update: position copied in, flag raised once the pointer at 0xC is
    // non-NULL and the flag is clear).
    struct SmallRequest {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ void* field_0xc; // tested against NULL
        /* 0x10 */ u8 field_0x10[8]; // not yet known
        /* 0x18 */ u8 mActive;
        /* 0x19 */ u8 field_0x19[3]; // not yet known
    }; // Size: 0x1C
    struct Request {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ void* field_0xc; // tested against NULL
        /* 0x10 */ u8 field_0x10[4]; // not yet known
        /* 0x14 */ f32 mValue;
        /* 0x18 */ u8 field_0x18[4]; // not yet known
        /* 0x1C */ u8 mActive;
        /* 0x1D */ u8 field_0x1d[3]; // not yet known
    }; // Size: 0x20
    // Up to two positions queued by act_environment_control_2's update
    // (count at 0x618, stored while it is below 2).
    struct QueuedPos {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ u8 field_0xc[8]; // not yet known
    }; // Size: 0x14
    /* 0x000 */ u8 field_0x0[0x2A8]; // not yet known
    /* 0x2A8 */ SmallRequest mSmallRequests[2];
    /* 0x2E0 */ Request mRequests[3];
    // Up to ten entries, filled by act_quicksand's vfunc_0x9C while
    // mQuicksandCount (0x4A8) is below 10: the quicksand's position, then
    // three of its floats (field_0x54.x, 0x398 and 0x3A8).
    struct QuicksandEntry {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ f32 field_0xc;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ u8 field_0x18[0x24 - 0x18]; // not yet known
    }; // Size: 0x24
    /* 0x340 */ QuicksandEntry mQuicksandEntries[10];
    /* 0x4A8 */ s32 mQuicksandCount;
    /* 0x4AC */ u8 field_0x4ac[0x5F0 - 0x4AC]; // not yet known
    /* 0x5F0 */ QueuedPos mQueuedPos[2];
    /* 0x618 */ s32 mQueuedPosCount;
    /* 0x61C */ u8 field_0x61c[0x7A8 - 0x61C]; // not yet known
    // SHARED-EDIT (act_opening_goal update): set to 1 through fn_801902C0()'s result.
    /* 0x7A8 */ u8 field_0x7a8;
    /* 0x7A9 */ u8 field_0x7a9[0x7D0 - 0x7A9]; // not yet known

    // SHARED-EDIT (act_ice_melted_by_four_players): the request the ICEB
    // update (act_ice_melted_by_four_players.cpp) fills at 0x7D0 when its pointer at 0x10 is NULL and
    // mActive is clear: its position, a percentage (100 - field_0x238 * 100)
    // and mActive = 1. Owner: game/tu_801902C0.cpp (not written yet).
    struct IceRequest {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ s32 mPercent;
        /* 0x10 */ void* field_0x10; // tested against NULL
        /* 0x14 */ u8 field_0x14[8]; // not yet known
        /* 0x1C */ u8 mActive;
    }; // Size: 0x20
    /* 0x7D0 */ IceRequest mIceRequest;
};
extern "C" {
// Owner: game/tu_801902C0.cpp (not written yet). No arguments; loads
// lbl_8053AAF8->field_0x4->field_0x20 (0x801902C0-0x801902C8).
GameRootSub20_801902C0* fn_801902C0(void);
}

// The object fn_80119DD0 returns: a function-local static (lbl_804BC504,
// registered with __register_global_object) built on first use. Named after
// the function fn_80119DD0 registers with __register_global_object (as the
// static's destructor), fn_80119E5C. Owner: game/tu_80118384.cpp (not
// written yet). 20+ units call fn_80119998 on it, e.g. game/tu_8012EA00.cpp
// passes (0x3C, 0, 1, 0, 1, 0x5A, 0).
class cls_80119E5C {
public:
    /* 0x00 */ u8 field_0x0; // tested by tu_801286C0's update (states 1 and 3)
    /* 0x01 */ u8 field_0x1; // set to 1 by tu_801286C0's update (state 3)
    /* 0x04 */ s32 field_0x4; // compared with 0xB3 by tu_801286C0's update
    void fn_80119998(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
};
extern "C" cls_80119E5C* fn_80119DD0();
extern "C" {
// Owner: game/tu_80134C04.cpp (defined there; not written yet). No arguments; reads
// lbl_8053AAF8->field_0x7e and tests all four players. Returns 0 or 1 (clrlwi 24).
bool fn_80138AB0();
}

class EnemyBase_804A4808;
extern "C" {
// Owner: game/tu_80212E3C.cpp (defined there; not written yet). Reads an
// EnemyBase's mFlags (0x230) and mState (0x248); takes the actor serial's
// result (fn_801F666C) in r3. Called by act_quicksand fn_80210D90 in state 1.
void fn_80212ECC(EnemyBase_804A4808* actor);
}

class ItemPedestal_804A82A8;
extern "C" {
// Owner: game/act_item_pedestal.cpp (not written yet; its generated header
// may not be edited, so not a member yet). Sets the word at 0x238 of an ITMS
// actor to 0; called by act_stop_npc (0x802ED198).
void fn_802AC09C(ItemPedestal_804A82A8* pedestal);
}

// The 0x90-byte object (named after its constructor) fn_8045A080 (game/tu_80457DFC.cpp) initialises and
// stores in lbl_8053AF80; owner unit not written yet, members unknown but
// the u32 at 0x58 (act_enemy_count_wipe_out_check copies it in init and
// compares it later).
struct Unk_8045A080 {
    /* 0x00 */ u8 field_0x0[0x34];
    /* 0x34 */ JAISeMgr* field_0x34; // fn_8045B154 (game/tu_8045B010.cpp) calls stopSoundID on it
    /* 0x38 */ u8 field_0x38[0x58 - 0x38];
    /* 0x58 */ u32 field_0x58;
    /* 0x5C */ u8 field_0x5c[0x90 - 0x5C];
    // game/tu_80457DFC.cpp: stores `pause` at 0x8a and passes it to
    // JAISeCategoryMgr::pause(bool) of three categories of the object at
    // 0x34 (0x8045868C-0x804586A8); called by act_stop_npc.
    void fn_80458658(bool pause);
    // game/tu_80457DFC.cpp. Sets up JAudio from the two loaded audio files
    // (the buffers at 0x14 of the loaders in tu_80442810): `heap` is the
    // heap lbl_80529DEC.field_0x4, 0x800000 the audio memory size stored in
    // the JAU_JASInitializer (fn_80459AE4 stores r5 at its 0x4).
    void fn_80459AE4(JKRSolidHeap* heap, u32 audioMemSize, void* initData, void* seqData);
    // game/tu_80457DFC.cpp: JASDriver::setOutputMode(mode).
    void fn_804594E4(u32 mode);
    // game/tu_80457DFC.cpp (0x80458FF0). act_enemy_generator_3 calls it with
    // (0x1000021, 1, 0) when field_0x58 differs from 0x1000021; the body
    // compares r4 against 0x1000000+n, tests r5 with clrlwi. 24 (0x8045908C) and
    // stores r6 (a byte) at 0x8d.
    void fn_80458FF0(s32 id, bool arg1, bool flag);
    // game/tu_80457DFC.cpp (0x8045A080): the constructor main calls on the
    // 0x90-byte object it allocates in the main heap.
    Unk_8045A080();
    // game/tu_80457DFC.cpp (0x80458880). fn_8045B030 (game/tu_8045B010.cpp)
    // calls it with an id from lbl_8052DEE0 in r4, or -1; the id is not a
    // JAISoundID (passed in a register).
    void fn_80458880(s32 id, s32 arg);
    // game/tu_80457DFC.cpp (0x80458F04). Callers pass a table entry in r4: if
    // the sound at 0x84 is not already id, creates it through the category
    // object at 0x30 (vtable slot 0xC) and starts a volume move.
    void fn_80458F04(u32 id);
    // game/tu_80457DFC.cpp (0x80458F9C). 30+ units pass 0x3C (the argument is
    // not read): stops the sound at 0x80 and clears the fields at
    // 0x58-0x68.
    void fn_80458F9C(s32 frames);
    // game/tu_80457DFC.cpp (0x80459274, same unit). Called on lbl_8053AF80
    // by cls_804B19F0::fn_80441A1C (game/tu_80441A1C.cpp) after the 0x0 call.
    void fn_80459274();
    // game/tu_80457DFC.cpp (0x80458090): loads wave archives 2, 3 and 4 of
    // the JAUSection at 0x44 (loadWaveArc). Called by tu_8039003C's
    // constructor when fn_804580DC returned 0.
    void fn_80458090();
    // game/tu_80457DFC.cpp (0x804580DC). Looks up entry `index` (a byte) of
    // the JAUSection at 0x44 and returns a byte (1 when its state is 2);
    // tu_8039003C's constructor passes 3.
    u8 fn_804580DC(u8 index);
    // game/tu_80457DFC.cpp (0x80458408). Acts only for id 0x100001A on the
    // sound at 0x80; r5 is tested with clrlwi. 24 (0x80458488). Called by
    // tu_8039003C's constructor with (0x100001A, true).
    void fn_80458408(s32 id, bool flag);
    // game/tu_80457DFC.cpp (0x8045815C, not written yet in this file's copy). Called by
    // fn_80290E90 with false when the player's 0xCE4 is below 8, with true when it is 8.
    void fn_8045815C(bool flag);
}; // Size: 0x90
extern Unk_8045A080* lbl_8053AF80;

// game/tu_80401848.cpp (not written yet): the objects fn_804019A8 and
// fn_80401E10 allocate (0x20 and 0x1C bytes) for a file found by path, and
// queue on the DVD thread. Both store the vtable 0x804B0FD0 and then their
// own over it: 0x804B0FA0 and 0x804B0FC0. They are named after those
// vtables. The word at 0x10 is the entrynum; the byte at 0x4 becomes 1 when
// loading has finished; the buffer is the word at 0x14.
// Base class (vtable 0x804B0FD0, stored first by both allocators).
class cls_804B0FD0 {
public:
    /* 0x00 */ u32 field_0x0;
    /* 0x04 */ u8 field_0x4;
    /* 0x08 */ virtual ~cls_804B0FD0();
    /* 0x0C */ u8 field_0xc;
    /* 0x10 */ s32 field_0x10;
    /* 0x14 */ void* field_0x14; // the loaded data
}; // Size: 0x18

class cls_804B0FA0 : public cls_804B0FD0 {
public:
    /* 0x08 */ virtual ~cls_804B0FA0();
    /* 0x18 */ u8 field_0x18[0x20 - 0x18];
}; // Size: 0x20

class cls_804B0FC0 : public cls_804B0FD0 {
public:
    /* 0x08 */ virtual ~cls_804B0FC0();
    /* 0x18 */ u32 field_0x18;
}; // Size: 0x1C

extern "C" {
// Owner: game/tu_80401848.cpp. (path, mode byte, heap)
cls_804B0FA0* fn_804019A8(const char* path, u8 mode, JKRHeap* heap);
cls_804B0FC0* fn_80401E10(const char* path, u8 mode, JKRHeap* heap);
}

extern "C" {
// game/tu_801CB270.cpp (not written yet; both clrlwi.): map tests of a
// point on `layer`.
bool fn_801CCBD0(const JGeometry::TVec3f* pos, s32 layer);
bool fn_801CCB1C(const JGeometry::TVec3f* pos, s32 layer);
}

// Random helpers on lbl_8053AAF8->mRandom, defined in game units.
f32 fn_80154744();          // [0, 1); game/act_sword_soldier.cpp
s32 fn_801CD664(s32 n);     // [0, n); game/tu_801CB270.cpp
s32 fn_801CD580(s32 except); // [0, 4) except `except`; game/tu_801CB270.cpp

// game/tu_801CB270.cpp: steps *angle towards target by at most step (a
// wrapper of fn_801CD970; the 0/1 it passes on is unused by all 9 callers).
void fn_801CD930(s16* angle, s32 target, s32 step);

// act_shadow_link_2.cpp (returns 0, no arguments). Callers (tu_801C5B08,
// tu_801C71BC) test the result as a byte (clrlwi. 24).
bool fn_801C5340();

// game/tu_801CB270.cpp functions its source doesn't define yet.
extern "C" {
s32 fn_801CD398(s32 a, s32 b); // [0, 4) avoiding a (and b)
// game/tu_801CB270.cpp (not written yet): the stepper fn_801CD930 wraps; true
// when *angle reached target.
bool fn_801CD970(s16* angle, s32 target, s32 step);
}

// game/tu_804023D0.cpp (not written yet); called from game/tu_80376FA0.cpp.
// Returns lbl_80529DEC->field_0x10->getResource(type, name) (vtable slot
// 0x18); the callers pass the 4CC 'TIMG' and a .bti name.
extern "C" {
void* fn_804032E8(u32 type, const char* name);
}

// game/act_block.cpp (not written yet); called from game/tu_80376FA0.cpp.
// Returns 1 or 0 (li 1 / li 0); its body reads *pos and writes it back with
// psq on the 1 path. The layer goes to fn_802D7544 on GameView.
extern "C" {
u8 fn_8028D4DC(JGeometry::TVec3f* pos, s32 index, s32 layer);
}

// game/act_strange_block.cpp (not written yet); called from game/tu_80376FA0.cpp
// on one axis. Returns center + radius, or center - radius on the side pos is
// not on (the side test is on pos against center).
extern "C" {
f32 fn_8033D4AC(f32 pos, f32 center, f32 radius);
}

// Loads the TIMG resource `name` from lbl_8053AAF8->field_0xa4 into texObj
// (and tlutObj, if given, for a palette texture); returns height / width.
// game/tu_801CB270.cpp; called from game/tu_801E9AFC.cpp.
f32 fn_801CB270(const char* name, GXTexObj* texObj, GXTlutObj* tlutObj);

// game/tu_801CB270.cpp; called from game/tu_801BEDBC.cpp. Whether the tile at pos
// is one of the codes 0x40-0x49.
bool fn_801CCC78(const JGeometry::TVec3f* pos, s32 layer);

// game/tu_801CB270.cpp (not written yet); called from game/tu_801D2C60.cpp.
// Two effects (fn_8013CC50 0x117 and 0x118) at pos on `layer`; arg3 is passed
// on and arg4 is passed on plus 1 (as the depth).
extern "C" {
void fn_801CB538(s32 layer, const JGeometry::TVec3f* pos, s32 arg3, s32 arg4);
}

// game/tu_801CB270.cpp; called from game/tu_80207548.cpp and
// game/tu_80220F54.cpp. Forwards to ActorManager_804A5800::fn_801F8544 with
// 'RUPY' and the layer and pos; the flag selects 4 or 5 in the low bits of
// its last argument. Returns the result (an actor serial).
s32 fn_801CB434(s32 layer, const JGeometry::TVec3f* pos, bool flag);

// game/tu_801CB270.cpp (not written yet); called from game/tu_801BD8C0.cpp.
// Maps a 16-bit angle to the one mirrored about the 0x4000 axes (a piecewise
// 0x4000 - a, a - 0x4000 negated, ... on the four quadrants). The body
// clrlwi 16s the argument for its unsigned compares and ends every path in
// extsh; the caller (0x801BD908) extends the result again, so the return
// type is an int.
extern "C" {
s32 fn_801CB7CC(s16 angle);
}

// game/tu_801CB270.cpp; called from game/tu_801DE714.cpp. Whether fn_80230188
// holds for every player that takes part (fn_80138A30); takes no arguments.
bool fn_801CCF7C();

// The objects at lbl_8053AB10->0x2C and 0x30, named after their vtables
// (stored at 0 by their constructors); no members declared yet. 0x20
// (cls_804AA858) is in game/Map.h, 0x24 (Camera_804AA9E4) in game/Camera.h.
// 0x804A0160: game/tu_80158624.cpp, constructor fn_801594E8, 0x10 bytes.
// Read by fn_80158B3C / fn_80158B5C and directly (+0x4, +0xC) in 28 units.
class JUTTexture;
class cls_804A0160 {
public:
    // Constructor (0x801594E8, game/tu_80158624.cpp): loads "circle.bti" from
    // lbl_8053AAF8->field_0xa4 into a new JUTTexture on `heap`. Its vtable
    // (lbl_804A0160) has one virtual destructor, at offset 0 (vptr).
    cls_804A0160(JKRHeap* heap);
    virtual ~cls_804A0160();

    /* 0x4 */ JUtility::TColor field_0x4; // colour: fn_80158B3C sets r,g,b = 0xFF, a = 0
    /* 0x8 */ JUTTexture* field_0x8;
    /* 0xC */ u8 field_0xc;      // act_post_warp_pillar_handling init stores 0xFF
    /* 0xD */ u8 field_0xd[3];   // not yet known

    // 0x80158B3C (owner unit game/tu_80158624.cpp, not written yet). Resets
    // the colour bytes: 0x4-0x6 = 0xFF, 0x7 = 0, 0xC = 0xFF. `this` is
    // lbl_8053AB10->0x2C; called by tu_8039003C's constructor.
    void fn_80158B3C();

    // 0x80158B5C (owner unit game/tu_80158624.cpp, not written yet). Draws a
    // shadow-like sprite on `layer` (< 8) at *pos with a colour and scale;
    // `this` is lbl_8053AB10->0x2C. Called by tu_802201D8's draw (colour
    // 0,0,0,0x80; scale 24.0f). The colour is a TColor passed by value (a
    // stack copy, its address in r6).
    void fn_80158B5C(s32 layer, const JGeometry::TVec3f* pos, JUtility::TColor color, f32 scale);

    // 0x8015945C (owner unit game/tu_80158624.cpp, not written yet). Resets the
    // colour and flag bytes 0x4-0x7, 0xC and 0xD-0xE of this object.
    void fn_8015945C();

    // 0x80159330 (owner unit game/tu_80158624.cpp, not written yet). Returns at
    // once when lbl_8053AAF8->0x4C is 0xC; otherwise sets 0xE to 0xFF and, unless
    // the GameView check (fn_802D6A9C()->fn_802DCD0C()) passes, clears 0xD[0]
    // when lbl_8053AAF8's 0x94 and 0x96 bytes are 0.
    void fn_80159330();

    // 0x80158F70 (owner unit game/tu_80158624.cpp, not written yet). Draws up
    // to three quads the size of the render mode (JUTVideo::sManager); `this`
    // is lbl_8053AB10->0x2C. Skipped when the colour is 0xFFFFFF00 and 0xC is 0xFF.
    void fn_80158F70();
}; // Size: 0x10
// 0x80543F28 (.sbss2, 8 bytes; no owner split yet). Its word is the colour
// that cls_804A0160::fn_80158F70 (game/tu_80158624.cpp) puts in TEV register 1
// with its own alpha byte.
extern JUtility::TColor lbl_80543F28;
// 0x804A03A8: game/tu_8015F454.cpp, constructor fn_801602E8, 0x298 bytes.
// Its +0x284 is read in 56 places; fn_8015F8FC takes it.
// Only the flag and the float that tu_801286C0's update sets are known.
class cls_804A03A8 {
public:
    /* 0x000 */ u8 field_0x0[0x284];
    /* 0x284 */ u8 field_0x284; // 0 until set to 1 (tu_801286C0 update)
    /* 0x285 */ u8 field_0x285[0x28C - 0x285];
    /* 0x28C */ f32 field_0x28c; // set to 0.05f with the flag
};

struct J3DDrawBuffer; // JSystem/J3D/J3DDrawBuffer.h

// *lbl_8053AB10: the second most-used singleton (267 units). Set in
// game/tu_8013AAD4.cpp (0x8013C024) and game/tu_80134C04.cpp (0x801393C4).
// Named after its vtable, 0x8049F108 (constructor fn_8013BFEC, which
// allocates every member below and stores it at the offset given).
struct GameRoot2_8049F108 {
    /* 0x00 */ u8 field_0x0[0x8];
    // Two J3DDrawBuffers (initialize, allocBuffer(0x80) in the constructor,
    // 0x8013C0xx); fn_8013B618 calls frameInit on both.
    /* 0x08 */ J3DDrawBuffer* field_0x8;
    /* 0x0C */ J3DDrawBuffer* field_0xc;
    // new J2DOrthoGraph(...) (__ct__13J2DOrthoGraphFffffff, stored at
    // 0x8013C11C); 36 units pass it to J2DScreen::draw as the
    // J2DGrafContext (67 sites).
    /* 0x10 */ J2DOrthoGraph* field_0x10;
    /* 0x14 */ u8 field_0x14[0x20 - 0x14];
    /* 0x20 */ cls_804AA858* field_0x20;
    /* 0x24 */ Camera_804AA9E4* field_0x24;
    /* 0x28 */ EffectSystem_8049F178* field_0x28;
    /* 0x2C */ cls_804A0160* field_0x2c;
    /* 0x30 */ cls_804A03A8* field_0x30;
    /* 0x34 */ u8 field_0x34[0x39 - 0x34];
    // Read as a byte by fn_80139CC4 and fn_80139D3C (game/tu_80139BE8.cpp);
    // not yet known what sets it.
    /* 0x39 */ u8 field_0x39;

    // game/tu_8013AAD4.cpp (owner unit, not written yet). Calls
    // field_0x24->fn_8030DFE8(0); `this` is lbl_8053AB10. Called twice by
    // fn_80139CC4 (game/tu_80139BE8.cpp).
    void fn_8013B438();
    // 0x8013B460 (between fn_8013B438 and fn_8013B490; body not yet known).
    // Called with `this` = lbl_8053AB10 by cls_804A0160::fn_80158F70
    // (game/tu_80158624.cpp), before its GX setup.
    void fn_8013B460();

    // game/tu_8013AAD4.cpp (owner unit, not written yet; same unit as the
    // constructor). Calls vtable slot 5 of field_0x10 (the J2DOrthoGraph);
    // `this` is lbl_8053AB10. Called by tu_803DA6B4's fn_803DA6B4 before
    // J2DScreen::draw.
    void fn_8013B490();
    // game/tu_8013AAD4.cpp. Called by GameRoot_8049EF20::fn_80137300
    // (tu_80134C04); calls field_0x20->fn_802FF0EC() in the modes
    // isGameModeSet() (game/GameMode.h) accepts.
    void fn_8013B538();
    // game/tu_8013AAD4.cpp. Called 12 times by tu_80134C04: the same mode
    // test as fn_8013B538, then field_0x28->fn_8013D5D4() and frameInit on
    // field_0x8 / field_0xc.
    void fn_8013B618();
};

extern GameRoot2_8049F108* lbl_8053AB10;

// The input object *lbl_8053AD30 (game/tu_803099FC.cpp, owner not written
// yet): named after its vtable, 0x804AA8C8 (constructor fn_8030AC4C, which
// stores it in lbl_8053AD30). Other members unknown.
class cls_804AA8C8 {
public:
    // Owner: game/tu_803099FC.cpp. Return the word at 4 + 4*slot (resp.
    // 0x24 + 4*slot) of the object ANDed with mask, or 0 for slot -0x3E7.
    // act_dummy_conversation's update tests the first for held buttons and
    // the second for fresh presses.
    u32 fn_80309D24(s32 slot, u32 mask);
    u32 fn_80309D6C(s32 slot, u32 mask);
    // Owner: game/tu_803099FC.cpp. A float for input slot `slot` (the
    // value at lbl_80541670 for slot -0x3E7, else from the bits of the word
    // at 4 + 4*slot); Player_804A6F38::fn_80246F28 returns it.
    f32 fn_80309D90(s32 slot);
    // Owner: game/tu_803099FC.cpp (0x80309C4C, same unit). Called by
    // cls_804B19F0::fn_80441A1C with mask 0x1300 and slot -1; returns a
    // pointer-sized word that is tested for zero.
    u32 fn_80309C4C(u32 mask, s32 slot);
    // Owner: game/tu_803099FC.cpp (0x8030A258 / 0x8030A2E4 / 0x8030A364 /
    // 0x8030A388, same unit). Each takes an input slot from fn_8030AAF4 and
    // returns a bool (byte) that cls_804B19F0::fn_80441A1C tests.
    u8 fn_8030A258(s32 slot);
    u8 fn_8030A2E4(s32 slot);
    u8 fn_8030A364(s32 slot);
    u8 fn_8030A388(s32 slot);
    // SHARED-EDIT (tu_803F0A54 fn_803FC304): owner game/tu_803099FC.cpp
    // (0x8030A308, same unit). Takes an input slot from fn_8030AAF4 and
    // returns a byte the caller tests after clrlwi 24 (the controller in
    // that slot is connected).
    u8 fn_8030A308(s32 slot);
    // SHARED-EDIT (tu_803F0A54 fn_803F8050): owner game/tu_803099FC.cpp
    // (0x80309A9C, same unit). Called like fn_80309C4C (r4 a button mask 4 /
    // 8, r5 an input slot); the result is tested with cmplwi 0.
    u32 fn_80309A9C(u32 mask, s32 slot);
};
extern cls_804AA8C8* lbl_8053AD30;
extern "C" {
// Owner: game/tu_803099FC.cpp. fn_8030AAF4 maps a player index (0-3) to an
// input slot (-0x3E7 for none); masks used: 0x100/0x200/0x400/0x20 and 1/2.
s32 fn_8030AAF4(s32 player);
} // extern "C"

// The object *lbl_8053AED0 (game/tu_8043A0E8.cpp, owner not written yet):
// named after its vtable, 0x804B1920 (constructor fn_8043F504, which stores
// it in lbl_8053AED0). Only the bytes at 0x1F0 are known (act_dummy_conversation
// stores 2 at 0x1F0 + player); its size and the rest are unknown.
class cls_804B1920 {
public:
    // Virtual destructor first: the vtable pointer goes where it is declared.
    virtual ~cls_804B1920(); // owner game/tu_8043A0E8.cpp (not written yet)
    /* 0x004 */ u8 pad_0x4[0xDC - 0x4];  // unknown fields
    // SHARED-EDIT (act_battle_royale_end_check fn_803D6AE8): a flag byte that
    // the battle royale end check ORs with 0x10 when it picks its winner.
    /* 0xDC */ u8 field_0xdc;
    /* 0xDD */ u8 pad_0xdd[0x1F0 - 0xDD];
    /* 0x1F0 */ u8 field_0x1f0[4]; // per player
    /* 0x1F4 */ u8 field_0x1f4[0x256 - 0x1F4]; // not yet known
    // SHARED-EDIT (act_battle_royale_end_check draw, 0x256 = the byte its
    // mFocus is stored into when mFocus is not 0xFF).
    /* 0x256 */ u8 field_0x256;

    // 0x8043D67C (game/tu_8043A0E8.cpp): 27 units call it with *lbl_8053AED0
    // in r3, an id in r4 (tu_8020FE50's actor passes 0x1A8 or 0x1C1) and two
    // more arguments (0 in most calls).
    void fn_8043D67C(s32 id, u32 arg1, u32 arg2);

    // 0x8043D5F0 (game/tu_8043A0E8.cpp, not written yet): takes no arguments
    // besides the object; fn_80139C5C calls it, then fn_8038D4A8.
    void fn_8043D5F0();

    // 0x8043D874 (game/tu_8043A0E8.cpp, not written yet): called with the id in
    // r4, a value in r5 (a table entry or 300 in act_shooting_gallery) and 0 in r6.
    void fn_8043D874(s32 id, s32 arg1, u32 arg2);

    // 0x8043CB24 (game/tu_8043A0E8.cpp): queues the decimal number `value`
    // for drawing with `digits` digits (r7), centred around (x, y) at `scale`.
    // Returns false when all ten slots (the byte at 0x252) are taken. r5 is
    // stored as a byte at 0x244 + slot, r6 (at most 5) at 0x23A + slot, and r8
    // (`flag`) switches the leading-zero handling. act_digging_game and
    // tu_8010089C call it too; tu_801ED3DC passes (n, 255, 0, 1, false).
    bool fn_8043CB24(s32 value, u8 arg2, u8 arg3, u8 digits, bool flag, f32 x, f32 y, f32 scale);

    // 0x8043E7CC (game/tu_8043A0E8.cpp): act_gba_appear's update calls it with a
    // position passed by value.
    void fn_8043E7CC(JGeometry::TVec3f pos);

    // 0x8043D514 (game/tu_8043A0E8.cpp, not written yet): returns a float in
    // f1 with only `this` (*lbl_8053AED0) in r3. game/tu_8044998C.cpp passes
    // it as y to cls_804AF86C::fn_80402E08 on both of its panes.
    f32 fn_8043D514();

    // 0x8043D57C (game/tu_8043A0E8.cpp, not written yet): like fn_8043D514,
    // returns a float in f1 with only `this` (*lbl_8053AED0) in r3.
    // game/tu_804455E0.cpp's fn_804455E0 passes it as y for the slots 2 and 3.
    f32 fn_8043D57C();

    // 0x8043D644 and 0x8043D8B4 (game/tu_8043A0E8.cpp, not written yet): no
    // arguments besides the object; fn_80139D70 calls them in turn.
    void fn_8043D644();
    void fn_8043D8B4();
    // 0x8043DC98 (game/tu_8043A0E8.cpp, not written yet): no arguments besides
    // the object; GameRootSub04_801F856C::fn_80139C84 calls it first.
    void fn_8043DC98();
};
extern cls_804B1920* lbl_8053AED0;

// The 0x60-byte object fn_80439C78 builds (game/tu_80438C68.cpp, not written
// yet), stored at GameRootSub04_801F856C::field_0x18. Named after its vtable,
// 0x804B17A0; it keeps a J2DScreen at 0x4 and its layout is not known beyond
// the destructor.
class cls_804B17A0 {
public:
    /* 0x000 */ // vtable, whose one slot is the destructor below
    virtual ~cls_804B17A0(); // owner game/tu_80438C68.cpp (not written yet)
};

// The 4-byte pointer at .sbss lbl_8053AB00: the heap GameRootSub04_801F856C's
// constructor (fn_8013A538, game/tu_80139BE8.cpp) stores, which the destructor
// asks for getTotalFreeSize between its steps.
extern JKRHeap* lbl_8053AB00;

// game/act_giant_bomb.cpp (0x80341CB0, not written yet); called from
// game/tu_8020FE50.cpp. Spawns a 'BBMB' actor (fn_801F9484) on `layer` at
// *pos.
extern "C" {
void fn_80341CB0(s32 layer, const JGeometry::TVec3f* pos);
}

// game/tu_8037DF68.cpp. Sets up a message box: msg is compared unsigned with
// the id cached at 0x3C of *lbl_8053AD78, param is stored as a word at 0x7C,
// param2 as a byte at 0xE5, the floats are a position (callers pass mPos.x,
// mPos.y, mPos.z, mPos.z), and the colour (passed by value) is copied byte by
// byte into the text box.
extern "C" void fn_8037F2FC(u32 msg, s32 param, u8 param2, f32 x, f32 y, f32 z, f32 f4,
                            JUtility::TColor color);

// game/tu_80301CD0.cpp (per-player lists, not written yet). ~100 calls in ~90
// units, mostly from actors' draw: serial, layer, position, then a sprite id.
// The callee tests d as a bool and reads g and the two stack arguments as
// bytes (clrlwi / lbz 0x4b, 0x4f); returns a 0/1 flag. Argument meanings
// unknown. Review: fsa-wt/_reviews/game__act_force_gem_shadow.review.md.
extern "C" bool fn_803075AC(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id,
                            bool d, s32 e, s32 f, u8 g, u8 h, u8 i);

// game/tu_80301CD0.cpp (not written yet). 18 units; same leading arguments as
// fn_803075AC: serial, layer, position, then a sprite id. Adds 0x200 to the
// id only when e == 2; ids below 0x45 then go to fn_80307830, the others get
// 0x400 added and go to fn_803075AC. d and e are converted to 0/1 flags for
// the callee. The result is unused.
extern "C" void fn_80307AC8(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, s32 d,
                            s32 e, s32 f);

// game/tu_80301CD0.cpp. Same leading arguments as fn_803075AC (serial, layer,
// position, sprite id, then d as a bool, e, f and g as a byte masked with 3);
// returns 0 below layer 8. game/tu_801D3CBC.cpp calls it with (0, 1, 0, player).
extern "C" bool fn_80307830(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, bool d,
                            s32 e, s32 f, u8 g);

class ActorBase_804A5770;

// The 0x20-byte object in .bss at 0x804BC540 (function-local static of
// fn_8011921C, built on first use); named after its destructor fn_801192AC. Only the fields
// fn_801186B0 writes are known.
struct cls_801192AC {
    /* 0x00 */ u8 field_0x0;
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u8 field_0x2;
    /* 0x04 */ s32 field_0x4; // compared as signed in tu_801286C0's vfunc_0x54
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ u8 field_0xc[0x4]; // not yet known
    // The actor list: fn_8011888C appends and fn_80118798 removes entries,
    // fn_8011871C searches [mBegin, mEnd) for an actor by mActorId.
    /* 0x10 */ ActorBase_804A5770** mBegin;
    /* 0x14 */ ActorBase_804A5770** mEnd;
    /* 0x18 */ u8 field_0x18[0x8]; // not yet known
}; // Size: 0x20

// game/tu_80118384.cpp (not written yet). Returns the static cls_801192AC.
extern "C" cls_801192AC* fn_8011921C();
// Not yet written (0x801184F0, game/tu_80118384.cpp range). Called on fn_8011921C()
// with 0 or 1 by act_opening_goal's vfunc_0x54, tu_801286C0 and tu_80139BE8.
extern "C" void fn_801184F0(cls_801192AC* obj, bool flag);
// game/tu_80118384.cpp (not written yet). Returns the first entry at or after
// `from` whose actor's mActorId (0x1A0) is `actorId`, or obj->mEnd if none.
extern "C" ActorBase_804A5770** fn_8011871C(cls_801192AC* obj, ActorBase_804A5770** from,
                                           u32 actorId);
// game/tu_80118384.cpp. Sets field_0x0 = 1, field_0x1 = field_0x2 = 0,
// field_0x4 = value, field_0x8 = 0 unless fn_801F68E0 (after fn_801F856C)
// returns 0; returns whether it did. Called with (fn_8011921C(), 2) from
// act_four_sword_demo.
extern "C" bool fn_801186B0(cls_801192AC* obj, u32 value);
// game/tu_80118384.cpp (not written yet). Called with fn_8011921C() by
// tu_801286C0's update (cases 3 and 5/7 of its state switch).
extern "C" void fn_801183BC(cls_801192AC* obj);
// game/tu_80118384.cpp (0x801184F0, 0x1C0 bytes, not written yet). Called with
// fn_8011921C() and a flag (1 from game/tu_80139BE8.cpp, 0 from act_opening_goal);
// the flag is tested as a byte.
extern "C" void fn_801184F0(cls_801192AC* obj, bool flag);
class ActorBase_804A5770;
// game/tu_80118384.cpp. Registers/removes an actor with the object (the actor's
// serial at 0x198 is stored in / removed from the list at 0xC..0x14); called
// by cls_8049D140's init and destructor (tu_8010DF44) and act_opening_goal.
extern "C" void fn_8011888C(cls_801192AC* obj, ActorBase_804A5770* actor);
extern "C" void fn_80118798(cls_801192AC* obj, ActorBase_804A5770* actor);

// game/tu_801C0D70.cpp (owner unit, not written yet). The 0x160-byte object
// in .bss at 0x804C7E70, built on first use by fn_801C0D70 (guard byte
// lbl_8053AC10); named after that function. Only the u16 at 0x0 is known:
// act_shadow_link_2's helper tests its bit 0x400.
struct cls_801C0D70 {
    /* 0x000 */ u16 field_0x0;
    /* 0x002 */ u8 field_0x2[0x50 - 0x2]; // not yet known
    // act_shadow_link_2's fn_801C53F8 tests bit 0x80000000.
    /* 0x050 */ u32 field_0x50;
    /* 0x054 */ u8 field_0x54[0x160 - 0x54]; // not yet known
    // Owner tu_801C0D70. True when a target for the player at *pos was found;
    // writes its x/y to *out. Called by cls_8021C2B4::fn_8021C468 and act_shadow_link_2.
    bool fn_801C0FEC(const JGeometry::TVec3f* pos, JGeometry::TVec2f* out);
    // Owner tu_801C0D70 (symbol fn_801C12B8). Called by act_shadow_link_2's
    // fn_801C53F8 on the object from fn_801C0D70, not yet known beyond that.
    void fn_801C12B8();
}; // Size: 0x160
// Getter of that object (returns its address).
extern "C" cls_801C0D70* fn_801C0D70();

// game/tu_80204638.cpp (owner unit, not written yet). The guarded static in
// .bss at 0x804CADE0 (constructed on first use by fn_80204638, flag lbl_8053AC58);
// named after that function. Only the byte at 0x0 is known: act_bandit_herding_npc's
// update sets it to 1 (and the actor manager's reset functions call the getter).
struct cls_80204638 {
    /* 0x00 */ u8 field_0x0;
    // The inlined constructor writes up to 0xD0 (a TVec3f[10] at 0x58), but the
    // .bss symbol is 0x1D0 bytes: the real size is not known, so none is claimed.
};
// Getter of that object (returns its address).
extern "C" cls_80204638* fn_80204638();
// game/tu_80204638.cpp (fn_802049E0, owner unit, not written yet). Removes the actor
// from the slot the byte at its 0x34a selects in the object's table (0x4 + 8 * slot):
// clears the first slot holding it, else the second. Called by act_thief's destructor
// with fn_80204638()'s result.
class ActorBase_804A5770;
extern "C" void fn_802049E0(cls_80204638* table, ActorBase_804A5770* actor);

// game/tu_801E9AFC.cpp (owner unit, not written yet). One element of the guarded
// static array in .bss at 0x804CA358 (guard lbl_8053AC50), named after its
// constructor fn_801E9B54 (an empty function). Only its member function is known.
struct cls_801E9B54 {
    /* 0x00 */ u8 field_0x0[0x78]; // not yet known
    // Owner tu_801E9AFC. With arg0 == 0, appends arg1 to the list at 0x64 (its
    // count at 0x74) and returns -1. Otherwise finds the entry whose byte at 0x4
    // is arg0, stores arg1 in its word at 0x8, and returns the serial (0x198) of
    // the actor its pointer at 0x0 names, or -1 when there is none.
    s32 fn_801EA6D8(u8 arg0, s32 arg1);
}; // Size: 0x78
// The object at 0x804CA358: two elements, then the byte at 0xF0 that selects
// one of them. Returned by fn_801E9AFC (owner unit tu_801E9AFC, not written yet).
struct cls_801E9AFC {
    /* 0x00 */ cls_801E9B54 field_0x0[2];
    /* 0xF0 */ u8 field_0xf0;
}; // Size: at least 0xF1 (the selector byte is the last field known)
// Getter of that object (returns its address; builds it on first use).
extern "C" cls_801E9AFC* fn_801E9AFC();
// game/tu_801E9AFC.cpp (owner unit, not written yet): draws at pos (x, y only)
// with the given angle (an index into the sin/cos table) and scale. arg0 is
// the slots object fn_801E9AFC returned; the callee doesn't read it at entry.
extern "C" void fn_801EA108(cls_801E9AFC* slots, const JGeometry::TVec2f* pos, s16 angle, f32 scale);

// game/tu_801DA7C0.cpp (owner unit, not written yet): the sequencer at
// cls_804A4C58 + 0x2A8 (include/game/act/tu_801D8400.h defines it). Both read
// its current value (word 0x4), clamped to 0-47: fn_801DA938 returns that
// entry of the string table lbl_804A4DA8; fn_801DA904 whether it is 28 or more.
struct Sequencer_801DAA24;
extern "C" const char* fn_801DA938(Sequencer_801DAA24* seq);
extern "C" bool fn_801DA904(Sequencer_801DAA24* seq);

// game/act_frostare_tail.cpp (owner unit, not written yet): takes an actor
// (a result of ActorManager_804A5800::fn_801F666C; reads fields 0xD8, 0xDC and
// 0x278) and returns a value 0-255 (clrlwi 24 in the body); callers use it
// as an int without extending it (declared s32: a u8 adds a clrlwi at the call).
// Called by game/tu_801DAC6C.cpp.
class ActorBase_804A5770;
extern "C" s32 fn_801DC264(ActorBase_804A5770* actor);

// game/act_navi.cpp (owner unit, not written yet). Each takes a navi actor
// (a result of ActorManager_804A5800::fn_801F666C) in r3; field 0x248 is its
// state. fn_8021D734 / fn_8021D764 switch it to state 8 / 6 unless already
// there; fn_8021D794 returns whether it is in state 5. Called by
// game/tu_8021F064.cpp.
class ActorBase_804A5770;
extern "C" {
void fn_8021D734(ActorBase_804A5770* navi);
void fn_8021D764(ActorBase_804A5770* navi);
bool fn_8021D794(ActorBase_804A5770* navi);
}

// A shared .sdata2 constant (1.0f), owned by game/act_shadow_link_generator's
// .sdata2 (8053EDF0); act_shadow_link_2's helper loads it and subtracts 0.001f at run time.
extern const f32 lbl_8053EDF0;
// The next one (64.0f, 8053EDF4), same owner; act_shadow_link_2's
// game/tu_801C6A4C.cpp loads it and squares it at run time.
extern const f32 lbl_8053EDF4;
// The next three (s32 constants at 8053EDF8, 8053EDFC, 8053EE00; owners:
// act_shadow_link_generator's .sdata2 for the first two, the following unit
// for the last): act_shadow_link_2's game/tu_801D3CBC.cpp converts each to
// float at run time as the divisor of a fade (255 * mStateTimer / value).
extern const s32 lbl_8053EDF8;
extern const s32 lbl_8053EDFC;
extern const s32 lbl_8053EE00;

// game/act_shadow_link_2.cpp (owner unit, defined there). Packs five fields
// into the u32 an 'SLK2' spawn takes as its arg_0x90: a in bits 0-4, b in
// 5-7, c in 8-11, d in 12-15 and e in 20-27. act_shadow_link_generator passes
// (8, 0, slot, 0, 0) for each spawn.
u32 fn_801C5660(u32 a, u32 b, u32 c, u32 d, u32 e);

// Shared .sdata2 constants owned by game/tu_801CB270.cpp (not written yet),
// global consts read by several Shadow Link and enemy units: the gravity step
// (act_shadow_link_2 vfunc_0x84, act_shadow_link_jump, act_terrorpin,
// act_frostare_tail, tu_801D8400) and the fall-speed limit (act_shadow_link_2,
// tu_801DE714).
extern const f32 lbl_8053EFB8;
extern const f32 lbl_8053EFBC;

// Shared .sdata2 constants owned by game/tu_801D3CBC.cpp (not written yet):
// 90.0f and 1.7f. game/tu_801EDBA4.cpp passes the first to fn_802F5FF4 and
// multiplies -12.0f by the second at run time (fmuls), so neither is a literal.
extern const f32 lbl_8053F0B8;
extern const f32 lbl_8053F0BC;

// Owner: game/tu_802F5040.cpp (defined there). f1 in, f1 out: 192 / (192 -
// min(x, 96)) (0x802F5FF4); game/tu_801EDBA4.cpp scales by it.
f32 fn_802F5FF4(f32 x);

// Objects and functions of the boot sequence main (game/tu_801344C0.cpp)
// runs; their owner units are not written yet. Named after their vtables.

// The loader (0x7C bytes; vtable 0x804B0F50, constructor fn_804009CC),
// owner game/tu_803FF2C4.cpp. main allocates it in the main heap and calls
// fn_803FF420 / fn_803FFAA8 each frame while mState is below 4, then
// fn_803FF2C4 (which frees its archives) once it reaches 4.
class cls_804B0F50 {
public:
    cls_804B0F50();
    void fn_803FF2C4();
    void fn_803FF420();
    void fn_803FFAA8();

    /* 0x00 */ void* field_0x0; // vtable 0x804B0F50, kept opaque: its slots are not known yet
    /* 0x04 */ u8 field_0x4[0x78 - 0x4];
    /* 0x78 */ u8 mState;
    /* 0x79 */ u8 field_0x79[0x7C - 0x79];
}; // Size: 0x7C
extern cls_804B0F50* lbl_8053AAF0;

// The memory card manager, owner game/tu_803B9940.cpp (0x2120 bytes at
// lbl_8051C1A0): fn_803BA980 runs CARDInit, initialises its mutex and
// condition and starts its thread; main calls fn_803B9E2C on it every frame.
class cls_803B9940 {
public:
    void fn_803BA980();
    void fn_803B9E2C();
    // SHARED-EDIT (tu_803BC6F0): request polls; each try-locks the mutex at
    // 0x1FD4 and reads the state word 0x1FD0. fn_803B9A94 copies the save
    // data at this + offset + 0x20 / + 0x280 into dst1 / dst2 when the state
    // is 3; fn_803B9BDC / fn_803B9C5C return 1 when the request finished
    // (state 4 / 5), 0 or 2 otherwise.
    s32 fn_803B9A94(void* dst1, u32 size1, void* dst2, u32 size2, s32 offset);
    s32 fn_803B9BDC();
    s32 fn_803B9C5C();
    // Sets the request word 0x1FCC to 4 and signals the condition at 0x1FEC
    // (fn_803CC17C, before it waits in state 0xD).
    void fn_803B9DDC();

    /* 0x0 */ u8 field_0x0[0x1FCC];
    // fn_802B1558 (the reset path) waits, one VIWaitForRetrace per pass, while
    // this word is nonzero.
    /* 0x1FCC */ s32 field_0x1fcc;
    /* 0x1FD0 */ u8 field_0x1fd0[0x1FF4 - 0x1FD0]; // mutex at 0x1FD4, cond at 0x1FEC
    // SHARED-EDIT (tu_803BC6F0 fn_803CC53C): the card channel passed to
    // CARDGetSerialNo.
    /* 0x1FF4 */ u8 field_0x1ff4;
    // SHARED-EDIT (tu_803BC6F0): fn_803CE9EC stores 2 here every frame;
    // fn_803CBE38 / fn_803CC3DC copy it into the first save row
    // (lbl_8050EC80 + 8) after a load.
    /* 0x1FF5 */ u8 field_0x1ff5;
    /* 0x1FF6 */ u8 field_0x1ff6[0x1FF8 - 0x1FF6];
    /* 0x1FF8 */ u64 field_0x1ff8; // CARDGetSerialNo's serial number (fn_803CC53C)
    /* 0x2000 */ u8 field_0x2000[0x2120 - 0x2000];
}; // Size: 0x2120
extern cls_803B9940 lbl_8051C1A0;

// Object with vtable 0x804A8578 (constructor fn_802B1EFC stores it in
// lbl_8053ACA8), owner game/tu_802B1558.cpp. The first three read bytes of
// the save region in lbl_80529DEC (see GameState_804B0FE0::SaveRegion); fn_802B1D98
// clears one and stores 0 at this+6.
class cls_804A8578 {
public:
    // fn_802B1EFC: stores this in lbl_8053ACA8 and clears the bytes below.
    // FIXME: the vtable (__vt__12cls_804A8578, .data 0x804A8578) is 88.57 % in
    // try.py. The original's 4th word is 0; the cause is not diagnosed yet.
    cls_804A8578();
    // fn_802B1EB4: the delete flag > 0 frees the object (__dl__FPv).
    virtual ~cls_804A8578();

    bool fn_802B1DCC();
    bool fn_802B1D7C();
    s32 fn_802B1D60();
    void fn_802B1D98();
    // fn_802B1DB0: tests save-region byte 0xA (tu_80134C04 calls it on
    // lbl_8053ACA8).
    bool fn_802B1DB0();
    // fn_802B1EB0: empty; tu_803FF2C4 calls it on the object it allocated.
    void fn_802B1EB0();
    // fn_802B1DE8: clears this+4/+5 and the save-region bytes 0x9/0xA, then
    // copies the 0x20-byte save region in lbl_80529DEC to 0x80700000.
    void fn_802B1DE8();
    // fn_802B1610: the JUTGamePad C3ButtonReset handler (tu_80134C04's
    // fn_80139994 forwards its port and argument to it). Not written yet.
    void fn_802B1610(s32 port, void* arg);

    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 field_0x6; // fn_802B1D98 clears it
    /* 0x7 */ u8 field_0x7; // 0xFF preset by the constructor
};
extern cls_804A8578* lbl_8053ACA8;

// game/tu_80021840.cpp (0x80021840): a stub that returns 0; tu_80134C04
// compares the result with 0, 1 and 2 (a screen mode query).
s32 fn_80021840();

// Global object with vtable 0x804B0F90 (destructor fn_80401848), owner
// game/tu_80400DC4.cpp; main polls fn_80401518 each frame (it watches the
// DVD drive status) and goes on once it returns 0.
class cls_804B0F90 {
public:
    virtual ~cls_804B0F90();
    s32 fn_80401518();
    s32 field_0x4;
};
extern cls_804B0F90 lbl_8053ADE8;

// game/tu_80401848.cpp (0x80401FEC): creates and resumes a thread of the
// given priority. main passes the current thread's priority minus 1, an
// expanded heap of 0x800 bytes and 0.
extern "C" void fn_80401FEC(s32 priority, JKRHeap* heap, s32 arg);

// .rodata render modes (GXRenderModeObj); main picks lbl_8048C1B8 when
// progressive scan is wanted and active, lbl_8048C17C otherwise.
extern const GXRenderModeObj lbl_8048C1B8;
extern const GXRenderModeObj lbl_8048C17C;

// The object fn_8016A3AC returns (lbl_8053AAF8->field_0x4->field_0x24; named
// after that getter). Owner: game/tu_8016A2B4.cpp (not written yet). Only
// the bytes act_environment_control_2 writes are declared.
struct GameRootSub24_8016A3AC {
    // Unknown pointers (cleared by fn_8016ED88): 0x0, 0x30, 0x38, 0x54, 0x68 (and the model at 0x4C).
    /* 0x000 */ void* field_0x0;
    // The J3DModels below (0x4 ... 0xEC) are passed to fn_8016A7DC.
    /* 0x004 */ struct J3DModel* field_0x4;
    /* 0x008 */ u8 field_0x8[0xC - 0x8];
    /* 0x00C */ struct J3DModel* field_0xc;
    /* 0x010 */ u8 field_0x10[0x14 - 0x10];
    /* 0x014 */ struct J3DModel* field_0x14;
    /* 0x018 */ u8 field_0x18[0x1C - 0x18];
    /* 0x01C */ struct J3DModel* field_0x1c;
    /* 0x020 */ u8 field_0x20[0x28 - 0x20];
    /* 0x028 */ struct J3DModel* field_0x28;
    /* 0x02C */ u8 field_0x2c[0x30 - 0x2C];
    /* 0x030 */ void* field_0x30;
    /* 0x034 */ struct J3DModel* field_0x34;
    /* 0x038 */ void* field_0x38;
    // Six models, placed by fn_8016B0B0 (loop over i < 6).
    /* 0x03C */ struct J3DModel* field_0x3c[6];
    /* 0x054 */ void* field_0x54;
    /* 0x058 */ u8 field_0x58[0x68 - 0x58];
    /* 0x068 */ void* field_0x68;
    /* 0x06C */ u8 field_0x6c[0x84 - 0x6C];
    /* 0x084 */ struct J3DModel* field_0x84;
    /* 0x088 */ u8 field_0x88[0x90 - 0x88];
    /* 0x090 */ struct J3DModel* field_0x90;
    /* 0x094 */ u8 field_0x94[0x9C - 0x94];
    /* 0x09C */ struct J3DModel* field_0x9c;
    /* 0x0A0 */ u8 field_0xa0[0xA8 - 0xA0];
    /* 0x0A8 */ struct J3DModel* field_0xa8;
    /* 0x0AC */ u8 field_0xac[0xB4 - 0xAC];
    /* 0x0B4 */ struct J3DModel* field_0xb4;
    /* 0x0B8 */ u8 field_0xb8[0xC0 - 0xB8];
    /* 0x0C0 */ struct J3DModel* field_0xc0;
    /* 0x0C4 */ u8 field_0xc4[0xCC - 0xC4];
    /* 0x0CC */ struct J3DModel* field_0xcc;
    /* 0x0D0 */ u8 field_0xd0[0xD4 - 0xD0];
    /* 0x0D4 */ struct J3DModel* field_0xd4;
    /* 0x0D8 */ u8 field_0xd8[0xE0 - 0xD8];
    /* 0x0E0 */ struct J3DModel* field_0xe0;
    /* 0x0E4 */ u8 field_0xe4[0xEC - 0xE4];
    /* 0x0EC */ struct J3DModel* field_0xec;
    /* 0x0F0 */ u8 field_0xf0[0x124 - 0xF0];
    // The three emitters fn_8016A2B4 deletes.
    /* 0x124 */ struct JPABaseEmitter* field_0x124;
    /* 0x128 */ struct JPABaseEmitter* field_0x128;
    /* 0x12C */ struct JPABaseEmitter* field_0x12c;
    // Words cleared by fn_8016EDB0 (types unknown).
    /* 0x130 */ s32 field_0x130;
    /* 0x134 */ s32 field_0x134;
    /* 0x138 */ s32 field_0x138;
    /* 0x13C */ s32 field_0x13c;
    /* 0x140 */ s32 field_0x140;
    /* 0x144 */ u8 field_0x144[0x154 - 0x144];
    /* 0x154 */ s32 field_0x154;
    /* 0x158 */ u8 field_0x158[0x15C - 0x158];
    /* 0x15C */ s32 field_0x15c;
    /* 0x160 */ u8 field_0x160[0x170 - 0x160];
    /* 0x170 */ s32 field_0x170;
    /* 0x174 */ u8 field_0x174[0x178 - 0x174];
    /* 0x178 */ s32 field_0x178;
    /* 0x17C */ u8 field_0x17c[0x18C - 0x17C];
    /* 0x18C */ s32 field_0x18c;
    /* 0x190 */ u8 field_0x190[0x194 - 0x190];
    /* 0x194 */ s32 field_0x194;
    /* 0x198 */ u8 field_0x198[0x1A8 - 0x198];
    /* 0x1A8 */ s32 field_0x1a8;
    /* 0x1AC */ u8 field_0x1ac[0x1B0 - 0x1AC];
    /* 0x1B0 */ s32 field_0x1b0;
    /* 0x1B4 */ u8 field_0x1b4;
    /* 0x1B5 */ u8 field_0x1b5[0x1B8 - 0x1B5];
    /* 0x1B8 */ s32 field_0x1b8;
    /* 0x1BC */ u8 field_0x1bc;
    /* 0x1BD */ u8 field_0x1bd[0x1C0 - 0x1BD];
    /* 0x1C0 */ s32 field_0x1c0;
    /* 0x1C4 */ u8 field_0x1c4;
    /* 0x1C5 */ u8 field_0x1c5;
    /* 0x1C6 */ u8 field_0x1c6;
    /* 0x1C7 */ u8 field_0x1c7;
    /* 0x1C8 */ u8 field_0x1c8;
    /* 0x1C9 */ u8 field_0x1c9;
    /* 0x1CA */ u8 field_0x1ca;
    /* 0x1CB */ u8 field_0x1cb[4];
    /* 0x1CF */ u8 field_0x1cf;
    /* 0x1D0 */ u8 field_0x1d0;
    /* 0x1D1 */ u8 field_0x1d1;
    /* 0x1D2 */ u8 field_0x1d2;
    /* 0x1D3 */ u8 field_0x1d3;
    /* 0x1D4 */ u8 field_0x1d4[4];
    /* 0x1D8 */ s32 field_0x1d8;
    /* 0x1DC */ u8 field_0x1dc;
    /* 0x1DD */ u8 field_0x1dd;
    /* 0x1DE */ u8 field_0x1de[0x1E4 - 0x1DE];
    /* 0x1E4 */ f32 field_0x1e4;
    /* 0x1E8 */ u8 field_0x1e8[0x1EC - 0x1E8];
    /* 0x1EC */ u8 field_0x1ec;
    /* 0x1ED */ u8 field_0x1ed;
    /* 0x1EE */ u8 field_0x1ee;
    /* 0x1EF */ u8 field_0x1ef;
    /* 0x1F0 */ u8 field_0x1f0;
    /* 0x1F1 */ u8 field_0x1f1;
    /* 0x1F2 */ u8 field_0x1f2;
    /* 0x1F3 */ u8 field_0x1f3;
    /* 0x1F4 */ u8 field_0x1f4;
    /* 0x1F5 */ u8 field_0x1f5;

    // game/tu_8016A2B4.cpp (0x8016A2B4): deletes the three emitters.
    void fn_8016A2B4();
    // 0x8016A330: clears the flag bytes, sets field_0x1d8 to -1.
    void fn_8016A330();
    // 0x8016A7DC: places model at pos plus an offset that follows the
    // 2D camera (x / y flags), scaled by `scale`, then runs the model's
    // calc / entry / viewCalc (vtable slots 0x10, 0xC, 0x1C). `this` is not read.
    void fn_8016A7DC(struct J3DModel* model, const JGeometry::TVec3f& pos, s32 dir, bool followX, bool followY, f32 parallax, f32 scale);

    // game/tu_8016A2B4.cpp: each draws one model when its flag is set (fn_8016A7DC).
    void fn_8016A9EC();
    void fn_8016AA48();
    void fn_8016AAA4();
    void fn_8016AB00();
    void fn_8016AB5C();
    void fn_8016ABB8();
    void fn_8016AC14();
    void fn_8016AC70();
    void fn_8016ACCC();
    void fn_8016AD28();
    void fn_8016AD84();
    void fn_8016B054();
    void fn_8016D284();
    void fn_8016D2E0();
    void fn_8016D33C();
    // 0x8016B8AC and 0x8016ED88 (clears the pointer fields).
    void fn_8016B8AC();
    void fn_8016ED88();
    // 0x8016B0B0: places the six models of field_0x3c against the 2D camera.
    void fn_8016B0B0();
    // Not written yet (game/tu_8016A2B4.cpp): called by fn_8016DA6C / fn_8016DB60.
    void fn_8016ADE0();
    // game/tu_8016A2B4.cpp (0x8016DA6C, not written yet). Takes `this` as its
    // only argument; called by fn_80139CC4 (game/tu_80139BE8.cpp).
    void fn_8016DA6C();
    // game/tu_8016A2B4.cpp (0x8016DB60, not written yet): runs a sequence of
    // fn_8016xxxx calls on this; called by fn_80139D3C.
    void fn_8016DB60();
    // game/tu_8016A2B4.cpp (0x8016EDB0, not written yet); called by fn_8013A1A4.
    void fn_8016EDB0();
    // game/tu_8016A2B4.cpp (0x8016EE84, 0x238 bytes, not written yet). Called
    // with 1 by GameRootSub04_801F856C's destructor, without a null check,
    // before it clears field_0x24.
    void fn_8016EE84(s32 arg);
};
// game/tu_8016A2B4.cpp (0x8016A3AC): returns lbl_8053AAF8->field_0x4->field_0x24.
GameRootSub24_8016A3AC* fn_8016A3AC();
// Owner: game/tu_8022AD64.cpp (0x8023F728, 0x8023F78C). Called by fn_8013A1A4: each
// runs a per-player routine on all four players (fn_8023F78C also stores -1 to lbl_8053A190).
void fn_8023F728();
void fn_8023F78C();
extern "C" {
// Owner not known (0x80191744, 0x9C bytes, not written yet). Called on
// field_0x20 of GameRootSub04_801F856C by its destructor (fn_8013A27C) with 1.
void fn_80191744(void* self, s32 arg);
// Owner: game/tu_801902C0.cpp (0x801902D0, not written yet). Takes the object
// at field_0x20 of GameRootSub04_801F856C (fn_80139CC4); its type is unknown.
void fn_801902D0(void* self);
// Owner not known (0x8023F2BC, not written yet). No arguments; called by
// fn_80139CC4 (game/tu_80139BE8.cpp) between the two fn_8013B438 calls.
void fn_8023F2BC(void);
// Owners not known (0x8023F088, 0x8023F728, 0x8023F78C, 0x8038D870,
// 0x803B8C90, 0x8043EDF8, 0x80439AB0, 0x8019169C; not written yet). Called by
// fn_8013A1A4 (game/tu_80139BE8.cpp); arguments are its fields.
void fn_8023F088(void);
void fn_8038D870(void);
u8 fn_803B8C90(void);
void fn_8043EDF8(void* self);
void fn_80439AB0(void* self);
void fn_8019169C(void* self);
}

// The 4-byte pointer at .sbss lbl_8053AD68 (the cls_804AE9A0 object, declared
// in game/tu_80379F84.h with its members fn_8037A708, fn_8037A580 and
// fn_8037A664). Used by game/tu_80134C04.cpp's fn_80139958 and the code
// around 0x80136FC4.
extern cls_804AE9A0* lbl_8053AD68;

// SHARED-EDIT (act_ice_melted_by_four_players): owner game/act_fire_rod_fire.cpp
// (0x802BA9F0 and 0x802BA9F8, defined there; the FIRE actor's unit, not written
// yet). Both read a word of the actor in r3 (lwz 0x260 and 0x264) and return
// it; the only caller passes the FIRE actor it finds by serial as an
// ActorBase_804A5770*.
extern "C" {
s32 fn_802BA9F0(ActorBase_804A5770* actor);
s32 fn_802BA9F8(ActorBase_804A5770* actor);
}

// SHARED-EDIT (tu_803F0A54, review): the singleton *lbl_8053AEC0
// (game/tu_8042E964.cpp, owner not written yet), named after its vtable
// 0x804B1634; its constructor fn_80431D84 stores `this` in lbl_8053AEC0.
// Size and most fields unknown.
class cls_804B1634 {
public:
    // 0x804316EC: draws field_0x8 with lbl_8053AB10's grafcontext unless
    // the byte 0xEC is 0. Called by cls_804B1D58::fn_804522C8 (tu_8044C7F0).
    void fn_804316EC();
    // 0x804310CC: no arguments besides `this` (lwz 8(r3), J2DPane search at
    // vtable 0x3C, stores at 0xC and 0xEB). cls_804B0F10::fn_803FC5E4.
    void fn_804310CC();
    // 0x80431A9C: r4 an id (0xC4, 0x10B), r5 a byte (clrlwi 24 in
    // cls_804B0F10::fn_803F2AEC), r6 an optional out byte (callers pass the
    // address of a u8 field or 0; it receives this->0xEA). Reads the byte 0xEC,
    // calls fn_8042EF9C on `this` and returns 1 or 0 (li r3 before the blr).
    bool fn_80431A9C(s32 id, u8 value, u8* out);
};
extern cls_804B1634* lbl_8053AEC0;

// game/tu_801A0710.cpp (.sdata 0x8053A100, not written yet; -1 in the
// original): act_terrorpin's vfunc_0x44 compares its field_0x2a8 with it.
extern s32 lbl_8053A100;

// SHARED-EDIT (act_battle_royale_end_check fn_803D6BDC): the only callers
// known so far. Owner units not written yet. fn_80239F10 (0x80239F10, 0x294
// bytes) takes no arguments and returns a bool; fn_802B3A48 (0x802B3A48,
// 0x6C bytes) takes the actor it was found in, as ActorBase_804A5770*.
// SHARED-EDIT (act_battle_royale_end_check fn_803D6494): called with the
// battle royale's GameRoot in r3 and 10; owner unit not written yet.
extern "C" {
bool fn_80239F10();
void fn_802B3A48(ActorBase_804A5770* actor);
void fn_801367E0(GameRoot_8049EF20* root, s32 value);
}

// game/tu_80301CD0.cpp (0x8030361C, 0x134 bytes, not written yet). Takes no
// arguments; called by game/tu_80139BE8.cpp (fn_80139BE8).
extern "C" void fn_8030361C();
// game/tu_8037DF68.cpp (0x8038D454, 0x54 bytes, not written yet). Takes no
// arguments; called by game/tu_80139BE8.cpp (fn_80139BE8) and tu_802D39B8.
extern "C" void fn_8038D454();
// .sbss byte at 0x8053ACD0, owned by game/tu_80301CD0.cpp (its split starts
// there). Read with lbz by that unit; stored as 0 by game/tu_80139BE8.cpp.
extern u8 lbl_8053ACD0;
// .sbss byte at 0x8053AB60 (8 bytes). Read with lbz by game/act_chief_soldier.cpp
// (draw), which skips the draw when it is 1; no writer found yet.
extern u8 lbl_8053AB60;

// .sbss word at 0x8053AB9C, owned by game/tu_80197070.cpp (not written yet).
// Set to 4 by act_recovery_heart (update) when ZLD2 exists; act_fairy writes it too.
extern s32 lbl_8053AB9C;
// .sbss word at 0x8053ABEC, owned by game/act_dodongo.cpp (data not defined yet).
// Set to 4 by act_recovery_heart (update) when ZLD4 exists; act_fairy writes it too.
extern s32 lbl_8053ABEC;

#endif
