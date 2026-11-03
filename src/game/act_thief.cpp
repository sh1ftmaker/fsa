#include "game/ActorBase.h"
#include "game/GameAudio.h"

bool ActorBase_804A5770::fn_80204D44() {
    bool result = false;
    if (field_0x94 == 1 || field_0xc4 != 0) {
        result = true;
    }
    return result;
}
