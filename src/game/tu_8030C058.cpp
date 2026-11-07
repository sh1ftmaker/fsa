#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include <dolphin/gx.h>
#include <string.h>
#include "JSystem/JUtility/JUTVideo.h"

// The screen pixel (px, py) as a position inside the box.
static inline void screenToWorld(const JGeometry::TBox2f& box, f32 px, f32 py, JGeometry::TVec3f* out) {
    out->x = box.i.x + px * (box.f.x - box.i.x) / (f32)(s32)JUTVideo::sManager->getFbWidth();
    out->y = box.i.y + py * (box.f.y - box.i.y) / (f32)(s32)JUTVideo::sManager->getEfbHeight();
    out->z = 0.0f;
}

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec2f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

Camera_804AA9E4::Camera_804AA9E4() {}

Camera_804AA9E4::~Camera_804AA9E4() {}

// FIXME(shotgun): 87 %, missing/extra (stores order, the box built in a different order)
void Camera_804AA9E4::fn_8030E5BC() {
    field_0x154.i.x = 0.0f;
    field_0x154.i.y = 0.0f;
    field_0x154.f.x = 512.0f;
    field_0x154.f.y = 384.0f;
    field_0x1e8 = 0;
    field_0x1b4.f.y = 0.0f;
    field_0x1b4.i.y = 0.0f;
    field_0x1b4.f.x = 0.0f;
    field_0x1b4.i.x = 0.0f;
    field_0x1dc.z = 0.0f;
    field_0x1dc.y = 0.0f;
    field_0x1dc.x = 0.0f;
    field_0x1c4[0] = 0;
    field_0x1c4[1] = 0;
    field_0x1c4[2] = 0;
    field_0x1c4[3] = 0;
    field_0x1d4 = -1.0f;
    field_0x1d8 = -1.0f;
    JGeometry::TBox2f box;
    box.i.x = 0.0f;
    box.i.y = 0.0f;
    box.f.x = 256.0f;
    box.f.y = 192.0f;
    fn_8030CB10(&box, 1024.0f);
    field_0x7f0 = 0;
    field_0x7f4 = 0;
    field_0x7f5 = 0;
    field_0x838 = -1;
    field_0x7f6 = 0;
    field_0x83c = -1;
    field_0x840 = 0;
    field_0x144.x0 = 0;
    field_0x144.y0 = 0;
    field_0x144.x1 = JUTVideo::sManager->getFbWidth();
    field_0x144.y1 = JUTVideo::sManager->getEfbHeight();
    field_0x858 = 1.0f;
    field_0x174 = 0;
    field_0x178 = 0;
    field_0x179 = 0;
    field_0x17a = 0;
    field_0x17b = 0;
    field_0x17c = 0;
    field_0x17d = 0;
    field_0x17e = 0;
}

// The per-frame update: picks the table entry for the pending request (flags 0x178-0x17B), lets
// fn_8030CD18 work out the target box and depth, blends towards it while a transition is armed,
// sets the view (fn_8030CB10) and the zoom, and tells the audio manager where the camera is.
// FIXME(shotgun): not compiled, needs fn_8030CD18 (4 KB, not written), the 0x24-byte entry table
// at lbl_804AA8E8 and the camera-centre position the audio call takes (exact args unchecked)
#ifdef NONMATCHING
void Camera_804AA9E4::fn_8030E154() {
    JGeometry::TBox2f box = field_0x154;
    if (gGameRoot_8053AAF8->field_0x7e != 0) {
        field_0x174 = field_0x179 != 0 ? 3 : 1;
    } else if (field_0x179 != 0) {
        field_0x174 = 3;
    } else if (field_0x17a != 0) {
        field_0x174 = 6;
    } else if (field_0x178 != 0) {
        field_0x174 = 2;
    } else if (field_0x17b != 0) {
        field_0x174 = 5;
    } else if (field_0x7f0 == 0) {
        field_0x174 = 4;
    } else {
        field_0x174 = 0;
    }
    field_0x179 = 0;
    field_0x178 = 0;
    field_0x17a = 0;
    field_0x17b = 0;
    if (field_0x1b4.i.x == 0.0f && field_0x1b4.i.y == 0.0f && field_0x1b4.f.x == 0.0f &&
        field_0x1b4.f.y == 0.0f) {
        field_0x7f5 = 0;
    }
    if (field_0x17c != 0) {
        f32 w = box.f.x - box.i.x;
        f32 h = box.f.y - box.i.y;
        box.i.x = box.i.x - w;
        box.i.y = box.i.y - h;
        box.f.x = box.f.x + w;
        box.f.y = box.f.y + h;
    }
    if (field_0x17d == 0) {
        JGeometry::TBox2f target;
        f32 depth;
        fn_8030CD18(&box, &lbl_804AA8E8[field_0x174], &field_0x1b4, &target, &depth);
        if (field_0x1e8 != 0) {
            f32 t = (f32)field_0x1ec / 30.0f;
            t = t * t;
            f32 u = 1.0f - t;
            target.i.x = t * target.i.x + u * field_0x1a0[0];
            target.i.y = t * target.i.y + u * field_0x1a0[1];
            target.f.x = t * target.f.x + u * field_0x1a0[2];
            target.f.y = t * target.f.y + u * field_0x1a0[3];
            depth = t * depth + u * field_0x1b0;
            if (field_0x1ec == 0) {
                field_0x1e8 = 0;
                fn_8030C85C();  // the reset of the armed state (inlined in the target)
            } else {
                field_0x1ec--;
            }
        }
        if (field_0x17e != 0) {
            field_0x1b4.i.x = 0.0f;
            field_0x1b4.i.y = 0.0f;
            field_0x1b4.f.x = 0.0f;
            field_0x1b4.f.y = 0.0f;
            target = field_0x180;
            depth = 0.5f * (target.i.y + target.f.y) + 3072.0f;
            field_0x17e = 0;
        }
        fn_8030CB10(&target, depth);
        field_0x858 = (f32)(s32)JUTVideo::sManager->getFbWidth() / (target.f.x - target.i.x);
        JGeometry::TVec3f center;
        center.set(0.5f * (target.i.x + target.f.x), 0.5f * (target.i.y + target.f.y), 0.5f * depth);
        gAudioMgr_8053AF80->fn_80459624(&center, field_0x4, 0);
    }
    field_0x7f0 = 0;
    field_0x7f4 = 0;
    field_0x7f5 = 0;
    field_0x840 = 0;
    field_0x17c = 0;
    field_0x17d = 0;
}
#endif

void Camera_804AA9E4::fn_8030E150() {}

// Loads the projection / view into the GX state (taking the pending matrices first when copy is
// set) and sets the viewport and scissor to the screen rect.
// FIXME(shotgun): 99.5 %, regalloc-only
void Camera_804AA9E4::fn_8030DFE8(bool copy) {
    if (copy) {
        JMath::gekko_ps_copy12(field_0x74, field_0x4);
        JMath::gekko_ps_copy16(field_0xa4, field_0x34);
    }
    GXSetProjection(field_0xa4, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(field_0x74, GX_PNMTX8);
    GXLoadNrmMtxImm(field_0x74, GX_PNMTX8);
    GXSetCurrentMtx(GX_PNMTX8);
    u32 w = field_0x144.x1 - field_0x144.x0;
    u32 h = field_0x144.y1 - field_0x144.y0;
    GXSetViewport((f32)field_0x144.x0, (f32)field_0x144.y0, (f32)w, (f32)h, 0.0f, 1.0f);
    GXSetScissor(field_0x144.x0, field_0x144.y0, w, h);
    GXSetZScaleOffset(1.0f, 0.0f);
}

// The 2D view of the box: the view matrix straight into the GX one, the projection from the box's
// size, and (when light is set) the light matrices; then the GX state of fn_8030DFE8.
// FIXME(shotgun): 75 %, missing/extra
void Camera_804AA9E4::fn_8030DD8C(const JGeometry::TBox2f* box, bool light) {
    field_0x164 = *box;
    f32 halfW = 0.5f * (box->f.x - box->i.x);
    f32 halfH = 0.5f * (box->f.y - box->i.y);
    f32 centerX = 0.5f * (box->i.x + box->f.x);
    f32 centerY = 0.5f * (box->i.y + box->f.y);
    f32 depth = 2048.0f;
    field_0x74[0][0] = 1.0f;
    field_0x74[0][1] = 0.0f;
    field_0x74[0][2] = 0.0f;
    field_0x74[0][3] = -centerX;
    field_0x74[1][0] = 0.0f;
    field_0x74[1][1] = 1.0f;
    field_0x74[1][2] = -1.0f;
    field_0x74[1][3] = -centerY;
    field_0x74[2][0] = 0.0f;
    field_0x74[2][1] = 1.0f;
    field_0x74[2][2] = 0.0f;
    field_0x74[2][3] = -depth;
    C_MTXOrtho(field_0xa4, -halfH, halfH, -halfW, halfW, 0.0f, 4096.0f);
    if (light) {
        PSMTXCopy(field_0x74, field_0xe4.mCamMtx);
        C_MTXLightOrtho(field_0xe4.mPrjMtx, halfH, -halfH, -halfW, halfW, 0.5f, 0.5f, 0.5f, 0.5f);
    }
    fn_8030DFE8(false);
}

void Camera_804AA9E4::fn_8030DD68() {
    GXSetCurrentMtx(GX_PNMTX8);
}

void Camera_804AA9E4::fn_8030DD20(const Mtx m) {
    Mtx tmp;
    PSMTXConcat(field_0x74, m, tmp);
    GXSetCurrentMtx(GX_PNMTX9);
    GXLoadPosMtxImm(tmp, GX_PNMTX9);
    GXLoadNrmMtxImm(tmp, GX_PNMTX9);
}

// Not inlined into its caller fn_8030E5BC in the original.
#pragma auto_inline off
// FIXME(shotgun): 70 %, missing/extra (the target keeps the box corners in float registers)
void Camera_804AA9E4::fn_8030CB10(const JGeometry::TBox2f* box, f64 depth) {
    f32 halfW = 0.5f * (box->f.x - box->i.x);
    f32 halfH = 0.5f * (box->f.y - box->i.y);
    f32 centerX = 0.5f * (box->i.x + box->f.x);
    f32 centerY = 0.5f * (box->i.y + box->f.y);
    // The view: x and y shifted to the box's centre, z the depth axis.
    field_0x4[0][0] = 1.0f;
    field_0x4[0][1] = 0.0f;
    field_0x4[0][2] = 0.0f;
    field_0x4[0][3] = -centerX;
    field_0x4[1][0] = 0.0f;
    field_0x4[1][1] = 1.0f;
    field_0x4[1][2] = -1.0f;
    field_0x4[1][3] = -centerY;
    field_0x4[2][0] = 0.0f;
    field_0x4[2][1] = 1.0f;
    field_0x4[2][2] = 0.0f;
    field_0x4[2][3] = (f32)-depth;
    C_MTXOrtho(field_0x34, -halfH, halfH, -halfW, halfW, 0.0f, 1024.0f);
    PSMTXCopy(field_0x4, field_0xe4.mCamMtx);
    C_MTXLightOrtho(field_0xe4.mPrjMtx, halfH, -halfH, -halfW, halfW, 0.5f, 0.5f, 0.5f, 0.5f);
    JMath::gekko_ps_copy12(field_0x74, field_0x4);
    JMath::gekko_ps_copy16(field_0xa4, field_0x34);
    field_0x164 = *box;
}
#pragma auto_inline reset

// Starts the focus transition to the new box: arms the camera, clears the damping, builds the
// focus state from the table entry for field_0x174 (fn_8030CD18), then the same bookkeeping as
// fn_8030C928.
// FIXME(shotgun): not compiled, needs fn_8030CD18 (4 KB, not written) and the 0x24-byte entry
// table at lbl_804AA8E8 (unit data, not in the source yet)
#ifdef NONMATCHING
void Camera_804AA9E4::fn_8030C9E4(const JGeometry::TBox2f* box) {
    field_0x1e8 = 1;
    field_0x1ec = 30;
    field_0x7f5 = 0;
    field_0x1d4 = -1.0f;
    field_0x1d8 = -1.0f;
    fn_8030CD18(box, &lbl_804AA8E8[field_0x174], 0, field_0x1a0, field_0x1b0);
    fn_8030C928(box);
}
#endif

// FIXME(shotgun): 93.4 %, missing/extra (box copy order)
void Camera_804AA9E4::fn_8030C928(const JGeometry::TBox2f* box) {
    field_0x190.set(*box);
    for (s32 i = 0; i < 4; i++) {
        gGameRoot2_8053AB10->field_0x20->field_0x2360[i] = gGameRoot2_8053AB10->field_0x20->field_0x235c[i];
    }
    gGameRoot2_8053AB10->field_0x20->field_0x2374 = gGameRoot2_8053AB10->field_0x20->field_0x2368;
    gGameRoot2_8053AB10->field_0x20->field_0x2368 = JGeometry::TVec3f(box->i.x, box->i.y, 0.0f);
}

void Camera_804AA9E4::fn_8030C85C() {
    field_0x1e8 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot2_8053AB10->field_0x20->field_0x235c[i]) {
            gGameRoot2_8053AB10->field_0x20->field_0x2360[i] = 0;
        }
    }
    getGameView_802D6A9C()->fn_802DCDF4();
    field_0x154.set(field_0x190);
    field_0x1b4.i.y = 0.0f;
    field_0x1b4.i.x = 0.0f;
    field_0x1b4.f.y = 0.0f;
    field_0x1b4.f.x = 0.0f;
}

// The camera's box of the given width around center (the height keeps the screen's aspect).
// FIXME(shotgun): 38 %, scheduling+regalloc
void Camera_804AA9E4::fn_8030C7C4(const JGeometry::TVec2f* center, f32 size) {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    f32 half = 0.5f * size;
    cam->field_0x180.i.x = center->x - half;
    f32 halfY = 0.5f * (size * (f32)(s32)JUTVideo::sManager->getEfbHeight() /
                        (f32)(s32)JUTVideo::sManager->getFbWidth());
    cam->field_0x180.i.y = center->y - halfY;
    cam->field_0x180.f.x = center->x + half;
    cam->field_0x180.f.y = center->y + halfY;
    cam->field_0x17e = 1;
}

// A world position's screen position: normalised (0..1) when normalised is set, else in the
// screen rect (inRect) or the whole screen. z is cleared.
// FIXME(shotgun): 75 %, missing/extra (constant 1.0 folding)
void Camera_804AA9E4::fn_8030C5F0(const Vec* pos, bool inRect, bool normalised, Vec* out) {
    Vec v;
    PSMTXMultVec(field_0x74, pos, &v);
    f32 x = v.x * field_0xa4[0][0] + v.y * field_0xa4[0][1] + v.z * field_0xa4[0][2] + 1.0f * field_0xa4[0][3];
    f32 y = v.x * field_0xa4[1][0] + v.y * field_0xa4[1][1] + v.z * field_0xa4[1][2] + 1.0f * field_0xa4[1][3];
    if (normalised) {
        out->x = 0.5f * (1.0f + x);
        out->y = 0.5f * (1.0f - y);
    } else if (inRect) {
        out->x = 0.5f * (1.0f + x) * (f32)(field_0x144.x1 - field_0x144.x0) + (f32)field_0x144.x0;
        out->y = 0.5f * (1.0f - y) * (f32)(field_0x144.y1 - field_0x144.y0) + (f32)field_0x144.y0;
    } else {
        out->x = 0.5f * (1.0f + x) * (f32)(s32)JUTVideo::sManager->getFbWidth();
        out->y = 0.5f * (1.0f - y) * (f32)(s32)JUTVideo::sManager->getEfbHeight();
    }
    out->z = 0.0f;
}

// FIXME(shotgun): 32 %, missing/extra (instruction order of the conversions)
void Camera_804AA9E4::fn_8030C4A0(const JGeometry::TVec2f* in, bool flag, JGeometry::TVec3f* out) {
    if (flag) {
        out->x = field_0x164.i.x +
                 (in->x - (f32)field_0x144.x0) * (field_0x164.f.x - field_0x164.i.x) /
                     (f32)(field_0x144.x1 - field_0x144.x0);
        out->y = field_0x164.i.y +
                 (in->y - (f32)field_0x144.y0) * (field_0x164.f.y - field_0x164.i.y) /
                     (f32)(field_0x144.y1 - field_0x144.y0);
        out->z = 0.0f;
    } else {
        out->x = field_0x164.i.x + in->x * (field_0x164.f.x - field_0x164.i.x) /
                                       (f32)(s32)JUTVideo::sManager->getFbWidth();
        out->y = field_0x164.i.y + in->y * (field_0x164.f.y - field_0x164.i.y) /
                                       (f32)(s32)JUTVideo::sManager->getEfbHeight();
        out->z = 0.0f;
    }
}

// The camera box's world positions of the screen's top-left and bottom-right corners.
// FIXME(shotgun): 35 %, missing/extra (the target converts the screen size more often)
void Camera_804AA9E4::fn_8030C380(JGeometry::TVec3f* out) {
    screenToWorld(field_0x164, 0.0f, 0.0f, &out[0]);
    screenToWorld(field_0x164, (f32)(s32)JUTVideo::sManager->getFbWidth(),
                  (f32)(s32)JUTVideo::sManager->getEfbHeight(), &out[1]);
}

// Whether the point's screen position lies inside the screen rect shrunk by 14 on every side.
// FIXME(shotgun): 98.6 %, operands (u32 -> float constant)
bool Camera_804AA9E4::fn_8030C2A8(const JGeometry::TVec3f* pos) {
    Vec screen;
    fn_8030C5F0(pos, true, false, &screen);
    bool result = false;
    if (screen.x >= (f32)(field_0x144.x0 + 14) && screen.x < (f32)(field_0x144.x1 - 14) &&
        screen.y >= (f32)(field_0x144.y0 + 14) && screen.y < (f32)(field_0x144.y1 - 14)) {
        result = true;
    }
    return result;
}

u8 Camera_804AA9E4::addFocusPoint_8030C210(const JGeometry::TVec3f* pos) {
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

// Adds the box's two corners as focus points when both lie inside the camera box and there is
// room for two.
// FIXME(shotgun): 90.1 %, scheduling/regalloc (operand load order)
bool Camera_804AA9E4::fn_8030C058(const JGeometry::TBox2f* box) {
    bool result = false;
    s32 count = field_0x7f0;
    if (count + 1 < 0x80) {
        if (contains(field_0x154, box->i) && contains(field_0x154, box->f)) {
            if (count < 0x80 && contains(field_0x154, box->i)) {
                JGeometry::TVec3f* dst = &field_0x1f0[count];
                dst->x = box->i.x;
                dst->y = box->i.y;
                dst->z = 0.0f;
                field_0x7f0++;
            }
            count = field_0x7f0;
            if (count < 0x80 && contains(field_0x154, box->f)) {
                JGeometry::TVec3f* dst = &field_0x1f0[count];
                dst->x = box->f.x;
                dst->y = box->f.y;
                dst->z = 0.0f;
                field_0x7f0++;
            }
            result = true;
        }
    }
    return result;
}
