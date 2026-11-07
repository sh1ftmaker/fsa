#ifndef GAME_ACT_ACT_PLAYER_EFFECT_H
#define GAME_ACT_ACT_PLAYER_EFFECT_H

// Code: game/act_player_effect.cpp.

#include "game/ActorBase.h"

// The PEFF actor ("Player effect", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203304, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A6E20.
class PlayerEffect_804A6E20 : public ActorBase_804A5770 {
public:
    PlayerEffect_804A6E20() {}
    /* 0x08 */ virtual ~PlayerEffect_804A6E20();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234; // the sprite frame set by init (by kind)
}; // Size: 0x238

#endif
