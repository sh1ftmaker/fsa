#ifndef GAME_CLS_804A7BE8_H
#define GAME_CLS_804A7BE8_H

// Code: game/tu_8029C584.cpp. An actor whose vtable is at 0x804A7BE8
// (__vt__20UnknownFORM_804A7BE8; the spawner's FORM case, docs/names/actors.tsv,
// low confidence, so no real name yet). Named after its vtable. Its
// constructor is not in this unit (set by 0x8020130C); the destructor,
// init, update and draw are, and override the base's slots 0x8, 0xC, 0x14
// and 0x20. Slots 0x10, 0x18, 0x1C and 0x24-0x50 are the base's.

#include "game/ActorBase.h"

class UnknownFORM_804A7BE8 : public ActorBase_804A5770 {
public:
    // 0x8029CF44: vtable slot 0x8. r4 = the delete flag; > 0 frees the
    // object (__dl__FPv). Runs its own cleanup, then the base's destructor.
    virtual ~UnknownFORM_804A7BE8();

    // 0x8029CFF8: vtable slot 0xC. Sets up the actor from its position and
    // the player and view state; no arguments read.
    virtual void init();

    // 0x8029C584 (game/tu_8029C584.cpp): vtable slot 0x20. Keeps the sprite position on screen
    // (clamped to the view box), then draws the sprite and the shadow.
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230; // 0x3D/0x3F or 0x43/0x44 by viewLayerFlag(layer)
    /* 0x234 */ s32 field_0x234; // 1 after init
    /* 0x238 */ s32 field_0x238; // 0 after init
    /* 0x23C */ s32 field_0x23c; // fn_8022CAF8 of the player; 5 is turned to 3 or 0
    /* 0x240 */ s32 field_0x240; // the same value as field_0x23c after init
    /* 0x244 */ bool field_0x244; // 1 after init
    /* 0x248 */ s32 field_0x248; // field_0x90 & 3
    /* 0x24C */ bool field_0x24c; // 1 when fn_8022F514 is true for exactly three players
    /* 0x250 */ s32 field_0x250; // 0 after init
    /* 0x254 */ ActorBase_804A5770* field_0x254; // the FGAL actor (alive) in the manager
};

#endif
