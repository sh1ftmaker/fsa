#ifndef GAME_ACT_TU_802E9428_H
#define GAME_ACT_TU_802E9428_H

// Code: game/tu_802E9428.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AA0FC.
class UnknownENDM_804AA0FC : public ActorBase_804A5770 {
public:
    UnknownENDM_804AA0FC() {}
    /* 0x08 */ virtual ~UnknownENDM_804AA0FC();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x6C]; // not yet known
}; // Size: 0x29C

#endif
