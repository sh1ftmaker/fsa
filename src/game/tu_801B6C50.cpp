#include "game/GameAudio.h"
#include "game/act/tu_801B6C50.h"

cls_804A39DC::~cls_804A39DC() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}
