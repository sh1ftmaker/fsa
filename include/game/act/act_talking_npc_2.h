#ifndef GAME_ACT_ACT_TALKING_NPC_2_H
#define GAME_ACT_ACT_TALKING_NPC_2_H

// Code: game/act_talking_npc_2.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804AFAD8).
class TalkingNpc2_804AFAD8 : public ActorBase_804A5770 {
public:
    TalkingNpc2_804AFAD8() {}
    virtual ~TalkingNpc2_804AFAD8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u32 field_0x234; // low 16 bits of the spawn parameter (0x90)
    /* 0x238 */ s32 field_0x238; // bits 16-27 of the spawn parameter
    /* 0x23C */ u8 field_0x23c;  // bits 28-31 of the spawn parameter
}; // Size: 0x240

#endif
