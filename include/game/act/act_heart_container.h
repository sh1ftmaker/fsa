#ifndef GAME_ACT_ACT_HEART_CONTAINER_H
#define GAME_ACT_ACT_HEART_CONTAINER_H

// Code: game/act_heart_container.cpp.

#include "game/ActorBase.h"

// The BHRT actor ("Heart Container", docs/names/actors.tsv, high confidence):
// the spawner constructs it with fn_80203DEC, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A7E58.
class HeartContainer_804A7E58 : public ActorBase_804A5770 {
public:
    HeartContainer_804A7E58() {}
    /* 0x08 */ virtual ~HeartContainer_804A7E58();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0xC]; // not yet known
}; // Size: 0x23C

#endif
