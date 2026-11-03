#ifndef GAME_ACT_ACT_VAATI_DEMO_H
#define GAME_ACT_ACT_VAATI_DEMO_H

// Code: game/act_vaati_demo.cpp.

#include "game/act/tu_8010DF44.h"

// Named after its vtable, 0x8049D940. Derives from cls_8049D140; its
// constructor (fn_80116BE8) zeroes field_0x230 after the base constructor.
class cls_8049D940 : public cls_8049D140 {
public:
    /* 0x58 */ virtual s32 vfunc_0x58();

    /* 0x230 */ s32 field_0x230;
}; // Size: at least 0x234

#endif
