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
};

// *lbl_8053AAF8: the most-used game singleton (309 of 529 units read it).
// Set in game/tu_801344C0.cpp (0x80134998) and game/tu_80134C04.cpp
// (0x80139340). Named after its vtable, 0x8049EF20 (constructor
// fn_80139300, allocated with __nw__(0x11C)); its real layout is unknown.
struct GameRoot_8049EF20 {
    /* 0x00 */ u8 field_0x0[0x4];
    /* 0x04 */ GameRootSub04_801F856C* field_0x4; // -> 0x8: the actor manager
    /* 0x08 */ u8 field_0x8[0x7E - 0x8];
    /* 0x7E */ u8 field_0x7e;
    /* 0x7F */ u8 field_0x7f;
    /* 0x80 */ u8 field_0x80[0x2];
    /* 0x82 */ u8 field_0x82; // bit i set: player i takes part (fn_801F6068)
    /* 0x83 */ u8 field_0x83;
    /* 0x84 */ u8 field_0x84[0xA4 - 0x84];
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

    // 15 units / 17 calls. Stores at this+0x50, 0x58 and 0x84 (per review);
    // act_ending_map_skipper calls it with (15, 0).
    void fn_80136798(s32 a, s32 b);
    // ~28 units. In tu_80134C04 with GameRoot's constructor; uses this->0xA0.
    // r3 only.
    void fn_80138B7C();
    // Stores r4 at this+0x96 (stb). Defined in game/tu_80134C04.cpp; called
    // by act_stop_npc (0x802ED1B4, 0).
    void fn_80134FBC(u8 value);
    // Owner: game/tu_80134C04.cpp (next to fn_80134FBC; not written yet).
    // 1 unit. act_wait_switch calls it with (10, count) when its switch is
    // seen; r3 is lbl_8053AAF8 (the only object this unit's calls pass).
    void fn_80134DE4(s32 a, s32 b);
};

extern GameRoot_8049EF20* lbl_8053AAF8;

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
    /* 0x00 */ u8 field_0x0[0x58];
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

// Random helpers on lbl_8053AAF8->mRandom, defined in game units.
f32 fn_80154744();          // [0, 1); game/act_sword_soldier.cpp
s32 fn_801CD664(s32 n);     // [0, n); game/tu_801CB270.cpp
s32 fn_801CD580(s32 except); // [0, 4) except `except`; game/tu_801CB270.cpp

// game/tu_801CB270.cpp functions its source doesn't define yet.
extern "C" {
s32 fn_801CD398(s32 a, s32 b); // [0, 4) avoiding a (and b)
// Steps *angle towards target by at most step (a tail-call wrapper of
// fn_801CD970; the 0/1 it passes on is unused by all 9 callers).
void fn_801CD930(s16* angle, s16 target, s16 step);
}

// Loads the TIMG resource `name` from lbl_8053AAF8->field_0xa4 into texObj
// (and tlutObj, if given, for a palette texture); returns height / width.
// game/tu_801CB270.cpp; called from game/tu_801E9AFC.cpp.
f32 fn_801CB270(const char* name, GXTexObj* texObj, GXTlutObj* tlutObj);

// The objects at lbl_8053AB10->0x2C and 0x30, named after their vtables
// (stored at 0 by their constructors); no members declared yet. 0x20
// (cls_804AA858) is in game/Map.h, 0x24 (Camera_804AA9E4) in game/Camera.h.
// 0x804A0160: game/tu_80158624.cpp, constructor fn_801594E8, 0x10 bytes.
// Read by fn_80158B3C / fn_80158B5C and directly (+0x4, +0xC) in 28 units.
class JUTTexture;
class cls_804A0160 {
public:
    /* 0x0 */ void* vtable;      // from fn_801594E8
    /* 0x4 */ s32 field_0x4;
    /* 0x8 */ JUTTexture* field_0x8;
    /* 0xC */ u8 field_0xc;      // act_post_warp_pillar_handling init stores 0xFF
    /* 0xD */ u8 field_0xd[3];   // not yet known

    // 0x80158B5C (owner unit game/tu_80158624.cpp, not written yet). Draws a
    // shadow-like sprite on `layer` (< 8) at *pos with a colour and scale;
    // `this` is lbl_8053AB10->0x2C. Called by tu_802201D8's draw (colour
    // 0,0,0,0x80; scale 24.0f). The colour is a TColor passed by value (a
    // stack copy, its address in r6).
    void fn_80158B5C(s32 layer, const JGeometry::TVec3f* pos, JUtility::TColor color, f32 scale);
}; // Size: 0x10
// 0x804A03A8: game/tu_8015F454.cpp, constructor fn_801602E8, 0x298 bytes.
// Its +0x284 is read in 56 places; fn_8015F8FC takes it.
class cls_804A03A8;

// *lbl_8053AB10: the second most-used singleton (267 units). Set in
// game/tu_8013AAD4.cpp (0x8013C024) and game/tu_80134C04.cpp (0x801393C4).
// Named after its vtable, 0x8049F108 (constructor fn_8013BFEC, which
// allocates every member below and stores it at the offset given).
struct GameRoot2_8049F108 {
    /* 0x00 */ u8 field_0x0[0x10];
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
};

extern GameRoot2_8049F108* lbl_8053AB10;

// The input object *lbl_8053AD30 (game/tu_803099FC.cpp, owner not written
// yet): named after its vtable, 0x804AA8C8 (constructor fn_8030AC4C, which
// stores it in lbl_8053AD30). Members unknown; the functions below take it
// in r3, so they stay extern "C" until that unit defines them.
class cls_804AA8C8;
extern cls_804AA8C8* lbl_8053AD30;
extern "C" {
// Owner: game/tu_803099FC.cpp. fn_8030AAF4 maps a player index (0-3) to an
// input slot (-0x3E7 for none). fn_80309D24 and fn_80309D6C return the word
// at 4 + 4*slot (resp. 0x24 + 4*slot) of the object ANDed with mask, or 0
// for slot -0x3E7. act_dummy_conversation's update tests the first for held
// buttons and the second for fresh presses (mask 0x100/0x200/0x400/0x20 and
// 1/2).
s32 fn_8030AAF4(s32 player);
u32 fn_80309D24(cls_804AA8C8* input, s32 slot, u32 mask);
u32 fn_80309D6C(cls_804AA8C8* input, s32 slot, u32 mask);
} // extern "C"

// The object *lbl_8053AED0 (game/tu_8043A0E8.cpp, owner not written yet):
// named after its vtable, 0x804B1920 (constructor fn_8043F504, which stores
// it in lbl_8053AED0). Only the bytes at 0x1F0 are known (act_dummy_conversation
// stores 2 at 0x1F0 + player); its size and the rest are unknown.
class cls_804B1920 {
public:
    /* 0x000 */ u8 field_0x0[0x1F0];
    /* 0x1F0 */ u8 field_0x1f0[4]; // per player
};
extern cls_804B1920* lbl_8053AED0;

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

// game/tu_8012FF9C.cpp (owner unit, not written yet). The guarded static at
// 0x8053AAC4: no vtable, constructor inlined into fn_8012FF9C, named after
// its destructor fn_8012FFF4, which fn_8012FF9C registers with
// __register_global_object. fn_801300D8 sets its words at 0x0 and 0x4 to -1
// and 0. Called by cls_8049EA00::init.
struct cls_8012FFF4;
// Getter of the guarded static (returns its address).
extern "C" cls_8012FFF4* fn_8012FF9C();
// Appends `value` to the object's list when it is positive: stores it at
// index obj[1] (stwx r4, r3, obj[1] * 4) and increments obj[1].
extern "C" void fn_801300B4(cls_8012FFF4* obj, s32 value);

// The 0x20-byte object in .bss at 0x804BC540 (function-local static of
// fn_8011921C, built on first use); named after its destructor fn_801192AC. Only the fields
// fn_801186B0 writes are known.
struct cls_801192AC {
    /* 0x00 */ u8 field_0x0;
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u8 field_0x2;
    /* 0x04 */ u32 field_0x4;
    /* 0x08 */ u32 field_0x8;
    /* 0x0C */ u8 field_0xc[0x14]; // not yet known
}; // Size: 0x20

// game/tu_80118384.cpp (not written yet). Returns the static cls_801192AC.
extern "C" cls_801192AC* fn_8011921C();
// game/tu_80118384.cpp. Sets field_0x0 = 1, field_0x1 = field_0x2 = 0,
// field_0x4 = value, field_0x8 = 0 unless fn_801F68E0 (after fn_801F856C)
// returns 0; returns whether it did. Called with (fn_8011921C(), 2) from
// act_four_sword_demo.
extern "C" bool fn_801186B0(cls_801192AC* obj, u32 value);

// game/tu_801C0D70.cpp (owner unit, not written yet). The 0x160-byte object
// in .bss at 0x804C7E70, built on first use by fn_801C0D70 (guard byte
// lbl_8053AC10); named after that function. Only the u16 at 0x0 is known:
// act_shadow_link_2's helper tests its bit 0x400.
struct cls_801C0D70 {
    /* 0x000 */ u16 field_0x0;
    /* 0x002 */ u8 field_0x2[0x160 - 0x2]; // not yet known
}; // Size: 0x160
// Getter of that object (returns its address).
extern "C" cls_801C0D70* fn_801C0D70();

// A shared .sdata2 constant (1.0f), owned by game/act_shadow_link_generator's
// .sdata2 (8053EDF0); act_shadow_link_2's helper loads it and subtracts 0.001f at run time.
extern const f32 lbl_8053EDF0;

#endif
