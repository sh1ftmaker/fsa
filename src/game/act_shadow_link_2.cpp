#include "game/GameAudio.h"

// Takes no arguments, so it is a free function rather than a member.
extern "C" s32 fn_801C5340() {
    return 0;
}

// Packs five fields into one word: bits 0-4, 5-7, 8-11, 12-15, 20-27.
extern "C" u32 fn_801C5660(u32 a, u32 b, u32 c, u32 d, u32 e) {
    return ((e & 0xFF) << 20) | ((d & 0xF) << 12) | ((c & 0xF) << 8) | ((b & 0x7) << 5) | (a & 0x1F);
}
