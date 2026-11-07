#ifndef GAME_CLS_8049D83C_H
#define GAME_CLS_8049D83C_H

// Code: game/tu_80113788.cpp. A cls_8049D140 subclass named after its vtable,
// 0x8049D83C. Its constructor (0x80115590) zeroes field_0x23c.

#include "game/act/tu_8010DF44.h"

class cls_8049D83C : public cls_8049D140 {
public:
    /* 0x20 */ virtual void draw();
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();

    // Members of this class, not written yet (0x80113790, 0x80113B54).
    void fn_80113790();
    void fn_8011395C();
    void fn_80113B54(s32 arg);

    /* 0x230 */ u8 field_0x230[0x232 - 0x230]; // not yet known
    /* 0x232 */ u8 field_0x232; // tested by draw
    /* 0x233 */ u8 field_0x233;
    /* 0x234 */ u8 field_0x234; // counter runs while set (fn_8011395C)
    /* 0x235 */ u8 field_0x235[0x238 - 0x235]; // not yet known
    /* 0x238 */ s32 field_0x238; // index into the table draw reads
    /* 0x23C */ s32 field_0x23c; // returned by vfunc_0x58
    /* 0x240 */ s32 field_0x240; // stage of the counter (fn_8011395C)
    /* 0x244 */ u8 field_0x244[0x248 - 0x244]; // not yet known
    /* 0x248 */ s32 field_0x248; // fn_80113790 draws the darkening while positive (the alpha)
    /* 0x24C */ s32 field_0x24c; // frame counter (fn_8011395C)
    /* 0x250 */ u8 field_0x250[0x258 - 0x250]; // not yet known
    /* 0x258 */ f32 field_0x258; // angle (fn_8011395C)
    /* 0x25C */ u8 field_0x25c[0x260 - 0x25C]; // not yet known
    /* 0x260 */ void* field_0x260; // "TIMG" resource of fn_80113790's image
    /* 0x264 */ u8 field_0x264[0x274 - 0x264]; // not yet known
    /* 0x274 */ s32 field_0x274; // added to the sprite frame (fn_80113B54)
    /* 0x278 */ u8 field_0x278[0x2F0 - 0x278]; // not yet known
}; // Size: 0x2F0 (the spawner's __nw__)

#endif
