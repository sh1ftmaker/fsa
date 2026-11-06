#ifndef GAME_ACT_ACT_BTI_NPC_H
#define GAME_ACT_ACT_BTI_NPC_H

// Code: game/act_bti_npc.cpp.

#include "game/ActorBase.h"

// The BTIN actor ("BTI NPC", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802021E0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A927C.
class BTINPC_804A927C : public ActorBase_804A5770 {
public:
    BTINPC_804A927C() {}
    /* 0x08 */ virtual ~BTINPC_804A927C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s16 field_0x248;
    /* 0x24C */ f32 field_0x24c;
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ u8 field_0x258;
    /* 0x259 */ u8 field_0x259;
}; // Size: 0x25C

#endif
