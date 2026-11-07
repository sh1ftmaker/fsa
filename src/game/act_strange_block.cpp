#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_strange_block.h"

// The push step of each direction (x, y, z), .rodata at 0x80471E28 (the rest of that object is
// string literals of update/draw).
static const f32 lbl_80471E28[12] = {20.0f, 0.0f, 0.0f, -20.0f, 0.0f, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f, -20.0f, 0.0f};

// .data at 0x804ABC80 (16 bytes, local to this unit).
extern const s32 lbl_804ABC80[4];

// 0x8033E188.
// FIXME(shotgun): 94.71 %, missing/extra + reloc-name
void StrangeBlock_804AC2D8::init() {
    field_0x25c = (field_0x90 >> 12) & 0xF;
    switch (field_0x25c) {
    case 0:
    case 1:
    case 5:
        mPos.add(JGeometry::TVec3f(0.0f, 8.0f, 0.0f));
        mBox.set(-15.0f, -23.0f, 15.0f, 7.0f);
        field_0x80.set(-15.0f, -23.0f, 15.0f, 7.0f);
        field_0x1a4 = 2;
        field_0x260 = 2;
        break;
    case 2:
    case 3:
    case 6:
        mPos.add(JGeometry::TVec3f(0.0f, 16.0f, 0.0f));
        mBox.set(-31.0f, -47.0f, 31.0f, 15.0f);
        field_0x80.set(-31.0f, -47.0f, 31.0f, 15.0f);
        field_0x1a4 = 4;
        field_0x260 = 4;
        break;
    case 4:
        mBox.set(-31.0f, -31.0f, 31.0f, 31.0f);
        field_0x80.set(-31.0f, -31.0f, 31.0f, 31.0f);
        field_0x1a4 = 4;
        field_0x260 = 4;
        break;
    }
    mSpawnPos = mPos;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x244 = 0;
    field_0x248 = 0;
    field_0xb8 = 2;
    field_0x234 = 0;
    field_0x250 = 0;
    field_0x24c = lbl_804ABC80[0];
    field_0x254 = 2;
    field_0x258 = -1;
    field_0x272 = 0;
    field_0x268 = (field_0x90 & 0xFFF) + 0x12B;
    field_0x265 = 0;
    field_0x264 = (field_0x90 >> 17) & 1;
    if (field_0x264 == 0) {
        if (fn_8028D4DC(&mPos, field_0x260 - 1, field_0x16c)) {
            field_0x265 = 1;
            vfunc_0x30(0xFF, true);
        }
    }
    field_0x238[0] = mPos.x;
    field_0x238[1] = mPos.y;
    field_0x238[2] = mPos.z;
    field_0x270 = 1;
    if (field_0x25c == 0 || field_0x25c == 2) {
        field_0x270 = 0;
    }
    field_0x26c = 0;
    field_0x271 = 0;
    field_0x230 = 0;
}

// 0x8033D4AC.
// FIXME(shotgun): 98.12 %, regalloc-only + reloc-name
f32 fn_8033D4AC(f32 value, f32 center, f32 margin) {
    f32 offset = 0.1f + margin;
    if (value < center) {
        return center - offset;
    }
    return center + offset;
}

// 0x8033D264.
// FIXME(shotgun): 82.31 %, missing/extra + reloc-name
bool StrangeBlock_804AC2D8::fn_8033D264(u8 dir) {
    bool movable = false;
    JGeometry::TVec3f savedPos = mPos;
    field_0x3c.x = lbl_80471E28[dir * 3];
    field_0x3c.y = lbl_80471E28[dir * 3 + 1];
    field_0x3c.z = lbl_80471E28[dir * 3 + 2];
    mPos.add(field_0x3c);
    JGeometry::TVec3f out(0.0f, 0.0f, 0.0f);
    if (fn_801F2618(&out, false)) {
        switch (dir) {
        case 0:
            if (out.x >= 0.0f) {
                movable = true;
            }
            break;
        case 1:
            if (out.x <= 0.0f) {
                movable = true;
            }
            break;
        case 2:
            if (out.y >= 0.0f) {
                movable = true;
            }
            break;
        case 3:
            if (out.y <= 0.0f) {
                movable = true;
            }
            break;
        }
    } else {
        movable = true;
    }
    if (!movable) {
        movable = true;
        JGeometry::TBox2f box(mBox);
        box.addPos(mPos.x, mPos.y);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
                if (fn_8022CAF8(i) != 5 || !fn_8022F478(i)) {
                    if (fn_80238CB4(i) != mSerial) {
                        movable = false;
                    }
                }
            }
        }
    }
    mPos = savedPos;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    return movable;
}

// Destructor at 0x8033CCE4.
StrangeBlock_804AC2D8::~StrangeBlock_804AC2D8() {}
