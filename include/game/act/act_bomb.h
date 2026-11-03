#ifndef GAME_ACT_ACT_BOMB_H
#define GAME_ACT_ACT_BOMB_H

// Code: game/act_bomb.cpp (not written yet). Only what other units read is
// declared.

#include "game/ActorBase.h"

// The HBMB actor ("Bomb", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203D1C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F784.
class Bomb_8049F784 : public ActorBase_804A5770 {
public:
    Bomb_8049F784() {}
    /* 0x230 */ u8 field_0x230[0x238 - 0x230]; // not yet known
    /* 0x238 */ s32 field_0x238;               // act_shadow_link_2's helper ignores bombs with >= 5
}; // Size: at least 0x23C

#endif
