#ifndef GAME_ROTMATRIX_H
#define GAME_ROTMATRIX_H

#include "dolphin/mtx.h"
#include "JSystem/JGeometry.h"
#include <math.h>

// The rotation part of `m` from Euler angles x, y, z (radians; R = Rz Ry Rx),
// every element times `scale`. Inlined in 20 functions of 19 units (draws
// that hand a SpriteParams_801F5AE0 a stack Mtx, e.g. act_hardhat_beetle):
// sin x, sin y, sin z, cos x, cos y, cos z through the double sin / cos
// (bl sin; frsp), then the nine products, each multiplied by scale last.
// The translation column is the caller's.
inline void setRotScaleMtx(Mtx m, const JGeometry::TVec3f& rot, f32 scale) {
    // Declared in this order: it gives sin x/y/z the original's f28/f27/f26
    // (act_hardhat_beetle's draw, tu_801A908C's fn_801A90D4).
    f32 sz, sy, sx;
    f32 cy, cx, cz;
    sx = sin(rot.x);
    sy = sin(rot.y);
    sz = sin(rot.z);
    cx = cos(rot.x);
    cy = cos(rot.y);
    cz = cos(rot.z);
    m[0][0] = scale * (cy * cz);
    m[1][0] = scale * (cy * sz);
    m[2][0] = scale * -sy;
    m[0][1] = scale * (sx * sy * cz - cx * sz);
    m[1][1] = scale * (sx * sy * sz + cx * cz);
    m[2][1] = scale * (sx * cy);
    m[0][2] = scale * (cx * cz * sy + sx * sz);
    m[1][2] = scale * (cx * sz * sy - sx * cz);
    m[2][2] = scale * (cx * cy);
}

#endif
