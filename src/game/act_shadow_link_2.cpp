#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_shadow_link_2.h"

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

// Member: r3 (this) is passed straight through to fn_801C3B7C.
void ShadowLink2_804A43E8::fn_801C26D8() {
    fn_801C3B7C(0xD);
}

// Member: switches to state 0xE unless the actor is already in it.
void ShadowLink2_804A43E8::fn_801C26FC() {
    if (mState != 0xE) {
        fn_801C3B7C(0xE);
    }
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

// Takes no arguments and ignores r3; callers test the result as a byte.
bool fn_801C5340() {
    return 0;
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

// Packs five fields into one word: bits 0-4, 5-7, 8-11, 12-15, 20-27.
extern "C" u32 fn_801C5660(u32 a, u32 b, u32 c, u32 d, u32 e) {
    return ((e & 0xFF) << 20) | ((d & 0xF) << 12) | ((c & 0xF) << 8) | ((b & 0x7) << 5) | (a & 0x1F);
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
