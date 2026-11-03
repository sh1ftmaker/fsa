#ifndef GAME_ACT_ACT_RED_BLUE_SWITCH_H
#define GAME_ACT_ACT_RED_BLUE_SWITCH_H

// Code: game/act_red_blue_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9020.
class cls_804A9020 : public ActorBase_804A5770 {
public:
    cls_804A9020() {}
    /* 0x08 */ virtual ~cls_804A9020();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor
    /* 0x234 */ JPABaseEmitter* mEmitter234; // released by the destructor
    /* 0x238 */ JPABaseEmitter* mEmitter238; // released by the destructor
    /* 0x23C */ JPABaseEmitter* mEmitter23c; // released by the destructor
    /* 0x240 */ u8 field_0x240[0x8]; // not yet known
}; // Size: 0x248

#endif
