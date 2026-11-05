#include "game/GameAudio.h"
#include "game/act/tu_8018EB94.h"

cls_804A1FA0::~cls_804A1FA0() {
    for (int i = 0; i < 3; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}
