#ifndef GAME_ACT_ACT_PRINCESS_ZELDA_DEMO_H
#define GAME_ACT_ACT_PRINCESS_ZELDA_DEMO_H

// Code: game/act_princess_zelda_demo.cpp.

#include "game/act/tu_8010DF44.h"

// The DMZL actor ("Princess Zelda (cutscene)", docs/names/actors.tsv). Its
// ctor calls cls_8049D140's ctor, then stores this vtable, 0x8049D208. Named
// after that vtable. Its last three slots (0x54, 0x58, 0x5C) override
// cls_8049D140's.
class PrincessZeldaDemo_8049D208 : public cls_8049D140 {
public:
    /* 0x58 */ virtual s32 vfunc_0x58();

    void fn_8010E2D4(s32 arg);
    void fn_8010E7E8(s32 arg);
    void fn_8010F7EC(s32 arg);
    void fn_8010F9F0();
    void fn_80110198();

    /* 0x230 */ u8 field_0x230; // set to 0 by the ctor
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ u8 field_0x23c[0xC0]; // not yet known
    /* 0x2FC */ s32 field_0x2fc;
}; // Size: unknown (>= 0x23C)

#endif
