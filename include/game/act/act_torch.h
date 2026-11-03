#ifndef GAME_ACT_ACT_TORCH_H
#define GAME_ACT_ACT_TORCH_H

// Code: game/act_torch.cpp.

#include "game/ActorBase.h"

// The TRCH actor ("Torch", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203430, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8520.
class Torch_804A8520 : public ActorBase_804A5770 {
public:
    Torch_804A8520() {}
    /* 0x08 */ virtual ~Torch_804A8520();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ JPABaseEmitter* mEmitter230; // effect 0xBD while lit
    /* 0x234 */ JPABaseEmitter* mEmitter234; // effect 0x54A while lit (not for type 4)
    /* 0x238 */ s32 mState;                  // 0: unlit, 1: lit
    /* 0x23C */ s32 mType;                   // field_0x90 & 0x7F; 0..3 draw a shadow, 0/2/4 spawn the flame
    /* 0x240 */ s32 field_0x240;             // not used by this unit
    /* 0x244 */ s32 mViewBit;                // field_0x90 >> 27, tested with GameView::fn_802D800C (0: always)
    /* 0x248 */ u8 mOn;                      // 1 while lit by the view bit
    /* 0x24C */ s32 field_0x24c;             // 1 while lit
    /* 0x250 */ f32 mShadowScale;            // 0..mShadowMax, drawn as the shadow scale
    /* 0x254 */ f32 mShadowMax;              // 70 (types 0, 2) or 20
}; // Size: 0x258

#endif
