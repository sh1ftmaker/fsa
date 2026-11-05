#ifndef GAME_VECEPSILON_H
#define GAME_VECEPSILON_H

#include "JSystem/JGeometry.h"

// Componentwise TUtil::epsilonEquals of two vectors: x and y into one bool, then z
// (the original materialises both bools). Shared by tu_8027E5BC (fn_8027E5BC) and
// tu_80256C74 (fn_80259830, the player standing still). Included only where used.
inline bool epsilonEqualsXY(const JGeometry::TVec3f& a, const JGeometry::TVec3f& b, f32 eps) {
    bool equal = false;
    if (JGeometry::TUtilf::epsilonEquals(a.x, b.x, eps) && JGeometry::TUtilf::epsilonEquals(a.y, b.y, eps)) {
        equal = true;
    }
    return equal;
}

inline bool epsilonEquals(const JGeometry::TVec3f& a, const JGeometry::TVec3f& b, f32 eps) {
    bool equal = false;
    if (epsilonEqualsXY(a, b, eps) && JGeometry::TUtilf::epsilonEquals(a.z, b.z, eps)) {
        equal = true;
    }
    return equal;
}

#endif
