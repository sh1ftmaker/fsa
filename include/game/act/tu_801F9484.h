#ifndef GAME_ACT_TU_801F9484_H
#define GAME_ACT_TU_801F9484_H

// Actor classes the spawner (game/tu_801F9484.cpp) constructs whose own units are not written
// yet: what their inline constructors show (vtable store, member initialisers, size from
// __nw__), named after their vtables. When a class's unit is written, move it to that
// unit's act header.

#include "game/ActorBase.h"
#include "game/act/act_arrow.h"
#include "game/act/act_flame.h"
#include "game/act/act_wallmaster.h"
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
    /* 0x230 */ u8 field_0x230[0x38];
}; // Size: 0x268

// Spawned for 'SIBT'.
class TempleBTIDisplay_8049CB04 : public ActorBase_804A5770 {
public:
    TempleBTIDisplay_8049CB04() {}
    /* 0x08 */ virtual ~TempleBTIDisplay_8049CB04();
    /* 0x230 */ u8 field_0x230[0x18];
}; // Size: 0x248

#include "game/act/act_princess_zelda.h"

// Spawned for 'SNBL'.
class cls_8049D020 : public ActorBase_804A5770 {
public:
    cls_8049D020();
    /* 0x08 */ virtual ~cls_8049D020();
    /* 0x230 */ u8 field_0x230[0x150];
}; // Size: 0x380

// Spawned for 'DMCR'.
class cls_8049D83C : public ActorBase_804A5770 {
public:
    cls_8049D83C();
    /* 0x08 */ virtual ~cls_8049D83C();
    /* 0x230 */ u8 field_0x230[0xC0];
}; // Size: 0x2F0

// Spawned for 'DMGA'.
class cls_8049DD68 : public cls_8049D140 {
public:
    cls_8049DD68() : field_0x230(0), field_0x234(0), field_0x238(0.0f), field_0x23c(0.0f) {}
    /* 0x08 */ virtual ~cls_8049DD68();
    /* 0x230 */ u8 field_0x230;
    /* 0x231 */ u8 field_0x231[0x3];
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ f32 field_0x238;
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ u8 field_0x240[0x34];
    /* 0x274 */ SpriteAnim_801D267C field_0x274;
}; // Size: 0x2AC

// Spawned for 'RNBW'.
class cls_8049E610 : public ActorBase_804A5770 {
public:
    cls_8049E610();
    /* 0x08 */ virtual ~cls_8049E610();
    /* 0x230 */ u8 field_0x230[0x20];
}; // Size: 0x250

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
    /* 0x230 */ u8 field_0x230[0x18];
}; // Size: 0x248

// Spawned for 'CRNV'.
class cls_8049ED30 : public ActorBase_804A5770 {
public:
    cls_8049ED30() : field_0x230(0), field_0x234(0), field_0x238(-1), field_0x23c(0.0f), field_0x278(0), field_0x27c(0) {}
    /* 0x08 */ virtual ~cls_8049ED30();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ SpriteAnim_801D267C field_0x240;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
}; // Size: 0x280

// Spawned for 'DGTL'.
class cls_8049F6C8 : public ActorBase_804A5770 {
public:
    cls_8049F6C8() {}
    /* 0x08 */ virtual ~cls_8049F6C8();
    /* 0x230 */ u8 field_0x230[0x90];
    /* 0x2C0 */ JGeometry::TVec3f field_0x2c0[10];
    /* 0x338 */ u8 field_0x338[0xA0];
    /* 0x3D8 */ JGeometry::TVec3f field_0x3d8[80];
    /* 0x798 */ u8 field_0x798[0x30];
}; // Size: 0x7C8

// Spawned for 'HINO'.
class Hinox_8049F9B4 : public ActorBase_804A5770 {
public:
    Hinox_8049F9B4() {}
    /* 0x08 */ virtual ~Hinox_8049F9B4();
    /* 0x230 */ u8 field_0x230[0x58];
}; // Size: 0x288

// Spawned for 'SLR2'.
class cls_804A00C8 : public ActorBase_804A5770 {
public:
    cls_804A00C8() {}
    /* 0x08 */ virtual ~cls_804A00C8();
    /* 0x230 */ u8 field_0x230[0x94];
}; // Size: 0x2C4

// Spawned for 'BPOH'.
class cls_804A0440 : public ActorBase_804A5770 {
public:
    cls_804A0440() {}
    /* 0x08 */ virtual ~cls_804A0440();
    /* 0x230 */ u8 field_0x230[0x88];
    /* 0x2B8 */ JGeometry::TVec3f field_0x2b8[4];
    /* 0x2E8 */ u8 field_0x2e8[0xC];
    /* 0x2F4 */ JUtility::TColor field_0x2f4[3];
}; // Size: 0x300

// Spawned for 'SLR3'.
class cls_804A0810 : public ActorBase_804A5770 {
public:
    cls_804A0810() {}
    /* 0x08 */ virtual ~cls_804A0810();
    /* 0x230 */ u8 field_0x230[0x6C];
    /* 0x29C */ JGeometry::TVec3f field_0x29c[10];
    /* 0x314 */ u8 field_0x314[0x24];
}; // Size: 0x338

// Spawned for 'WIZR'.
class Wizzrobe_804A0EDC : public ActorBase_804A5770 {
public:
    Wizzrobe_804A0EDC() {}
    /* 0x08 */ virtual ~Wizzrobe_804A0EDC();
    /* 0x230 */ u8 field_0x230[0xDC];
}; // Size: 0x30C

// Spawned for 'FGAN'.
class cls_804A1318 : public ActorBase_804A5770 {
public:
    cls_804A1318() {}
    /* 0x08 */ virtual ~cls_804A1318();
    /* 0x24 */ virtual void vfunc_0x24(); // fn_8017C01C
    /* 0x230 */ u8 field_0x230[0x8A];
    /* 0x2BA */ s8 field_0x2ba;
    /* 0x2BB */ u8 field_0x2bb[0x25];
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
    /* 0x230 */ u8 field_0x230[0x74];
    /* 0x2A4 */ JGeometry::TVec3f field_0x2a4[4];
    /* 0x2D4 */ JGeometry::TVec3f field_0x2d4[4];
    // SHARED-EDIT: thirteen emitters, each freed by the destructor (game/tu_80197070.cpp).
    /* 0x304 */ JPABaseEmitter* mEmitters[13];
}; // Size: 0x338

// Spawned for 'ALCE'.
class AllEnemiesDefeated_804A2AB0 : public ActorBase_804A5770 {
public:
    AllEnemiesDefeated_804A2AB0() {}
    /* 0x08 */ virtual ~AllEnemiesDefeated_804A2AB0();
    /* 0x230 */ u8 field_0x230[0x38];
}; // Size: 0x268

// Spawned for 'SYKY'.
class ShootingGallery_804A2B18 : public ActorBase_804A5770 {
public:
    ShootingGallery_804A2B18() {}
    /* 0x08 */ virtual ~ShootingGallery_804A2B18();
    /* 0x230 */ u8 field_0x230[0x80];
}; // Size: 0x2B0

// Spawned for 'TENN'.
class TennisShop_804A2BF4 : public ActorBase_804A5770 {
public:
    TennisShop_804A2BF4() {}
    /* 0x08 */ virtual ~TennisShop_804A2BF4();
    /* 0x230 */ u8 field_0x230[0x124];
}; // Size: 0x354

// Spawned for 'SAYA'.
class ZoraFishHerding_804A2E30 : public ActorBase_804A5770 {
public:
    ZoraFishHerding_804A2E30() {}
    /* 0x08 */ virtual ~ZoraFishHerding_804A2E30();
    /* 0x230 */ u8 field_0x230[0x44];
}; // Size: 0x274

// Spawned for 'GOMA'.
class Gohma_804A307C : public ActorBase_804A5770 {
public:
    Gohma_804A307C() {}
    /* 0x08 */ virtual ~Gohma_804A307C();
    /* 0x230 */ u8 field_0x230[0x54];
}; // Size: 0x284

// Spawned for 'ZLD3'.
class cls_804A35AC : public ActorBase_804A5770 {
public:
    cls_804A35AC() {}
    /* 0x08 */ virtual ~cls_804A35AC();
    /* 0x230 */ u8 field_0x230[0x58];
    /* 0x288 */ JGeometry::TVec3f field_0x288[4];
    /* 0x2B8 */ u8 field_0x2b8[0x8];
}; // Size: 0x2C0

// Spawned for 'ZLD4'.
class cls_804A3698 : public ActorBase_804A5770 {
public:
    cls_804A3698() {}
    /* 0x08 */ virtual ~cls_804A3698();
    /* 0x230 */ u8 field_0x230[0x114];
}; // Size: 0x344

// Spawned for 'TLWP'.
class cls_804A38D4 : public ActorBase_804A5770 {
public:
    cls_804A38D4() {}
    /* 0x08 */ virtual ~cls_804A38D4();
    /* 0x230 */ u8 field_0x230[0x34];
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ u8 field_0x294[0x5C];
}; // Size: 0x2F0

#include "game/act/act_bird_manager.h"


// Spawned for 'ZSLD'.
class cls_804A6D00 : public ActorBase_804A5770 {
public:
    cls_804A6D00();
    /* 0x08 */ virtual ~cls_804A6D00();
    /* 0x230 */ u8 field_0x230[0x94];
}; // Size: 0x2C4

// Spawned for 'GOAL'.
class cls_804A7EB0 : public ActorBase_804A5770 {
public:
    cls_804A7EB0() {}
    /* 0x08 */ virtual ~cls_804A7EB0();
    /* 0x230 */ u8 field_0x230[0x34];
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ u8 field_0x294[0x8C];
}; // Size: 0x320

// Spawned for 'SWTH'.
class Switch_804A7F38 : public ActorBase_804A5770 {
public:
    Switch_804A7F38() {}
    /* 0x08 */ virtual ~Switch_804A7F38();
    /* 0x230 */ u8 field_0x230[0x38];
}; // Size: 0x268

// Spawned for 'FIRE'.
class FireRodFire_804A89F0 : public ActorBase_804A5770 {
public:
    FireRodFire_804A89F0() {}
    /* 0x08 */ virtual ~FireRodFire_804A89F0();
    // 0x802BA9F0 / 0x802BA9F8 (game/act_fire_rod_fire.cpp, not written yet):
    // return the words at 0x260 and 0x264. act_ice_melted_by_four_players
    // calls both on FIRE actors it finds.
    s32 fn_802BA9F0();
    s32 fn_802BA9F8();
    // 0x802BAA0C (game/act_fire_rod_fire.cpp, not written yet): centres the fire on
    // `box` and sets its boxes from it; the player's state 0x45 (fn_80259830, tu_80256C74)
    // calls it each frame with the box in front of the player.
    void fn_802BAA0C(const JGeometry::TBox2f* box);
    /* 0x230 */ u8 field_0x230[0x64];
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
    /* 0x230 */ u8 field_0x230[0x28];
    /* 0x258 */ JGeometry::TVec3f field_0x258[4];
    /* 0x288 */ u8 field_0x288[0x70];
}; // Size: 0x2F8

// Spawned for 'SWDL'.
class PlayerSwordLevelCheckNPC_804AA590 : public ActorBase_804A5770 {
public:
    PlayerSwordLevelCheckNPC_804AA590() {}
    /* 0x08 */ virtual ~PlayerSwordLevelCheckNPC_804AA590();
    /* 0x230 */ u8 field_0x230[0x44];
}; // Size: 0x274

// Spawned for 'TWTG'.
class cls_804AAD60 : public ActorBase_804A5770 {
public:
    cls_804AAD60();
    /* 0x08 */ virtual ~cls_804AAD60();
    /* 0x230 */ u8 field_0x230[0x4C];
}; // Size: 0x27C

// Spawned for 'SBAL'.
class IronBallHoming_804AB9C0 : public ActorBase_804A5770 {
public:
    IronBallHoming_804AB9C0() {}
    /* 0x08 */ virtual ~IronBallHoming_804AB9C0();
    /* 0x230 */ u8 field_0x230[0x40];
}; // Size: 0x270

// Spawned for 'BBMB'.
class GiantBomb_804AC588 : public ActorBase_804A5770 {
public:
    GiantBomb_804AC588() {}
    /* 0x08 */ virtual ~GiantBomb_804AC588();
    /* 0x230 */ u8 field_0x230[0x3C];
}; // Size: 0x26C

// Spawned for 'BEMN'.
class BeeManager_804AC810 : public ActorBase_804A5770 {
public:
    BeeManager_804AC810() {}
    /* 0x08 */ virtual ~BeeManager_804AC810();
    /* 0x230 */ u8 field_0x230[0x54];
}; // Size: 0x284

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
class cls_804AE088 : public ActorBase_804A5770 {
public:
    cls_804AE088();
    /* 0x08 */ virtual ~cls_804AE088();
    /* 0x230 */ u8 field_0x230[0x1E4];
}; // Size: 0x414

// Spawned for 'ELK2'.
class Link2Ending_804AE338 : public ActorBase_804A5770 {
public:
    Link2Ending_804AE338() {}
    /* 0x08 */ virtual ~Link2Ending_804AE338();
    /* 0x230 */ u8 field_0x230[0x88];
}; // Size: 0x2B8

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

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ u8 field_0x240[8];
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ u8 field_0x24c[0x24];
    /* 0x270 */ u8 field_0x270[3];
    /* 0x273 */ u8 field_0x273;
    /* 0x274 */ cls_8031709C* field_0x274;
    /* 0x278 */ JPABaseEmitter* field_0x278;
}; // Size: 0x27C

// Spawned for 'BFRY'.
class GreatFairy_804AF338 : public ActorBase_804A5770 {
public:
    GreatFairy_804AF338() {}
    /* 0x08 */ virtual ~GreatFairy_804AF338();
    /* 0x230 */ u8 field_0x230[0x78];
}; // Size: 0x2A8

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
