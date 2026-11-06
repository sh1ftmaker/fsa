#ifndef GAME_ACT_ACT_DROPPED_ITEM_H
#define GAME_ACT_ACT_DROPPED_ITEM_H

// Code: game/act_dropped_item.cpp.

#include "game/ActorBase.h"

// The LSTI actor ("Dropped item", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80202B44, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8C80.
class DroppedItem_804A8C80 : public ActorBase_804A5770 {
public:
    DroppedItem_804A8C80() {}
    /* 0x08 */ virtual ~DroppedItem_804A8C80();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x40 */ virtual void vfunc_0x40();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    void fn_802C0570();
    // Serial and layer are loaded before the position copy, the player after it (RULES_CARD, by-value message calls).
    void talk(s32 serial, s32 layer, const JGeometry::TVec3f& pos);
    bool fn_802BFF30(s32 player);

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
    /* 0x264 */ u32 field_0x264;
    /* 0x268 */ JGeometry::TVec2f field_0x268;
    /* 0x270 */ s32 field_0x270;
}; // Size: 0x274

#endif
