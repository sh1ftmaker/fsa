#ifndef GAME_CLS_804A6A00_H
#define GAME_CLS_804A6A00_H

#include "game/EnemyBase.h"

// The shared base of the minor Shadow Link actors (act_shadow_link_jump,
// act_shadow_link_bow, tu_80220F54, tu_80221B44, tu_80222CC4, tu_802233A4):
// vtable 0x804A6A00. Its constructor fn_802218C8 (game/tu_80220F54.cpp)
// calls EnemyBase's, stores this vtable and initialises 0x2A8-0x2B4; the
// derived destructors inline its destructor (vtable store, then EnemyBase's).
// Named after its vtable. Only the members the written code uses are
// declared; the code is in game/tu_80220F54.cpp (not written yet).
class cls_804A6A00 : public EnemyBase_804A4808 {
public:
    // 0x802218C8.
    cls_804A6A00();
    virtual ~cls_804A6A00() {}
    // 0x802217C8: EnemyBase::init, then the boxes, mFlags |= 0x300, mLife
    // 1000 and the fields below from field_0x90.
    /* 0x0C */ virtual void init();
    // 0x80220F54.
    /* 0x20 */ virtual void draw();
    // 0x80221158.
    /* 0x60 */ virtual bool vfunc_0x60();
    // 0x80221184: field_0x98 from the angle of field_0x3c, animation
    // field_0x98 + 0xC, then sound 0x400C2.
    /* 0x6C */ virtual void vfunc_0x6C();
    // 0x80221160.
    /* 0x74 */ virtual void vfunc_0x74();
    // 0x80221210.
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    // 0x80221114.
    /* 0xA0 */ virtual void vfunc_0xA0();

    // game/tu_80220F54.cpp: the per-state handlers cls_804A6C38's
    // vfunc_0x90 dispatches to (states 0, 1, 2), and the movement step
    // vfunc_0x88 takes in state 2 (fn_801D01A0 toward field_0x2b0, then
    // vtable slot 0x84).
    void fn_80221408();
    void fn_80221334();
    void fn_802212AC();
    void fn_80221250();
    // Sets the sprite animation and sound of `state` (0, 1, 2 ...).
    void fn_8022149C(s32 state);
    // Sets 0x2000 in mFlags once the state timer reaches `limit` (kind 0),
    // or a player is near (kinds 1, 2), or the timer reaches field_0x2ad.
    void fn_802216E8(s32 limit);

    /* 0x2A8 */ s32 field_0x2a8; // bits 4-5 of field_0x90
    /* 0x2AC */ u8 field_0x2ac;  // bits 8-9 of field_0x90: the kind
    /* 0x2AD */ u8 field_0x2ad;  // bits 12-19 of field_0x90
    /* 0x2B0 */ f32 field_0x2b0; // mPos.x at init
    /* 0x2B4 */ f32 field_0x2b4; // mPos.y at init
}; // Size: 0x2B8

// 2.5f in tu_80220F54's .sdata2; loaded by act_shadow_link_jump,
// tu_80221B44, act_shadow_link_bow, tu_80222CC4 and tu_802233A4.
extern const f32 lbl_8053FE70;

// mFlags bit fn_802216E8 sets, tested by cls_804A6C38's vfunc_0x90.
#define CLS_804A6A00_FLAG_0x2000 0x2000

#endif
