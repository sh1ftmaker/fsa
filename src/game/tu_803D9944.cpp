#include "game/GameAudio.h"
#include "game/act/tu_803D9944.h"

cls_804B0478::~cls_804B0478() {
    for (int i = 0; i < 4; i++) {
        fn_801EE434(mEmitters[i]);
    }
}
