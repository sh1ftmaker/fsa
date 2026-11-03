#ifndef GAME_ACT_TU_801DCC98_H
#define GAME_ACT_TU_801DCC98_H

#include "types.h"
#include "game/EnemyBase.h"

// An EnemyBase_804A4808 whose effects (ids 0x256 and 0x257) follow it; code
// in game/tu_801DCC98.cpp (constructor 0x801DCF40). Named after its vtable
// (0x804A5038). It moves by field_0x3c each frame and removes itself when
// it leaves the view or hits a player.
// Size: unknown (>= 0x2B0)
class cls_804A5038 : public EnemyBase_804A4808 {
public:
    cls_804A5038();
    virtual ~cls_804A5038();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();

    /* 0x2A8 */ JPABaseEmitter* mEmitter2a8; // fn_8013CC50 id 0x256
    /* 0x2AC */ JPABaseEmitter* mEmitter2ac; // fn_8013CC50 id 0x257
};

#endif
