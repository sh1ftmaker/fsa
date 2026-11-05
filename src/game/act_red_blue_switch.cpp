#include "game/GameAudio.h"
#include "game/act/act_red_blue_switch.h"

RedBlueSwitch_804A9020::~RedBlueSwitch_804A9020() {
    if (mEmitter230 != NULL) {
        releaseEmitter_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        releaseEmitter_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        releaseEmitter_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        releaseEmitter_801EE434(mEmitter23c);
    }
}
