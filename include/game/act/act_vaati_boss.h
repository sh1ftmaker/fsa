#ifndef GAME_ACT_ACT_VAATI_BOSS_H
#define GAME_ACT_ACT_VAATI_BOSS_H

// Code: game/act_vaati_boss.cpp (not written yet); the draw helpers are in
// game/tu_801EDBA4.cpp.

#include "types.h"
#include "game/EnemyBase.h"

// The object at 0x52c of Vaati (named after its first function, 0x801D6CEC,
// in game/tu_801D6CEC.cpp, not written yet): fn_801D6CEC reads the floats at
// 0x0 and 0x14. The rest is unknown.
struct Sub52c_801D6CEC {
    // Writes (0x0, 0x14, 0.0f) to *out.
    void fn_801D6CEC(JGeometry::TVec3f* out);
    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ u8 field_0x4[0x14 - 0x4];
    /* 0x14 */ f32 field_0x14;
}; // Size: unknown (at least 0x18)

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
    // a matrix scaled in z by 1.0f + field_0x554 (at least 0.001f); in
    // state 5 it draws it once more first and scales the matrix by a
    // height-dependent factor (fn_802F5FF4 of mPos.z).
    void fn_801EDD0C();

    /* 0x2A8 */ u8 field_0x2a8[0x52C - 0x2A8]; // not yet known
    /* 0x52C */ Sub52c_801D6CEC field_0x52c;
    /* 0x544 */ u8 field_0x544[0x554 - 0x544]; // not yet known
    /* 0x554 */ f32 field_0x554;
    /* 0x558 */ u8 field_0x558[0x5D1 - 0x558]; // not yet known
    /* 0x5D1 */ u8 field_0x5d1;
};

#endif
