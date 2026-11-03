#ifndef GAME_ACT_ACT_PLAYER_START_POSITION_H
#define GAME_ACT_ACT_PLAYER_START_POSITION_H

// Code: game/act_player_start_position.cpp.

#include "game/ActorBase.h"

// The PLAS actor ("Player start position", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_8020364C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8338.
class PlayerStartPosition_804A8338 : public ActorBase_804A5770 {
public:
    PlayerStartPosition_804A8338() {}
    /* 0x08 */ virtual ~PlayerStartPosition_804A8338();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x4]; // not yet known
    /* 0x234 */ s32 field_0x234; // init: (field_0x90 >> 8) & 0xF; 0 when GameView field_0x131f6 == 0 or field_0x131f9 == 2
}; // Size: 0x238

#endif
