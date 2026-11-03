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
    /* 0x230 */ u8 field_0x230[0x30]; // not yet known
}; // Size: 0x260

#endif
