#ifndef GAME_ACT_TU_802E6EC4_H
#define GAME_ACT_TU_802E6EC4_H

// Code: game/tu_802E6EC4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9E50.
class cls_804A9E50 : public ActorBase_804A5770 {
public:
    cls_804A9E50() {}
    /* 0x08 */ virtual ~cls_804A9E50();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mMode;   // field_0x90 & 0x7F (init)
    /* 0x234 */ s32 mFlag;   // (field_0x90 >> 7) & 0x1F (init); draw tests it != 0
    /* 0x238 */ s32 mStep;   // sprite id offset (draw adds 0x26D)
    /* 0x23C */ s32 mCount;  // update: ends the actor at 8
    /* 0x240 */ s32 mTimer;  // counts down from 1 or 2 (init)
}; // Size: 0x244

#endif
