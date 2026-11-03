#ifndef GAME_ACT_ACT_LIFTABLE_ITEM_H
#define GAME_ACT_ACT_LIFTABLE_ITEM_H

// Code: game/act_liftable_item.cpp.

#include "game/ActorBase.h"

// The IMUP actor ("Liftable item", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203BB4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A01D0.
class LiftableItem_804A01D0 : public ActorBase_804A5770 {
public:
    LiftableItem_804A01D0() {}
    /* 0x08 */ virtual ~LiftableItem_804A01D0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ u8 field_0x230[0x78]; // not yet known
    /* 0x2A8 */ JPABaseEmitter* mEmitter2a8; // released by the destructor
    /* 0x2AC */ JPABaseEmitter* mEmitter2ac; // released by the destructor
    /* 0x2B0 */ JPABaseEmitter* mEmitter2b0; // released by the destructor
}; // Size: 0x2B4

#endif
