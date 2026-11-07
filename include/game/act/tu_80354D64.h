#ifndef GAME_ACT_TU_80354D64_H
#define GAME_ACT_TU_80354D64_H

// Code: game/tu_80354D64.cpp (vtable, destructor fn_80356514 and the members below
// are written at any %, see the FIXME(shotgun) lines).

#include <float.h>

#include "game/ActorBase.h"

// The MRTA actor ("Log", docs/names/actors.tsv, med confidence): the
// spawner (spawnActor_801F9484) allocates 0x2E4 bytes for it and constructs it with
// fn_801FFC84, which stores the vtable at 0x804ACD70 (lbl_804ACD70; there
// is no symbol for it yet, so the class is named after the address). Only
// the members act_npc_ride_switch reads are declared.
struct LogSlot_804ACD70 {
    /* 0x00 */ u8 field_0x0;
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ s32 field_0x10;
}; // Size: 0x14

class Log_804ACD70 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~Log_804ACD70(); // fn_80356514
    virtual void init();                                              // 0x0C: fn_80356268
    virtual void update();                                            // 0x14: fn_803558A4
    virtual void draw();                                              // 0x20: fn_80355610
    virtual void vfunc_0x24();                                        // fn_803554B8
    virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos); // fn_8035533C
    bool fn_80354D64(u8 dir);
    void fn_80354FE8();
    bool isAtRest() const {
        if (field_0x230 == 0 && isMotionless() == true) {
            return true;
        }
        return false;
    }
    // True while field_0x2d4 is (nearly) zero.
    bool isMotionless() const { return field_0x2d4.squared() <= 32.0f * FLT_EPSILON; }
    /* 0x230 */ s32 field_0x230; // 0 while the log is at rest (npc_ride_switch)
    /* 0x234 */ s32 field_0x234[4]; // -1 after init
    /* 0x244 */ s32 field_0x244[4]; // 0 after init
    /* 0x254 */ s32 field_0x254[4]; // -1 after init
    /* 0x264 */ u8 field_0x264[4];  // 0 after init
    /* 0x268 */ LogSlot_804ACD70 field_0x268[4]; // plain scalars: init only stores them
    /* 0x2B8 */ s32 field_0x2b8; // the timer fn_8035533C hands to fn_801F132C
    /* 0x2BC */ s32 field_0x2bc;
    /* 0x2C0 */ s32 field_0x2c0; // bit 0 of field_0x90 (axis)
    /* 0x2C4 */ s32 field_0x2c4; // bit 4 of field_0x90 (size)
    /* 0x2C8 */ s32 field_0x2c8; // bit 8 of field_0x90 (0 log, 1 stump)
    /* 0x2CC */ u8 field_0x2cc;
    /* 0x2D0 */ s32 field_0x2d0; // -1 after init
    /* 0x2D4 */ JGeometry::TVec3f field_0x2d4; // velocity? (zero while at rest)
    /* 0x2E0 */ JPABaseEmitter* mEmitter2e0; // released by the destructor
}; // Size: 0x2E4

#endif
