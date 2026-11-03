#ifndef GAME_ACT_ACT_STRANGE_BLOCK_H
#define GAME_ACT_ACT_STRANGE_BLOCK_H

// Code: game/act_strange_block.cpp.

#include "game/ActorBase.h"

// The STBL actor ("Strange block", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80201C68, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC2D8.
class StrangeBlock_804AC2D8 : public ActorBase_804A5770 {
public:
    StrangeBlock_804AC2D8() {}
    /* 0x08 */ virtual ~StrangeBlock_804AC2D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x44]; // not yet known
}; // Size: 0x274

#endif
