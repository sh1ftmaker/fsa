#ifndef GAME_ACT_ACT_TREASURE_MARK_H
#define GAME_ACT_ACT_TREASURE_MARK_H

// Code: game/act_treasure_mark.cpp.

#include "game/ActorBase.h"

// The TKRM actor ("Treasure mark", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802020B4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804ABAE8.
class TreasureMark_804ABAE8 : public ActorBase_804A5770 {
public:
    TreasureMark_804ABAE8() {}
    /* 0x08 */ virtual ~TreasureMark_804ABAE8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mBlinkTimer; // counts down from 10 (update)
    /* 0x234 */ s32 mFrame;      // 0 or 1, added to sprite frame 431
}; // Size: 0x238

#endif
