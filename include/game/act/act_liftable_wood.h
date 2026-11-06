#ifndef GAME_ACT_ACT_LIFTABLE_WOOD_H
#define GAME_ACT_ACT_LIFTABLE_WOOD_H

// Code: game/act_liftable_wood.cpp.

#include "game/ActorBase.h"

// The WOOD actor ("Liftable wood", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203270, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A87A8.
class LiftableWood_804A87A8 : public ActorBase_804A5770 {
public:
    LiftableWood_804A87A8() {}
    /* 0x08 */ virtual ~LiftableWood_804A87A8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor
    /* 0x234 */ s32 mState;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 mTimer23c;
    /* 0x240 */ s32 mTimer240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ const char* field_0x248;
}; // Size: 0x24C

#endif
