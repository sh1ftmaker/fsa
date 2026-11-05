#ifndef GAME_ACT_TU_80354D64_H
#define GAME_ACT_TU_80354D64_H

// Code: game/tu_80354D64.cpp (not written yet; it holds the vtable and
// destructor fn_80356514).

#include <float.h>

#include "game/ActorBase.h"

// The MRTA actor ("Log", docs/names/actors.tsv, med confidence): the
// spawner (spawnActor_801F9484) allocates 0x2E4 bytes for it and constructs it with
// fn_801FFC84, which stores the vtable at 0x804ACD70 (lbl_804ACD70; there
// is no symbol for it yet, so the class is named after the address). Only
// the members act_npc_ride_switch reads are declared.
class Log_804ACD70 : public ActorBase_804A5770 {
public:
    bool isAtRest() const {
        if (field_0x230 == 0 && isMotionless() == true) {
            return true;
        }
        return false;
    }
    // True while field_0x2d4 is (nearly) zero.
    bool isMotionless() const { return field_0x2d4.squared() <= 32.0f * FLT_EPSILON; }
    /* 0x230 */ s32 field_0x230; // 0 while the log is at rest (npc_ride_switch)
    /* 0x234 */ u8 field_0x234[0xA0]; // not yet known
    /* 0x2D4 */ JGeometry::TVec3f field_0x2d4; // velocity? (zero while at rest)
    /* 0x2E0 */ u8 field_0x2e0[0x4]; // not yet known
}; // Size: 0x2E4

#endif
