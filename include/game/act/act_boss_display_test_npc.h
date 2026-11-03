#ifndef GAME_ACT_ACT_BOSS_DISPLAY_TEST_NPC_H
#define GAME_ACT_ACT_BOSS_DISPLAY_TEST_NPC_H

// Code: game/act_boss_display_test_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x8049C7F0).
// Size: 0x248 (the spawner's BOTS case allocates 0x248 at 0x801FCD7C).
class BossDisplayTestNpc_8049C7F0 : public ActorBase_804A5770 {
public:
    BossDisplayTestNpc_8049C7F0() {}

    /* 0x08 */ virtual ~BossDisplayTestNpc_8049C7F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ u8 field_0x230[0x18]; // not touched by this unit's code
}; // Size: 0x248

#endif
