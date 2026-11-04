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
    // Mode 0's drawing: four pictures of the stack, each turned by one of
    // field_0x264 at field_0x238[i] (called by draw).
    void fn_80103348();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 mMode; // field_0x90 & 0xF; init() branches on it
    /* 0x238 */ JGeometry::TVec3f field_0x238[3]; // copies of mPos (init)
    /* 0x25C */ s32 field_0x25c; // (field_0x90 >> 22) & 0x1F
    /* 0x260 */ JPABaseEmitter* mEmitter260; // released by the destructor
    /* 0x264 */ s32 field_0x264[3];
    /* 0x270 */ u8 field_0x270[0x18]; // not yet known
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ JGeometry::TVec3f field_0x28c;
    /* 0x298 */ u8 field_0x298[0x1C]; // not yet known
}; // Size: 0x2B4

#endif
