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
    // with releaseEmitter_801EE434, then the members and the base destructor run.
    /* 0x08 */ virtual ~cls_8049D940();

    // Slot 0x54, overriding cls_8049D140::vfunc_0x54 (0x801162E0, game/act_vaati_demo.cpp).
    // Reads field_0x4 and field_0x8 of fn_8011921C()'s object and, when the
    // first is 2, calls fn_80115F9C with the index of the matching
    // lbl_804649A4 entry.
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual s32 vfunc_0x58();
    // Slot 0x5C, overriding cls_8049D140::vfunc_0x5C (0x801162B8, game/act_vaati_demo.cpp,
    // was fn_801162B8): 1 when field_0x230 is 11, else 0.
    /* 0x5C */ virtual s32 vfunc_0x5C();

    // Mangled fn_80115A7C__12cls_8049D940Fv (0x80115A7C, game/act_vaati_demo.cpp,
    // was fn_80115A7C). Called by fn_8011661C; returns true.
    bool fn_80115A7C();
    // Placeholder fn_80115BC4 (0x80115BC4, same unit, not written yet): takes
    // this, the position and an output vector.
    void fn_80115BC4(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out);

    // Mangled fn_801158B8__12cls_8049D940Fv (0x801158B8, same unit, not
    // written yet): called by draw() when field_0x230 is 10 or more.
    void fn_801158B8();

    // Slot 0x20, overriding cls_8049D140::draw (0x80116458, game/act_vaati_demo.cpp,
    // was fn_80116458).
    /* 0x20 */ virtual void draw();

    // Mangled fn_80115F9C__12cls_8049D940Fl (0x80115F9C, same unit, not written
    // yet). Takes the table index 1..11 found by vfunc_0x54.
    void fn_80115F9C(s32 index);

    // Named after its vtable, 0x8049D940, slot 0x0C. __ct__12cls_8049D940Fv
    // (game/act_vaati_demo.cpp) runs the base constructor, sets the vtable and
    // initialises the members in declaration order. init() (0x801169E8,
    // was fn_801169E8) runs cls_8049D140::init(), loads four 'TIMG'
    // resources named by lbl_8049D930 into field_0x31c[0..3], the kk_black.bti
    // resource into field_0x32c, sets up both SpriteAnims and copies mPos to
    // field_0x308.
    /* 0x0C */ virtual void init();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234;
    /* 0x238 */ SpriteAnim_801D267C field_0x238;
    /* 0x270 */ SpriteAnim_801D267C field_0x270;
    /* 0x2A8 */ cls_801EB8CC field_0x2a8;
    /* 0x308 */ JGeometry::TVec3f field_0x308; // mPos, copied by init()
    /* 0x314 */ JPABaseEmitter* field_0x314;
    /* 0x318 */ JPABaseEmitter* field_0x318;
    /* 0x31C */ void* field_0x31c[4]; // fn_804032E8('TIMG', lbl_8049D930[i])
    /* 0x32C */ void* field_0x32c;    // getResource('????', "kk_black.bti")
}; // Size: 0x330

#endif
