#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/act/tu_801DB18C.h"

void cls_804A4EE8::vfunc_0x94() {
    s32 cel = mAnim.mCel;
    EnemyBase_804A4808::vfunc_0x94();
    if (cel == 8 && mAnim.mCel == 7) {
        fn_801F0E34(0x4005B, 0);
    }
}

void cls_804A4EE8::fn_801DB950(s32 state) {
    // A one-case switch: MWCC's cmpwi/beq/b shape for state 1 is what matches.
    switch (state) {
    case 1:
        field_0x250 = -1;
        mAnim.mFrameCtrl.setRate(1.0f);
        field_0x2ac = 0;
        fn_801DB9CC();
        mTargetPlayer = fn_801D05F8(&mSearchPos, mSearchRange);
        break;
    }
    mStateTimer = 0;
    mState = state;
}

void cls_804A4EE8::vfunc_0x9C(u8 mapHit) {
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    fn_8013CB44(field_0x2e4, &mPos, 1, 0, true);
}
