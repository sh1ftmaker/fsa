#ifndef GAME_ACT_ACT_HINOX_H
#define GAME_ACT_ACT_HINOX_H

// Code: game/act_hinox.cpp.

#include "game/ActorBase.h"

// Spawned for 'HINO' (Hinox). Named after its vtable, 0x8049F9B4.
class Hinox_8049F9B4 : public ActorBase_804A5770 {
public:
    Hinox_8049F9B4() {}
    /* 0x08 */ virtual ~Hinox_8049F9B4();
    /* 0x0C */ virtual void init();   // fn_8014F9FC
    /* 0x14 */ virtual void update(); // fn_8014DD14, not written yet
    /* 0x20 */ virtual void draw();   // fn_8014DCD4
    // 0x8014F76C: lets go of the actor held at field_0x250 (it drops with vertical speed 3).
    void fn_8014F76C();

    // SHARED-EDIT: carved from field_0x230[0x58] (game/act_hinox.cpp).
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250; // serial of the held actor, -1 for none
    /* 0x254 */ s32 field_0x254; // player index held, -1 for none
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ u16 field_0x26c;
    /* 0x26E */ u16 field_0x26e;
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
}; // Size: 0x288

#endif
