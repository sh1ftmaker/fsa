#ifndef GAME_ACT_ACT_NAVI_MAIDEN_CRYSTAL_H
#define GAME_ACT_ACT_NAVI_MAIDEN_CRYSTAL_H

// Code: game/act_navi_maiden_crystal.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804ADFE8.
class Navi54MaidenCrystalCutscene_804ADFE8 : public ActorBase_804A5770 {
public:
    Navi54MaidenCrystalCutscene_804ADFE8() {}
    /* 0x08 */ virtual ~Navi54MaidenCrystalCutscene_804ADFE8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x48]; // not yet known
}; // Size: 0x278

#endif
