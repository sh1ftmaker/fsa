#ifndef GAME_ACT_TU_801A0710_H
#define GAME_ACT_TU_801A0710_H

// Code: game/tu_801A0710.cpp. Spawned for 'TENN' (vtable 0x804A2BF4).

#include "game/ActorBase.h"

class JPABaseEmitter;

class TennisShop_804A2BF4 : public ActorBase_804A5770 {
public:
    TennisShop_804A2BF4() {}
    /* 0x08 */ virtual ~TennisShop_804A2BF4();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x801A0754: draws the sprite scaled by 1.2 (mode 1) or 2.0 (mode 2).
    void fn_801A0754();
    // 0x801A0910: draws the "obachan" TIMG picture and the counter.
    void fn_801A0910();
    // 0x801A0BE4 / 0x801A0F90 / 0x801A17B8: the update of mode 0 / 1 / 2.
    void fn_801A0BE4();
    void fn_801A0F90();
    void fn_801A17B8();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234[4]; // not yet known
    /* 0x238 */ s32 field_0x238[4];
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c[6];
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ u8 field_0x270;
    /* 0x271 */ u8 field_0x271[3]; // not yet known
    /* 0x274 */ s32 field_0x274; // mode, the low nibble of the spawn parameter
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c[4];
    /* 0x28C */ s32 field_0x28c[4];
    /* 0x29C */ s32 field_0x29c[4];
    /* 0x2AC */ s32 field_0x2ac[4];
    /* 0x2BC */ u8 field_0x2bc[4];
    /* 0x2C0 */ s32 field_0x2c0;
    /* 0x2C4 */ s32 field_0x2c4;
    /* 0x2C8 */ s32 field_0x2c8[20];
    /* 0x318 */ s32 field_0x318;
    /* 0x31C */ s32 field_0x31c;
    /* 0x320 */ s32 field_0x320;
    /* 0x324 */ f32 field_0x324[6];
    /* 0x33C */ f32 field_0x33c;
    /* 0x340 */ f32 field_0x340;
    /* 0x344 */ JPABaseEmitter* mEmitters[4];
}; // Size: 0x354

#endif
