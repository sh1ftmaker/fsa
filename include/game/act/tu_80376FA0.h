#ifndef GAME_ACT_TU_80376FA0_H
#define GAME_ACT_TU_80376FA0_H

// Code: game/tu_80376FA0.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE7D8.
class cls_804AE7D8 : public ActorBase_804A5770 {
public:
    cls_804AE7D8() {}
    /* 0x08 */ virtual ~cls_804AE7D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    bool touchesPlayer(s32 player, JGeometry::TVec3f& point);
    // Effect emitters from spawnEmitter_8013CC50, one per effect id (0x542, 0xB7-0xBA,
    // and 0x319-0x31C for the one update() re-creates); fn_801EE434 frees them.
    /* 0x230 */ JPABaseEmitter* mEmitter230;
    /* 0x234 */ JPABaseEmitter* mEmitter234;
    /* 0x238 */ JPABaseEmitter* mEmitter238;
    /* 0x23C */ JPABaseEmitter* mEmitter23c;
    /* 0x240 */ JPABaseEmitter* mEmitter240;
    // 0 until init() sets it to 1 (its check passed, the sprite setup ran);
    // update() moves it from 0 to 1 once the emitters are placed, and draw()
    // and update() switch on it.
    /* 0x244 */ s32 field_0x244;
    // Copy of mPos made by init(); update() takes its distance to mPos.
    /* 0x248 */ JGeometry::TVec3f field_0x248;
    /* 0x254 */ s32 field_0x254;
    // init(): (field_0x90 & 0xFF) + 1, stored as a word.
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ u8 field_0x264;
    // init(): bit 16 of field_0x90.
    /* 0x265 */ u8 field_0x265;
    /* 0x266 */ u8 field_0x266[0x268 - 0x266];
}; // Size: 0x268

#endif
