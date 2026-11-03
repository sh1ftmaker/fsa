#include "game/GameAudio.h"
#include "game/act/act_shadow_link_2.h"

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

// Packs five fields into one word: bits 0-4, 5-7, 8-11, 12-15, 20-27.
extern "C" u32 fn_801C5660(u32 a, u32 b, u32 c, u32 d, u32 e) {
    return ((e & 0xFF) << 20) | ((d & 0xF) << 12) | ((c & 0xF) << 8) | ((b & 0x7) << 5) | (a & 0x1F);
}
