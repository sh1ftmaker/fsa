#ifndef GAME_ACT_ACT_SWORD_SOLDIER_H
#define GAME_ACT_ACT_SWORD_SOLDIER_H

// Code: game/act_sword_soldier.cpp.

#include "game/ActorBase.h"

// The SLR1 actor (Sword Soldier, docs/names/actors.tsv, high confidence).
// Named after its vtable, lbl_8049FCA0 (.data 0x270, size 0x58).
class SwordSoldier_8049FCA0 : public ActorBase_804A5770 {
public:
    /* 0x230 */ u8 field_0x230[0x58]; // not yet known
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ u8 field_0x294[0x2A5 - 0x294]; // not yet known
    /* 0x2A5 */ bool field_0x2a5;
    /* 0x2A6 */ u8 field_0x2a6[0x2C8 - 0x2A6]; // not yet known (size: the spawner's __nw__)
    s32 fn_801544AC() const;
    s32 fn_801544C0();
    void fn_80154490();
    void fn_8015449C();
    void fn_801544B4();
    void fn_801544D0(s32 value);
    bool fn_801545C0(JGeometry::TVec3f* out, bool flag);
    s32 fn_801545E0() const;
    void fn_80154614(s32 value);
};

#endif
