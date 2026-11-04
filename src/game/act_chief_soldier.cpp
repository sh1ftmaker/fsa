#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/act_chief_soldier.h"

ChiefSoldier_804A0B90::~ChiefSoldier_804A0B90() {
    if ((s32)(field_0x90 & 0xFF) != 7 && (s32)(field_0x90 & 0xFF) != 8) {
        if ((s32)(field_0x90 & 0xFF) != 6 && lbl_8053AAF8->field_0x4c != 9 &&
            lbl_8053AAF8->field_0x4c != 0xB && mLife != 0) {
            lbl_8053AAF8->fn_80138B7C();
        }
        s32 serial = field_0x238;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
    }
}

void ChiefSoldier_804A0B90::vfunc_0x24() {
    if ((s32)(field_0x90 & 0xFF) == 6) {
        field_0x18c = 0;
        fn_801F856C()->fn_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
    }
    ActorBase_804A5770::vfunc_0x24();
}
