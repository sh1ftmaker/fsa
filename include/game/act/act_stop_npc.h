#ifndef GAME_ACT_ACT_STOP_NPC_H
#define GAME_ACT_ACT_STOP_NPC_H

// Code: game/act_stop_npc.cpp.

#include "game/ActorBase.h"

// The STPN actor ("Stop NPC", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80200548 (allocating 0x234 bytes), which
// stores this vtable (config/G4SE01/game_units.tsv). Named after its vtable,
// 0x804AA2D8.
class StopNPC_804AA2D8 : public ActorBase_804A5770 {
public:
    StopNPC_804AA2D8() {}
    /* 0x08 */ virtual ~StopNPC_804AA2D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update() {}
    /* 0x20 */ virtual void draw() {}

    // Undoes init(): called by the destructor (inlined) and by
    // act_liftable_item on the actor it finds by serial.
    void fn_802ED140();

    /* 0x230 */ s32 field_0x230; // 100 when init() starts the stop
}; // Size: 0x234

#endif
