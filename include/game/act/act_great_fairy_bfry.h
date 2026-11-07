#ifndef GAME_ACT_ACT_GREAT_FAIRY_BFRY_H
#define GAME_ACT_ACT_GREAT_FAIRY_BFRY_H

// Code: game/act_great_fairy_bfry.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// Spawned for 'BFRY' (moved here from game/act/tu_801F9484.h). Named after its
// vtable, 0x804AF338. A spawn parameter with low nibble 3 is a fairy that a
// player summoned (its type is in bits 4-7, the player in bits 8-9).
class GreatFairy_804AF338 : public ActorBase_804A5770 {
public:
    GreatFairy_804AF338() {}
    /* 0x08 */ virtual ~GreatFairy_804AF338();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x8038DE74: picks the kind of fairy (field_0x29c) for player field_0x234.
    void fn_8038DE74();

    // SHARED-EDIT: carved from u8 field_0x230[0x78] (game/act_great_fairy_bfry.cpp).
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234; // player index of the summoner
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c[4];
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274[4];
    // Plain Vec (not TVec3f): a class-typed member would add constructor calls to the inline
    // constructor tu_801F9484 runs, and its unit would no longer link byte-identical.
    /* 0x284 */ Vec field_0x284;
    /* 0x290 */ Vec field_0x290;
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ JPABaseEmitter* field_0x2a0[2]; // released by the destructor
}; // Size: 0x2A8

#endif
