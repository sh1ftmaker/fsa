#ifndef GAME_ACT_ACT_GREAT_FAIRY_SPFR_H
#define GAME_ACT_ACT_GREAT_FAIRY_SPFR_H

// Code: game/act_great_fairy_spfr.cpp. The class is named after its vtable,
// 0x8049EB8C (__vt__12cls_8049EB8C); only the slots the unit defines are declared.
// Derives from cls_8049D140 (game/act/tu_8010DF44.h): its constructor
// (0x801336B4, __ct__12cls_8049EB8CFv) calls cls_8049D140's first.

#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"

class cls_8049EB8C : public cls_8049D140 {
public:
    cls_8049EB8C();
    /* 0x08 */ virtual ~cls_8049EB8C();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual s32 vfunc_0x58();

    // Not yet written (0x80132454, same unit). Takes an index into lbl_80465204.
    void fn_80132454(s32 index);

    /* 0x230 */ u8 field_0x230; // set to 0 by the constructor, not yet known
    /* 0x234 */ s32 field_0x234; // state: fn_801321E4 switches on it
    /* 0x238 */ s32 field_0x238; // set to 0 by the constructor, not yet known
    /* 0x23C */ s32 field_0x23c; // set to -1 by the constructor, not yet known
    /* 0x240 */ s32 field_0x240; // set to 0 by the constructor, not yet known
    /* 0x244 */ s32 field_0x244; // set to 0 by the constructor, not yet known
    /* 0x248 */ s32 field_0x248; // set to 0 by the constructor, not yet known
    /* 0x24C */ s32 field_0x24c; // set to 0 by the constructor, not yet known
    /* 0x250 */ SpriteAnim_801D267C mAnim; // constructed after the words above
    /* 0x288 */ JPABaseEmitter* field_0x288; // emitter, freed by the destructor with releaseEmitter_801EE434; set to 0 by the constructor, after mAnim
}; // Size: at least 0x28C

#endif
