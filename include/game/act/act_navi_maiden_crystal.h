#ifndef GAME_ACT_ACT_NAVI_MAIDEN_CRYSTAL_H
#define GAME_ACT_ACT_NAVI_MAIDEN_CRYSTAL_H

// Code: game/act_navi_maiden_crystal.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// Named after its vtable, 0x804ADFE8.
class Navi54MaidenCrystalCutscene_804ADFE8 : public ActorBase_804A5770 {
public:
    Navi54MaidenCrystalCutscene_804ADFE8() {}
    /* 0x08 */ virtual ~Navi54MaidenCrystalCutscene_804ADFE8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_803676CC();

    /* 0x230 */ u8 field_0x230;
    /* 0x231 */ u8 mPad231[7];
    /* 0x238 */ s32 mState;
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ s16 field_0x240;
    /* 0x242 */ u8 mPad242[2];
    /* 0x244 */ JGeometry::TVec3f field_0x244;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ ActorBase_804A5770* field_0x254;
    /* 0x258 */ JGeometry::TVec3f field_0x258;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ JPABaseEmitter* mEmitter270;
    /* 0x274 */ JPABaseEmitter* mEmitter274;
}; // Size: 0x278

#endif
