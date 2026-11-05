#ifndef GAME_ACT_ACT_TINGLE_WALLET_H
#define GAME_ACT_ACT_TINGLE_WALLET_H

// Code: game/act_tingle_wallet.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A3948.
class TinglesWallet_804A3948 : public ActorBase_804A5770 {
public:
    TinglesWallet_804A3948() {}
    /* 0x08 */ virtual ~TinglesWallet_804A3948();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state: 0 waiting, 1 falling, 2 landed
    /* 0x234 */ s32 mTimers[3]; // counted down to 0 by update
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ f32 field_0x24c; // update eases field_0x3c.x towards this
    /* 0x250 */ f32 field_0x250; // ... and field_0x3c.y towards this
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258; // draw's scale matrix x
    /* 0x25C */ f32 field_0x25c; // y and z
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264; // update eases field_0x258 towards this
    /* 0x268 */ f32 field_0x268; // ... and field_0x25c towards this
    /* 0x26C */ f32 field_0x26c;
}; // Size: 0x270

#endif
