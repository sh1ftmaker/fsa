#ifndef GAME_ACT_ACT_SPREADING_FIRE_H
#define GAME_ACT_ACT_SPREADING_FIRE_H

// Code: game/act_spreading_fire.cpp. Actor "Spreading fire" (BLZE in
// docs/names/actors.tsv, med confidence). Named after its vtable (0x804A8A50,
// the spawner stores it in game/tu_801F9484.cpp). Its vtable adds one slot
// (0x54) to ActorBase_804A5770's 0x54 bytes; it releases its four emitters
// and is itself released by vfunc_0x24.

#include "game/ActorBase.h"

class SpreadingFire_804A8A50 : public ActorBase_804A5770 {
public:
    SpreadingFire_804A8A50() {}
    virtual ~SpreadingFire_804A8A50();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    // Releases the four emitters (mEmitters) and clears the array.
    /* 0x54 */ virtual void vfunc_0x54();

    /* 0x230 */ JPABaseEmitter* mEmitters[4];
    /* 0x240 */ s32 field_0x240; // state: 0-7, 6 draws the sprite instead of the fire
    /* 0x244 */ u8 field_0x244[0x250 - 0x244];
    /* 0x250 */ s32 field_0x250; // bits 22-25 of field_0x90
    /* 0x254 */ s32 field_0x254; // bits 16-19 of field_0x90, doubled (30 when 0)
    /* 0x258 */ s32 field_0x258; // bits 28-31 of field_0x90 (15 -> -1)
    /* 0x25C */ JGeometry::TBox2f field_0x25c; // the view layer's box (view 0xCC / 0xDC)
    /* 0x26C */ s32 mViewBit;    // GameView bit index, cleared when the actor goes
    /* 0x270 */ s32 field_0x270; // mPos.x - box.i.x, clamped to 0..0x1FF
    /* 0x274 */ s32 field_0x274; // mPos.y - box.i.y, clamped to 0..0x1FF
    /* 0x278 */ u8 field_0x278;
    /* 0x279 */ u8 field_0x279[0x27C - 0x279];
    /* 0x27C */ s32 field_0x27c; // 0-7, random
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ JGeometry::TVec3f field_0x284; // the spot to return to (y: the sweep's end)
    /* 0x290 */ u8 field_0x290;
    /* 0x291 */ u8 field_0x291[0x294 - 0x291];
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ s32 field_0x298; // frames left in state 1, 8 at the start
    /* 0x29C */ u16 mTile;       // tile code from fn_80226850 at mPos
}; // Size: 0x2A0 at least

#endif
