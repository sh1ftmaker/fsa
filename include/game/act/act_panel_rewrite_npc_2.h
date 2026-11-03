#ifndef GAME_ACT_ACT_PANEL_REWRITE_NPC_2_H
#define GAME_ACT_ACT_PANEL_REWRITE_NPC_2_H

// Code: game/act_panel_rewrite_npc_2.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9F58.
class cls_804A9F58 : public ActorBase_804A5770 {
public:
    cls_804A9F58() {}
    /* 0x08 */ virtual ~cls_804A9F58();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
