#ifndef GAME_ACT_ACT_TINGLE_TIME_UP_MANAGER_H
#define GAME_ACT_ACT_TINGLE_TIME_UP_MANAGER_H

// Code: game/act_tingle_time_up_manager.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804ACBF0.
class cls_804ACBF0 : public ActorBase_804A5770 {
public:
    cls_804ACBF0() {}
    /* 0x08 */ virtual ~cls_804ACBF0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x64]; // not yet known
}; // Size: 0x294

#endif
