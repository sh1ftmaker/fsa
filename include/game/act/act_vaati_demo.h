#ifndef GAME_ACT_ACT_VAATI_DEMO_H
#define GAME_ACT_ACT_VAATI_DEMO_H

// Code: game/act_vaati_demo.cpp.

#include "game/act/tu_8010DF44.h"
#include "game/cls_801EB8CC.h"
#include "game/SpriteAnim.h"

// Named after its vtable, 0x8049D940. Derives from cls_8049D140. Its
// constructor, __ct__12cls_8049D940Fv (0x80116BE8, game/act_vaati_demo.cpp,
// was fn_80116BE8), runs the base constructor, sets the vtable, then
// initialises the members in declaration order.
class cls_8049D940 : public cls_8049D140 {
public:
    cls_8049D940();
    // __dt__12cls_8049D940Fv (0x80116B08, game/act_vaati_demo.cpp, was
    // fn_80116B08): clears 0x31C-0x32C, frees the emitters at 0x314 and 0x318
    // with fn_801EE434, then the members and the base destructor run.
    /* 0x08 */ virtual ~cls_8049D940();

    /* 0x58 */ virtual s32 vfunc_0x58();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234;
    /* 0x238 */ SpriteAnim_801D267C field_0x238;
    /* 0x270 */ SpriteAnim_801D267C field_0x270;
    /* 0x2A8 */ cls_801EB8CC field_0x2a8;
    /* 0x308 */ u8 field_0x308[0xC]; // not yet known
    /* 0x314 */ JPABaseEmitter* field_0x314;
    /* 0x318 */ JPABaseEmitter* field_0x318;
    /* 0x31C */ s32 field_0x31c;
    /* 0x320 */ s32 field_0x320;
    /* 0x324 */ s32 field_0x324;
    /* 0x328 */ s32 field_0x328;
    /* 0x32C */ s32 field_0x32c;
}; // Size: 0x330

#endif
