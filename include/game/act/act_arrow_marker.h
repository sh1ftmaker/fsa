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
    // Set by init() from the spawn parameter in field_0x90 and used by
    // update() and draw(). Placeholder names until the arrow's behaviour is
    // worked out.
    /* 0x230 */ s32 field_0x230; // update's state: 0 idle, 1 active
    /* 0x234 */ f32 field_0x234; // x offset of the box (mPos.x or 256 + view box x)
    /* 0x238 */ f32 field_0x238; // y offset of the box (192 + view box y or mPos.y)
    /* 0x23C */ f32 field_0x23c; // mPos.z
    /* 0x240 */ s32 field_0x240; // mode: field_0x90 & 0xF
    /* 0x244 */ s32 field_0x244; // kind: field_0x90 >> 7 & 0x1F, the switch in draw and update
    /* 0x248 */ s32 field_0x248; // field_0x90 >> 4 & 1; clear lets update play the effect
    /* 0x24C */ s32 field_0x24c; // frame countdown, reset to 8 (update, kind 0)
    /* 0x250 */ s32 field_0x250; // frame index 0-3 (update), also the draw gate
    /* 0x254 */ s32 field_0x254; // alpha in draw; offset stepped by lbl_80540968 in update
    /* 0x258 */ s32 field_0x258; // index into lbl_80540968 (0 or 1)
    /* 0x25C */ s32 field_0x25c; // player in the box (vfunc_0x30 on change)
}; // Size: 0x260

#endif
