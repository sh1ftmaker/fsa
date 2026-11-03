#ifndef GAME_ACT_ACT_DROPPED_ITEM_H
#define GAME_ACT_ACT_DROPPED_ITEM_H

// Code: game/act_dropped_item.cpp.

#include "game/ActorBase.h"

// The LSTI actor ("Dropped item", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202B44, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8C80.
class DroppedItem_804A8C80 : public ActorBase_804A5770 {
public:
    DroppedItem_804A8C80() {}
    /* 0x08 */ virtual ~DroppedItem_804A8C80();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x40 */ virtual void vfunc_0x40();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x44]; // not yet known
}; // Size: 0x274

#endif
