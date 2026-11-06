#ifndef GAME_ACT_ACT_RAINBOW_BRIDGE_H
#define GAME_ACT_ACT_RAINBOW_BRIDGE_H

// Code: game/act_rainbow_bridge.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// Spawned for 'RNBW' ("Rainbow Bridge", docs/names/actors.tsv, high confidence).
// Named after its vtable, 0x8049E610.
class cls_8049E610 : public ActorBase_804A5770 {
public:
    cls_8049E610();
    /* 0x08 */ virtual ~cls_8049E610();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();

    void checkActivate();
    bool isPlayerOn(s32 player);

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ JPABaseEmitter* field_0x23c[5];
}; // Size: 0x250

#endif
