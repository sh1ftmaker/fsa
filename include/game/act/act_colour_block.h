#ifndef GAME_ACT_ACT_COLOUR_BLOCK_H
#define GAME_ACT_ACT_COLOUR_BLOCK_H

// Code: game/act_colour_block.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE620.
class ColourBlock1Player_804AE620 : public ActorBase_804A5770 {
public:
    ColourBlock1Player_804AE620() {}
    /* 0x08 */ virtual ~ColourBlock1Player_804AE620();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    bool fn_80374964(u8 dir);
    /* 0x230 */ JGeometry::TVec3f field_0x230;
    /* 0x23C */ u8 field_0x23c[4];
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ u8 field_0x248;
    /* 0x249 */ u8 field_0x249[3];
}; // Size: 0x24C

#endif
