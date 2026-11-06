#ifndef GAME_ACT_ACT_FROSTARE_TAIL_H
#define GAME_ACT_ACT_FROSTARE_TAIL_H

#include "types.h"
#include "game/EnemyBase.h"

// Code: game/act_frostare_tail.cpp. The Frostare tail ('BOIT',
// docs/names/actors.tsv), an EnemyBase_804A4808 named after its vtable,
// 0x804A4F90 (the class name stays provisional). Its serial goes to the
// Frostare singleton (Singleton_801DAC6C::fn_801DAF3C in init, cleared by the
// destructor). Each player hit (vfunc_0x5C) drops a rupee and counts in
// mHitCount; the tenth hit's reaction (vfunc_0x74) switches to state 1.
class cls_804A4F90 : public EnemyBase_804A4808 {
public:
    // 0x801DCA30.
    cls_804A4F90();
    // 0x801DC990: clears the singleton's mSerial4.
    /* 0x08 */ virtual ~cls_804A4F90();
    // 0x801DC89C.
    /* 0x0C */ virtual void init();
    // 0x801DC2D4: sprite mAnim.mCel + 0xE3 of "enemy_both_sob.bin".
    /* 0x20 */ virtual void draw();
    // 0x801DC3C4: the players hitting the tail (fn_801F285C, fn_801F37AC);
    // each one spawns a 'RUPY' thrown up and away.
    /* 0x5C */ virtual u8 vfunc_0x5C();
    // 0x801DC364: actor hits (fn_801F2DB4) are processed but never reported.
    /* 0x60 */ virtual u8 vfunc_0x60();
    // 0x801DC6B4.
    /* 0x74 */ virtual void vfunc_0x74();
    // 0x801DC758.
    /* 0x88 */ virtual void vfunc_0x88();
    // 0x801DC7D0: states 1 (shake the view for 60 frames) and 2 (removed
    // after 120 frames).
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    // 0x801DC790.
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);

    // 0x801DC264, called by Singleton_801DAC6C::fn_801DAD2C: in animation 1,
    // a 0-255 fade of the hit-reaction timer field_0xd8 over its first or
    // last 25 frames (clrlwi 24 in the body, returned without extending).
    u8 fn_801DC264();

    /* 0x2A8 */ u8 mHitCount; // player hits; cleared by the constructor
}; // Size: 0x2AC

#endif
