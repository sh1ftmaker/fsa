#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include <dolphin/gx.h>

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

void Camera_804AA9E4::fn_8030C85C() {
    field_0x1e8 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (lbl_8053AB10->field_0x20->field_0x235c[i]) {
            lbl_8053AB10->field_0x20->field_0x2360[i] = 0;
        }
    }
    fn_802D6A9C()->fn_802DCDF4();
    field_0x154.set(field_0x190);
    field_0x1b4.i.y = 0.0f;
    field_0x1b4.i.x = 0.0f;
    field_0x1b4.f.y = 0.0f;
    field_0x1b4.f.x = 0.0f;
}

void Camera_804AA9E4::fn_8030DD20(const Mtx m) {
    Mtx tmp;
    PSMTXConcat(field_0x74, m, tmp);
    GXSetCurrentMtx(GX_PNMTX9);
    GXLoadPosMtxImm(tmp, GX_PNMTX9);
    GXLoadNrmMtxImm(tmp, GX_PNMTX9);
}

void Camera_804AA9E4::fn_8030DD68() {
    GXSetCurrentMtx(GX_PNMTX8);
}

void Camera_804AA9E4::fn_8030E150() {}

Camera_804AA9E4::~Camera_804AA9E4() {}
