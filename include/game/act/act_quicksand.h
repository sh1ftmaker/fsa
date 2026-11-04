#ifndef GAME_ACT_ACT_QUICKSAND_H
#define GAME_ACT_ACT_QUICKSAND_H

// Code: game/act_quicksand.cpp. RUSA, the quicksand (docs/names/actors.tsv).

#include "game/EnemyBase.h"
#include "game/RailBase.h"

// Named after its vtable, 0x804A6270 (still lbl_804A6270 in symbols.txt).
// Only the fields fn_80210E90 and vfunc_0x54 read are known.
class cls_804A6270 : public EnemyBase_804A4808 {
public:
    // 0x80212170 (vtable slot 0x54, over EnemyBase's empty vfunc_0x54): if
    // mFlags has bit 0x8000 and field_0x2bc is set, the rail at field_0x2c0
    // is called with field_0x2bc, mPos and field_0x4.
    /* 0x54 */ virtual void vfunc_0x54();
    // 0x80211078: zeroes field_0x3c, then calls fn_802110A8. Its only caller
    // is the vtable (slot 0x88, over EnemyBase's vfunc_0x88).
    /* 0x88 */ virtual void vfunc_0x88();
    // 0x802110A8, direct call from vfunc_0x88 (not virtual).
    void fn_802110A8();

    /* 0x2A8 */ u8 field_0x2a8[0x2BC - 0x2A8]; // not yet known
    /* 0x2BC */ u8 field_0x2bc; // vfunc_0x54's first argument, and its test
    /* 0x2BD */ u8 field_0x2bd[0x2C0 - 0x2BD]; // not yet known
    /* 0x2C0 */ cls_800FE3EC field_0x2c0; // the rail vfunc_0x54 calls (0xC8 bytes)
    /* 0x388 */ u8 field_0x388[0x3A4 - 0x388]; // not yet known
    /* 0x3A4 */ f32 field_0x3a4; // the most fn_80210E90 pushes a position out by
}; // Size: not yet known

#endif
