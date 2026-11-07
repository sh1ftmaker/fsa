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
    // 1 caller (tu_802957CC): called on a door the block moved; adds the push vector's x, y.
    void fn_802C59B0(const JGeometry::TVec3f* delta);
    /* 0x230 */ u8 field_0x230[0x28]; // not yet known
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ u8 field_0x260[0x1C]; // not yet known
}; // Size: 0x27C

#endif
