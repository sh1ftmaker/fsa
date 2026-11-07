#ifndef GAME_ACT_ACT_CRYSTAL_BALL_H
#define GAME_ACT_ACT_CRYSTAL_BALL_H

// Code: game/act_crystal_ball.cpp.

#include "game/ActorBase.h"

// The CRTL crystal ball (docs/names/actors.tsv). Named after its vtable,
// 0x804AB308; the destructor (0x80323708) releases its two emitters.
class CrystalBall_804AB308 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~CrystalBall_804AB308();

    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x40 */ virtual void vfunc_0x40();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x54 */ virtual void vfunc_0x54();

    void fn_80322F44();
    void fn_803226EC();
    void fn_80322964();
    void fn_80322BC4();

    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ f32 field_0x234;
    /* 0x238 */ f32 field_0x238;
    /* 0x23C */ s32 field_0x23c; // (spawn parameter >> 16) & 3: the player
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248; // counts down
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ s32 field_0x284; // (spawn parameter >> 8) & 0xF
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ u8 field_0x28c;
    /* 0x28D */ u8 field_0x28d[3];
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ s32 field_0x298;
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ s32 field_0x2a0;
    /* 0x2A4 */ u8 field_0x2a4;
    /* 0x2A5 */ u8 field_0x2a5;
    /* 0x2A6 */ u8 field_0x2a6;
    /* 0x2A7 */ u8 field_0x2a7;
    /* 0x2A8 */ s32 field_0x2a8;
    /* 0x2AC */ u8 field_0x2ac;
    /* 0x2AD */ u8 field_0x2ad;
    /* 0x2AE */ u8 field_0x2ae[2];
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
    /* 0x2B8 */ JGeometry::TVec3f field_0x2b8;
    /* 0x2C4 */ JPABaseEmitter* field_0x2c4; // released by the destructor
    /* 0x2C8 */ JPABaseEmitter* field_0x2c8; // released by the destructor
}; // Size: 0x2CC

#endif
