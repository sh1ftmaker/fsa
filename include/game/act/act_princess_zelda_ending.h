#ifndef GAME_ACT_ACT_PRINCESS_ZELDA_ENDING_H
#define GAME_ACT_ACT_PRINCESS_ZELDA_ENDING_H

// Code: game/act_princess_zelda_ending.cpp.

#include "game/ActorBase.h"

// The partner actor that fn_801F666C(field_0x23c) returns. Its class is not
// identified yet (no vtable, constructor or name in the repo): states 4 and 5
// write its state word and timer at 0x230 and 0x234, as they write ours, so
// it is viewed as this until its class is known.
class EndingPartnerView : public ActorBase_804A5770 {
public:
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    // Set to 1 by LinkEnding_804AD858::fn_8035EAA4 (game/act_link_ending.cpp);
    // read by this actor's state 0 (field_0x238 of EZLD itself).
    /* 0x238 */ s32 field_0x238;
};

// The class between EZLD and ActorBase, with the vtable at 0x804A5958 (0x58
// bytes; its own code is not known yet). EZLD's destructor stores its vtable
// and then calls ActorBase's destructor, the way the inlined destructor of
// this class does.
class cls_804A5958 : public ActorBase_804A5770 {
public:
    cls_804A5958() {}
    virtual ~cls_804A5958() {}
};

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

    /* 0x230 */ s32 mState;           // index into the state table, 0-5
    /* 0x234 */ s32 field_0x234;      // countdown, decremented by update()
    /* 0x238 */ s32 field_0x238;      // sub-step counter within a state
    /* 0x23C */ s32 field_0x23c;      // serial of the partner actor (fn_801F666C)
    /* 0x240 */ s32 field_0x240;      // 0 or 1: picks the SOB file in draw()
    /* 0x244 */ s32 field_0x244;      // second countdown, decremented by update()
    /* 0x248 */ s32 field_0x248;      // frame offset added to field_0x250 in draw()
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;      // base frame of the SOB animation; init() sets 0x274
}; // Size: 0x254

#endif
