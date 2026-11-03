#ifndef GAME_ACT_ACT_SHADOW_LINK_JUMP_H
#define GAME_ACT_ACT_SHADOW_LINK_JUMP_H

#include "game/cls_804A6A00.h"

// A float in .sdata2 (0x8053EFB8), read by vfunc_0x88 and fn_8022089C.
extern const f32 lbl_8053EFB8;

// A minor Shadow Link actor (sub-base cls_804A6A00) that waits until a player
// is near, then hops along field_0x98 (a 16-bit direction) a few times and
// wanders off. Code in game/act_shadow_link_jump.cpp. Named after its vtable
// (0x804A68D0). The constructor is at 0x80220C48, the deleting destructor
// at 0x802205E0.
class cls_804A68D0 : public cls_804A6A00 {
public:
    cls_804A68D0();
    virtual ~cls_804A68D0();
    /* 0x0C */ virtual void init();
    // 0x80220680: the movement step. State 2 calls cls_804A6A00's
    // fn_80221250; states 4 and 6 add lbl_8053EFB8 to field_0x3c.z;
    // others zero field_0x3c.xy and call vfunc_0x84.
    /* 0x88 */ virtual void vfunc_0x88();
    // 0x802206FC: the per-state handlers (states 0-6).
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);

    // 0x8022089C: sets mState to `state`, after the state's box and velocity
    // setup (3-6; others only reset mStateTimer), then clears mStateTimer.
    void fn_8022089C(s32 state);

    // field_0x90 & 3, chosen at init(): the hop direction pattern (0-3).
    /* 0x2B8 */ s32 mKind;
    // Hops made; state 4 adds one and odd counts flip the velocity's sign.
    /* 0x2BC */ s32 mHops;
    // Hops before the next state (3 or 4, from fn_801CD664(1)).
    /* 0x2C0 */ s32 field_0x2c0;
}; // Size: 0x2C4

#endif
