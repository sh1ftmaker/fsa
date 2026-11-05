#include "game/GameAudio.h"
#include "game/act/act_arrow.h"

cls_804A05E0::~cls_804A05E0() {
    for (int i = 0; i < 2; i++) {
        fn_801EE434(mEmitters[i]);
    }
}
