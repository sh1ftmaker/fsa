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
    /* 0x230 */ s32 field_0x230; // act_small_key rides it while nonzero
    /* 0x234 */ u8 field_0x234[0x6C]; // not yet known
}; // Size: 0x2A0

#endif
