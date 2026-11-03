#ifndef GAME_ACT_ACT_OCTOROK_ROCK_H
#define GAME_ACT_ACT_OCTOROK_ROCK_H

// Code: game/act_octorok_rock.cpp.

#include "game/ActorBase.h"

// The OKTM actor ("Octorok rock", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203E9C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F5D8.
class OctorokRock_8049F5D8 : public ActorBase_804A5770 {
public:
    OctorokRock_8049F5D8() {}
    /* 0x08 */ virtual ~OctorokRock_8049F5D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x30]; // not yet known
}; // Size: 0x260

#endif
