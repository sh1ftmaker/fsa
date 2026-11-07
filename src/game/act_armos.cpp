#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/Player.h"
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

// The box of the actor around mPos.
static inline JGeometry::TBox2f boxAround(const JGeometry::TBox2f& box, const JGeometry::TVec3f& pos) {
    JGeometry::TBox2f out = box;
    out.i.x += pos.x;
    out.i.y += pos.y;
    out.f.x += pos.x;
    out.f.y += pos.y;
    return out;
}

// FIXME(shotgun): 36.03 %, missing/extra + reloc-name (partial: states 3 tail, 4, 5 not written)
// FIXME(shotgun): state 3 from the branch at 0x80141938 (moving, grid-snapping step), states 4 and 5
// (0x80141FF0, 0x80142024) are not written yet.
void Armos_8049F260::update() {
    JGeometry::TBox2f box = boxAround(mBox, mPos);
    if (fn_801F3D94(true) != 0) {
        fn_801F2618(NULL, false);
        return;
    }
    if (field_0x2f8 == 2 || field_0x2f8 == 5) {
        if (field_0x94 == 3 || field_0x94 == 5) {
            for (s32 i = 0; i < 4; i++) {
                s32 player = fn_801F285C(NULL, NULL, false, i);
                if (player < 0) {
                    continue;
                }
                JGeometry::TVec2f mid(0.5f * (box.i.x - box.f.x) + box.f.x, 0.5f * (box.i.y - box.f.y) + box.f.y);
                if (fn_80237948(player, &mid)) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x400D1, 0);
                }
            }
        } else if (fn_801F305C()) {
            if (field_0x2f8 != 2) {
                field_0x318 = 0;
                field_0x2f8 = 2;
            }
            if (mLife <= 0) {
                startSound_801F0E34(0x400A1, 0);
            }
            fn_801F2618(NULL, false);
            return;
        }
    }
    switch (field_0x2f8) {
    case 0:
        fn_80239914(&box, field_0x4, mSerial);
        fn_801EF510();
        if (field_0x94 == 2) {
            if (vfunc_0x34()) {
                startSound_801F0E34(0x400A0, 0);
                if (field_0x94 != 3) {
                    field_0xbc = 2;
                }
                field_0x2f8 = 1;
                field_0x2fc = 0x3C;
            }
            return;
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                const JGeometry::TVec3f* playerPos = getPlayerPos_8023E724(i);
                JGeometry::TVec3f d(playerPos->x - mPos.x, playerPos->y - mPos.y, playerPos->z - mPos.z);
                f32 threshold = 4096.0f;
                if (field_0x94 == 3) {
                    threshold = 784.0f;
                }
                if (d.x * d.x + d.y * d.y + d.z * d.z < threshold) {
                    startSound_801F0E34(0x400A0, 0);
                    if (field_0x94 != 3) {
                        field_0xbc = 2;
                    }
                    field_0x2f8 = 1;
                    field_0x2fc = 0x3C;
                    return;
                }
            }
        }
        break;
    case 1: {
        field_0x2fc--;
        JGeometry::TBox2f moved = boxAround(mBox, mPos);
        fn_80239914(&moved, field_0x4, mSerial);
        if (!(field_0x2fc & 2)) {
            field_0x318 = field_0x318 ^ 1;
        }
        if (field_0x2fc < 0) {
            field_0xb8 = 1;
            if (field_0x94 != 3) {
                field_0xb0 = 0x12205;
            }
            field_0x318 = 0;
            field_0x2f8 = 2;
        }
        break;
    }
    case 2:
        if (field_0x300 > 0) {
            field_0x300--;
        }
        field_0x3c.z += -0.2f;
        mPos += field_0x3c;
        if (mPos.z <= 0.0f) {
            JGeometry::TVec3f p(mPos.x, mPos.y + 8.0f, mPos.z);
            if (field_0x304 == 0) {
                if (field_0x94 == 3 || field_0x94 == 4) {
                    spawnEmitter_8013CC50(field_0x4, &p, 0x3CF, 0, 0, NULL, NULL, true, 2.0f);
                } else {
                    spawnEmitter_8013CC50(field_0x4, &p, 0x3CF, 0, 0, NULL, NULL, true, 1.0f);
                }
                startSound_801F0E34(0x40007, 0);
            }
            field_0x304 = 1;
            mPos.z = 0.0f;
            if (field_0x94 == 3 || field_0x94 == 4) {
                s32 outDir = -1;
                s32 outPlayer = -1;
                fn_801EEC98(mSerial, NULL, &outDir, NULL, &outPlayer, 0, 0);
                bool away = false;
                if (outPlayer < 0) {
                    away = true;
                } else if (outDir != fn_80230CFC(outPlayer)) {
                    away = true;
                }
                if (away == true) {
                    field_0x304 = 0;
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                    JGeometry::TBox2f moved = boxAround(mBox, mPos);
                    fn_80239914(&moved, field_0x4, mSerial);
                    fn_801EF510();
                    fn_801F2B7C(-100, -1, 1);
                    fn_801F2618(NULL, false);
                    return;
                }
            }
            JGeometry::TVec3f dir = p;
            fn_801EE8F4(&dir, NULL, 0);
            dir.x *= 0.5f;
            dir.y *= 0.5f;
            dir.z *= 0.5f;
            field_0x3c.x = dir.x;
            field_0x3c.y = dir.y;
            field_0x3c.z = 2.0f;
        }
        fn_801F2B7C(-100, -1, 1);
        fn_801F2618(NULL, false);
        break;
    case 3: {
        fn_8020D58C()->addBox(mSerial);
        fn_801EF510();
        if (field_0x94 == 6 || field_0x94 == 7) {
            JGeometry::TBox2f moved = boxAround(mBox, mPos);
            fn_80239914(&moved, field_0x4, mSerial);
            return;
        }
        if (PSVECDistance(&mPos, &field_0x320) >= 15.9f) {
            mPos.x = (f32)((((s32)mPos.x + 4) / 8) * 8);
            mPos.y = (f32)((((s32)mPos.y + 4) / 8) * 8);
            field_0x320.x = mPos.x;
            field_0x320.y = mPos.y;
            field_0x320.z = mPos.z;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            JGeometry::TBox2f moved = boxAround(mBox, mPos);
            fn_80239914(&moved, field_0x4, mSerial);
            mPrevPos = mPos;
            field_0x300 = 8;
            return;
        }
        // FIXME(shotgun): the branch at 0x80141938 is not written.
        break;
    }
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
