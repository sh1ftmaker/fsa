#ifndef GAME_ACT_ACT_CHIEF_SOLDIER_H
#define GAME_ACT_ACT_CHIEF_SOLDIER_H

// Code: game/act_chief_soldier.cpp.

#include "game/ActorBase.h"

// The Chief Soldier actor ("SLR4", docs/names/actors.tsv): the spawner
// constructs it with fn_80203090, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0B90.
class ChiefSoldier_804A0B90 : public ActorBase_804A5770 {
public:
    ChiefSoldier_804A0B90() {}
    // Destructor (game/act_chief_soldier.cpp): clears mAlive on the actor
    // whose serial is field_0x238, unless the state or the game root says
    // not to, then runs the base destructor and frees the object if the
    // delete flag is set.
    /* 0x08 */ virtual ~ChiefSoldier_804A0B90();
    /* 0x24 */ virtual void vfunc_0x24();

    /* 0x230 */ u8 field_0x230[0x238 - 0x230]; // not read by the destructor
    /* 0x238 */ s32 field_0x238; // serial of the actor the destructor clears
};

#endif
