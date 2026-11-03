#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/act_shadow_link_bow.h"
#include "game/act/act_octorok_rock.h"

// A random offset in -range..range.
static inline f32 spread(f32 range) {
    return range * (2.0f * lbl_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
}

cls_804A6B70::~cls_804A6B70() {}

void cls_804A6B70::vfunc_0x88() {
    if (mState == 2) {
        fn_80221250();
    } else {
        EnemyBase_804A4808::vfunc_0x88();
    }
}

// FIXME: 99.85%. Only the jump table relocation (jumptable_804A6C14 vs @NNN)
// and the .data order differ, as in tu_80221B44's vfunc_0x90.
void cls_804A6B70::vfunc_0x90(u8 mapHit) {
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
            fn_80222668(5);
        }
        break;
    case 4:
        fn_802216E8(120);
        if (mPos.z <= 0.0f) {
            if (mFlags & CLS_804A6A00_FLAG_0x2000) {
                fn_80222668(5);
            } else {
                field_0x3c.z = lbl_8053FE70;
            }
        }
        break;
    case 5:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_80222668(6);
        } else if (mStateTimer == 25) {
            fn_801F0E34(0x400c6, 0);
        }
        break;
    case 6:
        if (mStateTimer >= 10) {
            if (mShots % 10 == 0) {
                mRounds++;
                if (mRounds >= 3) {
                    fn_80222668(7);
                } else {
                    fn_80222668(3);
                }
            } else {
                fn_80222668(6);
            }
        }
        break;
    case 7:
        if (mStateTimer >= 120) {
            fn_8022149C(1);
        }
        break;
    }
}

void cls_804A6B70::fn_80222668(s32 state) {
    switch (state) {
    case 3:
        field_0x250 = 120;
        mAnim.fn_801D22DC(field_0x98, 0, false);
        break;
    case 5:
        mAnim.fn_801D22DC(0x1b, 0, false);
        break;
    case 6:
        fn_80222730();
        mShots++;
        break;
    case 7:
        mAnim.fn_801D22DC(field_0x98, 0, false);
        break;
    }
    mStateTimer = 0;
    mState = state;
}

// FIXME: 98.7%. The original draws the y jitter (15) before the x jitter (7)
// and keeps reach / x / bottom in f24 / f25 / f26 (ours f25 / f24 / f26 with
// x and y swapped in the pairs), so two lbl_8053AAF8 loads use r4/r5 the
// other way round; it also loads box.f.y before box.i.y. The .sdata2 pool
// order differs too (32.0f before 64.0f here).
void cls_804A6B70::fn_80222730() {
    s32 layer = field_0x4;
    GameView_804A9950* view = fn_802D6A9C();
    const JGeometry::TBox2f& box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
    f32 half = 0.5f * (box.f.x - box.i.x);
    f32 bottom = box.i.y - 32.0f;
    f32 reach = 64.0f + (32.0f + (box.f.y - box.i.y));
    f32 x = mPos.x - 0.5f * half;
    for (s32 i = 0; i < (s32)(0.0625f * half) + 1; i++) {
        JGeometry::TVec3f pos(x + spread(7.0f), bottom + spread(15.0f), 0.0f);
        s32 serial = fn_801F856C()->fn_801F9484(ACTOR_OKTM, field_0x4, &pos, 5, -1, -1);
        if (serial >= 0) {
            OctorokRock_8049F5D8* rock =
                static_cast<OctorokRock_8049F5D8*>(fn_801F856C()->fn_801F666C(serial));
            if (rock != NULL) {
                rock->field_0x98 = 2;
                JGeometry::TVec3f vel;
                vel.x = 0.0f;
                vel.y = 3.2f + 0.6f * (2.0f * lbl_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
                vel.z = 0.0f;
                rock->field_0x3c = vel;
                rock->field_0x194 = 1;
                rock->field_0x23c = (s32)(reach * (1.0f / rock->field_0x3c.y));
            }
        }
        x += 16.0f;
    }
}

void cls_804A6B70::init() {
    cls_804A6A00::init();
    field_0x98 = 2;
    mState = 4;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D22DC(2, 0, false);
}

cls_804A6B70::cls_804A6B70() {
    mRounds = 0;
    mShots = 0;
}
