#include "game/GameAudio.h"
#include "game/act/act_liftable_wood.h"

LiftableWood_804A87A8::~LiftableWood_804A87A8() {
    if (mEmitter230 != NULL) {
        fn_801EE434(mEmitter230);
    }
}
