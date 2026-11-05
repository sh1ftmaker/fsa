#ifndef GAME_DRAWSHADOW_H
#define GAME_DRAWSHADOW_H

#include "game/GameGlobals.h"

// Draws the shadow-like sprite of ShadowCircle_804A0160 (gGameRoot2_8053AB10->0x2C). 18 units
// (24 call sites) compute the layer and position first and load the receiver
// last, which this wrapper reproduces (a direct chained call loads the
// receiver first). Name is descriptive (scratchpad research_drawrecv).
inline void drawShadow(s32 layer, const JGeometry::TVec3f* pos, JUtility::TColor color, f32 scale) {
    gGameRoot2_8053AB10->field_0x2c->drawShadow_80158B5C(layer, pos, color, scale);
}

#endif
