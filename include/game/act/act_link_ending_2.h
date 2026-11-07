#ifndef GAME_ACT_ACT_LINK_ENDING_2_H
#define GAME_ACT_ACT_LINK_ENDING_2_H

// Code: game/act_link_ending_2.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// Spawned for 'ELK2' (Link 2, ending).
class Link2Ending_804AE338 : public ActorBase_804A5770 {
public:
    Link2Ending_804AE338() {}
    /* 0x08 */ virtual ~Link2Ending_804AE338();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Force-deletes the emitters (field_0x258, field_0x2A8); the destructor inlines it.
    void fn_8036E544();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238; // -1 after init
    /* 0x23C */ s32 field_0x23C; // 10 after init
    /* 0x240 */ s32 field_0x240; // 10 after init
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24C; // 0x18 after init; marker id base (fn_80230498)
    /* 0x250 */ ActorBase_804A5770* field_0x250; // the live EOWL actor (init)
    /* 0x254 */ ActorBase_804A5770* field_0x254; // the live ENDM actor (init)
    /* 0x258 */ JPABaseEmitter* field_0x258[20];
    /* 0x2A8 */ JPABaseEmitter* field_0x2A8;
    /* 0x2AC */ s32 field_0x2AC;
    /* 0x2B0 */ s32 field_0x2B0;
    /* 0x2B4 */ s32 field_0x2B4;
}; // Size: 0x2B8

#endif
