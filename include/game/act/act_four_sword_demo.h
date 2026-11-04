#ifndef GAME_ACT_ACT_FOUR_SWORD_DEMO_H
#define GAME_ACT_ACT_FOUR_SWORD_DEMO_H

// Code: game/act_four_sword_demo.cpp.

#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"

// Named after its vtable, 0x8049DA0C, which derives from cls_8049D140. Its
// constructor (0x801180D0, not written yet) clears field_0x230, field_0x231
// and field_0x234. The destructor (0x80118018) runs four rounds over the
// emitter arrays, releasing each slot and clearing it; mEmitter2a8 is
// released in every round. Then it destroys mAnim250 and runs
// cls_8049D140's destructor.
class cls_8049DA0C : public cls_8049D140 {
public:
    /* 0x08 */ virtual ~cls_8049DA0C();

    // Slot 0x58, overriding cls_8049D140::vfunc_0x58 (game/tu_8010DF44.cpp).
    // Returns field_0x234.
    /* 0x58 */ virtual s32 vfunc_0x58();

    // Slot 0x5C, overriding cls_8049D140::vfunc_0x5C (game/tu_8010DF44.cpp).
    // 1 when field_0x234 is 12, else 0.
    /* 0x5C */ virtual s32 vfunc_0x5C();

    /* 0x230 */ u8 field_0x230;
    /* 0x231 */ u8 field_0x231;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ u8 field_0x238[0x250 - 0x238]; // not yet known

    /* 0x250 */ SpriteAnim_801D267C mAnim250; // its J3DFrameCtrl at 0x274
    /* 0x288 */ JPABaseEmitter* mEmitters288[4];
    /* 0x298 */ JPABaseEmitter* mEmitters298[4];
    /* 0x2A8 */ JPABaseEmitter* mEmitter2a8;
}; // Size: at least 0x2AC

#endif
