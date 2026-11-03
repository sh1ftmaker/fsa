#ifndef GAME_ACT_ACT_SHADOW_LINK_2_H
#define GAME_ACT_ACT_SHADOW_LINK_2_H

// Code: game/act_shadow_link_2.cpp (not written yet). Only the members that
// game/tu_802188B0.cpp and game/tu_8021CDE4.cpp use are declared.

#include "game/cls_800FE8C0.h"
#include "game/cls_804A46F4.h"
#include "game/cls_80214244.h"
#include "game/cls_80217128.h"
#include "game/cls_80217374.h"
#include "game/cls_80218A8C.h"

// .sdata2 floats: 0x8053EFB8 is also read by act_shadow_link_jump.cpp.
extern const f32 lbl_8053EFB8;
extern const f32 lbl_8053EFBC;

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
    // act_shadow_link_2.cpp: true if mFlags has bit 0x40000 and the byte at
    // 0x55C is at most 0x14.
    bool fn_801C53C4();
    // act_shadow_link_2.cpp: gravity, EnemyBase's vfunc_0x84 override. Unless
    // field_0xd8 > 0 && field_0xf8 == 2, field_0x3c.z += lbl_8053EFB8, clamped
    // at -lbl_8053EFBC from below.
    /* 0x84 */ virtual void vfunc_0x84();
    // act_shadow_link_2.cpp: copies field_0x98 (direction) into the animation
    // sequencer's mDir, advances the sequencer unless it is in state 0x13 past
    // its first tick or mFlags has bit 0x80000, then updates the hit box.
    void fn_801C2E70();
    // act_shadow_link_2.cpp: switches to state 0xD (fn_801C3B7C only).
    void fn_801C26D8();
    // act_shadow_link_2.cpp: switches to state 0xE unless mState is 0xE.
    void fn_801C26FC();
    // act_shadow_link_2.cpp: if the object from fn_801C0D70 has bit 0x80000000
    // at 0x50 and mFlags has bit 0x2000, calls its fn_801C12B8.
    void fn_801C53F8();
    // act_shadow_link_2.cpp: if mFlags has bit 0x4000 and mState is 0 or 1,
    // switches to state 2 (fn_801C3B7C).
    void fn_801C27A8();
    // game/tu_801C6A4C.cpp: per-tick state handlers (field_0x340 anim
    // finished flags, then the next fn_801C3B7C state).
    void fn_801C6A4C();
    void fn_801C6BA8();
    void fn_801C6C7C();
    // game/tu_801C71BC.cpp: for a nonzero flag, sets the byte at 0x55D from
    // a rail query (fn_800FD894 on field_0x358) and fn_801CD664; otherwise
    // from fn_801CD664(0xC) alone.
    void fn_801C8BC4(u8 flag);
    // game/tu_801C71BC.cpp: rail step. Clears mFlags 0x2000000, sets or clears
    // 0x20000 from fn_800FD594 on field_0x358, reads the rail point at 0xFF
    // into field_0x32c, mPos and mPrevPos, steps the rail (fn_800FD818) in the
    // direction of its field_0x1, then fn_801C8BC4(1) and fn_801C3B7C 0x17/0x18.
    void fn_801C79A0();
    // game/tu_801C71BC.cpp (not written yet): called with (0, 1) and (0, 0),
    // the arguments are literals; s32 is a guess until its body is read.
    void fn_801C8C84(s32, s32);
    // game/tu_801C9AF8.cpp (not written yet): 0 if field_0x560 is 4 or
    // `player`, else 1.
    bool fn_801CAED4(int player);
    u8 fn_801C976C();
    bool fn_801CAEF4(const JGeometry::TVec3f* pos, s32 layer);
    // game/tu_801D3320.cpp: picks the next waypoint of the rail in
    // field_0x358 and moves field_0x32c toward it.
    void fn_801D3320();
    // game/tu_801D3CBC.cpp: per-tick draw by state (mState 0-3, 0xD: none;
    // 0x17 calls fn_801D41B8 while field_0x524 has bit 0x200).
    void fn_801D3CBC();
    // game/tu_801D3CBC.cpp: draws the player sprite (player_sob.bin) at mPos,
    // frame `frame`, faded in states 8 and 9, then the shadow sprite.
    void fn_801D3DAC(s32 frame) const;
    // game/tu_801D3CBC.cpp: draws a sprite at field_0xc or field_0x570 with an
    // alpha ramped by mStateTimer when the tile there is 0x9B or 0x9C.
    void fn_801D41B8() const;

    /* 0x340 */ cls_800FE8C0 field_0x340; // animation sequencer (mIndex at 0x34D, mFlags at 0x354)
    /* 0x358 */ cls_80214244 field_0x358;
    /* 0x4EC */ u8 field_0x4ec[0x524 - 0x4EC];
    /* 0x524 */ u32 field_0x524; // bits 0x8, 0x10, 0x20 pick the attack
    /* 0x528 */ cls_80217128 mHitBox; // mOwner is this actor
    /* 0x540 */ cls_80218A8C mHelper540;       // fn_801C573C calls its constructor
    /* 0x548 */ u8 field_0x548[0x550 - 0x548];
    /* 0x550 */ cls_80217374 field_0x550;
    /* 0x55C */ u8 field_0x55c[0x560 - 0x55C];
    /* 0x560 */ s32 field_0x560;
    /* 0x564 */ s32 field_0x564;
    /* 0x568 */ s32 field_0x568;
    /* 0x56C */ s32 field_0x56c;
    /* 0x570 */ JGeometry::TVec3f field_0x570;
    /* 0x57C */ s32 field_0x57c;
    /* 0x580 */ JGeometry::TVec3f field_0x580;
    /* 0x58C */ s32 field_0x58c;
    /* 0x590 */ u8 field_0x590[0x594 - 0x590];
    /* 0x594 */ u8 field_0x594; // counts finished animation loops
    /* 0x595 */ u8 field_0x595[0x59E - 0x595];
    /* 0x59E */ u16 field_0x59e;
    /* 0x5A0 */ u8 field_0x5a0[0x5B0 - 0x5A0];
    /* 0x5B0 */ f32 field_0x5b0; // squared against the distance to the player
}; // Size: at least 0x5B4


// Table at 0x8046A748 (0x28 bytes, 5 u16 per state; only [0] is read).
extern const u16 lbl_8046A748[][5];

#endif
