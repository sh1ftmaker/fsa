#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801DE714.h"
#include "game/tu_80379F84.h"

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

// tu_801DE714 (0x801E1D7C): whether mState is 0xB or 0xC.
bool cls_804A51E4::fn_801E1D7C() {
    bool result = false;
    if (mState < 0xD && mState >= 0xB) {
        result = true;
    }
    return result;
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

// tu_801DE714 (0x801E2004, vtable slot 0x54, EnemyBase's start hook): see the
// declaration in act/tu_801DE714.h.
// FIXME: 99.8 %. Only the literal label (30.0f is @NNN, not lbl_8053F4A4:
// the unit's earlier .sdata2 users are not written yet).
void cls_804A51E4::vfunc_0x54() {
    fn_801F856C()->fn_801F7AB8(mActorId, mSerial);
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    mPos.x = box->i.x - 30.0f;
    field_0x2c4.set(mPos.x, mPos.y);
}

// tu_801DE714 (0x801E2180, vtable slot 0x08): the deleting destructor. Its
// own part runs first; EnemyBase's destructor is inlined after it.
cls_804A51E4::~cls_804A51E4() {
    if (mTargetSerial >= 0) {
        if (field_0x2c0 > 0) {
            // Bit 0x80000 of mFlags is not in EnemyFlag_804A4808 yet.
            if ((mFlags & 0x80000) == 0) {
                SaveRow_8050EC80& row = lbl_8050EC80[lbl_8050EC80[0].field_0x10];
                row.field_0x2c += field_0x2c0;
                // The clamp looks the current row up again (the original
                // indexes lbl_8050EC80 a second time after the add).
                SaveRow_8050EC80& clamped = lbl_8050EC80[lbl_8050EC80[0].field_0x10];
                if (clamped.field_0x2c < 0) {
                    clamped.field_0x2c = 0;
                }
            }
            field_0x2c0 = 0;
        }
        ActorBase_804A5770* target = fn_801F856C()->findActorBySerial_801F666C(mTargetSerial);
        if (target != NULL) {
            target->mAlive = false;
        }
        mTargetSerial = -1;
    }
}

// tu_801DE714 (0x801E22A4): the constructor, after EnemyBase's (a plain bl).
// FIXME: 99.8 %. Only the literal label of 0.0f (@NNN vs lbl_; data split).
cls_804A51E4::cls_804A51E4() {
    field_0x2a8 = -1;
    field_0x2ac = -1;
    field_0x2b0 = -1;
    field_0x2b4 = -1;
    field_0x2b8 = -1;
    mTargetSerial = -1;
    field_0x2c0 = 0;
    field_0x2c4.set(0.0f, 0.0f);
    field_0x2cc = 0;
    field_0x2ce = 0;
    field_0x2d0 = -1;
}
