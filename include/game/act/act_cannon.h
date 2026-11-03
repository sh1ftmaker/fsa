#ifndef GAME_ACT_ACT_CANNON_H
#define GAME_ACT_ACT_CANNON_H

// Code: game/act_cannon.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The CNON actor ("Cannon", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80108168, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049CCF0.
class Cannon_8049CCF0 : public ActorBase_804A5770 {
public:
    Cannon_8049CCF0() {}
    /* 0x08 */ virtual ~Cannon_8049CCF0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ SpriteAnim_801D267C mAnim;
    /* 0x268 */ u8 field_0x268[0x2C]; // not yet known
}; // Size: 0x294

#endif
