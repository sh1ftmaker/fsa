#ifndef GAME_ACT_ACT_SIDE_VIEW_GATE_DISPLAY_NPC_H
#define GAME_ACT_ACT_SIDE_VIEW_GATE_DISPLAY_NPC_H

// Code: game/act_side_view_gate_display_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804ACB98).
class SideViewGateDisplayNpc_804ACB98 : public ActorBase_804A5770 {
public:
    SideViewGateDisplayNpc_804ACB98() {}
    virtual ~SideViewGateDisplayNpc_804ACB98();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ u8 field_0x230[4];
    /* 0x234 */ JPABaseEmitter* mEmitter234; // fn_8013CC50(field_0x4, &mPos, 0x447, ...)
    /* 0x238 */ JPABaseEmitter* mEmitter238; // fn_8013CC50(field_0x4, &mPos, 0x448, ...)
}; // Size: 0x23C

#endif
