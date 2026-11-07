#ifndef GAME_ROTMATRIX_H
#define GAME_ROTMATRIX_H

#include "dolphin/mtx.h"
#include "JSystem/JGeometry.h"
#include <math.h>

// game/act_princess_zelda_demo.cpp (owner unit; the callers include tu_8013C5C8's fn_8013CAA4). The rotation of `angle` radians about `axis`
// (normalised inside) into the 3x3 part of `m`; the translation is the caller's.
void fn_8010F668(Mtx m, const JGeometry::TVec3f* axis, f32 angle);

// The rotation part of `m` from Euler angles x, y, z (radians; R = Rz Ry Rx),
// every element times `scale`. Inlined in 20 functions of 19 units (draws
// that hand a SpriteParams_801F5AE0 a stack Mtx, e.g. act_hardhat_beetle):
// sin x, sin y, sin z, cos x, cos y, cos z through the double sin / cos
// (bl sin; frsp), then the nine products, each multiplied by scale last.
// The translation column is the caller's. Unproven: no caller is 100 % yet
// (tu_801A908C fn_801A90D4, act_hardhat_beetle's draw: 99.0 %, regalloc).
inline void setRotScaleMtx(Mtx m, const JGeometry::TVec3f& rot, f32 scale) {
    f32 sx = sin(rot.x);
    f32 sy = sin(rot.y);
    f32 sz = sin(rot.z);
    f32 cx = cos(rot.x);
    f32 cy = cos(rot.y);
    f32 cz = cos(rot.z);
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
