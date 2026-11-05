#ifndef GAME_ACT_ACT_ON_OFF_SWITCH_H
#define GAME_ACT_ACT_ON_OFF_SWITCH_H

// Code: game/act_on_off_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8FC8.
class cls_804A8FC8 : public ActorBase_804A5770 {
public:
    cls_804A8FC8() {}
    /* 0x08 */ virtual ~cls_804A8FC8();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor
    /* 0x234 */ JPABaseEmitter* mEmitter234; // released by the destructor
    /* 0x238 */ s32 mState;           // state of update (0-4); draw shows the shadow in 1 and 2
    /* 0x23C */ s32 field_0x23c;      // countdown: update sets 25 and counts it down
    /* 0x240 */ s32 field_0x240;      // (field_0x90 >> 12) & 7; update and init branch on it
    /* 0x244 */ s32 field_0x244;      // field_0x90 >> 27; compared with a switch's mode in vfunc_0x10
    /* 0x248 */ s32 field_0x248;      // (field_0x90 >> 25) & 3; init's players switch
    /* 0x24C */ s32 field_0x24c;      // field_0x90 & 0xFFF; update copies it into the countdown
    /* 0x250 */ u8 field_0x250[0x4];  // not yet known
    /* 0x254 */ s32 field_0x254;      // flag: init 0; update sets 0 or 1
    /* 0x258 */ s32 field_0x258;      // top 5 bits of the switch's field_0x90; fn_802D800C reads it
}; // Size: 0x25C

#endif
