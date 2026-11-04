#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_recovery_heart.h"

// .sbss ints that only this unit's update() uses (no owner written yet).
extern s32 lbl_8053AB9C;
extern s32 lbl_8053ABEC;

RecoveryHeart_8049F4F0::~RecoveryHeart_8049F4F0() {}

// The ignore list is the multi-character tags 'TOMI' and 'NULL'.
// FIXME: 61.51 %. The original compares each id 0, 4, 5, 6, 7, 2, 3 with its own cmpwi;
// MWCC merges 2..7 in the || chain into one subi/cmplwi range check.
bool RecoveryHeart_8049F4F0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    const u32 ignoreIds[2] = {'TOMI', 'NULL'};
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, ignoreIds) == true) {
        return true;
    }
    s32 kind = fn_8022461C(pos, layer);
    s32 floor = fn_80226850(pos, layer);
    if ((u32)(kind - 0x10) <= 7 || (floor >= 0x140 && floor <= 0x15F)) {
        return false;
    }
    if (kind == 0 || kind == 4 || kind == 5 || kind == 6 || kind == 7 || kind == 2 ||
        kind == 3 || kind == 0x50 || kind == 0xE || kind == 0xC || kind == 0x27 ||
        kind == 0x28 || kind == 0x29 || kind == 0x2A || kind == 0x2B || kind == 0x2C ||
        kind == 0x2D || kind == 0x2E || kind == 0x2F || kind == 0x20 || kind == 0xD ||
        kind == 0x39 || kind == 0x3A || kind == 0x3B || kind == 0x4C || kind == 0x4D ||
        kind == 0x4E || kind == 0x4F || kind == 0x24 || kind == 0x23) {
        return false;
    }
    if (kind == 0x25) {
        return fn_802D6A9C()->field_0x171 == 0;
    }
    if (kind == 0x26) {
        return fn_802D6A9C()->field_0x171 == 1;
    }
    if (kind == 0x22) {
        if (fn_802D6A9C()->field_0x170 != 1) {
            return false;
        }
    }
    return true;
}

void RecoveryHeart_8049F4F0::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x2d = 1;
    if (field_0x234 >= 0x100 || (field_0x234 & 2) == 0) {
        fn_801F06F0("enemy_both2_sob.bin", 0x2C, &params, 1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, 0x2C, false, 2, -1, 0, 0, 0);
    }
}

// FIXME: 96.63 %. Landing in state 1 (z <= 0) matches. From the probe in state 3 on, the
// original stores mPos.x and reloads it for the probe at 0x24(r1) (ours keeps it in f0 and
// uses 0x34(r1)), stores probe.y twice (mPos.y, then mPos.y + 4), and its sway add result
// is in f1, not f0; three instructions are missing.
void RecoveryHeart_8049F4F0::update() {
    JGeometry::TVec2f basePos(mPos.x, mPos.y);
    if (field_0x240 == 0) {
        field_0x234 -= 1;
        if (field_0x234 <= 0) {
            mAlive = false;
            return;
        }
    }
    mPos += field_0x3c;

    switch (field_0x230) {
    case 0:
        field_0x3c.z -= 0.1f;
        if (field_0x3c.z < 0.0f) {
            field_0x238 = 0;
            if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x238 = 1;
            }
            field_0x23c = 0;
            field_0x230 = 1;
        }
        break;
    case 1:
        field_0x3c.z -= 0.02f;
        if (field_0x238 == 0) {
            field_0x23c += 0x700;
        } else {
            field_0x23c -= 0x700;
        }
        mPos.x += JMath::sincosTable_.sinShort(field_0x23c);
        if (mPos.z <= 0.0f) {
            field_0x3c.x = 0.0f;
            field_0x3c.z = 0.0f;
            mPos.z = 0.0f;
            field_0x230 = 4;
        }
        break;
    case 2:
        field_0x3c.y += 0.1f;
        if (field_0x3c.y > 0.0f) {
            field_0x238 = 0;
            if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x238 = 1;
            }
            field_0x23c = 0;
            field_0x230 = 3;
        }
        break;
    case 3: {
        field_0x3c.y += 0.02f;
        if (field_0x3c.y > 5.0f) {
            field_0x3c.y = 5.0f;
        }
        if (field_0x238 == 0) {
            field_0x23c += 0x700;
        } else {
            field_0x23c -= 0x700;
        }
        mPos.x += JMath::sincosTable_.sinShort(field_0x23c);
        JGeometry::TVec3f probe(mPos.x, mPos.y + 4.0f, mPos.z);
        if (vfunc_0x4C(field_0x4, &probe) == true) {
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x230 = 4;
        }
        break;
    }
    case 4: {
        JGeometry::TBox2f box(mBox);
        box.addPos(basePos);
        s32 player = fn_8023AF14(&box, field_0x4, lbl_8053AAF8->field_0x7f == 0, 0.0f);
        s32 hitPlayer = fn_8023B8E4(&box, field_0x4, &basePos, true);
        if (hitPlayer < 0) {
            fn_8023B768(&box, field_0x4, &basePos);
        }
        if (player >= 0) {
            s32 owner = fn_80237810(player);
            if (fn_80237774(player) == owner) {
                fn_80230EB8(player, 9);
            }
            fn_80236554(player, 4, 0);
            mAlive = false;
        } else if (hitPlayer >= 0) {
            s32 owner = fn_80237810(hitPlayer);
            if (fn_80237774(hitPlayer) == owner) {
                fn_80230EB8(hitPlayer, 9);
            }
            fn_80236554(hitPlayer, 4, 0);
            mAlive = false;
        } else if (fn_801F2718('ZLD2') >= 0) {
            fn_801F0E34(0x400C9, 0);
            lbl_8053AB9C = 4;
            mAlive = false;
        } else if (fn_801F2718('ZLD4') >= 0) {
            fn_801F0E34(0x400C9, 0);
            lbl_8053ABEC = 4;
            mAlive = false;
        }
        break;
    }
    }

    if (field_0x4 < 8) {
        JGeometry::TVec3f camPos(mPos.x, mPos.y - mPos.z, 0.0f);
        lbl_8053AB10->field_0x24->fn_8030C210(&camPos);
    }
}

void RecoveryHeart_8049F4F0::vfunc_0x40() {
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x230 = 4;
}

// FIXME: 98.31 %. The original stores 0 to field_0x230 with a fresh li r0, 0 in the
// branch; here the zero already in r5 (for field_0x238) is reused. The literals are not
// yet in the original .sdata2 order.
void RecoveryHeart_8049F4F0::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-4.0f, -4.0f, 4.0f, 4.0f);
    field_0x238 = 0;
    field_0x234 = 0x3FF;
    field_0x240 = field_0x90 & 3;
    field_0xb0 = 0x21000;
    if (field_0x240 != 1) {
        field_0x3c.z = 1.5f;
        field_0x230 = 0;
        s32 layer = field_0x4;
        if (viewLayerFlag(layer)) {
            mPos.y = mPos.y - mPos.z;
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.y = -1.5f;
            field_0x230 = 2;
        }
    } else {
        field_0x230 = 4;
    }
}
