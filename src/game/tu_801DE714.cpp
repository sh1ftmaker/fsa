#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801DE714.h"

// .sdata2 float of this unit (no name evidence; read by the constructor).
static const f32 lbl_8053F480[1] = {0.0f};

// tu_801DE714 (0x801E22A4): the constructor, after EnemyBase's (a plain bl).
cls_804A51E4::cls_804A51E4() {
    field_0x2a8 = -1;
    field_0x2ac = -1;
    field_0x2b0 = -1;
    field_0x2b4 = -1;
    field_0x2b8 = -1;
    mTargetSerial = -1;
    field_0x2c0 = 0;
    field_0x2c4 = lbl_8053F480[0];
    field_0x2c8 = lbl_8053F480[0];
    field_0x2cc = 0;
    field_0x2ce = 0;
    field_0x2d0 = -1;
}

// tu_801DE714 (0x801DE714): vtable slot 0x80. In state 0xA the floor check
// is EnemyBase's, called directly (a plain bl, not through the vtable).
void cls_804A51E4::vfunc_0x80() {
    if (mState == 0xA) {
        EnemyBase_804A4808::vfunc_0x80();
    }
}

// tu_801DE714 (0x801DE9C0): vtable slot 0x9C. The step is EnemyBase's
// fn_801CE2BC (a plain bl), then the view check.
void cls_804A51E4::vfunc_0x9C(u8 mapHit) {
    EnemyBase_804A4808::fn_801CE2BC(mapHit);
    fn_8021835C();
}

// tu_801DE714 (0x801E1D9C)
bool cls_804A51E4::fn_801E1D9C() const {
    bool result = false;
    if (mAnim.mAnim == 2) {
        result = true;
    }
    return result;
}

// tu_801DE714 (0x801E1DE8): forwards the result of fn_801CCF7C.
bool cls_804A51E4::fn_801E1DE8() {
    return fn_801CCF7C();
}

// tu_801DE714 (0x801E1D7C): whether mState is 0xB or 0xC.
// The upper bound is written first: the original branches on mState >= 0xD
// before mState < 0xB, and this order is what matches it.
bool cls_804A51E4::fn_801E1D7C() {
    bool result = false;
    if (mState < 0xD && mState >= 0xB) {
        result = true;
    }
    return result;
}
