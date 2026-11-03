#include "game/GameAudio.h"
#include "game/act/act_zora_escort.h"

extern "C" int fn_8011FFB8() { return 0; }
extern "C" int fn_8011FFC0() { return 0; }
extern "C" void fn_8011FFC8() {}
extern "C" void fn_8011FFCC() {}
extern "C" int fn_80120040() { return 1; }

void ZoraEscort_8049DF78::update() {
    if (field_0x33c != 0xF) {
        EscortBase_8049E0C8::update();
    }
}

s32 ZoraEscort_8049DF78::vfunc_0x8C() {
    return vfunc_0x6C(0xC);
}
