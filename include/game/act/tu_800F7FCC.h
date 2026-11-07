#ifndef GAME_ACT_TU_800F7FCC_H
#define GAME_ACT_TU_800F7FCC_H

// Code: game/tu_800F7FCC.cpp.

#include "game/ActorBase.h"

// A ring that grows from mPos and hits each player once (no vtable;
// named after its lowest-address function). cls_8049C718 has one at 0x290,
// ShadowLinkBase_804A6A00 (game/tu_80221B44.cpp) at 0x2BC.
struct cls_800F802C {
    // Grows the ring (by mTimer, 40 or 20 frames by mType) and hits the
    // active players of layer 0 within mRadius (fn_80238548, arg 0xB).
    void fn_800F802C();
    // 0x800F8210 (game/tu_800F7FCC.cpp, not written yet): starts the ring at
    // pos on `layer`; type 0-2 as in fn_800F802C. Called with 2 by
    // cls_804A6AA8 (game/tu_80221B44.cpp).
    void fn_800F8210(s32 layer, JGeometry::TVec3f pos, u8 type);

    /* 0x00 */ JGeometry::TVec3f mPos;
    /* 0x0C */ u8 mActive;
    /* 0x0D */ u8 mType;
    /* 0x10 */ f32 mRadius;
    /* 0x14 */ u16 mTimer;
    /* 0x16 */ u8 mHitMask; // bit i: player i was hit
}; // Size: 0x18

class cls_8049C718;

// Named after its vtable, 0x8049C678.
class StoneArrghusChildPuff_8049C678 : public ActorBase_804A5770 {
public:
    StoneArrghusChildPuff_8049C678() {}
    /* 0x08 */ virtual ~StoneArrghusChildPuff_8049C678();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x40 */ virtual void vfunc_0x40();
    // 0x800F8BAC / 0x800F9170 / 0x800F953C: the three ways a puff moves
    // (shotgun pass, named from the asm only).
    void fn_800F8BAC();
    void fn_800F9170();
    void fn_800F953C();
    /* 0x230 */ s32 mFrame;   // update() calls so far
    /* 0x234 */ s32 mState;   // 0 wait, 1 fn_800F953C, 2 fn_800F9170 (vfunc_0x40 sets 2)
    /* 0x238 */ s32 mStep;    // wait counter while mState == 0
    /* 0x23C */ s32 mVariant; // picks the sprite name (lbl_8049C648[mVariant + 1])
    /* 0x240 */ cls_8049C718* mParent;
    /* 0x244 */ u16 mAngle;
    /* 0x246 */ u8 mPhase;
    /* 0x247 */ u8 mIndex;   // 0x14 spawns four gems, else 1 to 2
    /* 0x248 */ u8 mFlag248; // 1: follows the parent's slots, else falls
    /* 0x249 */ u8 mFlag249; // sprite parameter 0x68 = 0xF when set
    /* 0x24A */ u8 mSoundTimer;
    /* 0x24B */ u8 field_0x24b;
    /* 0x24C */ u16 mDelay;
    /* 0x24E */ u16 mCount;
}; // Size: 0x250

// Named after its vtable, 0x8049C718 ("BWRT" per docs/names/actors.tsv: the
// Stone Arrghus boss). Field names come from the asm only (shotgun pass).
class cls_8049C718 : public ActorBase_804A5770 {
public:
    cls_8049C718() {
        mRing.mActive = 0;
    }
    /* 0x08 */ virtual ~cls_8049C718();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);

    void fn_800FA6B0(); // steps the animation tables (mAnimState)
    void fn_800FA1A4(); // keeps the two partner actors apart
    void fn_800FB070(); // state 9
    void fn_800FB32C(); // state 7
    void fn_800FB6E0(bool first); // starts a hop
    void fn_800FBA14(); // state 6
    void fn_800FBD90(); // state 5
    void fn_800FC128(); // state 4
    void fn_800FC4A8(); // state 3
    void fn_800FC984(); // state 2
    void fn_800FCCCC(); // state 0

    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 mTimer;
    /* 0x238 */ u32 mFlags; // 1 flying, 4, 8 two-player variant (spawn parameter 1), 0x10
    /* 0x23C */ JGeometry::TVec3f mHome;
    /* 0x248 */ JGeometry::TVec3f mOrigin;
    /* 0x254 */ u8 field_0x254;
    /* 0x255 */ u8 field_0x255; // hits taken
    /* 0x256 */ u8 mCooldown;
    /* 0x257 */ u8 field_0x257;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ f32 mZBase;
    /* 0x260 */ u16 field_0x260;
    /* 0x262 */ u16 field_0x262;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ u32 mWobble;
    /* 0x26C */ f32 mScale;
    /* 0x270 */ f32 mAlpha;
    /* 0x274 */ f32 mTint;
    /* 0x278 */ u8 mFrame;
    /* 0x279 */ u8 mAnimStep;
    /* 0x27A */ u8 mAnimTime;
    /* 0x27C */ u32 mAnimState;
    /* 0x280 */ u32 mAnimPrev;
    /* 0x284 */ s16 mSlotUsed[2];
    /* 0x288 */ s32 mPartner[2];
    /* 0x290 */ cls_800F802C mRing;
    /* 0x2A8 */ f32 mRadius;
    /* 0x2AC */ u8 mSlot;
    /* 0x2AD */ u8 mSignal;
    /* 0x2AE */ u16 mAngle;
    /* 0x2B0 */ u16 mAngleSpeed;
    /* 0x2B2 */ u16 mAngleA;
    /* 0x2B4 */ u16 mAngleB;
    /* 0x2B6 */ u8 field_0x2b6[2];
}; // Size: 0x2B8

// game/tu_800F7FCC.cpp (0x800F82CC): moves `cur` toward `target` by `step`; true when it arrived.
bool fn_800F82CC(JGeometry::TVec3f* cur, const JGeometry::TVec3f* target, f32 step);

#endif
