#ifndef GAME_ACT_ACT_STAGE_INFO_NPC_H
#define GAME_ACT_ACT_STAGE_INFO_NPC_H

// Code: game/act_stage_info_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804AA7A8).
class StageInfoNpc_804AA7A8 : public ActorBase_804A5770 {
public:
    StageInfoNpc_804AA7A8() {}

    /* 0x08 */ virtual ~StageInfoNpc_804AA7A8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
}; // Size: 0x230

#endif
