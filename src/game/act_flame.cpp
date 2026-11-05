#include "game/GameAudio.h"
#include "game/act/act_flame.h"

Flame_804A0330::~Flame_804A0330() {
    for (int i = 0; i < 4; i++) {
        fn_801EE434(mEmitters[i]);
    }
}
