#ifndef GAME_ACT_ACT_ON_OFF_SWITCH_H
#define GAME_ACT_ACT_ON_OFF_SWITCH_H

// Code: game/act_on_off_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8FC8.
class cls_804A8FC8 : public ActorBase_804A5770 {
public:
    cls_804A8FC8() {}
    /* 0x08 */ virtual ~cls_804A8FC8();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor
    /* 0x234 */ JPABaseEmitter* mEmitter234; // released by the destructor
    /* 0x238 */ u8 field_0x238[0x24]; // not yet known
}; // Size: 0x25C

#endif
