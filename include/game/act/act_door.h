#ifndef GAME_ACT_ACT_DOOR_H
#define GAME_ACT_ACT_DOOR_H

// Code: game/act_door.cpp.

#include "game/ActorBase.h"

// The DOOR actor ("Door", docs/names/actors.tsv, high confidence): the spawner
// constructs it with fn_80202ACC, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8D88.
class Door_804A8D88 : public ActorBase_804A5770 {
public:
    Door_804A8D88() {}
    /* 0x08 */ virtual ~Door_804A8D88();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // 1 caller (tu_802957CC): called on a door the block moved. Body not yet written.
    void fn_802C59B0();
    /* 0x230 */ u8 field_0x230[0x4C]; // not yet known
}; // Size: 0x27C

#endif
