#ifndef GAME_ACT_ACT_DIGGING_GAME_H
#define GAME_ACT_ACT_DIGGING_GAME_H

// Code: game/act_digging_game.cpp.

#include "game/ActorBase.h"

// The ANHO actor ("Digging Game Keeper", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_8020148C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049C970.
class DiggingGameKeeper_8049C970 : public ActorBase_804A5770 {
public:
    DiggingGameKeeper_8049C970() {}
    /* 0x08 */ virtual ~DiggingGameKeeper_8049C970();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x54]; // not yet known
}; // Size: 0x284

#endif
