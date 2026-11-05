#include "game/GameAudio.h"
#include "game/act/act_bomb.h"

Bomb_8049F784::~Bomb_8049F784() {
    for (int i = 0; i < 3; i++) {
        fn_801EE434(mEmitters[i]);
    }
}
