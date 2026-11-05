#ifndef GAME_ACT_TU_801F9484_H
#define GAME_ACT_TU_801F9484_H

// Actor classes the spawner (game/tu_801F9484.cpp) constructs whose own units are not written
// yet: what their inline constructors show (vtable store, member initialisers, size from
// __nw__), named after their vtables. When a class's unit is written, move it to that
// unit's act header.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"
#include "game/act/tu_801F9484_elems.h"
#include "JSystem/JUtility/TColor.h"

// Spawned for 'LPLK'.
class cls_8049C578 : public ActorBase_804A5770 {
public:
    cls_8049C578() {}
    /* 0x08 */ virtual ~cls_8049C578();
    /* 0x230 */ u8 field_0x230[0x38];
}; // Size: 0x268

// Spawned for 'SIBT'.
class cls_8049CB04 : public ActorBase_804A5770 {
public:
    cls_8049CB04() {}
    /* 0x08 */ virtual ~cls_8049CB04();
    /* 0x230 */ u8 field_0x230[0x18];
}; // Size: 0x248

// Spawned for 'ZLDA'.
class cls_8049CEE0 : public ActorBase_804A5770 {
public:
    cls_8049CEE0();
    /* 0x08 */ virtual ~cls_8049CEE0();
    /* 0x230 */ u8 field_0x230[0x144];
}; // Size: 0x374

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
class cls_8049DD68 : public ActorBase_804A5770 {
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
class cls_8049F9B4 : public ActorBase_804A5770 {
public:
    cls_8049F9B4() {}
    /* 0x08 */ virtual ~cls_8049F9B4();
    /* 0x230 */ u8 field_0x230[0x58];
}; // Size: 0x288

// Spawned for 'SLR2'.
class cls_804A00C8 : public ActorBase_804A5770 {
public:
    cls_804A00C8() {}
    /* 0x08 */ virtual ~cls_804A00C8();
    /* 0x230 */ u8 field_0x230[0x94];
}; // Size: 0x2C4

// Spawned for 'FLME'.
class cls_804A0330 : public ActorBase_804A5770 {
public:
    cls_804A0330() {}
    /* 0x08 */ virtual ~cls_804A0330();
    /* 0x230 */ u8 field_0x230[0x44];
}; // Size: 0x274

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

// Spawned for 'ARRW'.
class cls_804A05E0 : public ActorBase_804A5770 {
public:
    cls_804A05E0() {}
    /* 0x08 */ virtual ~cls_804A05E0();
    /* 0x230 */ u8 field_0x230[0x50];
}; // Size: 0x280

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
class cls_804A0EDC : public ActorBase_804A5770 {
public:
    cls_804A0EDC() {}
    /* 0x08 */ virtual ~cls_804A0EDC();
    /* 0x230 */ u8 field_0x230[0xDC];
}; // Size: 0x30C

// Spawned for 'FGAN'.
class cls_804A1318 : public ActorBase_804A5770 {
public:
    cls_804A1318() {}
    /* 0x08 */ virtual ~cls_804A1318();
    /* 0x230 */ u8 field_0x230[0xB0];
}; // Size: 0x2E0

// Spawned for 'FGN2'.
class cls_804A1A98 : public ActorBase_804A5770 {
public:
    cls_804A1A98() {}
    /* 0x08 */ virtual ~cls_804A1A98();
    /* 0x230 */ u8 field_0x230[0xE8];
}; // Size: 0x318

// Spawned for 'BPH2'.
class cls_804A1E50 : public ActorBase_804A5770 {
public:
    cls_804A1E50() {}
    /* 0x08 */ virtual ~cls_804A1E50();
    /* 0x230 */ u8 field_0x230[0xC8];
    /* 0x2F8 */ JGeometry::TVec3f field_0x2f8[4];
    /* 0x328 */ u8 field_0x328[0xC];
}; // Size: 0x334

// Spawned for 'DGT2'.
class cls_804A1FA0 : public ActorBase_804A5770 {
public:
    cls_804A1FA0() {}
    /* 0x08 */ virtual ~cls_804A1FA0();
    /* 0x230 */ u8 field_0x230[0x58];
    /* 0x288 */ JGeometry::TVec3f field_0x288[6];
    /* 0x2D0 */ JGeometry::TVec3f field_0x2d0[20];
    /* 0x3C0 */ u8 field_0x3c0[0xC];
}; // Size: 0x3CC

// Spawned for 'GNON'.
class cls_804A25F0 : public ActorBase_804A5770 {
public:
    cls_804A25F0() {}
    /* 0x08 */ virtual ~cls_804A25F0();
    /* 0x230 */ u8 field_0x230[0x10C];
}; // Size: 0x33C

// Spawned for 'ZLD2'.
class cls_804A2944 : public ActorBase_804A5770 {
public:
    cls_804A2944() {}
    /* 0x08 */ virtual ~cls_804A2944();
    /* 0x230 */ u8 field_0x230[0x74];
    /* 0x2A4 */ JGeometry::TVec3f field_0x2a4[4];
    /* 0x2D4 */ JGeometry::TVec3f field_0x2d4[4];
    /* 0x304 */ u8 field_0x304[0x34];
}; // Size: 0x338

// Spawned for 'ALCE'.
class cls_804A2AB0 : public ActorBase_804A5770 {
public:
    cls_804A2AB0() {}
    /* 0x08 */ virtual ~cls_804A2AB0();
    /* 0x230 */ u8 field_0x230[0x38];
}; // Size: 0x268

// Spawned for 'SYKY'.
class cls_804A2B18 : public ActorBase_804A5770 {
public:
    cls_804A2B18() {}
    /* 0x08 */ virtual ~cls_804A2B18();
    /* 0x230 */ u8 field_0x230[0x80];
}; // Size: 0x2B0

// Spawned for 'TENN'.
class cls_804A2BF4 : public ActorBase_804A5770 {
public:
    cls_804A2BF4() {}
    /* 0x08 */ virtual ~cls_804A2BF4();
    /* 0x230 */ u8 field_0x230[0x124];
}; // Size: 0x354

// Spawned for 'SAYA'.
class cls_804A2E30 : public ActorBase_804A5770 {
public:
    cls_804A2E30() {}
    /* 0x08 */ virtual ~cls_804A2E30();
    /* 0x230 */ u8 field_0x230[0x44];
}; // Size: 0x274

// Spawned for 'GOMA'.
class cls_804A307C : public ActorBase_804A5770 {
public:
    cls_804A307C() {}
    /* 0x08 */ virtual ~cls_804A307C();
    /* 0x230 */ u8 field_0x230[0x54];
}; // Size: 0x284

// Spawned for 'DDGO'.
class cls_804A3334 : public ActorBase_804A5770 {
public:
    cls_804A3334() {}
    /* 0x08 */ virtual ~cls_804A3334();
    /* 0x230 */ u8 field_0x230[0x74];
}; // Size: 0x2A4

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

// Spawned for 'GLWP'.
class cls_804A39DC : public ActorBase_804A5770 {
public:
    cls_804A39DC() {}
    /* 0x08 */ virtual ~cls_804A39DC();
    /* 0x230 */ u8 field_0x230[0x34];
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ u8 field_0x294[0x10];
}; // Size: 0x2A4

// Spawned for 'FALL'.
class cls_804A3E44 : public ActorBase_804A5770 {
public:
    cls_804A3E44();
    /* 0x08 */ virtual ~cls_804A3E44();
    /* 0x230 */ u8 field_0x230[0xC0];
}; // Size: 0x2F0

// Spawned for 'BOIT'.
class cls_804A4F90 : public ActorBase_804A5770 {
public:
    cls_804A4F90();
    /* 0x08 */ virtual ~cls_804A4F90();
    /* 0x230 */ u8 field_0x230[0x7C];
}; // Size: 0x2AC

// Spawned for 'BRMN'.
class cls_804A5C88 : public ActorBase_804A5770 {
public:
    cls_804A5C88();
    /* 0x08 */ virtual ~cls_804A5C88();
    /* 0x230 */ u8 field_0x230[0x140];
}; // Size: 0x370

// Spawned for 'NAVI'.
class cls_804A6708 : public ActorBase_804A5770 {
public:
    cls_804A6708();
    /* 0x08 */ virtual ~cls_804A6708();
    /* 0x230 */ u8 field_0x230[0xA8];
}; // Size: 0x2D8

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
class cls_804A7F38 : public ActorBase_804A5770 {
public:
    cls_804A7F38() {}
    /* 0x08 */ virtual ~cls_804A7F38();
    /* 0x230 */ u8 field_0x230[0x38];
}; // Size: 0x268

// Spawned for 'FIRE'.
class cls_804A89F0 : public ActorBase_804A5770 {
public:
    cls_804A89F0() {}
    /* 0x08 */ virtual ~cls_804A89F0();
    /* 0x230 */ u8 field_0x230[0x64];
}; // Size: 0x294

// Spawned for 'TUBO'.
class cls_804A9428 : public ActorBase_804A5770 {
public:
    cls_804A9428() {}
    /* 0x08 */ virtual ~cls_804A9428();
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
class cls_804AA590 : public ActorBase_804A5770 {
public:
    cls_804AA590() {}
    /* 0x08 */ virtual ~cls_804AA590();
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
class cls_804AB9C0 : public ActorBase_804A5770 {
public:
    cls_804AB9C0() {}
    /* 0x08 */ virtual ~cls_804AB9C0();
    /* 0x230 */ u8 field_0x230[0x40];
}; // Size: 0x270

// Spawned for 'BBMB'.
class cls_804AC588 : public ActorBase_804A5770 {
public:
    cls_804AC588() {}
    /* 0x08 */ virtual ~cls_804AC588();
    /* 0x230 */ u8 field_0x230[0x3C];
}; // Size: 0x26C

// Spawned for 'BEMN'.
class cls_804AC810 : public ActorBase_804A5770 {
public:
    cls_804AC810() {}
    /* 0x08 */ virtual ~cls_804AC810();
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
class cls_804AD5B8 : public ActorBase_804A5770 {
public:
    cls_804AD5B8() {}
    /* 0x08 */ virtual ~cls_804AD5B8();
    /* 0x230 */ u8 field_0x230[0x4C];
}; // Size: 0x27C

// Spawned for 'EETC'.
class cls_804ADF28 : public ActorBase_804A5770 {
public:
    cls_804ADF28() {}
    /* 0x08 */ virtual ~cls_804ADF28();
    /* 0x230 */ u8 field_0x230[0x4C];
    /* 0x27C */ JGeometry::TVec3f field_0x27c[3];
    /* 0x2A0 */ JGeometry::TVec3f field_0x2a0[3];
    /* 0x2C4 */ JGeometry::TVec3f field_0x2c4[3];
    /* 0x2E8 */ u8 field_0x2e8[0x34];
}; // Size: 0x31C

// Spawned for 'DCR2'.
class cls_804AE088 : public ActorBase_804A5770 {
public:
    cls_804AE088();
    /* 0x08 */ virtual ~cls_804AE088();
    /* 0x230 */ u8 field_0x230[0x1E4];
}; // Size: 0x414

// Spawned for 'ELK2'.
class cls_804AE338 : public ActorBase_804A5770 {
public:
    cls_804AE338() {}
    /* 0x08 */ virtual ~cls_804AE338();
    /* 0x230 */ u8 field_0x230[0x88];
}; // Size: 0x2B8

// Spawned for 'JIJI'.
class cls_804AEED0 : public ActorBase_804A5770 {
public:
    cls_804AEED0();
    /* 0x08 */ virtual ~cls_804AEED0();
    /* 0x230 */ u8 field_0x230[0x4C];
}; // Size: 0x27C

// Spawned for 'BFRY'.
class cls_804AF338 : public ActorBase_804A5770 {
public:
    cls_804AF338() {}
    /* 0x08 */ virtual ~cls_804AF338();
    /* 0x230 */ u8 field_0x230[0x78];
}; // Size: 0x2A8

// Spawned for 'TNGL'.
class cls_804B0478 : public ActorBase_804A5770 {
public:
    cls_804B0478() {}
    /* 0x08 */ virtual ~cls_804B0478();
    /* 0x230 */ u8 field_0x230[0x30];
    /* 0x260 */ JGeometry::TVec3f field_0x260[4];
    /* 0x290 */ u8 field_0x290[0x10];
}; // Size: 0x2A0

#endif
