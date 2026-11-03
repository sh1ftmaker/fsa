#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_deadrock.h"

// .sdata2 floats: 1.0f (the draw's scale and the random offset), the random
// scale, half, zero, -1 and 50. Arrays, so the names survive into the object.
static const f32 lbl_8053D020[1] = {1.0f};
static const f32 lbl_8053D024[1] = {0.5f};
static const f32 lbl_8053D028[1] = {0.0f};
static const f32 lbl_8053D02C[1] = {-1.0f};
static const f32 lbl_8053D030[1] = {50.0f};
static const f32 lbl_8053D034[1] = {3.99f};
static const f32 lbl_8053D038[1] = {-8.0f};
static const f32 lbl_8053D03C[1] = {8.0f};

// .rodata string, the "SOB " bank draw() shows.
static const char lbl_80465BA8[] = "enemy_gc_sob.bin";

// .rodata: the two-word tags of the hit checks in update().
static const u32 lbl_80465B98[4] = {0x5357424D, 0x5448434C, 0x47524153, 0x4E554C4C};

// TRandom_fast_::get_ufloat_1 in [0, 1), with its 1.0f read from the named
// constant lbl_8053D020 as the original does (the header's `- 1` is an
// anonymous literal in every unit that inlines it).
static inline f32 randomUfloat(JMath::TRandom_fast_& random) {
    union {
        f32 f;
        u32 s;
    } out;
    out.s = (random.get() >> 9) | 0x3f800000;
    return out.f - lbl_8053D020[0];
}

Deadrock_8049F498::~Deadrock_8049F498() {}

void Deadrock_8049F498::init() {
    mBox.i.x = lbl_8053D038[0];
    mBox.i.y = lbl_8053D038[0];
    mBox.f.x = lbl_8053D03C[0];
    mBox.f.y = lbl_8053D03C[0];
    field_0x80.i.x = lbl_8053D038[0];
    field_0x80.i.y = lbl_8053D038[0];
    field_0x80.f.x = lbl_8053D03C[0];
    field_0x80.f.y = lbl_8053D03C[0];
    field_0x98 = (s32)(lbl_8053D034[0] * randomUfloat(lbl_8053AAF8->mRandom)) & 3;
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

void Deadrock_8049F498::draw() {
    s32 frame = field_0x244 + field_0x98 * 2;
    if (field_0x230 == 3 || field_0x230 == 4) {
        frame = 8;
    }
    if (field_0x230 == 4 && (field_0x23c & 1) == 0) {
        return;
    }
    fn_801F06F0(lbl_80465BA8, frame + 0x1c, NULL, 0, lbl_8053D020[0], lbl_8053D020[0]);
}

void Deadrock_8049F498::update() {
    JGeometry::TBox2f box(mBox);
    u32 list[4];
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
            // The box.f copies are locals to match the original's register
            // use in the centre sums.
            f32 fx = box.f.x;
            f32 fy = box.f.y;
            center.x = fx + lbl_8053D024[0] * (box.i.x - fx);
            center.y = fy + lbl_8053D024[0] * (box.i.y - fy);
            if (fn_802379E4(player, (const JGeometry::TVec3f*)&center)) {
                fn_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, lbl_8053D020[0]);
                fn_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, lbl_8053D020[0]);
                fn_801F0E34(0x400D1, 0);
            }
        }
    }

    if (fn_801F3D94(1) != 0) {
        field_0x3c.x = lbl_8053D028[0];
        field_0x3c.y = lbl_8053D028[0];
        field_0x3c.z = lbl_8053D028[0];
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
                field_0x3c.x = lbl_8053D028[0];
                field_0x3c.y = lbl_8053D028[0];
                field_0x3c.z = lbl_8053D028[0];
                if (field_0x250 <= 0) {
                    field_0x3c.z = lbl_8053D028[0];
                    field_0x3c.y = lbl_8053D028[0];
                    field_0x3c.x = lbl_8053D028[0];
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
        list[0] = lbl_80465B98[0];
        list[1] = lbl_80465B98[1];
        list[2] = lbl_80465B98[2];
        list[3] = lbl_80465B98[3];
        if (fn_801F2DB4(0x4E554C4C, 0x4E554C4C, list)) {
            if (oldLife == mLife) {
                field_0x3c.z = lbl_8053D028[0];
                field_0x3c.y = lbl_8053D028[0];
                field_0x3c.x = lbl_8053D028[0];
                field_0x230 = 2;
            }
            fn_801F2618(NULL, false);
            return;
        }
    } else {
        if (fn_801F2DB4(0x4E554C4C, 0x48424D42, NULL)) {
            fn_801F0E34(0x400D1, 0);
            field_0x3c.x = lbl_8053D028[0];
            field_0x3c.y = lbl_8053D028[0];
            field_0x3c.z = lbl_8053D028[0];
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
                field_0x3c.z = field_0x3c.z - lbl_8053D024[0];
            }
            if (mPos.z < lbl_8053D028[0] && field_0x3c.z < lbl_8053D028[0]) {
                field_0x3c.x = lbl_8053D028[0];
                field_0x3c.y = lbl_8053D028[0];
                field_0x3c.z = lbl_8053D028[0];
                mPos.z = lbl_8053D028[0];
                field_0x254 = 0;
            }
        }
        field_0x234 = 8;
        if (field_0x238 == 0 && field_0x254 == 0) {
            if ((field_0x98 & 2) == 0) {
                f32 u = randomUfloat(lbl_8053AAF8->mRandom);
                if (u < lbl_8053D024[0]) {
                    field_0x98 = 2;
                } else {
                    field_0x98 = 3;
                }
            } else {
                f32 u = randomUfloat(lbl_8053AAF8->mRandom);
                if (u < lbl_8053D024[0]) {
                    field_0x98 = 0;
                } else {
                    field_0x98 = 1;
                }
            }
            switch (field_0x98) {
            case 0:
                field_0x3c.x = lbl_8053D020[0];
                field_0x3c.y = lbl_8053D028[0];
                field_0x3c.z = lbl_8053D028[0];
                break;
            case 1:
                field_0x3c.x = lbl_8053D02C[0];
                field_0x3c.y = lbl_8053D028[0];
                field_0x3c.z = lbl_8053D028[0];
                break;
            case 2:
                field_0x3c.x = lbl_8053D028[0];
                field_0x3c.y = lbl_8053D020[0];
                field_0x3c.z = lbl_8053D028[0];
                break;
            case 3:
                field_0x3c.x = lbl_8053D028[0];
                field_0x3c.y = lbl_8053D02C[0];
                field_0x3c.z = lbl_8053D028[0];
                break;
            }
            field_0x234 = 0;
            f32 hold = randomUfloat(lbl_8053AAF8->mRandom);
            field_0x238 = (s32)(lbl_8053D030[0] * hold) + 50;
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
            field_0x238 = (s32)(lbl_8053D030[0] * randomUfloat(lbl_8053AAF8->mRandom)) + 50;
            field_0x3c.x = lbl_8053D028[0];
            field_0x3c.y = lbl_8053D028[0];
            field_0x3c.z = lbl_8053D028[0];
            field_0x230 = 0;
        }
        break;
    case 2:
        field_0x238 = 200;
        field_0xb0 = 0x0A042104;
        fn_801F0E34(0x400D1, 0);
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
            f32 u = randomUfloat(lbl_8053AAF8->mRandom);
            field_0x98 = (s32)(lbl_8053D034[0] * u) & 3;
            switch (field_0x98) {
            case 0:
                field_0x3c.x = lbl_8053D020[0];
                field_0x3c.y = lbl_8053D028[0];
                field_0x3c.z = lbl_8053D028[0];
                break;
            case 1:
                field_0x3c.x = lbl_8053D02C[0];
                field_0x3c.y = lbl_8053D028[0];
                field_0x3c.z = lbl_8053D028[0];
                break;
            case 2:
                field_0x3c.x = lbl_8053D028[0];
                field_0x3c.y = lbl_8053D020[0];
                field_0x3c.z = lbl_8053D028[0];
                break;
            case 3:
                field_0x3c.x = lbl_8053D028[0];
                field_0x3c.y = lbl_8053D02C[0];
                field_0x3c.z = lbl_8053D028[0];
                break;
            }
            field_0xb0 = 0x08042104;
            field_0x250 = 1;
            field_0x3c.x = lbl_8053D028[0];
            field_0x3c.y = lbl_8053D028[0];
            field_0x3c.z = lbl_8053D028[0];
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
