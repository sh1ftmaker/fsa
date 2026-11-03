#ifndef GAME_ACT_ACT_BANDIT_HERDING_NPC_H
#define GAME_ACT_ACT_BANDIT_HERDING_NPC_H

// Code: game/act_bandit_herding_npc.cpp.

#include "game/ActorBase.h"

// The TOOI actor ("Bandit herding NPC", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_8020034C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804ACB40.
class BanditHerdingNPC_804ACB40 : public ActorBase_804A5770 {
public:
    BanditHerdingNPC_804ACB40() {}
    /* 0x08 */ virtual ~BanditHerdingNPC_804ACB40();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[4]; // not yet known
    /* 0x234 */ s32 field_0x234;    // init(): field_0x90 & 0xFF; how many TZOK are to be herded in
    /* 0x238 */ s32 field_0x238;    // update(): how many TZOK are inside the box this frame
    /* 0x23C */ s32 field_0x23c;    // init(): (field_0x90 >> 22) & 0x1F; bit passed to GameView::fn_802D800C
    /* 0x240 */ JGeometry::TBox2f field_0x240; // init(): mBox moved to mPos; the area TZOK must be in
}; // Size: 0x250

#endif
