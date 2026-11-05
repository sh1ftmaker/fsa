#include "game/GameAudio.h"
#include "game/act/act_cucco.h"

Cucco_804AB8C0::~Cucco_804AB8C0() {
    releaseEmitter_801EE434(mEmitter2f4);
    releaseEmitter_801EE434(mEmitter2f8);
    releaseEmitter_801EE434(mEmitter2fc);
    releaseEmitter_801EE434(mEmitter300);
}
