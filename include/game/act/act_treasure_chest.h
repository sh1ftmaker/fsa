#ifndef GAME_ACT_ACT_TREASURE_CHEST_H
#define GAME_ACT_ACT_TREASURE_CHEST_H

// Code: game/act_treasure_chest.cpp.

#include "game/ActorBase.h"

// The TKRA actor ("Treasure Chest", docs/names/actors.tsv, high confidence):
// the spawner constructs it with 80200C98, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AA178.
class TreasureChest_804AA178 : public ActorBase_804A5770 {
public:
    TreasureChest_804AA178() {}
    /* 0x08 */ virtual ~TreasureChest_804AA178();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // A slot after ActorBase's 19 (the last word of the vtable). update()
    // calls it on the chest and releases the chest when it returns true.
    /* 0x54 */ virtual bool vfunc_0x54();

    // Visits the four actors whose serials are in field_0x24c and returns
    // whether one of them was in state 3 (fn_801597B8) and was released
    // (fn_801597C0).
    bool fn_802EB27C();

    // The fields below are set in init() from the spawn flags (the actor's
    // field_0x90 word): bits 0-7 are field_0x234, bits 8-11 field_0x238 and
    // bits 12-15 field_0x244 (-1 from 4 up).
    /* 0x230 */ s32 field_0x230; // 0, 2 or 3 by field_0x238
    /* 0x234 */ s32 field_0x234; // the low byte of field_0x90, zeroed for some kinds
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 mCell;       // fn_80226BC4's packed cell index of (mPos.x, mPos.y)
    /* 0x240 */ s32 field_0x240; // not yet known
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248; // -1 after init
    /* 0x24C */ s32 field_0x24c[4]; // serials of four actors; negative when unused
    /* 0x25C */ JGeometry::TVec3f field_0x25c; // mPos before init's offset
};

#endif
