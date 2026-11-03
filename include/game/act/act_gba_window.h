#ifndef GAME_ACT_ACT_GBA_WINDOW_H
#define GAME_ACT_ACT_GBA_WINDOW_H

// Code: game/act_gba_window.cpp.

#include "game/ActorBase.h"

// The AGBW actor ("GBA window control", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80200194, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA440.
class GBAWindowControl_804AA440 : public ActorBase_804A5770 {
public:
    GBAWindowControl_804AA440() {}
    /* 0x08 */ virtual ~GBAWindowControl_804AA440();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
