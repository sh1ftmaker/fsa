#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/act/act_zora.h"

extern "C" void fn_801BA65C() {}
extern "C" s32 fn_801BA660() { return 0; }
extern "C" void fn_801BA668() {}
extern "C" void fn_801BA66C() {}
extern "C" void fn_801BB1A8() {}

void ZoraEnemy_804A3D7C::vfunc_0xA0() {
    if (mState != 0) {
        lbl_8053AB10->field_0x24->fn_8030C210(&mPos);
    }
}
