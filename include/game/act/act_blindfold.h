#ifndef GAME_ACT_ACT_BLINDFOLD_H
#define GAME_ACT_ACT_BLINDFOLD_H

// Code: game/act_blindfold.cpp.

#include "game/ActorBase.h"

// The BLDF actor ("Blindfold", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202958, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8EF8.
class Blindfold_804A8EF8 : public ActorBase_804A5770 {
public:
    Blindfold_804A8EF8() {}
    /* 0x08 */ virtual ~Blindfold_804A8EF8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state: 0 waiting, 1 fading out, 2 clear, 3 fading in, 4 never set
    /* 0x234 */ s32 field_0x234; // spawn parameter & 0xFF
    /* 0x238 */ s32 field_0x238; // width in 8-pixel tiles ((param >> 8 & 0xFF) + 1)
    /* 0x23C */ s32 field_0x23c; // height in tiles ((param >> 16 & 0xFF) + 1)
    /* 0x240 */ bool field_0x240; // spawn parameter bit 24: burst when first uncovered
    /* 0x241 */ bool field_0x241; // spawn parameter bit 25: fades
    /* 0x242 */ bool field_0x242; // spawn parameter bit 26: starts uncovered
    /* 0x244 */ s32 field_0x244; // alpha of the black sprite
}; // Size: 0x248

#endif
