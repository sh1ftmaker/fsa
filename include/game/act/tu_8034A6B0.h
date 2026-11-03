#ifndef GAME_ACT_TU_8034A6B0_H
#define GAME_ACT_TU_8034A6B0_H

// Code: game/tu_8034A6B0.cpp.

#include "game/ActorBase.h"
#include "game/GameGlobals.h"

// Named after its vtable, 0x804AC868.
class cls_804AC868 : public ActorBase_804A5770 {
public:
    cls_804AC868() {}
    /* 0x08 */ virtual ~cls_804AC868();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // countdown, set to 2
    /* 0x234 */ s32 field_0x234; // phase 0 or 1; sprite frame is 0x2DD + this
}; // Size: 0x238

#endif
