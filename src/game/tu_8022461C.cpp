#include "game/GameAudio.h"

#include "game/GameGlobals.h"
#include "game/Map.h"

bool fn_802255F4(const JGeometry::TVec3f* pos, s32 layer) {
    return fn_802252F4(pos, layer);
}

bool fn_802265DC(const JGeometry::TVec3f* pos, s32 layer, bool flag) {
    return fn_802260DC((const JGeometry::TVec2f*)pos, layer, flag);
}

// Clamps a box-relative coordinate to the 9-bit cell range [0, 0x200).
static inline void clampCell(s32& v) {
    if (v < 0) {
        v = 0;
    } else if (v >= 0x200) {
        v = 0x1FF;
    }
}

// The packed cell index of pos (no lookup; clamped like the queries, no outside test).
// The y term is written first to match the original's instruction order.
s32 fn_80226BC4(const JGeometry::TVec2f* pos, s32 layer) {
    GameView_804A9950* view = fn_802D6A9C();
    const JGeometry::TBox2f* src = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    JGeometry::TBox2f box;
    box = *src;
    s32 x = (s32)(pos->x - box.i.x);
    s32 y = (s32)(pos->y - box.i.y);
    clampCell(x);
    clampCell(y);
    return ((y & 0x100) << 1) | (x & 0x100) | (y & 0xF0) | ((x >> 4) & 0xF);
}
