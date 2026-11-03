#ifndef GAME_ACT_ACT_ENVIRONMENT_CONTROL_2_H
#define GAME_ACT_ACT_ENVIRONMENT_CONTROL_2_H

// Code: game/act_environment_control_2.cpp.

#include "game/ActorBase.h"
#include "game/ActorManager.h"

// Actor SEV2 ("Environment control 2" in docs/names/actors.tsv): its
// destructor stores the vtable 0x804A9CD0. Named after that vtable. The
// spawn parameter field_0x90 holds a flag per bit (16 of them, bits 11-26
// from the least significant end), copied to the s32 members in init.
class EnvironmentControl2_804A9CD0 : public ActorBase_804A5770 {
public:
    EnvironmentControl2_804A9CD0() {}
    /* 0x08 */ virtual ~EnvironmentControl2_804A9CD0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x802E43E0, not virtual: clears field_0x280.
    void fn_802E43E0();

    /* 0x230 */ ActorSoundObj_804578F4* mSound; // own sound object, made in init
    /* 0x234 */ s32 field_0x234; // queue mPos in the shared object (bit 26)
    /* 0x238 */ s32 field_0x238; // bit 25: second small request
    /* 0x23C */ s32 field_0x23c; // bit 24: queue mPos (up to two)
    /* 0x240 */ s32 field_0x240; // bit 23: field_0x284 counts up, else down
    /* 0x244 */ s32 field_0x244; // limit of field_0x284 (0xFF if field_0x240)
    /* 0x248 */ s32 field_0x248; // bit 22
    /* 0x24C */ s32 field_0x24c; // bit 21: plays sound 0x50078
    /* 0x250 */ s32 field_0x250; // bit 20
    /* 0x254 */ s32 field_0x254; // bit 19: field_0x285 counts up, else down
    /* 0x258 */ s32 field_0x258; // limit of field_0x285 (0xFF if field_0x254)
    /* 0x25C */ s32 field_0x25c; // bit 18
    /* 0x260 */ s32 field_0x260; // bit 17
    /* 0x264 */ s32 field_0x264; // bit 16
    /* 0x268 */ s32 field_0x268; // bit 15: field_0x286 counts up, else cleared
    /* 0x26C */ s32 field_0x26c; // limit of field_0x286 (0xFF if field_0x268)
    /* 0x270 */ s32 field_0x270; // bit 14: field_0x287 counts up, else down
    /* 0x274 */ s32 field_0x274; // limit of field_0x287 (0xFF if field_0x270)
    /* 0x278 */ s32 field_0x278; // bit 13
    /* 0x27C */ s32 field_0x27c; // bit 12
    /* 0x280 */ s32 field_0x280; // bit 11; field_0x288 counts down while 0
    /* 0x284 */ u8 field_0x284; // mirrored into the shared object's 0x1F2
    /* 0x285 */ u8 field_0x285; // ... 0x1F3
    /* 0x286 */ u8 field_0x286; // ... 0x1F4
    /* 0x287 */ u8 field_0x287; // ... 0x1F5
    /* 0x288 */ u8 field_0x288; // ... 0x1EF
}; // Size: unknown (>= 0x289)

#endif
