#ifndef GAME_ACT_ACT_SMALL_KEY_H
#define GAME_ACT_ACT_SMALL_KEY_H

// Code: game/act_small_key.cpp.

#include "game/ActorBase.h"

// The KEY0 actor ("Small Key", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80202B08, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8CD8.
class SmallKey_804A8CD8 : public ActorBase_804A5770 {
public:
    SmallKey_804A8CD8() {}
    /* 0x08 */ virtual ~SmallKey_804A8CD8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // 0x802C1970 (act_small_key's range): the key's own check after the
    // collision list finds nothing (layer, pos); returns 0 or 1.
    bool fn_802C1970(s32 layer, const JGeometry::TVec3f* pos);
    // 0x802C2BD0 (act_small_key's range): takes the key in r3, sets byte
    // 0x2B0 and passes its (0x230, 0x234) position and 0x238 to GameView's
    // fn_802D6C10. Called by act_invisible_npc_wall_tomi (0x803764BC).
    void fn_802C2BD0();
    // 0x802C2B64: if the key's state (0x240) is 2, does what fn_802C2BD0
    // does: sets byte 0x2B0 and passes (0x230, 0x234) and 0x238 to GameView's
    // fn_802D6C10.
    void fn_802C2B64();
    // 0x802C1D90: passes the key's (0x230, 0x234) position and 0x238 layer to
    // GameView's fn_802D6C10 (view from fn_802D6A9C()), then clears mAlive.
    void fn_802C1D90();
    // 0x802C5284: passes its position (0x230) and layer (0x238) to GameView's
    // fn_802D6C10, with the view from fn_802D6A9C().
    void fn_802C5284();
    // 0x802C1D30: takes a position and a layer (the caller's player layer,
    // Player field_0x3b4). Copies the position to 0x260, the layer to 0x26C
    // and the view's current screen column/row (GameView field_0x14/0x18)
    // to 0x2AA. Called by tu_8023FBF0 with the key from fn_801F666C.
    void fn_802C1D30(const JGeometry::TVec3f& pos, s32 layer);
    /* 0x230 */ JGeometry::TVec2f field_0x230; // position (x, y)
    /* 0x238 */ s32 field_0x238; // layer
    /* 0x23C */ u8 field_0x23C[4]; // not yet known
    /* 0x240 */ s32 field_0x240; // state; 4 is tested by act_invisible_npc_wall_tomi
    /* 0x244 */ u8 field_0x244[0x260 - 0x244]; // not yet known
    /* 0x260 */ JGeometry::TVec3f field_0x260; // position, set by fn_802C1D30
    /* 0x26C */ s32 field_0x26c; // layer, set by fn_802C1D30
    /* 0x270 */ u8 field_0x270[0x288 - 0x270]; // not yet known
    /* 0x288 */ s32 field_0x288; // countdown: while nonzero, vfunc_0x4C decrements it and returns 0
    /* 0x28C */ u8 field_0x28c[0x2AA - 0x28C]; // not yet known
    /* 0x2AA */ u16 field_0x2aa; // screen column in the high byte, row in the low
    /* 0x2AC */ u8 field_0x2ac[0x2B0 - 0x2AC]; // not yet known
    /* 0x2B0 */ bool field_0x2b0; // set to true by fn_802C2BD0
    /* 0x2B1 */ u8 field_0x2b1[0x2B8 - 0x2B1]; // not yet known
}; // Size: 0x2B8

#endif
