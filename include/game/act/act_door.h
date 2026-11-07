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
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234; // type, spawn parameter bits 0-6
    /* 0x238 */ s32 field_0x238; // spawn parameter bits 7-11
    /* 0x23C */ s32 field_0x23c; // spawn parameter bits 12-16
    /* 0x240 */ s32 field_0x240; // spawn parameter bit 17
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248; // -1 at init
    /* 0x24C */ s32 field_0x24c; // spawn parameter bit 18
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254; // frame countdown of states 10-14 (update)
    // SHARED-EDIT: was three f32s; update copies it as one TVec3f (psq) and passes its address.
    /* 0x258 */ JGeometry::TVec3f field_0x258; // spawn position
    /* 0x264 */ JGeometry::TVec2f field_0x264;
    /* 0x26C */ JGeometry::TBox2f field_0x26c;
}; // Size: 0x27C

#endif
