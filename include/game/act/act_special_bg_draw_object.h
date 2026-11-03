#ifndef GAME_ACT_ACT_SPECIAL_BG_DRAW_OBJECT_H
#define GAME_ACT_ACT_SPECIAL_BG_DRAW_OBJECT_H

// Code: game/act_special_bg_draw_object.cpp.

#include "game/ActorBase.h"

// The SPBG actor ("Special BG draw object", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80201348, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC760.
class SpecialBGDrawObject_804AC760 : public ActorBase_804A5770 {
public:
    SpecialBGDrawObject_804AC760() {}
    /* 0x08 */ virtual ~SpecialBGDrawObject_804AC760();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x1C]; // not yet known
}; // Size: 0x24C

#endif
