#ifndef GAME_ACT_TU_801E6D48_H
#define GAME_ACT_TU_801E6D48_H

// Code: game/tu_801E6D48.cpp. An EnemyBase_804A4808 named after its vtable
// (0x804A5418); its update() is slot 0x14 and runs the base update() unless
// mFlags has the 0x8000 bit set.

#include "game/EnemyBase.h"

class cls_804A5418 : public EnemyBase_804A4808 {
public:
    /* 0x14 */ virtual void update();
};

#endif
