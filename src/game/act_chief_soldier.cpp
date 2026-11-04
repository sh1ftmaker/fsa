#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/act_chief_soldier.h"

void ChiefSoldier_804A0B90::vfunc_0x24() {
    if ((s32)(field_0x90 & 0xFF) == 6) {
        field_0x18c = 0;
        fn_801F856C()->fn_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
    }
    ActorBase_804A5770::vfunc_0x24();
}
