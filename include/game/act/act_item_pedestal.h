#ifndef GAME_ACT_ACT_ITEM_PEDESTAL_H
#define GAME_ACT_ACT_ITEM_PEDESTAL_H

// Code: game/act_item_pedestal.cpp.

#include "game/ActorBase.h"

// The ITMS actor ("Item pedestal", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203688, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A82A8.
class ItemPedestal_804A82A8 : public ActorBase_804A5770 {
public:
    ItemPedestal_804A82A8() {}
    /* 0x08 */ virtual ~ItemPedestal_804A82A8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x802AC09C (not written yet): stores 0 to the word at 0x238. Called by
    // act_stop_npc (0x802ED198) on each ITMS actor.
    void fn_802AC09C();

    /* 0x230 */ s32 field_0x230; // state: 0 waiting, 1 waiting for the player to leave
    /* 0x234 */ s32 field_0x234; // player (bits 16-19 of field_0x90)
    /* 0x238 */ s32 field_0x238; // countdown
    /* 0x23C */ s32 field_0x23c; // bit 4 of field_0x90: stay after use
    /* 0x240 */ s32 field_0x240; // the player's group leader
}; // Size: 0x244

#endif
