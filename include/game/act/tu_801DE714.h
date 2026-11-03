#ifndef GAME_ACT_TU_801DE714_H
#define GAME_ACT_TU_801DE714_H

#include "types.h"
#include "game/EnemyBase.h"

// The TINK actor (spawner fn_801F9484; constructor fn_801E22A4, vtable
// 0x804A51E4), code in game/tu_801DE714.cpp. Named after its vtable.
// Only the members the written code uses are known. The constructor
// (0x801E22A4) writes -1 to 0x2A8-0x2BC and 0x2D0, plus other fields up to
// 0x2D4.
// Size: unknown (>= 0x2D4)
class cls_804A51E4 : public EnemyBase_804A4808 {
public:
    cls_804A51E4() {}

    // tu_801DE714 (0x801E1D9C): whether mAnim's current animation is 2.
    bool fn_801E1D9C();
    // tu_801DE714 (0x801E1DE8): calls fn_801CCF7C (r3 is not forwarded; it
    // ignores it) in a framed `bl`; callers test r3 with clrlwi. (a bool).
    bool fn_801E1DE8();
    // tu_8021827C (0x8021827C), the last slot of the vtable (EnemyBase's
    // vfunc_0xA0).
    void vfunc_0xA0();
    // tu_8021827C (0x8021835C): called from vfunc_0x9C's override (0x801DE9C0).
    void fn_8021835C();
    // tu_8021827C (0x80218538): whether the actor's box is in view.
    bool fn_80218538();
    // tu_8021FDB8 (0x8021FDB8): moves the actor found by mTargetSerial next
    // to this one, offset by mAnim.mCel (z) and the target's field_0x90 and
    // our direction field_0x98 (x).
    void fn_8021FDB8();

    /* 0x2A8 */ s32 field_0x2a8;
    /* 0x2AC */ s32 field_0x2ac;
    /* 0x2B0 */ u8 field_0x2b0[0xC];
    /* 0x2BC */ s32 mTargetSerial; // serial of the actor fn_8021FDB8 moves
};

#endif
