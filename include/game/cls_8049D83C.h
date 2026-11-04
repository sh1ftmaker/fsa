#ifndef GAME_CLS_8049D83C_H
#define GAME_CLS_8049D83C_H

// Code: game/tu_80113788.cpp. A cls_8049D140 subclass named after its vtable,
// 0x8049D83C. Its constructor (0x80115590) zeroes field_0x23c.

#include "game/act/tu_8010DF44.h"

class cls_8049D83C : public cls_8049D140 {
public:
    /* 0x20 */ virtual void draw();
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();

    // Members of this class, not written yet (0x80113790, 0x80113B54).
    void fn_80113790();
    void fn_80113B54(s32 arg);

    /* 0x230 */ u8 field_0x230[0x232 - 0x230]; // not yet known
    /* 0x232 */ u8 field_0x232; // tested by draw
    /* 0x233 */ u8 field_0x233[0x238 - 0x233]; // not yet known
    /* 0x238 */ s32 field_0x238; // index into the table draw reads
    /* 0x23C */ s32 field_0x23c; // returned by vfunc_0x58
}; // Size: at least 0x240

#endif
