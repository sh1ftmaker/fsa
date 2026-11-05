#include "game/GameAudio.h"
#include "game/act/act_flame.h"

cls_804A0330::~cls_804A0330() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}
