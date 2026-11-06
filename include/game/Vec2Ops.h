#ifndef GAME_VEC2OPS_H
#define GAME_VEC2OPS_H

#include "JSystem/JGeometry.h"

// TVec2f arithmetic operators. The libs JGeometry TVec2 (mkdd's) has none; later JGeometry
// versions (SMG's TVec.hpp, reference only) have the same shape: copy *this into a local,
// change the copy, return it. The original builds every intermediate of such a chain in its
// own stack TVec2 (the copy, its result, the returned value), and operator+ hands back a
// reference to its local (no return copy: the caller copies straight from the local), as the
// later JGeometry's addOperatorInline does. Evidence: tu_8025EB18 fn_802604CC.

inline JGeometry::TVec2f operator-(const JGeometry::TVec2f& a, const JGeometry::TVec2f& b) {
    JGeometry::TVec2f r(a);
    r.x -= b.x;
    r.y -= b.y;
    return r;
}

inline const JGeometry::TVec2f& operator+(const JGeometry::TVec2f& a, const JGeometry::TVec2f& b) {
    JGeometry::TVec2f r(a);
    r.x += b.x;
    r.y += b.y;
    return r;
}

inline JGeometry::TVec2f operator*(const JGeometry::TVec2f& a, f32 s) {
    JGeometry::TVec2f r(a);
    r.x *= s;
    r.y *= s;
    return r;
}

// x / y of a TVec3f seen as a TVec2f (later JGeometry: TVec3's operator const TVec2<f32>&).
inline const JGeometry::TVec2f& xy(const JGeometry::TVec3f& v) {
    return *reinterpret_cast<const JGeometry::TVec2f*>(&v);
}

#endif
