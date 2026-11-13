#include "game/GameAudio.h"
#include "game/act/act_kaepora_gaebora.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/BoxTest.h"
#include "game/Camera.h"
#include "game/GameView.h"
#include "game/StageFlag.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/ActorManager.h"

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

extern const s16* lbl_804A6540[4]; // the sprite animation tables (.data of this unit)

// FIXME(shotgun): 98.56 %, missing/extra + reloc-name; behaviour drift.
void cls_804A6550::init() {
    EnemyBase_804A4808::init();
    bool cleared = isStageFlagSet(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c) != FALSE;
    bool viewFlag = getGameView_802D6A9C()->field_0x131f9 != 0;
    field_0x2a8 = (field_0x90 & 0xFFFF) + 0x12B;
    u32 t = (field_0x90 >> 20) & 0x1F;
    field_0x2ac = __fsel((f32)t - 1.5f, (f32)t, 1.5f);
    field_0x2b0 = (field_0x90 >> 25) & 0x1F;
    if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 0 && (field_0x90 >> 31) != 0) {
        mFlags |= 0x10000;
    }
    if ((field_0x90 >> 19) & 1) {
        field_0x2b4 = 2;
    } else if (cleared || viewFlag || (field_0x2b0 != 0 && getGameView_802D6A9C()->fn_802D800C(field_0x2b0))) {
        field_0x2b4 = 1;
        mFlags &= ~0x10000;
    } else {
        field_0x2b4 = 0;
    }
    switch (field_0x2b4) {
    case 0:
    case 3:
        mFlags |= 0x4000;
        break;
    }
    switch (field_0x2b4) {
    case 0:
    case 3:
        if ((field_0x90 >> 30) & 1) {
            mFlags |= 0x8000;
        }
        break;
    }
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 24.0f;
    mBox2.i.x = 1.0f;
    mBox2.i.y = 1.0f;
    mBox2.f.x = -1.0f;
    mBox2.f.y = -1.0f;
    field_0x80.i.x = 1.0f;
    field_0x80.i.y = 1.0f;
    field_0x80.f.x = -1.0f;
    field_0x80.f.y = -1.0f;
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    fn_801D0E58();
    mPos.z = 16.0f;
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    if (mPos.x <= 0.5f * (box->i.x + box->f.x)) {
        mFlags |= 0x2000;
    }
    switch (field_0x2b4) {
    case 0:
        if (mFlags & 0x10000) {
            mState = 0;
        } else {
            mState = 2;
        }
        break;
    case 1:
        mState = 3;
        break;
    case 2:
        mState = 6;
        mHead.fn_8021ABC0();
        break;
    case 3:
        mState = 2;
        mHead.fn_8021ABC0();
        break;
    }
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A6540, 4);
    mAnim.startAnim_801D22DC(0, 0, false);
    if (mFlags & 0x10000) {
        gGameRoot2_8053AB10->field_0x2c->field_0xc = (u8)field_0x2d8;
    }
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

// FIXME: 98.32 %, missing/extra + reloc-name; behaviour drift.
// (missing/extra + reloc-name). Target reloads mFlags before ori 0x2000 in case 12.
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

extern const f32 lbl_8046A640[4][2]; // x, y offsets (about +-16) by facing, .rodata of this unit

// FIXME(shotgun): 84.84 %, missing/extra + reloc-name; behaviour drift.
void cls_804A6550::vfunc_0x90(u8 mapHit) {
    JGeometry::TBox2f box;
    if (mState == 3) {
        box.i.x = mPos.x - 8.0f;
        box.i.y = mPos.y - 8.0f;
        box.f.x = mPos.x + 8.0f;
        box.f.y = mPos.y + 8.0f;
    } else {
        fn_801F31C4(&box);
    }
    switch (mState) {
    case 0:
        field_0x2b8 = 0;
        fn_8022FE80(field_0x2b8, field_0x4, mSerial, false, false);
        if (field_0x2b8 == fn_8022FD8C(mSerial)) {
            JGeometry::TVec3f pos(0.0f, -10.0f, 0.0f);
            if (fn_8038BD08(field_0x2b8, 0x150, field_0x4, pos, 2)) {
                fn_80219AFC(1);
            }
        }
        break;
    case 1:
        fn_8022FE80(field_0x2b8, field_0x4, mSerial, false, false);
        if (fn_803865B8(field_0x2b8) && fn_80384C58(field_0x2b8, mSerial)) {
            fn_80219AFC(4);
        }
        break;
    case 2:
        field_0x2b8 = fn_801D05F8(&mPos, 16.0f * field_0x2ac);
        if (field_0x2b8 >= 0 && fn_8023077C()) {
            if (field_0x2b4 == 0) {
                fn_80219AFC(4);
            } else if (field_0x2b4 == 3) {
                fn_80219AFC(8);
            }
        }
        break;
    case 3: {
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        bool found = false;
        for (s32 i = 0; i < 4; i++) {
            if (fn_801D0B44(i)) {
                s32 slot = fn_8022F514(i) ? getPlayerInputSlot_8030AAF4(fn_8022F3DC(i)) : getPlayerInputSlot_8030AAF4(i);
                if (lbl_8053AD30->fn_80309D6C(slot, 0x100)) {
                    s32 dir = fn_80230CFC(i);
                    JGeometry::TVec2f p(lbl_8046A640[dir][0] + getPlayerPos_8023E724(i)->x,
                                        lbl_8046A640[dir][1] + getPlayerPos_8023E724(i)->y);
                    if (containsPoint(box, p)) {
                        found = true;
                        break;
                    }
                }
            }
        }
        if (found) {
            gAudioMgr_8053AF80->fn_80458880(0x47, -1);
            fn_80219AFC(4);
        }
        break;
    }
    case 4:
        if (__fabsf(mSpawnPos.x - mPos.x) < 0.1f && __fabsf(mSpawnPos.y - mPos.y) < 0.1f) {
            mPos.x = mSpawnPos.x;
            mPos.y = mSpawnPos.y;
            fn_80219AFC(5);
        } else {
            if (mAnim.fn_801D21F0()) {
                if (mAnim.mAnim == 0) {
                    mAnim.startAnim_801D22DC(1, 0, false);
                } else {
                    mAnim.startAnim_801D22DC(0, 0, false);
                }
            }
            if (mStateTimer == 45) {
                startSound_801F0E34(0x4006c, 0);
            }
        }
        break;
    case 5:
        if (mPos.z <= 16.0f) {
            mPos.z = 16.0f;
            if (mAnim.fn_801D21F0()) {
                if (field_0x2b4 == 0) {
                    fn_80219AFC(8);
                } else {
                    fn_80219AFC(6);
                }
            }
        }
        break;
    case 6:
        if (getActorManager_801F856C()->field_0x3041 == 0) {
            fn_8022FEB8(&box, field_0x4, mSerial, true, false);
            field_0x2b8 = fn_8022FD8C(mSerial);
            if (field_0x2b8 >= 0) {
                JGeometry::TVec3f pos(0.0f, -10.0f, 0.0f);
                if (fn_8038C678(field_0x2b8, field_0x2a8, mSerial, field_0x4, pos, 0)) {
                    fn_80219AFC(7);
                }
            }
        }
        break;
    case 7:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x2b8, mSerial)) {
            if (mFlags & 0x4000) {
                fn_80219AFC(10);
            } else {
                fn_80219AFC(6);
            }
        }
        break;
    case 8:
        fn_8022FE80(field_0x2b8, field_0x4, mSerial, false, false);
        if (field_0x2b8 == fn_8022FD8C(mSerial)) {
            JGeometry::TVec3f pos(0.0f, -10.0f, 0.0f);
            if (fn_8038C678(field_0x2b8, field_0x2a8, mSerial, field_0x4, pos, 0x4002)) {
                if (mFlags & 0x10000) {
                    gGameRoot_8053AAF8->fn_80138B7C();
                    s32 count = gGameRoot_8053AAF8->fn_80138A30();
                    for (s32 i = 0; i < count; i++) {
                        s32 dir = fn_801CD354(fn_801D0020(&mPos, i, false));
                        fn_80230BF4(i, dir);
                        fn_80230498(i, getPlayerPos_8023E724(i), dir + 0x3c, 0xFF, -0x80, -0x80, true);
                    }
                }
                fn_80219AFC(9);
            }
        }
        break;
    case 9:
        fn_8022FE80(field_0x2b8, field_0x4, mSerial, false, false);
        if (fn_803865B8(field_0x2b8) && fn_80384C58(field_0x2b8, mSerial)) {
            if (mFlags & 0x10000) {
                s32 count = gGameRoot_8053AAF8->fn_80138A30();
                for (s32 i = 0; i < count; i++) {
                    fn_80230BF4(i, 1);
                    fn_80230498(i, getPlayerPos_8023E724(i), 0x3d, 0xFF, -0x80, -0x80, true);
                }
            }
            fn_80230534(false);
            if (mFlags & 0x4000) {
                fn_80219AFC(10);
            } else {
                fn_80219AFC(6);
            }
        }
        break;
    case 10:
        if (mHead.mTimer == 0) {
            fn_80219AFC(11);
        }
        break;
    case 11:
        if (mPos.z >= 35.0f) {
            mPos.z = 35.0f;
            if (mAnim.fn_801D21F0()) {
                fn_80219AFC(12);
            }
        }
        break;
    case 12:
        if (__fabsf(field_0x2bc.x - mPos.x) < 0.1f && __fabsf(field_0x2bc.y - mPos.y) < 0.1f) {
            mAlive = false;
        } else if (mAnim.fn_801D21F0()) {
            if (mAnim.mAnim == 0) {
                mAnim.startAnim_801D22DC(1, 0, false);
            } else {
                mAnim.startAnim_801D22DC(0, 0, false);
            }
        }
        break;
    }
    bool settle = false;
    if (mState != 10) {
        settle = fn_80219E04();
    }
    if (mHead.fn_8021A570(settle)) {
        switch (mHead.mState) {
        case 0:
            startSound_801F0E34(0x4006e, 0);
            break;
        case 1:
            startSound_801F0E34(0x4006d, 0);
            break;
        }
    }
}

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

// The original stores each __fsel result to pos straight from the double register and rounds it
// (frsp) only where it is read again; ours rounds before the store (extra frsp / moved stores).
// The camera: in states 4, 5, 8 and 9 the owl is kept in view (clamped into the view box by 0.001),
// a scripted camera move to it (states 4 and 5 while field_0x2b4 is 0) or focus points (also
// 24 units higher in states 8 and 9).
// FIXME: 94.66 %, missing/extra; behaviour drift.
void cls_804A6550::vfunc_0xA0() {
    if (field_0x2b4 == 0 || field_0x2b4 == 3) {
        switch (mState) {
        case 4:
        case 5:
        case 8:
        case 9: {
            cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
            if (!sub->field_0x284) {
                sub->field_0x284 = 1;
                sub->field_0x28c = 0.05f;
            }
            break;
        }
        }
    }
    if (mFlags & 0x10000) {
        switch (mState) {
        case 0:
        case 1:
            gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
            break;
        }
        return;
    }
    if (field_0x2b4 == 0 || field_0x2b4 == 1) {
        switch (mState) {
        case 4:
        case 5:
        case 8:
        case 9: {
            s32 layer = field_0x4;
            GameView_804A9950* view = getGameView_802D6A9C();
            JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
            JGeometry::TVec3f pos(mPos.x, mPos.y - mPos.z, 0.0f);
            // Kept 0.001 inside the box's left, top and right edges: max / min with fsel (the
            // MWCC intrinsic, like __fabsf elsewhere), as the original's fsel / frsp show.
            float top = box->i.y + 0.001f;
            pos.y = __fsel(top - pos.y, top, pos.y);
            float left = box->i.x + 0.001f;
            pos.x = __fsel(left - pos.x, left, pos.x);
            float right = box->f.x - 0.001f;
            pos.x = __fsel(right - pos.x, pos.x, right);
            if (containsPoint(*box, pos)) {
                if (field_0x2b4 == 0 && (mState == 4 || mState == 5)) {
                    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
                    if (camera->field_0x7f5 == 0) {
                        camera->field_0x7f4 = 1;
                        camera->field_0x7f8 = 4;
                        camera->field_0x7fc.x = pos.x;
                        camera->field_0x7fc.y = pos.y;
                        camera->field_0x7fc.z = pos.z;
                        camera->field_0x7f6 = 0;
                    }
                } else {
                    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
                    switch (mState) {
                    case 8:
                    case 9:
                        float top2 = box->i.y + 0.001f;
                        pos.y = __fsel(top2 - (pos.y - 24.0f), top2, pos.y - 24.0f);
                        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
                        break;
                    }
                }
            }
            break;
        }
        }
    }
}
