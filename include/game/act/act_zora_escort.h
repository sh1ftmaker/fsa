#ifndef GAME_ACT_ACT_ZORA_ESCORT_H
#define GAME_ACT_ACT_ZORA_ESCORT_H

// Code: game/act_zora_escort.cpp.

#include "game/tu_801213EC.h"

// The escorted Zora ('FAZR', docs/names/actors.tsv, med confidence). Named
// after its vtable, 0x8049DF78. Its constructor (fn_80121164) and destructor
// (fn_80121104) call the escort base's constructor and destructor.
class ZoraEscort_8049DF78 : public EscortBase_8049E0C8 {
public:
    ZoraEscort_8049DF78() : EscortBase_8049E0C8("enemy_both2_sob.bin", 0x242, 0) {}
    /* 0x14 */ virtual void update();
    // Slots the escort base (tu_801213EC, not written yet) declares only up
    // to 0x6C; the zora's own overrides are declared here in slot order.
    // 0x8011FFB8: returns 0 (the base slot's u8 result).
    /* 0x60 */ virtual u8 vfunc_0x60();
    // 0x8011FFC0: returns 0.
    /* 0x78 */ virtual s32 vfunc_0x78() const;
    // Vtable slot 0x88, fn_80120098: forwards 9 to the state slot 0x6C.
    /* 0x88 */ virtual s32 vfunc_0x88();
    // Vtable slot 0x8C, fn_80120068: forwards 0xC to the state slot 0x6C.
    /* 0x8C */ virtual void vfunc_0x8C();
    /* 0x90 */ virtual void vfunc_0x90(); // 0x8011FFC8, empty
    /* 0x94 */ virtual void vfunc_0x94(); // 0x8011FFCC, empty
    // 0x80120040: returns 1.
    /* 0xC0 */ virtual s32 vfunc_0xC0();

    /* 0x3C4 */ u8 field_0x3c4[0x3D4 - 0x3C4]; // not yet known (size: the spawner's __nw__)
}; // Size: 0x3D4

#endif
