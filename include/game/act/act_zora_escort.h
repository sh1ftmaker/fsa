#ifndef GAME_ACT_ACT_ZORA_ESCORT_H
#define GAME_ACT_ACT_ZORA_ESCORT_H

// Code: game/act_zora_escort.cpp.

#include "game/tu_801213EC.h"

// The escorted Zora ('FAZR', docs/names/actors.tsv, med confidence). Named
// after its vtable, 0x8049DF78. Its constructor (fn_80121164) and destructor
// (fn_80121104) call the escort base's constructor and destructor.
class ZoraEscort_8049DF78 : public EscortBase_8049E0C8 {
public:
    // __ct__19ZoraEscort_8049DF78Fv: passes "enemy_both2_sob.bin", 0x242 and 0 to the base
    // constructor, then sets field_0x3c4 to 0, field_0x3c8 to 0, field_0x3cc to -1 and field_0x3d0 to 0.
    ZoraEscort_8049DF78();
    // fn_80121104: the destructor (calls the base's).
    /* 0x08 */ virtual ~ZoraEscort_8049DF78();
    // fn_801210A0: the base's fn_80124448; when field_0x90 bit 2 (mask 4) is set, takes bits 3-6
    // of field_0x90 as the state word field_0x344, registers itself with the escort list
    // (fn_80131F0C) and enters state 7.
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    // Slots the escort base (tu_801213EC, not written yet) declares only up
    // to 0x6C; the zora's own overrides are declared here in slot order.
    // Slots 0x54-0x5C: fn_80121040 (state 7 or 9-12), fn_80120FC0 (state 7, 11 or 12 and
    // the escort within 32 units of the player it names) and fn_80120F68 (3 in state 0 or 0xF,
    // else 2 when vfunc_0x58 holds, else 1).
    /* 0x54 */ virtual bool vfunc_0x54();
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();
    // 0x8011FFB8: returns 0 (the base slot's u8 result).
    /* 0x60 */ virtual u8 vfunc_0x60();
    // fn_80120F38: sets field_0x3c4 to 1 when field_0x90 bit 0 is set; field_0x35c and field_0x3d0
    // get bits 8-15 and 16-23 of field_0x90.
    /* 0x64 */ virtual void vfunc_0x64();
    /* 0x68 */ virtual void vfunc_0x68();
    /* 0x6C */ virtual s32 vfunc_0x6C(s32 state);
    // fn_80120118: the key tables of mAnim (14 of them, lbl_8049DF40), animation 7 in mode 2,
    // frame 0 (like the escort woman's).
    /* 0x70 */ virtual void vfunc_0x70();
    // fn_801200F8: the base's fn_80121DB0.
    /* 0x74 */ virtual void vfunc_0x74();
    // 0x8011FFC0: returns 0.
    /* 0x78 */ virtual s32 vfunc_0x78() const;
    // fn_801200C8: forwards 0xB to the state slot 0x6C.
    /* 0x84 */ virtual s32 vfunc_0x84();
    // Vtable slot 0x88, fn_80120098: forwards 9 to the state slot 0x6C.
    /* 0x88 */ virtual s32 vfunc_0x88();
    // Vtable slot 0x8C, fn_80120068: forwards 0xC to the state slot 0x6C.
    /* 0x8C */ virtual void vfunc_0x8C();
    /* 0x90 */ virtual void vfunc_0x90(); // 0x8011FFC8, empty
    /* 0x94 */ virtual void vfunc_0x94(); // 0x8011FFCC, empty
    // fn_80120048: forwards its arguments to the base's slot 0x9C.
    /* 0x9C */ virtual void vfunc_0x9C(s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
    // 0x80120040: returns 1.
    /* 0xC0 */ virtual s32 vfunc_0xC0();
    // fn_8011FFD0: false with fewer than two escorts registered (fn_80131F0C); otherwise whether
    // this actor's mSerial is the one of the escort at index 1.
    /* 0xC4 */ virtual bool vfunc_0xC4();

    // Dead-stripped helpers inlined into vfunc_0x68 (their source order explains .sdata2 pool order).
    s32 findNearbyPlayer();
    bool hasNearbyBari();
    bool stepToPlayer();

    /* 0x3C4 */ u8 field_0x3c4; // set by vfunc_0x64 from field_0x90 bit 0
    /* 0x3C8 */ s32 field_0x3c8; // 0
    /* 0x3CC */ s32 field_0x3cc; // -1
    /* 0x3D0 */ s32 field_0x3d0; // field_0x90 bits 16-23, set by vfunc_0x64
}; // Size: 0x3D4

#endif
