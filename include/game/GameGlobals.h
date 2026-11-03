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
class JAISeMgr;
class JPAEmitterManager;
// JSystem/J2D/J2DGrafContext.h (mkdd's J2D, which FSA's J2DGraph units use).
class J2DOrthoGraph;

// The object at offset 0x4 of *lbl_8053AAF8 (type unknown; named after
// fn_801F856C, the first function seen reading it).
struct GameRootSub04_801F856C {
    /* 0x0 */ u8 field_0x0[0x8];
    /* 0x8 */ ActorManager_804A5800* mActorManager; // fn_801F856C; allocated in
                                           // game/tu_80139BE8.cpp
    /* 0xC */ GameView_804A9950* mView;             // fn_802D6A9C
    /* 0x10 */ u8 field_0x10[0x24 - 0x10];
    // fn_8016A3AC returns it (game/tu_8016A2B4.cpp).
    /* 0x24 */ struct GameRootSub24_8016A3AC* field_0x24;
    /* 0x28 */ u8 field_0x28[0x30 - 0x28];
    // A heap: game/tu_8044B80C.cpp makes it current, frees its J2DScreens
    // and calls freeAll on it (becomeCurrentHeap / freeAll calls on r3).
    /* 0x30 */ JKRHeap* field_0x30;
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
    /* 0xFF */ u8 field_0xff[0x108 - 0xFF];
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
    /* 0x340 */ u8 field_0x340[0x5F0 - 0x340]; // not yet known
    /* 0x5F0 */ QueuedPos mQueuedPos[2];
    /* 0x618 */ s32 mQueuedPosCount;
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

// game/tu_801CB270.cpp functions its source doesn't define yet.
extern "C" {
s32 fn_801CD398(s32 a, s32 b); // [0, 4) avoiding a (and b)
// Steps *angle towards target by at most step (a wrapper of fn_801CD970;
// the 0/1 it passes on is unused by all 9 callers).
void fn_801CD930(s16* angle, s32 target, s32 step);
// game/tu_801CB270.cpp (not written yet): the stepper fn_801CD930 wraps; true
// when *angle reached target.
bool fn_801CD970(s16* angle, s32 target, s32 step);
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

// game/tu_801CB270.cpp (not written yet); called from game/tu_80207548.cpp and
// game/tu_80220F54.cpp. Forwards to ActorManager_804A5800::fn_801F8544 with
// the fourcc 'RUPY' (0x52555059) and the layer and pos; the flag selects 4 or
// 5 in the low bits of its last argument. Returns the result (an actor serial).
extern "C" {
s32 fn_801CB434(s32 layer, const JGeometry::TVec3f* pos, bool flag);
}

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
extern "C" {
bool fn_801CCF7C();
}

// The objects at lbl_8053AB10->0x2C and 0x30, named after their vtables
// (stored at 0 by their constructors); no members declared yet. 0x20
// (cls_804AA858) is in game/Map.h, 0x24 (Camera_804AA9E4) in game/Camera.h.
// 0x804A0160: game/tu_80158624.cpp, constructor fn_801594E8, 0x10 bytes.
// Read by fn_80158B3C / fn_80158B5C and directly (+0x4, +0xC) in 28 units.
class JUTTexture;
class cls_804A0160 {
public:
    /* 0x0 */ void* vtable;      // from fn_801594E8
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
}; // Size: 0x10
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
    /* 0x000 */ u8 pad_0x0[0x1F0]; // vtable pointer and unknown fields
    /* 0x1F0 */ u8 field_0x1f0[4]; // per player

    // 0x8043D67C (game/tu_8043A0E8.cpp): 27 units call it with *lbl_8053AED0
    // in r3, an id in r4 (tu_8020FE50's actor passes 0x1A8 or 0x1C1) and two
    // more arguments (0 in most calls).
    void fn_8043D67C(s32 id, u32 arg1, u32 arg2);

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
};
extern cls_804B1920* lbl_8053AED0;

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

// game/act_quicksand.cpp (owner unit, not written yet). Both take an actor
// (a result of ActorManager_804A5800::fn_801F666C) in r3 and a position in r4:
// fn_80211000 returns 0/1 (clrlwi 24) for "is the position inside the actor's
// area" and fn_80210E90 writes the push-out vector to *out (r5; zeroed first).
// Called by game/tu_80212AC0.cpp's fn_80212AF0.
class ActorBase_804A5770;
extern "C" {
bool fn_80211000(ActorBase_804A5770* actor, const JGeometry::TVec3f* pos);
void fn_80210E90(const ActorBase_804A5770* actor, const JGeometry::TVec3f* pos, JGeometry::TVec3f* out);
}

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

// Shared .sdata2 constants owned by game/tu_801D3CBC.cpp (not written yet):
// 90.0f and 1.7f. game/tu_801EDBA4.cpp passes the first to fn_802F5FF4 and
// multiplies -12.0f by the second at run time (fmuls), so neither is a literal.
extern const f32 lbl_8053F0B8;
extern const f32 lbl_8053F0BC;

// Owner: game/tu_802F5040.cpp (defined there). f1 in, f1 out: 1.0f / (1.0f -
// max(x, a constant)) (0x802F5FF4); game/tu_801EDBA4.cpp scales by it.
extern "C" f32 fn_802F5FF4(f32 x);

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

    /* 0x0 */ u8 field_0x0[0x1FCC];
    // fn_802B1558 (the reset path) waits, one VIWaitForRetrace per pass, while
    // this word is nonzero.
    /* 0x1FCC */ s32 field_0x1fcc;
    /* 0x1FD0 */ u8 field_0x1fd0[0x2120 - 0x1FD0]; // mutex at 0x1FD4, cond at 0x1FEC
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
    // fn_802B1DE8: clears this+4/+5 and the save-region bytes 0x9/0xA, then
    // copies the 0x20-byte save region in lbl_80529DEC to 0x80700000.
    void fn_802B1DE8();

    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 field_0x6; // fn_802B1D98 clears it
    /* 0x7 */ u8 field_0x7; // 0xFF preset by the constructor
};
extern cls_804A8578* lbl_8053ACA8;

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
    /* 0x000 */ u8 field_0x0[0x1CA];
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
    /* 0x1DE */ u8 field_0x1de[0x1EF - 0x1DE];
    /* 0x1EF */ u8 field_0x1ef;
    /* 0x1F0 */ u8 field_0x1f0[2];
    /* 0x1F2 */ u8 field_0x1f2;
    /* 0x1F3 */ u8 field_0x1f3;
    /* 0x1F4 */ u8 field_0x1f4;
    /* 0x1F5 */ u8 field_0x1f5;
};
extern "C" {
// Owner: game/tu_8016A2B4.cpp (0x8016A3AC, defined there; not written yet).
// No arguments; loads lbl_8053AAF8->field_0x4->field_0x24 (lwz 0x4, lwz 0x24).
GameRootSub24_8016A3AC* fn_8016A3AC(void);
}

// The 4-byte pointer at .sbss lbl_8053AD68 (type unknown). Passed in r3 to
// fn_8037A708, fn_8037A580 and fn_8037A664 (game/tu_80379F84.cpp) by
// game/tu_80134C04.cpp's fn_80139958 and the code around 0x80136FC4.
extern void* lbl_8053AD68;

extern "C" {
// Owner: game/tu_80379F84.cpp (0x8037A708, defined there; not written yet).
// Takes the object lbl_8053AD68 holds; it reads the pointers at 0x4, 0x8 and
// 0xC and clears the byte at 0x1C.
void fn_8037A708(void* obj);
}

// SHARED-EDIT (act_blizzard_battle): owner game/tu_802B3674.cpp (0x802B39E8,
// defined there; not written yet). Takes the actor in r3 (fn_801F666C's result)
// and reads its 0x23C emitter (forceDeleteEmitter) and 0x290 word.
extern "C" {
void fn_802B39E8(ActorBase_804A5770* actor);
}

// SHARED-EDIT (act_princess_zelda_ending): owner game/tu_8035C15C (0x8035C15C
// and 0x8035C1C4, defined there; not written yet). Both take the actor in r3
// (the EZLD actor, act_princess_zelda_ending.cpp) and an id in r4; the
// caller ignores the result of fn_8035C1C4. fn_8035C15C returns a byte that
// the caller tests against 1 (clrlwi 24, cmplwi 1).
extern "C" {
u8 fn_8035C15C(ActorBase_804A5770* actor, s32 id);
void fn_8035C1C4(ActorBase_804A5770* actor, s32 id, s32 arg);
}

#endif
