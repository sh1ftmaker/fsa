#ifndef GAME_ACT_ACT_LINK_ENDING_H
#define GAME_ACT_ACT_LINK_ENDING_H

// Code: game/act_link_ending.cpp.

#include "game/act/act_princess_zelda_ending.h" // cls_804A5958, EndingPartnerView

// Defined in another, unfinished unit (0x802382D0, two li 0 arguments; the
// parameter types are not known yet).
extern "C" void fn_802382D0(s32 a, s32 b);

// The ELNK actor ("Link (ending)", docs/names/actors.tsv): the spawner
// constructs it with fn_80200BB4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AD858. Its
// partner is the Princess Zelda ending actor (EZLD, game/act_princess_zelda_ending.cpp):
// the field_0x23c serial is EZLD's, and the two set each other's state and
// go flags.
class LinkEnding_804AD858 : public cls_804A5958 {
public:
    LinkEnding_804AD858() {}
    /* 0x08 */ virtual ~LinkEnding_804AD858();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // The state functions, in the order of lbl_804AD7F8 (update() calls
    // entry mState): 0 fn_8035ED60, 1 fn_8035EC98, 2 fn_8035EB90, 3
    // fn_8035EB18, 4 fn_8035EAA4, 5 fn_8035EA48, 6 fn_8035E9A0, 7 fn_8035E93C.
    void fn_8035ED60();
    void fn_8035EC98();
    void fn_8035EB90();
    void fn_8035EB18();
    void fn_8035EAA4();
    void fn_8035EA48();
    void fn_8035E9A0();
    void fn_8035E93C();

    /* 0x230 */ s32 mState;       // index into the state table
    /* 0x234 */ s32 field_0x234;  // countdown, decremented by update()
    /* 0x238 */ s32 field_0x238;  // go flag set by the partner (fn_8035E9A0)
    /* 0x23C */ s32 field_0x23c;  // serial of the partner actor (fn_801F666C)
    /* 0x240 */ s32 field_0x240;  // not read by these functions
    /* 0x244 */ s32 field_0x244;  // second countdown, decremented by update()
    /* 0x248 */ s32 field_0x248;  // animation step 0-7, wrapped by the states
    /* 0x24C */ s32 field_0x24c;  // not read by these functions
    /* 0x250 */ s32 field_0x250;  // base frame of the animation; init() sets 0x18
    // Three sprite x positions (32 + mPos.x + 32 * i), set by init().
    /* 0x254 */ f32 field_0x254[3];
    /* 0x260 */ f32 field_0x260;  // mPos.y, copied by fn_8035ED60 and fn_8035EB90
    // The sprite id base of the three side sprites (init() sets 0x18; -1
    // makes update() use the player index of lbl_80529DEC.field_0x1a4
    // instead), and an offset added to it.
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;  // zeroed by init(); not read by these functions
}; // Size: 0x270

#endif
