#include "game/GameAudio.h"
#include <math.h>
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_pengator.h"
#include "game/tu_80212AC0.h"

// .data tables indexed by state * 4 + field_0x240 (update's state 2 and 3).
static s32 lbl_804A1808[16] = {0, 1, 0, 2, 3, 4, 3, 5, 6, 7, 8, 7, 10, 9, 11, 9};
static s32 lbl_804A1848[16] = {4, 9, 4, 9, 4, 9, 4, 9, 9, 4, 9, 4, 9, 4, 9, 4};

Pengator_804A1888::~Pengator_804A1888() {}

// The hit reaction update() runs after fn_801F3D94 or fn_801F305C reports a
// hit; the original has this code twice (inlined at both checks).
static inline void hitReaction(Pengator_804A1888* self) {
    self->field_0xbc = 2;
    if (self->mPos.z > 0.0f) {
        self->field_0x3c.z -= 1.0f;
    } else {
        self->mPos.z = 0.0f;
        self->field_0x3c.z = 0.0f;
    }
    self->fn_801F2618(NULL, false);
}

void Pengator_804A1888::draw() {
    fn_801F06F0("enemy_gc_sob.bin", field_0x24c + 0x145, NULL, 7, 1.0f, 1.0f);
}

// FIXME: 93.01 %. What differs: (1) the (field_0x98 & 2) test's branch layout
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
    f32 outDist = 0.0f;
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

    fn_801F71A4(&field_0x3c.x, field_0x258, 0.1f, field_0x250);
    fn_801F71A4(&field_0x3c.y, field_0x25c, 0.1f, field_0x250);
    mPos += field_0x3c;
    fn_80212AC0()->fn_80212AF0(&mPos);
    fn_801F2B7C(-100, -1, 1);
    fn_801EEC98(mSerial, &outAngle, &outDir, &outDist, &outPlayer, 0, 0);
    bool moved = fn_801F2618(NULL, false);

    box.addPos(pos);
    center.x = box.f.x + 0.5f * (box.i.x - box.f.x);
    center.y = box.f.y + 0.5f * (box.i.y - box.f.y);
    u16 tile = fn_802265FC((const JGeometry::TVec3f*)&center, field_0x4);

    field_0x250 = 1.0f;
    if (tile == 0x50) {
        field_0x250 = 0.01f;
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
                near = FABS(fn_8023E724(outPlayer)->y - mPos.y) < 24.0f;
            } else {
                near = FABS(fn_8023E724(outPlayer)->x - mPos.x) < 24.0f;
            }
            if (near && !fn_801F23BC(field_0x4, &mPos, fn_8023E724(outPlayer), &field_0x80, NULL, NULL, false)) {
                field_0x230 = 2;
                break;
            }
        }
        if (moved || field_0x238 == 0) {
            if (moved || tile != 0x50) {
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
            }
            field_0x238 = (s32)(100.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 0x32;
            bool flag = moved;
            if (!moved) {
                if (lbl_8053AAF8->mRandom.get_ufloat_1() >= 0.7f) {
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
                    field_0x98 = (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.5f) ? 0 : 1;
                } else {
                    field_0x98 = (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.5f) ? 2 : 3;
                }
            }
        }
        switch (field_0x98) {
        case 0:
            field_0x258 = 0.7f;
            field_0x25c = 0.0f;
            field_0x260 = 0.0f;
            break;
        case 1:
            field_0x258 = -0.7f;
            field_0x25c = 0.0f;
            field_0x260 = 0.0f;
            break;
        case 2:
            field_0x258 = 0.0f;
            field_0x25c = 0.7f;
            field_0x260 = 0.0f;
            break;
        case 3:
            field_0x258 = 0.0f;
            field_0x25c = -0.7f;
            field_0x260 = 0.0f;
            break;
        }
        if (field_0x234 == 0) {
            field_0x24c = lbl_804A1808[field_0x98 * 4 + field_0x240];
            field_0x234 = lbl_804A1848[field_0x98 * 4 + field_0x240];
            field_0x240 = (field_0x240 + 1) & 3;
        }
        break;
    case 2:
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x260 = 0.0f;
        field_0x25c = 0.0f;
        field_0x258 = 0.0f;
        field_0x234 = 0;
        field_0x240 = 0;
        field_0x24c = field_0x98 + 0xc;
        fn_801F0E34(0x400AB, 0);
        field_0x254 = 5.0f;
        if (tile != 0x50) {
            field_0x254 = 3.0f;
        }
        switch (field_0x98) {
        case 0:
            field_0x258 = field_0x254;
            field_0x25c = 0.0f;
            field_0x260 = 0.0f;
            field_0x3c.x = 0.3f * field_0x254;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 5.0f;
            break;
        case 1:
            field_0x258 = -field_0x254;
            field_0x25c = 0.0f;
            field_0x260 = 0.0f;
            field_0x3c.x = -(0.3f * field_0x254);
            field_0x3c.y = 0.0f;
            field_0x3c.z = 5.0f;
            break;
        case 2:
            field_0x258 = 0.0f;
            field_0x25c = field_0x254;
            field_0x260 = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.3f * field_0x254;
            field_0x3c.z = 5.0f;
            break;
        case 3:
            field_0x258 = 0.0f;
            field_0x25c = -field_0x254;
            field_0x260 = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x3c.y = -(0.3f * field_0x254);
            field_0x3c.z = 5.0f;
            break;
        }
        field_0xbc = 3;
        field_0x230 = 3;
        break;
    case 3: {
        f32 rate = 0.05f;
        if (tile != 0x50) {
            rate = 0.1f;
        }
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        fn_801F71A4(&field_0x258, 0.0f, 0.1f, rate);
        fn_801F71A4(&field_0x25c, 0.0f, 0.1f, rate);
        if (mPos.z <= 0.0f) {
            break;
        }
        field_0x3c.z = 0.0f;
        if (!moved) {
            if (FABS(field_0x3c.x) >= 0.1f || field_0x258 >= 0.1f ||
                FABS(field_0x3c.y) >= 0.1f || field_0x25c >= 0.1f) {
                break;
            }
        }
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x23c = (s32)(100.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 0x32;
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

// FIXME: 99.00 %. Regalloc only: the zero literal of the 0x234-0x248 stores
// is in r7 in the original and r8 here (it swaps with the lbl_8053AAF8
// pointer loaded after it).
void Pengator_804A1888::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x98 = (s32)(3.99f * lbl_8053AAF8->mRandom.get_ufloat_1());
    field_0x24c = field_0x98 << 2;
    field_0x260 = 0.0f;
    field_0x25c = 0.0f;
    field_0x258 = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0xb0 = 0x11101;
    field_0xb8 = 1;
    field_0x195 = 1;
    field_0x254 = 0.0f;
    field_0x250 = 1.0f;
    field_0x230 = 0;
}
