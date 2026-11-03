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
    /* 0x230 */ s32 field_0x230; // field_0x90 & 0xFFF; compared with the tile code
    /* 0x234 */ s32 field_0x234; // (field_0x90 >> 12) & 0xFFF; the tile code written back
    /* 0x238 */ s32 field_0x238; // (field_0x90 >> 24) & 1; plays a sound on a match
    /* 0x23C */ s32 field_0x23c; // (field_0x90 >> 25) & 1; spawns an effect or actor
}; // Size: 0x240

#endif
