#ifndef GAME_ACT_TU_8036FB60_H
#define GAME_ACT_TU_8036FB60_H

// Code: game/tu_8036FB60.cpp.

#include "game/ActorBase.h"

class cls_804AEED0;

// Named after its vtable, 0x804AE4B0.
class cls_804AE4B0 : public ActorBase_804A5770 {
public:
    cls_804AE4B0() {}
    /* 0x08 */ virtual ~cls_804AE4B0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Shotgun pass: the state handlers (table lbl_804AE420 in update) and helpers,
    // named by address; fields from the asm only.
    void fn_8036FB60();
    void fn_8036FDAC();
    void fn_80370020();
    s32 fn_80370200();
    void fn_803703C8();
    void fn_803704E8();
    void fn_80370508();
    void fn_803705A0();
    void fn_803706AC();
    void fn_80370764();
    void fn_80370A7C();
    void fn_80370C18();
    void fn_80370C84();
    void fn_8037100C();
    void fn_803711F0();
    void fn_80371284();
    void fn_803717CC();
    void fn_80371A2C();
    void fn_80371D50(s32 arg);
    void fn_803721F4();

    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 mTimer;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ f32 field_0x244;
    /* 0x248 */ f32 field_0x248;
    /* 0x24C */ f32 field_0x24c;
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ JGeometry::TVec3f field_0x294[4];
    /* 0x2C4 */ JGeometry::TVec3f field_0x2c4[4];
    /* 0x2F4 */ JGeometry::TVec3f field_0x2f4[4];
    /* 0x324 */ JGeometry::TVec3f field_0x324;
    /* 0x330 */ s32 field_0x330[4];
    /* 0x340 */ s32 field_0x340[4];
    /* 0x350 */ s32 field_0x350[4];
    /* 0x360 */ s32 field_0x360;
    /* 0x364 */ s32 field_0x364;
    /* 0x368 */ s32 field_0x368;
    /* 0x36C */ s32 field_0x36c;
    /* 0x370 */ s32 field_0x370;
    /* 0x374 */ s32 field_0x374[4];
    /* 0x384 */ s32 field_0x384[4];
    /* 0x394 */ s32 field_0x394;
    /* 0x398 */ u8 field_0x398;
    /* 0x399 */ u8 field_0x399;
    /* 0x39C */ s32 field_0x39c;
    /* 0x3A0 */ s32 field_0x3a0;
    /* 0x3A4 */ s32 field_0x3a4;
    /* 0x3A8 */ u8 field_0x3a8;
    /* 0x3AC */ s32 field_0x3ac;
    /* 0x3B0 */ cls_804AEED0* field_0x3b0[4];
}; // Size: 0x3C0

#endif
