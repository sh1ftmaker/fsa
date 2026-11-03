#include "game/GameAudio.h"
#include "game/act/act_tower_collapse_effect.h"

TowerCollapseEffect_8049CA40::~TowerCollapseEffect_8049CA40() {
    if (mEmitter260 != NULL) {
        fn_801EE434(mEmitter260);
    }
}
