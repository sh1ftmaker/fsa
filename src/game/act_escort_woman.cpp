#include "game/GameAudio.h"
#include "game/act/act_escort_woman.h"

s32 EscortWoman_8049E1D0::vfunc_0x74() {
    return fn_80121DB0();
}

s32 EscortWoman_8049E1D0::vfunc_0x94() {
    return vfunc_0x6C(9);
}

s32 EscortWoman_8049E1D0::vfunc_0x88() {
    return vfunc_0x6C(5);
}

s32 EscortWoman_8049E1D0::vfunc_0x84() {
    return vfunc_0x6C(7);
}

s32 EscortWoman_8049E1D0::vfunc_0x8C() {
    return vfunc_0x6C(8);
}

void EscortWoman_8049E1D0::vfunc_0x64() {
    field_0x35c = field_0x90 & 0xFF;
    field_0x3cc = (field_0x90 >> 22) & 0x1F;
}

void EscortWoman_8049E1D0::init() {
    fn_80124448();
}
