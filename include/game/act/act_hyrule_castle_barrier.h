#ifndef GAME_ACT_ACT_HYRULE_CASTLE_BARRIER_H
#define GAME_ACT_ACT_HYRULE_CASTLE_BARRIER_H

// Code: game/act_hyrule_castle_barrier.cpp.

#include "game/ActorBase.h"

// The BARI actor ("Hyrule Castle barrier", docs/names/actors.tsv, med
// confidence). Named after its vtable, 0x8049DEE8.
class HyruleCastleBarrier_8049DEE8 : public ActorBase_804A5770 {
public:
    HyruleCastleBarrier_8049DEE8();
    /* 0x08 */ virtual ~HyruleCastleBarrier_8049DEE8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();

    void fn_8011EEEC();
    void fn_8011F314();
    void fn_8011F5C0(s32 arg);

    // fn_8011EEEC's particle positions: plain functions of the .cpp (dead-stripped;
    // every caller inlines them).
    const JGeometry::TVec3f* getEndPos1() const;
    const JGeometry::TVec3f* getEndPos2() const;
    const JGeometry::TVec3f* getMidPos(s32 i) const;

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234; // (field_0x90 >> 4) & 0xF; 1 selects the first box set
    /* 0x238 */ s32 field_0x238; // (field_0x90 >> 22) & 0x1F
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ bool field_0x240; // field_0x90 bit 0, set in init
    /* 0x241 */ bool field_0x241; // field_0x90 bit 1, set in init
    /* 0x244 */ JPABaseEmitter* mEmitters[3]; // released by the destructor
    /* 0x250 */ JPABaseEmitter* field_0x250;
    /* 0x254 */ JPABaseEmitter* field_0x254;
}; // Size: at least 0x258 (the last field written by the constructor is 0x254)

#endif
