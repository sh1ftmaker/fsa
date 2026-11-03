#ifndef GAME_ACT_ACT_MERCHANT_H
#define GAME_ACT_ACT_MERCHANT_H

// Code: game/act_merchant.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The DEAL actor ("Merchant", docs/names/actors.tsv, med confidence): a
// shop NPC drawing one "SOB " sprite and opening a message box when a
// player stands in front of it. The spawner constructs it with fn_801087F4,
// which stores this vtable (config/G4SE01/game_units.tsv). Named after its
// vtable, 0x8049CD48.
class Merchant_8049CD48 : public ActorBase_804A5770 {
public:
    Merchant_8049CD48();
    /* 0x08 */ virtual ~Merchant_8049CD48();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState; // 0 waiting for a player, 1 and 3 message open
    /* 0x234 */ s32 mPlayer; // fn_8022FD8C(mSerial); -1 when none
    /* 0x238 */ SpriteAnim_801D267C mAnim;
}; // Size: 0x270

#endif
