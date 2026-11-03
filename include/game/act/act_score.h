#ifndef GAME_ACT_ACT_SCORE_H
#define GAME_ACT_ACT_SCORE_H

// Code: game/act_score.cpp.

#include "game/ActorBase.h"

// The SCRE actor ("Score", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_8020346C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AF4C0.
class Score_804AF4C0 : public ActorBase_804A5770 {
public:
    Score_804AF4C0() {}
    /* 0x08 */ virtual ~Score_804AF4C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x20]; // not yet known
}; // Size: 0x250

#endif
