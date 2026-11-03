#ifndef GAME_ACT_ACT_ENDING_MAP_SKIPPER_H
#define GAME_ACT_ACT_ENDING_MAP_SKIPPER_H

// Code: game/act_ending_map_skipper.cpp.

#include "game/ActorBase.h"

// The EMSK actor ("Ending map skipper", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_801FFFE8, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA498.
class EndingMapSkipper_804AA498 : public ActorBase_804A5770 {
public:
    EndingMapSkipper_804AA498() {}
    /* 0x08 */ virtual ~EndingMapSkipper_804AA498();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mIndex;  // low 7 bits of the spawn argument; picks a table row
    /* 0x234 */ s32 mTimer;  // frames left (500 after init)
}; // Size: 0x238

#endif
