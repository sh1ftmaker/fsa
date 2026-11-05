#include "game/GameAudio.h"
#include "game/act/act_zora_escort.h"

u8 ZoraEscort_8049DF78::vfunc_0x60() {
    return 0;
}

s32 ZoraEscort_8049DF78::vfunc_0x78() {
    return 0;
}

void ZoraEscort_8049DF78::vfunc_0x90() {}

void ZoraEscort_8049DF78::vfunc_0x94() {}

s32 ZoraEscort_8049DF78::vfunc_0xC0() {
    return 1;
}

s32 ZoraEscort_8049DF78::vfunc_0x8C() {
    return vfunc_0x6C(0xC);
}

s32 ZoraEscort_8049DF78::vfunc_0x88() {
    return vfunc_0x6C(9);
}

void ZoraEscort_8049DF78::update() {
    if (field_0x33c != 0xF) {
        EscortBase_8049E0C8::update();
    }
}
