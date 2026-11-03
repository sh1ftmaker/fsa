#ifndef GAME_ACT_ACT_ZOOM_OUT_NPC_H
#define GAME_ACT_ACT_ZOOM_OUT_NPC_H

// Code: game/act_zoom_out_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AA228.
class cls_804AA228 : public ActorBase_804A5770 {
public:
    cls_804AA228() {}
    /* 0x08 */ virtual ~cls_804AA228();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0xC]; // not yet known
}; // Size: 0x23C

#endif
