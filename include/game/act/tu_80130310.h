#ifndef GAME_ACT_TU_80130310_H
#define GAME_ACT_TU_80130310_H

// Code: game/tu_80130310.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049EA88.
class cls_8049EA88 : public ActorBase_804A5770 {
public:
    cls_8049EA88() : field_0x230(0), field_0x234(-1), field_0x238(-1), field_0x23c(-1), field_0x240(-1), field_0x244(0), field_0x248(0), field_0x24c(0) {}
    /* 0x08 */ virtual ~cls_8049EA88();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    s32 fn_8013056C(u16 msg, s32 mode, s32 value);
    bool fn_80130370();
    void setState(s32 state); // the state switch after every field_0x230 store (defined inline)
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
}; // Size: 0x250

#endif
