#ifndef GAME_ACT_ACT_SPAWNER_H
#define GAME_ACT_ACT_SPAWNER_H

// Code: game/act_spawner.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9328.
class cls_804A9328 : public ActorBase_804A5770 {
public:
    cls_804A9328() {}
    /* 0x08 */ virtual ~cls_804A9328();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x448]; // not yet known
}; // Size: 0x678

#endif
