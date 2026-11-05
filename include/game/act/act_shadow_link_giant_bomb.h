#ifndef GAME_ACT_ACT_SHADOW_LINK_GIANT_BOMB_H
#define GAME_ACT_ACT_SHADOW_LINK_GIANT_BOMB_H

// Code: game/act_shadow_link_giant_bomb.cpp. The actor's constructor (0x80201B00,
// another unit) stores this vtable, 0x804AC4D0 (21 slots; the destructor, init,
// update and draw are overridden, the rest is ActorBase's). Named after it.
#include "game/ActorBase.h"

class ShadowLinkGiantBomb_804AC4D0 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~ShadowLinkGiantBomb_804AC4D0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 mState;      // update() dispatches on it (0..11); init sets 0 or 1
    /* 0x234 */ s32 mStateTimer; // init: field_0x90 & 0x7F, or 0 when the top bits are set
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c; // init clears it; update passes it to drawSprite_801F06F0
    // Bits 12-19 of field_0x90, counted down once in init.
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244; // init stores -1
    // Bits 22-26 of field_0x90; a GameView screen bit (fn_802D800C) in init.
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ u8 field_0x24c[0x250 - 0x24C];
    /* 0x250 */ f32 field_0x250; // init stores 1.0f
    /* 0x254 */ u8 field_0x254;  // set when field_0x240 was 0 in init
    /* 0x255 */ u8 field_0x255;  // set when the view bit is clear and the mode flag is set
}; // Size: at least 0x256

#endif
