#ifndef GAME_ACT_TU_8012EA00_H
#define GAME_ACT_TU_8012EA00_H

// Code: game/tu_8012EA00.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049E8F8.
class cls_8049E8F8 : public ActorBase_804A5770 {
public:
    cls_8049E8F8() : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(0) {}
    /* 0x08 */ virtual ~cls_8049E8F8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ s32 field_0x230; // state: 0 waiting for the players, 1 timed, 2 done
    /* 0x234 */ s32 field_0x234; // (field_0x90 >> 8) & 0xF; first argument of GameView::fn_802D9FF4
    /* 0x238 */ s32 field_0x238; // (field_0x90 >> 12) & 0xF; second argument of GameView::fn_802D9FF4
    /* 0x23C */ s32 field_0x23c; // frame counter of state 1 (ends at 0x78)

    // Switches to `state` and runs the entry action of states 1 and 2.
    void setState(s32 state);
    // Whether every participating player is active, alive and inside mBox.
    bool playersInside();
}; // Size: 0x240

#endif
