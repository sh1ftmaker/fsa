#ifndef GAME_ACT_ACT_FLAME_H
#define GAME_ACT_ACT_FLAME_H

// Code: game/act_flame.cpp.

#include "game/ActorBase.h"

// Spawned for 'FLME'; moved here from game/act/tu_801F9484.h.
class Flame_804A0330 : public ActorBase_804A5770 {
public:
    Flame_804A0330() {}
    /* 0x08 */ virtual ~Flame_804A0330();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    void fn_8015DF7C();
    void fn_8015EA2C();
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234; // timers counted down by update
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ u8 field_0x244[0x10];
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ u8 field_0x258[0xC];
    /* 0x264 */ JPABaseEmitter* mEmitters[4]; // released by the destructor
}; // Size: 0x274

#endif
