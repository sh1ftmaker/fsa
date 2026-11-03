#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_vaati_boss.h"

cls_801D6A58::cls_801D6A58() {
    fn_801D7874();
}

cls_801D6A88::cls_801D6A88() {
    field_0xc.fn_8021BD18(2);
    fn_801D789C();
}

void VaatiBoss_804A4958::vfunc_0x24() {}

void VaatiBoss_804A4958::vfunc_0x94() {
    switch (mState) {
    case 6:
    case 7:
    case 0xB:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        break;
    default:
        mAnim.fn_801D1C18();
        break;
    }
}

extern "C" void fn_801D652C() {
    lbl_8053AF80->fn_80458F9C(60);
}
