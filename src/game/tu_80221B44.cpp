#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/act/tu_80221B44.h"

cls_804A6AA8::~cls_804A6AA8() {}

void cls_804A6AA8::vfunc_0x88() {
    if (mState == 2) {
        fn_80221250();
    } else {
        EnemyBase_804A4808::vfunc_0x88();
    }
}

void cls_804A6AA8::vfunc_0x9C(u8 mapHit) {
    cls_804A6A00::vfunc_0x9C(mapHit);
    mRing.fn_800F802C();
}

// FIXME: 99.93%. Only the stack slots of the position locals differ: the
// original has the by-value copy for the ring at 0x20, the state 7 position
// at 0x14 and the state 5 position at 0x8; ours has them at 0x8, 0x14 and
// 0x20 (the copy gets the lowest slot). Jump table reloc is a data-name
// difference (jumptable_804A6B4C vs @NNN).
void cls_804A6AA8::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        fn_80221408();
        break;
    case 1:
        fn_80221334();
        break;
    case 2:
        fn_802212AC();
        break;
    case 3:
        if (mStateTimer >= field_0x250) {
            mAnim.fn_801D22DC(0x18, 0, false);
            mStateTimer = 0;
            mState = 5;
        }
        break;
    case 4:
        fn_802216E8(60);
        if (mPos.z <= 0.0f) {
            if (mFlags & CLS_804A6A00_FLAG_0x2000) {
                mAnim.fn_801D22DC(0x18, 0, false);
                mStateTimer = 0;
                mState = 5;
            } else {
                field_0x3c.z = lbl_8053FE70;
            }
        }
        break;
    case 5:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            mAnim.fn_801D22DC(0x19, 0, false);
            JGeometry::TVec3f pos = mPos;
            pos.y -= 22.0f;
            fn_8013CC50(field_0x4, &pos, 0xcb, 0, 1, NULL, NULL, true, 1.0f);
            fn_8013CC50(field_0x4, &pos, 0xcc, 0, 1, NULL, NULL, true, 1.0f);
            fn_801F0E34(0x400c4, 0);
            mStateTimer = 0;
            mState = 6;
        }
        break;
    case 6:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            mAnim.fn_801D22DC(0x1a, 0, false);
            mStateTimer = 0;
            mState = 7;
        }
        break;
    case 7:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            mLandings++;
            if (mLandings >= 3) {
                mAnim.fn_801D22DC(field_0x98, 0, false);
                mStateTimer = 0;
                mState = 8;
            } else {
                field_0x250 = 120;
                mAnim.fn_801D22DC(field_0x98, 0, false);
                mStateTimer = 0;
                mState = 3;
            }
        } else if (mStateTimer == 3) {
            JGeometry::TVec3f pos(mPos.x, mPos.y, 0.0f);
            switch (field_0x98) {
            case 0:
                pos.x += 12.0f;
                break;
            case 1:
                pos.x -= 12.0f;
                break;
            case 2:
                pos.y += 12.0f;
                break;
            case 3:
                pos.y -= 12.0f;
                break;
            }
            startRing(field_0x4, pos);
            fn_8013CC50(field_0x4, &pos, 0x23b, 0, 0, NULL, NULL, true, 1.0f);
            fn_8013CC50(field_0x4, &pos, 0x23c, 0, 0, NULL, NULL, true, 1.0f);
            fn_8013CC50(field_0x4, &pos, 0x23d, 0, 0, NULL, NULL, true, 5.0f);
            fn_8013CC50(field_0x4, &pos, 0x23e, 0, 0, NULL, NULL, true, 2.0f);
            fn_8013CC50(field_0x4, &pos, 0x23f, 0, 0, NULL, NULL, true, 2.0f);
            if (fn_802258D4(&pos, field_0x4) || fn_80225614(&pos, field_0x4)) {
                fn_8013CC50(field_0x4, &pos, 0x3d0, 0, 0, NULL, NULL, true, 2.0f);
            } else {
                fn_8013CC50(field_0x4, &pos, 0x3cf, 0, 0, NULL, NULL, true, 2.0f);
            }
            fn_801F0E34(0x400e6, 0);
            fn_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
            lbl_8053AAF8->fn_80134DE4(1, 0);
        }
        break;
    case 8:
        if (mStateTimer >= 60) {
            fn_8022149C(1);
        }
        break;
    }
}

void cls_804A6AA8::init() {
    cls_804A6A00::init();
    field_0x98 = 2;
    mState = 4;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D22DC(2, 0, false);
}

cls_804A6AA8::cls_804A6AA8() {
    mLandings = 0;
    mRing.mActive = 0;
}
