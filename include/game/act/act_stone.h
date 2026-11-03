#ifndef GAME_ACT_ACT_STONE_H
#define GAME_ACT_ACT_STONE_H

// Code: game/act_stone.cpp.

#include "game/ActorBase.h"

// The STON actor ("Stone", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80204130, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A93D0.
class Stone_804A93D0 : public ActorBase_804A5770 {
public:
    Stone_804A93D0() {}
    /* 0x08 */ virtual ~Stone_804A93D0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x48 */ virtual void vfunc_0x48();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230; // state word handed to ActorBase::fn_801F132C by vfunc_0x4C; 0 in init
    /* 0x234 */ u8 field_0x234;  // 0xFF unless carried by a player (update): the player's fn_8023BCD4 value; draw uses it as the sprite alpha
    /* 0x235 */ bool field_0x235; // set by update once the stone has been reset to the ground (layer flag set)
    /* 0x236 */ u8 field_0x236[2]; // padding
}; // Size: 0x238

#endif
