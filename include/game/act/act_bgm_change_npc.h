#ifndef GAME_ACT_ACT_BGM_CHANGE_NPC_H
#define GAME_ACT_ACT_BGM_CHANGE_NPC_H

// Code: game/act_bgm_change_npc.cpp.

#include "game/ActorBase.h"

// The BGMN actor ("BGM change NPC", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_802015B8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9DA0.
class BGMChangeNPC_804A9DA0 : public ActorBase_804A5770 {
public:
    BGMChangeNPC_804A9DA0() {}
    /* 0x08 */ virtual ~BGMChangeNPC_804A9DA0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230; // cleared by init
    /* 0x234 */ s32 field_0x234; // spawn parameter (0x90) & 0x7F
    // Each of the two counts is the parameter's 5-bit field + 1; the box
    // written in init() is 16 units per count on each side.
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240; // (0x90 >> 12) & 0x1F
    /* 0x244 */ u8 field_0x244[0x4]; // not yet known
    /* 0x248 */ s32 field_0x248; // GameView's field_0x34, read in init()
    /* 0x24C */ s32 field_0x24c; // -1 after init
    /* 0x250 */ s32 field_0x250; // (0x90 >> 7) & 1
    /* 0x254 */ u8 field_0x254[0xC]; // not yet known
}; // Size: 0x260

#endif
