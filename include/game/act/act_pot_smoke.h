#ifndef GAME_ACT_ACT_POT_SMOKE_H
#define GAME_ACT_ACT_POT_SMOKE_H

// Code: game/act_pot_smoke.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8418.
class cls_804A8418 : public ActorBase_804A5770 {
public:
    cls_804A8418() {}
    /* 0x08 */ virtual ~cls_804A8418();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mTimer;  // frames until the next sprite frame
    /* 0x234 */ s32 mFrame;  // sprite frame offset
    /* 0x238 */ s32 mKind;   // field_0x90 & 0x3F: 1..6 select the sprite set
    /* 0x23C */ s32 mFlag;   // (field_0x90 >> 7) & 0x1F, passed on as a bool
    /* 0x240 */ s32 mCount;  // completed loops (kind 1)
}; // Size: 0x244

#endif
