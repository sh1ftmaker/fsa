#ifndef GAME_ACT_ACT_PRINCESS_ZELDA_ENDING_H
#define GAME_ACT_ACT_PRINCESS_ZELDA_ENDING_H

// Code: game/act_princess_zelda_ending.cpp.

#include "game/ActorBase.h"

// The base of the ending actors EZLD, ELNK (game/act_link_ending.cpp) and
// EMIK (game/act_maiden_ending.cpp), with the vtable at 0x804A5958 (0x58
// bytes): their destructors store this vtable and then call ActorBase's. The
// fields below are the ones they share: EZLD and ELNK hold each other's
// serial in field_0x23c and set each other's mState, field_0x234 and
// field_0x238 (fn_8035DF48, fn_8035EA48), and ENPC sets field_0x238 of both.
class cls_804A5958 : public ActorBase_804A5770 {
public:
    cls_804A5958() {}
    virtual ~cls_804A5958() {}

    // The animation step sequencer, called only by EZLD and EMIK on
    // themselves; its code and tables (lbl_804AD460) sit in
    // game/act_enemy_for_ending_control.cpp. fn_8035C15C advances sequence
    // `seq` and returns true once it is over; fn_8035C1C4 restarts it with
    // base frame `frame`.
    bool fn_8035C15C(s32 seq);
    void fn_8035C1C4(s32 seq, s32 frame);

    /* 0x230 */ s32 mState;      // index into the subclass's state table
    /* 0x234 */ s32 field_0x234; // countdown, decremented by update()
    /* 0x238 */ s32 field_0x238; // go flag / sub-step counter
    /* 0x23C */ s32 field_0x23c; // serial of the partner actor (fn_801F666C)
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244; // second countdown (sequencer timer)
    /* 0x248 */ s32 field_0x248; // frame offset (sequencer step value)
    /* 0x24C */ s32 field_0x24c; // sequencer step index
    /* 0x250 */ s32 field_0x250; // base frame of the animation
}; // Size: 0x254

// The EZLD actor ("Princess Zelda (ending)", docs/names/actors.tsv): the
// spawner constructs it with fn_80200EDC, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AD6F0.
class PrincessZeldaEnding_804AD6F0 : public cls_804A5958 {
public:
    PrincessZeldaEnding_804AD6F0() {}
    /* 0x08 */ virtual ~PrincessZeldaEnding_804AD6F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // The state functions: update() calls mStates[mState] (lbl_804AD6A8).
    void fn_8035DF48();
    void fn_8035DFE4();
    void fn_8035E070();
    void fn_8035E238();
    void fn_8035E274();
    void fn_8035E394();
}; // Size: 0x254

#endif
