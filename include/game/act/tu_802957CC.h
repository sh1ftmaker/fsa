#ifndef GAME_ACT_TU_802957CC_H
#define GAME_ACT_TU_802957CC_H

// Code: game/tu_802957CC.cpp. The spawner (game/tu_801F9484.cpp, fn_802019BC)
// constructs it and stores this vtable, 0x804A7AE0.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A7AE0 (__vt__12cls_804A7AE0). Its size is
// 0x3F8: the spawner allocates 0x3F8 bytes for it.
class cls_804A7AE0 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~cls_804A7AE0();

    bool fn_802957CC(u8 dir);
    virtual void vfunc_0x24(); // fn_80295C40
    void fn_80295EE8();
    void fn_802961AC();
    void fn_802984B0();
    void fn_80298978();
    void fn_80298C18();
    void fn_80298FC4();
    void fn_80299374();
    void fn_80299720();
    void fn_80299AD0();
    void fn_80299FB0();
    void fn_8029A494();
    void fn_8029A974();
    void bakeTiles();
    virtual void init(); // fn_8029B2EC
    virtual void vfunc_0x10(); // fn_8029AE58
    virtual void draw(); // fn_802965BC

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ JGeometry::TVec3f field_0x238;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ u8 field_0x24c;
    /* 0x24D */ u8 field_0x24d[0x250 - 0x24D];
    /* 0x250 */ JGeometry::TVec3f field_0x250;
    /* 0x25C */ s32 mSerialsA[4];
    /* 0x26C */ s32 mSerialsB[4];
    /* 0x27C */ s32 mSerialsC[4];
    /* 0x28C */ s32 mSerialsD[4];
    /* 0x29C */ s32 mSerialCountA;
    /* 0x2A0 */ s32 mSerialCountB;
    /* 0x2A4 */ s32 mSerialCountC;
    /* 0x2A8 */ s32 mSerialCountD;
    /* 0x2AC */ s32 field_0x2ac;
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ u8 field_0x2b4;
    /* 0x2B5 */ u8 field_0x2b5;
    /* 0x2B6 */ u8 field_0x2b6;
    /* 0x2B7 */ u8 field_0x2b7;
    /* 0x2B8 */ u8 field_0x2b8;
    /* 0x2B9 */ u8 field_0x2b9;
    /* 0x2BA */ u8 field_0x2ba;
    /* 0x2BB */ u8 field_0x2bb;
    /* 0x2BC */ u8 mPlayerFlags[4];
    /* 0x2C0 */ u8 field_0x2c0;
    /* 0x2C1 */ u8 field_0x2c1[3];
    /* 0x2C4 */ s32 mTimer;
    /* 0x2C8 */ s32 mTimerMax;
    /* 0x2CC */ s32 field_0x2cc;
    /* 0x2D0 */ JGeometry::TVec3f field_0x2d0;
    /* 0x2DC */ JGeometry::TVec2f field_0x2dc[4];
    /* 0x2FC */ JPABaseEmitter* mEmitters[2][2]; // released by the destructor
    /* 0x30C */ s32 field_0x30c; // not yet known
    /* 0x310 */ JPABaseEmitter* mEmitters2[4]; // released by the destructor
    /* 0x320 */ u16 mTilesA[6][6];
    /* 0x368 */ u16 mTilesB[6][6];
    /* 0x3B0 */ u16 mTiles[6][6];
}; // Size: 0x3F8

#endif
