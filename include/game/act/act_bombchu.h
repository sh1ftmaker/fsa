#ifndef GAME_ACT_ACT_BOMBCHU_H
#define GAME_ACT_ACT_BOMBCHU_H

// Code: game/act_bombchu.cpp.

#include "game/EnemyBase.h"

// The BMTY actor ("Bombchu", docs/names/actors.tsv, high confidence): its
// constructor stores this vtable (config/G4SE01/game_units.tsv). Named after
// its vtable, 0x804AB464.
class Bombchu_804AB464 : public EnemyBase_804A4808 {
public:
    /* 0x48 */ virtual void vfunc_0x48();
};

#endif
