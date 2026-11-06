#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_blade_trap.h"
#include "std/math.h"

// Offset of the wall spark from mPos per direction (field_0x98: +x, -x, +y, -y).
static const f32 lbl_80466288[4][2] = {
    {8.0f, 0.0f},
    {-8.0f, 0.0f},
    {0.0f, 8.0f},
    {0.0f, -8.0f},
};

// Actors vfunc_0x4C's collision box test ignores.
static const u32 lbl_804662A8[5] = {'TOMI', 'IPOT', 'IBBM', 'BMST', 'NULL'};

inline void BladeTrap_804A04D8::setMoveSpeed() {
    switch (field_0x98) {
    case 0:
        field_0x268.set(3.0f, 0.0f, 0.0f);
        break;
    case 1:
        field_0x268.set(-3.0f, 0.0f, 0.0f);
        break;
    case 2:
        field_0x268.set(0.0f, 3.0f, 0.0f);
        break;
    case 3:
        field_0x268.set(0.0f, -3.0f, 0.0f);
        break;
    }
}

void BladeTrap_804A04D8::init() {
    mBox.i.x = -6.0f;
    mBox.i.y = -6.0f;
    mBox.f.x = 6.0f;
    mBox.f.y = 6.0f;
    field_0x80.i.x = -3.0f;
    field_0x80.i.y = -3.0f;
    field_0x80.f.x = 3.0f;
    field_0x80.f.y = 3.0f;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x268.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x254 = (field_0x90 >> 4) & 1;
    if (field_0x254) {
        mBox.i.x = -12.0f;
        mBox.i.y = -12.0f;
        mBox.f.x = 12.0f;
        mBox.f.y = 12.0f;
    }
    field_0x98 = field_0x90 & 3;
    field_0x258 = (field_0x90 >> 8) & 0xF;
    field_0x25c = (field_0x90 >> 12) & 0xF;
    field_0x260 = 0;
    if (field_0x258) {
        field_0x260 = (s32)(3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    }
    field_0x274.set(mPos);
    field_0x264 = 0;
    if (field_0x90 & 4) {
        field_0x264 = 1;
    }
    field_0xb8 = 2;
    field_0x230 = 0;
    if (field_0x25c) {
        field_0x230 = 3;
    }
}

void BladeTrap_804A04D8::update() {
    JGeometry::TVec3f vel(field_0x3c);
    // Copied and never read again; the original stores this copy too.
    JGeometry::TVec3f prevPos(mPos);
    mLife = 1000;
    for (s32 i = 0; i < 4; i++) {
        s32 player = fn_801F285C(NULL, NULL, false, i);
        if (player < 0) {
            continue;
        }
        JGeometry::TVec3f pos(mPos);
        JGeometry::TBox2f box(mBox);
        box.addPos(pos.x, pos.y);
        JGeometry::TVec2f center;
        center.x = box.f.x + 0.5f * (box.i.x - box.f.x);
        center.y = box.f.y + 0.5f * (box.i.y - box.f.y);
        if (fn_80237948(player, &center)) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x400D1, 0);
        }
        field_0x3c.set(vel);
    }

    addCalc2_801F71A4(&field_0x3c.x, field_0x268.x, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x268.y, 0.3f, 1.0f);
    mPos += field_0x3c;
    fn_801F2B7C(-100, -1, 1);

    if (field_0x258 && field_0x238 == 0) {
        field_0x260++;
        field_0x260 &= 3;
        field_0x238 = 10;
    }

    u8 hit = fn_801F2618(NULL, false);
    s32 player = -1;
    s32 dir;
    fn_801EEC98(mSerial, NULL, &dir, NULL, &player, 0, 0);

    s32 state = field_0x230;
    if (state != 2 && state != 3 && state != 0 && (field_0x9c >= 0 || (state == 1 && hit))) {
        JGeometry::TVec3f pos(mPos);
        pos.x += lbl_80466288[field_0x98][0];
        pos.y += lbl_80466288[field_0x98][1];
        if (field_0x23c == 0) {
            spawnEmitter_8013CC50(field_0x4, &pos, 0x4A1, 0, 1, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30014, 0);
            field_0x23c = 4;
        }
        field_0x268.set(0.0f, 0.0f, 0.0f);
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        field_0x230 = 2;
        return;
    }

    switch (state) {
    case 0:
        if (field_0x234 == 0 && player >= 0) {
            f32 dx;
            f32 dy;
            if (getPlayerLayer_8023DE58(player) < 8 && field_0x4 == 8) {
                s32 layer = getPlayerLayer_8023DE58(player);
                GameView_804A9950* view = getGameView_802D6A9C();
                const JGeometry::TBox2f* src = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
                JGeometry::TBox2f box;
                box = *src;
                dx = fabs(mPos.x - (getPlayerPos_8023E724(player)->x - box.i.x));
                dy = fabs(mPos.y - (getPlayerPos_8023E724(player)->y - box.i.y));
            } else {
                dx = fabs(mPos.x - getPlayerPos_8023E724(player)->x);
                dy = fabs(mPos.y - getPlayerPos_8023E724(player)->y);
            }
            if (field_0x264 == 1) {
                if ((!(dir & 2) && dy < 22.0f) || ((dir & 2) && dx < 22.0f)) {
                    field_0x98 = dir;
                }
            }
            if ((!(field_0x98 & 2) && dy < 22.0f) || ((field_0x98 & 2) && dx < 22.0f)) {
                setMoveSpeed();
                startSound_801F0E34(0x30013, 0);
                field_0x230 = 1;
            }
        }
        break;
    case 2:
        if (!hit) {
            addCalc2_801F71A4(&mPos.x, field_0x274.x, 0.3f, 0.5f);
            addCalc2_801F71A4(&mPos.y, field_0x274.y, 0.3f, 0.5f);
            f32 dx = mPos.x - field_0x274.x;
            f32 dy = mPos.y - field_0x274.y;
            if (std::sqrtf(dx * dx + dy * dy) < 1.0f || hit) {
                field_0x234 = 10;
                if (hit) {
                    field_0x274.set(mPos);
                }
                mPos.set(field_0x274);
                field_0x230 = 0;
            }
        }
        break;
    case 3:
        if (field_0x9c >= 0 || hit) {
            JGeometry::TVec3f pos(mPos);
            pos.x += lbl_80466288[field_0x98][0];
            pos.y += lbl_80466288[field_0x98][1];
            if (field_0x23c == 0) {
                spawnEmitter_8013CC50(field_0x4, &pos, 0x4A1, 0, 1, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x30014, 0);
                field_0x23c = 4;
            }
            field_0x98 ^= 1;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
        }
        setMoveSpeed();
        break;
    }

    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
}

void BladeTrap_804A04D8::draw() {
    SpriteParams_801F5AE0 params;
    s32 mode = 0;
    if (field_0x254) {
        mode = 1;
    }
    if (field_0x258 == 0) {
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, 0x154, &params, 1.0f, 1.0f);
        }
        fn_801F02BC("enemy_both2_sob.bin", 0x154, &params, 0, mode, false, 1.0f, 1.0f);
    } else {
        fn_801F02BC("enemy_gc_sob.bin", field_0x260 + 0x17A, &params, 0, mode, false, 1.0f, 1.0f);
    }
}

bool BladeTrap_804A04D8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, lbl_804662A8) == true) {
        return true;
    }
    if (field_0x24c == 0) {
        s32 tile = fn_8022461C(pos, layer);
        if (tile != 0 && tile != 4 && tile != 5 && tile != 6 && tile != 7 && tile != 2 && tile != 3 &&
            tile != 0x50 && tile != 0xE && tile != 0xC && tile != 0x27 && tile != 0x28 && tile != 0x29 &&
            tile != 0x2A && tile != 0x2B && tile != 0x2C && tile != 0x2D && tile != 0x2E && tile != 0x2F &&
            tile != 0x20 && tile != 0xD && tile != 0x39 && tile != 0x3A && tile != 0x3B && tile != 0x4C &&
            tile != 0x4D && tile != 0x4E && tile != 0x4F && tile != 0x24) {
            if (tile == 0x10 || tile == 0x11 || tile == 0x12 || tile == 0x13) {
                field_0x24c = 120;
                return false;
            }
            if (tile == 0x25) {
                return (u8)!getGameView_802D6A9C()->field_0x171;
            }
            if (tile == 0x26) {
                return (u8)(getGameView_802D6A9C()->field_0x171 == 1);
            }
            if (tile != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
                return true;
            }
        }
    } else {
        field_0x24c--;
    }
    return false;
}

BladeTrap_804A04D8::~BladeTrap_804A04D8() {}
