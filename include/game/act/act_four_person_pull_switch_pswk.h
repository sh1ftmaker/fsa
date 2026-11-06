#ifndef GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_PSWK_H
#define GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_PSWK_H

// Code: game/act_four_person_pull_switch_pswk.cpp.

#include "game/ActorBase.h"

// The PSWK actor ("Four-person pull switch (chain version)", docs/names/actors.tsv,
// med confidence): the spawner constructs it with fn_801FFB00, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AE5C8.
class FourPersonPullSwitchChainVersion_804AE5C8 : public ActorBase_804A5770 {
public:
    FourPersonPullSwitchChainVersion_804AE5C8() {}
    /* 0x08 */ virtual ~FourPersonPullSwitchChainVersion_804AE5C8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_80373C38();

    /* 0x230 */ JGeometry::TVec3f field_0x230; // mPos with y + 16 when pulled
    /* 0x23C */ s32 field_0x23c;               // sprite frame offset (0, 4..7)
    /* 0x240 */ u8 field_0x240;                // set by init from vfunc_0x34; 1 once fully pulled
    /* 0x244 */ f32 field_0x244;               // pull distance (0..40)
    /* 0x248 */ s32 field_0x248;               // countdown timer
    /* 0x24C */ s32 field_0x24c;               // state: -1 unpulled, 0 waiting for pull, 4 pulling animation
    /* 0x250 */ s32 field_0x250;               // animation stage step
    /* 0x254 */ u8 field_0x254;                // initial state from vfunc_0x34
}; // Size: 0x258

#endif
