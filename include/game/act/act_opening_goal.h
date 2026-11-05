#ifndef GAME_ACT_ACT_OPENING_GOAL_H
#define GAME_ACT_ACT_OPENING_GOAL_H

// Code: game/act_opening_goal.cpp. The Goal (DMGL) actor of the opening,
// named after its vtable, 0x8049DB30 (docs/names/actors.tsv). Spawned by
// spawnActor_801F9484 through its constructor (0x8011B0DC).

#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"

class cls_8049DB30 : public cls_8049D140 {
public:
    cls_8049DB30();
    /* 0x08 */ virtual ~cls_8049DB30();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();

    // 0x8011A4BC: called from update(). Fades the five colour bytes below
    // towards their targets (state 1 and 2) and copies them to the shadow
    // sprite's colour (lbl_8053AB10->0x2C).
    void fn_8011A4BC();

    /* 0x230 */ u8 field_0x230; // set to 1 by the constructor, not yet known
    /* 0x234 */ s32 field_0x234; // state: vfunc_0x54 switches on it, vfunc_0x58 returns it
    /* 0x238 */ s32 field_0x238; // init(): field_0x90 & 0xF
    /* 0x23C */ s32 field_0x23c; // init(): (field_0x90 >> 4) & 0xF
    /* 0x240 */ s32 field_0x240; // frame countdown: set by vfunc_0x54, counted down by fn_8011A4BC
    // Colour bytes (R, G, B, A of the shadow sprite, then one more byte):
    // the constructor sets 0x9F, 0xAF, 0xFF, 0, 0xFF; fn_8011A4BC fades them.
    /* 0x244 */ u8 field_0x244;
    /* 0x245 */ u8 field_0x245;
    /* 0x246 */ u8 field_0x246;
    /* 0x247 */ u8 field_0x247;
    /* 0x248 */ u8 field_0x248;
    // Emitters from spawnEmitter_8013CC50, freed by the destructor with releaseEmitter_801EE434.
    /* 0x24C */ JPABaseEmitter* mEmitter24c;
    /* 0x250 */ JPABaseEmitter* mEmitter250;
    /* 0x254 */ JPABaseEmitter* mEmitter254;
    /* 0x258 */ JPABaseEmitter* mEmitter258;
    /* 0x25C */ SpriteAnim_801D267C mAnim; // constructed last
}; // Size: 0x294

#endif
