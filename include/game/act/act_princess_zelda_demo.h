#ifndef GAME_ACT_ACT_PRINCESS_ZELDA_DEMO_H
#define GAME_ACT_ACT_PRINCESS_ZELDA_DEMO_H

// Code: game/act_princess_zelda_demo.cpp.

#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"

// The DMZL actor ("Princess Zelda (cutscene)", docs/names/actors.tsv). Its
// ctor calls cls_8049D140's ctor, then stores this vtable, 0x8049D208. Named
// after that vtable. Its last three slots (0x54, 0x58, 0x5C) override
// cls_8049D140's.
class PrincessZeldaDemo_8049D208 : public cls_8049D140 {
public:
    // Frees the emitters at 0x364, 0x368, 0x36C, 0x370[0..3] and 0x380[0..1]
    // with releaseEmitter_801EE434, then the members and cls_8049D140's destructor run.
    /* 0x08 */ virtual ~PrincessZeldaDemo_8049D208();
    // 0x10: copies the Goal (DMGL) and Maiden (DMMD) actors' positions.
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x58 */ virtual s32 vfunc_0x58();

    void fn_8010E2D4(s32 arg);
    void fn_8010E7E8(s32 arg);
    void fn_8010F7EC(s32 arg);
    void fn_8010F4DC();
    void fn_8010F9F0();
    void fn_8011093C();
    void fn_80110E9C();

    /* 0x230 */ u8 field_0x230; // set to 0 by the ctor
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ u8 field_0x23c[0x40]; // not yet known
    // The position of the Goal (DMGL) actor, copied by vfunc_0x10.
    /* 0x27C */ JGeometry::TVec3f field_0x27c;
    // The Maiden (DMMD) actors' positions, one slot per mode (0..5).
    /* 0x288 */ JGeometry::TVec3f field_0x288[6];
    // A copy of mPos, taken at the end of vfunc_0x10.
    /* 0x2D0 */ JGeometry::TVec3f field_0x2d0;
    /* 0x2DC */ u8 field_0x2dc[0x18]; // not yet known
    // Two sprite animations; the first one's mAnim (0x2FC) is the animation
    // index that draw() compares against.
    /* 0x2F4 */ SpriteAnim_801D267C mAnim0;
    /* 0x32C */ SpriteAnim_801D267C mAnim1;
    /* 0x364 */ JPABaseEmitter* field_0x364; // released by the destructor
    /* 0x368 */ JPABaseEmitter* field_0x368; // released by the destructor
    /* 0x36C */ JPABaseEmitter* field_0x36c; // released by the destructor
    /* 0x370 */ JPABaseEmitter* field_0x370[4]; // released by the destructor
    /* 0x380 */ JPABaseEmitter* field_0x380[2]; // the first two released by the destructor
}; // Size: unknown (>= 0x388)

#endif
