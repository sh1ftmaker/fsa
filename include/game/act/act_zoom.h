#ifndef GAME_ACT_ACT_ZOOM_H
#define GAME_ACT_ACT_ZOOM_H

// Code: game/act_zoom.cpp.

#include "game/ActorBase.h"

// The ZOOM actor ("Zoom", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_8020157C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9DF8.
class Zoom_804A9DF8 : public ActorBase_804A5770 {
public:
    Zoom_804A9DF8() {}
    /* 0x08 */ virtual ~Zoom_804A9DF8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x1C]; // not yet known
}; // Size: 0x24C

#endif
