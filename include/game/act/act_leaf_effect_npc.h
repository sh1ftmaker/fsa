#ifndef GAME_ACT_ACT_LEAF_EFFECT_NPC_H
#define GAME_ACT_ACT_LEAF_EFFECT_NPC_H

// Code: game/act_leaf_effect_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE2E0.
class LeafEffectNPC_804AE2E0 : public ActorBase_804A5770 {
public:
    LeafEffectNPC_804AE2E0() {}
    /* 0x08 */ virtual ~LeafEffectNPC_804AE2E0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Spawns the leaf emitter (spawnEmitter_8013CC50) at mPos and starts the sound.
    void spawnLeaves();
    /* 0x230 */ u8 field_0x230; // set by the generator (fn_8036D858): burst again
    /* 0x234 */ s32 field_0x234; // kind 0-3: index of the emitter id table
    /* 0x238 */ const char* field_0x238; // stage name (GameView fn_802D9C4C)
    /* 0x23C */ u32 field_0x23c; // sound id, -1 for none
    /* 0x240 */ u16 field_0x240; // tile code of the leaf's tile
    /* 0x244 */ JGeometry::TVec3f field_0x244; // mPos before init() shifted it
    /* 0x250 */ u8 field_0x250; // tile changed
}; // Size: 0x254

#endif
