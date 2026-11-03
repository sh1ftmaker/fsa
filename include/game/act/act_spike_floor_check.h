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
    /* 0x230 */ s32 mState;  // 0..3: wait, open, wait, close
    /* 0x234 */ s32 mType;    // field_0x90 & 0xFF; 0 opens on vfunc_0x34(), else on field_0x305c
    /* 0x238 */ s32 mTimer;   // frames to the next state
}; // Size: 0x23C

#endif
