#ifndef GAME_ACT_ACT_TOWER_COLLAPSE_EFFECT_H
#define GAME_ACT_ACT_TOWER_COLLAPSE_EFFECT_H

// Code: game/act_tower_collapse_effect.cpp.

#include "game/ActorBase.h"

// The TWEV actor ("Tower collapse effect", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_802002D4, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049CA40.
class TowerCollapseEffect_8049CA40 : public ActorBase_804A5770 {
public:
    TowerCollapseEffect_8049CA40() {}
    /* 0x08 */ virtual ~TowerCollapseEffect_8049CA40();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x30]; // not yet known
    /* 0x260 */ JPABaseEmitter* mEmitter260; // released by the destructor
    /* 0x264 */ u8 field_0x264[0x50]; // not yet known
}; // Size: 0x2B4

#endif
