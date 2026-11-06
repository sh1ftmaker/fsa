#ifndef GAME_ACT_ACT_WALLMASTER_H
#define GAME_ACT_ACT_WALLMASTER_H

// Code: game/act_wallmaster.cpp.

#include "game/EnemyBase.h"

struct WallmasterParam {
    /* 0x00 */ s32 mType;
    /* 0x04 */ s32 mOrder[3];
    /* 0x10 */ s32 field_0x10[3];
    /* 0x1C */ s32 mIndex;
    /* 0x20 */ s32 mTarget;
    /* 0x24 */ s32 field_0x24;

    void fn_801BCD70(s32 arg);
}; // Size: 0x28

// The FALL actor ("Wallmaster", docs/names/actors.tsv, high confidence):
// spawned by fn_801F9484 (ctor 0x801BCCF0, vtable 0x804A3E44).
class cls_804A3E44 : public EnemyBase_804A4808 {
public:
    cls_804A3E44();
    /* 0x08 */ virtual ~cls_804A3E44();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x60 */ virtual bool vfunc_0x60();
    /* 0x78 */ virtual void vfunc_0x78();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0xA0 */ virtual void vfunc_0xA0();

    void fn_801BC244();
    void fn_801BC554(s32 state);
    u8 computeShadowAlpha();
    bool checkPlayerHit();

    /* 0x2A8 */ s32 field_0x2a8;
    /* 0x2AC */ s32 field_0x2ac;
    /* 0x2B0 */ JGeometry::TVec2f field_0x2b0;
    /* 0x2B8 */ s32 field_0x2b8;
    /* 0x2BC */ s32 field_0x2bc;
    /* 0x2C0 */ s32 field_0x2c0;
    /* 0x2C4 */ s32 field_0x2c4;
    /* 0x2C8 */ WallmasterParam mParam;
}; // Size: 0x2F0

#endif
