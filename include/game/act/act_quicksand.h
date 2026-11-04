#ifndef GAME_ACT_ACT_QUICKSAND_H
#define GAME_ACT_ACT_QUICKSAND_H

// Code: game/act_quicksand.cpp. RUSA, the quicksand (docs/names/actors.tsv).

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A6270 (still lbl_804A6270 in symbols.txt).
// Only the fields fn_80210E90 reads are known.
class cls_804A6270 : public EnemyBase_804A4808 {
public:
    // 0x80211078: zeroes field_0x3c, then calls fn_802110A8. Its only caller
    // is the vtable (slot 0x88, over EnemyBase's vfunc_0x88).
    /* 0x88 */ virtual void vfunc_0x88();
    // 0x802110A8, direct call from vfunc_0x88 (not virtual).
    void fn_802110A8();

    /* 0x2A8 */ u8 field_0x2a8[0x3A4 - 0x2A8]; // not yet known
    /* 0x3A4 */ f32 field_0x3a4; // the most fn_80210E90 pushes a position out by
}; // Size: not yet known

#endif
