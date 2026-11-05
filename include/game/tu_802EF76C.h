#ifndef GAME_TU_802EF76C_H
#define GAME_TU_802EF76C_H

// Code: game/tu_802EF76C.cpp. The actor whose vtable is at 0x804AA4F0 (no
// owner or name is known yet; its spawner is in game/tu_801F9484.cpp, which
// stores the vtable). Named after its vtable.

#include "types.h"
#include "game/ActorBase.h"

// Owned by other units (not written yet). fn_80128C58 has C++ linkage
// (fn_80128C58__Fl; game/act/tu_801286C0.h declares it the same way).
// fn_8013A884 (game/tu_80139BE8.cpp) copies the four bytes at color into the
// colour of the object at lbl_8053AB10->0x2C; fn_801B7550 (game/tu_801B6C50.cpp)
// returns that object's byte at 0xC. fn_8018330C is in game/tu_80182914.h.
void fn_80128C58(s32 value);
extern "C" {
void fn_8013A884(u32* color);
u8 fn_801B7550(void);
}

class cls_804AA4F0 : public ActorBase_804A5770 {
public:
    cls_804AA4F0() { mColor.mWord = -1; }
    /* 0x08 */ virtual ~cls_804AA4F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Slot 0x54, appended after ActorBase's last slot. init() calls it with
    // the colour (r4-r7), a frame count (r8) and the state (r9): state 0 sets
    // the end colour and the frame count (the per-frame deltas are computed
    // here), any other state sets the start colour and the counter.
    /* 0x54 */ virtual void vfunc_0x54(s32 r, s32 g, s32 b, s32 a, s32 frames, s32 state);

    /* 0x230 */ s32 mState; // 0..5, the update's state machine; set by vfunc_0x54
    /* 0x234 */ f32 field_0x234; // -x of the camera's view translation (lbl_8053AB10->field_0x24->field_0x4[0][3]), set in init
    /* 0x238 */ f32 field_0x238; // -y of the same translation ([1][3]), set in init
    /* 0x23C */ f32 field_0x23c; // 0.0f, set in init
    /* 0x240 */ s32 field_0x240; // field_0x90 bits 22-26
    /* 0x244 */ s32 field_0x244; // field_0x90 & 0xFF, or -1; the switch index in init
    /* 0x248 */ SpriteParamsColor_801F5AE0 mColor; // written by fn_8018330C; its four bytes are split below
    /* 0x24C */ s32 mColorCur[4]; // r, g, b, a of mColor (0x258 is also set by vfunc_0x54)
    /* 0x25C */ s32 mColorEnd[3]; // end colour r, g, b (vfunc_0x54, state 0)
    /* 0x268 */ s32 mCounter; // frame counter, 0..mDuration
    /* 0x26C */ s32 mDuration; // frame count set by vfunc_0x54
    /* 0x270 */ s32 mColorStep[3]; // per-frame steps (end - cur) / 4 for r, g, b
}; // Size: at least 0x27C

#endif
