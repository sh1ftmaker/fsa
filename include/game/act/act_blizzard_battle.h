#ifndef GAME_ACT_ACT_BLIZZARD_BATTLE_H
#define GAME_ACT_ACT_BLIZZARD_BATTLE_H

// Code: game/act_blizzard_battle.cpp. The BTBZ actor ("Blizzard (battle)",
// docs/names/actors.tsv). Named after its vtable, 0x8049E7F0.

#include "game/ActorBase.h"

class JPABaseEmitter;

class cls_8049E7F0 : public ActorBase_804A5770 {
public:
    cls_8049E7F0();
    /* 0x08 */ virtual ~cls_8049E7F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();

    /* 0x230 */ u8 field_0x230; // field_0x90 bit 0
    /* 0x231 */ u8 field_0x231; // field_0x90 bit 1; when set, update calls fn_801F0D20(6)
    /* 0x234 */ s32 field_0x234; // phase: 0 emitter stopped, 1 emitter playing, 2 spawn (runPhase)
    /* 0x238 */ s32 field_0x238; // frame counter
    /* 0x23C */ JPABaseEmitter* mEmitter; // created in init with the view's visible box

private:
    void stopEmitter();
    void playEmitter();
    void notifySwev();
    void spawnPuff();
    void runPhase();
};

#endif
