#include "game/GameAudio.h"
#include "game/act/act_force_gem_spawner.h"

ForceGemSpawner_804AB660::~ForceGemSpawner_804AB660() {
    releaseEmitter_801EE434(mEmitter24c);
    releaseEmitter_801EE434(mEmitter250);
    releaseEmitter_801EE434(mEmitter254);
}
