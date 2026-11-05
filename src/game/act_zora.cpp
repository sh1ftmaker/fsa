#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/Map.h"
#include "game/act/act_zora.h"

void ZoraEnemy_804A3D7C::vfunc_0xB0() {}

bool ZoraEnemy_804A3D7C::vfunc_0xAC() {
    return false;
}

void ZoraEnemy_804A3D7C::vfunc_0xA8() {}

void ZoraEnemy_804A3D7C::vfunc_0xA4() {}

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

ZoraEnemy_804A3D7C::~ZoraEnemy_804A3D7C() {
    fn_801EE434(field_0x2a8);
    field_0x2a8 = NULL;
}

ZoraEnemy_804A3D7C::ZoraEnemy_804A3D7C() {
    field_0x2a8 = 0;
}
