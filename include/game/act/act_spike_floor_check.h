#ifndef GAME_ACT_ACT_SPIKE_FLOOR_CHECK_H
#define GAME_ACT_ACT_SPIKE_FLOOR_CHECK_H

// Code: game/act_spike_floor_check.cpp.

#include "game/ActorBase.h"

// The THRN actor ("Spike floor check", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80203868, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8170.
class SpikeFloorCheck_804A8170 : public ActorBase_804A5770 {
public:
    SpikeFloorCheck_804A8170() {}
    /* 0x08 */ virtual ~SpikeFloorCheck_804A8170();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0xC]; // not yet known
}; // Size: 0x23C

#endif
