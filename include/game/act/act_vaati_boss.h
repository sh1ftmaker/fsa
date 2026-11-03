#ifndef GAME_ACT_ACT_VAATI_BOSS_H
#define GAME_ACT_ACT_VAATI_BOSS_H

// Owner: game/act_vaati_boss.cpp (not written yet). The draw helpers are in
// game/tu_801EDBA4.cpp, the member at 0x568 in game/tu_801E667C.cpp.

#include "types.h"
#include "game/EnemyBase.h"
#include "game/SpriteAnim.h"

// The shake offset at 0x52c of Vaati: three DampedSine_801E4A18
// (game/tu_801E4A18.cpp, 0x14 bytes each, `value` first) for x, y and z.
// The actor's constructor (0x801D686C) calls fn_801E4B0C on 0x52c, 0x540 and
// 0x554 and fn_801D4ED4 steps them with fn_801E4A18. No vtable, ctor or dtor:
// named after its lowest-address function, 0x801D6CEC (game/tu_801D6CEC.cpp,
// not written yet; fn_801D6D08 there starts the z sine at 0x28).
struct cls_801D6CEC {
    // Writes (x value, y value, 0.0f) to *out.
    void fn_801D6CEC(JGeometry::TVec3f* out);
    /* 0x00 */ f32 field_0x0; // x DampedSine_801E4A18::value
    /* 0x04 */ u8 field_0x4[0x14 - 0x4];
    /* 0x14 */ f32 field_0x14; // y DampedSine_801E4A18::value
    /* 0x18 */ u8 field_0x18[0x28 - 0x18];
    /* 0x28 */ f32 field_0x28; // z DampedSine_801E4A18::value
    /* 0x2C */ u8 field_0x2c[0x3C - 0x2C];
}; // Size: 0x3C

// The member at 0x568 of the Vaati boss actor (code in game/tu_801E667C.cpp,
// called from the actor's functions at 0x801D5B58, 0x801D6398 and its
// constructor, 0x801D6904); named after its constructor, 0x801E6AB8 (no
// vtable). A short timed sequence that plays an animation on the players in
// mActive and ends after state 4. mState steps 0 -> 1 -> 2 -> 3 -> 4.
class cls_801E6AB8 {
public:
    cls_801E6AB8();
    // 0x801E667C: steps the timer and the state; true once state 4 is reached.
    bool fn_801E667C();
    // 0x801E67C4: enters state `state`.
    void fn_801E67C4(s32 state);
    // 0x801E6A84: starts the sequence (state 1) if it is idle.
    void fn_801E6A84(s32 arg);

    /* 0x00 */ s32 mState;
    /* 0x04 */ s32 mTimer;
    /* 0x08 */ s32 field_0x8; // set by fn_801E6A84; state 4 acts only when 0
    /* 0x0C */ u8 mActive[4]; // per player
    /* 0x10 */ SpriteAnim_801D267C mAnim;
}; // Size: 0x48

// The boss Vaati (actor 'BOGF', EnemyBase_804A4808 child), named after its
// vtable at 0x804A4958 (its draw is slot 0x20, fn_801EDBA4). Only the members
// the written code uses are known.
class VaatiBoss_804A4958 : public EnemyBase_804A4808 {
public:
    VaatiBoss_804A4958() {}
    /* 0x20 */ virtual void draw();

    // 0x801EDC0C (draw's body): adds the offset of field_0x52c to mPos,
    // shakes mPos.x by 1.0f every other frame in state 0xB once the timer is
    // 0x3C or more, then draws "enemy_both_sob.bin" frame `frame`
    // (fn_801F02BC) on the lower layers, else fn_801EDD0C.
    void fn_801EDC0C(s32 frame);
    // 0x801EDD0C: draws the 'TIMG' resource e_gufu_<mAnim.mCel + 1>.bti with
    // a matrix scaled in z by 1.0f + the z shake (at least 0.001f); in
    // state 5 it draws it once more first and scales the matrix by a
    // height-dependent factor (fn_802F5FF4 of mPos.z).
    void fn_801EDD0C();

    /* 0x2A8 */ u8 field_0x2a8[0x52C - 0x2A8]; // not yet known
    /* 0x52C */ cls_801D6CEC field_0x52c;
    /* 0x568 */ cls_801E6AB8 field_0x568;
    /* 0x5B0 */ u8 field_0x5b0[0x5D1 - 0x5B0]; // not yet known
    /* 0x5D1 */ u8 field_0x5d1;
    /* 0x5D2 */ u8 field_0x5d2[0x618 - 0x5D2]; // not yet known
}; // Size: 0x618 (the spawner's __nw__, li r3 0x618 before the 0x801FC280 ctor call)

#endif
