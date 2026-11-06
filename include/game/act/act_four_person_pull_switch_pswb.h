#ifndef GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_PSWB_H
#define GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_PSWB_H

// Code: game/act_four_person_pull_switch_pswb.cpp.

#include "game/ActorBase.h"

// The PSWB actor ("Four-person pull switch (BTI)", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_801FFB78, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AE574.
class FourPersonPullSwitchBTIVersion_804AE574 : public ActorBase_804A5770 {
public:
    FourPersonPullSwitchBTIVersion_804AE574() {}
    /* 0x08 */ virtual ~FourPersonPullSwitchBTIVersion_804AE574();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_80373198();

    /* 0x230 */ JGeometry::TVec3f field_0x230; // mPos with y + 16 when pulled
    /* 0x23C */ s32 field_0x23c;               // texture frame index (0..4)
    /* 0x240 */ u8 field_0x240;                // set by init from vfunc_0x34; 1 once fully pulled
    /* 0x244 */ f32 field_0x244;               // pull height (0..14)
    /* 0x248 */ s32 field_0x248;               // countdown timer
    /* 0x24C */ s32 field_0x24c;               // state: -1 unpulled, 0 waiting for pull, 4 pulling animation
    /* 0x250 */ s32 field_0x250;               // animation stage step
    /* 0x254 */ u8 field_0x254;                // initial state from vfunc_0x34
}; // Size: 0x258

#endif
