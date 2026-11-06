#ifndef GAME_ACT_ACT_KAEPORA_GAEBORA_H
#define GAME_ACT_ACT_KAEPORA_GAEBORA_H

// Kaepora Gaebora. Code: game/act_kaepora_gaebora.cpp (partly written),
// and its drawing and effect helpers in game/tu_8021AF28.cpp.

#include "game/EnemyBase.h"
#include "game/tu_8021A570.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Named after its vtable, 0x804A6550 (an EnemyBase_804A4808 descendant).
// The overrides below are the vtable slots that differ from EnemyBase's;
// init, vfunc_0x90 and vfunc_0xA0 are not written yet (draw() is in tu_8021AF28).
class cls_804A6550 : public EnemyBase_804A4808 {
public:
    cls_804A6550();
    /* 0x08 */ virtual ~cls_804A6550();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x5C */ virtual u8 vfunc_0x5C();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x94 */ virtual void vfunc_0x94();
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    /* 0xA0 */ virtual void vfunc_0xA0();

    // tu_8021AF28 (the sprite and effect helpers draw() and the actor's
    // states call).
    // 0x80219AFC (act_kaepora_gaebora): switches to `state`: the exit and entry
    // actions of the old and new state, then mStateTimer = 0 and mState = state.
    void fn_80219AFC(s32 state);
    // 0x80219E04 (act_kaepora_gaebora): true in states 6-10 and in state 2
    // while field_0x2b4 == 3; the sprites and effects then move by -6.
    bool fn_80219E04() const;
    // Draws the flipped sprite with the 0x25C + mAnim.mCel frame.
    void fn_8021AFB8();
    // Draws the body (frame 0x279) and the rotated head (frame 0x27A).
    void fn_8021B1AC();
    // fn_801CB71C on the emitter at this actor's position.
    void fn_8021B548();
    // fn_801CB7A0 on the emitter.
    void fn_8021B5A0();
    // Replaces the emitter with effect 0x344.
    void fn_8021B5C4();
    // Effect 0x343 at this actor's position (not kept).
    void fn_8021B668() const;

    /* 0x2A8 */ s32 field_0x2a8;
    /* 0x2AC */ f32 field_0x2ac;
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
    /* 0x2B8 */ s32 field_0x2b8;
    /* 0x2BC */ JGeometry::TVec2f field_0x2bc;
    /* 0x2C4 */ JPABaseEmitter* mEmitter;
    /* 0x2C8 */ cls_8021ACE0 mHead; // swinging head; mHead.mAngle is the head's yaw
    /* 0x2D8 */ f32 field_0x2d8;
}; // Size: 0x2DC

#endif
