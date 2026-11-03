#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/Map.h"
#include "game/act/act_zora.h"

ZoraEnemy_804A3D7C::ZoraEnemy_804A3D7C() {
    field_0x2a8 = 0;
}

ZoraEnemy_804A3D7C::~ZoraEnemy_804A3D7C() {
    fn_801EE434(field_0x2a8);
    field_0x2a8 = NULL;
}

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

bool ZoraEnemy_804A3D7C::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (fn_8022461C(pos, layer) == 0xD) {
        return false;
    }
    return ActorBase_804A5770::vfunc_0x4C(layer, pos);
}
