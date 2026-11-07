#ifndef GAME_ACT_ACT_OBJ_LIFT_H
#define GAME_ACT_ACT_OBJ_LIFT_H

// Code: game/act_obj_lift.cpp.

#include "game/ActorBase.h"

// The OBLF actor ("OBJ lift", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802007F8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804ACA00.
class OBJLift_804ACA00 : public ActorBase_804A5770 {
public:
    OBJLift_804ACA00() {}
    /* 0x08 */ virtual ~OBJLift_804ACA00();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // 0x803513C0: breaks the lift apart (effects and sound).
    void fn_803513C0();
    // 0x80351020: whether a player's hit point `player` (8 per player) lands inside the lift's box.
    bool fn_80351020(s32 player);
    // 0x8035001C: sets the push velocity for direction `dir`.
    void fn_8035001C(u32 dir);
    // 0x80350530 / 0x803506E0: which sides a player pushes (sets field_0x270); returns the direction or -1.
    s32 fn_80350530();
    s32 fn_803506E0();
    // 0x803502CC: moves the lift's box by the push and tests it against the map.
    bool fn_803502CC();
    // 0x803511F0: respawns the lift at its spawn position (with an effect when it was far away).
    void fn_803511F0();
    /* 0x230 */ s32 field_0x230; // act_small_key rides it while nonzero
    /* 0x234 */ u8 field_0x234[0xC]; // not yet known
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ u8 field_0x250[4]; // not yet known
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258; // state
    /* 0x25C */ u8 field_0x25c[4]; // not yet known
    /* 0x260 */ s32 field_0x260; // half the lift's size, in units
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268; // push direction (0-7), -1 for none
    /* 0x26C */ u8 field_0x26c[4]; // not yet known
    /* 0x270 */ s32 field_0x270; // bit mask of the sides a player pushes
    /* 0x274 */ s32 field_0x274; // pushing player
    /* 0x278 */ u8 field_0x278[4]; // not yet known
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ u8 field_0x280[2]; // not yet known
    /* 0x282 */ u8 field_0x282;
    /* 0x283 */ u8 field_0x283[9]; // not yet known
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ u8 field_0x294[0xC]; // not yet known
}; // Size: 0x2A0

#endif
