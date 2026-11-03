#ifndef GAME_GAMEGLOBALS_H
#define GAME_GAMEGLOBALS_H

// Globals most game units touch. Names are dtk's placeholders (they are the
// link names, so keep them until a unit that defines them names them);
// types are placeholders too. Field types come from the loads (lwz -> a
// 32-bit value or pointer, lbz -> u8); pointers whose target is unknown
// are void*.
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function that shows
// it.

#include "types.h"

class ActorManager;
struct JPAEmitterManager;

// lbl_8053AAF8->0x4->0xC, returned by fn_802D6A9C (type unknown). The
// box at 0xCC is the visible area in top-down coordinates (x, y - z):
// ActorBase's destructor lets an emitter inside it run out instead of
// deleting it.
struct GameView {
    /* 0x00 */ u8 field_0x0[0xCC];
    /* 0xCC */ f32 mVisibleMinX;
    /* 0xD0 */ f32 mVisibleMinY;
    /* 0xD4 */ f32 mVisibleMaxX;
    /* 0xD8 */ f32 mVisibleMaxY;
};

// The object at offset 0x4 of *lbl_8053AAF8 (type unknown).
struct GameRoot_0x4 {
    /* 0x0 */ u8 field_0x0[0x8];
    /* 0x8 */ ActorManager* mActorManager; // fn_801F856C; allocated in
                                           // game/tu_80139BE8.cpp
    /* 0xC */ GameView* mView;             // fn_802D6A9C
};

// *lbl_8053AAF8: the most-used game singleton (309 of 529 units read it).
// Set in game/tu_801344C0.cpp (0x80134998) and game/tu_80134C04.cpp
// (0x80139340). The struct's real size is unknown.
struct GameRoot {
    /* 0x00 */ u8 field_0x0[0x4];
    /* 0x04 */ GameRoot_0x4* field_0x4; // -> 0x8: the actor manager
    /* 0x08 */ u8 field_0x8[0x7E - 0x8];
    /* 0x7E */ u8 field_0x7e;
    /* 0x7F */ u8 field_0x7f;
    /* 0x80 */ u8 field_0x80[0x2];
    /* 0x82 */ u8 field_0x82; // bit i set: player i takes part (fn_801F6068)
    /* 0x83 */ u8 field_0x83;
    /* 0x84 */ u8 field_0x84[0xA4 - 0x84];
    /* 0xA4 */ void* field_0xa4; // has a vtable; slot 0x18 looks a resource
                                 // up by type 'ENM ' and name (fn_801F62AC)
};

extern GameRoot* lbl_8053AAF8;

// lbl_8053AB10->0x28 (type unknown), returned by fn_8013C824.
struct EffectSystem {
    /* 0x0 */ u8 field_0x0[0xC];
    /* 0xC */ JPAEmitterManager* mEmitterManager;
};

// *lbl_8053AB10: the second most-used singleton (267 units). Set in
// game/tu_8013AAD4.cpp (0x8013C024) and game/tu_80134C04.cpp (0x801393C4).
struct GameRoot2 {
    /* 0x00 */ u8 field_0x0[0x28];
    /* 0x28 */ EffectSystem* field_0x28;
};

extern GameRoot2* lbl_8053AB10;

// Out-of-line getters (extern "C" keeps the dtk names as link names until
// their units name them).
extern "C" {
GameView* fn_802D6A9C(void);     // lbl_8053AAF8->0x4->0xC
EffectSystem* fn_8013C824(void); // lbl_8053AB10->0x28
}

#endif
