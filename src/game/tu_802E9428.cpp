#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Camera.h"
#include "JSystem/JMath/JMath.h"
#include "game/Unk_8045A080.h"
#include "game/act/tu_8044B80C.h"
#include "game/act/tu_802E9428.h"

// FIXME(shotgun): not written (size 0x4A4): fn_802EAD78, a compiler-generated static-init function
// (float tables and JAudio memory pools), nothing to write by hand.
// FIXME(shotgun): not written (size 0x115C): update (0x802E9B6C)

// FIXME(shotgun): 99.7 %, operands + reloc-name
void UnknownENDM_804AA0FC::init() {
    field_0x284 = getGameView_802D6A9C()->field_0x4;
    field_0x280 = getGameView_802D6A9C()->field_0x8;
    field_0x23c = 0;
    field_0x230 = 0;
    field_0x28d = 0;
    field_0x298 = 0;
    field_0x240 = 0;
    field_0x294 = 0;
    field_0x295 = 0;
    field_0x299 = 1;
    field_0x270 = 300.0f;
    field_0x274 = 300.0f;
    field_0x278 = 0.0f;
    gAudioMgr_8053AF80->fn_80458FF0(0x100028, true, 0);
    field_0x234 = 0;
    field_0x231 = 0;
    field_0x29a = lbl_8053AF30->fn_8044B80C(field_0x294);
}

// Whether the focus `pos` has reached the edge of the 512 x 384 tile (the walk direction in
// field_0x290: 0 up, 1 down, 2 left, 3 right); in the settling state (field_0x297 set) it
// clears the flag once the edge is passed.
// FIXME(shotgun): 98 %, scheduling+regalloc
bool UnknownENDM_804AA0FC::fn_802E9884(const JGeometry::TVec2f* pos) {
    s32 ry = (s32)pos->y % 384 - 30;
    s32 rx = (s32)pos->x % 512;
    if (field_0x297 == 0) {
        u8 dir = *field_0x290;
        if (dir == 0xFF) {
            field_0x23c = 7;
            return false;
        }
        switch (dir) {
        case 0:
            if (ry - ((s32)field_0x270 * 3 / 4) / 2 < 0x10) {
                return true;
            }
            break;
        case 1:
            if (ry + ((s32)field_0x270 * 3 / 4) / 2 > 0x170) {
                return true;
            }
            break;
        case 2:
            if (rx - (s32)field_0x270 / 2 < 0x10) {
                return true;
            }
            break;
        case 3:
            if (rx + (s32)field_0x270 / 2 > 0x1F0) {
                return true;
            }
            break;
        }
    } else {
        switch (field_0x28d) {
        case 0: {
            s32 v = ry + ((s32)field_0x270 * 3 / 4) / 2;
            if (v < 0x170 && v > 0x160) {
                field_0x297 = 0;
            }
            break;
        }
        case 1: {
            s32 v = ry - ((s32)field_0x270 * 3 / 4) / 2;
            if (v > 0x20 && v < 0x30) {
                field_0x297 = 0;
            }
            break;
        }
        case 2: {
            s32 v = rx + (s32)field_0x270 / 2;
            if (v < 0x1F0 && v > 0x1E0) {
                field_0x297 = 0;
            }
            break;
        }
        case 3: {
            s32 v = rx - (s32)field_0x270 / 2;
            if (v > 0x10 && v < 0x20) {
                field_0x297 = 0;
            }
            break;
        }
        }
    }
    if (field_0x297 == 0) {
        getGameView_802D6A9C()->fn_802D9CB0();
    }
    return false;
}

// Follows the target focus: closes in on (field_0x244, field_0x248) by easing, keeps the camera box
// inside the limits, eases the zoom to its target and hands both to the camera.
// FIXME(shotgun): 84 %, missing/extra + reloc-name
void UnknownENDM_804AA0FC::fn_802E94F4() {
    f32 tx = field_0x244;
    f32 ty = field_0x248;
    if (field_0x298 != 0) {
        field_0x250 = tx;
        field_0x254 = ty;
    } else {
        JGeometry::TVec3f vel = field_0x240->field_0x3c;
        f32 dx = field_0x250 - tx;
        f32 dy = field_0x254 - ty;
        f32 sq = dx * dx + dy * dy;
        f32 dist = sq;
        if (!(sq <= 0.0f)) {
            dist = sq * JGeometry::TUtil<f32>::inv_sqrt(sq);
        }
        f32 speed = PSVECMag(&vel);
        if (dist <= 1.5f * speed) {
            field_0x250 = tx;
            field_0x254 = ty;
            field_0x298 = 1;
        } else if (dist <= 2.0f * speed) {
            field_0x250 = 0.5f * (field_0x244 - field_0x250) + field_0x250;
            field_0x254 = 0.5f * (field_0x248 - field_0x254) + field_0x254;
        } else if (0.1f * dist < 2.5f) {
            field_0x250 = 0.1f * (field_0x244 - field_0x250) + field_0x250;
            field_0x254 = 0.1f * (field_0x248 - field_0x254) + field_0x254;
        } else {
            s32 angle = (s32)(JMath::atanTable_.atan2_(field_0x248 - field_0x254, field_0x244 - field_0x250) * 10430.378f);
            field_0x250 = 2.5f * JMath::sincosTable_.cosShort(angle) + field_0x250;
            field_0x254 = 2.5f * JMath::sincosTable_.sinShort(angle) + field_0x254;
        }
    }
    JGeometry::TVec2f focus;
    focus.x = field_0x250 + 0.0f;
    focus.y = field_0x254 + -30.0f;
    JGeometry::TBox2f view = gGameRoot2_8053AB10->field_0x24->field_0x164;
    f32 halfW = (view.f.x - view.i.x) * 0.5f;
    f32 halfH = (view.f.y - view.i.y) * 0.5f;
    if (focus.x + halfW >= field_0x268) {
        focus.x = field_0x268 - halfW;
    } else if (focus.x - halfW <= field_0x260) {
        focus.x = field_0x260 + halfW;
    }
    if (focus.y + halfH >= field_0x26c) {
        focus.y = field_0x26c - halfH;
    } else if (focus.y - halfH <= field_0x264) {
        focus.y = field_0x264 + halfH;
    }
    if (field_0x270 != field_0x274) {
        field_0x270 += field_0x278;
        if (field_0x278 < 0.0f) {
            if (field_0x270 < field_0x274) {
                field_0x270 = field_0x274;
            }
        } else if (field_0x270 > field_0x274) {
            field_0x270 = field_0x274;
        }
    }
    gGameRoot2_8053AB10->field_0x24->fn_8030C7C4(&focus, (f32)(s32)field_0x270);
}

// FIXME(shotgun): 93.7 %, missing/extra
bool UnknownENDM_804AA0FC::fn_802E948C(f32 target, s32 frames) {
    if (field_0x270 == target) {
        return false;
    }
    f32 step = fabs(field_0x270 - target) / (f32)frames;
    if (field_0x270 < target) {
        field_0x278 = step;
    } else {
        field_0x278 = -step;
    }
    field_0x274 = target;
    return true;
}

void UnknownENDM_804AA0FC::draw() {}

UnknownENDM_804AA0FC::~UnknownENDM_804AA0FC() {}
