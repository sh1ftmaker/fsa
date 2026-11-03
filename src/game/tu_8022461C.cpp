#include "game/GameAudio.h"

#include "game/GameGlobals.h"
#include "game/Map.h"

// fn_802252F4 (this unit, not written yet): tests the x/y of pos against the
// box of `layer` (fn_801CCBD0's family).
extern "C" bool fn_802252F4(const JGeometry::TVec3f* pos, s32 layer);

extern "C" bool fn_802255F4(const JGeometry::TVec3f* pos, s32 layer) {
    return fn_802252F4(pos, layer);
}

bool fn_802265DC(const JGeometry::TVec3f* pos, s32 layer, bool flag) {
    return fn_802260DC((const JGeometry::TVec2f*)pos, layer, flag);
}
