#ifndef GAME_ACT_ACT_HARDHAT_BEETLE_H
#define GAME_ACT_ACT_HARDHAT_BEETLE_H

// Code: game/act_hardhat_beetle.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A06C0.
class cls_804A06C0 : public ActorBase_804A5770 {
public:
    cls_804A06C0() {}
    /* 0x08 */ virtual ~cls_804A06C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState; // draw builds the rotated sprite in state 2
    /* 0x234 */ u8 field_0x234[0x250 - 0x234]; // not yet known
    /* 0x250 */ s32 mFrame;  // draw: "SOB " frame mFrame + 0x92
    /* 0x254 */ f32 mScale;  // draw: the sprite matrix's scale
    /* 0x258 */ f32 mAngle;  // draw: the rotation about y (radians)
    /* 0x25C */ u8 field_0x25c[0x284 - 0x25C]; // not yet known
}; // Size: 0x284

#endif
