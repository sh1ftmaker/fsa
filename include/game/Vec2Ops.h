#ifndef GAME_VEC2OPS_H
#define GAME_VEC2OPS_H

#include "JSystem/JGeometry.h"

// x / y of a TVec3f seen as a TVec2f: the body of TVec3's `operator const TVec2<f32>&` in the
// later JGeometry (SMG's TVec.hpp, reference only). The TVec2f arithmetic operators (+, -, *)
// are members of TVec2 in libs JGeometry/Vec.h. Evidence: tu_8025EB18 fn_802604CC.
inline const JGeometry::TVec2f& xy(const JGeometry::TVec3f& v) {
    return *reinterpret_cast<const JGeometry::TVec2f*>(&v);
}

#endif
