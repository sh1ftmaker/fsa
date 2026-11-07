#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/act/act_force_gem_fleeing.h"

// FIXME(shotgun): not written (size 0x1518): update (0x8031C598)
// FIXME(shotgun): not written (size 0x4E8): draw (0x8031C0B0)

// FIXME(shotgun): 99.2 %, different
void FleeingForceGem_804AB170::init() {
    field_0x268 = field_0x90 & 3;
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    for (int i = 0; i < 5; i++) {
        field_0x240[i] = 0;
    }
    field_0x274 = 0;
    field_0x276 = field_0x274;
    mLife = 10000;
    field_0x264 = -1;
    field_0x98 = 0;
    field_0x288.x = mPos.x;
    field_0x288.y = mPos.y;
    field_0x288.z = mPos.z;
    field_0x260 = 100;
    field_0x238 = 1.0f;
    field_0x23c = 0.0f;
    field_0x278 = 0;
    field_0x270 = field_0x4;
    field_0x234 = 30.0f;
    field_0x25c = (s32)(gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 65535.0f);
    field_0xb0 = 0x2000A;
    field_0x294.z = 0.0f;
    field_0x294.y = 0.0f;
    field_0x294.x = 0.0f;
    field_0x26c = 0;
    switch (field_0x94) {
    case 0:
        field_0x268 = field_0x90 & 3;
        field_0x230 = 0;
        field_0x278 = 1;
        break;
    case 1:
        field_0x268 = field_0x90 & 3;
        field_0x230 = 4;
        break;
    case 2:
        field_0x230 = 8;
        break;
    }
    field_0x27c.x = 0.0f;
    field_0x27c.y = 0.0f;
    field_0x27c.z = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0xb8 = 1;
    field_0x80.i.x = -6.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 6.0f;
    field_0x80.f.y = 6.0f;
    mBox.i.x = -6.0f;
    mBox.i.y = -6.0f;
    mBox.f.x = 6.0f;
    mBox.f.y = 6.0f;
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        fn_801F3668();
    } else {
        field_0x194 = 1;
    }
}

// FIXME(shotgun): 89.2 %, missing/extra
bool FleeingForceGem_804AB170::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tile = (u16)fn_8022461C(pos, layer);
    if (tile == 3) {
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x230 = 5;
        return false;
    }
    if (tile == 0xE) {
        if (field_0x230 != 7) {
            field_0x240[2] = 10;
            field_0x230 = 7;
        }
        return false;
    }
    if (tile == 0xD) {
        if (field_0x230 != 6) {
            field_0x240[2] = 10;
            field_0x230 = 6;
        }
        return false;
    }
    if (tile == 0 || tile == 0x27 || tile == 2) {
        return false;
    }
    if (tile == 0x28 && (s32)mPos.y % 16 > 8) {
        return false;
    }
    if (tile == 0x29 && (s32)mPos.y % 16 < 8) {
        return false;
    }
    if (tile == 0x2C && (s32)mPos.y % 16 > 8) {
        return false;
    }
    if (tile == 0x2D && (s32)mPos.y % 16 < 8) {
        return false;
    }
    if (tile == 0x2A && (s32)mPos.x % 16 > 8) {
        return false;
    }
    if (tile == 0x2B && (s32)mPos.x % 16 < 8) {
        return false;
    }
    if (tile == 0x2E && (s32)mPos.x % 16 > 8) {
        return false;
    }
    if (tile == 0x2F && (s32)mPos.x % 16 < 8) {
        return false;
    }
    return true;
}

FleeingForceGem_804AB170::~FleeingForceGem_804AB170() {}
