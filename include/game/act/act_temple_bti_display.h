#ifndef GAME_ACT_ACT_TEMPLE_BTI_DISPLAY_H
#define GAME_ACT_ACT_TEMPLE_BTI_DISPLAY_H

// Code: game/act_temple_bti_display.cpp.

#include "game/ActorBase.h"

// Spawned for 'SIBT'; moved here from game/act/tu_801F9484.h.
class TempleBTIDisplay_8049CB04 : public ActorBase_804A5770 {
public:
    TempleBTIDisplay_8049CB04() {}
    /* 0x08 */ virtual ~TempleBTIDisplay_8049CB04();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ u8 field_0x230[4];
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ f32 field_0x240;
    /* 0x244 */ u8 field_0x244;
    /* 0x245 */ u8 field_0x245;
    /* 0x246 */ u8 field_0x246;
    /* 0x247 */ u8 field_0x247;
}; // Size: 0x248

#endif
