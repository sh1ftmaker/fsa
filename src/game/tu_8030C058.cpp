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
