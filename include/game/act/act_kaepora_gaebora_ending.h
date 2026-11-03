#ifndef GAME_ACT_ACT_KAEPORA_GAEBORA_ENDING_H
#define GAME_ACT_ACT_KAEPORA_GAEBORA_ENDING_H

// Code: game/act_kaepora_gaebora_ending.cpp.

#include "game/ActorBase.h"
#include "game/tu_8021A570.h"

// Named after its vtable, 0x804ADAC8.
class cls_804ADAC8 : public ActorBase_804A5770 {
public:
    cls_804ADAC8() {}
    /* 0x08 */ virtual ~cls_804ADAC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Advances the swinging head (mSway) while field_0x2B0 is 1, and steps
    // through the table of sway targets (lbl_804742A8) once the head is idle.
    void fn_8035FF70();
    void fn_80360914();

    /* 0x230 */ u8 field_0x230[0x4]; // not yet known
    /* 0x234 */ s32 field_0x234;     // countdown, loaded from the table
    /* 0x238 */ u8 field_0x238[0x4]; // not yet known
    /* 0x23C */ s32 field_0x23C;     // value loaded from the table
    /* 0x240 */ s32 mStep;           // index into lbl_804742A8
    /* 0x244 */ u8 field_0x244[0x2C]; // not yet known
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ u8 field_0x274[0xC]; // not yet known
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ u8 field_0x284[0x1C]; // not yet known
    /* 0x2A0 */ cls_8021ACE0 mSway;
    /* 0x2B0 */ u8 field_0x2B0;      // gates the sway logic (1 = active)
    /* 0x2B1 */ u8 field_0x2B1[0xB]; // not yet known
}; // Size: 0x2BC

#endif
