#include "game/GameAudio.h"
#include <math.h>
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_pengator.h"
#include "game/tu_80212AC0.h"

// .sdata2 floats (named so the names survive into the object, as in
// act_deadrock.cpp): 1.0f (the draw's scale, the random offset, the warp
// step), 0.01f (tile 0x50), 0.1f, 0.5f, 0.7f, -0.7f, 0.3f, 0.05f, 100, 24,
// 5, 3, zero, -8, 8 and the random scale.
static const f32 lbl_8053DF00[1] = {1.0f};
static const f32 lbl_8053DF04[1] = {0.0f};
static const f32 lbl_8053DF08[1] = {0.1f};
static const f32 lbl_8053DF0C[1] = {0.5f};
static const f32 lbl_8053DF10[1] = {0.01f};
static const f32 lbl_8053DF14[1] = {24.0f};
static const f32 lbl_8053DF18[1] = {100.0f};
static const f32 lbl_8053DF1C[1] = {0.7f};
static const f32 lbl_8053DF20[1] = {-0.7f};
static const f32 lbl_8053DF24[1] = {5.0f};
static const f32 lbl_8053DF28[1] = {3.0f};
static const f32 lbl_8053DF2C[1] = {0.3f};
static const f32 lbl_8053DF30[1] = {0.05f};
static const f32 lbl_8053DF34[1] = {-8.0f};
static const f32 lbl_8053DF38[1] = {8.0f};
static const f32 lbl_8053DF3C[1] = {3.99f};

// .data tables indexed by state * 4 + field_0x240 (update's state 2 and 3).
static s32 lbl_804A1808[16] = {0, 1, 0, 2, 3, 4, 3, 5, 6, 7, 8, 7, 10, 9, 11, 9};
static s32 lbl_804A1848[16] = {4, 9, 4, 9, 4, 9, 4, 9, 9, 4, 9, 4, 9, 4, 9, 4};

// .rodata string, the "SOB " bank draw() shows.
static const char lbl_80466EC0[] = "enemy_gc_sob.bin";

// TRandom_fast_::get_ufloat_1 in [0, 1), with its 1.0f read from the named
// constant lbl_8053DF00 as the original does (see act_deadrock.cpp).
static inline f32 randomUfloat(JMath::TRandom_fast_& random) {
    union {
        f32 f;
        u32 s;
    } out;
    out.s = (random.get() >> 9) | 0x3f800000;
    return out.f - lbl_8053DF00[0];
}

Pengator_804A1888::~Pengator_804A1888() {}

// The hit reaction update() runs after fn_801F3D94 or fn_801F305C reports a
// hit; the original has this code twice (inlined at both checks).
static inline void hitReaction(Pengator_804A1888* self) {
    self->field_0xbc = 2;
    if (self->mPos.z > lbl_8053DF04[0]) {
        self->field_0x3c.z -= lbl_8053DF00[0];
    } else {
        self->mPos.z = lbl_8053DF04[0];
        self->field_0x3c.z = lbl_8053DF04[0];
    }
    self->fn_801F2618(NULL, false);
}

// FIXME: 99.00 %. Regalloc only: the zero literal of the 0x234-0x248 stores
// is in r7 in the original and r8 here (it swaps with the lbl_8053AAF8
// pointer loaded after it).
void Pengator_804A1888::init() {
    mBox.i.x = lbl_8053DF34[0];
    mBox.i.y = lbl_8053DF34[0];
    mBox.f.x = lbl_8053DF38[0];
    mBox.f.y = lbl_8053DF38[0];
    field_0x80.i.x = lbl_8053DF34[0];
    field_0x80.i.y = lbl_8053DF34[0];
    field_0x80.f.x = lbl_8053DF38[0];
    field_0x80.f.y = lbl_8053DF38[0];
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x98 = (s32)(lbl_8053DF3C[0] * randomUfloat(lbl_8053AAF8->mRandom));
    field_0x24c = field_0x98 << 2;
    field_0x260 = lbl_8053DF04[0];
    field_0x25c = lbl_8053DF04[0];
    field_0x258 = lbl_8053DF04[0];
    field_0x3c.z = lbl_8053DF04[0];
    field_0x3c.y = lbl_8053DF04[0];
    field_0x3c.x = lbl_8053DF04[0];
    field_0xb0 = 0x11101;
    field_0xb8 = 1;
    field_0x195 = 1;
    field_0x254 = lbl_8053DF04[0];
    field_0x250 = lbl_8053DF00[0];
    field_0x230 = 0;
}

// FIXME: 92.86 %. What differs: (1) the (field_0x98 & 2) test's branch layout
// (the original falls through into the y check, we branch to it), so the
// near-check blocks shuffle; (2) the case 2 velocity stores are ordered
// differently (0x254 reloaded, DF2C and DF24 hoisted above the stores); (3)
// the tail's field_0x23c decrement goes through `this + 8` in the original
// (addi r4, r29, 8; stw 0x234(r4)), which natural `-= 1` doesn't reproduce;
// (4) several register choices (lbl_8053AAF8 and the zero literal swap).
void Pengator_804A1888::update() {
    // Entry copies: the box and the position's x, y are read before the calls.
    JGeometry::TBox2f box(field_0x80);
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    u16 outAngle = 0;
    f32 outDist = lbl_8053DF04[0];
    s32 outPlayer = -1;
    s32 outDir = -1;
    JGeometry::TVec2f center;

    if (fn_801F3D94(1)) {
        hitReaction(this);
        return;
    }
    if (fn_801F305C()) {
        hitReaction(this);
        return;
    }

    fn_801F71A4(&field_0x3c.x, field_0x258, lbl_8053DF08[0], field_0x250);
    fn_801F71A4(&field_0x3c.y, field_0x25c, lbl_8053DF08[0], field_0x250);
    mPos += field_0x3c;
    fn_80212AC0()->fn_80212AF0(&mPos);
    fn_801F2B7C(-100, -1, 1);
    fn_801EEC98(mSerial, &outAngle, &outDir, &outDist, &outPlayer, 0, 0);
    bool moved = fn_801F2618(NULL, false);

    box.addPos(pos);
    center.x = box.f.x + lbl_8053DF0C[0] * (box.i.x - box.f.x);
    center.y = box.f.y + lbl_8053DF0C[0] * (box.i.y - box.f.y);
    u16 tile = fn_802265FC((const JGeometry::TVec3f*)&center, field_0x4);

    field_0x250 = lbl_8053DF00[0];
    if (tile == 0x50) {
        field_0x250 = lbl_8053DF10[0];
    }

    switch (field_0x230) {
    case 0:
        field_0x234 = 0;
        field_0x240 = 0;
        field_0xbc = 2;
        field_0x230 = 1;
        // fall through
    case 1:
        if (field_0x23c == 0 && field_0x98 == outDir) {
            bool near;
            if (!(field_0x98 & 2)) {
                near = FABS(fn_8023E724(outPlayer)->y - mPos.y) < lbl_8053DF14[0];
            } else {
                near = FABS(fn_8023E724(outPlayer)->x - mPos.x) < lbl_8053DF14[0];
            }
            if (near && !fn_801F23BC(field_0x4, &mPos, fn_8023E724(outPlayer), &field_0x80, NULL, NULL, false)) {
                field_0x230 = 2;
                break;
            }
        }
        if (moved || field_0x238 == 0) {
            if (moved || tile != 0x50) {
                field_0x3c.z = lbl_8053DF04[0];
                field_0x3c.y = lbl_8053DF04[0];
                field_0x3c.x = lbl_8053DF04[0];
            }
            field_0x238 = (s32)(lbl_8053DF18[0] * randomUfloat(lbl_8053AAF8->mRandom)) + 0x32;
            bool flag = moved;
            if (!moved) {
                if (randomUfloat(lbl_8053AAF8->mRandom) >= lbl_8053DF1C[0]) {
                    field_0x234 = 8;
                    flag = true;
                } else if (outDir != -1) {
                    if (field_0x98 == outDir) {
                        break;
                    }
                    field_0x98 = outDir;
                }
            }
            if (flag) {
                if (field_0x98 & 2) {
                    field_0x98 = (randomUfloat(lbl_8053AAF8->mRandom) < lbl_8053DF0C[0]) ? 0 : 1;
                } else {
                    field_0x98 = (randomUfloat(lbl_8053AAF8->mRandom) < lbl_8053DF0C[0]) ? 2 : 3;
                }
            }
        }
        switch (field_0x98) {
        case 0:
            field_0x258 = lbl_8053DF1C[0];
            field_0x25c = lbl_8053DF04[0];
            field_0x260 = lbl_8053DF04[0];
            break;
        case 1:
            field_0x258 = lbl_8053DF20[0];
            field_0x25c = lbl_8053DF04[0];
            field_0x260 = lbl_8053DF04[0];
            break;
        case 2:
            field_0x258 = lbl_8053DF04[0];
            field_0x25c = lbl_8053DF1C[0];
            field_0x260 = lbl_8053DF04[0];
            break;
        case 3:
            field_0x258 = lbl_8053DF04[0];
            field_0x25c = lbl_8053DF20[0];
            field_0x260 = lbl_8053DF04[0];
            break;
        }
        if (field_0x234 == 0) {
            field_0x24c = lbl_804A1808[field_0x98 * 4 + field_0x240];
            field_0x234 = lbl_804A1848[field_0x98 * 4 + field_0x240];
            field_0x240 = (field_0x240 + 1) & 3;
        }
        break;
    case 2:
        field_0x3c.z = lbl_8053DF04[0];
        field_0x3c.y = lbl_8053DF04[0];
        field_0x3c.x = lbl_8053DF04[0];
        field_0x260 = lbl_8053DF04[0];
        field_0x25c = lbl_8053DF04[0];
        field_0x258 = lbl_8053DF04[0];
        field_0x234 = 0;
        field_0x240 = 0;
        field_0x24c = field_0x98 + 0xc;
        fn_801F0E34(0x400AB, 0);
        field_0x254 = lbl_8053DF24[0];
        if (tile != 0x50) {
            field_0x254 = lbl_8053DF28[0];
        }
        switch (field_0x98) {
        case 0:
            field_0x258 = field_0x254;
            field_0x25c = lbl_8053DF04[0];
            field_0x260 = lbl_8053DF04[0];
            field_0x3c.x = lbl_8053DF2C[0] * field_0x254;
            field_0x3c.y = lbl_8053DF04[0];
            field_0x3c.z = lbl_8053DF24[0];
            break;
        case 1:
            field_0x258 = -field_0x254;
            field_0x25c = lbl_8053DF04[0];
            field_0x260 = lbl_8053DF04[0];
            field_0x3c.x = -(lbl_8053DF2C[0] * field_0x254);
            field_0x3c.y = lbl_8053DF04[0];
            field_0x3c.z = lbl_8053DF24[0];
            break;
        case 2:
            field_0x258 = lbl_8053DF04[0];
            field_0x25c = field_0x254;
            field_0x260 = lbl_8053DF04[0];
            field_0x3c.x = lbl_8053DF04[0];
            field_0x3c.y = lbl_8053DF2C[0] * field_0x254;
            field_0x3c.z = lbl_8053DF24[0];
            break;
        case 3:
            field_0x258 = lbl_8053DF04[0];
            field_0x25c = -field_0x254;
            field_0x260 = lbl_8053DF04[0];
            field_0x3c.x = lbl_8053DF04[0];
            field_0x3c.y = -(lbl_8053DF2C[0] * field_0x254);
            field_0x3c.z = lbl_8053DF24[0];
            break;
        }
        field_0xbc = 3;
        field_0x230 = 3;
        break;
    case 3: {
        f32 rate = lbl_8053DF30[0];
        if (tile != 0x50) {
            rate = lbl_8053DF08[0];
        }
        if (mPos.z > lbl_8053DF04[0]) {
            field_0x3c.z -= lbl_8053DF00[0];
        }
        fn_801F71A4(&field_0x258, lbl_8053DF04[0], lbl_8053DF08[0], rate);
        fn_801F71A4(&field_0x25c, lbl_8053DF04[0], lbl_8053DF08[0], rate);
        if (mPos.z <= lbl_8053DF04[0]) {
            break;
        }
        field_0x3c.z = lbl_8053DF04[0];
        if (!moved) {
            if (FABS(field_0x3c.x) >= lbl_8053DF08[0] || field_0x258 >= lbl_8053DF08[0] ||
                FABS(field_0x3c.y) >= lbl_8053DF08[0] || field_0x25c >= lbl_8053DF08[0]) {
                break;
            }
        }
        field_0x3c.z = lbl_8053DF04[0];
        field_0x3c.y = lbl_8053DF04[0];
        field_0x3c.x = lbl_8053DF04[0];
        field_0x23c = (s32)(lbl_8053DF18[0] * randomUfloat(lbl_8053AAF8->mRandom)) + 0x32;
        field_0x230 = 0;
        break;
    }
    }

    if (field_0x234 > 0) {
        field_0x234 -= 1;
    }
    if (field_0x238 > 0) {
        field_0x238 -= 1;
    }
    if (field_0x23c > 0) {
        field_0x23c -= 1;
    }
}

void Pengator_804A1888::vfunc_0x18() {
    fn_80212AC0()->fn_80212AF0(&mPos);
    ActorBase_804A5770::vfunc_0x18();
}

void Pengator_804A1888::draw() {
    fn_801F06F0(lbl_80466EC0, field_0x24c + 0x145, NULL, 7, lbl_8053DF00[0], lbl_8053DF00[0]);
}
