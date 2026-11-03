#include "game/GameAudio.h"
#include "game/Camera.h"
#include <dolphin/gx.h>

void Camera_804AA9E4::fn_8030DD68() {
    GXSetCurrentMtx(GX_PNMTX8);
}

Camera_804AA9E4::~Camera_804AA9E4() {}

void Camera_804AA9E4::fn_8030DD20(const Mtx m) {
    Mtx tmp;
    PSMTXConcat(field_0x74, m, tmp);
    GXSetCurrentMtx(GX_PNMTX9);
    GXLoadPosMtxImm(tmp, GX_PNMTX9);
    GXLoadNrmMtxImm(tmp, GX_PNMTX9);
}

void Camera_804AA9E4::fn_8030E150() {}

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec2f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

u8 Camera_804AA9E4::fn_8030C210(const JGeometry::TVec3f* pos) {
    s32 result = 0;
    s32 count = field_0x7f0;
    if (count < 0x80) {
        JGeometry::TVec2f p(pos->x, pos->y);
        if (contains(field_0x154, p)) {
            JGeometry::TVec3f* dst = &field_0x1f0[count];
            dst->x = p.x;
            dst->y = pos->y;
            dst->z = pos->z;
            field_0x7f0++;
            result = true;
        }
    }
    return result;
}
