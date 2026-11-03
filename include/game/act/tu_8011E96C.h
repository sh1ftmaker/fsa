#ifndef GAME_ACT_TU_8011E96C_H
#define GAME_ACT_TU_8011E96C_H

// Code: game/tu_8011E96C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049DE90.
class cls_8049DE90 : public ActorBase_804A5770 {
public:
    cls_8049DE90() {}
    /* 0x08 */ virtual ~cls_8049DE90();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ u8 field_0x230[0x24]; // not yet known
}; // Size: 0x254

#endif
