#ifndef GAME_ACT_ACT_PRINCESS_ZELDA_ENDING_H
#define GAME_ACT_ACT_PRINCESS_ZELDA_ENDING_H

// Code: game/act_princess_zelda_ending.cpp.

#include "game/ActorBase.h"

// The class between EZLD and ActorBase, with the vtable at 0x804A5958 (0x58
// bytes; its own code is not known yet). EZLD's destructor stores its vtable
// and then calls ActorBase's destructor, the way the inlined destructor of
// this class does.
//
// EZLD and ELNK (act_link_ending) lay out 0x230-0x253 identically and the
// maidens (EMIK, act_maiden_ending) call the same step sequencer on
// themselves, so these fields and the sequencer belong to this class. The
// sequencer's code is in game/act_enemy_for_ending_control.cpp.
class cls_804A5958 : public ActorBase_804A5770 {
public:
    cls_804A5958() {}
    virtual ~cls_804A5958() {}

    // 0x8035C15C: the step sequencer. When the step timer (0x244) is 0, reads
    // step 0x24C of table `seq` (lbl_804AD460): returns true at its end
    // marker, else loads the timer and the value (0x248) and advances.
    bool fn_8035C15C(s32 seq);
    // 0x8035C1C4: restarts the sequencer on table `seq` with `arg` in 0x250.
    void fn_8035C1C4(s32 seq, s32 arg);

    /* 0x230 */ s32 mState;      // index into the derived class's state table
    /* 0x234 */ s32 field_0x234; // countdown, decremented by update()
    /* 0x238 */ s32 field_0x238; // go flag, set by the partner actor
    /* 0x23C */ s32 field_0x23c; // serial of the partner actor (findActorBySerial_801F666C)
    /* 0x240 */ s32 field_0x240; // EZLD: 0 or 1, picks the SOB file in draw()
    /* 0x244 */ s32 field_0x244; // sequencer step timer, decremented by update()
    /* 0x248 */ s32 field_0x248; // sequencer step value (the frame offset)
    /* 0x24C */ s32 field_0x24c; // sequencer step index
    /* 0x250 */ s32 field_0x250; // base frame; fn_8035C1C4's `arg`
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
