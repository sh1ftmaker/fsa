#ifndef GAME_ACT_ACT_BATTLE_ROYALE_END_CHECK_H
#define GAME_ACT_ACT_BATTLE_ROYALE_END_CHECK_H

// Code: game/act_battle_royale_end_check.cpp.

#include "game/ActorBase.h"

// The BFIN actor ("Battle royale end check", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80202D84, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804B0380.
class BattleRoyaleEndCheck_804B0380 : public ActorBase_804A5770 {
public:
    BattleRoyaleEndCheck_804B0380() {}
    /* 0x08 */ virtual ~BattleRoyaleEndCheck_804B0380();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x18]; // not yet known
}; // Size: 0x248

#endif
