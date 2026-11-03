#ifndef GAME_ACT_ACT_LOCAL_SWITCH_H
#define GAME_ACT_ACT_LOCAL_SWITCH_H

// Code: game/act_local_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804AA008).
class LocalSwitch_804AA008 : public ActorBase_804A5770 {
public:
    LocalSwitch_804AA008() {}
    virtual ~LocalSwitch_804AA008();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
}; // Size: 0x230

#endif
