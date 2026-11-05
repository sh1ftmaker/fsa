#ifndef GAME_ACT_TU_801286C0_H
#define GAME_ACT_TU_801286C0_H

// Code: game/tu_801286C0.cpp.

#include "game/act/tu_8010DF44.h"

// Named after its vtable, 0x8049E500. Derives from cls_8049D140 and adds the
// state word (field_0x230, 0..10 for the switch in fn_80128728), two floats
// and the resource pointer that init() gets from the archive.
class cls_8049E500 : public cls_8049D140 {
public:
    cls_8049E500() : field_0x230(0), field_0x234(0.0f), field_0x238(0.0f), field_0x23c(0) {}
    /* 0x08 */ virtual ~cls_8049E500();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();

    // Switch on the state (case 0..10), called by init with 0.
    void fn_80128728(int state);

    /* 0x230 */ s32 field_0x230;
    // Converted to an integer by draw() and stored in the byte at 0xC of
    // the object at gGameRoot2_8053AB10->0x2C (fn_80128C58).
    /* 0x234 */ f32 field_0x234;
    // Passed to fn_8044C028 by draw() when the state is 1..3 (its offset).
    /* 0x238 */ f32 field_0x238;
    // getResource('????', lbl_80539F38) from the archive at gGameRoot_8053AAF8
    // (init); cleared by the destructor.
    /* 0x23C */ void* field_0x23c;
}; // Size: at least 0x240

// Stores the value in the byte at 0xC of the object at gGameRoot2_8053AB10->0x2C.
void fn_80128C58(s32 value);

#endif
