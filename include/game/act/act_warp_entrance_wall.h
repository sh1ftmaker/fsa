#ifndef GAME_ACT_ACT_WARP_ENTRANCE_WALL_H
#define GAME_ACT_ACT_WARP_ENTRANCE_WALL_H

// Code: game/act_warp_entrance_wall.cpp.

#include "game/ActorBase.h"

// The WPWL actor ("Warp entrance wall", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_801FF834, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AE948.
class WarpEntranceWall_804AE948 : public ActorBase_804A5770 {
public:
    WarpEntranceWall_804AE948() {}
    /* 0x08 */ virtual ~WarpEntranceWall_804AE948();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    // Whether another wall-like actor (a moving house or a log) overlaps
    // this wall's box, skipping the slot whose index equals mSerial
    // (0x803798D0).
    bool fn_803798D0();
    /* 0x230 */ s32 field_0x230; // field_0x90 bits 22-26: shape (0 or 1)
    /* 0x234 */ s32 field_0x234; // field_0x90 >> 27
}; // Size: 0x238

#endif
