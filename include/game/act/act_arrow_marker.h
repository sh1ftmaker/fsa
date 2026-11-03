#ifndef GAME_ACT_ACT_ARROW_MARKER_H
#define GAME_ACT_ACT_ARROW_MARKER_H

// Code: game/act_arrow_marker.cpp.

#include "game/ActorBase.h"

// The YAJI actor ("Arrow marker", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_802034E4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8470.
class ArrowMarker_804A8470 : public ActorBase_804A5770 {
public:
    ArrowMarker_804A8470() {}
    /* 0x08 */ virtual ~ArrowMarker_804A8470();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x30]; // not yet known
}; // Size: 0x260

#endif
