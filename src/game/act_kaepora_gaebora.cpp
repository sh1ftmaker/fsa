#include "game/GameAudio.h"
#include "game/act/act_kaepora_gaebora.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ViewLayer.h"

// FIXME: 99.52% (reloc-name). Floating-point literals pooled in sdata2.
cls_804A6550::cls_804A6550()
    : field_0x2a8(0),
      field_0x2ac(1.5f),
      field_0x2b0(0),
      field_0x2b4(0),
      field_0x2b8(-1),
      field_0x2bc(0.0f, 0.0f),
      mEmitter(NULL),
      field_0x2d8(0.0f) {
}

cls_804A6550::~cls_804A6550() {
    releaseEmitter_801EE434(mEmitter);
}

bool cls_804A6550::fn_80219E04() const {
    bool ret = false;
    switch (mState) {
    case 2:
        if (field_0x2b4 == 3) {
            ret = true;
        }
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        ret = true;
        break;
    }
    return ret;
}

// FIXME: 98.07% (missing/extra + reloc-name). Target reloads mFlags before ori 0x2000 in case 12.
void cls_804A6550::fn_80219AFC(s32 state) {
    // The state tests (states 5 and 11) are written out as in the original's inlined helper; a shared helper drops to 91.68 %.
    bool oldFlying = false;
    switch (mState) {
    case 5:
    case 11:
        oldFlying = true;
        break;
    }
    if (!oldFlying) {
        bool newFlying = false;
        switch (state) {
        case 5:
        case 11:
            newFlying = true;
            break;
        }
        if (newFlying) {
            fn_8021B5C4();
        }
    } else {
        bool oldFlying2 = false;
        switch (mState) {
        case 5:
        case 11:
            oldFlying2 = true;
            break;
        }
        if (oldFlying2) {
            bool newFlying2 = false;
            switch (state) {
            case 5:
            case 11:
                newFlying2 = true;
                break;
            }
            if (!newFlying2) {
                fn_8021B5A0();
            }
        }
    }

    switch (mState) {
    case 5:
        mHead.fn_8021ABC0();
        break;
    case 7:
    case 9: {
        s32 id = field_0x2b0;
        if (id != 0) {
            getGameView_802D6A9C()->fn_802D8050(id, true, 0xFF);
        }
        if (mFlags & 0x10000) {
            fn_8043A0E8(true);
        }
        if (mFlags & 0x8000) {
            lbl_8053AED0->fn_8043A608();
            mFlags &= ~0x8000;
        }
        break;
    }
    }

    switch (state) {
    case 0:
        break;
    case 4: {
        mTargetSpeed = 1.5f;
        JGeometry::TBox2f* box = layerBox(field_0x4);
        // spawn is taken before the angle is chosen (the original's addi r4 comes first), and py/px are both
        // loaded before either store; without them this drops to 94.99 %.
        const JGeometry::TVec3f* spawn = &mSpawnPos;
        s32 angle = -0x5556;
        if (mFlags & 0x2000) {
            angle = 0x5556;
        }
        JGeometry::TVec2f pos;
        fn_801CBB98(box, spawn, angle, 60.0f, &pos);
        f32 py = pos.y;
        f32 px = pos.x;
        mPos.x = px;
        mPos.y = py;
        mPos.z = 35.0f;
        break;
    }
    case 5:
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(2, 0, false);
        break;
    case 6:
    case 8:
        mTargetSpeed = 0.0f;
        break;
    case 7:
    case 9:
        mTargetSpeed = 0.0f;
        startSound_801F0E34(0x4006C, 0);
        break;
    case 10:
        mTargetSpeed = 0.0f;
        mHead.field_0xc = 1;
        break;
    case 11:
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(3, 0, false);
        break;
    case 12: {
        mTargetSpeed = 1.5f;
        mAnim.startAnim_801D22DC(1, 0, false);
        JGeometry::TBox2f* box = layerBox(field_0x4);
        s32 angle;
        if (mFlags & 0x10000) {
            mFlags |= 0x2000;
            angle = -0x3556;
        } else if (mFlags & 0x2000) {
            angle = -0x2AAA;
        } else {
            angle = 0x2AAA;
        }
        fn_801CBB98(box, &mSpawnPos, angle, 60.0f, &field_0x2bc);
        break;
    }
    }

    mStateTimer = 0;
    mState = state;
}

// FIXME: 99.69% (reloc-name). Frame check float literals pooled in sdata2.
void cls_804A6550::vfunc_0x94() {
    if (mAnim.mAnim == 1) {
        if (mAnim.mFrameCtrl.checkPass(0.1f) ||
            mAnim.mFrameCtrl.checkPass(22.1f) ||
            mAnim.mFrameCtrl.checkPass(44.1f)) {
            startSound_801F0E34(0x4006B, 0);
        }
    }
    s32 oldCel = mAnim.mCel;
    EnemyBase_804A4808::vfunc_0x94();
    s32 newCel = mAnim.mCel;
    if (mAnim.mAnim == 1 && oldCel != 5 && newCel == 5) {
        fn_8021B668();
    }
}

// FIXME: 99.85% (reloc-name). Floating-point literals pooled in sdata2.
void cls_804A6550::vfunc_0x9C(u8 mapHit) {
    // The 0x10000 test appears twice in the original (a single test drops to 98.38 %).
    if (mFlags & 0x10000) {
        if (mFlags & 0x10000) {
            if (mState >= 2 || mState < 0) {
                if (field_0x2d8 != 255.0f) {
                    if (fn_801CD950(&field_0x2d8, 255.0f, 1.0f) == 0) {
                        s32 playerCount = gGameRoot_8053AAF8->fn_80138A30();
                        for (int i = 0; i < playerCount; i++) {
                            fn_80230BF4(i, 1);
                            fn_80230498(i, getPlayerPos_8023E724(i), 0x3d, 0xff, -0x80, -0x80, true);
                        }
                    }
                }
            }
            gGameRoot2_8053AB10->field_0x2c->field_0xc = field_0x2d8;
        }
    }
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    fn_8021B548();
}

// FIXME: 99.73% (reloc-name). Velocity float literals pooled in sdata2.
void cls_804A6550::vfunc_0x88() {
    JGeometry::TVec2f vel;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    switch (mState) {
    case 4:
        fn_801D01A0(&mSpawnPos, &vel, mSpeed);
        field_0x3c.x = vel.x;
        field_0x3c.y = vel.y;
        break;
    case 5:
        field_0x3c.z = -0.5f;
        break;
    case 11:
        if (mAnim.mCel >= 15 || mAnim.mCel < 11) {
            field_0x3c.z = 0.8f;
        }
        break;
    case 12:
        // (const JGeometry::TVec3f*)&field_0x2bc: 2D target position at +0x2BC passed to 3D query
        fn_801D01A0((const JGeometry::TVec3f*)&field_0x2bc, &vel, mSpeed);
        field_0x3c.x = vel.x;
        field_0x3c.y = vel.y;
        break;
    }
}

u8 cls_804A6550::vfunc_0x5C() {
    return 0;
}

u8 cls_804A6550::vfunc_0x60() {
    return 0;
}
