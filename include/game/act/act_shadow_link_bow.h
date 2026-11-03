#ifndef GAME_ACT_ACT_SHADOW_LINK_BOW_H
#define GAME_ACT_ACT_SHADOW_LINK_BOW_H

#include "game/cls_804A6A00.h"

// A minor Shadow Link actor (sub-base cls_804A6A00) that, after its hop
// animation, shoots volleys of 'OKTM' actors (Octorok rocks) across the
// view; code in game/act_shadow_link_bow.cpp. Not tied to a spawner ID.
// Named after its vtable (0x804A6B70). The destructor is fn_802223FC, the
// constructor fn_80222A58.
class cls_804A6B70 : public cls_804A6A00 {
public:
    cls_804A6B70();
    virtual ~cls_804A6B70();
    /* 0x0C */ virtual void init();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);

    // 0x80222668: sets up the animation / timer of `state` (3, 5, 6, 7),
    // then clears mStateTimer and stores the state.
    void fn_80222668(s32 state);
    // 0x80222730: spawns 'OKTM' actors along the bottom of the view's box.
    void fn_80222730();

    // Finished volleys; the third one ends the cycle (state 7).
    /* 0x2B8 */ s32 mRounds;
    // Shots fired (state 6 entries); every tenth ends a round.
    /* 0x2BC */ s32 mShots;
}; // Size: 0x2C0

#endif
