#ifndef GAME_ACT_ACT_SHOVEL_OLD_TIMER_H
#define GAME_ACT_ACT_SHOVEL_OLD_TIMER_H

// Code: game/act_shovel_old_timer.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AAB40.
class cls_804AAB40 : public ActorBase_804A5770 {
public:
    cls_804AAB40() {}
    /* 0x08 */ virtual ~cls_804AAB40();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x2C]; // not yet known
}; // Size: 0x25C

#endif
