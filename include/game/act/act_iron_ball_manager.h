#ifndef GAME_ACT_ACT_IRON_BALL_MANAGER_H
#define GAME_ACT_ACT_IRON_BALL_MANAGER_H

// Code: game/act_iron_ball_manager.cpp.

#include "game/ActorBase.h"

// The TKYU actor ("Iron ball manager", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80203AC4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804ABA38.
class IronBallManager_804ABA38 : public ActorBase_804A5770 {
public:
    IronBallManager_804ABA38() {}
    /* 0x08 */ virtual ~IronBallManager_804ABA38();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ JGeometry::TVec3f field_0x230; // copy of mPos from init
    /* 0x23C */ f32 field_0x23c;               // chance threshold, 0.5 at first
    /* 0x240 */ s32 field_0x240;               // frames until the next ball
    /* 0x244 */ u8 field_0x244;                // field_0x90 byte 1: base delay
    /* 0x245 */ u8 field_0x245;                // field_0x90 byte 2: random delay range
    /* 0x246 */ u8 field_0x246;
    /* 0x247 */ u8 field_0x247;                // field_0x90 & 0xF
    /* 0x248 */ u8 field_0x248;                // (field_0x90 >> 4) & 0xF: < 2 moves x, else y
    /* 0x249 */ u8 field_0x249[3];             // padding
}; // Size: 0x24C

#endif
