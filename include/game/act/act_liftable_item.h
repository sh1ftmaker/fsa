#ifndef GAME_ACT_ACT_LIFTABLE_ITEM_H
#define GAME_ACT_ACT_LIFTABLE_ITEM_H

// Code: game/act_liftable_item.cpp.

#include "game/ActorBase.h"

// The IMUP actor ("Liftable item", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203BB4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A01D0.
class LiftableItem_804A01D0 : public ActorBase_804A5770 {
public:
    LiftableItem_804A01D0() {}
    /* 0x08 */ virtual ~LiftableItem_804A01D0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    s32 fn_801597B8();
    void fn_801597C0();
    void fn_801597EC();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
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
    /* 0x27C */ JGeometry::TVec3f field_0x27c;
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ s32 field_0x298;
    /* 0x29C */ JGeometry::TVec3f field_0x29c;
    /* 0x2A8 */ JPABaseEmitter* mEmitter2a8; // released by the destructor
    /* 0x2AC */ JPABaseEmitter* mEmitter2ac; // released by the destructor
    /* 0x2B0 */ JPABaseEmitter* mEmitter2b0; // released by the destructor
}; // Size: 0x2B4

#endif
