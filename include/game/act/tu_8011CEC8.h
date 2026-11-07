#ifndef GAME_ACT_TU_8011CEC8_H
#define GAME_ACT_TU_8011CEC8_H

// Code: game/tu_8011CEC8.cpp. Spawned for 'DMGA' (vtable 0x8049DD68).

#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"

class cls_8049DD68 : public cls_8049D140 {
public:
    cls_8049DD68() : field_0x230(0), field_0x234(0), field_0x238(0.0f), field_0x23c(0.0f) {}
    /* 0x08 */ virtual ~cls_8049DD68();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();
    // 0x8011CF48: per-state camera centre / zoom, fed to the camera each frame.
    void fn_8011CF48();
    // 0x8011D1B8: enters a state (0-9) and stores it in field_0x234.
    void fn_8011D1B8(s32 state);

    /* 0x230 */ u8 field_0x230;
    /* 0x231 */ u8 field_0x231[0x3];
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ f32 field_0x238;
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ f32 field_0x240; // camera centre x (init copies mPos)
    /* 0x244 */ f32 field_0x244; // camera centre y
    /* 0x248 */ f32 field_0x248;
    /* 0x24C */ u8 field_0x24c[0xC];
    /* 0x258 */ f32 field_0x258; // camera zoom size
    /* 0x25C */ u8 field_0x25c[0x18];
    /* 0x274 */ SpriteAnim_801D267C field_0x274;
}; // Size: 0x2AC

#endif
