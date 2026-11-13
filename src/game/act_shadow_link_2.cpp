#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/Map.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_shadow_link_2.h"

const u32 lbl_80469450[4] = {
    0x000400BB,
    0x000400BC,
    0x000400BD,
    0x000400BE,
};

// 0x801C5854: cls_804A46F4's destructor, emitted here.
// FIXME(shotgun): not written; it is the implicit inline destructor of the base classes.

// 0x801C573C.
// FIXME(shotgun): 99.93 %, reloc-name; behaviour clean.
ShadowLink2_804A43E8::ShadowLink2_804A43E8() {
    field_0x550.fn_80217484();
    field_0x55c[0] = 0;
    field_0x55c[1] = 0;
    field_0x55c[2] = 0;
    field_0x560 = 4;
    field_0x564 = 0;
    field_0x568 = lbl_8046A748[0][0];
    field_0x56c = 0;
    field_0x570.x = 0.0f;
    field_0x570.y = 0.0f;
    field_0x570.z = 0.0f;
    field_0x57c = 0;
    field_0x580.x = 0.0f;
    field_0x580.y = 0.0f;
    field_0x580.z = 0.0f;
    field_0x58c = 0;
    field_0x590 = -1;
    field_0x594 = 0;
    field_0x595 = 0;
    field_0x596 = 0;
    field_0x597 = 0;
    field_0x598 = 0;
    field_0x599 = 0;
    field_0x59a = 0;
    field_0x59b = 0;
    field_0x59c = 0;
    field_0x59e = 0;
    field_0x5a0 = 0;
    field_0x5a2 = 0;
    field_0x5a4 = 0.0f;
    field_0x5a8 = 0.0f;
    field_0x5ac = 0.0f;
    field_0x5b0 = 0.0f;
    field_0x4ec.mOwner = this;
    mHitBox.mOwner = this;
    mHelper540.mOwner = this;
}

// 0x801C5680.
ShadowLink2_804A43E8::~ShadowLink2_804A43E8() {
    fn_801C0D70()->fn_801C1440(mSerial);
}

// Packs five fields into one word: bits 0-4, 5-7, 8-11, 12-15, 20-27.
u32 fn_801C5660(u32 a, u32 b, u32 c, u32 d, u32 e) {
    return ((e & 0xFF) << 20) | ((d & 0xF) << 12) | ((c & 0xF) << 8) | ((b & 0x7) << 5) | (a & 0x1F);
}

// 0x801C544C.
// FIXME(shotgun): 87.74 %, missing/extra + reloc-name; behaviour drift.
void ShadowLink2_804A43E8::init() {
    EnemyBase_804A4808::init();
    field_0x94 = fn_801C0D70()->fn_801C14A8(mSerial);
    u32 kind = (field_0x90 >> 5) & 7;
    if (kind == 1 || kind == 2) {
        mFlags |= 0x6000;
        cls_801C0D70* obj = fn_801C0D70();
        obj->field_0x54.set(mPos);
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
    field_0x524.fn_802157B8(n);
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
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

// FIXME(shotgun): 99.84 %, reloc-name; behaviour clean.
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
// Remaining diff: the float constants are literals where the original uses lbl_ names.
// FIXME: 99.76 %, reloc-name; behaviour drift.
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
        mBox2.set(mBox);
        field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
        break;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        mFlags &= ~0x1100;
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox2.set(mBox);
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
        mBox2.set(mBox);
        break;
    default:
        break;
    }
}

// 0x801C3B7C.
// FIXME(shotgun): 67.55 %, missing/extra + reloc-name; behaviour drift.
void ShadowLink2_804A43E8::fn_801C3B7C(int state) {
    fn_801C50A8(state);
    switch (state) {
    case 0:
    case 1:
        break;
    case 2:
        field_0x250 = 1;
        if (!(mFlags & 0x2000) && (mFlags & 0x4000)) {
            if (fn_801C0D70()->field_0x2 >= 2) {
                field_0x250 += fn_801CD664(0x2D);
            }
        }
        break;
    case 3:
        if (fn_801C0D70()->field_0x50 >= 0) {
            field_0x250 = 0xA0;
        } else {
            field_0x250 = 0xEB;
        }
        field_0x4ec.fn_80214740(field_0x4, &mPos, false);
        break;
    case 4:
        mDrawEnabled = true;
        if (fn_801C0D70()->field_0x50 >= 0) {
            mPos.y += 32.0f;
        }
        field_0x3c.z = 4.5f;
        field_0x340.fn_800FEB78(4);
        startSound_801F0E34(0x40035, 0);
        if (getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
            field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
        }
        break;
    case 5:
        if (fn_801C0D70()->fn_801C11F0() || (fn_801C0D70()->field_0x0 & 1)) {
            field_0x250 = 0x3C;
        } else {
            field_0x250 = 0xF;
        }
        field_0x340.fn_800FEB78(0);
        break;
    case 6: {
        if (fn_801C0D70()->field_0x0 & 2) {
            if (mFlags & 0x2000) {
                startSound_801F0E34(0x400D7, 0);
            }
            field_0x250 = 0x1E;
        } else if (fn_801C0D70()->field_0x0 & 4) {
            if (mFlags & 0x2000) {
                startSound_801F0E34(0x4003A, 0);
            }
            field_0x250 = 0x3C;
        }
        if (!(mFlags & 0x4000)) {
            if (getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
                field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
            }
            mPos = fn_801C0D70()->field_0x54;
            mPrevPos = fn_801C0D70()->field_0x54;
            field_0x3c.x = (mSpawnPos.x - mPos.x) * (1.0f / (f32)field_0x250);
            field_0x3c.y = (mSpawnPos.y - mPos.y) * (1.0f / (f32)field_0x250);
            if (fn_801C0D70()->field_0x0 & 2) {
                field_0x3c.z = 0.0f;
                field_0x33c = 0;
                field_0x340.fn_800FEB78(0);
            } else if (fn_801C0D70()->field_0x0 & 4) {
                field_0x3c.z = -lbl_8053EFB8 * (f32)field_0x250 * 0.5f;
                field_0x33c = JMath::atanTable_.atan2_(field_0x3c.x, field_0x3c.y) * 10430.378f;
                field_0x340.fn_800FEB78(4);
            }
        }
        break;
    }
    case 7:
    case 8:
    case 9:
    case 0xB:
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        if (state != 0xB && !(fn_801C0D70()->field_0x50 < 0)) {
            mPos.set(mSpawnPos.x, mSpawnPos.y, mSpawnPos.z);
        }
        if (field_0x340.mState != 0) {
            field_0x340.fn_800FEB78(0);
        }
        break;
    case 0xA:
        mTargetSpeed = 0.0f;
        mSpeed = 1.0f;
        field_0x33c = 0;
        if (field_0x340.mState != 1) {
            field_0x340.fn_800FEB78(1);
        }
        break;
    case 0xC:
        if (mFlags & 0x2000) {
            lbl_8053AED0->fn_8043D67C(0x19B, 0, 0);
            gAudioMgr_8053AF80->fn_80458FF0(0x100000D, true, 0);
            getGameView_802D6A9C()->fn_802D6BC8(1);
        }
        break;
    case 0xD:
        if (field_0x94 % 4 == 0) {
            field_0x4ec.fn_80214740(field_0x4, &mPos, true);
        }
        break;
    case 0xE:
    case 0xF:
    case 0x10:
        if (state == 0xE) {
            if (mFlags & 0x2000) {
                if (fn_801C0D70()->field_0x50 >= 0) {
                    fn_801C0D70()->field_0x0 |= 0x40;
                }
                getActorManager_801F856C()->fn_801F6884();
                fn_8022ADE4();
            }
            mDrawEnabled = true;
            if (!(field_0x524.field_0x0 & 2) && !(fn_801C0D70()->field_0x50 < 0)) {
                mPos.set(mSpawnPos.x, mSpawnPos.y, mSpawnPos.z);
            }
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            mFlags &= ~0x100;
            mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
            mBox2 = mBox;
            field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
            field_0x358.fn_8021425C(0, (field_0x90 >> 8) & 0xF, mPos, field_0x4);
            field_0x358.fn_80214254(0);
            field_0x250 = fn_801CD664(0x1A) + 5;
            field_0x550.fn_80217484();
        } else if (state == 0x10) {
            u16 minVal = lbl_8046A748[field_0x564][3];
            u16 maxVal = lbl_8046A748[field_0x564][4];
            field_0x250 = minVal + fn_801CD664(maxVal - minVal + 1);
            mFlags &= ~0x00800000;
        } else if (state == 0xF) {
            field_0x250 = fn_801CD664(0x10) + 0xF;
            field_0x550.fn_80217484();
        }
        mSpeed = 0.0f;
        if (field_0x340.mState != 0) {
            field_0x340.fn_800FEB78(0);
        }
        field_0x5a8 = 0.0f; // FIXME(shotgun): field_0x5a4 and u16s at 0x5a0/0x5a2 not typed in header
        break;
    case 0x11:
        if (mState != 0x17 && mState != 0x18) {
            mSpeed = 1.0f;
            if (field_0x340.mState != 1) {
                field_0x340.fn_800FEB78(1);
            }
            if (mState != 0x16) {
                field_0x5a8 = (f32)(fn_801CD664(0x51) + 0x78);
                mPathCount = 0;
                fn_801C7F98();
                fn_801C7EE4();
            }
        }
        break;
    case 0x12:
        if (fn_801C5348(32.0f)) {
            field_0x250 = 2;
        } else {
            field_0x250 = 0x5A;
        }
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        if (field_0x340.mState != 0) {
            field_0x340.fn_800FEB78(0);
        }
        field_0x33c = JMath::atanTable_.atan2_(field_0x32c.x - mPos.x, field_0x32c.y - mPos.y) * 10430.378f;
        mFlags &= ~0x20000;
        break;
    case 0x13:
        if (fn_801C5348(32.0f)) {
            field_0x250 = 2;
        } else {
            field_0x250 = 0x1E;
        }
        mSpeed = 0.0f;
        field_0x340.fn_800FEB78(4);
        break;
    case 0x14:
        mSpeed = 0.0f;
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
        field_0x3c.x = (field_0x32c.x - mPos.x) * (1.0f / 60.0f);
        field_0x3c.y = (field_0x32c.y - mPos.y) * (1.0f / 60.0f);
        field_0x3c.z = 0.5f * (60.0f * -lbl_8053EFB8);
        field_0x33c = JMath::atanTable_.atan2_(field_0x3c.x, field_0x3c.y) * 10430.378f;
        startSound_801F0E34(0x40036, 0);
        break;
    case 0x15:
    case 0x1F:
        if (state == 0x15) {
            if (fn_801C5348(32.0f)) {
                field_0x250 = 2;
            } else {
                field_0x250 = 0x1E;
            }
        } else {
            u16 minVal = lbl_8046A748[field_0x564][1];
            u16 maxVal = lbl_8046A748[field_0x564][2];
            field_0x250 = minVal + fn_801CD664(maxVal - minVal + 1);
        }
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        field_0x340.mIndex = 8;
        mFlags &= ~0x100;
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox2 = mBox;
        field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
        break;
    case 0x16:
        if (fn_801C5348(32.0f)) {
            field_0x250 = 2;
        } else {
            field_0x250 = 0x5A;
        }
        mSpeed = 0.0f;
        if (field_0x340.mState != 0) {
            field_0x340.fn_800FEB78(0);
        }
        break;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        mSpeed = 1.0f;
        mPathCount = 0;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        mFlags |= 0x1100;
        mFlags &= ~0x20000;
        if (state != 0x17) {
            if (field_0x58c == 1) {
                field_0x4ec.fn_80214740(field_0x57c, &field_0x580, true);
            } else {
                startSound_801F0E34(0x4004A, 0);
            }
        }
        break;
    case 0x1B:
        if (mState == 0x11) {
            field_0x599 = 0x1E;
        }
        mSpeed = 1.0f;
        if (field_0x340.mState != 1) {
            field_0x340.fn_800FEB78(1);
        }
        field_0x5ac = (f32)(fn_801CD664(0x10) + 0x41);
        field_0x595 = 0;
        field_0x596 = 1;
        break;
    case 0x1C:
        mSpeed = 0.0f;
        field_0x340.fn_800FEB78(4);
        field_0x3c.z = 4.5f;
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        if (mState != 0x2A) {
            startSound_801F0E34(0x40036, 0);
        }
        break;
    case 0x1D:
        mSpeed = 0.0f;
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        if (fn_801C0D70()->field_0x0 & 0x400) {
            field_0x4ec.fn_80214E50();
            field_0x59c = 0x3C;
            if (field_0x340.mState != 0) {
                field_0x340.fn_800FEB78(0);
            }
        }
        break;
    case 0x1E:
        if (mState == 0xD) {
            mDrawEnabled = true;
            if (!(field_0x524.field_0x0 & 2) && !(fn_801C0D70()->field_0x50 < 0)) {
                mPos.set(mSpawnPos.x, mSpawnPos.y, mSpawnPos.z);
            }
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            mFlags &= ~0x100;
            mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
            mBox2 = mBox;
            field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
            field_0x358.fn_8021425C(0, (field_0x90 >> 8) & 0xF, mPos, field_0x4);
            field_0x358.fn_80214254(0);
        }
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
        field_0x340.fn_800FEB78(4);
        if (mState == 0xD) {
            s16 angle = -((s16)field_0x94 / 4 + 0x2000);
            f32 s = JMath::sincosTable_.sinShort(angle);
            f32 c = JMath::sincosTable_.cosShort(angle);
            field_0x32c = mSpawnPos;
            field_0x32c.x += 90.0f * s - 0.0f * c;
            field_0x32c.y += 90.0f * c + 0.0f * s;
            startSound_801F0E34(0x40035, 0);
        } else if (fn_801C0D70()->field_0x0 & 0x400) {
            startSound_801F0E34(0x40036, 0);
        } else {
            fn_801D3320();
            startSound_801F0E34(0x40036, 0);
        }
        field_0x3c.x = (field_0x32c.x - mPos.x) * (1.0f / 60.0f);
        field_0x3c.y = (field_0x32c.y - mPos.y) * (1.0f / 60.0f);
        field_0x3c.z = 0.5f * (60.0f * -lbl_8053EFB8);
        field_0x33c = JMath::atanTable_.atan2_(field_0x3c.x, field_0x3c.y) * 10430.378f;
        break;
    case 0x20:
        field_0x250 = 10;
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        break;
    case 0x21:
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        if (mFlags & 0x04000000) {
            if (field_0x340.mState != 0) {
                field_0x340.fn_800FEB78(0);
            }
        } else {
            s16 ang = JMath::atanTable_.atan2_(field_0x3c.x, field_0x3c.y) * 10430.378f;
            field_0x33c = ang - 0x8000;
            field_0x340.fn_800FEB78(5);
        }
        startSound_801F0E34(0x400C2, 0);
        break;
    case 0x22:
        field_0x250 = fn_801CD664(0x10) + 5;
        mSpeed = 1.0f;
        if (field_0x340.mState != 1) {
            field_0x340.fn_800FEB78(1);
        }
        mPathCount = 0;
        mFlags |= 0x1000;
        field_0x5b0 = (f32)(fn_801CD664(0x15) + 0x14);
        break;
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        if (state == 0x23) {
            startSound_801F0E34(0x40000, 0);
            startSound_801F0E34(0x400CC, 0);
            field_0x340.fn_800FEB78(2);
        } else {
            field_0x340.fn_800FEB78(9);
        }
        field_0x594 = 0;
        break;
    case 0x27:
    case 0x28:
        if (state == 0x27) {
            field_0x250 = fn_801CD664(1) + 0x5A;
        } else {
            field_0x250 = fn_801CD664(1) + 0x5A;
        }
        mSpeed = 0.0f;
        if (field_0x340.mState != 3 && field_0x340.mState != 0) {
            field_0x340.fn_800FEB78(0);
        }
        break;
    case 0x29:
        mSpeed = 0.0f;
        if (field_0x340.mState != 0) {
            field_0x340.fn_800FEB78(0);
        }
        mLife = mLifeBackup;
        mFlags |= 0x300;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        field_0xb0 = 0;
        startSound_801F0E34(0x40037, 0);
        fn_801C0D70()->field_0x6++;
        if (fn_801C0D70()->fn_801C1314()) {
            mFlags |= 0x08000000;
            gGameRoot_8053AAF8->fn_80134DE4(0xA0, 0);
        } else if (mFlags & 0x10000) {
            gGameRoot_8053AAF8->fn_80134DE4(3, 0);
        }
        break;
    case 0x2A:
        mSpeed = 0.0f;
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        break;
    case 0x2B:
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        field_0x33c = 0;
        if (field_0x340.mState != 7) {
            field_0x340.fn_800FEB78(7);
        }
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        break;
    case 0x2C:
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        if (mState == 0x2B || mState == 0x1E) {
            if (field_0x340.mState != 0) {
                field_0x340.fn_800FEB78(0);
            }
        }
        if (field_0x560 == 4) {
            field_0x560 = fn_801CD664(4);
        }
        // FIXME(shotgun): field_0x548 is cls_802153A8 (fn_80215558((u8)field_0x560)), not typed in header
        if (field_0x560 >= 0 && field_0x560 <= 3) {
            startSound_801F0E34(lbl_80469450[field_0x560], 0);
        }
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        break;
    case 0x2D:
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        field_0x33c = 0;
        field_0x340.fn_800FEB78(8);
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        startSound_801F0E34(0x40036, 0);
        break;
    case 0x2E:
        mSpeed = 0.0f;
        field_0x33c = 0;
        if (field_0x340.mState != 7) {
            field_0x340.fn_800FEB78(7);
        }
        break;
    case 0x2F:
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        if (field_0x340.mState != 0) {
            field_0x340.fn_800FEB78(0);
        }
        mFlags |= 0x100;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
        mPathCount = 0;
        break;
    }
    mStateTimer = 0;
    mState = state;
    field_0x98 = fn_801F681C(field_0x33c);
}

// 0x801C2ED4.
// FIXME(shotgun): 81.62 %, missing/extra + reloc-name; behaviour drift.
void ShadowLink2_804A43E8::vfunc_0x90(u8 mapHit) {
    if (mapHit) {
        if (vfunc_0x4C(field_0x4, &mPos)) {
            mPos.x = mPrevPos.x;
            mPos.y = mPrevPos.y;
            JGeometry::TVec2f nudge;
            fn_801D01A0(&field_0x32c, &nudge, 8.0f);
            mPos.x += nudge.x;
            mPos.y += nudge.y;
        }
    }
    if ((fn_801C0D70()->field_0x0 & 0x400) && fn_801D0DD4()) {
        switch (mState) {
        case 0xE:
        case 0xF:
        case 0x11:
        case 0x1B:
        case 0x22:
        case 0x27: {
            JGeometry::TVec2f out;
            if (fn_801C0D70()->fn_801C0FEC(&mPos, &out)) {
                if (fn_801C0D70()->field_0x0 & 0x800) {
                    field_0x32c.set(out.x, out.y, 0.0f);
                    fn_801C3B7C(0x1D);
                }
            } else {
                fn_801C3B7C(0x2F);
            }
            break;
        }
        }
    }
    mHelper540.fn_802188B0();
    if (mFlags & 0x40000) {
        if (field_0x55c[0] != 0xFF) {
            field_0x55c[0]++;
        }
    } else {
        field_0x55c[0] = 0;
    }
    if (field_0x597 != 0) {
        field_0x597--;
    }
    if (field_0x598 != 0) {
        field_0x598--;
    }
    if (field_0x599 != 0) {
        field_0x599--;
    }
    if (field_0x59a != 0) {
        field_0x59a--;
    }
    if (field_0x59b != 0) {
        field_0x59b--;
    }
    switch (mState) {
    case 0:
        if ((mFlags & 0x2000) && fn_801CD228(field_0x4)) {
            if (fn_801D05F8(&mPos, 16.0f * (f32)field_0x55c[2]) >= 0) {
                fn_801C3B7C(1);
            }
        }
        break;
    case 1:
        if (getActorManager_801F856C()->fn_801F68E0() != 0) {
            gAudioMgr_8053AF80->fn_80458F9C(0x3C);
            fn_801C0D70()->field_0x0 |= 8;
        }
        break;
    case 2:
        if (mStateTimer >= field_0x250) {
            fn_801C3B7C(3);
        }
        break;
    case 3:
        if (mStateTimer >= field_0x250) {
            fn_801C3B7C(4);
        }
        break;
    case 4:
        if (mStateTimer > 1 && fn_801D0DD4()) {
            fn_801C3B7C(5);
        }
        break;
    case 5:
        if (mStateTimer >= field_0x250) {
            mFlags |= 0x8000;
            fn_801C0D70()->field_0x0 |= 0x10;
        }
        break;
    case 6:
        if (mStateTimer >= field_0x250) {
            field_0x33c = 0;
            if (!(fn_801C0D70()->field_0x0 & 1)) {
                fn_801C3B7C(9);
            } else {
                fn_801C3B7C(7);
            }
        }
        break;
    case 7:
        if (mStateTimer >= 0x1E) {
            fn_801C3B7C(8);
        }
        break;
    case 8:
        if (mStateTimer >= 0x5A) {
            fn_801C3B7C(9);
        }
        break;
    case 9:
        if (mStateTimer >= 0x3C) {
            if (field_0x524.field_0x0 & 2) {
                fn_801C3B7C(0xA);
            } else {
                fn_801C3B7C(0xC);
            }
        }
        break;
    case 0xA:
        if (mStateTimer >= 0x73) {
            fn_801C3B7C(0xB);
        }
        break;
    case 0xB:
        if (mStateTimer >= 0x3C) {
            fn_801C3B7C(0xC);
        }
        break;
    case 0xC:
        if ((mFlags & 0x2000) && lbl_8053AED0->field_0x10a == 0) {
            fn_801C0D70()->field_0x0 |= 0x20;
        }
        break;
    case 0xD:
        if (mStateTimer >= 0xA0) {
            mPos.y += 32.0f;
            fn_801C3B7C(0x1E);
        }
        break;
    case 0xE:
    case 0xF:
        if (mStateTimer >= field_0x250) {
            // FIXME(shotgun): field_0x5a4 not typed in header
            if (field_0x59c == 0) {
                field_0x4ec.fn_80214E50();
                field_0x59c = 0x3C;
            }
            fn_801C3B7C(0x11);
        } else {
            field_0x33c = field_0x550.fn_80217374(field_0x33c);
        }
        break;
    case 0x10:
        if (mStateTimer >= field_0x250) {
            if (field_0x564 == 3) {
                fn_801C3B7C(0x2E);
            } else {
                fn_801C3B7C(0xF);
            }
        } else if (mStateTimer % 6 == 0) {
            field_0x33c += 0x4000;
        }
        break;
    case 0x11:
        fn_801C7370(mapHit);
        break;
    case 0x12:
        if (mStateTimer >= field_0x250 || mHelper540.fn_802189AC()) {
            fn_801C3B7C(0x13);
        }
        break;
    case 0x13:
        if (fn_801D0DD4()) {
            if (mStateTimer >= field_0x250 || (mStateTimer >= 2 && mHelper540.fn_802189AC())) {
                fn_801C3B7C(0x14);
            }
        }
        break;
    case 0x14:
        if (field_0x340.mIndex == 7) {
            mFlags &= ~0x100;
            if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0) {
                mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
                mBox2 = mBox;
            }
        }
        if (mStateTimer > 1 && mPos.z <= 0.0f) {
            field_0x4ec.fn_802144F0();
            fn_801C3B7C(0x15);
        }
        break;
    case 0x15:
        if (mStateTimer >= field_0x250) {
            fn_801C3B7C(0x16);
        }
        break;
    case 0x16:
        if (mStateTimer >= field_0x250) {
            fn_801C3B7C(0x11);
        }
        break;
    case 0x17:
        if (mStateTimer >= 0x6E) {
            fn_801C3B7C(0x11);
        }
        break;
    case 0x18:
    case 0x19:
    case 0x1A:
        if (mStateTimer >= 0x73) {
            if (mState == 0x19 || mState == 0x1A) {
                fn_801C3B7C(0xF);
            } else {
                JGeometry::TVec3f savedPos = mPos;
                s32 savedLayer = field_0x4;
                field_0x4 = field_0x57c;
                mPos = field_0x580;
                s32 p = fn_801D05F8(&mPos, 60.0f);
                field_0x4 = savedLayer;
                mPos = savedPos;
                if (p >= 0) {
                    fn_801C3B7C(0x1E);
                } else {
                    fn_801C3B7C(0x11);
                }
            }
        } else {
            if (mStateTimer == 0x46 && field_0x58c == 0) {
                field_0x4ec.fn_802146A4();
            }
            if (mStateTimer % 4 == 0) {
                field_0x33c += 0x4000;
            }
        }
        break;
    case 0x1B:
        fn_801C5B08(mapHit);
        break;
    case 0x1C:
        if (field_0x340.mIndex == 7) {
            mFlags &= ~0x100;
            if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0) {
                mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
                mBox2 = mBox;
            }
        }
        if (mStateTimer > 1 && fn_801D0DD4()) {
            fn_801C3B7C(0x1F);
        }
        break;
    case 0x1D:
        if (fn_801D0DD4()) {
            if (fn_801C0D70()->field_0x0 & 0x400) {
                if (mStateTimer > 0x1E) {
                    fn_801C3B7C(0x1E);
                }
            } else {
                if (mStateTimer > 1) {
                    fn_801C3B7C(0x1E);
                }
            }
        }
        break;
    case 0x1E:
        if (mStateTimer > 1 && mPos.z <= 0.0f) {
            field_0x4ec.fn_802144F0();
            if (mFlags & 0x01000000) {
                mFlags &= ~0x01000000;
                fn_801C3B7C(0x2C);
            } else if (mFlags & 0x02000000) {
                fn_801C3B7C(0x11);
            } else {
                fn_801C3B7C(0x1F);
            }
        }
        break;
    case 0x1F:
        if (mStateTimer >= field_0x250) {
            fn_801C3B7C(0x1B);
        }
        break;
    case 0x20:
    case 0x27:
    case 0x28:
        if (mStateTimer >= field_0x250) {
            if ((field_0x524.field_0x0 & 8) && field_0x597 == 0 && gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.7f &&
                fn_801D183C(64.0f)) {
                fn_801C3B7C(0x22);
            } else {
                field_0x597 = 0x3C;
                fn_801C3B7C(0x1B);
            }
        }
        break;
    case 0x21:
        if ((mFlags & 0x04000000) && mStateTimer % 4 == 0) {
            field_0x33c += 0x4000;
        }
        break;
    case 0x22:
        fn_801C6C7C();
        break;
    case 0x23:
    case 0x24:
        fn_801C6BA8();
        break;
    case 0x25:
    case 0x26:
        fn_801C6A4C();
        break;
    case 0x29:
        if (mStateTimer >= 0xA0) {
            if (mFlags & 0x10000) {
                fn_801C0D70()->fn_801C1224(mSerial);
            }
            vfunc_0x24();
        } else {
            if (mFlags & 0x08010000) {
                getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.5f, false);
            }
            if (mStateTimer % 8 == 0) {
                field_0x33c += 0x4000;
            }
        }
        break;
    case 0x2A:
        if (fn_801D0DD4()) {
            fn_801C6784();
            fn_801C3B7C(0x1C);
        }
        break;
    case 0x2B:
        if (mStateTimer >= 0x78) {
            mFlags |= 0x01000000;
            fn_801C3B7C(0x1D);
        }
        break;
    case 0x2C: {
        s32 prev = field_0x560;
        // FIXME(shotgun): field_0x548 is cls_802153A8 (fn_802153A8() and cur at +1), not typed in header
        if (mState == 0x2C && prev != field_0x560 && field_0x560 >= 0 && field_0x560 <= 3) {
            startSound_801F0E34(lbl_80469450[field_0x560], 0);
        }
        break;
    }
    case 0x2D:
        if (mStateTimer >= 0x4A) {
            fn_801C3B7C(0x1B);
        }
        break;
    case 0x2E:
        if (fn_8021CFF8()) {
            fn_8021CDE4(2);
        }
        break;
    case 0x2F:
        if (!(fn_801C0D70()->field_0x0 & 0x400)) {
            fn_801C3B7C(0xF);
        }
        break;
    }
    if (mFlags & 0x100000) {
        fn_801C71BC();
    }
    field_0x98 = fn_801F681C(field_0x33c);
}

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
// FIXME(shotgun): 88.86 %, different + reloc-name; behaviour near.
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
    case 0x11: {
        s16 angle = field_0x33c;
        field_0x3c.x = JMath::sincosTable_.sinShort(angle) * speed;
        field_0x3c.y = JMath::sincosTable_.cosShort(angle) * mSpeed;
        break;
    }
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
        {
            s16 angle = field_0x33c;
            field_0x3c.x = JMath::sincosTable_.sinShort(angle) * speed;
            field_0x3c.y = JMath::sincosTable_.cosShort(angle) * speed;
        }
        break;
    }
    vfunc_0x84();
}

// 0x801C2A84.
// FIXME(shotgun): 96.36 %, missing/extra + reloc-name; behaviour near.
void ShadowLink2_804A43E8::vfunc_0x98() {
    switch (mState) {
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x2F:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
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
    if ((field_0x524.field_0x0 & 1) && !fn_801CD2F8(field_0x4) && mPos.z <= 0.0f) {
        switch (mState) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 0xB: case 0xC: case 0xD: case 0xE:
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
                } else if (field_0x524.field_0x0 & 0x80) {
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
// FIXME(shotgun): 91.56 %, missing/extra + reloc-name; behaviour clean.
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

void ShadowLink2_804A43E8::fn_801C27F0() {
    bool flag = (mFlags & 0x08000000) != 0;
    cls_801C0D70* tracker = fn_801C0D70();
    tracker->fn_801C1374(flag);
    ActorBase_804A5770::vfunc_0x24();
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
