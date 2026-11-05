#ifndef GAME_ACT_TU_80104C3C_H
#define GAME_ACT_TU_80104C3C_H

// Code: game/tu_80104C3C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049CB58.
class cls_8049CB58 : public ActorBase_804A5770 {
public:
    cls_8049CB58() : field_0x24e(0) {}
    /* 0x08 */ virtual ~cls_8049CB58();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    bool fn_80104EB0();
    /* 0x230 */ s32 field_0x230[4]; // init(): (field_0x90 >> shift) & 0x1F per slot; bits for GameView::fn_802D800C
    /* 0x240 */ s32 field_0x240;    // field_0x90 & 0xF; update()'s mode (0: all four, 1: any, 2: some but not all)
    /* 0x244 */ s32 field_0x244;    // field_0x90 & 0x10
    /* 0x248 */ u16 field_0x248;    // countdown
    /* 0x24A */ u16 field_0x24a;    // step
    /* 0x24C */ u8 field_0x24c;
    /* 0x24D */ u8 field_0x24d;     // frames counted while field_0x24c is set (50)
    /* 0x24E */ u8 field_0x24e;
    /* 0x24F */ u8 field_0x24f;     // fn_80104EB0's state
}; // Size: 0x250

#endif
