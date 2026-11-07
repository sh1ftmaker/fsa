#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_armos.h"

// FIXME(shotgun): 99.45 %, reloc-name
void Armos_8049F260::init() {
    mBox.i.set(-7.0f, -7.0f);
    mBox.f.set(7.0f, 7.0f);
    mBox2.i.set(-12.0f, -16.0f);
    mBox2.f.set(12.0f, 12.0f);
    field_0x80.i.set(-7.0f, -7.0f);
    field_0x80.f.set(7.0f, 7.0f);
    field_0x300 = 0;
    field_0x30c = 0;
    field_0xb0 = 0xA040004;
    field_0x310 = -1;
    field_0x314 = -1;
    field_0x31c = (field_0x90 >> 4) & 1;
    if (field_0x31c != 0) {
        mBox.i.set(-12.0f, -12.0f);
        mBox.f.set(12.0f, 12.0f);
        field_0xc0 = 3;
    }
    field_0x94 = field_0x90 & 0xF;
    field_0x308 = (field_0x90 >> 8) & 0xFF;
    field_0x318 = 1;
    field_0x304 = 0;
    field_0xbc = 3;
    if (field_0x94 == 3 || field_0x94 == 4 || field_0x94 == 5 || field_0x94 == 7) {
        field_0xc0 = 3;
        mBox.i.set(-14.0f, -20.0f);
        mBox.f.set(14.0f, 6.0f);
        field_0x80.i.set(-14.0f, -20.0f);
        field_0x80.f.set(14.0f, 6.0f);
        mBox2.i.set(-16.0f, -22.0f);
        mBox2.f.set(16.0f, 8.0f);
    }
    field_0x2f8 = 3;
    switch (field_0x94) {
    case 1:
    case 4:
    case 6:
    case 7:
        field_0x196 = 1;
        field_0x1a8 = 0;
        break;
    case 5:
        field_0x196 = 1;
        field_0x1a8 = 0;
        field_0xb8 = 1;
        field_0x318 = 0;
        field_0x2f8 = 4;
        break;
    case 3:
        field_0x196 = 1;
        // fall through
    default:
        field_0xb8 = 1;
        field_0x2f8 = 0;
        break;
    }
}

// FIXME(shotgun): 92.22 %, missing/extra + reloc-name
void Armos_8049F260::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x20 = 0;
    s32 flags = 0;
    s32 mode = 0;
    if (field_0x31c != 0) {
        mode = 1;
        flags = 3;
    }
    mtx[0][0] = 1.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.0f;
    params.mMtx = mtx;
    if (field_0x94 == 3 || field_0x94 == 4 || field_0x94 == 5 || field_0x94 == 7) {
        fn_801F02BC("enemy_gc_sob.bin", field_0x318 + 0x444, &params, 3, mode, false, 1.0f, 1.0f);
    } else {
        fn_801F02BC("enemy_both2_sob.bin", field_0x318 + 0x152, &params, flags, mode, false, 1.0f, 1.0f);
    }
}

Armos_8049F260::~Armos_8049F260() {}

// update() (0x80141064, 0x11C0 bytes): not written (size).
