#ifndef GAME_ACT_ACT_SHADOW_LINK_2_H
#define GAME_ACT_ACT_SHADOW_LINK_2_H

// Code: game/act_shadow_link_2.cpp (not written yet). Only the members
// that game/tu_8021CDE4.cpp uses are laid out; the rest is padding.

#include "game/EnemyBase.h"
#include "game/RailBase.h"
#include "game/cls_80214244.h"
#include "game/cls_80217374.h"

// The Shadow Link (SLK2) class: vtable 0x804A46F4 (stored by its
// destructor dtor_801C5854, which act_thief shares). Named after its
// vtable. fn_8021CDE4, fn_8021CFF8 and fn_8021D020 (game/tu_8021CDE4.cpp)
// take it in r3; fn_801C3B7C(this, int) is its state-change function.
class cls_804A46F4 : public EnemyBase_804A4808 {
public:
    // State change: sets up the state-dependent members and calls
    // fn_801C3B7C. 1, 2 and 3 do something; the old state ends up in
    // field_0x564. (game/tu_8021CDE4.cpp)
    void fn_8021CDE4(int state);
    // Whether field_0x564 is 3 and the timer field_0x59e ran out.
    bool fn_8021CFF8();
    // Whether the life lost since init is at least field_0x568.
    bool fn_8021D020();
    // act_shadow_link_2.cpp: switches the animation/state by number.
    void fn_801C3B7C(int);

    /* 0x2A8 */ u8 field_0x2a8[0x32C - 0x2A8];
    /* 0x32C */ JGeometry::TVec3f field_0x32c;
    /* 0x338 */ u8 field_0x338[0x358 - 0x338];
    /* 0x358 */ cls_80214244 field_0x358;
    /* 0x4EC */ u8 field_0x4ec[0x550 - 0x4EC];
    /* 0x550 */ cls_80217374 field_0x550;
    /* 0x55C */ u8 field_0x55c[0x564 - 0x55C];
    /* 0x564 */ s32 field_0x564;
    /* 0x568 */ s32 field_0x568;
    /* 0x56C */ s32 field_0x56c;
    /* 0x570 */ JGeometry::TVec3f field_0x570;
    /* 0x57C */ s32 field_0x57c;
    /* 0x580 */ JGeometry::TVec3f field_0x580;
    /* 0x58C */ s32 field_0x58c;
    /* 0x590 */ u8 field_0x590[0x59E - 0x590];
    /* 0x59E */ u16 field_0x59e;
}; // Size: at least 0x5A0

// Table at 0x8046A748 (0x28 bytes, 5 u16 per state; only [0] is read).
extern const u16 lbl_8046A748[][5];

#endif
