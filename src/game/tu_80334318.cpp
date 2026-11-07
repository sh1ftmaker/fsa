#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/tu_801F9484.h"

// 0x8033571C.
// FIXME(shotgun): 99.11 %, reloc-name
void IronBallHoming_804AB9C0::init() {
    mLife = 0x80;
    field_0xb0 = 0x42304;
    if (field_0x90 & 1) {
        field_0x250 = 1;
        mBox.set(-8.0f, -4.0f, 8.0f, 12.0f);
        field_0x80.set(-14.0f, -8.0f, 14.0f, 20.0f);
    } else {
        field_0x250 = 0;
        mBox.set(-4.0f, -3.0f, 4.0f, 5.0f);
        field_0x80.set(-7.0f, -6.0f, 7.0f, 8.0f);
    }
    field_0x94 = (field_0x90 >> 4) & 1;
    field_0x234 = 0.0f;
    field_0x238 = 0.0f;
    field_0x23c = 0.0f;
    field_0x240 = 1.0f;
    field_0x268 = 0.0f;
    field_0x264 = 0.0f;
    field_0x260 = 0.0f;
    if (field_0x94 == 0) {
        field_0x230 = 0;
    } else {
        field_0x230 = 5;
        mPos.z = 100.0f;
    }
    field_0x24c = 0;
    field_0x248 = 10;
    field_0x244 = 100;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x26c = 0;
    field_0xb8 = 1;
}

// 0x80335688.
// FIXME(shotgun): 85.76 %, different
bool IronBallHoming_804AB9C0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    u16 tile = fn_8022461C(pos, layer);
    if (field_0x230 != 3 && (tile == 0xC || tile == 3 || tile == 0xE)) {
        if (field_0x230 != 7) {
            field_0x25c = 30;
        }
        field_0x230 = 7;
        return false;
    }
    if (tile == 0 || tile == 0x22) {
        return false;
    }
    return true;
}

// 0x80334378.
// FIXME(shotgun): 85.24 %, missing/extra
s32 IronBallHoming_804AB9C0::vfunc_0x54() {
    s32 serial = -1;
    while (true) {
        serial = getActorManager_801F856C()->fn_801F78F4('WARP', serial);
        if (serial < 0) {
            break;
        }
        ActorBase_804A5770* warp = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (warp == NULL) {
            continue;
        }
        u32 param = warp->field_0x90;
        if ((param & 0xFF) != 7) {
            continue;
        }
        s32 layer = (param >> 3) & 7;
        f32 baseX;
        f32 baseY;
        if (layer == 0) {
            baseX = getGameView_802D6A9C()->mVisibleBox.i.x;
            baseY = getGameView_802D6A9C()->mVisibleBox.i.y;
        } else {
            baseX = 0.0f;
            baseY = 0.0f;
            layer += 7;
        }
        JGeometry::TVec3f dest(8.0f + ((f32)((param >> 13) & 0x1F8) + baseX), 8.0f + ((f32)((param >> 5) & 0x1F8) + baseY), 0.0f);
        fn_801F349C(layer, &dest);
        break;
    }
    return serial;
}

// Destructor at 0x80334318.
IronBallHoming_804AB9C0::~IronBallHoming_804AB9C0() {}
