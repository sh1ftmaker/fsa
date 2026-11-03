#ifndef GAME_ACT_ACT_CHAIN_SWITCH_H
#define GAME_ACT_ACT_CHAIN_SWITCH_H

// Code: game/act_chain_switch.cpp.

#include "game/ActorBase.h"

// The CHSW actor ("Chain switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80200388, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA3E8.
class ChainSwitch_804AA3E8 : public ActorBase_804A5770 {
public:
    ChainSwitch_804AA3E8() {}
    /* 0x08 */ virtual ~ChainSwitch_804AA3E8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mTimer;      // frames left before the action fires
    /* 0x234 */ bool mWasOn;     // vfunc_0x34() at the last check
    /* 0x238 */ s32 mDelay;      // field_0x90 & 0x1FFFF
    /* 0x23C */ u8 mCallWhenDone; // bit 17: run vfunc_0x24() when the timer ends
    /* 0x23D */ u8 mSwitchMode;  // bits 18-19: 0 sets the switch bit, 1 clears it
    /* 0x23E */ u8 mSoundMode;   // bits 20-21: 1 calls vfunc_0x30(0xFF, true), 2 (0xFF, false)
    /* 0x240 */ u16 mSwitchBit;  // bits 22-26: bit passed to fn_802D8050
    /* 0x242 */ u16 field_0x242; // bits 27-31 (vfunc_0x3C())
}; // Size: 0x244

#endif
