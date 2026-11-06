#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_horse_race_man.h"

HorseRaceMan_804A5630::HorseRaceMan_804A5630() {
    field_0x2a8 = gGameRoot_8053AAF8->fn_80138A30();
    field_0x2ac = 0;
    field_0x2b0 = -1;
    field_0x2b4 = 0;
    mCountdown.fn_801ED974();
    fn_80216178(&mTable);
    lbl_8053A160 = -1;
}

HorseRaceMan_804A5630::~HorseRaceMan_804A5630() {
    lbl_8053A160 = -1;
}

void HorseRaceMan_804A5630::vfunc_0x94() {
    mAnim.fn_801D2264(field_0x98, 2);
    EnemyBase_804A4808::vfunc_0x94();
}
