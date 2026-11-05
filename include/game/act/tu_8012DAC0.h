#ifndef GAME_ACT_TU_8012DAC0_H
#define GAME_ACT_TU_8012DAC0_H

// Code: game/tu_8012DAC0.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049E848.
class cls_8049E848 : public ActorBase_804A5770 {
public:
    cls_8049E848() : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(0), field_0x240(0), field_0x244(0) {}
    /* 0x08 */ virtual ~cls_8049E848();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ s32 field_0x230; // state: 0 waiting, 1 held while vfunc_0x3C() && !vfunc_0x34()
    /* 0x234 */ s32 field_0x234; // (field_0x90 >> 7) & 0x1F, 0 if >= 4 (init only)
    /* 0x238 */ s32 field_0x238; // frame counter
    /* 0x23C */ s32 field_0x23c; // (field_0x90 & 0x7F) * 10, 0x78 if not positive
    /* 0x240 */ s32 field_0x240; // index 0..3 into the u16 tile table
    /* 0x244 */ s32 field_0x244; // (field_0x90 >> 22) & 0x1F: bit for GameView::fn_802D800C
}; // Size: 0x248

#endif
