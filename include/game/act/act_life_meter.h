#ifndef GAME_ACT_ACT_LIFE_METER_H
#define GAME_ACT_ACT_LIFE_METER_H

// Code: game/act_life_meter.cpp.

#include "game/ActorBase.h"

// The LIMT actor ("Life meter", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_8020185C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804B174C.
class LifeMeter_804B174C : public ActorBase_804A5770 {
public:
    LifeMeter_804B174C() {}
    /* 0x08 */ virtual ~LifeMeter_804B174C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x44]; // not yet known
}; // Size: 0x274

#endif
