#ifndef GAME_ACT_ACT_SHADOW_LINK_2_H
#define GAME_ACT_ACT_SHADOW_LINK_2_H

// Code: game/act_shadow_link_2.cpp (not written yet). Only the members that
// game/tu_802188B0.cpp and game/tu_8021CDE4.cpp use are declared.

#include "game/cls_804A46F4.h"
#include "game/cls_80214244.h"
#include "game/cls_80217374.h"
#include "game/cls_80218A8C.h"

// The SLK2 actor ("Shadow Link 2", docs/names/actors.tsv, high confidence):
// the spawner constructs it with fn_801C573C, which calls the shared base's
// constructor and stores this vtable (0x804A43E8). Named after its vtable.
class ShadowLink2_804A43E8 : public cls_804A46F4 {
public:
    ShadowLink2_804A43E8() {}
    // game/tu_801C0D70.cpp. Whether a player is near mPos (fn_801D05F8 with
    // 1.0f, then fn_801D0D50 with `range`); 0 or 1.
    bool fn_801C5348(f32 range);
    // game/tu_8021CDE4.cpp: state change; 1, 2 and 3 set up the members
    // below and call fn_801C3B7C. The old state ends up in field_0x564.
    void fn_8021CDE4(int state);
    // Whether field_0x564 is 3 and the timer field_0x59e ran out.
    bool fn_8021CFF8();
    // Whether the life lost since init is at least field_0x568.
    bool fn_8021D020();
    // act_shadow_link_2.cpp: switches the animation/state by number.
    void fn_801C3B7C(int);

    /* 0x340 */ u8 field_0x340[0x358 - 0x340];
    /* 0x358 */ cls_80214244 field_0x358;
    /* 0x4EC */ u8 field_0x4ec[0x540 - 0x4EC];
    /* 0x540 */ cls_80218A8C mHelper540;       // fn_801C573C calls its constructor
    /* 0x548 */ u8 field_0x548[0x550 - 0x548];
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
    /* 0x5A0 */ u8 field_0x5a0[0x5B4 - 0x5A0];
}; // Size: at least 0x5B4

// Table at 0x8046A748 (0x28 bytes, 5 u16 per state; only [0] is read).
extern const u16 lbl_8046A748[][5];

#endif
