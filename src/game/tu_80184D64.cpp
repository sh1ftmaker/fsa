#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/tu_80184D64.h"

// 0x80186D24.
// FIXME(shotgun): 77.73 %, missing/extra + reloc-name
void cls_804A18E0::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(-10.0f, -10.0f, 10.0f, 10.0f);
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x98 = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x272 = fn_801F6874(field_0x98);
    field_0x264 = field_0x98 * 2 + field_0x24c;
    field_0x288[2] = 0.0f;
    field_0x288[1] = 0.0f;
    field_0x288[0] = 0.0f;
    field_0x384[0] = mPos.x;
    field_0x384[1] = mPos.y;
    field_0x384[2] = mPos.z;
    field_0x270 = -1;
    for (s32 i = 0; i < 5; i++) {
        field_0x294[i * 2] = mPos;
        field_0x30c[i * 2] = mPos;
        field_0x274[i * 2] = 0;
        field_0x294[i * 2 + 1] = mPos;
        field_0x30c[i * 2 + 1] = mPos;
        field_0x274[i * 2 + 1] = 0;
    }
    field_0x26c = 0.0f;
    field_0x268 = 3;
    field_0x260 = -1;
    field_0x118 = 4;
    field_0xb0 = 0xA040004;
    field_0x230 = 0;
}

// 0x80184DC4.
// FIXME(shotgun): 31.76 %, missing/extra
void cls_804A18E0::draw() {
    SpriteParams_801F5AE0 params;
    JGeometry::TVec3f savedPos = mPos;
    s32 start = 0;
    if (field_0x260 >= 0) {
        start = 1;
    }
    for (s32 i = start; i < 9; i++) {
        mPos = field_0x294[i];
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x13B, &params, 7, 1.0f, 1.0f);
    }
    mPos = savedPos;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x264 + 0x134, &params, 0, 1.0f, 1.0f);
}

// Destructor at 0x80184D64.
cls_804A18E0::~cls_804A18E0() {}
