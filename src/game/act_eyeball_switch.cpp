#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/act/act_eyeball_switch.h"

void cls_804AE678::vfunc_0x24() {
    deleteEmitter(&mEmitter);
    mAlive = false;
}
