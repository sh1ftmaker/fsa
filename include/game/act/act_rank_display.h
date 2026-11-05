#ifndef GAME_ACT_ACT_RANK_DISPLAY_H
#define GAME_ACT_ACT_RANK_DISPLAY_H

// Code: game/act_rank_display.cpp.

#include "game/ActorBase.h"

// The RANK actor ("Rank display", docs/names/actors.tsv, med confidence): the
// spawner constructs it with the ctor that stores this vtable, 0x804A8F74
// (config/G4SE01/game_units.tsv). Named after its vtable.
class RankDisplay_804A8F74 : public ActorBase_804A5770 {
public:
    RankDisplay_804A8F74() {}
    /* 0x08 */ virtual ~RankDisplay_804A8F74();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ JPABaseEmitter* mEmitters[4]; // one per player; init clears them
    /* 0x244 */ s32 field_0x244[4]; // init clears them
    /* 0x254 */ s32 field_0x254[4]; // init clears them
    /* 0x264 */ s32 field_0x264[4]; // init: fn_80237774(i) per player, else 0
    /* 0x274 */ u8 field_0x274[4];  // init: 0xFF per player
    /* 0x278 */ u8 field_0x278;     // init: 1 when gGameRoot_8053AAF8->0x7E is 1 and the three GameView tests fail
    /* 0x27C */ s32 field_0x27c[4]; // init clears them
    /* 0x28C */ s32 field_0x28c[4]; // init clears them
}; // Size: 0x29C

#endif
