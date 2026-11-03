#ifndef GAME_ACT_TU_8033F448_H
#define GAME_ACT_TU_8033F448_H

// Code: game/tu_8033F448.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC388.
class cls_804AC388 : public ActorBase_804A5770 {
public:
    cls_804AC388() {}
    /* 0x08 */ virtual ~cls_804AC388();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // countdown from 300 in init()
    /* 0x234 */ s32 field_0x234; // (field_0x90 >> 4) & 0xF; first argument of GameView::fn_802D9FF4
    /* 0x238 */ s32 field_0x238; // (field_0x90 >> 8) & 0xF; second argument
    /* 0x23C */ bool field_0x23c; // set once a player is in the state the update loop looks for
}; // Size: 0x240

#endif
