#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_wallmaster.h"

static const s16 lbl_8053EBF0[] = { 0, 10, -1 };
static const s16 lbl_8053EBF8[] = { 1, 10, -1 };
static const s16 lbl_80469398[] = { 0, 15, 1, 15, -1 };
static const s16 lbl_804693A4[] = { 0, 3, 1, 7, 0, 5, 1, 20, -1 };
static const s16 lbl_804693B8[] = { 0, 30, 1, 30, -1 };

static const s16* lbl_804A3E30[5] = {
    lbl_8053EBF0,
    lbl_8053EBF8,
    lbl_80469398,
    lbl_804693A4,
    lbl_804693B8,
};

void WallmasterParam::fn_801BCD70(s32 arg) {
    mType = (arg >> 5) & 7;
    switch (mType) {
    case 0:
        mOrder[0] = 3;
        mOrder[1] = 0;
        mOrder[2] = 2;
        break;
    case 1:
        mOrder[0] = 3;
        mOrder[1] = 1;
        mOrder[2] = 2;
        break;
    case 2:
        mOrder[0] = 1;
        mOrder[1] = 3;
        mOrder[2] = 0;
        break;
    case 3:
        mOrder[0] = 1;
        mOrder[1] = 2;
        mOrder[2] = 0;
        break;
    case 4:
        mOrder[0] = 2;
        mOrder[1] = 0;
        mOrder[2] = 3;
        break;
    case 5:
        mOrder[0] = 2;
        mOrder[1] = 1;
        mOrder[2] = 3;
        break;
    case 6:
        mOrder[0] = 0;
        mOrder[1] = 3;
        mOrder[2] = 1;
        break;
    case 7:
        mOrder[0] = 0;
        mOrder[1] = 2;
        mOrder[2] = 1;
        break;
    }

    switch (mType) {
    case 0:
    case 1:
    case 4:
    case 5:
        field_0x10[1] = 480;
        break;
    default:
        field_0x10[1] = 240;
        break;
    }

    field_0x10[2] = 40;
    field_0x10[0] = 40;
    mIndex = 0;
    mTarget = mOrder[mIndex];
    field_0x24 = 0;
}

cls_804A3E44::cls_804A3E44() {
    field_0x2a8 = 0;
    field_0x2ac = 0;
    field_0x2b0.set(0.0f, 0.0f);
    field_0x2b8 = -1;
    field_0x2bc = 0;
    field_0x2c0 = 0;
    field_0x2c4 = -1;
    mParam.mType = 8;
    mParam.mIndex = 0;
    mParam.mTarget = 2;
    mParam.field_0x24 = 0;
}

cls_804A3E44::~cls_804A3E44() {
    if (mFlags & 0x8000) {
        if (field_0x2c4 >= 0) {
            fn_8022DD74(field_0x2c4);
        }
    }
}

void cls_804A3E44::init() {
    EnemyBase_804A4808::init();
    u32 p = field_0x90 & 0x1F;
    if (p >= 2) {
        mFlags |= 0x4000;
    }
    field_0x2a8 = p & 1;
    mParam.fn_801BCD70(field_0x90);
    if (mFlags & 0x4000) {
        field_0x2ac = field_0x4;
    } else {
        field_0x2ac = (field_0x90 >> 24) & 7;
        if (field_0x2ac != 0) {
            field_0x2ac += 7;
        }
    }
    if (!(mFlags & 0x4000)) {
        f32 f4 = 0.0f;
        f32 f5 = 0.0f;
        if (field_0x2ac == 0) {
            const JGeometry::TBox2f* box = layerBox(field_0x2ac);
            f4 = box->i.x;
            f5 = box->i.y;
        }
        u32 x = ((field_0x90 >> 16) & 0x3F) * 8;
        u32 y = ((field_0x90 >> 8) & 0x3F) * 8;
        field_0x2b0.x = x + f4 + 8.0f;
        field_0x2b0.y = y + f5 + 8.0f;
    }
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x98 = 2;
    mFlags |= 0x100;
    field_0x194 = 1;
    if (field_0x2a8 == 0) {
        mState = 0;
        field_0x250 = fn_801CD664(0x79) + 60;
        mPos.z = 100.0f;
    } else {
        mState = 9;
        field_0x250 = fn_801CD664(0xB5) + 180;
    }
    mStateTimer = 0;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A3E30, 5);
    mAnim.startAnim_801D22DC(0, 0, false);
    bool b = false;
    switch (mState) {
    case 4:
    case 6:
    case 7:
    case 8:
        b = true;
        break;
    }
    if (b) {
        field_0xb0 = 0x40004;
    } else {
        field_0xb0 = 0x41101;
    }
}

// FIXME: 99.09% (missing/extra + reloc-name: 1 extra instruction at 0x3EC; target shares r3=0 for mStateTimer and switch).
void cls_804A3E44::fn_801BC554(s32 state) {
    switch (state) {
    case 0:
        mFlags &= ~0x2000;
        field_0x2b8 = -1;
        field_0x2bc = 0;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2.set(mBox);
        field_0x98 = 2;
        field_0x250 = fn_801CD664(0x79) + 60;
        mPos.set(mSpawnPos.x, mSpawnPos.y, 100.0f);
        field_0x4 = field_0x8;
        break;
    case 1:
        field_0x2bc = 0;
        mBox.set(-12.0f, -20.0f, 12.0f, 8.0f);
        mBox2.set(mBox);
        mAnim.startAnim_801D22DC(0, 0, false);
        if (mState != 5) {
            mPos.set(mSpawnPos.x, mSpawnPos.y, 100.0f);
            field_0x4 = field_0x8;
        }
        break;
    case 3:
        field_0x2bc = 0;
        mBox.set(-12.0f, -20.0f, 12.0f, 8.0f);
        mBox2.set(mBox);
        mAnim.startAnim_801D22DC(2, 0, false);
        break;
    case 4:
    case 6:
    case 7:
    case 8:
        mFlags |= 0x2000;
        field_0x2bc = 0;
        mBox.set(-12.0f, -20.0f, 12.0f, 8.0f);
        mBox2.set(mBox);
        mAnim.startAnim_801D22DC(1, 0, false);
        if (state == 4) {
            if (mFlags & 0x4000) {
                JGeometry::TVec3f* pPos = fn_8023E604(field_0x2b8);
                field_0x2b0.x = pPos->x;
                field_0x2b0.y = pPos->y;
            }
        }
        break;
    case 5:
        field_0x2b8 = -1;
        field_0x2bc = 0;
        mBox.set(-12.0f, -20.0f, 12.0f, 8.0f);
        mBox2.set(mBox);
        mAnim.startAnim_801D22DC(0, 0, false);
        break;
    case 9:
        mFlags &= ~0x2000;
        field_0x2b8 = -1;
        field_0x2bc = 0;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2.set(mBox);
        field_0x98 = 2;
        field_0x250 = fn_801CD664(0xB5) + 180;
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        mPos.x = mSpawnPos.x;
        mPos.y = mSpawnPos.y;
        mPos.z = mSpawnPos.z;
        field_0x4 = field_0x8;
        break;
    case 10:
        mFlags &= ~0x2000;
        field_0x2b8 = -1;
        field_0x2bc = 0;
        mBox.set(-12.0f, -20.0f, 12.0f, 8.0f);
        mBox2.set(mBox);
        mSpeed = 0.5f;
        mTargetSpeed = 0.5f;
        mParam.mIndex = 0;
        mParam.mTarget = mParam.mOrder[mParam.mIndex];
        mParam.field_0x24 = 0;
        mAnim.fn_801D22A0(4, 2);
        break;
    }

    bool b = false;
    mStateTimer = 0;
    mState = state;
    switch (mState) {
    case 6:
    case 7:
    case 8:
        b = true;
        break;
    }
    if (!b) {
        field_0x2c0 = 0;
    }

    bool b2 = false;
    switch (mState) {
    case 4:
    case 6:
    case 7:
    case 8:
        b2 = true;
        break;
    }
    if (b2) {
        field_0xb0 = 0x40004;
    } else {
        field_0xb0 = 0x41101;
    }
}

inline u8 cls_804A3E44::computeShadowAlpha() {
    u8 val;
    switch (mState) {
    case 0:
    case 9:
        val = 0;
        break;
    case 7:
    case 8:
        val = 255;
        break;
    default: {
        f32 f = (mPos.z - 100.0f) * -0.01f;
        if (f > 1.0f) {
            f = 1.0f;
        } else if (f < 0.0f) {
            f = 0.0f;
        }
        val = f * 255.0f;
        break;
    }
    }
    return val;
}

inline bool cls_804A3E44::checkPlayerHit() {
    bool hitFlag = false;
    if (mState == 10 || (mState == 3 && mAnim.mCel == 1)) {
        if (field_0x2b8 >= 0 && !fn_8022DCD8(field_0x2b8)) {
            if (fn_801CD2F8(field_0x4) || !fn_801CD2F8(field_0x2ac) || !fn_801CD1D0(field_0x2b8)) {
                mBox.set(-16.0f, -24.0f, 16.0f, 0.0f);
                u8 hit = fn_801F2740(-1);
                if (hit & (1 << field_0x2b8)) {
                    hitFlag = true;
                }
                mBox.set(-12.0f, -20.0f, 12.0f, 8.0f);
                mBox2.set(mBox);
            }
        }
    }
    return hitFlag;
}

// FIXME: 99.64% (regalloc-only + reloc-name: f30/f31 register allocation for dz/dy and literal relocations).
void cls_804A3E44::fn_801BC244() {
    switch (mState) {
    case 1:
    case 2: {
        u8 val = computeShadowAlpha();
        ActorSoundObj_804578F4* sound = fn_801F1184();
        sound->field_0x4c = (f32)val / 255.0f;
        startSound_801F0E34(0x400A3, 0);
        break;
    }
    }

    switch (mState) {
    case 1:
    case 2: {
        field_0x2bc++;
        s32 player = fn_801D05F8(&mPos, 0.0f);
        if (player != field_0x2b8) {
            if (player < 0) {
                if (mState != 1 || mStateTimer >= 60) {
                    fn_801BC554(5);
                }
            } else if (field_0x2bc >= 45) {
                field_0x2b8 = player;
                field_0x2bc = 0;
            }
        }
        break;
    }
    }

    bool b = false;
    switch (mState) {
    case 4:
    case 6:
    case 7:
    case 8:
        b = true;
        break;
    }
    if (b) {
        if (!fn_8022DCD8(field_0x2b8)) {
            fn_801BC554(5);
        } else {
            f32 dz = (mPos.z - getPlayerPos_8023E724(field_0x2b8)->z) - 2.0f;
            f32 dy = mPos.y - getPlayerPos_8023E724(field_0x2b8)->y;
            f32 dx = mPos.x - getPlayerPos_8023E724(field_0x2b8)->x;
            JGeometry::TVec3f vec;
            vec.y = dy;
            vec.z = dz;
            vec.x = dx;
            vec.x *= 0.1f;
            vec.y *= 0.1f;
            fn_8023DFF4(field_0x2b8, &vec);
        }
    }

    bool b2 = false;
    switch (mState) {
    case 6:
    case 7:
    case 8:
        b2 = true;
        break;
    }
    if (b2) {
        field_0x2c0++;
        if (field_0x2c0 >= 120) {
            if (fn_80236228(field_0x2b8) >= 20) {
                fn_80236074(field_0x2b8, 20, 1);
            }
            field_0x2c0 = 0;
        }
    }

    if (mFlags & 0x8000) {
        if (fn_8022DD74(field_0x2c4)) {
            mFlags &= ~0x8000;
            field_0x2c4 = -1;
        }
    }
}

// FIXME: 99.13% (missing/extra: 2 extra instrs, 528 vs 526; reloc-name)
void cls_804A3E44::vfunc_0x90(u8 mapHit) {
    fn_801BC244();
    switch (mState) {
    case 0:
        if (mStateTimer >= field_0x250 && !(mFlags & 0x2000)) {
            s32 p = fn_801D05F8(&mSpawnPos, 32.0f);
            if (p >= 0) {
                field_0x2b8 = p;
                fn_801BC554(1);
            }
        }
        break;
    case 1:
        if (mStateTimer >= 60) {
            bool b = !fn_801D08B4(field_0x2b8, &mSpawnPos, 48.0f);
            if (b) {
                fn_801BC554(5);
                break;
            }
        }
        if (mPos.z <= 32.0f) {
            fn_801BC554(2);
            break;
        }
        if (mStateTimer >= 16) {
            switch (mAnim.mAnim) {
            case 0: {
                u8 val = computeShadowAlpha();
                if (val >= 150 && val <= 200) {
                    mAnim.startAnim_801D22DC(3, 0, false);
                }
                break;
            }
            case 3:
                if (mAnim.fn_801D21F0()) {
                    mAnim.startAnim_801D22DC(0, 0, false);
                }
                break;
            }
        }
        break;
    case 2: {
        bool b = !fn_801D08B4(field_0x2b8, &mSpawnPos, 48.0f);
        if (b) {
            fn_801BC554(5);
        } else if (mPos.z <= 2.0f) {
            mPos.z = 2.0f;
            fn_801BC554(3);
        } else if (mAnim.mAnim == 3) {
            if (mAnim.fn_801D21F0()) {
                mAnim.startAnim_801D22DC(0, 0, false);
            }
        }
        break;
    }
    case 3:
        if (checkPlayerHit()) {
            if (fn_8022DE10(field_0x2b8, 0)) {
                fn_801BC554(4);
            } else if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
                fn_801BC554(5);
            }
        } else if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_801BC554(5);
        }
        break;
    case 4:
        if (mPos.z >= 4.0f) {
            if (field_0x2ac != field_0x4) {
                fn_801BC554(7);
            } else {
                mPos.z = 4.0f;
                fn_801BC554(6);
            }
        }
        break;
    case 5: {
        s32 p = fn_801D05F8(&mSpawnPos, 32.0f);
        if (mStateTimer >= 100 && !(mFlags & 0x2000) && p >= 0) {
            field_0x2b8 = p;
            fn_801BC554(1);
        } else if (mPos.z >= 100.0f) {
            if (field_0x2a8 == 0) {
                fn_801BC554(0);
            } else {
                fn_801BC554(9);
            }
        }
        break;
    }
    case 6:
        if (__fabsf(field_0x2b0.x - mPos.x) < 1.0f && __fabsf(field_0x2b0.y - mPos.y) < 1.0f) {
            if (!fn_8022DD74(field_0x2b8)) {
                mFlags |= 0x8000;
                field_0x2c4 = field_0x2b8;
            }
            fn_801BC554(5);
        }
        break;
    case 7:
        if (mPos.z >= 100.0f) {
            mPos.z = 100.0f;
            field_0x4 = field_0x2ac;
            mPos.x = field_0x2b0.x;
            mPos.y = field_0x2b0.y;
            JGeometry::TVec3f pos;
            pos = JGeometry::TVec3f(mPos.x, mPos.y, mPos.z - 2.0f);
            fn_802316C8(field_0x2b8, field_0x2ac, &pos);
            fn_801BC554(8);
        }
        break;
    case 8:
        if (mPos.z <= 4.0f) {
            mPos.z = 4.0f;
            if (!fn_8022DD74(field_0x2b8)) {
                mFlags |= 0x8000;
                field_0x2c4 = field_0x2b8;
            }
            fn_801BC554(5);
        }
        break;
    case 9:
        if (mStateTimer >= field_0x250) {
            getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &mPos, 3, -1, -1);
            fn_801BC554(10);
        }
        break;
    case 10: {
        bool flag = false;
        if (mParam.mIndex > 2) {
            flag = true;
        } else {
            mParam.field_0x24++;
            if (mParam.field_0x24 >= mParam.field_0x10[mParam.mIndex]) {
                mParam.mIndex++;
                if (mParam.mIndex > 2) {
                    flag = true;
                } else {
                    mParam.mTarget = mParam.mOrder[mParam.mIndex];
                    mParam.field_0x24 = 0;
                }
            }
        }
        if (flag) {
            getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &mPos, 3, -1, -1);
            fn_801BC554(9);
        } else {
            field_0x98 = mParam.mTarget;
            field_0x2b8 = fn_801D05F8(&mPos, 32.0f);
            if (field_0x2b8 >= 0) {
                if (checkPlayerHit()) {
                    if (fn_8022DE10(field_0x2b8, 0)) {
                        field_0x98 = 2;
                        fn_801BC554(4);
                    }
                }
            }
        }
        break;
    }
    }
}

void cls_804A3E44::vfunc_0x88() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    switch (mState) {
    case 1:
    case 2:
        field_0x3c.z = -0.7f;
        if (field_0x2b8 >= 0) {
            JGeometry::TVec2f vec;
            fn_801D00B4(field_0x2b8, &vec, 1.0f);
            field_0x3c.x = vec.x;
            field_0x3c.y = vec.y;
        }
        break;
    case 4:
        field_0x3c.z = 0.3f;
        break;
    case 5:
        field_0x3c.z = 0.5f;
        break;
    case 6: {
        JGeometry::TVec2f vec;
        // fn_801D01A0 reads only (x, y) of both points; field_0x2b0 is an 8-byte TVec2f slot.
        fn_801D01A0((const JGeometry::TVec3f*)&field_0x2b0, &vec, 0.5f);
        field_0x3c.x = vec.x;
        field_0x3c.y = vec.y;
        break;
    }
    case 7:
        field_0x3c.z = 0.3f;
        break;
    case 8:
        field_0x3c.z = -0.5f;
        break;
    case 10:
        EnemyBase_804A4808::vfunc_0x88();
        break;
    }
}

void cls_804A3E44::vfunc_0x78() {
    if (field_0x2b8 >= 0 && fn_8022DCD8(field_0x2b8)) {
        if (!fn_8022DD74(field_0x2b8)) {
            mFlags |= 0x8000;
            field_0x2c4 = field_0x2b8;
        }
        field_0x2b8 = -1;
    }
}
