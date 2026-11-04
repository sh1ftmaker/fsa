#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/act/tu_801DB18C.h"

void cls_804A4EE8::vfunc_0x94() {
    s32 cel = mAnim.mCel;
    EnemyBase_804A4808::vfunc_0x94();
    if (cel == 8 && mAnim.mCel == 7) {
        fn_801F0E34(0x4005B, 0);
    }
}

void cls_804A4EE8::vfunc_0x9C(u8 mapHit) {
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    fn_8013CB44(field_0x2e4, &mPos, 1, 0, true);
}
