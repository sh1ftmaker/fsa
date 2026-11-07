#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/Map.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_shadow_link_2.h"

// 0x801C5854: cls_804A46F4's destructor, emitted here.
// FIXME(shotgun): not written; it is the implicit inline destructor of the base classes.

// 0x801C573C.
// FIXME(shotgun): not written; the members at 0x524 (cls_802157B8, fn_80215864) and 0x548
// (cls_802153A8, fn_80215578) are constructed by functions not yet named as constructors.

// 0x801C5680.
// FIXME(shotgun): 100 %, matches
ShadowLink2_804A43E8::~ShadowLink2_804A43E8() {
    fn_801C0D70()->fn_801C1440(mSerial);
}

// Packs five fields into one word: bits 0-4, 5-7, 8-11, 12-15, 20-27.
u32 fn_801C5660(u32 a, u32 b, u32 c, u32 d, u32 e) {
    return ((e & 0xFF) << 20) | ((d & 0xF) << 12) | ((c & 0xF) << 8) | ((b & 0x7) << 5) | (a & 0x1F);
}

// 0x801C544C.
// FIXME(shotgun): 66.85 %, different + reloc-name
void ShadowLink2_804A43E8::init() {
    EnemyBase_804A4808::init();
    field_0x94 = fn_801C0D70()->fn_801C14A8(mSerial);
    u32 kind = (field_0x90 >> 5) & 7;
    if (kind == 1 || kind == 2) {
        mFlags |= 0x6000;
        fn_801C0D70()->field_0x54.set(mPos.x, mPos.y, mPos.z);
        switch (kind) {
        case 1:
            fn_801C0D70()->field_0x0 |= 2;
            break;
        case 2:
            fn_801C0D70()->field_0x0 |= 4;
            break;
        }
    } else if (kind == 3) {
        mFlags |= 0x4000;
    }
    if (mFlags & 0x4000) {
        fn_801C0D70()->field_0x2++;
    }
    field_0x55c[2] = (field_0x90 >> 20) & 0xFF;
    if (field_0x55c[2] == 0) {
        field_0x55c[2] = 10;
    }
    s32 n = field_0x90 & 0x1F;
    if (n > 8) {
        n = 8;
    } else if (n < 0) {
        n = 0;
    }
    // FIXME(shotgun): field_0x524 is a cls_802157B8 here: field_0x524.fn_802157B8((u8)n) is not
    // written until the header member has that type (other units test it as a u32).
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2 = mBox;
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0x50303;
    mFlags |= 0x100;
    mDrawEnabled = false;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x114 = 2;
    field_0x196 = 1;
    field_0x33c = 0;
    field_0x98 = 2;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    field_0x340.fn_800FEB78(0);
    field_0x32c.set(mPos.x, mPos.y, mPos.z);
}

// Member: calls fn_801C12B8 on the fn_801C0D70 object when that object's
// word at 0x50 has its top bit set and mFlags has bit 0x2000.
void ShadowLink2_804A43E8::fn_801C53F8() {
    bool clear = (fn_801C0D70()->field_0x50 >> 31) == 0;
    if (clear) {
        return;
    }
    if (mFlags & 0x2000) {
        fn_801C0D70()->fn_801C12B8();
    }
}

// Member: true when mFlags has bit 0x40000 set and the byte at 0x55C is at most 0x14.
bool ShadowLink2_804A43E8::fn_801C53C4() {
    u8 result = 0;
    if (mFlags & 0x40000) {
        if (field_0x55c[0] <= 0x14) {
            result = true;
        }
    }
    return result;
}

// FIXME(shotgun): 99 %, reloc-name
bool ShadowLink2_804A43E8::fn_801C5348(f32 range) {
    bool near = false;
    s32 player = fn_801D05F8(&mPos, 0.0f);
    if (player >= 0 && fn_801D0D50(player, &mPos, range)) {
        near = true;
    }
    return near;
}

// Takes no arguments and ignores r3; callers test the result as a byte.
bool fn_801C5340() {
    return 0;
}

// 0x801C50A8. Leaves the old state (mState) for `state`.
// FIXME(shotgun): 77.03 %, different + reloc-name
void ShadowLink2_804A43E8::fn_801C50A8(int state) {
    mFlags &= ~0x40000;
    field_0x55c[0] = 0;
    if (state != 0x12 && mState != 0x12 && state != 0x13 && mState != 0x13 && state != 0x14 &&
        mState != 0x14 && state != 0x15 && mState != 0x15 && state != 0x16 && mState != 0x16 &&
        state != 0x17 && mState != 0x17 && state != 0x18 && mState != 0x18) {
        field_0x55c[1] = 0;
    }
    if (mState == 0x11) {
        field_0x598 = 0;
    }
    mHitBox.mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    switch (mState) {
    case 0x10:
        field_0x4ec.fn_80214900();
        mFlags &= ~0x00800000;
        break;
    case 0x11:
    case 0x22:
        mFlags &= ~0x1000;
        break;
    case 0x14:
    case 0x1C:
    case 0x1E:
        mFlags &= ~0x100;
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox2 = mBox;
        field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
        break;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        mFlags &= ~0x1100;
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox2 = mBox;
        mFlags |= 0x40000;
        if (mState == 0x17) {
            field_0x59b = 6;
        } else {
            if (mState == 0x19) {
                field_0x560 = fn_801CD664(4);
            }
            field_0x4 = field_0x57c;
            mPos = field_0x580;
            mPrevPos = field_0x580;
        }
        break;
    case 0x20:
        mFlags &= ~0x80000;
        break;
    case 0x21:
        mFlags &= ~0x04000000;
        break;
    case 0x2B:
    case 0x2C:
    case 0x2D:
    case 0x2F:
        mFlags &= ~0x100;
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox2 = mBox;
        break;
    }
}

// 0x801C3B7C.
// FIXME(shotgun): not written yet

// 0x801C2ED4.
// FIXME(shotgun): not written yet

// Member: copies field_0x98 into the sequencer's direction, advances the
// sequencer unless it is in state 0x13 past its first tick or mFlags has bit
// 0x80000, then updates the attack hit box.
void ShadowLink2_804A43E8::fn_801C2E70() {
    field_0x340.mDir = field_0x98;
    if (mState != 0x13 || mStateTimer <= 1) {
        if (!(mFlags & 0x80000)) {
            field_0x340.fn_800FE8C0();
        }
    }
    mHitBox.fn_80216A8C();
}

// 0x801C2D84.
// FIXME(shotgun): 100 %, matches
void ShadowLink2_804A43E8::vfunc_0x9C(u8 mapHit) {
    fn_801CE2BC(mapHit);
    if (field_0x59c != 0) {
        field_0x59c--;
    }
    if (field_0x564 == 3 && field_0x59e != 0) {
        switch (mState) {
        case 0x10:
        case 0x2E:
            field_0x59e--;
            break;
        }
    }
    switch (mState) {
    case 0x24:
    case 0x25:
    case 0x26:
        if (field_0x340.mState == 9 && field_0x340.mIndex == 0 && field_0x340.mTimer == 5) {
            startSound_801F0E34(0x400C4, 0);
            startSound_801F0E34(0x400C5, 0);
            field_0x4ec.fn_80214578();
        }
        break;
    }
}

// Gravity override (EnemyBase's vfunc_0x84): z changes by lbl_8053EFB8 per call,
// skipped while field_0xd8 > 0 and field_0xf8 == 2; clamped at -lbl_8053EFBC.
void ShadowLink2_804A43E8::vfunc_0x84() {
    if (field_0xd8 > 0 && field_0xf8 == 2) {
        return;
    }
    f32 minZ = -lbl_8053EFBC;
    field_0x3c.z += lbl_8053EFB8;
    if (field_0x3c.z < minZ) {
        field_0x3c.z = minZ;
    }
}

// 0x801C2BF4.
// FIXME(shotgun): 81.51 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::vfunc_0x88() {
    switch (mState) {
    case 6:
    case 0x14:
    case 0x1E:
        field_0x3c.z += lbl_8053EFB8;
        return;
    }
    f32 speed = mSpeed;
    switch (mState) {
    case 0x20:
        if (mPos.z <= 0.0f) {
            field_0x3c.x *= 0.78f;
            field_0x3c.y *= 0.78f;
        }
        break;
    case 0x11:
        field_0x3c.x = JMath::sincosTable_.sinShort(field_0x33c) * speed;
        field_0x3c.y = JMath::sincosTable_.cosShort(field_0x33c) * mSpeed;
        break;
    default:
        if (mState == 0x1C) {
            switch (field_0x340.mIndex) {
            case 0:
            case 8:
                speed = 0.0f;
                break;
            default:
                speed = 2.0f;
                break;
            }
        }
        field_0x3c.x = JMath::sincosTable_.sinShort(field_0x33c) * speed;
        field_0x3c.y = JMath::sincosTable_.cosShort(field_0x33c) * speed;
        break;
    }
    vfunc_0x84();
}

// 0x801C2A84.
// FIXME(shotgun): 90.67 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::vfunc_0x98() {
    switch (mState) {
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x2F:
        field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        return;
    }
    u32 flags = mFlags;
    if (!(flags & 7)) {
        fn_801CE32C();
    } else if (!(flags & 2)) {
        fn_801CE5D4();
    }
    field_0x4ec.fn_80214CE4();
    field_0x4ec.fn_80214994();
    if ((field_0x524 & 1) && !fn_801CD2F8(field_0x4) && mPos.z <= 0.0f) {
        switch (mState) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 0xF: case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16:
        case 0x17: case 0x18: case 0x19: case 0x1A:
        case 0x29:
            break;
        default:
            if (fn_8022461C(&mPos, field_0x4) == 0 && fn_80226850(&mPos, field_0x4) != 0xAD) {
                gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0xAD);
                startSound_801F0E34(0x3008A, 0);
            }
            break;
        }
    }
}

// 0x801C2960.
// FIXME(shotgun): 100 %, matches
void ShadowLink2_804A43E8::vfunc_0x74() {
    mFlags &= ~0x02000000;
    switch (mState) {
    case 0x10:
        if (!(mFlags & 0x00800000)) {
            field_0x4ec.fn_80214AF8();
        }
        break;
    case 0x21:
        if (fn_8021D020()) {
            switch (field_0x564) {
            case 0:
                if (!(mFlags & 0x10000)) {
                    fn_801C3B7C(0x29);
                } else if (field_0x524 & 0x80) {
                    fn_8021CDE4(1);
                } else {
                    fn_8021CDE4(2);
                }
                break;
            case 1:
                fn_8021CDE4(2);
                break;
            case 2:
                fn_8021CDE4(3);
                break;
            case 3:
                fn_801C3B7C(0x29);
                break;
            }
        } else {
            fn_801C3B7C(0x1B);
        }
        break;
    }
}

// 0x801C283C.
// FIXME(shotgun): 91.56 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::vfunc_0xA0() {
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 0xD:
    case 0x17:
        return;
    case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 0xA: case 0xB: case 0xC:
        if (mFlags & 0x2000) {
            cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
            if (!obj->field_0x284) {
                obj->field_0x284 = 1;
                obj->field_0x28c = 0.05f;
            }
            Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
            if (camera->field_0x7f5 == 0) {
                camera->field_0x7f4 = 1;
                camera->field_0x7f8 = 4;
                camera->field_0x7fc.set(mPos.x, mPos.y, 0.0f);
                camera->field_0x7f6 = 0;
            }
        }
        break;
    default: {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        camera->addFocusPoint_8030C210(&mPos);
        if (gGameRoot2_8053AB10->field_0x24->field_0x179 == 0) {
            gGameRoot2_8053AB10->field_0x24->field_0x178 = 1;
        }
        break;
    }
    }
    lbl_8053AED0->fn_8043E7CC(mPos);
}

// FIXME(shotgun): 72 %, missing/extra
void ShadowLink2_804A43E8::fn_801C27F0() {
    bool flag = (mFlags & 0x08000000) != 0;
    cls_801C0D70* tracker = fn_801C0D70();
    tracker->fn_801C1374(flag);
    vfunc_0x24();
}

// Member: switches to state 2 when mState is 0 or 1 and mFlags has bit 0x4000.
void ShadowLink2_804A43E8::fn_801C27A8() {
    if (!(mFlags & 0x4000)) {
        return;
    }
    if (mState >= 2 || mState < 0) {
        return;
    }
    fn_801C3B7C(2);
}

// FIXME(shotgun): 100 %, matches
void ShadowLink2_804A43E8::fn_801C272C() {
    mDrawEnabled = true;
    if (fn_801C0D70()->fn_801C11F0()) {
        fn_801C3B7C(6);
    } else if (fn_801C0D70()->field_0x0 & 1) {
        fn_801C3B7C(7);
    } else {
        fn_801C3B7C(9);
    }
}

// Member: switches to state 0xE unless the actor is already in it.
void ShadowLink2_804A43E8::fn_801C26FC() {
    if (mState != 0xE) {
        fn_801C3B7C(0xE);
    }
}

// Member: r3 (this) is passed straight through to fn_801C3B7C.
void ShadowLink2_804A43E8::fn_801C26D8() {
    fn_801C3B7C(0xD);
}
