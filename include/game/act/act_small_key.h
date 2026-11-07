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
    // 0x802C3AE4: the key lying on the floor (pick-up, pushes, pits; registers its place with
    // the view through fn_802D6D70).
    void fn_802C3AE4();
    // 0x802C1674 / 0x802C1778 (act_small_key's range, not written yet): no
    // arguments besides the key; Player_804A6F38::fn_80242560 calls them on
    // every 'KEY0' actor (fn_802C1778 also on the key a player holds).
    void fn_802C1674();
    // 0x802C2684: whether `other` is a lift ('LIFT', 'LIF2', 'CLOD', a running 'OBLF' or 'SSLF')
    // whose box (relative to the view) overlaps `box`.
    bool fn_802C2684(ActorBase_804A5770* other, JGeometry::TBox2f* box);
    // 0x802C2840: rides the lift the key stands on.
    void fn_802C2840();
    // 0x802C2154: whether a 'WARP' actor of state 4 on this layer overlaps the key's box (grown by 4); sets field_0x2b0.
    bool fn_802C2154();
    // 0x802C24A8: bounces the key off an overlapping 'STBL' actor (state 0 or 2).
    void fn_802C24A8();
    void fn_802C1778();
    // 0x802C2BD0 (act_small_key's range): takes the key in r3, sets byte
    // 0x2B0 and passes its (0x230, 0x234) position and 0x238 to GameView's
    // fn_802D6C10. Called by act_invisible_npc_wall_tomi (0x803764BC).
    void fn_802C2BD0();
    // 0x802C2B64: if the key's state (0x240) is 2, does what fn_802C2BD0
    // does: sets byte 0x2B0 and passes (0x230, 0x234) and 0x238 to GameView's
    // fn_802D6C10.
    void fn_802C2B64();
    // 0x802C1D90: passes the key's (0x230, 0x234) position and 0x238 layer to
    // GameView's fn_802D6C10 (view from getGameView_802D6A9C()), then clears mAlive.
    void fn_802C1D90();
    // 0x802C1DFC: at the key's own layer (field_0x4 < 8) plays effect 0x503
    // at mPos; from layer 8 up spawns a KMRI actor there instead. Then passes
    // the (0x230, 0x234) position and 0x238 layer to GameView's fn_802D6C10
    // and clears mAlive.
    void fn_802C1DFC();
    // 0x802C2300 (act_small_key's range): takes a player bitmask (bits 1-3
    // pick players 1-3, else 0). Stores the player's index in 0x2A4 and the
    // index of the player it is paired with (fn_8022F3DC, if fn_8022F514
    // holds) in 0x2A0. Returns false when the early checks or the view's
    // bit 0x168 (fn_802D46D8) refuse; otherwise returns true, calling
    // fn_8022FE80 when the conversation test (fn_802308E0 or fn_8023077C) holds.
    bool fn_802C2300(u8 playerBits);
    // 0x802C5284: passes its position (0x230) and layer (0x238) to GameView's
    // fn_802D6C10, with the view from getGameView_802D6A9C().
    void fn_802C5284();
    // 0x802C1D30: takes a position and a layer (the caller's player layer,
    // Player field_0x3b4). Copies the position to 0x260, the layer to 0x26C
    // and the view's current screen column/row (GameView field_0x14/0x18)
    // to 0x2AA. Called by tu_8023FBF0 with the key from findActorBySerial_801F666C.
    void fn_802C1D30(const JGeometry::TVec3f& pos, s32 layer);
    // 0x803A3220 (game/tu_803944C8.cpp, the message tag processor's unit):
    // returns field_0x280. fn_8039FE78 calls it on the key the player holds
    // ('KEY0', fn_8015445C of fn_8023EF50's serial) and compares it with 0, 1,
    // 2 and 9.
    s32 fn_803A3220();
    /* 0x230 */ JGeometry::TVec2f field_0x230; // position (x, y)
    /* 0x238 */ s32 field_0x238; // layer
    /* 0x23C */ u32 field_0x23c; // GameView_804A9950::fn_802D8328 stores the key value's top 2 bits (stw)
    /* 0x240 */ s32 field_0x240; // state; 4 is tested by act_invisible_npc_wall_tomi
    /* 0x244 */ u8 field_0x244[0x254 - 0x244]; // not yet known
    /* 0x254 */ JGeometry::TVec3f field_0x254; // position of the actor carrying the key
    /* 0x260 */ JGeometry::TVec3f field_0x260; // position, set by fn_802C1D30
    /* 0x26C */ s32 field_0x26c; // layer, set by fn_802C1D30
    /* 0x270 */ s32 field_0x270; // spawn parameter bits 22-26
    /* 0x274 */ s32 field_0x274; // bits 17-21
    /* 0x278 */ s32 field_0x278; // bits 12-16
    /* 0x27C */ s32 field_0x27c; // kind: 8 hides the key, 2 draws it as held
    /* 0x280 */ s32 field_0x280; // 6 is tested by act_magic_circle_generator fn_802E7DB8 (word compare)
    /* 0x284 */ s32 field_0x284;
    /* 0x288 */ s32 field_0x288; // countdown: while nonzero, vfunc_0x4C decrements it and returns 0
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294; // a counter; its thresholds pick which of the bits 2/4/8/0x10 hides the key
    /* 0x298 */ s32 field_0x298; // slot of the actor carrying the key, -1 for none
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ s32 field_0x2a0; // player's index (0..3, u8), set by 0x802C2300
    /* 0x2A4 */ s32 field_0x2a4; // player's index (0..3, u8), set by 0x802C2300
    /* 0x2A8 */ u16 field_0x2a8; // 0xFFFF when nothing is registered with the view
    /* 0x2AA */ u16 field_0x2aa; // screen column in the high byte, row in the low
    /* 0x2AC */ s8 field_0x2ac;
    /* 0x2AD */ u8 field_0x2ad; // alpha of the draw colour
    /* 0x2AE */ u8 field_0x2ae; // not yet known
    /* 0x2AF */ u8 field_0x2af; // set: fn_802C1674 registers the key with the view
    /* 0x2B0 */ bool field_0x2b0; // set to true by fn_802C2BD0
    /* 0x2B1 */ u8 field_0x2b1;
    /* 0x2B2 */ u8 field_0x2b2;
    /* 0x2B3 */ u8 field_0x2b3;
    /* 0x2B4 */ u8 field_0x2b4; // draw only while set
    /* 0x2B5 */ u8 field_0x2b5;
    /* 0x2B6 */ u8 field_0x2b6[2]; // not yet known
}; // Size: 0x2B8

#endif
