#ifndef GAME_ACT_ACT_CHEEP_CHEEP_H
#define GAME_ACT_ACT_CHEEP_CHEEP_H

// Code: game/act_cheep_cheep.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A11B8.
class CheepCheep_804A11B8 : public ActorBase_804A5770 {
public:
    CheepCheep_804A11B8() {}
    /* 0x08 */ virtual ~CheepCheep_804A11B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s16 field_0x25c;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ JGeometry::TVec3f field_0x270;
}; // Size: 0x27C

#endif
