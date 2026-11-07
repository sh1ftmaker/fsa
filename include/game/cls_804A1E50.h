#ifndef GAME_CLS_804A1E50_H
#define GAME_CLS_804A1E50_H

// Code: game/tu_8018A8D8.cpp.

#include "game/ActorBase.h"

// An actor with 0x334 bytes (the spawner allocates it at 0x801FD0AC and
// constructs it with fn_80201E28, which stores this vtable and constructs the
// four TVec3f at 0x2F8). Named after its vtable, 0x804A1E50; its unit's
// resources are boss_p_rantan.bti and enemy_both2_sob.bin. Only what the
// functions written so far touch is declared.
class cls_804A1E50 : public ActorBase_804A5770 {
public:
    cls_804A1E50() {}
    /* 0x08 */ virtual ~cls_804A1E50();
    /* 0x0C */ virtual void init();   // fn_8018E5E4, not written yet
    /* 0x14 */ virtual void update(); // fn_8018AFFC, not written yet
    /* 0x20 */ virtual void draw();   // fn_8018A930
    /* 0x24 */ virtual void vfunc_0x24();

    void fn_8018E894(s32 value);
    void fn_8018E30C(s32 anim);
    void fn_8018DFCC();
    bool fn_8018DA6C();
    void fn_8018DC70(s32 side);

    // SHARED-EDIT: carved from field_0x230[] / field_0x270[] (game/tu_8018A8D8.cpp).
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234; // frame countdown, reloaded from lbl_804A1B70
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
    /* 0x264 */ s32 field_0x264; // step within the animation
    /* 0x268 */ s32 field_0x268; // animation
    /* 0x26C */ s32 field_0x26c; // tested by the destructor and fn_8018AFFC
    /* 0x270 */ s32 field_0x270; // first entry of the animation in lbl_804A1B70
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s16 field_0x278;
    /* 0x27A */ u8 field_0x27a;
    /* 0x27B */ u8 field_0x27b;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ f32 field_0x294;
    /* 0x298 */ f32 field_0x298;
    /* 0x29C */ u8 field_0x29c;
    /* 0x29D */ u8 field_0x29d[3];
    /* 0x2A0 */ JGeometry::TVec3f field_0x2a0;
    /* 0x2AC */ f32 field_0x2ac;
    /* 0x2B0 */ f32 field_0x2b0;
    /* 0x2B4 */ f32 field_0x2b4;
    /* 0x2B8 */ f32 field_0x2b8;
    /* 0x2BC */ f32 field_0x2bc;
    /* 0x2C0 */ f32 field_0x2c0;
    /* 0x2C4 */ JGeometry::TVec3f field_0x2c4[3];
    /* 0x2E8 */ s32 field_0x2e8[4];
    /* 0x2F8 */ JGeometry::TVec3f field_0x2f8[4];
    // Emitters (spawnEmitter_8013CC50), released by the destructor with releaseEmitter_801EE434.
    /* 0x328 */ JPABaseEmitter* field_0x328;
    /* 0x32C */ JPABaseEmitter* field_0x32c;
    /* 0x330 */ JPABaseEmitter* field_0x330;
}; // Size: 0x334

#endif
