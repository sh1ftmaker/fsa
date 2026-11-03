#ifndef GAME_ACT_ACT_SMALL_KEY_H
#define GAME_ACT_ACT_SMALL_KEY_H

// Code: game/act_small_key.cpp.

#include "game/ActorBase.h"

// The KEY0 actor ("Small Key", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80202B08, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A8CD8.
class SmallKey_804A8CD8 : public ActorBase_804A5770 {
public:
    SmallKey_804A8CD8() {}
    /* 0x08 */ virtual ~SmallKey_804A8CD8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // 0x802C2BD0 (act_small_key's range): takes the key in r3, sets byte
    // 0x2B0 and passes its (0x230, 0x234) position and 0x238 to GameView's
    // fn_802D6C10. Called by act_invisible_npc_wall_tomi (0x803764BC).
    void fn_802C2BD0();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
    /* 0x240 */ s32 field_0x240; // state; 4 is tested by act_invisible_npc_wall_tomi
    /* 0x244 */ u8 field_0x244[0x74]; // not yet known
}; // Size: 0x2B8

#endif
