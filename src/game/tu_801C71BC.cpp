#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ViewLayer.h"
#include "game/act/act_shadow_link_2.h"

// FIXME(shotgun): 71 %, missing/extra + reloc-name
u8 ShadowLink2_804A43E8::fn_801C976C() {
    u8 start = field_0x358.fn_80214244()->field_0x3;
    field_0x358.fn_80214244()->fn_800FD818(1);
    u8 next = field_0x358.fn_80214244()->field_0x3;
    if (start < field_0x358.fn_80214244()->field_0x2) {
        field_0x358.fn_80214244()->field_0x3 = start;
    }
    field_0x358.fn_80214244()->fn_800FD818(-1);
    u8 prev = field_0x358.fn_80214244()->field_0x3;
    if (start < field_0x358.fn_80214244()->field_0x2) {
        field_0x358.fn_80214244()->field_0x3 = start;
    }
    u8 opt[2];
    opt[1] = prev;
    opt[0] = next;
    u8 pick;
    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
        pick = opt[0];
        s32 layer = field_0x4;
        if (layer != field_0x358.fn_80214244()->fn_800FE25C(pick)) {
            pick = opt[1];
        }
    } else {
        pick = opt[1];
        s32 layer = field_0x4;
        if (layer != field_0x358.fn_80214244()->fn_800FE25C(pick)) {
            pick = opt[0];
        }
    }
    return pick;
}

void ShadowLink2_804A43E8::fn_801C8BC4(u8 flag) {
    if (flag != 0) {
        u8 buf[0x30];
        s32 result = field_0x358.fn_80214244()->fn_800FD894(field_0x4, buf, 0x30);
        if (result <= 3) {
            field_0x55c[1] = 1;
        } else {
            field_0x55c[1] = fn_801CD664(4) + 1;
            if (field_0x524 & 0x100) {
                field_0x55c[1]++;
            }
            u8 cap = result - 2;
            u8 cur = field_0x55c[1];
            if (cap >= cur) {
                cap = cur;
            }
            field_0x55c[1] = cap;
        }
    } else {
        field_0x55c[1] = fn_801CD664(0xC) + 1;
    }
}

// FIXME(shotgun): 99 %, scheduling
void ShadowLink2_804A43E8::fn_801C8B50() {
    mPathCount = 0;
    mFlags &= ~ENEMY_FLAG_NO_MAP_HIT;
    field_0x358.fn_80214244()->fn_800FDE38();
    fn_801C8C84(0, 0);
    field_0x33c = fn_801D02E0(&field_0x32c);
    mFlags &= ~0x20000;
}

// FIXME(shotgun): 89 %, missing/extra
void ShadowLink2_804A43E8::fn_801C7EE4() {
    if (!(mFlags & 0x2000000)) {
        if (field_0x358.fn_80214244()->fn_800FD548(0xFF)) {
            s32 layer = field_0x4;
            if (viewLayerFlag(layer) && __fabs(mPos.z - field_0x32c.z) > 1.0f) {
                mFlags |= 0x400000;
            } else if (fn_801CF888((const JGeometry::TVec2f*)&mPos, &field_0x32c, field_0x4)) {
                mFlags |= 0x400000;
            }
        }
    }
}

void ShadowLink2_804A43E8::fn_801C79A0() {
    JGeometry::TVec3f railPos;

    mFlags &= ~0x02000000;
    fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
    // Copies through set() where the original copies field by field;
    // operator= (the psq pairs) gives other code.
    field_0x32c.set(railPos);
    field_0x56c = field_0x4;
    field_0x570.set(mPos);
    field_0x4 = field_0x358.fn_80214244()->fn_800FE25C(0xFF);
    mPos = field_0x32c;
    mPrevPos = field_0x32c;
    field_0x57c = field_0x4;
    field_0x580.set(mPos);
    if (field_0x358.fn_80214244()->fn_800FD594(0xFF)) {
        mFlags |= 0x20000;
    } else {
        mFlags &= ~0x20000;
    }

    s32 dir = 1;
    if (field_0x358.fn_80214244()->field_0x1 != 0) {
        dir = -1;
    }
    field_0x358.fn_80214244()->fn_800FD818(dir);
    fn_801C8C84(0, 1);
    fn_801C8BC4(1);

    if (field_0x524 & 0x4) {
        field_0x4 = field_0x56c;
        mPos.set(field_0x570);
        if (fn_801CD2F8(field_0x56c)) {
            field_0x58c = 1;
        } else {
            field_0x58c = 0;
        }
        fn_801C3B7C(0x18);
    } else {
        fn_801C3B7C(0x17);
    }
}

// FIXME(shotgun): 67 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::fn_801C71BC() {
    mFlags &= ~0x100000;
    if (!(mFlags & 0x2000000)) {
        bool moved = false;
        u8 start = field_0x358.fn_80214244()->field_0x3;
        field_0x358.fn_80214244()->fn_800FD818(1);
        u8 next = field_0x358.fn_80214244()->field_0x3;
        if (start < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start;
        }
        field_0x358.fn_80214244()->fn_800FD818(-1);
        u8 prev = field_0x358.fn_80214244()->field_0x3;
        if (start < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start;
        }
        u8 opt[3];
        opt[0] = prev;
        opt[1] = start;
        opt[2] = next;
        if (field_0x358.fn_80214244()->fn_800FD4C0(opt[1], opt[0]) ||
            field_0x358.fn_80214244()->fn_800FD4C0(opt[1], opt[2])) {
            JGeometry::TVec3f railPos;
            mPathCount = 0;
            mFlags &= ~(0x1000 | 0x20000);
            mFlags |= 0x400000;
            fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
            field_0x32c.x = railPos.x;
            field_0x32c.y = railPos.y;
            field_0x32c.z = railPos.z;
            moved = true;
        }
        if (moved) {
            return;
        }
    }
    fn_801C8B50();
    field_0x33c += fn_801CD664(0x4001) - 0x2000;
    if (!(field_0x524 & 0x40)) {
        fn_801C3B7C(0x1C);
    }
}
