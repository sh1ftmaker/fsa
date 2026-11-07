#ifndef GAME_ACT_TU_800FF060_H
#define GAME_ACT_TU_800FF060_H

// Code: game/tu_800FF060.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049C858.
class HammerShopMinigame_8049C858 : public ActorBase_804A5770 {
public:
    HammerShopMinigame_8049C858() {}
    /* 0x08 */ virtual ~HammerShopMinigame_8049C858();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Not written yet: the minigame's per-frame work.
    void fn_801003F4();

    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ u8 field_0x234;
    /* 0x235 */ u8 field_0x235;
    /* 0x236 */ u8 field_0x236;
    /* 0x237 */ u8 field_0x237[2]; // not yet known
    /* 0x239 */ u8 field_0x239; // bits 22..26 of field_0x90
    /* 0x23A */ u8 field_0x23a[2]; // not yet known
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ u8 field_0x244[4]; // not yet known
    /* 0x248 */ s32 field_0x248; // frame counter
    /* 0x24C */ u8 field_0x24c[4]; // not yet known
    /* 0x250 */ s32 field_0x250[4]; // the players' scores
    /* 0x260 */ s32 field_0x260; // 600 after init
    /* 0x264 */ s32 field_0x264[4]; // the players ordered by score, lowest first
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ u8 field_0x284[0x28]; // not yet known
}; // Size: 0x2AC

#endif
