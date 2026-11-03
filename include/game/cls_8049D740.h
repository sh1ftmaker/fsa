#ifndef GAME_CLS_8049D740_H
#define GAME_CLS_8049D740_H

// Code: game/act_maiden_demo.cpp. The Maiden (DMMD) actor, named after its
// vtable, 0x8049D740. Its constructor is not written yet; the destructor
// (0x8011342C) releases three emitters, then the mAnim member and cls_8049D140's
// destructor run. Only the field PrincessZeldaDemo reads is known.

#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"

class cls_8049D740 : public cls_8049D140 {
public:
    /* 0x08 */ virtual ~cls_8049D740();
    /* 0x10 */ virtual void vfunc_0x10(); // copies the Goal (DMGL) actor's position
    /* 0x58 */ virtual s32 vfunc_0x58();

    // Calls cls_8049D140::init(), then sets mMode and the Maiden's animation
    // (key table, animation 5 in mode 2, frame 0).
    void fn_801133C0();

    /* 0x230 */ s32 field_0x230; // not yet known
    /* 0x234 */ s32 mMode; // field_0x90 & 0x1F, set by fn_801133C0
    /* 0x238 */ s32 field_0x238; // returned by vfunc_0x58
    /* 0x23C */ u8 field_0x23c[0x18]; // not yet known
    // The position of the Goal (DMGL) actor, copied by vfunc_0x10.
    /* 0x254 */ JGeometry::TVec3f mGoalPos;
    /* 0x260 */ SpriteAnim_801D267C mAnim;
    /* 0x298 */ JPABaseEmitter* mEmitter298; // released by the destructor
    /* 0x29C */ JPABaseEmitter* mEmitter29c; // released by the destructor
    /* 0x2A0 */ JPABaseEmitter* mEmitter2a0; // released by the destructor
}; // Size: at least 0x2A4

#endif
