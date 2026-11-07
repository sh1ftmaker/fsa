#ifndef GAME_ACT_TU_801E801C_H
#define GAME_ACT_TU_801E801C_H

// Code: game/tu_801E801C.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5568.
class cls_804A5568 : public EnemyBase_804A4808 {
public:
    cls_804A5568();
    /* 0x08 */ virtual ~cls_804A5568();
    /* 0x0C */ virtual void init();
    /* 0x5C */ virtual u8 vfunc_0x5C();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x68 */ virtual u8 vfunc_0x68();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0xA0 */ virtual void vfunc_0xA0();
    // Not written yet (game/tu_801E801C.cpp); called from game/tu_801E9AFC.cpp.
    // 0x801E801C: draws the sprite (fn_801EA108) unless field_0x248 is set.
    void fn_801E801C();
    // 0x801E8080
    void fn_801E8080();
    // 0x801E8C74: stores arg at 0x3B8 and counts the low bits set in 0x2A8.
    void fn_801E8C74(s32 arg);
    // 0x801E8EB4: sets bit `index` of the word at 0x2A8 and stores pos's x / y
    // at 0x2B0 + 8 * index; arg is unused.
    void fn_801E8EB4(u8 index, s32 arg, const JGeometry::TVec2f* pos);
    // Shotgun pass: fn_801E9224 is the chain's per-frame step (not written yet).
    void fn_801E9224();

    /* 0x2A8 */ u32 mMask; // bit i: link i is set (fn_801E8EB4)
    /* 0x2AC */ u8 field_0x2ac;
    /* 0x2AD */ u8 mCount; // the run of low bits set in mMask (fn_801E8C74)
    /* 0x2B0 */ JGeometry::TVec2f field_0x2b0[32]; // link positions (8 groups of 4 in the original's construct_array)
    /* 0x3B0 */ u8 field_0x3b0;
    /* 0x3B4 */ f32 field_0x3b4;
    /* 0x3B8 */ s32 field_0x3b8;
    /* 0x3BC */ s16 field_0x3bc;
    /* 0x3C0 */ JPABaseEmitter* mEmitter3c0; // released by the destructor
}; // Size: 0x3C4

#endif
