#ifndef GAME_ACT_ACT_INVISIBLE_NPC_WALL_TOMI_H
#define GAME_ACT_ACT_INVISIBLE_NPC_WALL_TOMI_H

// Code: game/act_invisible_npc_wall_tomi.cpp.

#include "game/ActorBase.h"

// The TOMI actor ("Invisible NPC wall", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80200584, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AE6D0.
class InvisibleNPCWall_804AE6D0 : public ActorBase_804A5770 {
public:
    InvisibleNPCWall_804AE6D0() {}
    /* 0x08 */ virtual ~InvisibleNPCWall_804AE6D0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ u8 field_0x230[0x8]; // not yet known
}; // Size: 0x238

#endif
