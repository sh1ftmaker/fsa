#ifndef GAME_ACT_TU_801E801C_H
#define GAME_ACT_TU_801E801C_H

// Code: game/tu_801E801C.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5568.
class cls_804A5568 : public EnemyBase_804A4808 {
public:
    cls_804A5568() {}
    /* 0x08 */ virtual ~cls_804A5568();
    /* 0x0C */ virtual void init();
    /* 0x5C */ virtual bool vfunc_0x5C();
    /* 0x60 */ virtual bool vfunc_0x60();
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
    /* 0x2A8 */ u8 field_0x2a8[0x118]; // not yet known
    /* 0x3C0 */ JPABaseEmitter* mEmitter3c0; // released by the destructor
}; // Size: 0x3C4

#endif
