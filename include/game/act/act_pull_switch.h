#ifndef GAME_ACT_ACT_PULL_SWITCH_H
#define GAME_ACT_ACT_PULL_SWITCH_H

// Code: game/act_pull_switch.cpp.

#include "game/ActorBase.h"

// The PLSW actor ("Pull switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202480, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A7908.
class PullSwitch_804A7908 : public ActorBase_804A5770 {
public:
    PullSwitch_804A7908() {}
    /* 0x08 */ virtual ~PullSwitch_804A7908();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ JGeometry::TVec3f field_0x230; // mPos with y + 16 when a player pushes in
    /* 0x23C */ s32 field_0x23c;               // sprite frame offset (0, 1, 2 or 4), from field_0x244
    /* 0x240 */ u8 field_0x240;                // set by init from vfunc_0x34; 1 once fully pulled
    /* 0x244 */ f32 field_0x244;               // pull height above mSpawnPos.y (0..8)
    /* 0x248 */ s32 field_0x248;               // frames left of the player lock
    /* 0x24C */ s32 field_0x24c;               // locked player, -1 for none
}; // Size: 0x250

#endif
