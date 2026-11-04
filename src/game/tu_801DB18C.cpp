#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/act/tu_801DB18C.h"

void cls_804A4EE8::vfunc_0x9C(u8 mapHit) {
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    fn_8013CB44(field_0x2e4, &mPos, 1, 0, true);
}
