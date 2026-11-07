#ifndef GAME_CLS_8049D83C_H
#define GAME_CLS_8049D83C_H

// Code: game/tu_80113788.cpp. A cls_8049D140 subclass named after its vtable,
// 0x8049D83C. Its constructor (0x80115590) zeroes field_0x23c.

#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"

class JPABaseEmitter;

class cls_8049D83C : public cls_8049D140 {
public:
    cls_8049D83C();
    /* 0x08 */ virtual ~cls_8049D83C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();
    /* 0x60 */ virtual void vfunc_0x60(s32 arg);

    // Members of this class, not written yet (0x80113790, 0x80113B54).
    void fn_80113790();
    void fn_8011395C();
    void fn_80113B54(s32 arg);

    /* 0x230 */ u8 field_0x230;
    /* 0x231 */ u8 field_0x231;
    /* 0x232 */ u8 field_0x232; // tested by draw
    /* 0x233 */ u8 field_0x233;
    /* 0x234 */ u8 field_0x234; // counter runs while set (fn_8011395C)
    /* 0x235 */ u8 field_0x235[0x238 - 0x235]; // not yet known
    /* 0x238 */ s32 field_0x238; // index into the table draw reads
    /* 0x23C */ s32 field_0x23c; // returned by vfunc_0x58
    /* 0x240 */ s32 field_0x240; // stage of the counter (fn_8011395C)
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248; // fn_80113790 draws the darkening while positive (the alpha)
    /* 0x24C */ s32 field_0x24c; // frame counter (fn_8011395C)
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258; // angle (fn_8011395C)
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ void* field_0x260; // "TIMG" resource of fn_80113790's image
    /* 0x264 */ SpriteAnim_801D267C mAnim; // its mCel (0x274) is added to the sprite frame (fn_80113B54)
    // Emitters, released by the destructor with releaseEmitter_801EE434.
    /* 0x29C */ JPABaseEmitter* field_0x29c;
    /* 0x2A0 */ JPABaseEmitter* field_0x2a0;
    /* 0x2A4 */ JPABaseEmitter* field_0x2a4;
    /* 0x2A8 */ JPABaseEmitter* field_0x2a8[7];
    /* 0x2C4 */ JPABaseEmitter* field_0x2c4[3];
    /* 0x2D0 */ JPABaseEmitter* field_0x2d0[5];
    /* 0x2E4 */ JPABaseEmitter* field_0x2e4;
    /* 0x2E8 */ JPABaseEmitter* field_0x2e8[2];
}; // Size: 0x2F0 (the spawner's __nw__)

#endif
