#ifndef GAME_ACT_TU_802F4AEC_H
#define GAME_ACT_TU_802F4AEC_H

// Code: game/tu_802F4AEC.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AA800.
class cls_804AA800 : public ActorBase_804A5770 {
public:
    cls_804AA800() {}
    /* 0x08 */ virtual ~cls_804AA800();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state 0-3 (switch in update)
    /* 0x234 */ s32 field_0x234; // step counter, 0 to 0xD
    /* 0x238 */ s32 field_0x238; // frame countdown
}; // Size: 0x23C

#endif
