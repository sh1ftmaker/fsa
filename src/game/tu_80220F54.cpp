#include "game/GameAudio.h"
#include "game/cls_804A6A00.h"
#include "game/GameGlobals.h"

bool ShadowLinkBase_804A6A00::vfunc_0x60() {
    return false;
}

void ShadowLinkBase_804A6A00::vfunc_0x74() {
    fn_8022149C(0);
}

void ShadowLinkBase_804A6A00::vfunc_0x9C(u8 mapHit) {
    if (!(mFlags & 7)) {
        fn_801CE2BC(mapHit);
    } else {
        vfunc_0x94();
    }
}

void ShadowLinkBase_804A6A00::fn_80221408() {
    if (mStateTimer >= 0xA0) {
        fn_801CB434(field_0x4, &mPos, false);
        vfunc_0x24();
    } else if (mStateTimer % 8 == 0) {
        field_0x98 = fn_801CD30C(field_0x98);
        mAnim.startAnim_801D22DC(field_0x98, 0, false);
    }
}
