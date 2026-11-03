#ifndef GAME_ACT_ACT_ENEMY_COUNT_WIPE_OUT_CHECK_H
#define GAME_ACT_ACT_ENEMY_COUNT_WIPE_OUT_CHECK_H

// Code: game/act_enemy_count_wipe_out_check.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8250.
class cls_804A8250 : public ActorBase_804A5770 {
public:
    cls_804A8250() {}
    /* 0x08 */ virtual ~cls_804A8250();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
