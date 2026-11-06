#ifndef GAME_ACT_ACT_ALL_ENEMIES_DEFEATED_H
#define GAME_ACT_ACT_ALL_ENEMIES_DEFEATED_H

// Code: game/act_all_enemies_defeated.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// Spawned for 'ALCE'. Named after its vtable, 0x804A2AB0.
class AllEnemiesDefeated_804A2AB0 : public ActorBase_804A5770 {
public:
    AllEnemiesDefeated_804A2AB0() {}
    /* 0x08 */ virtual ~AllEnemiesDefeated_804A2AB0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();

    void fn_8019D2AC();
    void fn_8019D958();
    void fn_8019DC5C();

    /* 0x230 */ s32 mField_0x230;
    /* 0x234 */ s32 mField_0x234;
    /* 0x238 */ s32 mField_0x238;
    /* 0x23C */ s32 mTimers[3];
    /* 0x248 */ s32 mField_0x248;
    /* 0x24C */ s32 mField_0x24c;
    /* 0x250 */ s32 mField_0x250;
    /* 0x254 */ s32 mField_0x254;
    /* 0x258 */ s32 mField_0x258;
    /* 0x25C */ s32 mField_0x25c;
    /* 0x260 */ JPABaseEmitter* mEmitters[2];
}; // Size: 0x268

#endif
