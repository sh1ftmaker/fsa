#ifndef GAME_ACT_ACT_SWITCH_RELATED_NPC_H
#define GAME_ACT_ACT_SWITCH_RELATED_NPC_H

// Code: game/act_switch_related_npc.cpp.

#include "game/ActorBase.h"

// The SWNP actor ("Switch-related NPC", docs/names/actors.tsv, med
// confidence); the spawner constructs it with fn_80202BBC, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8C28.
class SwitchRelatedNpc_804A8C28 : public ActorBase_804A5770 {
public:
    SwitchRelatedNpc_804A8C28() {}
    /* 0x08 */ virtual ~SwitchRelatedNpc_804A8C28();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;  // not touched by this unit
    /* 0x234 */ s32 mType;        // field_0x90 & 0x3F: 0 sets the cell tile, 1 tile from table, 2 actor-id slot, 3 despawn
    /* 0x238 */ s32 field_0x238;  // (field_0x90 >> 6) & 0xF, unused here
    /* 0x23C */ s32 mTileIndex;   // (field_0x90 >> 10) & 0xF: index into the first tile table
    /* 0x240 */ s32 mTable;       // (field_0x90 >> 14) & 0xF: index into the 2nd/3rd tile tables
    /* 0x244 */ s32 mSound;       // (field_0x90 >> 18) & 0xF: selects the sound played in update()
    /* 0x248 */ s32 mDelay;       // frames to wait before acting (set to 0x32 by init() for type 3 / sound 0)
    /* 0x24C */ s32 mCell;        // fn_80226BC4's packed cell index of (mPos.x, mPos.y)
    // Out-parameter of GameView::fn_802D4874 (mBit, type 0) or fn_802D4768
    // (mSlot, type 2); update() passes its low word (0x254) as the slot.
    /* 0x250 */ union {
        u64 mBit;
        s64 mSlot;
    };
    /* 0x258 */ u64 mMask;  // the screen's mask (the other out-parameter)
    /* 0x260 */ bool mEffects;    // field_0x90 bit 26: spawn effects and sound when done
}; // Size: 0x268

#endif
