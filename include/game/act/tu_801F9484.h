#ifndef GAME_ACT_TU_801F9484_H
#define GAME_ACT_TU_801F9484_H

// Actor classes the spawner (game/tu_801F9484.cpp) constructs whose own units are not written
// yet: what their inline constructors show (vtable store, member initialisers, size from
// __nw__), named after their vtables. When a class's unit is written, move it to that
// unit's act header.

#include "game/ActorBase.h"
#include "game/RailBase.h"
#include "game/act/act_all_enemies_defeated.h"
#include "game/act/act_arrow.h"
#include "game/act/act_flame.h"
#include "game/act/act_gohma.h"
#include "game/act/act_great_fairy_bfry.h"
#include "game/act/act_wallmaster.h"
#include "game/act/act_wizzrobe.h"
#include "game/act/act_zora_fish_herder.h"
#include "game/act/tu_8018EB94.h"
#include "game/act/tu_801B6C50.h"
#include "game/act/tu_803D9944.h"
#include "game/act/act_princess_zelda_ending.h"
#include "game/act/tu_8010DF44.h"
#include "game/act/tu_801D2C60.h"
#include "game/act/act_frostare_tail.h"
#include "game/act/act_navi.h"
#include "game/act/act_dodongo.h"
#include "game/act/tu_80362634.h"
#include "game/SpriteAnim.h"
#include "game/act/tu_801F9484_elems.h"
#include "JSystem/JUtility/TColor.h"

class JPABaseEmitter;

// Spawned for 'LPLK'.
class ForceLike_8049C578 : public ActorBase_804A5770 {
public:
    ForceLike_8049C578() {}
    /* 0x08 */ virtual ~ForceLike_8049C578();
    /* 0x0C */ virtual void init(); // game/act_force_like.cpp (0x800F7CC8)
    virtual void update(); // 0x14: 0x800F6D80
    virtual void draw();   // 0x20: 0x800F6AB4
    virtual void vfunc_0x24(); // 0x800F6820
    // SHARED-EDIT: carved from field_0x230[0x38] (game/act_force_like.cpp).
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234;
    /* 0x235 */ u8 field_0x235;
    /* 0x236 */ u8 field_0x236[2];
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ u8 field_0x23c;
    /* 0x23D */ u8 field_0x23d;
    /* 0x23E */ s16 field_0x23e; // heading (JMath angle)
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ JGeometry::TVec3f field_0x254; // direction away from the player, plain float pair + z in the asm
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
}; // Size: 0x268

// Spawned for 'SIBT'.
#include "game/act/act_temple_bti_display.h"

#include "game/act/act_princess_zelda.h"

#include "game/act/act_snowball.h"

// Spawned for 'DMCR'.
class cls_8049D83C : public ActorBase_804A5770 {
public:
    cls_8049D83C();
    /* 0x08 */ virtual ~cls_8049D83C();
    /* 0x230 */ u8 field_0x230[0xC0];
}; // Size: 0x2F0

// Spawned for 'DMGA'.
#include "game/act/tu_8011CEC8.h"

// Spawned for 'RNBW'.
#include "game/act/act_rainbow_bridge.h"

// Spawned for 'DMTW'.
class cls_8049E770 : public ActorBase_804A5770 {
public:
    cls_8049E770() {}
    /* 0x08 */ virtual ~cls_8049E770();
    /* 0x230 */ u8 field_0x230[0x2C];
    /* 0x25C */ JGeometry::TVec3f field_0x25c[6];
    /* 0x2A4 */ u8 field_0x2a4[0x24];
    /* 0x2C8 */ JGeometry::TVec3f field_0x2c8[4];
    /* 0x2F8 */ JGeometry::TVec3f field_0x2f8[4];
    /* 0x328 */ u8 field_0x328[0x90];
    /* 0x3B8 */ JGeometry::TVec3f field_0x3b8[4];
    /* 0x3E8 */ u8 field_0x3e8[0x20];
}; // Size: 0x408

// Spawned for 'SNWL'.
class cls_8049E8A0 : public ActorBase_804A5770 {
public:
    cls_8049E8A0();
    /* 0x08 */ virtual ~cls_8049E8A0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // No address (dead-stripped; update inlines both): state 0 once the fire
    // rod hits (emitters, then after 60 frames the melt), state 1 (shrink).
    void melt();
    void shrink();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ f32 field_0x238;
    /* 0x23C */ void* field_0x23c;
    /* 0x240 */ JPABaseEmitter* field_0x240;
    /* 0x244 */ JPABaseEmitter* field_0x244;
}; // Size: 0x248

// Spawned for 'CRNV'.
class cls_8049ED30 : public ActorBase_804A5770 {
public:
    cls_8049ED30() : field_0x230(0), field_0x234(0), field_0x238(-1), field_0x23c(0.0f) {
        mEmitters[0] = NULL;
        mEmitters[1] = NULL;
    }
    /* 0x08 */ virtual ~cls_8049ED30();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    void fn_801339EC();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ SpriteAnim_801D267C field_0x240;
    /* 0x278 */ JPABaseEmitter* mEmitters[2];
}; // Size: 0x280

bool fn_80133B58();

#include "game/act/tu_80147B50.h"

#include "game/act/act_hinox.h"

// Spawned for 'SLR2'. Code: game/tu_801555D8.cpp.
class cls_804A00C8 : public ActorBase_804A5770 {
public:
    cls_804A00C8() {}
    /* 0x08 */ virtual ~cls_804A00C8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // 0x80157E20: clears the partner's (field_0x284) knock-back velocity, takes one off the count of
    // the actor field_0x280 names when this one's life is spent.
    /* 0x44 */ virtual void vfunc_0x44();
    // 0x80157C68: after a landing on a tile of kind 2, 3 or 0xE puts it in state 0xC; true when it did.
    /* 0x54 */ virtual bool vfunc_0x54();
    /* 0x230 */ s32 mState;
    /* 0x234 */ s16 field_0x234;
    /* 0x236 */ u8 field_0x236[2];
    // SHARED-EDIT: written by the 'SLR4' actor (game/act_chief_soldier.cpp).
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ s32 field_0x284;
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ f32 field_0x294;
    /* 0x298 */ f32 field_0x298;
    /* 0x29C */ f32 field_0x29c;
    /* 0x2A0 */ f32 field_0x2a0;
    /* 0x2A4 */ f32 field_0x2a4;
    /* 0x2A8 */ u8 field_0x2a8;
    /* 0x2A9 */ u8 field_0x2a9;
    /* 0x2AA */ s16 field_0x2aa;
    /* 0x2AC */ f32 field_0x2ac;
    /* 0x2B0 */ f32 field_0x2b0;
    /* 0x2B4 */ f32 field_0x2b4;
    /* 0x2B8 */ JGeometry::TVec3f field_0x2b8;
}; // Size: 0x2C4

// Spawned for 'BPOH'. Code: game/tu_801618D0.cpp.
class cls_804A0440 : public ActorBase_804A5770 {
public:
    cls_804A0440() {}
    /* 0x08 */ virtual ~cls_804A0440();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    // 0x80161F98: switches to colour set `set` (fading over `frames`; keeps fading when called again).
    void fn_80161F98(s32 set, s32 frames);
    // 0x801618D0: draws the ghost quad (obake_*.bti, four TEV stages) at pos; the colours
    // come from field_0x2f4 and, if given, params' two colours.
    void fn_801618D0(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params);
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ u8 field_0x264[4]; // not yet known
    /* 0x268 */ s32 field_0x268; // kind (low 2 bits of field_0x90)
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270; // GameView bit set by vfunc_0x24 (bits 2-6 of field_0x90)
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ s16 field_0x290;
    /* 0x292 */ u8 field_0x292[2]; // not yet known
    /* 0x294 */ f32 field_0x294[9]; // three positions (x, y, z) copied from mPos by init
    /* 0x2B8 */ JGeometry::TVec3f field_0x2b8[4];
    /* 0x2E8 */ JPABaseEmitter* field_0x2e8;
    /* 0x2EC */ s32 field_0x2ec; // colour set in use (-1: none yet)
    /* 0x2F0 */ s32 field_0x2f0; // frames left of the fade
    /* 0x2F4 */ JUtility::TColor field_0x2f4[3];
}; // Size: 0x300

// Spawned for 'SLR3'. Code: game/tu_80170008.cpp.
class cls_804A0810 : public ActorBase_804A5770 {
public:
    cls_804A0810() {}
    /* 0x08 */ virtual ~cls_804A0810();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ s32 field_0x230; // state (0-3)
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268; // the life at init
    /* 0x26C */ s32 field_0x26c; // serial of the 'SLR3' helper actor, 0 for none
    /* 0x270 */ s32 field_0x270; // bit 8 of field_0x90
    /* 0x274 */ u8 field_0x274;
    /* 0x275 */ u8 field_0x275[3]; // not yet known
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ JPABaseEmitter* field_0x28c;
    /* 0x290 */ f32 field_0x290[3];
    /* 0x29C */ JGeometry::TVec3f field_0x29c[10];
    /* 0x314 */ JGeometry::TVec3f field_0x314; // the shell's draw position
    // SHARED-EDIT: carved from field_0x320[0xC] (game/tu_80170008.cpp). Plain Vec, not TVec3f, so the spawner's inline ctor does not emit a construct call.
    /* 0x320 */ Vec field_0x320;
    /* 0x32C */ f32 field_0x32c[3]; // copy of mPos made by init
}; // Size: 0x338


// Spawned for 'FGAN'.
class cls_804A1318 : public ActorBase_804A5770 {
public:
    cls_804A1318() {}
    /* 0x08 */ virtual ~cls_804A1318();
    /* 0x0C */ virtual void init();   // fn_8017E770
    /* 0x14 */ virtual void update(); // fn_8017C40C, not written yet
    /* 0x20 */ virtual void draw();   // fn_8017C078
    /* 0x24 */ virtual void vfunc_0x24(); // fn_8017C01C
    // SHARED-EDIT: carved from field_0x230[0x8A] / field_0x2bb[0x25] (game/tu_8017C01C.cpp).
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ f32 field_0x294;
    /* 0x298 */ f32 field_0x298;
    /* 0x29C */ f32 field_0x29c;
    /* 0x2A0 */ JGeometry::TVec3f field_0x2a0;
    /* 0x2AC */ f32 field_0x2ac;
    /* 0x2B0 */ f32 field_0x2b0;
    /* 0x2B4 */ f32 field_0x2b4;
    /* 0x2B8 */ s16 field_0x2b8;
    /* 0x2BA */ s8 field_0x2ba;
    /* 0x2BB */ u8 field_0x2bb;
    /* 0x2BC */ JPABaseEmitter* mEmitters2bc[4];
    /* 0x2CC */ JPABaseEmitter* mEmitters2cc[4];
    /* 0x2DC */ JPABaseEmitter* mEmitter2dc;
}; // Size: 0x2E0

// Spawned for 'BPH2'.
class cls_804A1E50 : public ActorBase_804A5770 {
public:
    cls_804A1E50() {}
    /* 0x08 */ virtual ~cls_804A1E50();
    /* 0x230 */ u8 field_0x230[0xC8];
    /* 0x2F8 */ JGeometry::TVec3f field_0x2f8[4];
    /* 0x328 */ u8 field_0x328[0xC];
}; // Size: 0x334

// Spawned for 'ZLD2'.
class cls_804A2944 : public ActorBase_804A5770 {
public:
    cls_804A2944() {}
    /* 0x08 */ virtual ~cls_804A2944();
    // 0x8019C500 (game/tu_80197070.cpp): for each present player on this actor's
    // layer, draws a marker (fn_80230498) at its stored position field_0x2a4[i];
    // the y is this actor's y plus a constant unless `keepY`.
    void fn_8019C500(s32 keepY);
    // 0x80197070 (draw), 0x80197550 (update), 0x8019C8A4 (init), 0x8019C5B8 (vfunc_0x44):
    // game/tu_80197070.cpp.
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    void fn_801976E8();
    void fn_801979C4();
    void fn_80197EB0();
    void fn_80198828();
    // SHARED-EDIT: carved from field_0x230[0x74] (game/tu_80197070.cpp).
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ u8 field_0x268;
    /* 0x269 */ u8 field_0x269[3];
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    // Plain SDK Vec, not TVec3f: the spawner's inline constructor (cls_804A2944() {}) emits no
    // __construct_array for this range, and every use is a component-wise x/y/z copy.
    /* 0x280 */ Vec field_0x280[3];
    /* 0x2A4 */ JGeometry::TVec3f field_0x2a4[4];
    /* 0x2D4 */ JGeometry::TVec3f field_0x2d4[4];
    // SHARED-EDIT: thirteen emitters, each freed by the destructor (game/tu_80197070.cpp).
    /* 0x304 */ JPABaseEmitter* mEmitters[13];
}; // Size: 0x338


#include "game/act/act_shooting_gallery.h"

// Spawned for 'TENN'.
#include "game/act/tu_801A0710.h"

// Spawned for 'ZLD3'. Code: game/tu_801B0A00.cpp.
class cls_804A35AC : public ActorBase_804A5770 {
public:
    cls_804A35AC() {}
    /* 0x08 */ virtual ~cls_804A35AC();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // 0x801B2000: moves the players' positions (mode 0) or eases them to y - yOffset (mode 1).
    void fn_801B2000(f32 yOffset, s32 mode);
    void fn_801B2138(s32 arg);
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ u8 field_0x258;
    /* 0x259 */ u8 field_0x259;
    /* 0x25A */ u8 field_0x25a[2]; // not yet known
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264[9]; // three positions copied from mPos by init
    /* 0x288 */ JGeometry::TVec3f field_0x288[4];
    /* 0x2B8 */ JPABaseEmitter* mEmitters[2];
}; // Size: 0x2C0

// Spawned for 'ZLD4'.
class cls_804A3698 : public ActorBase_804A5770 {
public:
    cls_804A3698() {}
    /* 0x08 */ virtual ~cls_804A3698();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ cls_800FE3EC mRail; // update steps it (fn_800FDEA0) while field_0x2f8 != 0
    /* 0x2F8 */ s32 field_0x2f8; // state
    /* 0x2FC */ u8 field_0x2fc;
    /* 0x2FD */ u8 field_0x2fd[3]; // not yet known
    /* 0x300 */ s32 field_0x300;
    /* 0x304 */ s32 field_0x304;
    /* 0x308 */ s32 field_0x308; // draw skips the sprite while bit 1 is clear and it is non-zero
    /* 0x30C */ s32 field_0x30c;
    /* 0x310 */ s32 field_0x310;
    /* 0x314 */ s32 field_0x314;
    /* 0x318 */ s32 field_0x318;
    /* 0x31C */ s32 field_0x31c;
    /* 0x320 */ s32 field_0x320; // draw's frame - 0x9B
    /* 0x324 */ s32 field_0x324; // serial of the 'LIMT' life meter init spawns
    /* 0x328 */ s32 field_0x328; // init: 5; the life meter's heart index
    /* 0x32C */ u8 field_0x32c;
    /* 0x32D */ u8 field_0x32d;
    /* 0x32E */ u8 field_0x32e[2]; // not yet known
    /* 0x330 */ s32 field_0x330; // init: field_0x90 & 7
    /* 0x334 */ f32 field_0x334; // update eases field_0x3c.x towards it
    /* 0x338 */ f32 field_0x338; // update eases field_0x3c.y towards it
    /* 0x33C */ f32 field_0x33c;
    /* 0x340 */ f32 field_0x340; // init: mPos.z
}; // Size: 0x344

// Spawned for 'TLWP'.
class cls_804A38D4 : public ActorBase_804A5770 {
public:
    cls_804A38D4() {}
    /* 0x08 */ virtual ~cls_804A38D4();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    void fn_801B4EC8();
    void fn_801B5228();
    void fn_801B56A0();
    /* 0x230 */ s32 field_0x230; // state; init: 0, or 8 when field_0x250 != 0
    /* 0x234 */ s32 mTimers[3]; // update counts them down to 0
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250; // bits 4-7 of field_0x90: update runs fn_801B56A0 at 0, fn_801B5228 at 1-3
    /* 0x254 */ s32 field_0x254; // bits 0-3 of field_0x90
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c; // init: 1.0f
    /* 0x260 */ u16 field_0x260;
    /* 0x262 */ u8 field_0x262[2]; // not yet known
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ JPABaseEmitter* mEmitters294[3];
    /* 0x2A0 */ JPABaseEmitter* mEmitters2A0[20]; // init spawns one per lbl_804A37E0 id
}; // Size: 0x2F0

#include "game/act/act_bird_manager.h"


// Spawned for 'ZSLD': cls_804A6D00, game/act/tu_802233A4.h.
#include "game/act/tu_802233A4.h"

// Spawned for 'GOAL'. Code: game/tu_802A4654.cpp.
class cls_804A7EB0 : public ActorBase_804A5770 {
public:
    cls_804A7EB0() {}
    /* 0x08 */ virtual ~cls_804A7EB0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Slot 0x54: shakes the actor over four steps (a camera focus point each) and restores mPos.
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234; // bits 12-16 of field_0x90: index into the shake tables
    /* 0x238 */ s32 field_0x238; // bits 0-4
    /* 0x23C */ s32 field_0x23c; // bits 17-21
    /* 0x240 */ s32 field_0x240; // bits 22-26
    /* 0x244 */ s32 field_0x244; // state timer (update)
    /* 0x248 */ u8 field_0x248[0x24C - 0x248]; // not yet known
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254[4];
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ u8 field_0x294[0x2A4 - 0x294]; // not yet known
    /* 0x2A4 */ s32 field_0x2a4[4];
    /* 0x2B4 */ JPABaseEmitter* mEmitters[5][4];
    /* 0x304 */ s32 field_0x304;
    /* 0x308 */ u8 field_0x308[4]; // not yet known
    /* 0x30C */ s32 field_0x30c;
    /* 0x310 */ u8 field_0x310;
    /* 0x311 */ u8 field_0x311[3]; // not yet known
    /* 0x314 */ s32 field_0x314;
    /* 0x318 */ s32 field_0x318;
    /* 0x31C */ s32 field_0x31c;
}; // Size: 0x320

// Spawned for 'SWTH'.
class Switch_804A7F38 : public ActorBase_804A5770 {
public:
    Switch_804A7F38() {}
    /* 0x08 */ virtual ~Switch_804A7F38();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x54 */ virtual void vfunc_0x54(s32 arg1);
    void readTileId();

    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ u16 field_0x258;
    /* 0x25A */ u8 field_0x25a[2];
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
}; // Size: 0x268

// Spawned for 'FIRE'.
class FireRodFire_804A89F0 : public ActorBase_804A5770 {
public:
    FireRodFire_804A89F0() {}
    /* 0x08 */ virtual ~FireRodFire_804A89F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x54 */ virtual void vfunc_0x54(const JGeometry::TVec3f* pos, bool arg2);
    /* 0x58 */ virtual void vfunc_0x58();

    s32 fn_802BA9F0();
    s32 fn_802BA9F8();
    void fn_802BAA0C(const JGeometry::TBox2f* box);
    bool fn_802BACE4();

    /* 0x230 */ JPABaseEmitter* mEmitter230;
    /* 0x234 */ JPABaseEmitter* mEmitter234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ JGeometry::TVec3f field_0x23c;
    /* 0x248 */ JGeometry::TVec3f field_0x248;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ s32 field_0x284;
    /* 0x288 */ s16 field_0x288;
    /* 0x28A */ u8 field_0x28a[2];
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ u8 field_0x290;
    /* 0x291 */ u8 field_0x291;
    /* 0x292 */ u8 field_0x292;
    /* 0x293 */ u8 field_0x293;
}; // Size: 0x294

// Spawned for 'TUBO'.
class Pot_804A9428 : public ActorBase_804A5770 {
public:
    Pot_804A9428() {}
    /* 0x08 */ virtual ~Pot_804A9428();
    /* 0x230 */ u8 field_0x230[0x28];
}; // Size: 0x258

// Spawned for 'GOL2'.
class cls_804A9C20 : public ActorBase_804A5770 {
public:
    cls_804A9C20() {}
    /* 0x08 */ virtual ~cls_804A9C20();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Code: game/tu_802E2990.cpp.
    void fn_802E2990(u8 value);
    /* 0x230 */ s32 field_0x230; // state; draw shows the four sprites in states 0-3
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238; // mode; 5 draws the shikimono.bti tile (the destructor skips the audio call)
    /* 0x23C */ s32 field_0x23c; // the player count
    /* 0x240 */ s32 field_0x240; // bits 12-16 of field_0x90
    /* 0x244 */ s32 field_0x244; // bit 7 of field_0x90
    /* 0x248 */ s32 field_0x248;
    // mPos at init. Plain floats: a TVec3f member would add construct calls to the inline
    // constructor the spawner (tu_801F9484) emits, which has none.
    /* 0x24C */ f32 field_0x24c[3];
    /* 0x258 */ JGeometry::TVec3f field_0x258[4];
    /* 0x288 */ s32 field_0x288[4]; // per sprite: 1 picks frame 0x25A, else 0x25B
    /* 0x298 */ s32 field_0x298[4];
    /* 0x2A8 */ s32 field_0x2a8[4];
    /* 0x2B8 */ u16 field_0x2b8[4]; // emitter ids of the second set
    /* 0x2C0 */ u8 field_0x2c0[4]; // per-sprite alpha
    /* 0x2C4 */ s32 field_0x2c4; // countdown
    /* 0x2C8 */ JPABaseEmitter* mEmitters2c8[4]; // released by the destructor
    /* 0x2D8 */ JPABaseEmitter* mEmitters2d8[4];
    /* 0x2E8 */ JPABaseEmitter* mEmitter2e8;
    /* 0x2EC */ JPABaseEmitter* mEmitter2ec;
    /* 0x2F0 */ u8 field_0x2f0[4];
    /* 0x2F4 */ u8 field_0x2f4;
    /* 0x2F5 */ u8 field_0x2f5[3];
}; // Size: 0x2F8

// Spawned for 'SWDL'. Code: game/act_player_sword_level_check_npc.cpp.
class ActorSoundObj_804578F4;
class PlayerSwordLevelCheckNPC_804AA590 : public ActorBase_804A5770 {
public:
    PlayerSwordLevelCheckNPC_804AA590() {}
    /* 0x08 */ virtual ~PlayerSwordLevelCheckNPC_804AA590();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    // 0x802F032C: empty.
    /* 0x20 */ virtual void draw();
    /* 0x230 */ ActorSoundObj_804578F4* mSoundObj;
    /* 0x234 */ JPABaseEmitter* mEmitters[4];
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s16 field_0x260;
    /* 0x262 */ s16 field_0x262;
    /* 0x264 */ s32 field_0x264[4];
}; // Size: 0x274

// Spawned for 'TWTG'.
class cls_804AAD60 : public ActorBase_804A5770 {
public:
    cls_804AAD60();
    /* 0x08 */ virtual ~cls_804AAD60();
    /* 0x230 */ u8 field_0x230[0x4C];
}; // Size: 0x27C

// Spawned for 'SBAL'. Code: game/tu_80334318.cpp.
class IronBallHoming_804AB9C0 : public ActorBase_804A5770 {
public:
    IronBallHoming_804AB9C0() {}
    /* 0x08 */ virtual ~IronBallHoming_804AB9C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // Slot 0x54 (an extra virtual of this class). Looks for a 'WARP' actor with
    // (param & 0xFF) == 7 and moves the ball there with fn_801F349C; returns its serial or -1.
    /* 0x54 */ virtual s32 vfunc_0x54();
    /* 0x230 */ s32 field_0x230; // state (0-7)
    /* 0x234 */ f32 field_0x234;
    /* 0x238 */ f32 field_0x238;
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ f32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250; // bit 0 of field_0x90: the big ball
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ JGeometry::TVec3f field_0x260; // the last player position seen (update)
    /* 0x26C */ s16 field_0x26c; // heading (sincosTable_ index)
    /* 0x26E */ u8 field_0x26e[2];
}; // Size: 0x270

#include "game/act/act_giant_bomb.h"

#include "game/act/act_bee_manager.h"

// Spawned for 'MRTA'.
class cls_804ACD70 : public ActorBase_804A5770 {
public:
    cls_804ACD70() {}
    /* 0x08 */ virtual ~cls_804ACD70();
    /* 0x230 */ u8 field_0x230[0x38];
    /* 0x268 */ Elem_801FFCDC field_0x268[4];
    /* 0x2B8 */ u8 field_0x2b8[0x2C];
}; // Size: 0x2E4

// Spawned for 'EMIK'.
class MaidenEnding_804AD5B8 : public cls_804A5958 {
public:
    MaidenEnding_804AD5B8() {}
    /* 0x08 */ virtual ~MaidenEnding_804AD5B8();
    /* 0x254 */ u8 field_0x254[0x28];
}; // Size: 0x27C

// Spawned for 'DCR2'.
class Navi54MaidenCrystalCutscene_804ADFE8;
class J2DPicture;
class cls_804AE088 : public cls_8049D140 {
public:
    cls_804AE088();
    /* 0x08 */ virtual ~cls_804AE088();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();
    /* 0x60 */ virtual void vfunc_0x60(s32 arg);

    void fn_8036AC58(s32 arg);
    void fn_8036AA60();
    void fn_8036A894();
    void fn_8036A65C();
    void fn_80369DDC();
    void fn_80368CD8();
    void fn_80368660();
    void fn_803683AC();
    void fn_80368198();

    /* 0x230 */ Navi54MaidenCrystalCutscene_804ADFE8* mNaviActor;
    /* 0x234 */ u8 field_0x234;
    /* 0x235 */ u8 field_0x235;
    /* 0x236 */ u8 field_0x236;
    /* 0x237 */ u8 field_0x237;
    /* 0x238 */ u8 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ u8 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ s32 field_0x294[5];
    /* 0x2A8 */ JGeometry::TVec3f field_0x2a8[6];
    /* 0x2F0 */ s32 field_0x2f0;
    /* 0x2F4 */ cls_804A9C20* field_0x2f4;
    /* 0x2F8 */ s32 field_0x2f8[6];
    /* 0x310 */ void* field_0x310;
    /* 0x314 */ J2DPicture* field_0x314;
    /* 0x318 */ SpriteAnim_801D267C field_0x318;
    /* 0x350 */ JPABaseEmitter* field_0x350;
    /* 0x354 */ JPABaseEmitter* field_0x354;
    /* 0x358 */ JPABaseEmitter* field_0x358;
    /* 0x35C */ JPABaseEmitter* field_0x35c[7];
    /* 0x378 */ JPABaseEmitter* field_0x378[3];
    /* 0x384 */ JPABaseEmitter* field_0x384[5];
    /* 0x398 */ JPABaseEmitter* field_0x398;
    /* 0x39C */ JPABaseEmitter* field_0x39c[6];
    /* 0x3B4 */ JPABaseEmitter* field_0x3b4[2];
    /* 0x3BC */ JPABaseEmitter* field_0x3bc[6];
    /* 0x3D4 */ JPABaseEmitter* field_0x3d4[6];
    /* 0x3EC */ JPABaseEmitter* field_0x3ec[5];
    /* 0x400 */ JPABaseEmitter* field_0x400[5];
}; // Size: 0x414

class cls_8031709C;

// Spawned for 'JIJI'.
class cls_804AEED0 : public ActorBase_804A5770 {
public:
    cls_804AEED0();
    /* 0x08 */ virtual ~cls_804AEED0();
    /* 0x0C */ virtual void init();   // fn_8037C788
    /* 0x14 */ virtual void update(); // fn_8037BC30
    /* 0x20 */ virtual void draw();   // fn_8037B7F8

    // Animation steppers (game/act_old_man.cpp): each counts field_0x234 down
    // and picks the next step's length and frame.
    void fn_8037B374(JGeometry::TVec3f pos, s32 arg);
    void fn_8037B3A8();
    void fn_8037B3E0();
    void fn_8037B488();
    void fn_8037B4D0();
    void fn_8037B554();
    void fn_8037B5B4();
    void fn_8037B5FC();
    void fn_8037B634();
    void fn_8037B67C();
    void fn_8037B6C4();
    void fn_8037B70C();
    void fn_8037B744();
    void fn_8037B78C();
    void fn_8037B7BC();
    void fn_8037B7EC();

    // No address (dead-stripped; init and update inline it): updates field_0x248
    // towards the nearest player (or player + 1) every 20 frames.
    void updateFacing(s32 target);

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ u8 field_0x270;
    /* 0x271 */ u8 field_0x271;
    /* 0x272 */ u8 field_0x272;
    /* 0x273 */ u8 field_0x273;
    /* 0x274 */ cls_8031709C* field_0x274;
    /* 0x278 */ JPABaseEmitter* field_0x278;
}; // Size: 0x27C

// ZOSH and ARSH ("ZOSH"/"ARSH", unwritten units): sub-classes of cls_804A48B0 that only add a
// destructor (the base destructors are inlined into it, as in cls_804A5418). Named after their
// vtables, 0x804A5810 and 0x804A58B4.
class cls_804A5810 : public cls_804A48B0 {
public:
    // The body names the base's constructor again, which builds and destroys a temporary.
    cls_804A5810() { cls_804A48B0(); }
    virtual ~cls_804A5810() {}
}; // Size: 0x2AC

class cls_804A58B4 : public cls_804A48B0 {
public:
    cls_804A58B4() { cls_804A48B0(); }
    virtual ~cls_804A58B4() {}
}; // Size: 0x2AC

#endif
