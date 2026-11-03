#ifndef GAME_ACT_ACT_SPOTLIGHT_H
#define GAME_ACT_ACT_SPOTLIGHT_H

// Code: game/act_spotlight.cpp.

#include "game/ActorBase.h"

// The SPOT actor ("Spotlight", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802034A8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A84C8.
class Spotlight_804A84C8 : public ActorBase_804A5770 {
public:
    Spotlight_804A84C8() {}
    /* 0x08 */ virtual ~Spotlight_804A84C8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;                  // 0: off, 1: on
    /* 0x234 */ JPABaseEmitter* mEmitter234; // effect 0xBD while on
    /* 0x238 */ JPABaseEmitter* mEmitter238; // effect 0xBE while on
    /* 0x23C */ JPABaseEmitter* mEmitter23c; // effect 0xBF while on
    /* 0x240 */ JPABaseEmitter* mEmitter240; // effect 0xC0 while on
    /* 0x244 */ JPABaseEmitter* mEmitter244; // effect 0x54A while on
    /* 0x248 */ s32 mViewBit0;               // (field_0x90 >> 22) & 0x1F, tested with GameView::fn_802D800C
    /* 0x24C */ s32 mViewBit1;               // (field_0x90 >> 17) & 0x1F
    /* 0x250 */ s32 mViewBit2;               // (field_0x90 >> 12) & 0x1F
    /* 0x254 */ s32 mOn;                     // 1 while lit, selects the shadow fade direction
    /* 0x258 */ f32 mShadowScale;            // 0..200, drawn as the shadow scale
}; // Size: 0x25C

#endif
