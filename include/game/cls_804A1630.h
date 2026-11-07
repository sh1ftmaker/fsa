#ifndef GAME_CLS_804A1630_H
#define GAME_CLS_804A1630_H

#include "game/ActorBase.h"

// An ActorBase-derived actor whose vtable is lbl_804A1630 (0x54 bytes, the 21
// base slots). Its constructor (fn_8020231C, not written yet) stores the vtable
// and builds its members; named after the vtable, like the other unnamed ones.
// Its destructor (game/tu_8017EF40.cpp) releases the two emitters at 0x318 and
// 0x31C and then runs the base destructor.
class cls_804A1630 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~cls_804A1630();
    // 0x8017EF40: starts sound 0x40038, then vfunc_0x28(2, 0.0f, -24.0f).
    /* 0x24 */ virtual void vfunc_0x24();

    /* 0x0C */ virtual void init();   // fn_801820B4
    /* 0x14 */ virtual void update(); // fn_8017FB10, not written yet
    /* 0x20 */ virtual void draw();   // 0x8017EF98
    // 0x80181BC8: per player on this layer, draws the marker (fn_80230498) at its stored
    // position; mode 0 stores the players' positions, mode 1 steps the animation.
    void fn_80181BC8(s32 row, s32 mode);

    // SHARED-EDIT: carved from field_0x230[0x64] / field_0x2f4[0x24] (game/tu_8017EF40.cpp).
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
    /* 0x278 */ s16 field_0x278;
    /* 0x27A */ u8 field_0x27a[2];
    /* 0x27C */ JGeometry::TVec3f field_0x27c;
    /* 0x288 */ JGeometry::TVec3f field_0x288;
    /* 0x294 */ JGeometry::TVec3f field_0x294[4];
    /* 0x2C4 */ JGeometry::TVec3f field_0x2c4[4];
    /* 0x2F4 */ s32 field_0x2f4[4];
    /* 0x304 */ u8 field_0x304[4];
    /* 0x308 */ f32 field_0x308;
    /* 0x30C */ f32 field_0x30c;
    /* 0x310 */ f32 field_0x310;
    /* 0x314 */ f32 field_0x314;
    /* 0x318 */ JPABaseEmitter* mEmitter318[2];
}; // Size: 0x320

#endif
