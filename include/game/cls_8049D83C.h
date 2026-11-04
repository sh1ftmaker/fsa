#ifndef GAME_CLS_8049D83C_H
#define GAME_CLS_8049D83C_H

// Code: game/tu_80113788.cpp. A cls_8049D140 subclass named after its vtable,
// 0x8049D83C. Its constructor (0x80115590) zeroes field_0x23c; only the slots
// at 0x58 and 0x5C are written so far.

#include "game/act/tu_8010DF44.h"

class cls_8049D83C : public cls_8049D140 {
public:
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();

    /* 0x230 */ u8 field_0x230[0x23C - 0x230]; // not yet known
    /* 0x23C */ s32 field_0x23c; // not yet known
}; // Size: at least 0x240

#endif
