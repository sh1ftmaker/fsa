#ifndef GAME_ACT_ACT_HARDHAT_BEETLE_H
#define GAME_ACT_ACT_HARDHAT_BEETLE_H

// Code: game/act_hardhat_beetle.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A06C0.
class HardhatBeetle_804A06C0 : public ActorBase_804A5770 {
public:
    HardhatBeetle_804A06C0() {}
    /* 0x08 */ virtual ~HardhatBeetle_804A06C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState; // draw builds the rotated sprite in state 2
    // SHARED-EDIT: five countdowns (update decrements them in a loop), were field_0x234..field_0x244.
    /* 0x234 */ s32 field_0x234[5]; // [1] and [3] are reloaded by update
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 mFrame;  // draw: "SOB " frame mFrame + 0x92
    /* 0x254 */ f32 mScale;  // draw: the sprite matrix's scale
    /* 0x258 */ f32 mAngle;  // draw: the rotation about y (radians)
    /* 0x25C */ u8 field_0x25c;
    /* 0x260 */ JGeometry::TVec3f mHome; // mPos at init
    /* 0x26C */ JGeometry::TVec3f field_0x26c;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
}; // Size: 0x284

#endif
