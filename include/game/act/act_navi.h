#ifndef GAME_ACT_ACT_NAVI_H
#define GAME_ACT_ACT_NAVI_H

#include "types.h"
#include "game/EnemyBase.h"

class JPABaseEmitter;

// Code: game/act_navi.cpp. The Navi actor ('NAVI', docs/names/actors.tsv),
// an EnemyBase_804A4808 named after its vtable, 0x804A6708.
class cls_804A6708 : public EnemyBase_804A4808 {
public:
    // 0x8021EDC0.
    cls_804A6708();
    // 0x8021ED04: releases emitters and unlists serial from tu_8021F064 list.
    /* 0x08 */ virtual ~cls_804A6708();
    // 0x8021E8D0.
    /* 0x0C */ virtual void init();
    // 0x8021D7A8: draws SOB sprite "enemy_both2_sob.bin".
    /* 0x20 */ virtual void draw();
    // 0x8021DA20.
    /* 0x88 */ virtual void vfunc_0x88();
    // 0x8021DC44: state handler.
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    // 0x8021D9D4.
    /* 0x98 */ virtual void vfunc_0x98();
    // 0x8021DB24.
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    // 0x8021D938: focus point / screen flash.
    /* 0xA0 */ virtual void vfunc_0xA0();

    // 0x8021D734: sets state to 8 if not already 8.
    void fn_8021D734();
    // 0x8021D764: sets state to 6 if not already 6.
    void fn_8021D764();
    // 0x8021D794: returns true if state == 5.
    bool fn_8021D794();

    // 0x8021DFAC: state transition helper.
    void fn_8021DFAC(s32 newState);
    // 0x8021E4F8.
    void fn_8021E4F8();

    /* 0x2A8 */ s32 field_0x2a8;
    /* 0x2AC */ u8 field_0x2ac;
    /* 0x2AD */ u8 field_0x2ad;
    /* 0x2AE */ u8 pad_0x2ae[2];
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
    /* 0x2B8 */ f32 field_0x2b8;
    /* 0x2BC */ f32 field_0x2bc;
    /* 0x2C0 */ f32 field_0x2c0;
    /* 0x2C4 */ f32 field_0x2c4;
    /* 0x2C8 */ f32 field_0x2c8;
    /* 0x2CC */ f32 field_0x2cc;
    /* 0x2D0 */ JPABaseEmitter* mEmitter2D0;
    /* 0x2D4 */ JPABaseEmitter* mEmitter2D4;
}; // Size: 0x2D8

#endif
