#ifndef GAME_ACT_TU_801AE9C4_H
#define GAME_ACT_TU_801AE9C4_H

// Code: game/tu_801AE9C4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A3448.
class OkameSan_804A3448 : public ActorBase_804A5770 {
public:
    OkameSan_804A3448() {}
    /* 0x08 */ virtual ~OkameSan_804A3448();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    void fn_801AF1EC();
    void fn_801AF678();
    void fn_801AF7D0(s32 state);

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258; // state, set by fn_801AF7D0
    /* 0x25C */ u8 field_0x25c;
    /* 0x25D */ u8 field_0x25D[0x3];
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
}; // Size: 0x284

#endif
