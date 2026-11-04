#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801AE9C4.h"

cls_804A3448::~cls_804A3448() {}

// TIMG names indexed by field_0x24c. Not const: the table sits in .data.
static const char* lbl_804A33D0[] = {
    "e_tes_aruki_0.bti",       "e_tes_aruki_1.bti",       "e_tes_aruki_2.bti",       "e_tes_aruki_3.bti",
    "e_tes_nobi_0.bti",        "e_tes_nobi_1.bti",        "e_tes_nobi_2.bti",        "e_tes_nobi_3.bti",
    "e_tes_n_aruki_0.bti",     "e_tes_n_aruki_1.bti",     "e_tes_n_aruki_2.bti",     "e_tes_n_aruki_3.bti",
    "e_tes_aruki_tate_0.bti",  "e_tes_aruki_tate_1.bti",  "e_tes_aruki_tate_2.bti",  "e_tes_aruki_tate_3.bti",
    "e_tes_nobi_tate_0.bti",   "e_tes_nobi_tate_1.bti",   "e_tes_nobi_tate_2.bti",   "e_tes_nobi_tate_3.bti",
    "e_tes_n_aruki_tate_0.bti", "e_tes_n_aruki_tate_1.bti", "e_tes_n_aruki_tate_2.bti", "e_tes_n_aruki_tate_3.bti",
    "e_tes_nobi_3.bti",        "e_tes_nobi_2.bti",        "e_tes_nobi_1.bti",
    "e_tes_nobi_tate_3.bti",   "e_tes_nobi_tate_2.bti",   "e_tes_nobi_tate_1.bti",
};

void cls_804A3448::draw() {
    SpriteParams_801F5AE0 params;
    void* res = lbl_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A33D0[field_0x24c]);
    fn_801EFD34(res, &params, -1, field_0x26c, field_0x270, 0.0f);
}

// Actor type ids and spawn arguments by field_0x90 bits 8-11 (0-2).
static u32 lbl_804A33B8[] = {'OKTA', 'PEAC', 'STAL'};
static u32 lbl_804A33C4[] = {0, 0, 2};

// FIXME: 99.90 %. The sdata2 literals differ only by @NNN names (data split).
void cls_804A3448::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;

    if (lbl_8053AAF8->fn_80138A30() <= 1) {
        s32 k = (field_0x90 >> 8) & 0xF;
        if (k > 2) {
            k = 0;
        }
        u32 id;
        u32 arg;
        arg = lbl_804A33C4[k];
        id = lbl_804A33B8[k];
        if (fn_801F856C()->fn_801F9484(id, field_0x4, &mPos, arg, -1, -1) >= 0) {
            mAlive = false;
        }
    } else {
        field_0x274 = 0.0f;
        field_0x270 = 0.0f;
        field_0x26c = 0.0f;
        field_0x280 = 0.0f;
        field_0x27c = 0.0f;
        field_0x278 = 0.0f;
        field_0x254 = 0;
        field_0x258 = -1;
        field_0x25c = 0;
        field_0x250 = field_0x90 & 1;
        if (field_0x250 == 0) {
            field_0x98 = 0;
            fn_801AF7D0(0);
        } else {
            field_0x98 = 2;
            fn_801AF7D0(4);
        }
        field_0x268 = 0.0f;
        field_0x264 = 0.0f;
        field_0x260 = 0.0f;
        field_0x3c.zero();
        field_0xb0 = 0x0A042004;
        field_0x230 = 0;
    }
}

// FIXME: 99.13 %. scheduling+regalloc: the loop's float locals sit in other
// registers than the original's (box i.x in f29 where it has f31, mPos.x in
// f31 where it has f27); the same instructions otherwise. The sdata2
// literals and the jump table differ only by @NNN names (data split).
void cls_804A3448::fn_801AF1EC() {
    JGeometry::TVec2f center;
    s32 result = -1;
    JGeometry::TVec2f pos;
    JGeometry::TBox2f box;
    s32 player = -1;
    for (s32 i = 0; i < 4; i++) {
        if (player < 0) {
            player = fn_801F285C(NULL, NULL, true, i);
        }
    }

    s32 state = field_0x258;
    switch (state) {
    case 0:
    case 4:
        if (state == 0) {
            mBox.set(-16.0f + field_0x26c, -7.0f + field_0x270, 16.0f + field_0x26c, 7.0f + field_0x270);
        } else {
            mBox.set(-7.0f + field_0x26c, -18.0f + field_0x270, 7.0f + field_0x26c, 18.0f + field_0x270);
        }
        field_0x80 = mBox;
        fn_801F2B7C(-100, -1, 1);
        break;
    case 2:
        mBox.set(-6.0f + field_0x26c, -6.0f + field_0x270, 6.0f + field_0x26c, 6.0f + field_0x270);
        if (player >= 0) {
            fn_801EEC98(mSerial, NULL, &result, NULL, NULL, player, 2);
            if (result >= 0 && (result & 2) != 0) {
                if (fn_801F305C()) {
                    fn_801F2618(NULL, false);
                    return;
                }
            }
        }
        // falls through
    case 1:
        mBox.set(-28.0f + field_0x26c, -7.0f + field_0x270, 28.0f + field_0x26c, 7.0f + field_0x270);
        field_0x80 = mBox;
        fn_801F2B7C(-100, -1, 1);
        break;
    case 3:
        break;
    case 6:
        mBox.set(-6.0f + field_0x26c, -6.0f + field_0x270, 6.0f + field_0x26c, 6.0f + field_0x270);
        if (player >= 0) {
            fn_801EEC98(mSerial, NULL, &result, NULL, NULL, player, 2);
            if (result >= 0 && (result & 2) == 0) {
                if (fn_801F305C()) {
                    fn_801F2618(NULL, false);
                    return;
                }
            }
        }
        // falls through
    case 5:
        mBox.set(-7.0f + field_0x26c, -28.0f + field_0x270, 7.0f + field_0x26c, 28.0f + field_0x270);
        field_0x80 = mBox;
        fn_801F2B7C(-100, -1, 1);
        break;
    }

    box.set(mBox);
    pos.set(mPos.x, mPos.y);
    for (s32 i = 0; i < 4; i++) {
        s32 hit = fn_801F285C(NULL, NULL, false, i);
        if (hit >= 0) {
            box.addPos(pos);
            center.x = 0.5f * (box.i.x - box.f.x) + box.f.x;
            center.y = 0.5f * (box.i.y - box.f.y) + box.f.y;
            if (fn_80237948(hit, &center)) {
                fn_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
                fn_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
                fn_801F0E34(0x400D1, 0);
            }
        }
    }
}

void cls_804A3448::fn_801AF678() {
    if (field_0x234 != 0) {
        return;
    }

    field_0x24c = field_0x254;
    field_0x24c += field_0x240;
    field_0x234 = 5;

    switch (field_0x258) {
    case 0:
    case 2:
    case 4:
    case 6:
        field_0x240 += 1;
        field_0x240 &= 3;
        break;
    case 1:
        field_0x240 += 1;
        field_0x240 &= 3;
        if (field_0x240 == 0) {
            fn_801AF7D0(2);
            field_0x25c = 1;
        }
        break;
    case 3:
        field_0x240 += 1;
        if (field_0x240 > 2) {
            fn_801AF7D0(0);
            field_0x25c = 1;
        }
        break;
    case 5:
        field_0x240 += 1;
        field_0x240 &= 3;
        if (field_0x240 == 0) {
            fn_801AF7D0(6);
            field_0x25c = 1;
        }
        break;
    case 7:
        field_0x240 += 1;
        if (field_0x240 > 2) {
            fn_801AF7D0(4);
            field_0x25c = 1;
        }
        break;
    }
}

// FIXME: 83.46 %. The (6, 5) early return is `cmpwi r4,5; bne; blr` in the
// original; ours is `cmpwi r4,5; beqlr`, one instruction fewer. The
// jump table and the sdata2 literals differ only by @NNN names (data split).
void cls_804A3448::fn_801AF7D0(s32 state) {
    if (field_0x258 == state) {
        return;
    }
    if (field_0x258 == 2 && state == 1) {
        return;
    }
    if (field_0x258 == 6 && state == 5) {
        return;
    }

    field_0x258 = state;
    field_0x25c = 0;
    field_0x234 = 0;
    field_0x240 = 0;

    switch (field_0x258) {
    case 0:
        field_0x254 = 0;
        break;
    case 1:
        field_0x254 = 4;
        if (field_0x260 > 0.0f) {
            field_0x278 = 16.0f;
        } else {
            field_0x278 = -16.0f;
        }
        break;
    case 2:
        field_0x254 = 8;
        break;
    case 3:
        field_0x280 = 0.0f;
        field_0x27c = 0.0f;
        field_0x278 = 0.0f;
        field_0x254 = 0x18;
        break;
    case 4:
        field_0x254 = 0xC;
        break;
    case 5:
        field_0x254 = 0x10;
        if (field_0x264 > 0.0f) {
            field_0x27c = 16.0f;
        } else {
            field_0x27c = -16.0f;
        }
        break;
    case 6:
        field_0x254 = 0x14;
        break;
    case 7:
        field_0x280 = 0.0f;
        field_0x27c = 0.0f;
        field_0x278 = 0.0f;
        field_0x254 = 0x1B;
        break;
    }

    field_0x24c = field_0x254;
    field_0x24c += field_0x240;
}
