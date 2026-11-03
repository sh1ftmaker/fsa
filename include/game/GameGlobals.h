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
// PSMTX/PSVEC call proves it (fn_8030C5F0's mangled name says Vec).
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function that shows
// it.

#include "types.h"
#include "dolphin/mtx.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/TColor.h"

class ActorManager_804A5800;
class JPAEmitterManager;
// JSystem/J2D/J2DGrafContext.h (mkdd's J2D, which FSA's J2DGraph units use).
struct J2DOrthoGraph;

// lbl_8053AAF8->0x4->0xC, returned by fn_802D6A9C (type unknown; named after
// that getter, no vtable or constructor known). The
// box at 0xCC is the visible area in top-down coordinates (x, y - z):
// ActorBase_804A5770's destructor lets an emitter inside it run out instead of
// deleting it.
struct GameView_802D6A9C {
    /* 0x00 */ u8 field_0x0[0xCC];
    /* 0xCC */ f32 mVisibleMinX;
    /* 0xD0 */ f32 mVisibleMinY;
    /* 0xD4 */ f32 mVisibleMaxX;
    /* 0xD8 */ f32 mVisibleMaxY;
    /* 0xDC */ u8 field_0xdc[0x131DE - 0xDC]; // 0xDC: a second box, used for
                                              // actor layers 8 and up (0x801F165C)
    /* 0x131DE */ u8 field_0x131de; // lbz in act_bg_rewrite_check_switch;
                                    // stb in fn_802D782C and 8 more places in
                                    // game/tu_802D39B8.cpp
};

// The object at offset 0x4 of *lbl_8053AAF8 (type unknown; named after
// fn_801F856C, the first function seen reading it).
struct GameRootSub04_801F856C {
    /* 0x0 */ u8 field_0x0[0x8];
    /* 0x8 */ ActorManager_804A5800* mActorManager; // fn_801F856C; allocated in
                                           // game/tu_80139BE8.cpp
    /* 0xC */ GameView_802D6A9C* mView;             // fn_802D6A9C
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
};

extern GameRoot_8049EF20* lbl_8053AAF8;

// lbl_8053AB10->0x28 (type unknown; named after its getter fn_8013C824).
struct EffectSystem_8013C824 {
    /* 0x0 */ u8 field_0x0[0xC];
    /* 0xC */ JPAEmitterManager* mEmitterManager;
};

// lbl_8053AB10->0x24. No vtable; its methods are in game/tu_8030C058.cpp.
// Also called from tu_8011CEC8, tu_8015F454, tu_802F5040, tu_8045BA84.
class cls_8030C058 {
public:
    // Multiplies pos by the Mtx at +0x74 (PSMTXMultVec) and writes x, y and a
    // constant z to out: a world-to-screen projection. With the 2nd argument
    // false it scales by JUTVideo's frame size (progress/game__tu_8045BA84.md).
    void fn_8030C5F0(const Vec* pos, bool, bool, Vec* out);
};

// The objects at lbl_8053AB10->0x20, 0x2C and 0x30, named after their
// vtables (stored at 0 by their constructors); no members declared yet.
// 0x804AA858: game/tu_802F5040.cpp, constructor fn_802FFDEC, 0x11B9C bytes.
// Called with r3 = it by fn_802F9974 (197 sites), fn_802F86CC (143),
// fn_802F9908, fn_802F745C, fn_802F6CF4, ... in 138 units.
class cls_804AA858;
// 0x804A0160: game/tu_80158624.cpp, constructor fn_801594E8, 0x10 bytes.
// Read by fn_80158B3C / fn_80158B5C and directly (+0x4, +0xC) in 28 units.
class cls_804A0160;
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
    /* 0x24 */ cls_8030C058* field_0x24;
    /* 0x28 */ EffectSystem_8013C824* field_0x28;
    /* 0x2C */ cls_804A0160* field_0x2c;
    /* 0x30 */ cls_804A03A8* field_0x30;
};

extern GameRoot2_8049F108* lbl_8053AB10;

// Out-of-line getters (extern "C" keeps the dtk names as link names until
// their units name them).
extern "C" {
GameView_802D6A9C* fn_802D6A9C(void);     // lbl_8053AAF8->0x4->0xC
EffectSystem_8013C824* fn_8013C824(void); // lbl_8053AB10->0x28
}

// Per-player queries in game/tu_8022AD64.cpp (player index 0-3; each
// checks lbl_8053AC90 and the index first). Names are dtk's.
extern "C" {
bool fn_8023F264(s32 player);  // the player takes part
s32 fn_8023DE58(s32 player);   // compared with ActorBase_804A5770::field_0x4
JGeometry::TVec3f* fn_8023E724(s32 player); // the player's position
// Called by ActorBase_804A5770::vfunc_0x18 with the actor's screen box, field_0x4
// and mSerial when the box overlaps a player's.
void fn_802397B4(const JGeometry::TBox2f* box, s32 arg_0x4, s32 serial);
}

// Functions of game/tu_802D39B8.cpp that take the view (fn_802D6A9C()) as
// their first argument. Several ignore it and re-fetch the view through
// lbl_8053AAF8 (said at each); names are dtk's.
extern "C" {
// Stores value at 0x5C of the view (r3 unused).
void fn_802D7934(GameView_802D6A9C* view, s32 value);
// Sets the byte at 0x131F0 of the view from flag (clrlwi 24, cmplwi 1) and
// lbl_8053AAF8's bytes 0x7E/0x7F.
void fn_802D7440(GameView_802D6A9C* view, bool flag);
// ORs bits into the u16 at 0x131B8 + index * 2 of the view (r3 unused).
void fn_802D77BC(GameView_802D6A9C* view, s32 index, u16 bits);
// Tests bit `bit` of the word at 0x348 + (view->0x4 * 16) + (view->0x8 * 4);
// returns 0 or 1 (neg/or/srwi 31).
bool fn_802D7F40(GameView_802D6A9C* view, s32 bit);
// Returns the byte at lbl_8053AB10->0x24->0x1E8 (r3 unused).
bool fn_802DCD0C(GameView_802D6A9C* view);
}

// game/tu_8022461C.cpp. Returns a 16-bit value for a position (clrlwi 22 of
// a lookup on a 0x200 x 0x200 grid relative to the view's box: 0xCC, or 0xDC
// for layer 8 and up), 0xF when the position is outside the box; callers
// clrlwi 16 the result.
extern "C" u16 fn_80226850(const JGeometry::TVec3f* pos, s32 layer);

// game/tu_8022AD64.cpp: set (fn_8022ADE4) and clear (fn_8022ADCC) the byte at
// 0x73 of *lbl_8053AC90 when it exists.
extern "C" {
void fn_8022ADE4(void);
void fn_8022ADCC(void);
}

// game/tu_8037DF68.cpp. Sets up a message box: msg is compared unsigned with
// the id cached at 0x3C of *lbl_8053AD78, param is stored as a word at 0x7C,
// param2 as a byte at 0xE5, the floats are a position (callers pass mPos.x,
// mPos.y, mPos.z, mPos.z), and the colour (passed by value) is copied byte by
// byte into the text box.
extern "C" void fn_8037F2FC(u32 msg, s32 param, u8 param2, f32 x, f32 y, f32 z, f32 f4,
                            JUtility::TColor color);

#endif
