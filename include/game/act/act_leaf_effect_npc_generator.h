#ifndef GAME_ACT_ACT_LEAF_EFFECT_NPC_GENERATOR_H
#define GAME_ACT_ACT_LEAF_EFFECT_NPC_GENERATOR_H

// Code: game/act_leaf_effect_npc_generator.cpp.

#include "game/ActorBase.h"
#include "JSystem/JGeometry.h"

// Named after its vtable, 0x804AE288.
class LeafEffectNPCGenerator_804AE288 : public ActorBase_804A5770 {
public:
    LeafEffectNPCGenerator_804AE288() {}
    /* 0x08 */ virtual ~LeafEffectNPCGenerator_804AE288();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x8036D858; the only caller is tu_802233A4 (0x80223CD0), which passes
    // the 'LFGN' actor in r3 (unused here) and a position. Flags the first
    // live 'LEAF' actor within 57 units of pos (and 42 in x) by setting its
    // byte at 0x230.
    void fn_8036D858(JGeometry::TVec3f pos);
}; // Size: 0x230

#endif
