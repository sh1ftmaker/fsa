#ifndef GAME_ACT_ACT_SPAWNER_H
#define GAME_ACT_ACT_SPAWNER_H

// Code: game/act_spawner.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9328.
class SpawnerGenerator_804A9328 : public ActorBase_804A5770 {
public:
    SpawnerGenerator_804A9328() {}
    /* 0x08 */ virtual ~SpawnerGenerator_804A9328();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234[3];
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ JGeometry::TVec3f field_0x258;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ u8 field_0x26c;
    /* 0x26D */ u8 field_0x26d[3];
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ s32 field_0x274[256];
    /* 0x674 */ s32 field_0x674;
}; // Size: 0x678

#endif
