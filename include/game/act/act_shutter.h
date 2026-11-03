#ifndef GAME_ACT_ACT_SHUTTER_H
#define GAME_ACT_ACT_SHUTTER_H

// Code: game/act_shutter.cpp.

#include "game/ActorBase.h"

// The SHTR actor ("Shutter", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203998, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8118.
class Shutter_804A8118 : public ActorBase_804A5770 {
public:
    Shutter_804A8118() {}
    /* 0x08 */ virtual ~Shutter_804A8118();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;   // 0..5: see update()
    /* 0x234 */ s32 mSwitch;  // (field_0x90 >> 22) & 0x1F: GameView bit, set once the shutter is open
    /* 0x238 */ u8 field_0x238[4]; // not yet known
    /* 0x23C */ s32 mFlag;    // (field_0x90 >> 17) & 0x1F; nonzero hides the actor instead of removing it
    /* 0x240 */ s32 mTimer;   // frames to the next state
    /* 0x244 */ u16 mTile;    // tile code of the cell at mPos before the shutter closes it
}; // Size: 0x248

#endif
