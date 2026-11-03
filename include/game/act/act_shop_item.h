#ifndef GAME_ACT_ACT_SHOP_ITEM_H
#define GAME_ACT_ACT_SHOP_ITEM_H

// Code: game/act_shop_item.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

extern "C" {
// Not named yet; a plain C symbol in the original. update() calls it after
// paying for kinds 4-7, with 9, 4, 3 or 6 as the second argument by kind and
// 1 as the third (its types are guesses from the calls).
void fn_802372F8(s32 player, s32 value, bool flag);
} // extern "C"

// The SITM actor ("Shop item", docs/names/actors.tsv, med confidence): the
// spawner constructs it with __ct__17ShopItem_8049CDC0Fv (0x801092B0), which
// stores this vtable (config/G4SE01/game_units.tsv). Named after its vtable,
// 0x8049CDC0.
class ShopItem_8049CDC0 : public ActorBase_804A5770 {
public:
    ShopItem_8049CDC0();
    /* 0x08 */ virtual ~ShopItem_8049CDC0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ SpriteAnim_801D267C mAnim;
    /* 0x268 */ s32 mKind;   // 0-7 (init() takes it from the spawn argument); the constructor sets 8
    /* 0x26C */ s32 mState;  // the state update() dispatches on, 0-6
    /* 0x270 */ s32 mPlayer; // -1 when none
    /* 0x274 */ s32 field_0x274; // zero from the constructor; not read in this file
    /* 0x278 */ JGeometry::TVec3f mMsgPos; // mPos with y - 32 (init); the message box's position
}; // Size: 0x284

#endif
