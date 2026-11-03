#ifndef GAME_ACT_ACT_RAT_H
#define GAME_ACT_ACT_RAT_H

// Code: game/act_rat.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x8049C400).
class Rat_8049C400 : public ActorBase_804A5770 {
public:
    Rat_8049C400() {}
    virtual ~Rat_8049C400();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 mDir;  // draw: 2 frames per step
    /* 0x235 */ u8 mStep; // draw: index into lbl_80539E68
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 mFrameBase; // draw: added to every frame
    /* 0x244 */ s32 field_0x244;
}; // Size: 0x248

#endif
