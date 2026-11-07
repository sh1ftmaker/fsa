#ifndef GAME_ACT_ACT_TRICK_FLOOR_H
#define GAME_ACT_ACT_TRICK_FLOOR_H

// Code: game/act_trick_floor.cpp.

#include "game/ActorBase.h"

// The TFLR actor ("Trick floor", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802035D4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A83A0.
class TrickFloor_804A83A0 : public ActorBase_804A5770 {
public:
    TrickFloor_804A83A0() {}
    /* 0x08 */ virtual ~TrickFloor_804A83A0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234; // kind: low 7 bits of field_0x90
    /* 0x238 */ s32 field_0x238; // field_0x90 bits 7-11
    /* 0x23C */ s32 field_0x23c; // field_0x90 bits 24-25
    /* 0x240 */ s32 field_0x240; // field_0x90 bits 12-23
    /* 0x244 */ s32 field_0x244; // 20 after init
    /* 0x248 */ f32 field_0x248;
    /* 0x24C */ f32 field_0x24c;
    /* 0x250 */ u16 field_0x250;
    /* 0x252 */ u16 field_0x252; // 12 or 13 after init
}; // Size: 0x254

#endif
