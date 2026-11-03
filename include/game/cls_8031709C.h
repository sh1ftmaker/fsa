#ifndef GAME_CLS_8031709C_H
#define GAME_CLS_8031709C_H

#include "types.h"
#include "JSystem/JGeometry.h"

// A small sound/effect sequencer, defined in game/tu_80316E34.cpp. No
// vtable; named after its default constructor (fn_8031709C, the one the
// callers run after __nw__(0x1C)). Held through a pointer by the Tingle and
// old-man actors (act_tingle_first +0x24C, act_tingle_tower +0x278,
// act_old_man +0x274), which delete it with the destructor at 0x80317060.
class cls_8031709C {
public:
    cls_8031709C();
    ~cls_8031709C();

    // The id of the current step, or -1 when no state is active.
    s32 fn_80316E34();
    // Resets the object to state -1 and stores the layer and position.
    void fn_80316E50(s32 layer, const JGeometry::TVec3f& pos);
    // Switches state to the one fn_8037E9F4(arg) returns and steps through
    // that state's sequence; returns the id for a new state, else -1.
    s32 fn_80316E74(s32 arg);

    /* 0x00 */ s32 field_0x0;  // id of the current step
    /* 0x04 */ s32 field_0x4;  // state, -1 for none
    /* 0x08 */ u8 field_0x8;   // step index in the state's table
    /* 0x09 */ u8 field_0x9;   // frames spent in the step
    /* 0x0C */ s32 field_0xc;  // effect layer (fn_8013CC50's first argument)
    /* 0x10 */ JGeometry::TVec3f field_0x10; // effect position
}; // Size: 0x1C

#endif
