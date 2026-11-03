#ifndef GAME_ACT_ACT_ROCK_H
#define GAME_ACT_ACT_ROCK_H

// Code: game/act_rock.cpp.

#include "game/ActorBase.h"

// The ROCK actor ("Rock", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_8020407C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9480.
class Rock_804A9480 : public ActorBase_804A5770 {
public:
    Rock_804A9480() {}
    /* 0x08 */ virtual ~Rock_804A9480();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x8]; // not yet known
}; // Size: 0x238

#endif
