#ifndef GAME_ACT_ACT_SHOOTING_GALLERY_H
#define GAME_ACT_ACT_SHOOTING_GALLERY_H

// Code: game/act_shooting_gallery.cpp.

#include "game/ActorBase.h"

// The SYKY actor ("Shooting Gallery", docs/names/actors.tsv, high confidence):
// spawned by fn_801F9484 (ctor 0x80201450, vtable 0x804A2B18).
class ShootingGallery_804A2B18 : public ActorBase_804A5770 {
public:
    ShootingGallery_804A2B18() {}
    /* 0x08 */ virtual ~ShootingGallery_804A2B18();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();

    void fn_8019E480();
    void fn_8019E7A4();
    void fn_8019EC04();
    void fn_8019EE74();
    void fn_8019F3AC();

    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 mTimers[4];
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c[4];
    /* 0x27C */ s32 field_0x27c[4];
    /* 0x28C */ u8 field_0x28c;
    /* 0x28D */ u8 field_0x28d[3];
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ f32 field_0x294;
    /* 0x298 */ f32 field_0x298;
    /* 0x29C */ f32 field_0x29c;
    /* 0x2A0 */ f32 field_0x2a0;
    /* 0x2A4 */ f32 field_0x2a4;
    /* 0x2A8 */ f32 field_0x2a8;
    /* 0x2AC */ s32 field_0x2ac;
}; // Size: 0x2B0

#endif
