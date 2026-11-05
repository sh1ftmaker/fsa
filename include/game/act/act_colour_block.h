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
    /* 0x230 */ u8 field_0x230[0x1C]; // not yet known
}; // Size: 0x24C

#endif
