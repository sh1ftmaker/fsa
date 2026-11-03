#ifndef GAME_ACT_ACT_FOUR_SWITCH_MONITOR_H
#define GAME_ACT_ACT_FOUR_SWITCH_MONITOR_H

// Code: game/act_four_switch_monitor.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A80A0.
class cls_804A80A0 : public ActorBase_804A5770 {
public:
    cls_804A80A0() {}
    /* 0x08 */ virtual ~cls_804A80A0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // update's state (jumptable_804A80F4, 0-7); init sets 0 or 7
    /* 0x234 */ s32 field_0x234; // field_0x90 & 0x1F, set in init()
    /* 0x238 */ s32 field_0x238; // counter stepped in update's state 3
    /* 0x23C */ s32 field_0x23c; // timer counted down in update
    /* 0x240 */ s32 field_0x240; // the timer's end value in state 4
    /* 0x244 */ s32 field_0x244[4]; // (field_0x90 >> lbl_8046C850[i]) & 0x1F, set in init()
}; // Size: 0x254

#endif
