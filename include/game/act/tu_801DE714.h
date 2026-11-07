#ifndef GAME_ACT_TU_801DE714_H
#define GAME_ACT_TU_801DE714_H

#include "types.h"
#include "game/EnemyBase.h"

// The TINK actor (spawner spawnActor_801F9484; constructor fn_801E22A4, vtable
// 0x804A51E4), code in game/tu_801DE714.cpp. Named after its vtable.
// Only the members the written code uses are known. The constructor
// (0x801E22A4, __ct__12cls_804A51E4Fv) writes -1 to 0x2A8-0x2BC and 0x2D0,
// zeroes 0x2C0, 0x2CC and 0x2CE and sets the TVec2f at 0x2C4 to 0.
// Size: unknown (>= 0x2D4)
class cls_804A51E4 : public EnemyBase_804A4808 {
public:
    // 0x801E22A4: the spawner (tu_801F9484) calls it after operator new(0x2D4).
    cls_804A51E4();
    // 0x801E2180 (vtable slot 0x08, the deleting destructor): gives back the
    // count in field_0x2c0 to the save row and clears mAlive of the actor
    // mTargetSerial names, then EnemyBase's destructor.
    /* 0x08 */ virtual ~cls_804A51E4();

    // tu_8020CCA8 (0x8020CCA8, vtable slot 0x20).
    void draw();
    // tu_8020CCA8 (0x8020CF7C): an effect (id 0x48D) at mPos.
    void fn_8020CF7C() const;
    // tu_8020CCA8 (0x8020CFC4): effects and a sound for the tile under mPos.
    void fn_8020CFC4() const;
    // tu_8020CCA8 (0x8020D16C): the same for the tile under mPos (0x39 or
    // the fn_80224B88 / fn_8022517C / fn_80224E88 tile tests).
    void fn_8020D16C() const;
    // tu_8020CCA8 (0x8020D2AC): an effect (id 0x33A) beside mPos, 26 below
    // it in y and 6 to the side the actor faces (field_0x98).
    void fn_8020D2AC() const;
    // tu_801DE714 (0x801DE714, vtable slot 0x80): calls EnemyBase's
    // vfunc_0x80 (the floor check) only while mState is 0xA.
    void vfunc_0x80();
    // tu_801DE714 (0x801DE9C0, vtable slot 0x9C): EnemyBase's fn_801CE2BC
    // (the step without the hit-bit test) with mapHit passed on, then
    // fn_8021835C.
    void vfunc_0x9C(u8 mapHit);
    // tu_801DE714 (0x801E2004, vtable slot 0x54): removes the other live actors of mActorId
    // (ActorManager fn_801F7AB8, this one excepted by serial), then sets mPos.x
    // from the visible box's left edge (the second box from layer 8 up) and
    // copies mPos.x and mPos.y to field_0x2c4.
    /* 0x54 */ virtual void vfunc_0x54();
    // tu_801DE714 (0x801E1D7C): whether mState is 0xB or 0xC.
    bool fn_801E1D7C();
    // tu_801DE714 (0x801E1D9C): whether mAnim's current animation is 2.
    bool fn_801E1D9C() const;
    // tu_801DE714 (0x801E1DE8): calls fn_801CCF7C (r3 is not forwarded; it
    // ignores it) in a framed `bl`; callers test r3 with clrlwi. (a bool).
    bool fn_801E1DE8();
    // tu_8021827C (0x8021827C), the last slot of the vtable (EnemyBase's
    // vfunc_0xA0).
    void vfunc_0xA0();
    // tu_8021827C (0x8021835C): called from vfunc_0x9C's override (0x801DE9C0).
    void fn_8021835C();
    // tu_8021827C (0x80218538): whether the actor's box is in view.
    bool fn_80218538() const;
    // tu_801E2540 (this unit's vtable slots 0x5C / 0x60 / 0x64 and helpers, 0x801E2540-0x801E2F40).
    /* 0x5C */ virtual u8 vfunc_0x5C();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x64 */ virtual u8 vfunc_0x64();
    // 0x801E2D88: sets the boxes for the current state.
    void fn_801E2D88();
    // 0x801E2EE4: registers the actor's box (8 around mPos) with the tile map.
    void fn_801E2EE4();
    // 0x801E2F40: whether the tile under mPos is 3, 13 or 14.
    bool fn_801E2F40();
    // Not written yet (tu_801DE714.cpp).
    bool fn_801E1CF8();
    void fn_801E1594(s32 state);
    bool fn_801E1DB4();
    // tu_8021FDB8 (0x8021FDB8): moves the actor found by mTargetSerial next
    // to this one, offset by mAnim.mCel (z) and the target's field_0x90 and
    // our direction field_0x98 (x).
    void fn_8021FDB8() const;

    /* 0x2A8 */ s32 field_0x2a8;
    /* 0x2AC */ s32 field_0x2ac;
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
    /* 0x2B8 */ s32 field_0x2b8;
    /* 0x2BC */ s32 mTargetSerial; // serial of the actor fn_8021FDB8 moves
    /* 0x2C0 */ s32 field_0x2c0;
    /* 0x2C4 */ JGeometry::TVec2f field_0x2c4; // vfunc_0x54: set(mPos.x, mPos.y)
    /* 0x2CC */ s16 field_0x2cc;
    /* 0x2CE */ s16 field_0x2ce;
    /* 0x2D0 */ s32 field_0x2d0;
};

#endif
