#ifndef GAME_CLS_804A8638_H
#define GAME_CLS_804A8638_H

// Code: game/tu_802B3674.cpp. An ActorBase whose vtable is lbl_804A8638 (slot
// 0x8, the destructor, is fn_802B51C0). No actor id is confirmed for it (it
// sets the fog colour, so likely SWEV, the environment switch control:
// act_blizzard_battle finds the SWEV actor and calls fn_802B39E8 on it), so it
// is named after the vtable.
#include "game/ActorBase.h"

class JPABaseEmitter;
class ActorSoundObj_804578F4;

class cls_804A8638 : public ActorBase_804A5770 {
public:
    cls_804A8638() : field_0x2dc(-1) {}
    // 0x802B51C0 (vtable slot 0x08): the emitters, then the sound object.
    /* 0x08 */ virtual ~cls_804A8638();
    // 0x802B5360 (slot 0x0C): decodes field_0x90 and starts the preset.
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x20 */ virtual void draw();
    /* 0x54 */ virtual void vfunc_0x54(s32 a, s32 b, s32 c, s32 d, s32 e, s32 state);
    /* 0x58 */ virtual void vfunc_0x58();
    /* 0x5C */ virtual void vfunc_0x5C();
    // 0x802B3674: starts BGM 0x100000E unless one of the stage tracks is already playing.
    /* 0x60 */ virtual void vfunc_0x60();

    // 0x802B39E8: clears field_0x290 and deletes mEmitter. act_blizzard_battle's
    // notifySwev calls it on the SWEV actor.
    void fn_802B39E8();
    // 0x802B3A3C: clears field_0x2b8. act_vaati_demo's
    // fn_80115F9C calls it on the SWEV actor.
    void fn_802B3A3C();
    // 0x802B3A48: clears byte 0x1EE of fn_8016A3AC()'s object,
    // releases the three emitters (0x234, 0x238, mEmitter) and clears byte
    // 0x11C. act_battle_royale_end_check (fn_803D6BDC) calls it on every SWEV actor.
    void fn_802B3A48();

    /* 0x230 */ ActorSoundObj_804578F4* mSoundObj; // new'd by init(), deleted by the destructor
    /* 0x234 */ JPABaseEmitter* field_0x234; // emitter, deleted by vfunc_0x10
    /* 0x238 */ JPABaseEmitter* field_0x238; // emitter, deleted by vfunc_0x10
    /* 0x23C */ JPABaseEmitter* mEmitter;
    /* 0x240 */ s32 field_0x240; // state; set to 0xC when the 0x310 index is below 0x314
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ JGeometry::TVec3f field_0x254; // init(): the camera's 0x10 / 0x20 fields, negated
    /* 0x260 */ s32 field_0x260; // bits 22..24 of field_0x90
    /* 0x264 */ s32 field_0x264; // bit 21
    /* 0x268 */ s32 field_0x268; // bit 20
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270; // bit 19
    /* 0x274 */ s32 field_0x274; // bit 18
    /* 0x278 */ s32 field_0x278; // bit 17
    /* 0x27C */ s32 field_0x27c; // bit 16
    /* 0x280 */ s32 field_0x280; // bit 15
    /* 0x284 */ s32 field_0x284;
    /* 0x288 */ s32 field_0x288; // state; vfunc_0x10 acts on 4 and resets it to 0
    /* 0x28C */ s32 field_0x28c; // bit 12
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294; // bit 10
    /* 0x298 */ s32 field_0x298;
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ s32 field_0x2a0;
    /* 0x2A4 */ s32 field_0x2a4;
    /* 0x2A8 */ s32 field_0x2a8;
    /* 0x2AC */ s32 field_0x2ac; // bit 4
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4; // bit 26
    /* 0x2B8 */ s32 field_0x2b8;
    /* 0x2BC */ s32 field_0x2bc;
    /* 0x2C0 */ f32 field_0x2c0[4];
    /* 0x2D0 */ u8 field_0x2d0[6]; // the saved fog bytes (fn_8016A3AC's 0x1EC..0x1F1)
    /* 0x2D6 */ u8 field_0x2d6[2];
    /* 0x2D8 */ s32 field_0x2d8; // low nibble of field_0x90: the preset
    /* 0x2DC */ s32 field_0x2dc;
    /* 0x2E0 */ s32 field_0x2e0;
    /* 0x2E4 */ s32 field_0x2e4;
    /* 0x2E8 */ s32 field_0x2e8;
    /* 0x2EC */ s32 field_0x2ec;
    /* 0x2F0 */ s32 field_0x2f0;
    /* 0x2F4 */ s32 field_0x2f4;
    /* 0x2F8 */ s32 field_0x2f8;
    /* 0x2FC */ u8 field_0x2fc[0x300 - 0x2FC]; // not yet identified
    /* 0x300 */ s32 field_0x300; // copied to 0x2E0..0x2E8 by vfunc_0x10
    /* 0x304 */ s32 field_0x304;
    /* 0x308 */ s32 field_0x308;
    /* 0x30C */ s32 field_0x30c;
    /* 0x310 */ s32 field_0x310;
    /* 0x314 */ s32 field_0x314;
    /* 0x318 */ s32 field_0x318;
    /* 0x31C */ s32 field_0x31c;
    /* 0x320 */ s32 field_0x320;
    /* 0x324 */ u8 field_0x324[0x334 - 0x324]; // not yet known
    /* 0x334 */ s32 field_0x334;
    /* 0x338 */ s32 field_0x338;
}; // Size: 0x33C

#endif
