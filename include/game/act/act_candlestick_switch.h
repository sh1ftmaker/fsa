#ifndef GAME_ACT_ACT_CANDLESTICK_SWITCH_H
#define GAME_ACT_ACT_CANDLESTICK_SWITCH_H

// Code: game/act_candlestick_switch.cpp.

#include "game/ActorBase.h"

// The LWSW actor ("Candlestick switch", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80202D04, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8B78.
class CandlestickSwitch_804A8B78 : public ActorBase_804A5770 {
public:
    CandlestickSwitch_804A8B78() {}
    /* 0x08 */ virtual ~CandlestickSwitch_804A8B78();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;                  // 0: unlit, 1: lit
    /* 0x234 */ JPABaseEmitter* mEmitter234; // effect 0xBD while lit
    /* 0x238 */ JPABaseEmitter* mEmitter238; // effect 0x54A while lit
    /* 0x23C */ JPABaseEmitter* mEmitter23c; // effect 0x57 right after going out
    /* 0x240 */ s32 mMode;                   // (field_0x90 >> 22) & 0x1F: 1 goes out by itself
    /* 0x244 */ s32 mLitTime;                // field_0x90 & 0xFFF, frames it stays lit
    /* 0x248 */ s32 mOutTimer;               // frames the going-out effect runs
    /* 0x24C */ s32 mLitTimer;               // counts down from mLitTime
    /* 0x250 */ s32 mLit;                    // 1 while lit, selects the shadow fade direction
    /* 0x254 */ f32 mShadowScale;            // 0..70, drawn as the shadow scale
}; // Size: 0x258

#endif
