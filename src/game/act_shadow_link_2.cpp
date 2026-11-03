#include "game/GameAudio.h"
#include "game/act/act_shadow_link_2.h"

// Gravity override (EnemyBase's vfunc_0x84): z changes by lbl_8053EFB8 per call,
// skipped while field_0xd8 > 0 and field_0xf8 == 2; clamped at -lbl_8053EFBC.
void ShadowLink2_804A43E8::vfunc_0x84() {
    if (field_0xd8 > 0 && field_0xf8 == 2) {
        return;
    }
    // The negated floor is computed before the store, as in the original; this
    // local is there to match that order.
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

// Takes no arguments, so it is a free function rather than a member.
extern "C" s32 fn_801C5340() {
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
