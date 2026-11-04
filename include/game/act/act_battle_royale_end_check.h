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

    // The state functions: update() calls lbl_804B032C[mState] (the table in
    // game/act_battle_royale_end_check.cpp). Entry order: 0 fn_803D6BDC,
    // 1 fn_803D6AE8, 2 fn_803D69AC, 3 fn_803D6730, 4 fn_803D66A8,
    // 5 fn_803D6618, 6 fn_803D6494.
    void fn_803D6BDC();
    void fn_803D6AE8();
    void fn_803D69AC();
    void fn_803D6730();
    void fn_803D66A8();
    void fn_803D6618();
    void fn_803D6494();

    /* 0x230 */ s32 mState;        // index into the state table, 0-6
    /* 0x234 */ s32 mWinner;       // player index, -1 until one is picked
    /* 0x238 */ s32 field_0x238;   // counter, bumped by fn_803D6730
    /* 0x23C */ s32 mTimer;        // countdown; reset to 0x82 by init
    /* 0x240 */ u8 mFocus;         // 0xFF when none; else drawn into lbl_8053AED0
    /* 0x241 */ u8 mPlayerFlag[4]; // per player, cleared by init
    /* 0x245 */ u8 field_0x245[0x248 - 0x245]; // not yet known
}; // Size: 0x248

#endif
