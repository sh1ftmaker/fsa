#ifndef GAME_ACT_ACT_VIEW_MODE_H
#define GAME_ACT_ACT_VIEW_MODE_H

// Code: game/act_view_mode.cpp.

#include "game/ActorBase.h"

// The VWMD actor ("View mode", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202078, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9A18.
class ViewMode_804A9A18 : public ActorBase_804A5770 {
public:
    ViewMode_804A9A18() {}
    /* 0x08 */ virtual ~ViewMode_804A9A18();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // field_0x90 >> 27 (init); the bit passed to GameView::fn_802D800C
}; // Size: 0x234

#endif
