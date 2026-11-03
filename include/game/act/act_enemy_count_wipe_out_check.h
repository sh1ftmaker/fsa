#ifndef GAME_ACT_ACT_ENEMY_COUNT_WIPE_OUT_CHECK_H
#define GAME_ACT_ACT_ENEMY_COUNT_WIPE_OUT_CHECK_H

// Code: game/act_enemy_count_wipe_out_check.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8250.
class cls_804A8250 : public ActorBase_804A5770 {
public:
    cls_804A8250() {}
    /* 0x08 */ virtual ~cls_804A8250();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // cleared in init, not read
    /* 0x234 */ s32 field_0x234; // field_0x90 bits 22-26; 0 counts all layer slots, else only field_0x4's
    /* 0x238 */ u32 field_0x238; // field_0x90 & 0xFF; counter value that triggers
    /* 0x23C */ u8 field_0x23c[4]; // not yet known
    /* 0x240 */ u8 field_0x240; // field_0x90 bit 21
    /* 0x244 */ u32 field_0x244; // lbl_8053AF80->field_0x58 at init
}; // Size: 0x248

#endif
