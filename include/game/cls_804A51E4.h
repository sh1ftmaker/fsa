#ifndef GAME_CLS_804A51E4_H
#define GAME_CLS_804A51E4_H

#include "types.h"
#include "game/EnemyBase.h"

// The TINK actor (spawner fn_801F9484; constructor fn_801E22A4, vtable
// 0x804A51E4), code in game/tu_801DE714.cpp. Named after its vtable.
// Only the members the written code uses are known: the constructor
// (0x801E22A4) sets 0x2A8 and 0x2AC to -1. The size is not established yet.
class cls_804A51E4 : public EnemyBase_804A4808 {
public:
    cls_804A51E4() {}

    // tu_801DE714 (0x801E1D9C): whether mAnim's current animation is 2.
    bool fn_801E1D9C();
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
