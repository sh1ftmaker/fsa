#include "game/GameAudio.h"
#include "game/act/tu_801E6D48.h"

void cls_804A5418::update() {
    if ((mFlags & 0x8000) == 0) {
        EnemyBase_804A4808::update();
    }
}
