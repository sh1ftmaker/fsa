#include "game/GameAudio.h"
#include "game/act/tu_802957CC.h"

cls_804A7AE0::~cls_804A7AE0() {
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 2; j++) {
            releaseEmitter_801EE434(mEmitters[i][j]);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters2[i]);
    }
}
