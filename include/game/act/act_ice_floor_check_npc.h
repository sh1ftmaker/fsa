#ifndef GAME_ACT_ACT_ICE_FLOOR_CHECK_NPC_H
#define GAME_ACT_ACT_ICE_FLOOR_CHECK_NPC_H

// Code: game/act_ice_floor_check_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049E950.
class cls_8049E950 : public ActorBase_804A5770 {
public:
    cls_8049E950();
    /* 0x08 */ virtual ~cls_8049E950();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    // Non-virtual, called by game/tu_8012F3E4.cpp: true once mState is 1.
    bool fn_8012F008();

    /* 0x230 */ s32 mState;   // 0 until update() sees the floor tile change, then 1
    /* 0x234 */ u16 mTile;    // fn_80226850 result at init()
    /* 0x236 */ u8 field_0x236[2]; // padding
}; // Size: 0x238

#endif
