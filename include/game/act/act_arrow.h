#ifndef GAME_ACT_ACT_ARROW_H
#define GAME_ACT_ACT_ARROW_H

// Code: game/act_arrow.cpp.

#include "game/ActorBase.h"

// Spawned for 'ARRW' (Arrow (player), docs/names/actors.tsv, high confidence);
// moved here from game/act/tu_801F9484.h. Named after its vtable, 0x804A05E0.
class ArrowPlayer_804A05E0 : public ActorBase_804A5770 {
public:
    ArrowPlayer_804A05E0() {}
    /* 0x08 */ virtual ~ArrowPlayer_804A05E0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230; // lwz / stw in act_arrow (tested 0, 4); 0 lets the player's state 0x43 go on (tu_80256C74 fn_8025AB00)
    /* 0x234 */ f32 field_0x234; // 255.0f after init
    /* 0x238 */ u8 field_0x238;
    /* 0x239 */ u8 field_0x239[3]; // not yet known
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // 500 after init
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268; // direction (bits 4-5 of field_0x90)
    // SHARED-EDIT: tested by the ZLD2 actor's vfunc_0x44 (game/tu_80197070.cpp).
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ u8 field_0x274;
    /* 0x275 */ u8 field_0x275[3]; // not yet known
    /* 0x278 */ JPABaseEmitter* mEmitters[2]; // released by the destructor
}; // Size: 0x280

#endif
