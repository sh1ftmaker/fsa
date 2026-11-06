#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_deadrock.h"

void Deadrock_8049F498::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x98 = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 3;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    mLife = 4;
    field_0x250 = 1;
    field_0xb0 = 0x08042104;
    field_0x230 = 0;
}

void Deadrock_8049F498::update() {
    JGeometry::TBox2f box(mBox);
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    JGeometry::TVec2f center;
    box.addPos(pos);

    if (field_0x230 == 2 || field_0x230 == 3 || field_0x230 == 4) {
        fn_802397B4(&box, field_0x4, mSerial);
        for (s32 i = 0; i < 4; i++) {
            s32 player = fn_801F285C(NULL, NULL, false, i);
            if (player < 0) {
                continue;
            }
            // The original builds the centre on the stack as an 8-byte pair
            // and passes it as a TVec3f; the callee reads only x and y.
            center.x = box.f.x + 0.5f * (box.i.x - box.f.x);
            center.y = box.f.y + 0.5f * (box.i.y - box.f.y);
            if (fn_802379E4(player, (const JGeometry::TVec3f*)&center)) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x400D1, 0);
            }
        }
    }

    if (fn_801F3D94(1) != 0) {
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        fn_801F2618(NULL, false);
        return;
    }

    if (field_0x230 != 2 && field_0x230 != 3 && field_0x230 != 4) {
        field_0xb0 = 0x08042104;
        for (s32 i = 0; i < 4; i++) {
            if (fn_801F285C(NULL, NULL, true, i) >= 0 || fn_801A6108(mSerial, &box, &pos) ||
                fn_801A4D1C(mSerial, &box, &pos) || fn_801A46FC(mSerial, &box, &pos))
            {
                mLife = 4;
                field_0x250 = field_0x250 - 1;
                field_0x3c.x = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                if (field_0x250 <= 0) {
                    field_0x3c.z = 0.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.x = 0.0f;
                    field_0x230 = 2;
                }
                fn_801F2618(NULL, false);
                return;
            }
        }
        field_0xb0 = 0x08002100;
    }

    if (field_0x230 != 2 && field_0x230 != 3 && field_0x230 != 4) {
        s32 oldLife = mLife;
        u32 list[4] = {'SWBM', 'THCL', 'GRAS', 'NULL'};
        if (fn_801F2DB4('NULL', 'NULL', list)) {
            if (oldLife == mLife) {
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                field_0x230 = 2;
            }
            fn_801F2618(NULL, false);
            return;
        }
    } else {
        if (fn_801F2DB4('NULL', 'HBMB', NULL)) {
            startSound_801F0E34(0x400D1, 0);
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0xd8 = 0;
            field_0xdc = 0;
        }
    }

    if (field_0x234 > 0) {
        field_0x234 = field_0x234 - 1;
    }
    if (field_0x238 > 0) {
        field_0x238 = field_0x238 - 1;
    }
    if (field_0x23c > 0) {
        field_0x23c = field_0x23c - 1;
    }
    if (field_0x240 > 0) {
        field_0x240 = field_0x240 - 1;
    }

    mPos += field_0x3c;
    bool landed = fn_801F2618(NULL, false);
    if (field_0x230 != 2 && field_0x230 != 3 && field_0x230 != 4) {
        fn_801F2B7C(-100, -1, 1);
    }

    switch (field_0x230) {
    case 0:
        if (field_0x254 != 0) {
            if (field_0xc4 == 0) {
                field_0x3c.z = field_0x3c.z - 0.5f;
            }
            if (mPos.z < 0.0f && field_0x3c.z < 0.0f) {
                field_0x3c.x = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                mPos.z = 0.0f;
                field_0x254 = 0;
            }
        }
        field_0x234 = 8;
        if (field_0x238 == 0 && field_0x254 == 0) {
            if ((field_0x98 & 2) == 0) {
                f32 u = gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                if (u < 0.5f) {
                    field_0x98 = 2;
                } else {
                    field_0x98 = 3;
                }
            } else {
                f32 u = gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                if (u < 0.5f) {
                    field_0x98 = 0;
                } else {
                    field_0x98 = 1;
                }
            }
            switch (field_0x98) {
            case 0:
                field_0x3c.x = 1.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                break;
            case 1:
                field_0x3c.x = -1.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                break;
            case 2:
                field_0x3c.x = 0.0f;
                field_0x3c.y = 1.0f;
                field_0x3c.z = 0.0f;
                break;
            case 3:
                field_0x3c.x = 0.0f;
                field_0x3c.y = -1.0f;
                field_0x3c.z = 0.0f;
                break;
            }
            field_0x234 = 0;
            f32 hold = gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            field_0x238 = (s32)(50.0f * hold) + 50;
            field_0x230 = 1;
        }
        break;
    case 1:
        if (landed || field_0x238 == 0) {
            field_0x24c = 0;
            if (landed) {
                field_0x24c = 1;
            }
            field_0x234 = 8;
            field_0x238 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x230 = 0;
        }
        break;
    case 2:
        field_0x238 = 200;
        field_0xb0 = 0x0A042104;
        startSound_801F0E34(0x400D1, 0);
        field_0x230 = 3;
        // fall through
    case 3:
        if (field_0x238 == 0) {
            field_0x23c = 100;
            field_0x230 = 4;
        }
        break;
    case 4:
        if (field_0x23c == 0) {
            f32 u = gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            field_0x98 = (s32)(3.99f * u) & 3;
            switch (field_0x98) {
            case 0:
                field_0x3c.x = 1.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                break;
            case 1:
                field_0x3c.x = -1.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                break;
            case 2:
                field_0x3c.x = 0.0f;
                field_0x3c.y = 1.0f;
                field_0x3c.z = 0.0f;
                break;
            case 3:
                field_0x3c.x = 0.0f;
                field_0x3c.y = -1.0f;
                field_0x3c.z = 0.0f;
                break;
            }
            field_0xb0 = 0x08042104;
            field_0x250 = 1;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x230 = 0;
        }
        break;
    default:
        break;
    }

    if (field_0x234 <= 0) {
        field_0x234 = 8;
        field_0x244 = field_0x244 + 1;
        field_0x244 = field_0x244 & 1;
    }
}

// FIXME: draw 99.67 %, update 99.95 %. The original's .rodata has update's
// tag list (0x80465B98) before draw's "enemy_gc_sob.bin" (0x80465BA8); ours
// emits them in order of use, so both relocations point at swapped offsets.
void Deadrock_8049F498::draw() {
    s32 frame = field_0x244 + field_0x98 * 2;
    if (field_0x230 == 3 || field_0x230 == 4) {
        frame = 8;
    }
    if (field_0x230 == 4 && (field_0x23c & 1) == 0) {
        return;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", frame + 0x1c, NULL, 0, 1.0f, 1.0f);
}

Deadrock_8049F498::~Deadrock_8049F498() {}
