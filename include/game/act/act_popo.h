#ifndef GAME_ACT_ACT_POPO_H
#define GAME_ACT_ACT_POPO_H

// Code: game/act_popo.cpp.

#include "game/ActorBase.h"

// The POPO actor ("Popo", docs/names/actors.tsv, high confidence): the spawner
// constructs it with fn_80203234, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0868.
class Popo_804A0868 : public ActorBase_804A5770 {
public:
    Popo_804A0868() {}
    /* 0x08 */ virtual ~Popo_804A0868();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Fields from init/update/draw (game/act_popo.cpp).
    /* 0x230 */ s32 field_0x230; // init 0; update skips the random-drift draw while nonzero
    /* 0x234 */ s32 mTimers[3];
    /* 0x240 */ s32 field_0x240; // init random 0..1; flips between drift and rest
    /* 0x244 */ s32 field_0x244; // init 0, not read
    /* 0x248 */ s32 field_0x248; // init 0, not read
    /* 0x24C */ s32 field_0x24c; // frame 0..3 (draw adds 0x99)
    /* 0x250 */ f32 field_0x250; // target velocity x
    /* 0x254 */ f32 field_0x254; // target velocity y
    /* 0x258 */ f32 field_0x258; // zeroed with the above, not read
}; // Size: 0x25C

#endif
