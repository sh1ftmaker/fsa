#ifndef GAME_ACT_ACT_PENGATOR_H
#define GAME_ACT_ACT_PENGATOR_H

// Code: game/act_pengator.cpp.

#include "game/ActorBase.h"

// The FAGN actor ("Pengator", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_8020221C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A1888.
class Pengator_804A1888 : public ActorBase_804A5770 {
public:
    Pengator_804A1888() {}
    /* 0x08 */ virtual ~Pengator_804A1888();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x18 */ virtual void vfunc_0x18();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state: update()'s switch (0-3)
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // draw's frame base: field_0x98 * 4 (init), then a table value (update)
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ f32 field_0x260;
}; // Size: 0x264

#endif
