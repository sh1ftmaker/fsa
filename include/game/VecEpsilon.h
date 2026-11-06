#ifndef GAME_VECEPSILON_H
#define GAME_VECEPSILON_H

#include "JSystem/JGeometry.h"

// Componentwise epsilon tests of two vectors: x and y into one bool, then z (the
// original materialises both bools). Shared by tu_8027E5BC (fn_8027E5BC), tu_80256C74
// (fn_80259830, the player standing still), tu_8026FC20 and tu_8025EB18 (the carry
// walk cycle). Included only where used.

// Whether the difference d lies within [-eps, eps], materialised as a bool. The vector
// tests take the component differences first: with TUtil::epsilonEquals(a, b, eps)
// (`a -= b` on its parameter) every caller keeps 32 * FLT_EPSILON and the difference in
// fresh registers (f3 / f4 for the original's f2 / f0); with the difference as the
// argument tu_8025EB18 fn_8026152C and fn_80263C64 match and the other callers in
// tu_80256C74, tu_8027E5BC and tu_8026FC20 come closer (none lower).
inline bool nearZero(f32 d, f32 eps) {
    bool r = false;
    if (-eps <= d && d <= eps) {
        r = true;
    }
    return r;
}

inline bool epsilonEqualsXY(const JGeometry::TVec3f& a, const JGeometry::TVec3f& b, f32 eps) {
    bool equal = false;
    if (nearZero(a.x - b.x, eps) && nearZero(a.y - b.y, eps)) {
        equal = true;
    }
    return equal;
}

inline bool epsilonEquals(const JGeometry::TVec3f& a, const JGeometry::TVec3f& b, f32 eps) {
    bool equal = false;
    if (epsilonEqualsXY(a, b, eps) && nearZero(a.z - b.z, eps)) {
        equal = true;
    }
    return equal;
}

#endif
