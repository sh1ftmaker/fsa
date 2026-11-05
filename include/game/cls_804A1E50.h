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
    /* 0x20 */ virtual void draw();   // fn_8018A930, not written yet
    /* 0x24 */ virtual void vfunc_0x24();

    /* 0x230 */ u8 field_0x230[0x26C - 0x230];
    /* 0x26C */ s32 field_0x26c; // tested by the destructor and fn_8018AFFC
    /* 0x270 */ u8 field_0x270[0x2F8 - 0x270];
    /* 0x2F8 */ JGeometry::TVec3f field_0x2f8[4];
    // Emitters (spawnEmitter_8013CC50), released by the destructor with fn_801EE434.
    /* 0x328 */ JPABaseEmitter* field_0x328;
    /* 0x32C */ JPABaseEmitter* field_0x32c;
    /* 0x330 */ JPABaseEmitter* field_0x330;
}; // Size: 0x334

#endif
