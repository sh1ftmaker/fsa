#include "game/GameAudio.h"
#include "game/DrawShadow.h"
#include "game/GameGlobals.h"
#include "game/cls_804A8588.h"

// Two entries; field_0x248 indexes it when field_0x240 is 1.
static const f32 lbl_805409C8[2] = {1.0f, -1.0f};

// FIXME: 99.46 %. Instructions identical; only the literal split differs
// (lbl_80540A0C/0A10/0A14 and others are @NNN here: earlier, unwritten
// functions of this unit own the first literals).
void RotatingSkull_804A8588::init() {
    mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
    field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
    mPos.y += 12.0f;
    field_0x234.set(mPos);
    field_0x250 = 4;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x248 = field_0x90 & 0x7F;
    field_0x24c = (field_0x90 >> 12) & 0x1F;
    field_0x240 = (field_0x90 >> 7) & 0x1F;
    if (field_0x240 == 0) {
        field_0x25c = 0;
    } else {
        field_0x25c = 1;
    }
    field_0x270.x = field_0x270.y = field_0x270.z = 0.0f;
    field_0x27c = 1;
    if (field_0x240 == 1) {
        field_0x260 = lbl_805409C8[field_0x248];
        field_0x264 = 1.0f;
        field_0x26c = 0.75f;
    } else if (field_0x240 == 2) {
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x80.set(-6.0f, -6.0f, 6.0f, 6.0f);
        switch (field_0x248) {
        case 0:
            field_0x3c = JGeometry::TVec3f(1.0f, 0.0f, 0.0f);
            break;
        case 1:
            field_0x3c = JGeometry::TVec3f(-1.0f, 0.0f, 0.0f);
            break;
        case 2:
            field_0x3c = JGeometry::TVec3f(0.0f, 1.0f, 0.0f);
            break;
        case 3:
            field_0x3c = JGeometry::TVec3f(0.0f, -1.0f, 0.0f);
            break;
        }
    }
    field_0x230 = 0;
}

// FIXME(shotgun): 99.91 %, reloc-name (params is built and unused in the original)
void RotatingSkull_804A8588::fn_802B2728() {
    if (field_0x25c != 0) {
        SpriteParams_801F5AE0 params;
        params.field_0x1d = 0;
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x258 + 0x78, NULL, 1, 1.0f, 1.0f);
        drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 128), 30.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x258 + 0x78, 0, 2, -1, false, 0, 0);
    }
}

// The sign of a bounce: -1 when the push (`along`) and the other velocity component point
// the way the axis needs, else 1.
// FIXME(shotgun): fn_802B21EC is structure only (the original's symbol is the plain `fn_802B21EC`).
void RotatingSkull_804A8588::fn_802B21EC() {
    f32 sign = 1.0f;
    JGeometry::TVec3f hitB(0.0f, 0.0f, 0.0f);
    JGeometry::TVec3f hitA(0.0f, 0.0f, 0.0f);
    JGeometry::TVec3f savedPos(mPos);
    JGeometry::TVec3f savedVel(field_0x3c);
    JGeometry::TVec3f savedVel2(field_0x270);
    field_0x27c = 0;
    mPos += field_0x3c;
    u8 hit = fn_801F2618(&hitA, false);
    if (field_0x270.x != 0.0f) {
        field_0x27c = 1;
        mPos += field_0x270;
        u8 hit2 = fn_801F2618(&hitB, false);
        if (hit == 1 && hitA.y != 0.0f) {
            if (field_0x270.x > 0.0f) {
                if (!(field_0x3c.y > 0.0f)) {
                    sign = -1.0f;
                }
            } else if (field_0x3c.y > 0.0f) {
                sign = -1.0f;
            }
            field_0x3c.x = -field_0x3c.y * sign;
            field_0x270.y = field_0x270.x * sign;
            field_0x3c.y = 0.0f;
            field_0x270.x = 0.0f;
        } else if (!hit2 || hitB.x == 0.0f) {
            if (field_0x270.x > 0.0f) {
                if (field_0x3c.y > 0.0f) {
                    sign = -1.0f;
                }
            } else if (!(field_0x3c.y > 0.0f)) {
                sign = -1.0f;
            }
            field_0x3c.x = -field_0x3c.y * sign;
            field_0x270.y = field_0x270.x * sign;
            field_0x3c.y = 0.0f;
            field_0x270.x = 0.0f;
            JGeometry::TVec3f prev(mPos);
            mPos.x += field_0x3c.x * 3.0f + field_0x270.x;
            mPos.y += field_0x3c.y * 3.0f + field_0x270.y;
            mPos.z += field_0x3c.z * 3.0f + field_0x270.z;
            JGeometry::TVec3f hitC(0.0f, 0.0f, 0.0f);
            if (fn_801F2618(&hitC, false)) {
                mPos = prev;
            } else {
                mPos = savedPos;
                field_0x3c = savedVel;
                field_0x270 = savedVel2;
                mPos += field_0x3c;
            }
        }
    } else if (field_0x270.y != 0.0f) {
        field_0x27c = 1;
        mPos += field_0x270;
        u8 hit2 = fn_801F2618(&hitB, false);
        if (hit == 1 && hitA.x != 0.0f) {
            if (field_0x270.y > 0.0f) {
                if (!(field_0x3c.x > 0.0f)) {
                    sign = -1.0f;
                }
            } else if (field_0x3c.x > 0.0f) {
                sign = -1.0f;
            }
            field_0x3c.y = -field_0x3c.x * sign;
            field_0x270.x = field_0x270.y * sign;
            field_0x3c.x = 0.0f;
            field_0x270.y = 0.0f;
        } else if (!hit2 || hitB.y == 0.0f) {
            if (field_0x270.y > 0.0f) {
                if (field_0x3c.x > 0.0f) {
                    sign = -1.0f;
                }
            } else if (!(field_0x3c.x > 0.0f)) {
                sign = -1.0f;
            }
            field_0x3c.y = -field_0x3c.x * sign;
            field_0x270.x = field_0x270.y * sign;
            field_0x3c.x = 0.0f;
            field_0x270.y = 0.0f;
            JGeometry::TVec3f prev(mPos);
            mPos.x += field_0x3c.x * 3.0f + field_0x270.x;
            mPos.y += field_0x3c.y * 3.0f + field_0x270.y;
            mPos.z += field_0x3c.z * 3.0f + field_0x270.z;
            JGeometry::TVec3f hitC(0.0f, 0.0f, 0.0f);
            if (fn_801F2618(&hitC, false)) {
                mPos = prev;
            } else {
                mPos = savedPos;
                field_0x3c = savedVel;
                field_0x270 = savedVel2;
                mPos += field_0x3c;
            }
        }
    }
    if (field_0x27c == 1) {
        mPos = savedPos;
        field_0x3c = savedVel;
        field_0x270 = savedVel2;
    }
}

u8 RotatingSkull_804A8588::fn_802B21AC(s32 layer, const JGeometry::TVec3f* pos) {
    bool result = ActorBase_804A5770::vfunc_0x4C(layer, pos);
    if (!result) {
        field_0x27c = 0;
    }
    return result;
}

RotatingSkull_804A8588::~RotatingSkull_804A8588() {}

// fn_802B28E4 (0x802B28E4, 0x660 bytes): not written (budget).
