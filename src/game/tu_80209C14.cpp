#include "game/GameAudio.h"
#include "game/act/tu_80209C14.h"

cls_804A5D60::~cls_804A5D60() {}

void cls_804A5D60::fn_80209D44() {
    mFlags |= 0x2000;
}

void cls_804A5D60::vfunc_0x74() {
    mFlags |= ENEMY_FLAG_NO_HURT;
}
