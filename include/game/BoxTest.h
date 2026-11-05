#ifndef GAME_BOXTEST_H
#define GAME_BOXTEST_H

#include "JSystem/JGeometry.h"

// Box helpers the Player units inline (tu_8026FC20, tu_80256C74). Included only where used.

// Whether p (x, y) is in box (min inclusive, max exclusive), the result materialised
// as a bool (li 0 / li 1).
template <typename V>
static inline bool containsPoint(const JGeometry::TBox2f& box, const V& p) {
    bool result = false;
    if (p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y) {
        result = true;
    }
    return result;
}

// `box` shrunk (d < 0) or grown by d on every side.
static inline void growBox(JGeometry::TBox2f& box, f32 d) {
    box.i.x -= d;
    box.i.y -= d;
    box.f.x += d;
    box.f.y += d;
}

#endif
