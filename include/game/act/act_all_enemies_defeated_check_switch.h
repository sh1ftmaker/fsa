#ifndef GAME_ACT_ACT_ALL_ENEMIES_DEFEATED_CHECK_SWITCH_H
#define GAME_ACT_ACT_ALL_ENEMIES_DEFEATED_CHECK_SWITCH_H

// Code: game/act_all_enemies_defeated_check_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8AC8.
class cls_804A8AC8 : public ActorBase_804A5770 {
public:
    cls_804A8AC8() {}
    /* 0x08 */ virtual ~cls_804A8AC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x30]; // not yet known
}; // Size: 0x260

#endif
