#ifndef GAME_ACT_TU_801291A0_H
#define GAME_ACT_TU_801291A0_H

// Code: game/tu_801291A0.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049E5B8. It keeps one spawned item actor
// ('CRTL' crystal ball, 'LSTI' dropped item or 'NNJN' carrot) and respawns
// it a while after that one is gone; the spawn kind comes from the low
// nibble of field_0x90 and the item list from its other bits (init).
class cls_8049E5B8 : public ActorBase_804A5770 {
public:
    cls_8049E5B8() : mState(1), mKind(4), mTimer(0), mSpawned(-1), mItemCount(0), mKindCount(0), field_0x280(0) {}
    /* 0x08 */ virtual ~cls_8049E5B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    bool fn_80129200();
    void fn_8012952C(s32 state);
    /* 0x230 */ s32 mState;  // 0 waits for the spawn to go, 1 counts down, 2 respawns
    /* 0x234 */ s32 mKind;   // field_0x90 & 0xF: 0, 2 or 3 spawn a kind, 0xF picks from mKinds
    /* 0x238 */ s32 mTimer;  // counts up in state 1 (600, or 300 for kind 2)
    /* 0x23C */ s32 mSpawned; // serial of the spawned actor, -1 for none
    /* 0x240 */ s32 mItemCount;
    /* 0x244 */ s32 mKindCount;
    /* 0x248 */ s32 mItems[10]; // item ids, 10 until init fills them from field_0x90's bits
    /* 0x270 */ s32 mKinds[4];  // kinds 0..3 whose bit is set in field_0x90
    /* 0x280 */ u32 field_0x280; // spawn flags; init sets bit 1 and one bit per item
}; // Size: 0x284

#endif
