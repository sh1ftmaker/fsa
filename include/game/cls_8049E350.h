#ifndef GAME_CLS_8049E350_H
#define GAME_CLS_8049E350_H

// Code: game/tu_80125B58.cpp. An actor named after its vtable, 0x8049E350
// (__vt__12cls_8049E350; its constructor is not written yet). It has two
// SpriteAnim members: mAnim244 (its J3DFrameCtrl at 0x268) and mAnim27c (its
// J3DFrameCtrl at 0x2A0). The destructor (0x80125B58) destroys both.
// Size: at least 0x2B4 (the end of mAnim27c).

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

class cls_8049E350 : public ActorBase_804A5770 {
public:
    cls_8049E350() : field_0x230(0), field_0x231(0), field_0x234(0), field_0x238(0), field_0x23c(-1), field_0x240(-1) {}
    /* 0x08 */ virtual ~cls_8049E350();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x80125BF4 (not virtual): the effect 0x33A at mPos with y lowered by
    // 24, on this actor's layer, scale 1.0.
    void fn_80125BF4();
    // 0x80125C58: stores the flag at 0x231. When it is set, plays animation
    // 0 (field_0x238 == 1) or 1 on mAnim27c and sets its frame to 0.
    void fn_80125C58(bool flag);
    // The state switch of init() and update() (defined inline in tu_80125B58.cpp).
    void enterState();

    /* 0x230 */ u8 field_0x230; // set by init's state switch (0 or 1)
    /* 0x231 */ bool field_0x231; // set by fn_80125C58
    /* 0x232 */ u8 field_0x232[0x234 - 0x232]; // not yet known
    /* 0x234 */ s32 field_0x234; // state: init sets 1, then switches on it
    /* 0x238 */ s32 field_0x238; // field_0x90 & 1; 1 selects animation 0 of mAnim27c in fn_80125C58
    /* 0x23C */ s32 field_0x23c; // -1 after state 1
    /* 0x240 */ s32 field_0x240; // (field_0x90 >> 22) & 0x1F: the view bit of state 2

    /* 0x244 */ SpriteAnim_801D267C mAnim244; // mCel (0x254) is the frame in fn_80125CDC
    /* 0x27C */ SpriteAnim_801D267C mAnim27c;
}; // Size: at least 0x2B4

#endif
