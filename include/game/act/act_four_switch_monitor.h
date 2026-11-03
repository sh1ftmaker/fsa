#ifndef GAME_ACT_ACT_FOUR_SWITCH_MONITOR_H
#define GAME_ACT_ACT_FOUR_SWITCH_MONITOR_H

// Code: game/act_four_switch_monitor.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A80A0.
class cls_804A80A0 : public ActorBase_804A5770 {
public:
    cls_804A80A0() {}
    /* 0x08 */ virtual ~cls_804A80A0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x24]; // not yet known
}; // Size: 0x254

#endif
