#ifndef GAME_ACT_ACT_SHADOW_LINK_DEMO_H
#define GAME_ACT_ACT_SHADOW_LINK_DEMO_H

// Code: game/act_shadow_link_demo.cpp. Named after its vtable, 0x8049DB90
// (24 slots; init and draw are not written yet). Only the
// field vfunc_0x58 returns is known, and vfunc_0x5C tests it.

#include "game/act/tu_8010DF44.h"
#include "game/cls_800FE8C0.h"

class cls_8049DB90 : public cls_8049D140 {
public:
    cls_8049DB90();
    /* 0x08 */ virtual ~cls_8049DB90();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update(); // 0x8011C308
    /* 0x20 */ virtual void draw();   // 0x8011C118
    void fn_8011B3F0();              // camera control by field_0x238
    /* 0x54 */ virtual void vfunc_0x54(); // compares fn_8011921C()'s field_0x8 with lbl_80464AF8
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C(); // 1 or 0, as cls_8049D140::vfunc_0x5C

    // 0x8011B878; called by vfunc_0x54 with the index of the matched entry.
    void fn_8011B878(s32 index);

    /* 0x230 */ u8 field_0x230; // set to 1 by the constructor
    /* 0x231 */ u8 field_0x231; // cleared by the constructor
    /* 0x232 */ u8 field_0x232[0x234 - 0x232]; // not yet known
    /* 0x234 */ s32 field_0x234; // cleared by the constructor
    /* 0x238 */ s32 field_0x238; // returned by vfunc_0x58
    /* 0x23C */ s32 field_0x23c; // cleared by the constructor
    /* 0x240 */ s32 field_0x240; // cleared by the constructor
    /* 0x244 */ f32 field_0x244; // cleared by the constructor
    /* 0x248 */ f32 field_0x248; // cleared by the constructor
    /* 0x24C */ u8 field_0x24c[0x250 - 0x24C]; // not yet known
    /* 0x250 */ f32 field_0x250; // cleared by the constructor
    /* 0x254 */ f32 field_0x254; // cleared by the constructor
    /* 0x258 */ f32 field_0x258; // cleared by the constructor
    /* 0x25C */ f32 field_0x25c; // cleared by the constructor
    /* 0x260 */ JGeometry::TVec3f field_0x260; // mSpawnPos, saved by fn_8011B878 state 11
    /* 0x26C */ JGeometry::TVec3f field_0x26c; // (mSpawnPos - player 0's position) / 60, state 11
    /* 0x278 */ cls_800FE8C0 mSequencer; // 0x18 bytes (the size of the spawner's __nw__ ends here)
}; // Size: 0x290

#endif
