#ifndef GAME_ACT_ACT_DIGGING_GAME_H
#define GAME_ACT_ACT_DIGGING_GAME_H

// Code: game/act_digging_game.cpp.

#include "game/ActorBase.h"

// The ANHO actor ("Digging Game Keeper", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_8020148C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049C970.
class DiggingGameKeeper_8049C970 : public ActorBase_804A5770 {
public:
    DiggingGameKeeper_8049C970() {}
    /* 0x08 */ virtual ~DiggingGameKeeper_8049C970();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234;
    /* 0x235 */ u8 field_0x235;
    /* 0x236 */ u8 field_0x236;
    /* 0x237 */ u8 pad_0x237;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ u8 field_0x250;
    /* 0x251 */ u8 pad_0x251[3];
    /* 0x254 */ s32 field_0x254[4];
    /* 0x264 */ s32 field_0x264[4];
    /* 0x274 */ s32 field_0x274[4];
}; // Size: 0x284

#endif
