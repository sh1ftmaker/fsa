#ifndef GAME_ACT_ACT_ZOOM_H
#define GAME_ACT_ACT_ZOOM_H

// Code: game/act_zoom.cpp.

#include "game/ActorBase.h"

// The ZOOM actor ("Zoom", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_8020157C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9DF8.
// Layout from init/update (0x802E6BCC, 0x802E69B0); field_0x90 is decoded by init.
class Zoom_804A9DF8 : public ActorBase_804A5770 {
public:
    Zoom_804A9DF8() {}
    /* 0x08 */ virtual ~Zoom_804A9DF8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;      // 0 waiting for a player, 1 and 3 camera hold, 2 counting down
    /* 0x234 */ s32 field_0x234; // (field_0x90 >> 23) & 3; index into the id table passed to fn_801F0D20
    /* 0x238 */ s32 field_0x238; // (field_0x90 >> 22) & 1
    /* 0x23C */ s32 mDuration;   // field_0x90 & 0xFFF
    /* 0x240 */ s32 mTimer;
    /* 0x244 */ s32 mHalfWidth;  // ((field_0x90 >> 17) & 0x1F) + 1, in 16-unit steps
    /* 0x248 */ s32 mHalfHeight; // ((field_0x90 >> 12) & 0x1F) + 1, in 16-unit steps
}; // Size: 0x24C

#endif
