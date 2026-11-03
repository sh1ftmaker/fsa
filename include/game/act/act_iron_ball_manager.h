#ifndef GAME_ACT_ACT_IRON_BALL_MANAGER_H
#define GAME_ACT_ACT_IRON_BALL_MANAGER_H

// Code: game/act_iron_ball_manager.cpp.

#include "game/ActorBase.h"

// The TKYU actor ("Iron ball manager", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80203AC4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804ABA38.
class IronBallManager_804ABA38 : public ActorBase_804A5770 {
public:
    IronBallManager_804ABA38() {}
    /* 0x08 */ virtual ~IronBallManager_804ABA38();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ u8 field_0x230[0x1C]; // not yet known
}; // Size: 0x24C

#endif
