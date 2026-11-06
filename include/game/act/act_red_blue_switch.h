#ifndef GAME_ACT_ACT_RED_BLUE_SWITCH_H
#define GAME_ACT_ACT_RED_BLUE_SWITCH_H

// Code: game/act_red_blue_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9020.
class RedBlueSwitch_804A9020 : public ActorBase_804A5770 {
public:
    RedBlueSwitch_804A9020() {}
    /* 0x08 */ virtual ~RedBlueSwitch_804A9020();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor
    /* 0x234 */ JPABaseEmitter* mEmitter234; // released by the destructor
    /* 0x238 */ JPABaseEmitter* mEmitter238; // released by the destructor
    /* 0x23C */ JPABaseEmitter* mEmitter23c; // released by the destructor
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
}; // Size: 0x248

#endif
