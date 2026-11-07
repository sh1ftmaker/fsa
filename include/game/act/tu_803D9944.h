#ifndef GAME_ACT_TU_803D9944_H
#define GAME_ACT_TU_803D9944_H

// Code: game/tu_803D9944.cpp.

#include "game/ActorBase.h"

// Spawned for 'TNGL'; moved here from game/act/tu_801F9484.h.
class cls_804B0478 : public ActorBase_804A5770 {
public:
    cls_804B0478() {}
    /* 0x08 */ virtual ~cls_804B0478();
    /* 0x0C */ virtual void init();
    // 0x803D9AA4 (symbol was fn_803D9AA4): the state machine, states 0 to 10.
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238; // timer
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ u8 field_0x250;
    /* 0x251 */ u8 field_0x251; // stb in tu_803D9944; the player's state 0x34 waits while it is clear
    /* 0x252 */ u8 field_0x252[2];
    /* 0x254 */ JGeometry::TVec3f field_0x254;
    /* 0x260 */ JGeometry::TVec3f field_0x260[4];
    /* 0x290 */ JPABaseEmitter* mEmitters[4]; // released by the destructor
}; // Size: 0x2A0

#endif
