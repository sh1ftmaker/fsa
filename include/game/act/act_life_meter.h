#ifndef GAME_ACT_ACT_LIFE_METER_H
#define GAME_ACT_ACT_LIFE_METER_H

// Code: game/act_life_meter.cpp.

#include "game/ActorBase.h"

// The LIMT actor ("Life meter", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_8020185C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804B174C.
class LifeMeter_804B174C : public ActorBase_804A5770 {
public:
    LifeMeter_804B174C() {}
    /* 0x08 */ virtual ~LifeMeter_804B174C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // 0x8019CEF8 (game/tu_80197070.cpp, the ZLD2 actor's update): follow the actor
    // with this serial, y offset and heart index.
    void fn_8019CEF8(s32 serial, f32 yOffset, s32 index);
    /* 0x230 */ s32 field_0x230; // state: 0 starts, 1 follows the player or actor
    /* 0x234 */ s32 mTimers[6]; // update counts the first three down to 0; init zeroes all
    /* 0x24C */ s32 field_0x24c; // index into the five "kk_heart_NN.bti" images; init sets 4
    /* 0x250 */ s32 field_0x250; // the followed player (field_0x90 & 0xF != 0) or actor serial
    /* 0x254 */ f32 field_0x254; // added to the followed position's y
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c; // draw's x scale
    /* 0x260 */ f32 field_0x260; // draw's y and z scale
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ f32 field_0x270;
}; // Size: 0x274

#endif
