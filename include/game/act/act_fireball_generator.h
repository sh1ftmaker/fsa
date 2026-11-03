#ifndef GAME_ACT_ACT_FIREBALL_GENERATOR_H
#define GAME_ACT_ACT_FIREBALL_GENERATOR_H

// Code: game/act_fireball_generator.cpp. Actor FBGE ("Fireball generator" in
// docs/names/actors.tsv): an EnemyBase_804A4808 that spawns 'FBAL' actors
// (the 4CC fn_801CECF4 gets) at the nearest player. Its one field, a u16
// countdown, sits right after EnemyBase_804A4808 (0x2A8), so the size is
// rounded to 0x2AC.

#include "game/EnemyBase.h"

// Named after its vtable (0x804A6660).
class FireballGenerator_804A6660 : public EnemyBase_804A4808 {
public:
    FireballGenerator_804A6660();
    virtual ~FireballGenerator_804A6660();
    /* 0x0C */ virtual void init();
    // Slot 0x90 (the per-state behaviour; mapHit is not read).
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);

    /* 0x2A8 */ u16 mCooldown; // frames until the next shot; 0x78 at start
}; // Size: 0x2AC

#endif
