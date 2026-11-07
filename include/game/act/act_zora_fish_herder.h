#ifndef GAME_ACT_ACT_ZORA_FISH_HERDER_H
#define GAME_ACT_ACT_ZORA_FISH_HERDER_H

// Code: game/act_zora_fish_herder.cpp.

#include "game/ActorBase.h"

// The SAYA actor ("Zora (fish-herding)", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80201010, which stores this
// vtable. Named after its vtable, 0x804A2E30. The fishing minigame: the
// parameter's low nibble picks the kind (0-1 play the game, the others are
// not handled by update / draw).
class ZoraFishHerding_804A2E30 : public ActorBase_804A5770 {
public:
    ZoraFishHerding_804A2E30() {}
    /* 0x08 */ virtual ~ZoraFishHerding_804A2E30();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    void fn_801A68DC();
    void fn_801A7C48();

    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234; // animation frames left (set to 8 / 15 by fn_801A7C48)
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240; // step within the animation of the current mode
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250; // animation mode, 0-5
    /* 0x254 */ s32 field_0x254; // sprite frame offset (0x303 + this in draw)
    /* 0x258 */ s32 field_0x258; // fishing time in frames, -1 for none
    /* 0x25C */ s32 field_0x25c; // fish count shown on the HUD
    /* 0x260 */ s32 mKind;       // field_0x90 & 0xF
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
}; // Size: 0x274

#endif
